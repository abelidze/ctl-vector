#pragma once

#include <cmath>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <initializer_list>

#include "allocator.hpp"
#include "iterator.hpp"


namespace ctl {

/**
 * ctl::Vector Definition
 */

template<typename T, class A = Allocator<T>>
class Vector
{
public:
  using value_type = T;
  using allocator_type = A;
  using size_type = typename A::size_type;
  using difference_type = typename A::difference_type;
  using reference = typename A::reference;
  using const_reference = typename A::const_reference;
  using pointer = typename A::pointer;
  using const_pointer = typename A::const_pointer;
  using iterator = ctl::Iterator<T>;
  using const_iterator = ctl::Iterator<const T>;

  Vector() {};
  Vector(const Vector<T>&);
  Vector(size_type);
  Vector(size_type, const_reference);
  Vector(std::initializer_list<T>);
  template<typename IteratorType, class = typename std::enable_if< !std::is_integral<IteratorType>::value >::type>
  Vector(IteratorType, IteratorType);

  virtual ~Vector();

  Vector<T, A>& operator=(const Vector<T, A>&);
  Vector<T, A>& operator=(Vector<T, A>&&);
  Vector<T, A>& operator=(std::initializer_list<T>);

  iterator begin() noexcept;
  iterator end() noexcept;
  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;

  size_type size() const noexcept;
  size_type max_size() const noexcept;
  void resize(size_type);
  void resize(size_type, const_reference);
  size_type capacity() const noexcept;
  bool empty() const noexcept;
  void reserve(size_type);
  void shrink_to_fit();

  reference at(size_type);
  reference operator[](size_type) const;
  reference front();
  reference back();
  pointer data() noexcept;

  void assign(size_type, const_reference);
  void assign(std::initializer_list<T>);

  template<typename IteratorType, class = typename std::enable_if< !std::is_integral<IteratorType>::value >::type>
  void assign(IteratorType, IteratorType);

  void push_back(const_reference);
  void push_back(value_type&&);
  void pop_back();

  iterator insert(iterator, const_reference);
  iterator insert(iterator, size_type, const_reference);
  iterator insert(iterator, std::initializer_list<T>);

  template<typename IteratorType, class = typename std::enable_if< !std::is_integral<IteratorType>::value >::type>
  iterator insert(iterator, IteratorType, IteratorType);

  iterator erase(iterator);
  iterator erase(iterator, iterator);
  void swap(Vector<T, A>&);
  void clear() noexcept;

  template<class... Args>
  iterator emplace(iterator, Args&&...);

  template<class... Args>
  iterator emplace_back(Args&&...);

  A get_allocator() const noexcept;

private:
  const float _growthFactor = 1.5f;

  A _allocator;
  pointer _begin = nullptr;
  pointer _last = nullptr;
  pointer _end = nullptr;

  void reallocate(size_type s);
  void initialize(iterator, iterator);
  void destroy(iterator, iterator);
};


/**
 * ctl::Vector Implementation
 */

template<typename T, typename A>
Vector<T, A>::Vector(const Vector<T>& other)
{
  reallocate(other.size());
  for (size_type i = 0; i < other.size(); ++i) {
    _allocator.construct(_begin + i, other[i]);
  }
}

template<typename T, typename A>
Vector<T, A>::Vector(size_type count)
{
  reallocate(count);
  _last = _begin + count;
  initialize(begin(), end());
}

template<typename T, typename A>
Vector<T, A>::Vector(size_type count, const_reference value)
{
  assign(count, value);
}

template<typename T, typename A>
Vector<T, A>::Vector(std::initializer_list<T> list)
{
  assign(list.begin(), list.end());
}

template<typename T, typename A>
template<typename IteratorType, typename isIterator>
Vector<T, A>::Vector(IteratorType first, IteratorType last)
{
  assign(first, last);
}

template<typename T, typename A>
Vector<T, A>::~Vector()
{
  destroy(begin(), end());
  _allocator.deallocate(_begin, capacity());
}

template<typename T, typename A>
Vector<T, A>& Vector<T, A>::operator=(const Vector<T, A>& other)
{
  if (this == &other) return *this;
  erase(begin(), end());
  if (other.size() > capacity()) {
    reallocate(other.size());
  }
  for (size_type i = 0; i < other.size(); ++i) {
    _allocator.construct(_begin + i, other[i]);
  }
  _last = _begin + other.size();
  return *this;
}

template<typename T, typename A>
Vector<T, A>& Vector<T, A>::operator=(Vector<T, A>&& other)
{
  if (this == &other) return *this;
  clear();
  swap(other);
  return *this;
}

template<typename T, typename A>
Vector<T, A>& Vector<T, A>::operator=(std::initializer_list<T> other)
{
  assign(other.begin(), other.end());
  return *this;
}

template<typename T, typename A>
typename Vector<T, A>::iterator Vector<T, A>::begin() noexcept
{
  return iterator(_begin);
}

template<typename T, typename A>
typename Vector<T, A>::iterator Vector<T, A>::end() noexcept
{
  return iterator(_last);
}

template<typename T, typename A>
typename Vector<T, A>::const_iterator Vector<T, A>::cbegin() const noexcept
{
  return const_iterator(_begin);
}

template<typename T, typename A>
typename Vector<T, A>::const_iterator Vector<T, A>::cend() const noexcept
{
  return const_iterator(_last);
}

template<typename T, typename A>
void Vector<T, A>::assign(size_type n, const_reference value)
{
  erase(begin(), end());
  if (n > capacity()) {
    reallocate(n);
  }
  for (size_type i = 0; i < n; ++i) {
    _allocator.construct(_begin + i, value);
  }
  _last = _begin + n;
}

template<typename T, typename A>
void Vector<T, A>::assign(std::initializer_list<T> il)
{
  assign(il.begin(), il.end());
}

template<typename T, typename A>
void Vector<T, A>::push_back(const_reference value)
{
  if (size() + 1 >= capacity()) {
    reallocate(size() + 1);
  }
  _allocator.construct(_last++, T(value));
}

template<typename T, typename A>
void Vector<T, A>::push_back(value_type&& value)
{
  if (size() + 1 >= capacity()) {
    reallocate(size() + 1);
  }
  std::swap(*(_last++), value);
}

template<typename T, typename A>
void Vector<T, A>::pop_back()
{
  _allocator.destroy(_last);
  --_last;
}

template<typename T, typename A>
typename Vector<T, A>::iterator Vector<T, A>::insert(iterator it, const_reference value)
{
  size_type index = it - begin();
  if (size() + 1 > capacity()) {
    reallocate(size() + 1);
  }
  it = begin() + index;
  std::copy_backward(it, end(), it + size() - index + 1);
  *it = value;
  ++_last;
  return it;
}

template<typename T, typename A>
typename Vector<T, A>::iterator Vector<T, A>::insert(iterator it, size_type count, const_reference value)
{
  size_type newSize = size() + count;
  size_type index = it - begin();
  if (newSize > capacity()) {
    reallocate(newSize);
  }
  it = begin() + index;
  std::copy_backward(it, end(), end() + count);
  std::fill(it, it + count, value);
  _last = _begin + newSize;
  return it;
}

template<typename T, typename A>
typename Vector<T, A>::iterator Vector<T, A>::insert(iterator it, std::initializer_list<T> il)
{
  return insert(it, il.begin(), il.end());
}

template<typename T, typename A>
template<typename IteratorType, typename isIterator>
typename Vector<T, A>::iterator Vector<T, A>::insert(iterator from, IteratorType first, IteratorType last)
{
  difference_type count = std::distance(first, last);
  size_type newSize = size() + count;
  size_type index = from - begin();
  if (newSize > capacity()) {
    reallocate(newSize);
  }
  from = begin() + index;
  std::copy_backward(from, end(), from + count + size() - index);
  std::copy(first, last, from);
  _last = _begin + newSize;
  return from;
}

template<typename T, typename A>
typename Vector<T, A>::iterator Vector<T, A>::erase(iterator it)
{
  _allocator.destroy(&*it);
  std::copy(it + 1, end(), it);
  --_last;
  return it;
}

template<typename T, typename A>
typename Vector<T, A>::iterator Vector<T, A>::erase(iterator first, iterator last)
{
  for (auto it = first; it != last; ++it) {
    _allocator.destroy(&*it);
  }
  if (end() > last + 1) {
    std::copy(last + 1, end(), first + 1);
  }
  _last -= std::distance(first, last);
  return first;
}

template<typename T, typename A>
void Vector<T, A>::swap(Vector<T, A>& other)
{
  std::swap(_begin, other._begin);
  std::swap(_last, other._last);
  std::swap(_end, other._end);
  std::swap(_allocator, other._allocator);
}

template<typename T, typename A>
void Vector<T, A>::clear() noexcept
{
  _allocator.deallocate(_begin, capacity());
  _begin = _last = _end = nullptr;
}

template<typename T, typename A>
typename Vector<T, A>::allocator_type Vector<T, A>::get_allocator() const noexcept
{
  return _allocator;
}

template<typename T, typename A>
void Vector<T, A>::resize(size_type newSize)
{
  size_type index = size();
  if (newSize > capacity()) {
    reallocate(newSize);
  }
  _last = _begin + newSize;
  if (index < size()) {
    initialize(begin() + index, end());
  } else {
    destroy(end(), begin() + index);
  }
}

template<typename T, typename A>
void Vector<T, A>::resize(size_type newSize, const_reference val)
{
  erase(begin(), end());
  assign(newSize, val);
}

template<typename T, typename A>
void Vector<T, A>::reserve(size_type newCapacity)
{
  if (newCapacity > max_size()) {
    throw std::length_error("ctl::Vector: too big capacity to reserve");
  }
  if (newCapacity > capacity()) {
    reallocate(newCapacity);
  }
}

template<typename T, typename A>
void Vector<T, A>::shrink_to_fit()
{
  reallocate( size() );
}

template<typename T, typename A>
inline typename Vector<T, A>::size_type Vector<T, A>::capacity() const noexcept
{
  return _end - _begin;
}

template<typename T, typename A>
inline typename Vector<T, A>::size_type Vector<T, A>::size() const noexcept
{
  return _last - _begin;
}

template<typename T, typename A>
typename Vector<T, A>::size_type Vector<T, A>::max_size() const noexcept
{
  return static_cast<size_type>(-1 / sizeof(T));
}

template<typename T, typename A>
inline bool Vector<T, A>::empty() const noexcept
{
  return size() == 0;
}

template<typename T, typename A>
inline typename Vector<T, A>::reference Vector<T, A>::at(size_type i)
{
  if (i >= size()) {
    throw std::out_of_range("ctl::Vector: out of range");
  }
  return _begin[i];
}

template<typename T, typename A>
inline typename Vector<T, A>::reference Vector<T, A>::operator[](size_type i) const
{
  return _begin[i];
}

template<typename T, typename A>
typename Vector<T, A>::reference Vector<T, A>::front()
{
  return *(begin());
}

template<typename T, typename A>
typename Vector<T, A>::reference Vector<T, A>::back()
{
  return *(--end());
}

template<typename T, typename A>
typename Vector<T, A>::pointer Vector<T, A>::data() noexcept
{
  return _begin;
}

template<typename T, typename A>
void Vector<T, A>::reallocate(size_type newCapacity)
{
  if (newCapacity >= capacity()) {
    newCapacity *= _growthFactor;
  }
  pointer newBegin = _allocator.allocate(newCapacity);
  if (newBegin == _begin) return;

  if (_begin) {
    try {
      std::move(
          _begin,
          _begin + std::min(size(), newCapacity),
          newBegin
        );
    } catch (...) {
      _allocator.deallocate(newBegin, newCapacity);
      throw;
    }
    _allocator.deallocate(_begin, capacity());
  }
  _last = newBegin + size();
  _begin = newBegin;
  _end = newBegin + newCapacity;
}

template<typename T, typename A>
void Vector<T, A>::initialize(iterator first, iterator last)
{
  for (auto it = first; it != last; ++it) {
    _allocator.construct(&*it, value_type());
  }
}

template<typename T, typename A>
void Vector<T, A>::destroy(iterator first, iterator last)
{
  for (auto it = first; it != last; ++it) {
    _allocator.destroy(&*it);
  }
}

template<typename T, typename A>
template<class... Args>
typename Vector<T, A>::iterator Vector<T, A>::emplace(iterator it, Args&&... args)
{
  size_type index = it - begin();
  if (size() + 1 > capacity()) {
    reallocate(size() + 1);
  }
  it = begin() + index;
  std::move_backward(
      it,
      end(),
      it + size() - index + 1
    );
  _allocator.construct(&*it, std::forward<Args>(args)...);
  return it;
}

template<typename T, typename A>
template<class... Args>
typename Vector<T, A>::iterator Vector<T, A>::emplace_back(Args&&... args)
{
  return emplace(end(), std::forward<Args>(args)...);
}

template<typename T, typename A>
template<typename IteratorType, typename isIterator>
void Vector<T, A>::assign(IteratorType first, IteratorType last)
{
  erase(begin(), end());
  typename std::iterator_traits<IteratorType>::difference_type count = std::distance(first, last);
  if (capacity() < count) {
    reallocate(count);
  }
  for (auto it = begin(); first != last; ++it, ++first) {
    _allocator.construct(&*it, value_type(*first));
  }
  _last = _begin + count;
}


} // namespace ctl

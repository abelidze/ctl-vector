#pragma once

#include <iterator>


namespace ctl {

template<typename T>
class Iterator;

} // namespace ctl


template<typename T>
struct std::iterator_traits< ctl::Iterator<T> >
{
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using pointer = T*;
  using reference = T&;
  using iterator_category = std::random_access_iterator_tag;
};

namespace ctl {

/**
 * ctl::Iterator Definition
 */

template<typename T>
class Iterator //: public std::iterator<std::random_access_iterator_tag, T>
{
public:
  using traits = typename std::iterator_traits< ctl::Iterator<T> >;

  using difference_type = typename traits::difference_type;
  using value_type = typename traits::value_type;
  using pointer = typename traits::pointer;
  using reference = typename traits::reference;
  using iterator_category = typename traits::iterator_category;

  Iterator() : ptr(nullptr) {}
  Iterator(const Iterator& other) : ptr(other.ptr) {}
  Iterator(Iterator&& other) : ptr( std::move(other.ptr) ) {}
  Iterator(const typename std::iterator_traits< Iterator<T> >::pointer& ptr) : ptr(ptr) {}
  virtual ~Iterator() = default;

  Iterator& operator=(const Iterator&);
  Iterator& operator++();
  Iterator& operator--();
  Iterator operator++(int);
  Iterator operator--(int);
  Iterator& operator+=(difference_type);
  Iterator& operator-=(difference_type);
  Iterator operator+(difference_type) const;
  Iterator operator-(difference_type) const;
  difference_type operator-(const Iterator&) const;

  reference operator[](difference_type) const;
  reference operator*() const;
  pointer operator->() const;

  bool operator==(const Iterator&) const;
  bool operator!=(const Iterator&) const;
  bool operator<=(const Iterator&) const;
  bool operator>=(const Iterator&) const;
  bool operator<(const Iterator&) const;
  bool operator>(const Iterator&) const;

private:
  pointer ptr;
};

/**
 * ctl::Iterator Implementation
 */

template<typename T>
Iterator<T>& Iterator<T>::operator=(const Iterator& other)
{
  ptr = other.ptr;
  return *this;
}

template<typename T>
Iterator<T>& Iterator<T>::operator++()
{
  ++ptr;
  return *this;
}

template<typename T>
Iterator<T>& Iterator<T>::operator--()
{
  --ptr;
  return *this;
}

template<typename T>
Iterator<T> Iterator<T>::operator++(int)
{
  Iterator foo(*this);
  ++ptr;
  return foo;
}

template<typename T>
Iterator<T> Iterator<T>::operator--(int)
{
  Iterator foo(*this);
  --ptr;
  return foo;
}

template<typename T>
Iterator<T>& Iterator<T>::operator+=(difference_type n)
{
  ptr += n;
  return *this;
}

template<typename T>
Iterator<T>& Iterator<T>::operator-=(difference_type n)
{
  ptr -= n;
  return *this;
}

template<typename T>
typename Iterator<T>::difference_type Iterator<T>::operator-(const Iterator& other) const
{
  return ptr - other.ptr;
}

template<typename T>
Iterator<T> Iterator<T>::operator+(difference_type n) const
{
  return Iterator(ptr + n);
}

template<typename T>
Iterator<T> Iterator<T>::operator-(difference_type n) const
{
  return Iterator(ptr - n);
}

template<typename T>
typename Iterator<T>::reference Iterator<T>::operator[](difference_type i) const
{
  return ptr[i];
}

template<typename T>
typename Iterator<T>::reference Iterator<T>::operator*() const
{
  return *ptr;
}

template<typename T>
typename Iterator<T>::pointer Iterator<T>::operator->() const
{
  return ptr;
}

template<typename T>
bool Iterator<T>::operator==(const Iterator& other) const
{
  return ptr == other.ptr;
}

template<typename T>
bool Iterator<T>::operator!=(const Iterator& other) const
{
  return !(*this == other);
}

template<typename T>
bool Iterator<T>::operator<=(const Iterator& other) const
{
  return ptr <= other.ptr;
}

template<typename T>
bool Iterator<T>::operator>=(const Iterator& other) const
{
  return ptr >= other.ptr;
}

template<typename T>
bool Iterator<T>::operator<(const Iterator& other) const
{
  return ptr < other.ptr;
}

template<typename T>
bool Iterator<T>::operator>(const Iterator& other) const
{
  return ptr > other.ptr;
}

} // namespace ctl

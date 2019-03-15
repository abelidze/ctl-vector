#pragma once

#include <list>
#include <memory>
#include <iostream>


namespace ctl {

#define FIXED_SIZE 100000000

template<typename T>
struct Allocator;

} // namespace ctl


template <typename T>
struct std::allocator_traits< ctl::Allocator<T> >
{
  using allocator_type = typename ctl::Allocator<T>;
  using value_type = T;
  using pointer = value_type*;
  using const_pointer = typename std::pointer_traits<pointer>::template rebind<const value_type>;
  using void_pointer = typename std::pointer_traits<pointer>::template rebind<void>;
  using const_void_pointer = typename std::pointer_traits<pointer>::template rebind<const void>;
  using difference_type = typename std::pointer_traits<pointer>::difference_type;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = typename std::make_unsigned<difference_type>::type;
  using propagate_on_container_copy_assignment = std::false_type;
  using propagate_on_container_move_assignment = std::false_type;
  using propagate_on_container_swap = std::false_type;
  using is_always_equal = std::true_type;

  template<class U> using rebind_alloc = ctl::Allocator<U>;
  template<class U> using rebind_traits = std::allocator_traits< rebind_alloc<U> >;

  static pointer allocate(allocator_type& a, size_type n)
  {
    return a.allocate(n);
  }

  static pointer allocate(allocator_type& a, size_type n, const_void_pointer hint)
  {
    return a.allocate(n);
  }

  static void deallocate(allocator_type &a, pointer p, size_type n)
  {
    a.deallocate(p, n);
  }

  static void destroy(allocator_type& a, pointer p)
  {
    a.destroy(p);
  }

  template<typename U , typename... Args>
  static void construct(allocator_type &a, U* p, Args&&... args)
  {
    a.construct(p, std::forward<Args>(args)...);
  };

  template<class U>
  static void destroy(allocator_type& a, U* p)
  {
    a.destroy(p);
  }

  static size_type max_size(const allocator_type& a)
  {
    return a.max_size();
  }

  static allocator_type select_on_container_copy_construction(const allocator_type& a)
  {
    return allocator_type(a);
  }
};


namespace ctl {

template<typename T>
struct MemoryPool
{
  using size_type = typename Allocator<T>::size_type;
  using pointer = typename Allocator<T>::pointer;

  struct MemoryChunk
  {
    pointer head;
    size_type length;
    bool isFree;

    MemoryChunk(pointer head, size_type length, bool isFree) : head(head), length(length), isFree(isFree) {}
  };

  pointer headPtr;
  std::list<MemoryChunk> chunks;
  const size_type maxSize = FIXED_SIZE;

  MemoryPool();
  virtual ~MemoryPool() = default;
};


template<typename T>
MemoryPool<T>::MemoryPool()
{
  headPtr = static_cast<pointer>(operator new[](sizeof(T) * maxSize) );
  chunks = { MemoryChunk(headPtr, maxSize, true) };
}


template<typename T>
MemoryPool<T>& getPool()
{
  static MemoryPool<T> instance;
  return instance;
}


template<typename T>
struct Allocator
{
public:
  using traits = typename std::allocator_traits< ctl::Allocator<T> >;

  using value_type = typename traits::value_type;
  using pointer = typename traits::pointer;
  using const_pointer = typename traits::const_pointer;
  using reference = typename traits::reference;
  using const_reference = typename traits::const_reference;
  using size_type = typename traits::size_type;
  using difference_type = typename traits::difference_type;
  using is_always_equal = typename traits::is_always_equal;

  Allocator() noexcept {}
  Allocator(const Allocator<T>&) noexcept {}
  template<typename U>
  Allocator(const Allocator<U>&) noexcept {}
  virtual ~Allocator() = default;

  Allocator<T>& operator=(const Allocator<T>&) { return *this; };
  pointer allocate(size_type);
  void deallocate(pointer, size_type);
  size_type max_size() const;
  template<typename... Args>
  void construct(pointer, Args&&...);
  void destroy(pointer);

private:
  MemoryPool<T>& _pool = getPool<T>();
};


template<typename T>
typename Allocator<T>::size_type Allocator<T>::max_size() const
{
  return _pool.maxSize;
}

template<typename T>
template<typename... Args>
void Allocator<T>::construct(pointer p, Args&&... args)
{
  ::new (static_cast<void*>(p)) value_type(std::forward<Args>(args)...);
};

template<typename T>
void Allocator<T>::destroy(pointer p)
{
  p->~value_type();
}

template<typename T>
typename Allocator<T>::pointer Allocator<T>::allocate(size_type n)
{
  for (auto it = _pool.chunks.rbegin(); it != _pool.chunks.rend(); ++it) {
    if (it->isFree && it->length == n) {
      it->isFree = false;
      return it->head;
    } else if (it->isFree && it->length > n) {
      auto res = _pool.chunks.emplace(--it.base(), it->head, n, false);
      it->head += n;
      it->length -= n;
      return res->head;
    }
  }
  throw std::bad_alloc();
}

template<typename T>
void Allocator<T>::deallocate(pointer p, size_type n)
{
  if (p == nullptr) return;

  for (auto it = _pool.chunks.begin(); it != _pool.chunks.end(); ++it) {
    if (it->head != p || it->length != n) continue;

    it->isFree = true;

    for (auto right = std::next(it); right != _pool.chunks.end() && right->isFree;) {
      it->length += right->length;
      right = _pool.chunks.erase(right);
    }

    if (it == _pool.chunks.begin()) continue;

    for (auto left = it; (--left) != _pool.chunks.begin() && left->isFree;) {
      it->head = left->head;
      it->length += left->length;
      left = _pool.chunks.erase(left);
    }
  }
}

template<typename T, typename U>
bool operator==(const Allocator<T>&, const Allocator<U>&)
{
  return true;
}

template<typename T, typename U>
bool operator!=(const Allocator<T>&, const Allocator<U>&)
{
  return false;
}

} // namespace ctl

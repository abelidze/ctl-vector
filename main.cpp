#include <iostream>
#include <vector>

#include "vector.hpp"
// #include "allocator.hpp"
// #include "iterator.hpp"

// using std_v_ctl_a = std::vector<int, ctl::Allocator<int>>;

struct NoCopy
{
  int a = 1;
  NoCopy(int a) : a(a) {}
  NoCopy(const NoCopy& other) = delete; // : a(other.a) {}
  NoCopy& operator=(const NoCopy& other) = delete; //{ a = other.a; return *this; }
  NoCopy& operator=(NoCopy&& other) { a = std::move(other.a); return *this; }
  NoCopy(NoCopy&& other) { *this = std::move(other); }
};


int main(int argc, char const *argv[])
{
  ctl::Vector<NoCopy> arr;
  arr.emplace_back( NoCopy(4) );
  arr.emplace_back( NoCopy(4) );
  arr.emplace_back( NoCopy(4) );
  arr.emplace_back( NoCopy(4) );
  arr.emplace_back( NoCopy(4) );
  arr.emplace_back( NoCopy(4) );
  arr.emplace_back( NoCopy(4) );
  std::cout << arr[0].a << std::endl;



  // std::vector< int, ctl::Allocator<int> > data;
  // data.push_back(13);
  // data.push_back(45);
  // data.pop_back();
  // data.push_back(65);
  // for (auto& x : data) {
  //   std::cout << x << std::endl;
  // }

  // int data[4] = {1, 2, 3, 0};
  // for (ctl::Iterator<int> it(data); *it != 0; ++it) {
  //   std::cout << (*it) << std::endl;
  // }

  // std::iterator_traits< ctl::Iterator<const int> >::value_type x = 2;
  // // x = 3;
  // std::cout << x << std::endl;


  // std::vector<int> x = { 1, 2, 3 };
  // ctl::Iterator<int> it( x.data() );
  // std::reverse_iterator< ctl::Iterator<int> >( std::forward< ctl::Iterator<int> >( it ) );


  // ctl::Vector<int, ctl::Allocator<int>> x;
  // for (size_t i = 0; i < 200; ++i) {
  //   std_v_ctl_a x;
  //   for (int i = 0; i < 100000; ++i) {
  //     x.push_back(i + i);
  //     // std::cout << x.back() << " ";
  //   }
  // }

  // int arr[5] = { 1, 2, 3, 4, 5 };
  // auto it = ctl::Iterator<int>(arr);
  // std::cout << *it << std::endl; // 1
  // it = std::next(it);
  // std::cout << *it << std::endl; // 2
  // std::advance(it, 3);
  // std::cout << *it << std::endl; // 5


  // ctl::Allocator<int> all;
  // auto pp = all.allocate(100);
  // all.construct(pp, 100500);
  // all.destroy(pp);
  // all.deallocate(pp, 100);
  // std::cout << all.max_size() << std::endl;


  std::cout << "OK" << std::endl;
  return 0;
}

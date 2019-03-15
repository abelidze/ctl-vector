#include <vector>
#include <exception>

#include "catch.hpp"
#include "vector.hpp"


TEST_CASE("Vector constructor tests") {

  SECTION("Pass no arguments") {
    ctl::Vector<int> v;

    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);
    REQUIRE(v.empty());
  }

  SECTION("Pass single integer") {
    ctl::Vector<int> v(5);

    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);
  }

  SECTION("Pass single integer and a value") {
    ctl::Vector<int> v(5, 1u);

    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == 1);
    }
  }

  SECTION("Pass two iterators") {
    ctl::Vector<int> src = { 1, 2, 3, 4, 5 };
    ctl::Vector<int> v(src.begin(), src.end());

    REQUIRE(v.size() == src.size());
    REQUIRE(v.capacity() >= src.size());
    for (size_t i = 0; i < src.size(); ++i) {
      REQUIRE(v[i] == src[i]);
    }
  }

  SECTION("Pass initializer list") {
    ctl::Vector<int> v{ 1, 2, 3, 4, 5 };

    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == i + 1);
    }
  }

}


TEST_CASE("Vector: all about data") {

  SECTION("At access") {
    ctl::Vector<int> v = { 1, 2, 3, 4, 5 };
    REQUIRE(v.at(2) == 3);
  }

  SECTION("Front and back access") {
    ctl::Vector<int> v = { 1, 2, 3, 4, 5 };
    REQUIRE(v.front() == 1);
    REQUIRE(v.back() == 5);
  }

  SECTION("Data access") {
    ctl::Vector<int> v = { 1, 2, 3, 4, 5 };
    REQUIRE(*(v.data()) == 1);
  }

  SECTION("Vector size") {
    ctl::Vector<int> v = { 1, 2, 3, 4, 5 };
    REQUIRE(v.max_size() == (size_t)(-1) / (sizeof(int)));
  }

  SECTION("Allocator access") {
    ctl::Vector<int> v = { 1, 2, 3 };
    typename ctl::Vector<int>::allocator_type a = v.get_allocator();
    REQUIRE(a == v.get_allocator());
    REQUIRE_FALSE(a != v.get_allocator());
  }

}


TEST_CASE("Vectors copy operator") {

  SECTION("Assign vector to initializer list") {
    ctl::Vector<int> v;
    v = { 1, 2, 3, 4, 5 };

    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == i + 1);
    }
  }

  SECTION("Assign vector to vector") {
    ctl::Vector<int> src = { 1, 2, 3, 4, 5 };
    ctl::Vector<int> v;
    v = src;

    REQUIRE(src.size() == 5);
    REQUIRE(v.size() == 5);
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == src[i]);
    }
  }

  SECTION("Move assign vector to vector") {
    ctl::Vector<int> src = { 1, 2, 3, 4, 5 };
    ctl::Vector<int> v;
    v = std::move(src);

    REQUIRE(v.size() == 5);
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == i + 1);
    }
  }

  SECTION("Assign vector to the same vector") {
    ctl::Vector<int> v = { 1, 2, 3, 4, 5 };
    v = v;
    v = std::move(v);

    REQUIRE(v.size() == 5);
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == i + 1);
    }
  }

}


TEST_CASE("Vector resizing") {

  SECTION("Pass integer") {
    ctl::Vector<int> v = { 1, 2, 3, 4, 5 };
    v.resize(3);
    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() >= 3);
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == i + 1);
    }
  }

  SECTION("Pass integer and value") {
    ctl::Vector<int> v = { 1, 2, 3, 4, 5 };
    v.resize(3, 1);
    REQUIRE(v.size() == 3);
    REQUIRE(v.capacity() >= 3);
    for (ctl::Vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
      REQUIRE(*it == 1);
    }
  }

}


TEST_CASE("Vector preallocation") {

  SECTION("Pass integer") {
    ctl::Vector<int> v = { 1, 2, 3, 4, 5 };
    v.reserve(20);
    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 20);
    for (size_t i = 0; i < v.size(); ++i) {
      v[i] = i + 1;
    }
  }

}


TEST_CASE("Vector modifications") {

  SECTION("Assign function passing integer and value") {
    ctl::Vector<int> v;
    v.assign(5, 1);
    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);
    for (std::size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == 1);
    }
  }

  SECTION("Assign function passing initializer list") {
    ctl::Vector<int> v;
    v.assign({ 1, 2, 3, 4, 5 });
    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == i + 1);
    }
  }

  SECTION("Assign funtion passing two iterators") {
    ctl::Vector<int> src{ 1, 2, 3, 4, 5 };
    ctl::Vector<int> v;
    v.assign(src.begin(), src.end());
    REQUIRE(v.size() == src.size());
    REQUIRE(v.capacity() >= src.size());
    for (size_t i = 0; i < src.size(); ++i) {
      REQUIRE(v[i] == src[i]);
    }
  }

  SECTION("Push items back") {
    ctl::Vector<int> src = { 6, 3, 8, 2, 21 };
    ctl::Vector<int> v;
    for (ctl::Vector<int>::iterator it = src.begin(); it != src.end(); ++it) {
      v.push_back(*it);
    }
    REQUIRE(v.size() == src.size());
    for (size_t i = 0; i < src.size(); ++i) {
      REQUIRE(src[i] == v[i]);
    }
  }

  SECTION("Move push items back") {
    ctl::Vector<int> src = { 6, 3, 8, 2, 21 };
    ctl::Vector<int> v;
    for (ctl::Vector<int>::iterator it = src.begin(); it != src.end(); ++it) {
      v.push_back(std::move(*it));
    }
    REQUIRE(v.size() == src.size());
  }

  SECTION("Pop items") {
    ctl::Vector<int> v = { 1, 2, 3, 4, 5 };
    for (int i = 0; i < 3; ++i) {
      v.pop_back();
    }
    REQUIRE(v.size() == 2);
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == i + 1);
    }
  }

  SECTION("Insert one item") {
    ctl::Vector<int> v = { 1, 2, 4, 5 };
    v.insert(v.begin() + 2, 3);
    REQUIRE(v.size() == 5);
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == i + 1);
    }
  }

  SECTION("Insert same items") {
    ctl::Vector<int> res = { 1, 2, 3, 3, 3, 4, 5 };
    ctl::Vector<int> v = { 1, 2, 4, 5 };
    v.insert(v.begin() + 2, 3, 3);
    REQUIRE(v.size() == 7);
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == res[i]);
    }
  }

  SECTION("Insert iterator range") {
    ctl::Vector<int> src = { 2, 3, 4 };
    ctl::Vector<int> v = { 1, 5 };
    v.insert(v.begin() + 1, src.begin(), src.end());
    REQUIRE(v.size() == 5);
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == i + 1);
    }
  }

  SECTION("Insert initializer list") {
    ctl::Vector<int> v = { 1, 5 };
    v.insert(v.begin() + 1, { 2, 3, 4 });
    REQUIRE(v.size() == 5);
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == i + 1);
    }
  }

  SECTION("Erase one element") {
    ctl::Vector<int> v = { 1, 2, 3, 3, 4, 5 };
    v.erase(v.begin() + 2);
    REQUIRE(v.size() == 5);
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == i + 1);
    }
  }

  SECTION("Erase elements") {
    ctl::Vector<int> v = { 1, 2, 3, 3, 3, 3, 4, 5 };
    v.erase(v.begin() + 2, v.begin() + 5);
    REQUIRE(v.size() == 5);
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == i + 1);
    }
  }

  SECTION("Swap vectors") {
    ctl::Vector<int> v = { 5, 4, 3, 2, 1 };
    ctl::Vector<int> src = { 1, 2, 3, 4, 5 };
    v.swap(src);
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == i + 1);
    }
    for (size_t i = 0; i < src.size(); ++i) {
      REQUIRE(src[i] == 5 - i);
    }
  }

  SECTION("Clear vector") {
    ctl::Vector<int> v = { 1, 2, 3, 4, 5 };
    v.clear();
    REQUIRE(v.empty());
    REQUIRE(v.capacity() == 0);
    REQUIRE(v.size() == 0);
  }

  SECTION("Shrink vector") {
    ctl::Vector<int> v = { 1, 2, 3, 4, 5, 6 };
    v.shrink_to_fit();
    REQUIRE(v.capacity() == 6);
    REQUIRE(v.size() == 6);
  }

  SECTION("Emplace item back") {
    ctl::Vector< ctl::Vector<int> > v;
    v.emplace_back(4, 1);
    ctl::Vector<int> a = v.front();
    for (ctl::Vector<int>::iterator it = a.begin(); it != a.end(); ++it) {
      REQUIRE(*it == 0);
    }
  }

  SECTION("Emplace item at positiion") {
    ctl::Vector< ctl::Vector<int> > v(3, ctl::Vector<int>(3, 2));
    ctl::Vector<int> a = *(v.emplace(v.begin() + 1, 3, 1));
    for (ctl::Vector<int>::iterator it = a.begin(); it != a.end(); ++it) {
      REQUIRE(*it == 0);
    }
  }

}


TEST_CASE("Vector exceptions") {

  struct Error
  {
    const int a = 1;
    Error(int a = 0) : a(a) {}
    Error(const Error& x) = delete;
    Error(Error&& x) : a(x.a) {}
    Error& operator=(const Error&) { throw std::exception(); }
  };

  struct SomeType
  {
    int a = 4;
    SomeType(int a) : a(a) {}

    static void initVectors()
    {
      ctl::Vector<int> vInt;
      ctl::Vector<char> vChar;
      ctl::Vector<double> vDouble;
      ctl::Vector<SomeType> vCustom;
    }

    static void testMethods()
    {
      ctl::Vector<int> v = { 1, 2, 3, 4 };

      v.max_size();
      v.shrink_to_fit();
      REQUIRE_FALSE( v.empty() );
      REQUIRE( v.size() == 4 );
      REQUIRE( v.capacity() == v.size() );
      REQUIRE( v.at(3) == v[3] );
      REQUIRE( v.at(3) == v[3] );
      REQUIRE( *v.begin() == *v.cbegin() );
      REQUIRE( *v.end() == *v.cend() );
      REQUIRE( *v.data() == v.front() );
      v.insert(v.begin(), 1);
      v.insert(v.begin() + 1, 2, 2);
      v.assign(3, 5);
      v.pop_back();
      v.push_back(6);
      v.erase(v.begin() + 2, v.end());
      REQUIRE( v.size() == 2 );
      v.clear();
      REQUIRE( v.empty() );
      v.get_allocator();
      v.resize(5);
      v.resize(10, 2);
      v.reserve(100);
      REQUIRE( v.capacity() >= 100 );
    }
  };

  SECTION("Working with different types") {
    REQUIRE_NOTHROW( SomeType::initVectors() );
  }

  SECTION("Each method throw no exceptions by default") {
    REQUIRE_NOTHROW( SomeType::testMethods() );
  }

  // SECTION("Exception on copy") {
  //   ctl::Vector<Error> v(1, Error());
  //   REQUIRE_THROWS(v.push_back(Error()));
  // }

  SECTION("Exception without copy constructor") {
    ctl::Vector<Error> v;
    Error err;
    // REQUIRE_THROWS(v.emplace_back( std::move(err) ));
    REQUIRE_NOTHROW(v.emplace_back( 0 ));
  }

  SECTION("Out of range") {
    ctl::Vector<int> v = { 1, 2, 3 };
    REQUIRE_THROWS(v.at(3));
  }

  SECTION("Invalid reserve") {
    ctl::Vector<int> v;
    REQUIRE_THROWS(v.reserve((size_t)(-1)));
  }

  SECTION("Too big allocation size") {
    REQUIRE_THROWS_AS(ctl::Vector<int>(100000000), std::bad_alloc);
  }

}


TEST_CASE("Object destruction with vector") {

  static bool isDestroyed = false;

  struct Destroyable
  {
    ~Destroyable() { isDestroyed = true; }
  };

  SECTION("Vector resize") {
    ctl::Vector<Destroyable> v(5);
    isDestroyed = false;
    v.resize(3);
    REQUIRE(isDestroyed);
  }

  SECTION("Vector erase") {
    ctl::Vector<Destroyable> v(5);
    isDestroyed = false;
    v.erase(v.begin() + 1);
    REQUIRE(isDestroyed);
  }

  SECTION("Vector erase range") {
    ctl::Vector<Destroyable> v(5);
    isDestroyed = false;
    v.erase(v.begin() + 1, v.end());
    REQUIRE(isDestroyed);
  }

}


TEST_CASE("Object without default constructor") {

  struct SuperMegaInt
  {
    int a;
    SuperMegaInt() = delete;
    SuperMegaInt(int a) : a(a) {}
  };

  SECTION("Emplace object") {
    ctl::Vector<SuperMegaInt> v;
    v.reserve(10);
    v.emplace_back(1);
    REQUIRE(v.front().a == 1);
  }

  SECTION("Object pushed back") {
    ctl::Vector<SuperMegaInt> v;
    v.reserve(10);
    v.push_back(SuperMegaInt(1));
    REQUIRE(v.front().a == 1);
  }

}


TEST_CASE("Iterator tests") {

  SECTION("Forward iteration") {
    int i = 1;
    ctl::Vector<int> v = { 1, 2, 3, 4, 5 };
    for (ctl::Vector<int>::iterator it = v.begin(); it != v.end(); ++it, ++i) {
      REQUIRE(*it == i);
    }
  }

  SECTION("Backward iteration") {
    int i = 5;
    ctl::Vector<int> v = { 1, 2, 3, 4, 5 };
    for (ctl::Vector<int>::iterator it = v.end() - 1; it >= v.begin(); --it, --i) {
      REQUIRE(*it == i);
    }
  }

  SECTION("Random access") {
    ctl::Vector<int> v = { 1, 2, 3, 4, 5 };
    ctl::Vector<int*> a;
    int b = 8;
    a.push_back(&b);
    REQUIRE(*(v.begin() + 2) == 3);
    REQUIRE(*(v.end() - 2) == 4);
    REQUIRE(v.begin()[2] == 3);
    ctl::Vector<int>::iterator it = v.begin();
    it += 2;
    REQUIRE(*it == 3);
    it -= 2;
    REQUIRE(*it == 1);
    REQUIRE(**(a.begin().operator->()) == 8);
    it++;
    REQUIRE(*it == 2);
    it--;
    REQUIRE(*it == 1);
  }

  SECTION("Comparision") {
    ctl::Vector<int> v = { 1, 2, 3, 4, 5 };
    ctl::Vector<int>::iterator first = v.begin();
    ctl::Vector<int>::iterator last = v.end();
    REQUIRE(first < last);
    REQUIRE(first == first);
    REQUIRE(last > first);
    REQUIRE(last >= last);
    REQUIRE(last >= first);
    REQUIRE(first <= last);
    REQUIRE(first <= first);
    REQUIRE(first != last);
  }

}


TEST_CASE("STL compatibility") {

  SECTION("Allocator") {
    std::vector<int, ctl::Allocator<int>> v = { 1, 2, 3, 4, 5 };
    v.push_back(4);
    v.push_back(3);
    v.push_back(2);
    v.push_back(1);
    for (size_t i = 0; i < v.size(); ++i) {
      v[i] += i + 1;
    }
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE( v[i] == ( i < 5 ? (i + 1) * 2 : 10 ) );
    }
  }

  SECTION("Iterator") {
    ctl::Vector<int> src = { 1, 2, 3, 4, 5 };
    std::vector<int> v(src.begin(), src.end());
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == i + 1);
    }

    std::vector<int> x = { 1, 2, 3 };
    ctl::Iterator<int> it( x.data() );
    auto rit = std::reverse_iterator< ctl::Iterator<int> >( std::forward< ctl::Iterator<int> >( it ) );
    REQUIRE(*(--rit) == *it);
  }

  SECTION("std::sort") {
    ctl::Vector<int> v;
    for (int i = 1245; i >= 0; --i) {
      v.push_back(i);
    }
    std::sort(v.begin(), v.end());
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == i);
    }
  }

}

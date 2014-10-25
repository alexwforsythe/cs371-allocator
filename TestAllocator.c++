// ------------------------------------
// projects/allocator/TestAllocator.c++
// Copyright (C) 2014
// Glenn P. Downing
// ------------------------------------

// --------
// includes
// --------

#include <algorithm> // count
#include <memory>    // allocator

#include "gtest/gtest.h"

#include "Allocator.h"

// -------------
// TestAllocator
// -------------

template <typename A>
struct TestAllocator : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          A                  allocator_type;
    typedef typename A::value_type      value_type;
    typedef typename A::difference_type difference_type;
    typedef typename A::pointer         pointer;};

typedef testing::Types<
            std::allocator<int>,
            std::allocator<double>,
            Allocator<int, 100>,
            Allocator<double, 100> >
        my_types;

TYPED_TEST_CASE(TestAllocator, my_types);

TYPED_TEST(TestAllocator, One) {
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::difference_type difference_type;
    typedef typename TestFixture::pointer         pointer;

    allocator_type x;
    const difference_type s = 1;
    const value_type      v = 2;
    const pointer         p = x.allocate(s);
    if (p != 0) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}}

TYPED_TEST(TestAllocator, Ten) {
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::difference_type difference_type;
    typedef typename TestFixture::pointer         pointer;

    allocator_type x;
    const difference_type s = 10;
    const value_type      v = 2;
    const pointer         b = x.allocate(s);
    if (b != 0) {
        pointer e = b + s;
        pointer p = b;
        try {
            while (p != e) {
                x.construct(p, v);
                ++p;}}
        catch (...) {
            while (b != p) {
                --p;
                x.destroy(p);}
            x.deallocate(b, s);
            throw;}
        ASSERT_EQ(s, std::count(b, e, v));
        while (b != e) {
            --e;
            x.destroy(e);}
        x.deallocate(b, s);}}


TYPED_TEST(TestAllocator, TEST_TEST) {
    typedef typename TestFixture::allocator_type  allocator_type;
    typedef typename TestFixture::value_type      value_type;
    typedef typename TestFixture::difference_type difference_type;
    typedef typename TestFixture::pointer         pointer;

    allocator_type x;
    const difference_type s = 1;
    const value_type      v = 2;
    const pointer         p = x.allocate(s);
    if (p != 0) {
        x.construct(p, v);
        ASSERT_EQ(v, *p);
        x.destroy(p);
        x.deallocate(p, s);}

    const difference_type s2 = 10;
    const pointer p2 = x.allocate(s2);
    if (p2 != 0) {
        x.construct(p2, v);
        ASSERT_EQ(v, *p2);
        x.destroy(p2);
        x.deallocate(p2, s2);
    }
}

// constructor
TEST(TestMyAllocator, constructor_char) {
    bool caught = false;
    try {
        Allocator<char, 100> x;
    } catch (...) {
        caught = true;
    }
    ASSERT_FALSE(caught);
}

TEST(TestMyAllocator, constructor_double) {
    bool caught = false;
    try {
        Allocator<double, 100> x;
    } catch (...) {
        caught = true;
    }
    ASSERT_FALSE(caught);
}

TEST(TestMyAllocator, constructor_int) {
    bool caught = false;
    try {
        Allocator<int, 100> x;
    } catch (...) {
        caught = true;
    }
    ASSERT_FALSE(caught);
}

TEST(TestMyAllocator, constructor_long) {
    bool caught = false;
    try {
        Allocator<long, 100> x;
    } catch (...) {
        caught = true;
    }
    ASSERT_FALSE(caught);
}

// bad constructor
TEST(TestMyAllocator, constructor_exception_char) {
    bool caught = false;
    try {
        Allocator<char, 8> x;
    } catch (...) {
        caught = true;
    }
    ASSERT_TRUE(caught);
}

TEST(TestMyAllocator, constructor_exception_double) {
    bool caught = false;
    try {
        Allocator<double, 15> x;
    } catch (...) {
        caught = true;
    }
    ASSERT_TRUE(caught);
}

TEST(TestMyAllocator, constructor_exception_int) {
    bool caught = false;
    try {
        Allocator<int, 11> x;
    } catch (...) {
        caught = true;
    }
    ASSERT_TRUE(caught);
}

TEST(TestMyAllocator, constructor_exception_long) {
    bool caught = false;
    try {
        Allocator<long, 15> x;
    } catch (...) {
        caught = true;
    }
    ASSERT_TRUE(caught);
}

// allocate
TEST(TestMyAllocator, allocate_char) {
    bool caught = false;
    try {
        Allocator<char, 100> x;
        Allocator<char, 100>::pointer p = x.allocate(1);
    } catch (...) {
        caught = true;
    }
    ASSERT_FALSE(caught);
}

TEST(TestMyAllocator, allocate_double) {
    bool caught = false;
    try {
        Allocator<double, 100> x;
        Allocator<double, 100>::pointer p = x.allocate(1);
    } catch (...) {
        caught = true;
    }
    ASSERT_FALSE(caught);
}

TEST(TestMyAllocator, allocate_int) {
    bool caught = false;
    try {
        Allocator<int, 100> x;
        Allocator<int, 100>::pointer p = x.allocate(1);
    } catch (...) {
        caught = true;
    }
    ASSERT_FALSE(caught);
}

TEST(TestMyAllocator, allocate_long) {
    bool caught = false;
    try {
        Allocator<long, 100> x;
        Allocator<long, 100>::pointer p = x.allocate(1);
    } catch (...) {
        caught = true;
    }
    ASSERT_FALSE(caught);
}

// allocate no space
TEST(TestMyAllocator, allocate_none_char) {
    bool caught = false;
    try {
        Allocator<char, 100> x;
        Allocator<char, 100>::pointer p = x.allocate(0);
    } catch (...) {
        caught = true;
    }
    ASSERT_TRUE(caught);
}

TEST(TestMyAllocator, allocate_none_double) {
    bool caught = false;
    try {
        Allocator<double, 100> x;
        Allocator<double, 100>::pointer p = x.allocate(0);
    } catch (...) {
        caught = true;
    }
    ASSERT_TRUE(caught);
}

TEST(TestMyAllocator, allocate_none_int) {
    bool caught = false;
    try {
        Allocator<int, 100> x;
        Allocator<int, 100>::pointer p = x.allocate(0);
    } catch (...) {
        caught = true;
    }
    ASSERT_TRUE(caught);
}

TEST(TestMyAllocator, allocate_none_long) {
    bool caught = false;
    try {
        Allocator<long, 100> x;
        Allocator<long, 100>::pointer p = x.allocate(0);
    } catch (...) {
        caught = true;
    }
    ASSERT_TRUE(caught);
}

// allocate too much space
TEST(TestMyAllocator, allocate_much_char) {
    bool caught = false;
    try {
        Allocator<char, 100> x;
        Allocator<char, 100>::pointer p = x.allocate(93);
    } catch (...) {
        caught = true;
    }
    ASSERT_TRUE(caught);
}

TEST(TestMyAllocator, allocate_much_double) {
    bool caught = false;
    try {
        Allocator<double, 100> x;
        Allocator<double, 100>::pointer p = x.allocate(12);
    } catch (...) {
        caught = true;
    }
    ASSERT_TRUE(caught);
}

TEST(TestMyAllocator, allocate_much_int) {
    bool caught = false;
    try {
        Allocator<int, 100> x;
        Allocator<int, 100>::pointer p = x.allocate(24);
    } catch (...) {
        caught = true;
    }
    ASSERT_TRUE(caught);
}

TEST(TestMyAllocator, allocate_much_long) {
    bool caught = false;
    try {
        Allocator<long, 100> x;
        Allocator<long, 100>::pointer p = x.allocate(12);
    } catch (...) {
        caught = true;
    }
    ASSERT_TRUE(caught);
}

// deallocate
TEST(TestMyAllocator, deallocate_char) {
    bool caught = false;
    try {
        Allocator<char, 100> x;
        Allocator<char, 100>::pointer p = x.allocate(1);
        x.deallocate(p, 1);
    } catch (...) {
        caught = true;
    }
    ASSERT_FALSE(caught);
}

TEST(TestMyAllocator, deallocate_double) {
    bool caught = false;
    try {
        Allocator<double, 100> x;
        Allocator<double, 100>::pointer p = x.allocate(1);
        x.deallocate(p, 1);
    } catch (...) {
        caught = true;
    }
    ASSERT_FALSE(caught);
}

TEST(TestMyAllocator, deallocate_int) {
    bool caught = false;
    try {
        Allocator<int, 100> x;
        Allocator<int, 100>::pointer p = x.allocate(1);
        x.deallocate(p, 1);
    } catch (...) {
        caught = true;
    }
    ASSERT_FALSE(caught);
}

TEST(TestMyAllocator, deallocate_long) {
    bool caught = false;
    try {
        Allocator<long, 100> x;
        Allocator<long, 100>::pointer p = x.allocate(1);
        x.deallocate(p, 1);
    } catch (...) {
        caught = true;
    }
    ASSERT_FALSE(caught);
}

// deallocate first and view free space
TEST(TestMyAllocator, deallocate_first_char) {
    bool caught = false;
    Allocator<char, 100> x;
    const Allocator<char, 100>& xr = x;
    try {
        Allocator<char, 100>::pointer p1 = x.allocate(1);
        Allocator<char, 100>::pointer p2 = x.allocate(1);
        x.deallocate(p1, 1);
    } catch (...) {
        caught = true;
    }
    ASSERT_FALSE(caught);
    ASSERT_EQ(xr.view(0), 1);
}

TEST(TestMyAllocator, deallocate_first_double) {
    bool caught = false;
    Allocator<double, 100> x;
    const Allocator<double, 100>& xr = x;
    try {
        Allocator<double, 100>::pointer p1 = x.allocate(1);
        Allocator<double, 100>::pointer p2 = x.allocate(1);
        x.deallocate(p1, 1);
    } catch (...) {
        caught = true;
    }
    ASSERT_FALSE(caught);
    ASSERT_EQ(xr.view(0), 8);
}

TEST(TestMyAllocator, deallocate_first_int) {
    bool caught = false;
    Allocator<int, 100> x;
    const Allocator<int, 100>& xr = x;
    try {
        Allocator<int, 100>::pointer p1 = x.allocate(1);
        Allocator<int, 100>::pointer p2 = x.allocate(1);
        x.deallocate(p1, 1);
    } catch (...) {
        caught = true;
    }
    ASSERT_FALSE(caught);
    ASSERT_EQ(xr.view(0), 4);
}

TEST(TestMyAllocator, deallocate_first_long) {
    bool caught = false;
    Allocator<long, 100> x;
    const Allocator<long, 100>& xr = x;
    try {
        Allocator<long, 100>::pointer p1 = x.allocate(1);
        Allocator<long, 100>::pointer p2 = x.allocate(1);
        x.deallocate(p1, 1);
    } catch (...) {
        caught = true;
    }
    ASSERT_FALSE(caught);
    ASSERT_EQ(xr.view(0), 8);
}

// deallocate last and view free space
TEST(TestMyAllocator, deallocate_last_char) {
    bool caught = false;
    Allocator<char, 100> x;
    const Allocator<char, 100>& xr = x;
    try {
        Allocator<char, 100>::pointer p1 = x.allocate(1);
        Allocator<char, 100>::pointer p2 = x.allocate(1);
        x.deallocate(p2, 1);
    } catch (...) {
        caught = true;
    }
    ASSERT_FALSE(caught);
    ASSERT_EQ(xr.view(96), 83);
}

TEST(TestMyAllocator, deallocate_last_double) {
    bool caught = false;
    Allocator<double, 100> x;
    const Allocator<double, 100>& xr = x;
    try {
        Allocator<double, 100>::pointer p1 = x.allocate(1);
        Allocator<double, 100>::pointer p2 = x.allocate(1);
        x.deallocate(p2, 1);
    } catch (...) {
        caught = true;
    }
    ASSERT_FALSE(caught);
    ASSERT_EQ(xr.view(96), 76);
}

TEST(TestMyAllocator, deallocate_last_int) {
    bool caught = false;
    Allocator<int, 100> x;
    const Allocator<int, 100>& xr = x;
    try {
        Allocator<int, 100>::pointer p1 = x.allocate(1);
        Allocator<int, 100>::pointer p2 = x.allocate(1);
        x.deallocate(p2, 1);
    } catch (...) {
        caught = true;
    }
    ASSERT_FALSE(caught);
    ASSERT_EQ(xr.view(96), 80);
}

TEST(TestMyAllocator, deallocate_last_long) {
    bool caught = false;
    Allocator<long, 100> x;
    const Allocator<long, 100>& xr = x;
    try {
        Allocator<long, 100>::pointer p1 = x.allocate(1);
        Allocator<long, 100>::pointer p2 = x.allocate(1);
        x.deallocate(p2, 1);
    } catch (...) {
        caught = true;
    }
    ASSERT_FALSE(caught);
    ASSERT_EQ(xr.view(96), 76);
}

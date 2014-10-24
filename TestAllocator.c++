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

using namespace std;

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

/************************************
 *      Start of My Unit Test       *
 *  The following test Allocator.h  *
 ************************************
 */

// Section One, Regular constructor

TEST(ConstructorType, Char){
    try{
        Allocator<char, 100> al;
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_TRUE(true);
}

TEST(ConstructorType, Int){
    try{
        Allocator<int, 100> al;
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_TRUE(true);
}

TEST(ConstructorType, Double){
    try{
        Allocator<double, 100> al;
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_TRUE(true);
}

TEST(ConstructorType, Long){
    try{
        Allocator<long, 100> al;
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_TRUE(true);
}

// Section TWo, BadConstructor

TEST(BadConstructor, Char){
    bool caught_it = false;
    try{
        Allocator<char, 1> al;
    }catch(...){
        caught_it = true;
    }
    ASSERT_TRUE(caught_it);
}

TEST(BadConstructor, Int){
    bool caught_it = false;
    try{
        Allocator<int, 1> al;
    }catch(...){
        caught_it = true;
    }
    ASSERT_TRUE(caught_it);
}

TEST(BadConstructor, Double){
    bool caught_it = false;
    try{
        Allocator<double, 1> al;
    }catch(...){
        caught_it = true;
    }
    ASSERT_TRUE(caught_it);
}

TEST(BadConstructor, Long){
    bool caught_it = false;
    try{
        Allocator<long, 1> al;
    }catch(...){
        caught_it = true;
    }
    ASSERT_TRUE(caught_it);
}

// Section Three: Test the allocate() function

TEST(AllocateGood, Char){
    try{
        Allocator<char, 100> al;
        Allocator<char, 100>::pointer p = al.allocate(15);
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_TRUE(true);

}

TEST(AllocateGood, Int){
    try{
        Allocator<int, 100> al;
        Allocator<int, 100>::pointer p = al.allocate(15);
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_TRUE(true);

}

TEST(AllocateGood, Double){
    try{
        Allocator<double, 100> al;
        Allocator<double, 100>::pointer p = al.allocate(3);
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_TRUE(true);

}

TEST(AllocateGood, Long){
    try{
        Allocator<long, 100> al;
        Allocator<long, 100>::pointer p = al.allocate(3);
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_TRUE(true);

}

// Section Four: bad_alloc in allocate

//4.a allocate cannot be 0
TEST(AllocateBadAlloc, Char){
    bool err = false;
    try{
        Allocator<char, 100> al;
        Allocator<char, 100>::pointer p = al.allocate(0);
    }catch(...){
        err = true;
    }
    ASSERT_TRUE(err);
}

TEST(AllocateBadAlloc, Int){
    bool err = false;
    try{
        Allocator<int, 100> al;
        Allocator<int, 100>::pointer p = al.allocate(0);
    }catch(...){
        err = true;
    }
    ASSERT_TRUE(err);
}

TEST(AllocateBadAlloc, Double){
    bool err = false;
    try{
        Allocator<double, 100> al;
        Allocator<double, 100>::pointer p = al.allocate(0);
    }catch(...){
        err = true;
    }
    ASSERT_TRUE(err);
}

//4.b allocate cannot be bigger than space already allocated 

TEST(AllocateBadAlloc, Char2){
    bool err = false;
    try{
        Allocator<char, 100> al;
        Allocator<char, 100>::pointer p = al.allocate(101);
    }catch(...){
        err = true;
    }
    ASSERT_TRUE(err);
}

TEST(AllocateBadAlloc, Int2){
    bool err = false;
    try{
        Allocator<int, 100> al;
        Allocator<int, 100>::pointer p = al.allocate(25);
    }catch(...){
        err = true;
    }
    ASSERT_TRUE(err);
}

TEST(AllocateBadAlloc, Double2){
    bool err = false;
    try{
        Allocator<double, 100> al;
        Allocator<double, 100>::pointer p = al.allocate(13);
    }catch(...){
        err = true;
    }
    ASSERT_TRUE(err);
}

//4.c Test smallest alloc size allowed
TEST(AllocateBadAlloc, Char3){
    bool err = true;
    try{
        Allocator<char, 100> al;
        Allocator<char, 100>::pointer p = al.allocate(1);
    }catch(...){
        err = false;
    }
    ASSERT_TRUE(err);
}

TEST(AllocateBadAlloc, Int3){
    bool err = true;
    try{
        Allocator<int, 100> al;
        Allocator<int, 100>::pointer p = al.allocate(1);
    }catch(...){
        err = false;
    }
    ASSERT_TRUE(err);
}

TEST(AllocateBadAlloc, Double3){
    bool err = true;
    try{
        Allocator<double, 100> al;
        Allocator<double, 100>::pointer p = al.allocate(1);
    }catch(...){
        err = false;
    }
    ASSERT_TRUE(err);
}

// Section 5: Extra alloc tests like calling it twice, add extra in future

TEST(AllocExtra, Char){
    bool err = true;
    try{
        Allocator<char, 100> al;
        Allocator<char, 100>::pointer p = al.allocate(5);
        Allocator<char, 100>::pointer p2 = al.allocate(20);
    }catch(...){
        err = false;
    }
    ASSERT_TRUE(err);
}

TEST(AllocExtra, Int){
    bool err = true;
    try{
        Allocator<int, 100> al;
        Allocator<int, 100>::pointer p = al.allocate(5);
        Allocator<int, 100>::pointer p2 = al.allocate(20);
    }catch(...){
        err = false;
    }
    ASSERT_TRUE(err);
}

TEST(AllocExtra, Double){
    bool err = true;
    try{
        Allocator<double, 100> al;
        Allocator<double, 100>::pointer p = al.allocate(3);
        Allocator<double, 100>::pointer p2 = al.allocate(5);
    }catch(...){
        err = false;
    }
    ASSERT_TRUE(err);
}

//Section 6: deallocate() tests

//6.a Deallocate working properly

TEST(DeallocateGood, Char){
    try{
        Allocator<char, 100> al;
        Allocator<char, 100>::pointer p = al.allocate(15);
        al.deallocate(p, 15);
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_TRUE(true);

}

TEST(DeallocateGood, Int){
    try{
        Allocator<int, 100> al;
        Allocator<int, 100>::pointer p = al.allocate(15);
        al.deallocate(p, 15);
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_TRUE(true);

}

TEST(DeallocateGood, Double){
    try{
        Allocator<double, 100> al;
        Allocator<double, 100>::pointer p = al.allocate(3);
        al.deallocate(p, 3);
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_TRUE(true);
}

//6.b deallocate something thats the first thing. 
TEST(DeallocateFront, Char){
    Allocator<char, 100> al;
    const Allocator<char, 100>& cal = al;
    try{
        Allocator<char, 100>::pointer p = al.allocate(15);
        Allocator<char, 100>::pointer p2 = al.allocate(5);
        al.deallocate(p, 15);
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_EQ(cal.view(0), 15);

}

TEST(DeallocateFront, Int){
    Allocator<int, 100> al;
    const Allocator<int, 100>& cal = al;
    try{
        Allocator<int, 100>::pointer p = al.allocate(15);
        Allocator<int, 100>::pointer p2 = al.allocate(5);
        al.deallocate(p, 15);
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_EQ(cal.view(0), 60);

}

TEST(DeallocateFront, Double){
    Allocator<double, 100> al;
    const Allocator<double, 100>& cal = al;
    try{
        Allocator<double, 100>::pointer p = al.allocate(5);
        Allocator<double, 100>::pointer p2 = al.allocate(3);
        al.deallocate(p, 5);
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_EQ(cal.view(0), 40);
}

TEST(DeallocateFront, Char2){
    Allocator<char, 100> al;
    const Allocator<char, 100>& cal = al;
    try{
        Allocator<char, 100>::pointer p = al.allocate(56);
        Allocator<char, 100>::pointer p2 = al.allocate(10);
        Allocator<char, 100>::pointer p3 = al.allocate(10);
        al.deallocate(p2,10);
        al.deallocate(p, 56);
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_EQ(cal.view(0), 74);

}

//6.c deallocate something thats in the end
TEST(DeallocateBack, Char){
    Allocator<char, 100> al;
    const Allocator<char, 100>& cal = al;
    try{
        Allocator<char, 100>::pointer p = al.allocate(74);
        Allocator<char, 100>::pointer p2 = al.allocate(10);
        al.deallocate(p2, 10);
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_EQ(cal.view(96), 10);

}

TEST(DeallocateBack, Int){
    Allocator<int, 100> al;
    const Allocator<int, 100>& cal = al;
    try{
        Allocator<int, 100>::pointer p = al.allocate(11);
        Allocator<int, 100>::pointer p2 = al.allocate(10);
        al.deallocate(p2, 10);
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_EQ(cal.view(96), 40);

}

TEST(DeallocateBack, Char2){
    Allocator<char, 100> al;
    const Allocator<char, 100>& cal = al;
    try{
        Allocator<char, 100>::pointer p = al.allocate(56);
        Allocator<char, 100>::pointer p2 = al.allocate(10);
        Allocator<char, 100>::pointer p3 = al.allocate(10);
        al.deallocate(p2, 10);  
        al.deallocate(p3, 10);
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_EQ(cal.view(96), 28);
}

//6.d deallocate something in the middle

TEST(DeallocateMiddle, Char){
    Allocator<char, 100> al;
    const Allocator<char, 100>& cal = al;
    try{
        Allocator<char, 100>::pointer p = al.allocate(56);
        Allocator<char, 100>::pointer p2 = al.allocate(10);
        Allocator<char, 100>::pointer p3 = al.allocate(10);
        al.deallocate(p2, 10);
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_EQ(cal.view(64), 10);
}

TEST(DeallocateMiddle, Char2){
    Allocator<char, 100> al;
    const Allocator<char, 100>& cal = al;
    try{
        Allocator<char, 100>::pointer p = al.allocate(10);
        Allocator<char, 100>::pointer p2 = al.allocate(10);
        Allocator<char, 100>::pointer p3 = al.allocate(10);
        al.deallocate(p, 10);
        al.deallocate(p2, 10);
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_EQ(cal.view(0), 28);
}

TEST(DeallocateMiddle, Char3){
    Allocator<char, 100> al;
    const Allocator<char, 100>& cal = al;
    try{
        Allocator<char, 100>::pointer p  = al.allocate(10);
        Allocator<char, 100>::pointer p2 = al.allocate(10);
        Allocator<char, 100>::pointer p3 = al.allocate(10);
        Allocator<char, 100>::pointer p4 = al.allocate(10);
        al.deallocate(p3, 10);
        al.deallocate(p2, 10);
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_EQ(cal.view(18), 28);
}

TEST(DeallocateMiddle, Char4){
    Allocator<char, 100> al;
    const Allocator<char, 100>& cal = al;
    try{
        Allocator<char, 100>::pointer p  = al.allocate(10);
        Allocator<char, 100>::pointer p2 = al.allocate(10);
        Allocator<char, 100>::pointer p3 = al.allocate(10);
        Allocator<char, 100>::pointer p4 = al.allocate(10);
        al.deallocate(p, 10);
        al.deallocate(p3, 10);
        al.deallocate(p2, 10);
    }catch(...){
        ASSERT_TRUE(false);
    }
    ASSERT_EQ(cal.view(0), 46);
}
// Copyright 2018 Your Name <your_email>


#include <gtest/gtest.h>
#include "header.hpp"

TEST(TestConstructor, AllTypes) {
    SharedPtr<int> sp0;
    ASSERT_EQ(0, sp0.use_count());
    int* pointer = new int;
    SharedPtr<int> sp1(pointer);
    ASSERT_EQ(1, sp1.use_count());
    SharedPtr<int> sp2(sp1);
    ASSERT_EQ(2, sp2.use_count());
    SharedPtr<int> sp3(std::move(sp1));
    ASSERT_EQ(2, sp3.use_count());
    ASSERT_EQ(0, sp1.use_count());
}

TEST(OperatorsTest, Lvalue_Copy){
  int* pointer = new int;
  *pointer = 88;
  SharedPtr<int> sp1(pointer);
  SharedPtr<int> sp2(new int);
  *sp2 = 15;
  sp1 = sp2;
  //ASSERT_EQ(88, *pointer);
  ASSERT_EQ(15, *sp1);
  ASSERT_EQ(2, sp2.use_count());
}

TEST(OperatorsTest, Rvalue_Copy){
  int* pointer = new int;
  *pointer = 15;
  SharedPtr<int> ps1(pointer);
  SharedPtr<int> ps2(new int);
  *ps2 = 88;
  ps1 = std::move(ps2);
  ASSERT_EQ(88, *ps1);
  ASSERT_EQ(nullptr, ps2.operator->());
  ASSERT_EQ(0, ps2.use_count());
  ASSERT_EQ(1, ps1.use_count());
  SharedPtr<int> sp3;
  ps1 = sp3;
  ASSERT_EQ(0, ps1.use_count());
}

TEST(ResetTest, WithoutPtr){
  int*  pointer = new int;
  SharedPtr<int> sp1(pointer);
  ASSERT_EQ(true, sp1.operator bool());
  ASSERT_EQ(pointer, sp1.get());
  SharedPtr<int> sp2(sp1);
  SharedPtr<int>sp3(sp2);
  sp1.reset();
  ASSERT_EQ(2, sp2.use_count());
  ASSERT_EQ(0, sp1.use_count());
  ASSERT_EQ(nullptr, sp1.operator->());
}

TEST(ResetTest, WithPtr){
  int* pointer = new int;
  SharedPtr<int> sp1(pointer);
  *sp1 = 15;
  SharedPtr<int> sp2(sp1);
  SharedPtr<int> sp3(sp1);
  sp1.reset(new int);
  ASSERT_EQ(2, sp2.use_count());
  ASSERT_EQ(1, sp1.use_count());
  ASSERT_NE(sp1.get(), sp3.get());
}

TEST(Swap, swap){
  SharedPtr<int> sp1(new int);
  *sp1 = 15;
  SharedPtr<int> sp2(new int);
  *sp2 = 88;
  sp1.swap(sp2);
  SharedPtr<int> sp3(sp1);
  ASSERT_EQ(15, *sp2);
  ASSERT_EQ(88, *sp1);
  ASSERT_EQ(1, sp2.use_count());
  ASSERT_EQ(2, sp1.use_count());
}

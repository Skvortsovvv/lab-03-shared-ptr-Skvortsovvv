// Copyright 2020 Skvortsov <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
#pragma once

#include <atomic>
#include <utility>
template <typename T>
class SharedPtr;
template <typename T>
class Block {
 private:
  std::atomic_uint reference_counter = 0;
  T* b_pointer;

 public:
  explicit Block(T* ptr) : reference_counter(1), b_pointer(ptr) {}
  ~Block(){
    reference_counter = 0;
    if (b_pointer != nullptr) {
      delete b_pointer;
      b_pointer = nullptr;
    }
  }
  friend class SharedPtr<T>;
};

template <typename T>
class SharedPtr {
 private:
  Block<T>* block_pointer;

 public:
  SharedPtr(): block_pointer(nullptr) {}
  explicit SharedPtr(T* ptr) : block_pointer(new Block<T>(ptr)) {}
  SharedPtr(const SharedPtr& r) {
      block_pointer = r.block_pointer;
      block_pointer->reference_counter++;
  }
  SharedPtr(SharedPtr&& r) {
      block_pointer = r.block_pointer;
      r.block_pointer = nullptr;
  }
  ~SharedPtr() {
    reset();
  }
  SharedPtr<T>& operator=(SharedPtr& r) {
    reset();
    block_pointer = r.block_pointer;
    if (block_pointer != nullptr)
      block_pointer->reference_counter++;
    return *this;
  }

  SharedPtr<T>& operator=(SharedPtr&& r) {
    reset();
    block_pointer = r.block_pointer;
    r.block_pointer = nullptr;
    return *this;
  }

  operator bool() const {
      return (block_pointer->b_pointer != nullptr);
    }

  T& operator*() const { return *block_pointer->b_pointer; }
  T* operator->() const {
    if (block_pointer != nullptr) {
      return block_pointer->b_pointer;
    }else{
      return nullptr;
    }
  }

  T* get() {
    if (block_pointer != nullptr) {
      return block_pointer->b_pointer;
    }else{
      return nullptr;
    }
  }

  void reset() {
    if (block_pointer != nullptr){
      block_pointer->reference_counter--;
      if (use_count() == 0){
        delete block_pointer;
      }
      block_pointer = nullptr;
    }
  }

  void reset(T* ptr) {
    reset();
    block_pointer = new Block<T>(ptr);
  }
  void swap(SharedPtr& r) {
      auto temp = r;
      r = *this;
      *this = temp;
  }
  std::size_t use_count() const {
    if (block_pointer == nullptr) {
      return 0;
    } else {
      return block_pointer->reference_counter.load();
    }
  }
};
#endif // INCLUDE_HEADER_HPP_

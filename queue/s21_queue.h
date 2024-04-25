#pragma once

#include "s21_list.h"

namespace s21 {

template <typename T, typename sequence_ = s21::List<T> >
class queue {
 public:
  using value_type = typename sequence_::value_type;
  using reference = typename sequence_::reference;
  using const_reference = typename sequence_::const_reference;
  using size_type = typename sequence_::size_type;

  queue() = default;
  queue(std::initializer_list<value_type> const& items) {
    for (auto& el : items) {
      c.push_back(el);
    }
  }
  queue(const queue& q) : c(q.c) {}
  queue(queue&& q) noexcept : c(std::move(q.c)) {}
  ~queue() = default;

  queue& operator=(const queue& q) {
    c = q.c;
    return *this;
  }
  queue& operator=(queue&& q) noexcept {
    c = std::move(q.c);
    return *this;
  }

  const_reference front() { return c.front(); }
  const_reference back() { return c.back(); }

  bool empty() const noexcept { return c.empty(); }
  size_type size() const noexcept { return c.size(); }

  void push(const_reference value) { c.push_back(value); }

  template <typename... Args>
  void insert_many(Args... args) {
    c.insert_many_back(std::forward<Args>(args)...);
  }

  void pop() { c.pop_front(); }
  void swap(queue& other) { c.swap(other.c); }

 private:
  sequence_ c;
};

}  // namespace s21

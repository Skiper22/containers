#include "s21_list.h"

namespace s21 {

template <typename T, typename sequence_ = s21::List<T> >
class stack {
 public:
  using value_type = typename sequence_::value_type;
  using reference = typename sequence_::reference;
  using const_reference = typename sequence_::const_reference;
  using size_type = typename sequence_::size_type;

  stack() = default;
  stack(std::initializer_list<value_type> const& items) {
    for (auto& el : items) {
      c.push_front(el);
    }
  }
  stack(const stack& s) : c(s.c) {}
  stack(stack&& s) noexcept : c(std::move(s.c)) {}
  ~stack() = default;

  stack& operator=(const stack& s) {
    c = s.c;
    return *this;
  }
  stack& operator=(stack&& s) noexcept {
    c = std::move(s.c);
    return *this;
  }

  const_reference top() { return c.front(); }

  bool empty() const noexcept { return c.empty(); }
  size_type size() const noexcept { return c.size(); }

  void push(const_reference value) { c.push_front(value); }

  template <typename... Args>
  void insert_many(Args&&... args) {
    c.push_front(std::forward<Args>(args)...);
  }

  void pop() { c.pop_front(); }
  void swap(stack& other) { c.swap(other.c); }

 private:
  sequence_ c;
};

}  // namespace s21

#include "s21_vector.h"

namespace s21 {

template <typename T, size_t N>
class Array {
 public:
  using sequence = s21::Vector<T>;
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = typename sequence::iterator;
  using const_iterator = typename sequence::const_iterator;
  using size_type = size_t;

  Array() : s(N) {}
  explicit Array(size_type size, const_reference value = {}) : s(size, value) {}
  Array(std::initializer_list<value_type> const& items) : s(items) {}
  Array(const Array& other) = default;
  Array(Array&& other) = default;
  ~Array() = default;

  Array& operator=(const Array& other) = default;
  Array& operator=(Array&& other) = default;

  reference at(size_type pos) { return s.at(pos); }
  const_reference at(size_type pos) const { return s.at(pos); }
  reference operator[](size_type pos) { return s[pos]; }
  const_reference front() { return s.front(); }
  const_reference back() { return s.back(); }

  iterator begin() { return s.begin(); }
  iterator end() { return s.end(); }
  const_iterator begin() const { return s.begin(); }
  const_iterator end() const { return s.end(); }
  const_iterator cbegin() const { return s.cbegin(); }
  const_iterator cend() const { return s.cend(); }

  bool empty() const noexcept { return s.empty(); }
  size_type size() const noexcept { return s.size(); }
  void swap(Array& other) { s.swap(other.s); }
  void fill(const_reference value) {
    for (size_t i = 0; i < size(); ++i) {
      s[i] = value;
    }
  }

 private:
  sequence s;
};

}  // namespace s21

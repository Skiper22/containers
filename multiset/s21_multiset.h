#include "hash_table.h"

namespace s21 {

template <typename K, typename H = std::hash<K>>
class Multiset {
 public:
  using table = hash_table<K, K, H>;
  using key_type = K;
  using mapped_type = K;
  using value_type = std::pair<key_type, mapped_type>;
  using reference = value_type&;
  using iterator = typename table::iterator;
  using size_type = size_t;

  Multiset() = default;

  Multiset(std::initializer_list<mapped_type> const& items) {
    for (auto& it : items) {
      t[it] = it;
    }
  }

  Multiset(const Multiset& other) = default;
  Multiset(Multiset&& other) = default;
  ~Multiset() noexcept = default;

  Multiset& operator=(const Multiset& other) = default;
  Multiset& operator=(Multiset&& other) noexcept = default;

  iterator begin() { return t.begin(); }
  iterator end() { return t.end(); }

  bool empty() const noexcept { return t.empty(); }
  size_type size() const noexcept { return t.size(); }

  void clear() { t.clear(); }
  std::pair<iterator, bool> insert(const mapped_type& value) {
    value_type p = {value, value};
    return t.insert(p);
  }
  void erase(iterator pos) { t.erase(pos); }
  void swap(Multiset& other) { t.swap(other.t); }

  iterator find(const key_type& key) { return t.find(key); }
  bool contains(const key_type& key) const noexcept { return t.contains(key); }
  template <typename... Args>
  s21::Vector<std::pair<iterator, bool>> insert_many(Args&&... args) {
    return {insert(std::forward<Args>(args))...};
  }

 private:
  table t;
};

}  // namespace s21

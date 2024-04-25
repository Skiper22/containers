#pragma once

#include "hash_table.h"

namespace s21 {

template <typename K, typename V, typename H = std::hash<K>>
class Map {
 public:
  using table = s21::hash_table<K, V, H>;
  using key_type = K;
  using mapped_type = V;
  using value_type = std::pair<key_type, mapped_type>;
  using reference = value_type&;
  using iterator = typename table::iterator;
  using size_type = size_t;

  Map() = default;

  Map(std::initializer_list<value_type> const& items) {
    for (auto& it : items) {
      t[it.first] = it.second;
    }
  }

  Map(const Map& other) = default;
  Map(Map&& other) noexcept = default;
  ~Map() noexcept = default;

  Map& operator=(const Map& other) = default;
  Map& operator=(Map&& other) noexcept = default;

  size_type size() const noexcept { return t.size(); }
  bool empty() const noexcept { return t.empty(); }
  void clear() { return t.clear(); }

  iterator begin() { return t.begin(); }
  iterator end() { return t.end(); }

  mapped_type& at(const key_type& key) { return t.at(key); }
  mapped_type& operator[](const key_type& key) { return t[key]; }

  void erase(iterator pos) { t.erase(pos); }
  void swap(Map& other) { t.swap(other.t); }

  std::pair<iterator, bool> insert(const value_type& value) {
    return t.insert(value);
  }
  std::pair<iterator, bool> insert(const key_type& key,
                                   const mapped_type& value) {
    return t.insert(key, value);
  }
  std::pair<iterator, bool> insert_or_assign(const key_type& key,
                                             const mapped_type& value) {
    return t.insert_or_assign(key, value);
  }
  template <typename... Args>
  s21::Vector<std::pair<iterator, bool>> insert_many(Args... args) {
    return t.insert_many(std::forward<Args>(args)...);
  }

  bool contains(const key_type& key) const noexcept { return t.contains(key); }

 private:
  table t;
};

}  // namespace s21

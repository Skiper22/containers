#pragma once

#include "../list/s21_list.h"
#include "../vector/s21_vector.h"
#include "hash_iterator.h"

namespace s21 {

template <typename K, typename V, typename H = std::hash<K>>
class hash_table {
 public:
  using key_type = K;
  using mapped_type = V;
  using value_type = std::pair<key_type, mapped_type>;
  using bucket = List<value_type>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = hash_iterator<key_type, mapped_type>;
  using const_iterator = const_hash_iterator<key_type, mapped_type>;
  using size_type = size_t;

  hash_table() : table_(defualt_capacity) {}
  hash_table(const hash_table& other) = default;
  hash_table(hash_table&& other) = default;
  ~hash_table() = default;

  hash_table<K, V>& operator=(const hash_table& other) = default;
  hash_table<K, V>& operator=(hash_table&& other) = default;

  size_type size() const noexcept;
  size_type capacity() const noexcept;
  bool empty() const noexcept;
  void clear();

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;
  const_iterator cbegin() const;
  const_iterator cend() const;

  mapped_type& at(const key_type& key);
  mapped_type& operator[](const key_type& key);

  void erase(iterator pos);
  void swap(hash_table& other);
  void assign(value_type& value);

  template <typename... Args>
  s21::Vector<std::pair<iterator, bool>> insert_many(Args&&... args);
  std::pair<iterator, bool> insert(const value_type& value);
  std::pair<iterator, bool> insert(const key_type& key,
                                   const mapped_type& value);
  std::pair<iterator, bool> insert_or_assign(const key_type& key,
                                             const mapped_type& value);

  iterator find(const key_type& key);
  bool contains(const key_type& key) const noexcept;

 protected:
  int compute_hash(const key_type& key) const noexcept {
    return hash_function(key);
  }
  void resize() { table_.reserve(table_.capacity()); }
  bool exceeds_limit(double load_limit = 0.7) {
    load_factor = (double)size() / capacity();
    return std::abs(load_factor - load_limit) >
           std::numeric_limits<double>::epsilon();
  }

 private:
  int hash_function(const key_type& key) const noexcept {
    return H()(key) % table_.capacity();
  }

  constexpr static int defualt_capacity = 10;
  size_type size_{};
  double load_factor{};
  Vector<bucket> table_;
};

template <typename K, typename V, typename H>
typename hash_table<K, V, H>::size_type hash_table<K, V, H>::size()
    const noexcept {
  return size_;
}

template <typename K, typename V, typename H>
typename hash_table<K, V, H>::size_type hash_table<K, V, H>::capacity()
    const noexcept {
  return table_.capacity();
}

template <typename K, typename V, typename H>
bool hash_table<K, V, H>::empty() const noexcept {
  return !size();
}

template <typename K, typename V, typename H>
void hash_table<K, V, H>::clear() {
  table_.clear();
}

template <typename K, typename V, typename H>
bool hash_table<K, V, H>::contains(const key_type& key) const noexcept {
  int hash = compute_hash(key);
  auto& bucket = table_[hash];

  for (auto& it : bucket) {
    if (it.first == key) {
      return true;
    }
  }

  return false;
}

template <typename K, typename V, typename H>
typename hash_table<K, V, H>::iterator hash_table<K, V, H>::find(
    const key_type& key) {
  int hash = compute_hash(key);
  auto& bucket = table_[hash];

  for (auto it = bucket.begin(); it != bucket.end(); ++it) {
    if (it->first == key) {
      return iterator{table_.begin() + hash, table_.end(), it};
    }
  }

  return end();
}

template <typename K, typename V, typename H>
typename hash_table<K, V, H>::iterator hash_table<K, V, H>::begin() {
  for (auto it = table_.begin(); it != table_.end(); ++it) {
    if (!it->empty()) {
      return iterator{it, table_.end(), it->begin()};
    }
  }

  return end();
}

template <typename K, typename V, typename H>
typename hash_table<K, V, H>::iterator hash_table<K, V, H>::end() {
  auto it = table_.end() - 1;
  for (; it >= table_.begin(); --it) {
    if (!it->empty()) {
      return iterator{table_.end(), table_.end(), it->end()};
    }
  }

  return iterator{table_.end(), table_.end(), it->end()};
}

template <typename K, typename V, typename H>
typename hash_table<K, V, H>::const_iterator hash_table<K, V, H>::begin()
    const {
  for (auto it = table_.begin(); it != table_.end(); ++it) {
    if (!it->empty()) {
      return const_iterator{it, table_.end(), it->begin()};
    }
  }

  return end();
}

template <typename K, typename V, typename H>
typename hash_table<K, V, H>::const_iterator hash_table<K, V, H>::end() const {
  auto it = table_.end() - 1;
  for (; it >= table_.begin(); --it) {
    if (!it->empty()) {
      return const_iterator{table_.end(), table_.end(), it->end()};
    }
  }

  return const_iterator{table_.end(), table_.end(), it->end()};
}

template <typename K, typename V, typename H>
typename hash_table<K, V, H>::const_iterator hash_table<K, V, H>::cbegin()
    const {
  for (auto it = table_.begin(); it != table_.end(); ++it) {
    if (!it->empty()) {
      return const_iterator{it, table_.end(), it->begin()};
    }
  }

  return end();
}

template <typename K, typename V, typename H>
typename hash_table<K, V, H>::const_iterator hash_table<K, V, H>::cend() const {
  auto it = table_.end() - 1;
  for (; it >= table_.begin(); --it) {
    if (!it->empty()) {
      return const_iterator{table_.end(), table_.end(), it->end()};
    }
  }

  return const_iterator{table_.end(), table_.end(), it->end()};
}

template <typename K, typename V, typename H>
void hash_table<K, V, H>::assign(value_type& value) {
  int hash = compute_hash(value.first);
  auto& bucket = table_[hash];

  for (auto& it : bucket) {
    if (it.first == value.first) {
      it.second = value.second;
      return;
    }
  }
}

template <typename K, typename V, typename H>
template <typename... Args>
s21::Vector<std::pair<typename hash_table<K, V, H>::iterator, bool>>
hash_table<K, V, H>::insert_many(Args&&... args) {
  return {insert(std::forward<Args>(args))...};
}

template <typename K, typename V, typename H>
std::pair<typename hash_table<K, V, H>::iterator, bool>
hash_table<K, V, H>::insert(const value_type& value) {
  if (exceeds_limit()) {
    resize();
  }

  int hash = compute_hash(value.first);
  auto& bucket = table_[hash];
  for (auto it = bucket.begin(); it != bucket.end(); ++it) {
    if (it->first == value.first) {
      return std::make_pair(iterator(table_.begin() + hash, table_.end(), it),
                            false);
    }
  }

  bucket.push_back(value);
  ++size_;

  return std::make_pair(
      iterator(table_.begin() + hash, table_.end(), bucket.end()), true);
}

template <typename K, typename V, typename H>
std::pair<typename hash_table<K, V, H>::iterator, bool>
hash_table<K, V, H>::insert(const key_type& key, const mapped_type& value) {
  std::pair<iterator, bool> it = insert(std::make_pair(key, value));
  return it;
}

template <typename K, typename V, typename H>
std::pair<typename hash_table<K, V, H>::iterator, bool>
hash_table<K, V, H>::insert_or_assign(const key_type& key,
                                      const mapped_type& value) {
  value_type p = std::make_pair(key, value);
  std::pair<iterator, bool> it = insert(p);

  if (!it.second) {
    assign(p);
  }

  return it;
}

template <typename K, typename V, typename H>
typename hash_table<K, V, H>::mapped_type& hash_table<K, V, H>::operator[](
    const key_type& key) {
  int hash = compute_hash(key);
  auto& bucket = table_[hash];

  for (auto& it : bucket) {
    if (it.first == key) {
      return it.second;
    }
  }
  if (exceeds_limit()) {
    resize();
  }
  bucket.push_back(std::make_pair(key, mapped_type{}));
  ++size_;

  return bucket.end()->second;
}

template <typename K, typename V, typename H>
typename hash_table<K, V, H>::mapped_type& hash_table<K, V, H>::at(
    const key_type& key) {
  if (!contains(key)) {
    throw std::out_of_range("Error: key doesn't exist");
  }

  return (*this)[key];
}

template <typename K, typename V, typename H>
void hash_table<K, V, H>::swap(hash_table& other) {
  table_.swap(other.table_);
}

template <typename K, typename V, typename H>
void hash_table<K, V, H>::erase(iterator pos) {
  int hash = pos->first;
  auto& bucket = table_[hash];

  typename bucket::iterator b = pos.get_bucket_it();
  bucket.erase(b);
  --size_;
}

}  // namespace s21

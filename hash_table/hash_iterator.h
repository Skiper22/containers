#pragma once

#include "../list/s21_list.h"
#include "../vector/s21_vector.h"

namespace s21 {

template <typename, typename, typename>
class hash_table;

template <typename K, typename V>
class base_hash_iterator {
 public:
  friend class hash_table<K, V, std::hash<K>>;
  using key_type = K;
  using mapped_type = std::remove_const_t<V>;
  using value_type = std::pair<key_type, mapped_type>;
  using reference = value_type&;
  using pointer = value_type*;
  using iterator_category = std::forward_iterator_tag;
  using bucket = List<value_type>;
  using table_it = typename Vector<bucket>::iterator;
  using bucket_it = typename bucket::iterator;

  base_hash_iterator(const base_hash_iterator& other) = default;
  base_hash_iterator(base_hash_iterator&& other) noexcept = default;

  base_hash_iterator& operator=(const base_hash_iterator& other) = default;
  base_hash_iterator& operator=(base_hash_iterator&& other) = default;

  reference operator*() { return *b_; }
  pointer operator->() { return &(*b_); }

  bucket_it get_bucket_it() { return b_; }

 protected:
  base_hash_iterator(table_it begin, table_it end, bucket_it b)
      : begin_(begin), end_(end), b_(b) {}

  void advance() {
    if (begin_ == end_) {
      throw std::out_of_range("Error: attempt to access beyond map");
    }

    if (b_ != begin_->end()) {
      ++b_;
      return;
    }
    ++begin_;
    while (begin_ != end_) {
      if (!(begin_->empty())) {
        b_ = begin_->begin();
        return;
      }
      ++begin_;
    }
  }

  bool equals(const base_hash_iterator& other) const {
    if (begin_ == other.begin_ && end_ == other.end_) return true;

    return b_ == other.b_;
  }

  table_it begin_;
  table_it end_;
  bucket_it b_;
};

template <typename K, typename V>
class hash_iterator : public base_hash_iterator<K, V> {
 public:
  friend class hash_table<K, V, std::hash<K>>;
  using base = base_hash_iterator<K, V>;
  using key_type = typename base::key_type;
  using mapped_type = typename base::mapped_type;
  using value_type = typename base::value_type;
  using reference = typename base::reference;
  using pointer = typename base::pointer;
  using iterator_category = std::forward_iterator_tag;

  hash_iterator(const hash_iterator& other) = default;
  hash_iterator(hash_iterator&& other) noexcept = default;

  hash_iterator& operator=(const hash_iterator& other) = default;
  hash_iterator& operator=(hash_iterator&& other) = default;

  hash_iterator operator++() {
    this->advance();

    return *this;
  }
  hash_iterator operator++(int) {
    auto tmp{*this};
    this->advance();

    return tmp;
  }

  friend bool operator==(const hash_iterator& a, const hash_iterator& b) {
    return a.equals(b);
  }
  friend bool operator!=(const hash_iterator& a, const hash_iterator b) {
    return !(a == b);
  }

 protected:
  using base::base;
};

template <typename K, typename V>
class const_hash_iterator : public base_hash_iterator<K, const V> {
 public:
  friend class hash_table<K, V, std::hash<K>>;
  using base = base_hash_iterator<K, const V>;
  using key_type = typename base::key_type;
  using mapped_type = typename base::mapped_type;
  using value_type = typename base::value_type;
  using reference = typename base::reference;
  using pointer = typename base::pointer;
  using iterator_category = std::forward_iterator_tag;

  const_hash_iterator(const const_hash_iterator& other) = default;
  const_hash_iterator(const_hash_iterator&& other) noexcept = default;

  const_hash_iterator& operator=(const const_hash_iterator& other) = default;
  const_hash_iterator& operator=(const_hash_iterator&& other) = default;

  const_hash_iterator operator++() {
    this->advance();

    return *this;
  }
  const_hash_iterator operator++(int) {
    auto tmp{*this};
    this->advance();

    return tmp;
  }

  friend bool operator==(const const_hash_iterator& a,
                         const const_hash_iterator& b) {
    return a.equals(b);
  }
  friend bool operator!=(const const_hash_iterator& a,
                         const const_hash_iterator b) {
    return !(a == b);
  }

 protected:
  using base::base;
};

}  // namespace s21

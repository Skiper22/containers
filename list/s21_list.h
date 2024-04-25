#pragma once

#include <limits>

#include "s21_list_iterator.h"
#include "s21_list_node.h"

namespace s21 {

template <typename T>
class List {
  using node = ListNode<T>;
  using node_ptr = std::shared_ptr<node>;

 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;
  using iterator = ListIterator<T>;
  using const_iterator = ConstListIterator<T>;

  List() = default;
  explicit List(size_type n, const_reference value = value_type{});
  List(std::initializer_list<value_type> const& items);
  List(const List& other) = default;
  List(List&& other) noexcept = default;
  ~List() noexcept = default;

  List<T>& operator=(const List& other) = default;
  List<T>& operator=(List&& other) noexcept = default;

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;
  const_iterator cbegin() const;
  const_iterator cend() const;

  const_reference front();
  const_reference back();

  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const noexcept;

  void assign(iterator first, iterator last);
  void clear() noexcept;

  iterator insert(const_iterator pos, const_reference value);
  template <typename... Args>
  iterator insert_many(const_iterator pos, Args&&... args);
  template <typename... Args>
  void insert_many_back(Args&&... args);
  template <typename... Args>
  void insert_many_front(Args&&... args);

  void erase(iterator pos);
  void push_back(const_reference value);
  void pop_back();
  void push_front(const_reference value);
  void pop_front();
  void swap(List<T>& other);
  void merge(List<T>& other);
  void splice(const_iterator pos, List<T>& other);
  void reverse();
  void unique();
  void sort();

 private:
  node_ptr head{};
  node_ptr tail{};
  size_type size_{};
};

template <typename T>
List<T>::List(size_type n, const_reference value) {
  while (n--) {
    push_back(value);
  }
}

template <typename T>
typename List<T>::size_type List<T>::size() const noexcept {
  return size_;
}

template <typename T>
bool List<T>::empty() const noexcept {
  return !size();
}

template <typename T>
typename List<T>::size_type List<T>::max_size() const noexcept {
  return std::numeric_limits<size_type>::max();
}

template <typename T>
void List<T>::clear() noexcept {
  head = tail = nullptr;
  size_ = 0;
}

template <typename T>
void List<T>::push_back(const_reference value) {
  insert_many_back(value);
}

template <typename T>
template <typename... Args>
void List<T>::insert_many_back(Args&&... args) {
  node_ptr ptr{};

  try {
    ptr = std::make_shared<node>(std::forward<Args>(args)...);
  } catch (std::bad_alloc& e) {
    throw std::runtime_error("Error: failed to allocate memory");
  }

  if (!head) {
    head = tail = ptr;
  } else {
    ptr->set_prev(tail);
    tail->set_next(ptr);
    tail = ptr;
  }

  size_ += sizeof...(args);
}

template <typename T>
void List<T>::pop_back() {
  if (empty()) {
    throw std::runtime_error("Error: List is empty");
  }

  auto prev = tail->prev();
  prev->set_next(nullptr);
  tail = prev;

  --size_;
}

template <typename T>
void List<T>::pop_front() {
  if (empty()) {
    throw std::runtime_error("Error: List is empty");
  }

  head = head->next();
  if (head) {
    head->set_prev(nullptr);
  } else {
    tail = nullptr;
  }

  --size_;
}

template <typename T>
void List<T>::push_front(const_reference value) {
  insert_many_front(value);
}

template <typename T>
template <typename... Args>
void List<T>::insert_many_front(Args&&... args) {
  node_ptr ptr{};

  try {
    ptr = std::make_shared<node>(std::forward<Args>(args)...);
  } catch (std::bad_alloc& e) {
    throw std::runtime_error("Error: failed to allocate memory");
  }

  ptr->set_next(head);
  if (head) {
    head->set_prev(ptr);
  }
  head = ptr;

  if (!tail) {
    tail = ptr;
  }

  size_ += sizeof...(args);
}

template <typename T>
List<T>::List(std::initializer_list<value_type> const& items) {
  for (auto& el : items) {
    push_back(el);
  }
}

template <typename T>
typename List<T>::iterator List<T>::begin() {
  return iterator{head};
}

template <typename T>
typename List<T>::const_iterator List<T>::begin() const {
  return const_iterator{head};
}

template <typename T>
typename List<T>::const_iterator List<T>::end() const {
  return const_iterator{tail, true};
}

template <typename T>
typename List<T>::iterator List<T>::end() {
  return iterator{tail, true};
}

template <typename T>
typename List<T>::const_iterator List<T>::cbegin() const {
  return const_iterator{head};
}

template <typename T>
typename List<T>::const_iterator List<T>::cend() const {
  return const_iterator{tail, true};
}

template <typename T>
typename List<T>::const_reference List<T>::front() {
  return head->get_data();
}

template <typename T>
typename List<T>::const_reference List<T>::back() {
  return tail->get_data();
}

template <typename T>
void List<T>::assign(iterator first, iterator last) {
  clear();
  for (; first != last; ++first) {
    push_back(*first);
  }
}

template <typename T>
typename List<T>::iterator List<T>::insert(const_iterator pos,
                                           const_reference value) {
  return insert_many(pos, value);
}

template <typename T>
template <typename... Args>
typename List<T>::iterator List<T>::insert_many(const_iterator pos,
                                                Args&&... args) {
  if (pos == cbegin()) {
    insert_many_front(T(std::forward<Args>(args)...));
    return begin();
  }

  if (pos == cend()) {
    insert_many_back(std::forward<Args>(args)...);
    return end();
  }

  node_ptr new_node{};
  try {
    new_node = std::make_shared<node>(T(std::forward<Args>(args)...));
  } catch (std::bad_alloc& e) {
    throw std::runtime_error("Error: Failed to allocate memory");
  }

  node_ptr current = pos.get_ptr();
  node_ptr prev = current->prev();

  prev->set_next(new_node);
  new_node->set_prev(prev);
  new_node->set_next(current);
  current->set_prev(new_node);

  ++size_;

  return iterator(new_node);
}

template <typename T>
void List<T>::erase(iterator pos) {
  if (pos == begin()) {
    pop_front();
    return;
  }
  if (pos == end()) {
    pop_back();
    return;
  }

  node_ptr current = pos.get_ptr();
  node_ptr prev = current->prev();
  node_ptr next = current->next();

  prev->set_next(next);
  next->set_prev(prev);

  --size_;
}

template <typename T>
void List<T>::swap(List<T>& other) {
  std::swap(head, other.head);
  std::swap(tail, other.tail);
  std::swap(size_, other.size_);
}

template <typename T>
void List<T>::sort() {
  if (empty()) {
    return;
  }
  bool swapped;
  do {
    swapped = false;
    node_ptr ptr = head;

    while (ptr->next()) {
      auto a = ptr->get_data();
      auto b = ptr->next()->get_data();
      if (a > b) {
        ptr->set_data(b);
        ptr->next()->set_data(a);
        swapped = true;
      }
      ptr = ptr->next();
    }
  } while (swapped);
}

template <typename T>
void List<T>::merge(List<T>& other) {
  if (this == &other) {
    return;
  }
  if (empty()) {
    assign(other.begin(), other.end());
  } else {
    for (auto& el : other) {
      this->push_back(el);
    }
  }

  this->sort();
}

template <typename T>
void List<T>::splice(const_iterator pos, List<T>& other) {
  if (empty()) {
    assign(other.begin(), other.end());
  } else if (pos == cbegin()) {
    auto tmp_head = head;
    auto tmp_tail = tail;
    size_type tmp_size = size();

    assign(other.begin(), other.end());

    tail->set_next(tmp_head);
    tmp_head->set_prev(tail);
    tail = tmp_tail;

    size_ += tmp_size;
  } else {
    auto it = pos;
    for (auto& el : other) {
      it = insert(it, el);
    }
  }
}

template <typename T>
void List<T>::reverse() {
  if (empty()) {
    return;
  }

  node_ptr it = head;
  node_ptr next = nullptr;
  node_ptr prev = nullptr;

  while (it) {
    next = it->next();
    it->set_prev(next);
    it->set_next(prev);

    prev = it;
    it = next;
  }

  tail = head;
  head = prev;
}

template <typename T>
void List<T>::unique() {
  if (empty()) {
    return;
  }

  node_ptr current = head;
  while (current && current->next()) {
    if (current->get_data() == current->next()->get_data()) {
      node_ptr duplicate = current->next();
      while (duplicate && duplicate->next() &&
             duplicate->next()->get_data() == current->get_data()) {
        duplicate = duplicate->next();
      }
      current->set_next(duplicate->next());
      if (duplicate->next()) {
        duplicate->next()->set_prev(current);
      } else {
        tail = current;
      }
    }
    current = current->next();
  }
}

}  // namespace s21

#include <memory>

#include "s21_list_node.h"

namespace s21 {

template <typename T>
class List;

template <typename T>
class BaseListIterator {
 public:
  using value_type = std::remove_const_t<T>;
  using reference = T&;
  using pointer = T*;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;
  using node = ListNode<value_type>;

  BaseListIterator() = default;

  std::weak_ptr<node> get_wp() const { return ptr_; }
  bool get_end() const { return is_end_; }

  explicit operator bool() const noexcept {
    return !ptr_.expired() && is_end_ && get_ptr().get() != nullptr;
  }
  bool is_alive() const noexcept { return (bool)*this; }

  reference operator*() const { return get_ref().get_data(); }
  pointer operator->() const { return &get_ptr()->get_data(); }

 protected:
  BaseListIterator(std::weak_ptr<node> ptr, bool is_end = false)
      : ptr_(ptr), is_end_(is_end) {}

  std::shared_ptr<node> get_shared() const {
    try {
      return ptr_.lock();
    } catch (std::bad_weak_ptr& e) {
      throw std::runtime_error(
          "Error: Attempt to access an expired weak pointer");
    }
  }

  std::shared_ptr<node> get_ptr() const { return get_shared(); }

  node& get_ref() const { return *get_ptr(); }

  void next() {
    if (is_end_) {
      throw std::out_of_range("Error: Attempt to access beyond list");
    }

    auto shared = get_shared();
    auto next = shared->next();
    if (next == nullptr) {
      is_end_ = true;
    }
    ptr_ = next;
  }

  void prev() {
    if (is_end_) {
      is_end_ = false;
      return;
    }

    auto shared = get_shared();
    ptr_ = shared->prev();
  }

  bool equals(const BaseListIterator<T>& other) const {
    if (ptr_.expired() && other.ptr_.expired()) {
      return true;
    }

    if ((ptr_.expired() && is_end_) &&
        (other.ptr_.expired() && other.is_end_)) {
      return true;
    }
    if (is_end_ && other.is_end_) {
      return true;
    }

    return !is_end_ && !other.is_end_ && get_ptr() == other.get_ptr();
  }

  std::weak_ptr<node> ptr_;
  bool is_end_;
};

template <typename T>
class ListIterator : public BaseListIterator<T> {
 public:
  friend class List<T>;
  using base = BaseListIterator<T>;
  using value_type = typename base::value_type;
  using pointer = typename base::pointer;
  using reference = typename base::reference;
  using difference_type = typename base::difference_type;
  using iterator_category = typename base::iterator_category;

  ListIterator() = default;
  ListIterator(const ListIterator<T>& other) = default;
  ListIterator(ListIterator<T>&& other) noexcept = default;
  ~ListIterator() = default;

  ListIterator<T>& operator=(const ListIterator<T>& other) = default;
  ListIterator<T>& operator=(ListIterator<T>&& other) noexcept = default;

  ListIterator<T>& operator++();
  ListIterator<T> operator++(int);
  ListIterator<T>& operator--();
  ListIterator<T> operator--(int);

  friend bool operator==(const ListIterator& a, const ListIterator& b) {
    return a.equals(b);
  }
  friend bool operator!=(const ListIterator& a, const ListIterator& b) {
    return !(a == b);
  }

 protected:
  using base::base;
};

template <typename T>
ListIterator<T>& ListIterator<T>::operator++() {
  this->next();
  return *this;
}

template <typename T>
ListIterator<T> ListIterator<T>::operator++(int) {
  auto tmp{*this};
  ++(*this);

  return tmp;
}

template <typename T>
ListIterator<T>& ListIterator<T>::operator--() {
  this->prev();
  return *this;
}

template <typename T>
ListIterator<T> ListIterator<T>::operator--(int) {
  auto tmp{*this};
  --(*this);

  return tmp;
}

template <typename T>
class ConstListIterator : public BaseListIterator<const T> {
 public:
  friend class List<T>;
  using base = BaseListIterator<const T>;
  using value_type = typename base::value_type;
  using pointer = typename base::pointer;
  using reference = typename base::reference;
  using difference_type = typename base::difference_type;
  using iterator_category = typename base::iterator_category;

  ConstListIterator() = default;
  ConstListIterator(const ConstListIterator<T>& other) = default;
  ConstListIterator(ConstListIterator<T>&& other) noexcept = default;
  ConstListIterator(const ListIterator<T>& other)
      : base(other.get_wp(), other.get_end()) {}
  ConstListIterator(ListIterator<T>&& other)
      : base(other.get_wp(), other.get_end()) {}
  ~ConstListIterator() = default;

  ConstListIterator<T>& operator=(const ConstListIterator<T>& other) = default;
  ConstListIterator<T>& operator=(ConstListIterator<T>&& other) noexcept =
      default;

  ConstListIterator<T>& operator=(const ListIterator<T>& other) {
    *this = ConstListIterator<T>(other);
    return *this;
  }

  ConstListIterator<T>& operator++();
  ConstListIterator<T> operator++(int);
  ConstListIterator<T>& operator--();
  ConstListIterator<T> operator--(int);

  friend bool operator==(const ConstListIterator& a,
                         const ConstListIterator& b) {
    return a.equals(b);
  }
  friend bool operator!=(const ConstListIterator& a,
                         const ConstListIterator& b) {
    return !(a == b);
  }

 protected:
  using base::base;
};

template <typename T>
ConstListIterator<T>& ConstListIterator<T>::operator++() {
  this->next();
  return *this;
}

template <typename T>
ConstListIterator<T> ConstListIterator<T>::operator++(int) {
  auto tmp{*this};
  ++(*this);

  return tmp;
}

template <typename T>
ConstListIterator<T>& ConstListIterator<T>::operator--() {
  this->prev();
  return *this;
}

template <typename T>
ConstListIterator<T> ConstListIterator<T>::operator--(int) {
  auto tmp{*this};
  --(*this);

  return tmp;
}

}  // namespace s21

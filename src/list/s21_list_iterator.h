#include <memory>

#include "s21_list_node.h"

template <typename T>
class S21List;

template <typename T>
class BaseListIterator {
    public:
        using value_type = std::remove_const_t<T>;
        using reference = T&;
        using pointer = T*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;

        BaseListIterator() = default;
        BaseListIterator(const BaseListIterator& other) = default;
        BaseListIterator(BaseListIterator&& other) = default;
        ~BaseListIterator() noexcept = default;

        BaseListIterator& operator=(const BaseListIterator& other) = default;
        BaseListIterator& operator=(BaseListIterator&& other) = default;

        reference operator*() const {
            return get_ref().get_data();
        }

        pointer operator->() const {
            return &get_ptr()->get_data();
        }

    protected:
        using node = S21ListNode<value_type>;
        
        BaseListIterator(std::shared_ptr<node> ptr, bool is_end = false) : ptr_(ptr), is_end_(is_end) {}
        
        std::shared_ptr<node> get_shared() const {
            try {
                return ptr_.lock();
            } catch (std::bad_weak_ptr& e) {
                throw std::runtime_error("Error: Attempt to access an expired weak pointer");
            }
        }

        node* get_ptr() const {
            return get_shared().get();
        }

        node& get_ref() const {
            return *get_ptr();
        }

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
            if ((ptr_.expired() && is_end_) && (other.ptr_.expired() && other.is_end_)) {
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
class S21ListIterator : public BaseListIterator<T> {
    public:
        friend class S21List<T>;
        using base = BaseListIterator<T>;
        using value_type = typename base::value_type;
        using pointer = typename base::pointer;
        using reference = typename base::reference;
        using difference_type = typename base::difference_type;
        using iterator_category = typename base::iterator_category;
        
        S21ListIterator() = default;
        S21ListIterator(const S21ListIterator<T>& other) = default;
        S21ListIterator(S21ListIterator<T>&& other) noexcept = default;
        ~S21ListIterator() = default;

        S21ListIterator<T>& operator=(const S21ListIterator<T>& other) = default;
        S21ListIterator<T>& operator=(S21ListIterator<T>&& other) noexcept = default;
        
        S21ListIterator<T>& operator++();
        S21ListIterator<T> operator++(int);
        S21ListIterator<T>& operator--();
        S21ListIterator<T> operator--(int);

        friend bool operator==(const S21ListIterator& a, const S21ListIterator& b) {
            return a.equals(b);
        }
        friend bool operator!=(const S21ListIterator& a, const S21ListIterator& b) {
            return !(a == b);
        }
    
    protected:
        using base::base;
};

template <typename T>
S21ListIterator<T>& S21ListIterator<T>::operator++() {
    this->next();
    return *this;
}

template <typename T>
S21ListIterator<T> S21ListIterator<T>::operator++(int) {
    auto tmp {*this};
    ++(*this);

    return tmp;
}

template <typename T>
S21ListIterator<T>& S21ListIterator<T>::operator--() {
    this->prev();
    return *this;
}

template <typename T>
S21ListIterator<T> S21ListIterator<T>::operator--(int) {
    auto tmp {*this};
    --(*this);

    return tmp;
}

template <typename T>
class S21ConstListIterator : public BaseListIterator<const T> {
    public:
        friend class S21List<T>;
        using base = BaseListIterator<const T>;
        using value_type = typename base::value_type;
        using pointer = typename base::pointer;
        using reference = typename base::reference;
        using difference_type = typename base::difference_type;
        using iterator_category = typename base::iterator_category;
        
        S21ConstListIterator() = default;
        S21ConstListIterator(const S21ConstListIterator<T>& other) = default;
        S21ConstListIterator(S21ConstListIterator<T>&& other) noexcept = default;
        ~S21ConstListIterator() = default;

        S21ConstListIterator<T>& operator=(const S21ConstListIterator<T>& other) = default;
        S21ConstListIterator<T>& operator=(S21ConstListIterator<T>&& other) noexcept = default;
        
        S21ConstListIterator<T>& operator++();
        S21ConstListIterator<T> operator++(int);
        S21ConstListIterator<T>& operator--();
        S21ConstListIterator<T> operator--(int);

        friend bool operator==(const S21ConstListIterator& a, const S21ConstListIterator& b) {
            return a.equals(b);
        }
        friend bool operator!=(const S21ConstListIterator& a, const S21ConstListIterator& b) {
            return !(a == b);
        }
    
    protected:
        using base::base;
};

template <typename T>
S21ConstListIterator<T>& S21ConstListIterator<T>::operator++() {
    this->next();
    return *this;
}

template <typename T>
S21ConstListIterator<T> S21ConstListIterator<T>::operator++(int) {
    auto tmp {*this};
    ++(*this);

    return tmp;
}

template <typename T>
S21ConstListIterator<T>& S21ConstListIterator<T>::operator--() {
    this->prev();
    return *this;
}

template <typename T>
S21ConstListIterator<T> S21ConstListIterator<T>::operator--(int) {
    auto tmp {*this};
    --(*this);

    return tmp;
}

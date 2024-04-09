#pragma once

#include <memory>
#include "s21_base_array_iterator.h"

namespace s21 {

template <typename T>
class S21Vector;

template <typename T>
class ConstVectorIterator : public BaseArrayIterator<const T> {
    public:
        using base = BaseArrayIterator<const T>;
        using value_type = typename base::value_type;
        using pointer = typename base::pointer;
        using reference = typename base::reference;
        using difference_type = typename base::difference_type;
        using iterator_category = typename base::iterator_category;
        
        using base::base;

        ConstVectorIterator() = default;
        ConstVectorIterator(const ConstVectorIterator<T>& other) = default;
        ConstVectorIterator(ConstVectorIterator<T>&& other) = default;
        ~ConstVectorIterator() = default;

        ConstVectorIterator<T>& operator=(const ConstVectorIterator<T>& other) = default;
        ConstVectorIterator<T>& operator=(ConstVectorIterator<T>&& other) noexcept = default;

        bool operator==(const ConstVectorIterator<T>& other) const noexcept;
        bool operator!=(const ConstVectorIterator<T>& other) const noexcept;
        bool operator>(const ConstVectorIterator<T>& other) const noexcept;
        bool operator<(const ConstVectorIterator<T>& other) const noexcept;
        bool operator>=(const ConstVectorIterator<T>& other) const noexcept;
        bool operator<=(const ConstVectorIterator<T>& other) const noexcept;

        ConstVectorIterator<T>& operator++();
        ConstVectorIterator<T> operator++(int);
        ConstVectorIterator<T>& operator--();
        ConstVectorIterator<T> operator--(int);

        ConstVectorIterator<T> operator+(int step);
        ConstVectorIterator<T> operator-(int step);
        difference_type operator-(const ConstVectorIterator<T>& other) const noexcept;

    private:
        friend class S21Vector<T>;
};

template <typename T>
bool ConstVectorIterator<T>::operator==(const ConstVectorIterator<T>& other) const noexcept {
    return this->get_shared().get() == other.get_shared().get() && this->index_ == other.index_;
}

template <typename T>
bool ConstVectorIterator<T>::operator!=(const ConstVectorIterator<T>& other) const noexcept {
    return !(*this == other);
}

template <typename T>
ConstVectorIterator<T>& ConstVectorIterator<T>::operator++() {
    this->forward();
    return *this;
}

template <typename T>
ConstVectorIterator<T> ConstVectorIterator<T>::operator++(int) {
    auto tmp {*this};
    this->forward();
    return tmp;
}

template <typename T>
ConstVectorIterator<T>& ConstVectorIterator<T>::operator--() {
    this->backward();
    return *this;
}

template <typename T>
ConstVectorIterator<T> ConstVectorIterator<T>::operator--(int) {
    auto tmp {*this};
    this->backward();
    return tmp;
}

template <typename T>
ConstVectorIterator<T> ConstVectorIterator<T>::operator+(int step) {
    this->forward(step);
    return *this;
}

template <typename T>
ConstVectorIterator<T> ConstVectorIterator<T>::operator-(int step) {
    this->backward(step);
    return *this;
}

template <typename T>
typename ConstVectorIterator<T>::difference_type ConstVectorIterator<T>::operator-(const ConstVectorIterator<T>& other) const noexcept {
    return this->index_ - other.index_;
}

template <typename T>
bool ConstVectorIterator<T>::operator>(const ConstVectorIterator<T>& other) const noexcept {
    return this->index_ > other.index_;
}

template <typename T>
bool ConstVectorIterator<T>::operator<(const ConstVectorIterator<T>& other) const noexcept {
    return this->index_ < other.index_;
}

template <typename T>
bool ConstVectorIterator<T>::operator>=(const ConstVectorIterator<T>& other) const noexcept {
    return this->index_ >= other.index_;
}

template <typename T>
bool ConstVectorIterator<T>::operator<=(const ConstVectorIterator<T>& other) const noexcept {
    return this->index_ <= other.index_;
}

}

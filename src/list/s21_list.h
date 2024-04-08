#pragma once

#include <limits>
#include "s21_list_iterator.h"
#include "s21_list_node.h"

template <typename T>
class S21List {
        using node = S21ListNode<T>;
        using node_ptr = std::shared_ptr<node>;
    public:
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using size_type = size_t;
        using iterator = S21ListIterator<T>;
        using const_iterator = S21ConstListIterator<T>;

        S21List() = default;
        explicit S21List(size_type n, const_reference value = value_type {});
        S21List(std::initializer_list<value_type> const& items);
        S21List(const S21List& other);
        S21List(S21List&& other) noexcept = default;
        ~S21List() noexcept = default;
        
        iterator begin();
        iterator end();
        const_iterator cbegin() const;
        const_iterator cend() const;
    
        const_reference front();
        const_reference back();

        bool empty() const noexcept;
        size_type size() const noexcept;
        size_type max_size() const noexcept;

        void clear() noexcept;
        iterator insert(iterator pos, const_reference value);
        void erase(iterator pos);
        void push_back(const_reference value);
        void pop_back();
        void push_front(const_reference value);
        void pop_front();
        void swap(S21List<T>& other);
        void merge(S21List<T>& other);
        /* void splice() */
        void reverse();
        void unique();
        void sort();

        S21List& operator=(S21List&& other) noexcept;
    private:
        node_ptr head {nullptr};
        node_ptr tail {nullptr};
        size_type size_ {};
};

template <typename T>
typename S21List<T>::size_type S21List<T>::size() const noexcept {
    return size_;
}

template <typename T>
bool S21List<T>::empty() const noexcept {
    return !size();
}

template <typename T>
typename S21List<T>::size_type S21List<T>::max_size() const noexcept {
    return std::numeric_limits<size_type>::max();
}

template <typename T>
void S21List<T>::clear() noexcept {
    head = tail = nullptr;
    size_ = 0;
}

template <typename T>
void S21List<T>::push_back(const_reference value) {
    node_ptr ptr {};
    ptr = std::make_shared<node>(value);
    
    if (!head) {
        head = tail = ptr;
    } else {
        ptr->set_prev(tail);
        tail->set_next(ptr);
        tail = ptr;

    }

    ++size_;
}

template <typename T>
void S21List<T>::pop_back() {
    if (empty()) {
        throw std::runtime_error("Error: List is empty");
    }

    auto prev = tail->prev();
    prev->set_next(nullptr);
    tail = prev;

    --size_;
}

template <typename T>
void S21List<T>::pop_front() {
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
void S21List<T>::push_front(const_reference value) {
    node_ptr ptr {};
    ptr = std::make_shared<node>(value);
    
    ptr->set_next(head);
    if (head) {
        head->set_prev(ptr);
    }
    head = ptr;

    if (!tail) {
        tail = ptr;
    }

    ++size_;
}

template <typename T>
S21List<T>::S21List(std::initializer_list<value_type> const& items) {
    for (auto& el: items) {
        push_back(el);
    }
}

template <typename T>
typename S21List<T>::iterator S21List<T>::begin() {
    return iterator {head};
}

template <typename T>
typename S21List<T>::iterator S21List<T>::end() {
    return iterator {tail, true};
}

template <typename T>
typename S21List<T>::const_iterator S21List<T>::cbegin() const {
    return const_iterator {head};
}

template <typename T>
typename S21List<T>::const_iterator S21List<T>::cend() const {
    return const_iterator {tail, true};
}

template <typename T>
typename S21List<T>::const_reference S21List<T>::front() {
    return head->get();
}

template <typename T>
typename S21List<T>::const_reference S21List<T>::back() {
    return tail->get();
}

template <typename T>
typename S21List<T>::iterator S21List<T>::insert(iterator pos, const_reference value) {
    if (empty() || pos == begin()) {
        push_front(pos, value);
    }

    iterator temp;
    for (; std::next(temp) != pos; ++temp) {
        ;
    }

    pos->set_next(temp.get().next());
    temp.get.set_next() = pos;
    if (!pos->next()) {
        tail = pos;
    }
}





























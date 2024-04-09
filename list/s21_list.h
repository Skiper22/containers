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
        explicit List(size_type n, const_reference value = value_type {});
        List(std::initializer_list<value_type> const& items);
        List(const List& other);
        List(List&& other) noexcept = default;
        ~List() noexcept = default;
        
        List<T>& operator=(const List& other) = default;
        List<T>& operator=(List&& other) noexcept = default;
        
        iterator begin();
        iterator end();
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
        node_ptr head {nullptr};
        node_ptr tail {nullptr};
        size_type size_ {};
};

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
List<T>::List(std::initializer_list<value_type> const& items) {
    for (auto& el: items) {
        push_back(el);
    }
}

template <typename T>
typename List<T>::iterator List<T>::begin() {
    return iterator {head};
}

template <typename T>
typename List<T>::iterator List<T>::end() {
    return iterator {tail, true};
}

template <typename T>
typename List<T>::const_iterator List<T>::cbegin() const {
    return const_iterator {head};
}

template <typename T>
typename List<T>::const_iterator List<T>::cend() const {
    return const_iterator {tail, true};
}

template <typename T>
typename List<T>::const_reference List<T>::front() {
    return head->get();
}

template <typename T>
typename List<T>::const_reference List<T>::back() {
    return tail->get();
}

template <typename T>
void List<T>::assign(iterator first, iterator last) {
    clear();
    for (; first != last; ++first) {
        push_back(*first);
    }
}

template <typename T>
typename List<T>::iterator List<T>::insert(const_iterator pos, const_reference value) {
    if (empty() || pos == begin()) {
        push_front(value);
        return begin();
    }
    
    iterator temp;
    for (; std::next(temp) != pos; ++temp) {
        ;
    }
    
    node_ptr ptr = std::make_shared<node>(value);

    ptr->set_next(temp.get_ref().next());
    temp.get_ref().set_next(ptr);
    if (!ptr->next()) {
        tail = ptr;
    }

    return ++temp;
}

template <typename T>
void List<T>::erase(iterator pos) {
    if (empty() || pos == begin()) {
        clear();
    }

    iterator temp;
    for (; std::next(temp) != pos; ++temp) {
        ;
    }
    
    auto next = temp.get.next()->next();
    temp.get().set_next(next);
    next->set_prev(temp.get_shared());
    --size_;
}

template <typename T>
void List<T>::swap(List<T>& other) {
    std::swap(head, other.head);
    std::swap(tail, other.tail);
    std::swap(size_, other.size);
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
    if (*this == other) {
        return;
    }
    for (auto& el : other) {
        this->push_back(el);
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
    node_ptr next = current->next();
    while (next) {
        if (next->get_data() == current->get_data()) {
            next = next->next();
            current->set_next(next);
            if (next) {
                next->set_prev(current);
            }    
        } else {
            current = next;
            if (next) {
                next = next->next();
            }
        }
    }
}

}

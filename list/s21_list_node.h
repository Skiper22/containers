#pragma once

#include <memory>

namespace s21 {

template <typename T>
class ListNode {
    public:
        ListNode() = default;
        explicit ListNode(const T& data) noexcept : data_(data) {}
        explicit ListNode(T&& data) noexcept : data_(data) {}
        ListNode(const ListNode<T>& other) = default;
        ListNode(ListNode<T>&& other) = default;
        ~ListNode() noexcept = default;
        
        std::shared_ptr<ListNode> next() noexcept {
            return next_;
        }
        std::shared_ptr<ListNode> prev() noexcept {
            return prev_.lock();
        }
        T& get_data() &noexcept {
            return data_;
        }
        const T& get_data() const &noexcept {
            return data_;
        }
        void set_data(const T data) {
            data_ = data;
        }
        void set_next(std::shared_ptr<ListNode> next) {
            next_ = next;
        }
        void set_prev(std::shared_ptr<ListNode> prev) {
            prev_ = prev;
        }

    private:
        std::shared_ptr<ListNode> next_;
        std::weak_ptr<ListNode> prev_;
        T data_ {};
};

}

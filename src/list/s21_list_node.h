#pragma once

#include <memory>

template <typename T>
class S21ListNode {
    public:
        S21ListNode() = default;
        explicit S21ListNode(const T& data) noexcept : data_(data) {}
        explicit S21ListNode(T&& data) noexcept : data_(data) {}
        S21ListNode(const S21ListNode<T>& other) = default;
        S21ListNode(S21ListNode<T>&& other) = default;
        ~S21ListNode() noexcept = default;
        
        std::shared_ptr<S21ListNode> next() noexcept {
            return next_;
        }
        std::shared_ptr<S21ListNode> prev() noexcept {
            return prev_.lock();
        }
        T& get_data() &noexcept {
            return data_;
        }
        const T& get_data() const &noexcept {
            return data_;
        }
        void set_next(std::shared_ptr<S21ListNode> next) {
            next_ = next;
        }
        void set_prev(std::shared_ptr<S21ListNode> prev) {
            prev_ = prev;
        }

    private:
        std::shared_ptr<S21ListNode> next_;
        std::weak_ptr<S21ListNode> prev_;
        T data_ {};
};

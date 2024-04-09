#pragma once

#include <memory>

namespace s21 {

template <typename T>
class BaseArrayIterator {
    public:
        using value_type = std::remove_const_t<T>;
        using pointer = T*;
        using reference = T&;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;
        
        BaseArrayIterator() = default;
        BaseArrayIterator(std::shared_ptr<value_type[]> ptr, size_t size) : ptr_(ptr), size_(size) {};
        virtual ~BaseArrayIterator() = default;
    
        reference operator*() const noexcept {
            return *(this->get_shared().get() + this->index_);
        }

        pointer operator->() const noexcept {
            return this->get_shared().get() + this->index_;
        }
    
    protected:
        std::shared_ptr<value_type[]> get_shared() const {
            try {
                return ptr_.lock();
            } catch (std::bad_weak_ptr &e) {
                throw std::runtime_error("Error: Attempted to access an expired weak pointer");
            }
        }
        void forward(int step=1) {
            if (index_ + step > size_) {
                throw std::out_of_range("Error: Step goes beyond vector size");
            }
            index_ += step;
        }
        void backward(int step=1) {
            if (index_ - step > size_) {
                throw std::runtime_error("Error: Step goes beyond vector size");
            }
            index_ -= step;
        }
    protected:
        std::weak_ptr<value_type[]> ptr_;
        size_t index_ {0};
        size_t size_{0};
};

}

/*
 * Copyright (c) 2020-2023 Jeferson Santiago da Silva.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdint>
#include <iterator>

namespace jtl {

template <typename T>
class list {
 private:
    struct Node {
        Node *next = nullptr;
        Node *previous = nullptr;
        T data;
        explicit Node(const T &d) : data{d} {}
    };

    class list_iterator {
     public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using pointer = Node *;
        using const_pointer = const pointer *;
        using reference = Node &;
        using const_reference = const Node &;

        constexpr list_iterator &operator++() {
            data_ = data_->next;
            return *this;
        }
        constexpr list_iterator &operator--() {
            data_ = data_->previous;
            return *this;
        }

        constexpr auto &operator*() {
            return data_->data;
        }
        constexpr const auto &operator*() const {
            return data_->data;
        }
        constexpr auto operator->() {
            return data_->data;
        }
        constexpr auto operator->() const {
            return data_->data;
        }

        explicit constexpr list_iterator(const pointer &ptr) : data_{ptr} {}
        explicit constexpr list_iterator(pointer &&ptr) : data_{ptr} {}

        // default operators
        constexpr list_iterator() = default;
        constexpr list_iterator(const list_iterator &) = default;
        constexpr list_iterator(list_iterator &&) = default;
        constexpr list_iterator &operator=(const list_iterator &) = default;
        constexpr list_iterator &operator=(list_iterator &&) = default;

        constexpr auto operator<=>(const list_iterator &other) const = default;

     protected:
        pointer data_{nullptr};
    };

    class list_reverse_iterator : public list_iterator {
     public:
        explicit constexpr list_reverse_iterator(
                const typename list_iterator::pointer &ptr)
                : list_iterator(ptr) {}

        constexpr list_reverse_iterator &operator++() {
            this->data_ = this->data_->previous;
            return *this;
        }
        constexpr list_reverse_iterator &operator--() {
            this->data_ = this->data_->next;
            return *this;
        }
    };

 public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = T *;
    using const_pointer = const T *;
    using iterator = list_iterator;
    using const_iterator = const iterator;
    using reverse_iterator = list_reverse_iterator;
    using const_reverse_iterator = const list_reverse_iterator;

    list() = default;
    ~list() {
        clear();
    }

    auto size() {
        return size_;
    }
    auto empty() {
        return size_ == 0;
    }

    const T &front() const {
        return head_->data;
    }
    T &front() {
        return head_->data;
    }
    const T &back() const {
        return tail_->data;
    }
    T &back() {
        return tail_->data;
    }

    void push_back(const T &value) {
        auto node = new Node{value};
        if (empty()) {
            head_ = tail_ = node;
            head_->next = tail_;
        } else {
            auto old_tail = tail_;
            tail_ = node;
            tail_->previous = old_tail;
            old_tail->next = tail_;
        }
        ++size_;
    }

    void pop_back() {
        if (empty()) {
            return;
        } else {
            auto old_tail = tail_;
            if (tail_ != head_) {
                tail_ = old_tail->previous;
                tail_->next = nullptr;
            } else {
                tail_ = nullptr;
                head_ = nullptr;
            }
            delete old_tail;
            --size_;
        }
    }

    void clear() {
        while (!empty()) {
            pop_back();
        }
        head_ = nullptr;
        tail_ = nullptr;
    }

    constexpr iterator begin() {
        return iterator(head_);
    }
    constexpr iterator begin() const {
        return iterator(head_);
    }

    constexpr iterator end() {
        return iterator(nullptr);
    }
    constexpr iterator end() const {
        return iterator(nullptr);
    }

    constexpr const_iterator cbegin() {
        return const_iterator(head_);
    }
    constexpr const_iterator &cbegin() const {
        return const_iterator(head_);
    }

    constexpr const_iterator cend() {
        return const_iterator(tail_->next);
    }
    constexpr const_iterator &cend() const {
        return const_iterator(tail_->next);
    }

    constexpr reverse_iterator rbegin() {
        return reverse_iterator(tail_);
    }
    constexpr reverse_iterator rbegin() const {
        return reverse_iterator(tail_);
    }

    constexpr reverse_iterator rend() {
        return reverse_iterator(nullptr);
    }
    constexpr reverse_iterator rend() const {
        return reverse_iterator(nullptr);
    }

    constexpr const_reverse_iterator crbegin() {
        return const_reverse_iterator(tail_);
    }
    constexpr const_reverse_iterator &crbegin() const {
        return const_reverse_iterator(tail_);
    }

    constexpr const_reverse_iterator crend() {
        return const_reverse_iterator(nullptr);
    }
    constexpr const_reverse_iterator &crend() const {
        return const_reverse_iterator(nullptr);
    }

 private:
    Node *head_ = nullptr;
    Node *tail_ = nullptr;
    std::uint64_t size_{0};
};

}  // namespace jtl

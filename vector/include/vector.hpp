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

#pragma once

/** @brief STL like vector
 *  Works with both custom and std allocators
 */

#include <algorithm>
#include <iterator>
#include <memory>
#include <vector>

namespace jtl {

template <typename T, typename Alloc = std::allocator<T>,
        typename Iterator = typename std::vector<T>::iterator>
class vector {
 public:
    using value_type = T;
    using allocator_type = Alloc;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer =
            typename std::allocator_traits<allocator_type>::const_pointer;
    using iterator = Iterator;
    using const_iterator = const iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

 private:
    pointer data_{nullptr};
    allocator_type alloc_{};
    size_type num_elements_{};
    size_type size_{};

    constexpr void reallocate(const size_type new_size,
            const bool shrink = false) {
        if (new_size > capacity() || shrink) {
            auto nptr = alloc_.allocate(new_size);
            std::copy(begin(), end(), nptr);
            alloc_.deallocate(data_, size());
            data_ = std::move(nptr);
            size_ = new_size;
        }
    }

 public:
    // Member functions
    constexpr vector() = default;

    template <typename InputIter>
    constexpr vector(InputIter first, InputIter last,
            const allocator_type alloc = allocator_type())
            : alloc_{alloc},
              num_elements_{
                      static_cast<std::size_t>(std::distance(first, last))},
              size_{static_cast<std::size_t>(std::distance(first, last))} {
        data_ = alloc_.allocate(size_);
        std::copy(first, last, &data_[0]);
    }

    constexpr vector(const vector &other)
            : num_elements_{other.size()},
              size_{other.capacity()} {
        data_ = alloc_.allocate(size_);
        std::copy(other.begin(), other.end(), data_);
    };

    constexpr vector(std::initializer_list<T> list,
            const allocator_type &alloc = Alloc())
            : vector(list.begin(), list.end(), alloc) {}

    constexpr vector &operator=(const vector &other) {
        // Protection against self assignment
        if (this != &other) {
            auto new_elems = alloc_.allocate(other.capacity());
            std::copy(other.begin(), other.end(), new_elems);
            alloc_.deallocate(data_, size_);
            data_ = std::move(new_elems);
            size_ = other.capacity();
            num_elements_ = other.size();
        }
        return *this;
    }
    // constexpr vector& operator=(vector&&) = default;

    constexpr ~vector() {
        alloc_.deallocate(data_, size_);
    };

    constexpr bool operator==(const vector &other) const {
        if ((size() == other.size()) && (capacity() == other.capacity())) {
            return std::equal(begin(), end(), other.begin());
        } else {
            return false;
        }
    }

    constexpr bool operator!=(const vector &other) const {
        return !(*this == other);
    }

    constexpr bool operator<(const vector &other) const {
        return std::lexicographical_compare(begin(), end(), other.begin(),
                other.end());
    }

    constexpr bool operator>(const vector &other) const {
        return std::lexicographical_compare(begin(), end(), other.begin(),
                other.end(),
                [](const auto lhs, const auto rhs) { return *lhs > *rhs; });
    }

    constexpr bool operator<=(const vector &other) const {
        return !(*this > other);
    }

    constexpr bool operator>=(const vector &other) const {
        return !(*this < other);
    }

    constexpr auto &operator[](std::size_t index) const {
        return data_[index];
    }
    constexpr auto &operator[](std::size_t index) {
        return data_[index];
    }

    constexpr void push_back(const value_type &d) {
        auto bkp_num_elements = num_elements_;
        if (num_elements_ >= capacity()) {
            resize(2 * size());
        }
        num_elements_ = bkp_num_elements;
        data_[num_elements_++] = d;
    }

    constexpr iterator begin() {
        return iterator(&data_[0]);
    }
    constexpr iterator begin() const {
        return iterator(&data_[0]);
    }

    constexpr iterator end() {
        return iterator(&data_[num_elements_]);
    }
    constexpr iterator end() const {
        return iterator(&data_[num_elements_]);
    }

    constexpr const_iterator cbegin() {
        return const_iterator(&data_[0]);
    }
    constexpr const_iterator &cbegin() const {
        return const_iterator(&data_[0]);
    }

    constexpr const_iterator cend() {
        return const_iterator(&data_[num_elements_]);
    }
    constexpr const_iterator &cend() const {
        return const_iterator(&data_[num_elements_]);
    }

    constexpr reverse_iterator rbegin() {
        return reverse_iterator(end());
    }
    constexpr reverse_iterator rbegin() const {
        return reverse_iterator(end());
    }

    constexpr reverse_iterator rend() {
        return reverse_iterator(begin());
    }
    constexpr reverse_iterator rend() const {
        return reverse_iterator(begin());
    }

    constexpr const_reverse_iterator crbegin() {
        return const_reverse_iterator(cend());
    }
    constexpr const_reverse_iterator &crbegin() const {
        return const_reverse_iterator(cend());
    }

    constexpr const_reverse_iterator crend() {
        return const_reverse_iterator(cbegin());
    }
    constexpr const_reverse_iterator &crend() const {
        return const_reverse_iterator(cbegin());
    }

    constexpr auto size() const {
        return static_cast<size_type>(std::distance(begin(), end()));
    }

    constexpr auto capacity() const {
        return size_;
    }

    constexpr void resize(const size_type new_size) {
        reallocate(new_size);
        num_elements_ = new_size;
    }

    constexpr void reserve(const size_type new_cap) {
        reallocate(new_cap);
        if (new_cap < size()) {
            num_elements_ = new_cap;
        }
    }

    constexpr void shrink_to_fit() {
        reallocate(size(), true);
    }

    constexpr reference emplace_back(auto &&...params) {
        auto bkp_num_elements = num_elements_;
        if (num_elements_ >= capacity()) {
            resize(2 * size());
        }
        num_elements_ = bkp_num_elements;
        data_[num_elements_++] = T{std::forward<decltype(params)>(params)...};
        return data_[bkp_num_elements];
    }

    [[nodiscard]] constexpr decltype(auto) empty() const noexcept {
        return begin() == end();
    }

    constexpr void clear() {
        num_elements_ = 0;
    }
};

}  // namespace jtl

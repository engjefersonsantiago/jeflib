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
/**
 * @brief STL complaint allocator
 * As std::allocator, this one is stateless
 * It's implemented using C++20 and can be used in constexpr contexts
 */

#include <memory>
namespace jtl {

template <typename T>
class allocator {
 public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

    constexpr allocator() noexcept = default;
    constexpr allocator(const allocator &other) noexcept = default;
    template <class U>
    [[maybe_unused]] constexpr allocator(
            [[maybe_unused]] const allocator<U> &other) noexcept {}
    constexpr ~allocator() = default;

    [[nodiscard]] constexpr T *allocate(size_type n) {
        return new T[n]{};
    }
    constexpr void deallocate(T *p, [[maybe_unused]] size_type n = 0) {
        delete[] p;
    }
};

}  // namespace jtl

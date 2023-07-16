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

#include <type_traits>

namespace jtl {

// Concepts
template <typename To, typename... From>
concept Is_Same_Decay = (std::is_same_v<std::decay_t<From>, To> && ...);

template <typename... Params>
concept Has_Plus = requires(Params... params) {
    {(params + ...)};
};

// Maybe constraint sum to integral and floating point types
// Or check if the + operator exists for the overloaded params
constexpr auto sum(
        const auto &...params) requires Has_Plus<decltype(params)...> {
    return (params + ...);
}

}  // namespace jtl

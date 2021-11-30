#pragma once

#include <iostream>
#include <stdio.h>
#include <unordered_map>
#include <functional>
#include <array>
#include <map>
#include <typeinfo>

namespace std {

template<typename T, size_t N>
struct hash<std::array<T, N>> {
    using argument_type = std::array<T, N>;

    std::size_t operator()(const argument_type& a) const {
        hash<T> hasher;
        std::size_t h = 0;
        for (std::size_t i = 0; i < N; ++i) {
            h = h * 31 + hasher(a[i]);
        }

        return h;
    }
};

}

namespace sparse_matrix {

template <typename M, std::size_t Dim>
class Proxy {

using value_t = typename M::value_t;
using multi_index_t = typename M::multi_index_t;

friend M;
friend class Proxy<M, Dim + 1>;

private:
    M& m;
    multi_index_t multi_index;

    Proxy(M& m_, std::size_t index) : m{m_} {
        multi_index[M::dimensions - Dim] = index;
    }
    Proxy(M& m_, std::size_t index, multi_index_t&& multi_index_) : m{m_}, multi_index{std::move(multi_index_)} {
        multi_index[M::dimensions - Dim] = index;
    }
public:

    Proxy<M, Dim - 1> operator[](std::size_t index) {
        if constexpr (Dim > 1) {
            return Proxy<M, Dim - 1>(m, index, std::move(multi_index));
        }
    }

    bool operator==(value_t&& value) const {
        if constexpr (Dim == 1) {
            auto it = m.storage.find(multi_index);
            if (it != m.storage.end()) {
                return m.storage.at(multi_index) == value;
            }
            return M::defaultVal == value;
        }
    }

    Proxy<M, Dim>& operator=(value_t&& value) {
        if constexpr (Dim == 1) {
            auto it = m.storage.find(multi_index);
            if (it != m.storage.end()) {
                if (value == M::defaultVal) {
                    m.storage.erase(multi_index);
                } else {
                    m.storage[multi_index] = value;
                }
            } else {
                if (value != M::defaultVal) {
                    m.storage[multi_index] = value;
                }
            }

            return *this;
        }
    }

    operator value_t() {
        if constexpr (Dim == 1) {
            auto it = m.storage.find(multi_index);
            if (it != m.storage.end()) {
                return m.storage.at(multi_index);
            }

            return M::defaultVal;
        }
    }
};

template <typename T, T Def, std::size_t Dim = 2>
class Matrix {

public:
using multi_index_t = std::array<std::size_t, Dim>;
using self_t  = Matrix<T, Def, Dim>;
using value_t = T;
using storage_t = std::unordered_map<multi_index_t, T>;
using const_iterator_t = typename storage_t::const_iterator;

friend class Proxy<self_t, 1>;
friend class Proxy<self_t, Dim>;

private:
    storage_t storage;

public:

    static constexpr std::size_t dimensions = Dim;
    static constexpr T           defaultVal = Def;

    std::size_t size() const {
        return storage.size();
    }

    bool empty() const {
        return storage.size() == 0;
    }

    Proxy<self_t, Dim> operator[](std::size_t index) {
        return Proxy<self_t, Dim>(*this, index);
    }

    const_iterator_t begin() const {
        return storage.cbegin();
    }

    const_iterator_t end() const {
        return storage.cend();
    }
};  

}
//
// Created by Alexander on 10.12.24.
//

#ifndef SYNTAX_HPP
#define SYNTAX_HPP


#include <array>
#include <cassert>
#include <string>


#define EPSILON 1e-10

/*

how it should be usable:

vectors:


float3 name1 = float3({1,2,3}); or
float3 name2 = float3(name1) or
float3 name3 = float3(1.0f)
float2 float3 float4

floatN<5> = floatN<5>({1,2,3,4,5});

float4 name4 = float4(name3,1.0f);
floatN<n> name5 = floatN<n>(f : floatN<n-1>, <imm>);

.T() methode which creates the transpose of a Matrix/vector
eg

              v there probably needs to be a cast happening
floatNxM<5,1> = floatN<5>({1,2,3,4,5}).T(); creates a row vector and transposes it


but in reality a floatN vector is just a 1xN matrix

a vector is just a special case of an NxM Matrix
*/


template<size_t N, size_t M>
class floatNxM {
public:
    std::array<float, N*M> data{0};

    floatNxM(const std::initializer_list<std::initializer_list<float>>& init);

    floatNxM() = default;

    ~floatNxM() = default;

    floatNxM(const floatNxM &other) = default;

    floatNxM &operator=(const floatNxM &other) = default;

    float& operator()(std::size_t row, std::size_t col) {
        return data[row * M + col];
    }

    const float& operator()(std::size_t row, std::size_t col) const {
        return data[row * M + col];
    }

    [[nodiscard]] std::string str() const {
        std::string res;
        for(int i=0;i<N;i++) {
            res += "(";
            for(int j=0;j<M;j++) {
                res += std::to_string(data[i * M + j]);
                if(j < M-1)
                    res += ", ";
            }
            res += ")";
            if(i < N-1)
                res += "\n";
        }
        return res;
    }
};

// template<size_t N>
// class floatN {
// public:
//     std::array<float, N> data{0};
//
//     floatN(const std::initializer_list<float> &init);
//
//     floatN() = default;
//
//     ~floatN() = default;
//
//     floatN(const floatN &other) = default;
//
//     floatN &operator=(const floatN &other) = default;
//
//     float& operator()(std::size_t index) {
//         return data[index];
//     }
//
//     const float& operator()(std::size_t index) const {
//         return data[index];
//     }
//
//     [[nodiscard]] std::string str() const {
//         std::string res = "[";
//         for (int i = 0; i < N; i++) {
//             res += std::to_string(data[i]);
//             if (i < N - 1) res += ", ";
//         }
//
//         return res + "]";
//     }
// };

template<size_t M>
class floatNxM<1,M> : public floatN<M>{
    using floatN<M>::floatN;

    float& operator()(std::size_t row, std::size_t col) {
        assert(row == 0 && "Row index out of bounds for row vector!");
        return (*this)[col];
    }

    const float& operator()(std::size_t row, std::size_t col) const {
        assert(row == 0 && "Row index out of bounds for row vector!");
        return (*this)[col];
    }
};

template<std::size_t N>
class floatNxM<N,1> : public floatN<N> {
public:
    using floatN<N>::floatN; // Inherit constructors

    float& operator()(std::size_t row, std::size_t col) {
        assert(col == 0 && "Row index out of bounds for row vector!");
        return (*this)[row];
    }

    const float& operator()(std::size_t row, std::size_t col) const {
        assert(col == 0 && "Row index out of bounds for row vector!");
        return (*this)[row];
    }
};

#define float2 floatNxM<1,2>
#define float3 floatNxM<1,3>
#define float4 floatNxM<1,4>

#define float2T floatNxM<2,1>
#define float3T floatNxM<3,1>
#define float4T floatNxM<4,1>

#define float2x2 floatNxM<2,2>
#define float3x3 floatNxM<3,3>
#define float4x4 floatNxM<4,4>

#endif //SYNTAX_HPP

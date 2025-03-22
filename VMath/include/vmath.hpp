#pragma once

#include <cassert>

#define EPSILON 1e-10



#define float2 floatNxM<1,2>
#define float3 floatNxM<1,3>
#define float4 floatNxM<1,4>

#define float2T floatNxM<2,1>
#define float3T floatNxM<3,1>
#define float4T floatNxM<4,1>

#define float2x2 floatNxM<2,2>
#define float3x3 floatNxM<3,3>
#define float4x4 floatNxM<4,4>



inline float RandomFloat(float min, float max) {
    assert(max > min);
    float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    float range = max - min;
    return (random*range) + min;
}



template<std::size_t N, std::size_t M> // N = rows M = cols
class floatNxM {
public:
    std::array<float, N*M> data{0};

    floatNxM(const std::initializer_list<std::initializer_list<float>>& init) {
        assert(init.size() <= N && "Initializer list size exceeds the number of rows!");

        std::size_t i = 0; // Index in the flat array
        for (const auto& row : init) {
            assert(row.size() <= M && "Row size exceeds the number of columns!");
            for (const auto& val : row) {
                data[i++] = val; // Copy value
            }
            // Fill the rest of the row with zeros if it's shorter than M
            for (std::size_t remaining = row.size(); remaining < M; ++remaining) {
                data[i++] = 0.0f;
            }
        }
    }

    floatNxM(const std::initializer_list<float>& init) {
        if(N == 1) {
            assert(init.size() <= M && "Initializer list size exceeds the fixed size!");
            std::size_t i = 0;
            for (float v: init) {
                data[i++] = v;
            }
            // Fill remaining elements with 0 if initializer list is smaller than N
            for (; i < N; ++i) {
                data[i] = 0.0f;
            }
            return;
        }
        if(M == 1) {
            assert(init.size() <= N && "Initializer list size exceeds the fixed size!");
            std::size_t i = 0;
            for (float v: init) {
                data[i++] = v;
            }
            // Fill remaining elements with 0 if initializer list is smaller than N
            for (; i < M; ++i) {
                data[i] = 0.0f;
            }
            return;
        }
    }

    floatNxM() = default;

    ~floatNxM() = default;

    floatNxM(const floatNxM &other) = default;


    void RandMatrix(float min, float max) {
        for(int i=0;i<N*M;i++) {
            data[i] = RandomFloat(min, max);
        }
    }

    explicit floatNxM(const float value) {
        for(int i=0;i<N*M;i++) {
            data[i] = value;
        }
    }

    floatNxM &operator=(const floatNxM &other) = default;

    bool operator==(const floatNxM &other) const {
        for (int i = 0; i < N*M; i++) {
            if (fabs(data[i] - other.data[i]) > EPSILON) return false;
        }
        return true;
    }

    bool operator!=(const floatNxM &other) const {
        return !(*this == other);
    }

    floatNxM operator+(const floatNxM& other) const {
        floatNxM res;
        for(int i=0;i<N*M;i++)
            res.data[i] = data[i] + other.data[i];
        return res;
    }

    floatNxM operator+(const float value) const {
        floatNxM res;
        for(int i=0;i<N*M;i++)
            res.data[i] = data[i] + value;
        return res;
    }

    friend floatNxM operator+(const float value, const floatNxM& other) {
        return other + value;
    }

    floatNxM& operator+=(const floatNxM& other) {
        for(int i=0;i<N*M;i++)
            data[i] += other.data[i];
        return *this;
    }

    floatNxM& operator+=(const float value) {
        for(int i=0;i<N*M;i++)
            data[i] += value;
        return *this;
    }

    floatNxM operator-(const floatNxM& other) const {
        floatNxM res;
        for(int i=0;i<N*M;i++)
            res.data[i] = data[i] - other.data[i];
        return res;
    }

    floatNxM operator-(const float value) const {
        floatNxM res;
        for(int i=0;i<N*M;i++)
            res.data[i] = data[i] - value;
        return res;
    }

    friend floatNxM operator-(const float value, const floatNxM& other) {
        return other - value;
    }

    floatNxM& operator-=(const floatNxM& other) {
        for(int i=0;i<N*M;i++)
            data[i] -= other.data[i];
        return *this;
    }

    floatNxM& operator-=(const float value) {
        for(int i=0;i<N*M;i++)
            data[i] -= value;
        return *this;
    }



    floatNxM operator*(const float scalar) const {
        floatNxM res;
        for (std::size_t i = 0; i < N*M; ++i) {
            res.data[i] = data[i] * scalar;
        }
        return res;
    }

    friend floatNxM operator*(const float scalar, const floatNxM& other) {
        return other * scalar;
    }

    floatNxM& operator*=(const floatNxM& other) {
        for(int i=0;i<N*M;i++)
            data[i] *= other.data[i];
        return *this;
    }

    floatNxM& operator*=(const float value) {
        for(int i=0;i<N*M;i++)
            data[i] *= value;
        return *this;
    }

    floatNxM operator/(const floatNxM& other) const {
        floatNxM res;
        for(int i=0;i<N*M;i++)
            res.data[i] = data[i] / other.data[i];
        return res;
    }

    floatNxM operator/(const float value) const {
        floatNxM res;
        for(int i=0;i<N*M;i++)
            res.data[i] = data[i] / value;
        return res;
    }

    friend floatNxM operator/(const float value, const floatNxM& other) {
        return other / value;
    }

    floatNxM& operator/=(const floatNxM& other) {
        for(int i=0;i<N*M;i++)
            data[i] /= other.data[i];
        return *this;
    }

    floatNxM& operator/=(const float value) {
        for(int i=0;i<N*M;i++)
            data[i] /= value;
        return *this;
    }

    floatNxM<M,N> T() const {
        floatNxM<M,N> res;
        for (std::size_t row = 0; row < N; ++row) {
            for (std::size_t col = 0; col < M; ++col) {
                res(col, row) = (*this)(row, col);
            }
        }
        return res;
    }



    [[nodiscard]] float det() {
        static_assert(N == M && "The Determinant can only be calculated for Square matricies!");
        if(N == 1) return data[0];
        if(N == 2) return data[0]*data[3] - data[1]*data[2];
        if(N == 3)
            return data[0]*data[4]*data[8] + data[1]*data[5]*data[6] + data[2]*data[3]*data[7]
                -  data[2]*data[4]*data[6] - data[0]*data[5]*data[7] - data[1]*data[3]*data[8];
        float det = 1;
        for(int i=0;i<N-1;i++) {
            int r=i;
            float maxA = fabs( (*this)(i,i) );
            for(int k = i + 1; k < N; k++) {
                float val = fabs( (*this)(k,i) );
                if(val > maxA) {
                    r=k;
                    maxA = val;
                }
            }
            if(r != i) {
                for(int j=i;j<N;j++) {
                    float t = (*this)(i,j);
                    auto l = (*this)(r,j);
                    (*this)(i,j) = l;
                    (*this)(r,j) = t;
                }
                det = -det;
            }

            float pivot = (*this)(i,i);
            if(fabs(pivot) <= EPSILON) return 0.0f;

            for(r=i+1;r<N;r++) {
                float multiple = (*this)(r,i) / pivot;
                for(int j=i;j<N;j++)(*this)(r,j) -= multiple * (*this)(i,j);
            }
            det *= pivot;
        }
        det *= (*this)(N-1,N-1);
        return det;
    }

    float& operator()(std::size_t row, std::size_t col) {
        return data[row * M + col];
    }

    const float& operator()(std::size_t row, std::size_t col) const {
        return data[row * M + col];
    }

    floatNxM<1,N> normalized() const {
        const auto len = mag(*this);
        if(len <= EPSILON)
            return floatNxM<1,N>(0.0f);
        return *this / len;
    }

    floatNxM<1,N> cross(const floatNxM<1,N>& other) const {
        assert(N == 3 && "Cross product is only defined for 3-dimensional vectors.");
        if(N == 3) {
            return {
                data[1] * other.data[2] - data[2] * other.data[1],
                data[2] * other.data[0] - data[0] * other.data[2],
                data[0] * other.data[1] - data[1] * other.data[0]
            };
        }
        return floatNxM<1,N>(0.0f);
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



template<size_t N>
using float1xN = floatNxM<1,N>;

template<size_t N>
using floatNx1 = floatNxM<N,1>;


template<std::size_t N, std::size_t M, std::size_t P>
floatNxM<N, P> operator*(const floatNxM<N, M>& mat1, const floatNxM<M, P>& mat2) {
    floatNxM<N, P> res; // Resultant matrix of size N x P

    for (std::size_t row = 0; row < N; ++row) {
        for (std::size_t col = 0; col < P; ++col) {
            double sum = 0.0f; // Accumulate the dot product for this element
            for (std::size_t k = 0; k < M; ++k) {
                sum += mat1(row, k) * mat2(k, col);
            }
            res(row, col) = sum; // Set the computed value
        }
    }
    return res;
}

template<std::size_t N>
float operator*(const floatNxM<1,N>& vec1, const floatNxM<1,N> vec2) {
    float res=0;
    for(int i=0;i<N;i++) {
        res += vec1.data[i] * vec2.data[i];
    }
    return res;
}

template<std::size_t N>
float dot(const floatNxM<1,N>& vec1, const floatNxM<1,N> vec2) {
    float res=0;
    for(int i=0;i<N;i++) {
        res += vec1.data[i] * vec2.data[i];
    }
    return res;
}

template<std::size_t N>
float mag(const floatNxM<1,N>& vec1) {
    float res=0;
    for(int i=0;i<N;i++) {
        res += vec1.data[i] * vec1.data[i];
    }
    return sqrt(res);
}

template<std::size_t N>
float magSq(const floatNxM<1,N>& vec1) {
    float res=0;
    for(int i=0;i<N;i++) {
        res += vec1.data[i] * vec1.data[i];
    }
    return res;
}

#define length(x) mag(x)
#define lengthSq(x) magSq(x)

template<std::size_t N>
float distanceSq(const floatNxM<1,N>& vec1, const floatNxM<1,N>& vec2) {
    float res=0;
    for(int i=0;i<N;i++) {
        const float tmp = vec1.data[i] - vec2.data[i];
        res += tmp*tmp;
    }
    return sqrt(res);
}

template<std::size_t N>
float distance(const floatNxM<1,N>& vec1, const floatNxM<1,N>& vec2) {
    return sqrt(distanceSq(vec1,vec2));
}

template<std::size_t N>
floatNxM<1,N> cross(const floatNxM<1,N>& self,const floatNxM<1,N>& other) {
    assert(N == 3 && "Cross product is only defined for 3-dimensional vectors.");
    if(N == 3) {
        return {
            self.data[1] * other.data[2] - self.data[2] * other.data[1],
            self.data[2] * other.data[0] - self.data[0] * other.data[2],
            self.data[0] * other.data[1] - self.data[1] * other.data[0]
        };
    }
    return floatNxM<1,N>(0.0f);
}

template<std::size_t N>
floatNxM<1,N> perp(const floatNxM<1,N>& self,const floatNxM<1,N>& other=floatNxM<1,N>(0.0f)) {
    assert((N == 2 || N == 3) && "Perpendicular vector only makes sense in 2D and 3D");
    if(N == 2) {
        return floatNxM<1,N>({-self.data[1],self.data[0]});
    }
    if(N == 3) {
        return self->cross(other);
    }
    return floatNxM<1,N>(0.0f);
}

template<std::size_t N>
float angle(const floatNxM<1,N> &self, const floatNxM<1,N> &other) {
    const float dotproduct = self.dot(other);
    const float mags = self.mag() * other.mag();
    assert(dotproduct > EPSILON && mags > EPSILON && "Division by Zero!");
    const float cosTheta = std::clamp(dotproduct / mags, -1.0f, 1.0f);
    return acos(cosTheta);
}

template<std::size_t N>
floatNxM<1,N> rotate(const floatNxM<1,N>& self, const float angle) {
    assert(N == 2 && "for 3D use rotate(const float angle, const floatN& axis)\n\t and for higher use rotate(const float angle, const floatN& u, const floatN& v)!");
    const float cos_theta = cos(angle);
    const float sin_theta = sin(angle);
    return floatNxM<1,N>({self.data[0] * cos_theta - self.data[1] * sin_theta, self.data[0] * sin_theta + self.data[1] * cos_theta});
}

template<std::size_t N>
floatNxM<1,N> rotate(const floatNxM<1,N>& self, const float angle, const floatNxM<1,N>& axis) {
    assert(axis.mag() > EPSILON && "Axis must be a non-zero vector!");
    assert(N == 3 && "for higher dimensions use rotate(const float angle, const floatN& u, const floatN& v)!");

    floatNxM<1,N> unitAxis = axis.normalized();
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);

    // Rodrigues' rotation formula components
    floatNxM<1,N> term1 = self * cosTheta;
    floatNxM<1,N> term2 = unitAxis.cross(self) * sinTheta;
    floatNxM<1,N> term3 = unitAxis * (unitAxis.dot(self) * (1 - cosTheta));

    return term1 + term2 + term3;
}

template<std::size_t N>
floatNxM<1,N> rotate(const floatNxM<1,N>& self, const float angle, const floatNxM<1,N>& u, const floatNxM<1,N>& v) {
    // Ensure `u` and `v` are orthogonal
    assert(fabs(u.dot(v)) < EPSILON && "Vectors u and v must be orthogonal!");

    // Compute basis vectors for the plane
    floatNxM<1,N> unitU = u.normalized();
    floatNxM<1,N> unitV = v.normalized();

    // Compute the rotated vector
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);

    // Project onto the plane
    float projU = self.dot(unitU);
    float projV = self.dot(unitV);

    // Components of the rotation
    floatNxM<1,N> term1 = self - unitU * projU - unitV * projV; // Component orthogonal to the plane
    floatNxM<1,N> term2 = unitU * projU * cosTheta + unitV * projV * cosTheta;
    floatNxM<1,N> term3 = unitU * projV * sinTheta - unitV * projU * sinTheta;

    return term1 + term2 + term3;
}


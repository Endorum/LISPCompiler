#ifndef MATRIX_HPP
#define MATRIX_HPP
#include <cassert>
#include <string>
#include <execution>
#include <vector>

inline float RandomFloat(float min, float max){
    // this  function assumes max > min, you may want
    // more robust error checking for a non-debug build
    assert(max >= min);
    float random = ((float) rand()) / (float) RAND_MAX;

    // generate (in your case) a float between 0 and (4.5-.78)
    // then add .78, giving you a float between .78 and 4.5
    float range = max - min;
    return (random*range) + min;
}

class Matrix {
public:
    size_t rows{};
    size_t cols{};
    double* data{};

    Matrix(size_t r, size_t c)
        : rows(r), cols(c) {
        data = new double[rows*cols];
    }

    void randomize(double low, double high) {
        assert(data);
        for(int i=0;i<rows*cols;i++) {
            data[i]=RandomFloat(low,high);
        }
    }

#include <execution> // For std::for_each
#include <vector>    // For intermediate storage

    Matrix DMatrix() const {
        assert(rows == 1 && "Input must be a 1xN row matrix!");

        Matrix D(cols, cols); // NxN distance matrix

        // Parallelize outer loop with OpenMP
        #pragma omp parallel for
        for (int i = 0; i < cols; ++i) {
            for (int j = i + 1; j < cols; ++j) { // Compute upper triangle only
                double diff = data[j] - data[i];
                double signedSquare = diff * std::abs(diff); // Preserve sign while squaring
                D.data[i * cols + j] = signedSquare;
                D.data[j * cols + i] = -signedSquare; // Symmetric assignment
            }
            D(i, i) = 0; // Set diagonal element
        }

        return D;
    }




    Matrix& ResMatrix() {
        Matrix res = *this;
        for(int i=0;i<rows*cols;i++) {
            if(abs(data[i]) <= 0.01) {
                res.data[i] = 0.0f;
                continue;
            }
            res.data[i] = 1/data[i];
        }
        return res;
    }

    Matrix rowSum() {
        Matrix res = Matrix(1,rows);
        for(int i=0;i<rows;i++) {
            double current = 0;
            for(int j=0;j<cols;j++) {
                current += data[i * cols + j];
            }
            res.data[i] = current;
        }
        return res;

    }

    Matrix operator+(const Matrix& other) const {
        assert(rows == other.rows && cols == other.cols && "Matrices must have the same dimensions!");

        Matrix res(rows, cols); // Allocate result once

        // Use a single loop for linear indexing
        #pragma omp parallel for
        for (int i = 0; i < rows * cols; ++i) {
            res.data[i] = data[i] + other.data[i];
        }

        return res;
    }

    Matrix& operator+=(const Matrix& other) {
        assert(rows == other.rows && cols == other.cols && "Matrices must have the same dimensions!");

        for (int i = 0; i < rows * cols; ++i) {
            data[i] += other.data[i];
        }

        return *this;
    }




    double& operator()(int row, int col) {
        assert(row >= 0 && row < rows && "Row Index out of Bounds!");
        assert(col >= 0 && col < cols && "Row Index out of Bounds!");
        return data[row * cols + col];
    }

    double operator()(int row, int col) const {
        assert(row >= 0 && row < rows && "Row index out of bounds!");
        assert(col >= 0 && col < cols && "Column index out of bounds!");
        return data[row * cols + col];
    }

    // element wise mul
    Matrix operator*(const Matrix& other) const {
        assert(rows == other.rows && cols == other.cols && "Matrices must have the same dimensions for element-wise multiplication!");

        Matrix res(rows, cols);

        for (int i = 0; i < rows * cols; ++i) {
            res.data[i] = data[i] * other.data[i];
        }

        return res;
    }



    std::string str() const {
        std::string res;
        for(int r=0;r<rows;r++) {
            for(int c=0;c<cols;c++) {
                res += std::to_string(data[r * cols + c]) + ", ";
            }
            res += "\n";
        }
        return res;
    }
};

inline Matrix Gmatrix(const Matrix& D, double G) {
    Matrix res = Matrix(D.rows,D.cols);

    for(int i=0;i<D.rows*D.cols;i++) {
        if(abs(D.data[i]) <= 0.1) {
            res.data[i] = 0.0;
        }else {
            res.data[i] = G/D.data[i];
        }
    }

    return res;
}




inline std::pair<Matrix,Matrix> computeForces(const Matrix& Px, const Matrix& Py, double G=1.0) {
    assert(Px.rows == 1 && Py.rows == 1 && "Px and Py must be row matrices!");
    assert(Px.cols == Py.cols && "Px and Py must have the same number of particles!");

    Matrix Dx = Px.DMatrix();
    Matrix Dy = Py.DMatrix();

    Matrix Gx = Gmatrix(Dx,G);
    Matrix Gy = Gmatrix(Dy,G);



    





    return {Gx.rowSum(),Gy.rowSum()};

}

#endif //MATRIX_HPP



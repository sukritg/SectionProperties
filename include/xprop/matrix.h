#pragma  once
#include <memory>

// Row and Column index starts from 1.

class Matrix {
private:
    int m_rows, m_columns;
    std::unique_ptr<double[]> m_data;
public:
    //Constructor
    Matrix();
    explicit Matrix(int size);
    Matrix(int rows, int columns);

    // Copy Constructor
    Matrix(const Matrix& other);

    // Move Constructor
    Matrix(Matrix&& other) noexcept ;

    // Move Assignment
    Matrix& operator=(Matrix&& other) noexcept;

    // Matrix Functions
    Matrix scale(double scalingFactor) const;
    Matrix transpose() const;
    void reSize(int rows, int columns);
    void clear();

    // Operator Overloading
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    double& operator()(int row, int column) const;
    double& operator()(int row, int column);
    Matrix& operator=(const Matrix& other);
    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);

    // Output Stream
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
};

#ifndef STRIXA_MATRIX_H
#define STRIXA_MATRIX_H

namespace strixa
{
    namespace math
    {
        class Matrix
        {
            //Member Properties
            private:
                int   columns;
                long* data;
                int   rows;

            //Member Methods
            public:
                Matrix(int rows,int columns);

                Matrix(const Matrix& copy);
                
                Matrix(Matrix&& move);

                ~Matrix();

                long at(int row,int column) const;

                void fill(long value);

                Matrix transpose() const;

                long* operator[](int row) const;

                Matrix& operator=(const Matrix& operand);

                Matrix operator+(const Matrix& operand) const;

                Matrix& operator+=(const Matrix& operand);

                Matrix operator-(const Matrix& operand) const;

                Matrix& operator-=(const Matrix& operand);

                Matrix operator*(long scalar) const;

                Matrix& operator*=(long scalar);

                Matrix operator*(const Matrix& operand) const;
                
                Matrix& operator*=(const Matrix& operand);

                Matrix operator/(long divisor) const;

                Matrix& operator/=(long divisor);
        };
    }
}

#endif
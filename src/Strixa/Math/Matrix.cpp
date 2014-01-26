#include "../../../include/strixa/math/Matrix.h"

#include <assert.h>
#include <string.h>

using namespace strixa::math;

#pragma region Matrix Class Definition
    Matrix::Matrix(int rows,int columns)
    {
        assert(rows >= 1);
        assert(columns >= 1);

        this->data = new long[rows * columns];
        
        this->rows = rows;
        this->columns = columns;

        this->fill(0);
    }

    Matrix::Matrix(const Matrix& copy)
    {
        this->data = new long[copy.columns * copy.rows];
        memcpy(this->data,copy.data,copy.columns * copy.rows);

        this->rows = copy.rows;
        this->columns = copy.columns;
    }

    Matrix::Matrix(Matrix&& move)
    {
        this->data = move.data;
        move.data = NULL;

        this->rows = move.rows;
        this->columns = move.columns;
        move.rows = 0;
        move.columns = 0;
    }

    Matrix::~Matrix()
    {
        if (this->data != NULL) {
            delete this->data;
        }
    }

    long Matrix::at(int row,int column) const
    {
        assert(row > 0 && row < this->rows);
        assert(column > 0 && column < this->columns);

        return (*this)[row][column];
    }

    void Matrix::fill(long value)
    {
        const long* END = this->data + (this->rows * this->columns);


        for (long* cell = this->data;cell < END;++cell) {
            *cell = value;
        }
    }

    Matrix Matrix::transpose() const
    {
        Matrix result(this->columns,this->rows);


        for (int row = 0;row < this->rows;++row) {
            for (int column = 0;column < this->rows;++column) {
                result[column][row] = (*this)[row][column];
            }
        }

        return result;
    }

    long* Matrix::operator[](int row) const
    {
        return this->data + (row * this->columns);
    }

    Matrix& Matrix::operator=(const Matrix& operand)
    {
        delete[] this->data;
        this->data = new long[operand.columns * operand.rows];
        memcpy(this->data,operand.data,operand.columns * operand.rows);

        this->rows = operand.rows;
        this->columns = operand.columns;

        return *this;
    }

    Matrix Matrix::operator+(const Matrix& operand) const
    {
        return Matrix(*this) += operand;
    }

    Matrix& Matrix::operator+=(const Matrix& operand)
    {
        assert(this->rows == operand.rows);
        assert(this->columns == operand.columns);

        const long* END = this->data + (this->rows * this->columns);

        long* own_cell;
        long* op_cell;


        own_cell = this->data;
        op_cell = operand.data;
        while (own_cell < END) {
            *own_cell += *op_cell;

            ++own_cell;
            ++op_cell;
        }

        return *this;
    }

    Matrix Matrix::operator-(const Matrix& operand) const
    {
        assert(this->rows == operand.rows);
        assert(this->columns == operand.columns);

        return Matrix(*this) -= operand;
    }

    Matrix& Matrix::operator-=(const Matrix& operand)
    {
        assert(this->rows == operand.rows);
        assert(this->columns == operand.columns);

        const long* END = this->data + (this->rows * this->columns);

        long* own_cell;
        long* op_cell;


        own_cell = this->data;
        op_cell = operand.data;
        while (own_cell < END) {
            *own_cell -= *op_cell;

            ++own_cell;
            ++op_cell;
        }

        return *this;
    }

    Matrix Matrix::operator*(long scalar) const
    {
        return Matrix(*this) *= scalar;
    }

    Matrix& Matrix::operator*=(long scalar)
    {
        const long* END = this->data + (this->rows * this->columns);


        for (long* cell = this->data;cell < END;++cell) {
            *cell *= scalar;
        }

        return *this;
    }

    Matrix Matrix::operator*(const Matrix& operand) const
    {
        assert(this->columns == operand.rows);

        Matrix result(this->rows,operand.columns);


        for (int lo_row = 0;lo_row < this->rows;++lo_row) {
            for (int ro_column = 0;ro_column < operand.columns;++ro_column) {
                for (int ro_row = 0;ro_row < operand.rows;++ro_row) {
                    result[lo_row][ro_column] += (*this)[lo_row][ro_row] * operand[ro_row][ro_column];
                }
            }
        }

        return result;
    }

    Matrix& Matrix::operator*=(const Matrix& operand)
    {
        assert(this->columns == operand.rows);
        assert(this->rows == operand.columns);

        return *this = *this * operand;
    }

    Matrix Matrix::operator/(long divisor) const
    {
        return Matrix(*this) /= divisor;
    }

    Matrix& Matrix::operator/=(long divisor)
    {
        const long* END = this->data + (this->rows * this->columns);


        for (long* cell = this->data;cell < END;++cell) {
            *cell /= divisor;
        }

        return *this;
    }
#pragma endregion
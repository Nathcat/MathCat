#include <stdlib.h>
#include <iostream>

class Matrix {
    private:
        float* pDataBuffer;

        /**
         * Gets the index of certain coordinates in the data buffer.
         * @param x The x coordinate
         * @param y The y coordinate
         * @returns The buffer index of the given coordinates
         */
        int bufferIndexOf(int x, int y) {
            return (y * this->columns) + x;
        }

    public:
        int rows, columns;

        Matrix(int rows, int columns) {
            this->rows = rows;
            this->columns = columns;

            this->pDataBuffer = new float[rows * columns];
        }

        Matrix(float* data, int rows, int columns) {
            this->rows = rows;
            this->columns = columns;

            this->pDataBuffer = data;
        }

        /**
         * Get the value at index i in the data buffer.
         * @param i The index from the data buffer to retrieve
         * @return The float value at that index
         */
        float get(int i) {
            return *(pDataBuffer + i);
        }

        /**
         * Get the value from a given coordinate in the matrix
         * @param x x ordinate
         * @param y y ordinate
         * @return The float value at that coordinate
        */
        float get(int x, int y) {
            return *(pDataBuffer + this->bufferIndexOf(x, y));
        }

         /**
         * Sets the value of an index in the buffer.
         * @param i The index in the buffer to set
         * @param val The value to set the buffer index to
         */
        void set(int i, float val) {
            *(pDataBuffer + i) = val;
        }

        /**
         * Sets the value at a given coordinate on the matrix to a given value.
         * @param x x ordinate
         * @param y y ordinate
         * @param val The value to assign
         */
        void set(int x, int y, float val) {
            *(pDataBuffer + this->bufferIndexOf(x, y)) = val;
        }

        void displayToConsole() {
            for (int row = 0; row < this->rows; row++) {
                for (int column = 0; column < this->columns; column++) {
                    std::cout << this->get(column, row) << " ";
                }

                std::cout << std::endl;
            }
        }

        /**
         * Make an identity matrix of size NxN.
         * @returns Pointer to the result identity matrix.
         */
        static Matrix* makeIdentity(int N) {
            Matrix* result = new Matrix(N, N);
            
            for (int n = 0; n < N; n++) {
                result->set(n, n, 1);
            }

            return result;
        }

        /**
         * Adds two matrices together. They must have the same dimensions.
         * @param b The matrix to add to this matrix.
         * @returns A pointer to the sum of the two matrices.
         */
        Matrix* add(Matrix* b)  {
            if (b->rows != this->rows || b->columns != this->columns) {
                throw std::invalid_argument("Matrices must have the same dimensions.");
            }

            Matrix* result = new Matrix(this->rows, this->columns);

            for (int x = 0; x < result->columns; x++) {
                for (int y = 0; y < result->rows; y++) {
                    result->set(result->bufferIndexOf(x, y), this->get(x, y) + b->get(x, y));
                }
            }

            return result;
        }

        /**
         * Multiply this matrix with a scalar float.
         * @param b The scalar to multiply by.
         * @return The result of the multiplication.
         */
        Matrix* multiply(float b) {
            Matrix* result = new Matrix(this->rows, this->columns);

            for (int i = 0; i < this->rows * this->columns; i++) {
                result->set(i, this->get(i) * b);
            }

            return result;
        }

        /**
         * Multiply this matrix and another together, the two matrices must be of size MxN and NxM, and the result is NxN.
         * @param b The matrix to multiply this matrix by.
         * @returns A pointer to the resultant matrix.
        */
        Matrix* multiply(Matrix* b) {
            if (this->rows != b->columns || this->columns != b->rows) {
                throw std::invalid_argument("Matrices are of incompatible dimensions.");
            }

            Matrix* result = new Matrix(b->rows, this->columns);

            for (int row_in_a = 0; row_in_a < this->rows; row_in_a++) {
                for (int column_in_b = 0; column_in_b < b->columns; column_in_b++) {
                    float val = 0;

                    for (int i = 0; i < this->columns; i++) {
                        val += this->get(i, row_in_a) * b->get(column_in_b, i);
                    }

                    result->set(result->bufferIndexOf(column_in_b, row_in_a), val);
                }
            }

            return result;
        }

        /**
         * Transpose this matrix.
         * @returns A pointer to the matrix which is the tranposition of this matrix.
         */
        Matrix* transpose() {
            Matrix* result = new Matrix(this->columns, this->rows);

            for (int column = 0; column < this->columns; column++) {
                for (int row = 0; row < this->rows; row++) {
                    result->set(row, column, this->get(column, row));
                }
            }

            return result;
        }

        /**
         * Calculate the determinant of this matrix.
         * @returns A pointe to the floating point value of the determinant of this matrix, if one exists
        */
        static float determinant(Matrix* m) {
            if (m->rows != m->columns) {
                throw std::invalid_argument("Input matrix must be square!");
            }

            if (m->rows == 2) {
                return m->get(0, 0) * m->get(1, 1) - m->get(1, 0) * m->get(0, 1);
            }

            float result = 0;
            int multiplier = 1;
            for (int i = 0; i < m->columns; i++) {
                Matrix* sub_matrix = new Matrix(m->rows - 1, m->columns - 1);
                int sub_index = 0;

                for (int row = 1; row < m->rows; row++) {
                    for (int column = 0; column < m->columns; column++) {
                        if (column == i) {
                            continue;
                        }

                        sub_matrix->set(sub_index, m->get(column, row));
                        sub_index++;
                    }
                }

                result += multiplier * (m->get(i, 0) * determinant(sub_matrix));
                multiplier *= -1;
            }

            return result;
        }

        /**
         * Calculate the minor matrix of this matrix.
         * @returns A pointer to the minor matrix.
         */
        Matrix* minorMatrix() {
            Matrix* result = new Matrix(this->rows, this->columns);

            for (int row = 0; row < this->rows; row++) {
                for (int column = 0; column < this->columns; column++) {
                    Matrix* sub_matrix = new Matrix(this->rows - 1, this->columns - 1);
                    int sub_index = 0;

                    for (int sub_row = 0; sub_row < this->rows; sub_row++) {
                        if (sub_row == row) {
                            continue;
                        }

                        for (int sub_column = 0; sub_column < this->columns; sub_column++) {
                            if (sub_column == column) {
                                continue;
                            }

                            sub_matrix->set(sub_index, this->get(sub_column, sub_row));
                            sub_index++;
                        }
                    }

                    result->set(column, row, determinant(sub_matrix));
                }
            }

            return result;
        }

        /**
         * Calculates the cofactor matrix of this matrix.
         * @returns A pointer to the cofactor matrix of this matrix.
         */
        Matrix* cofactorMatrix() {
            Matrix* result = new Matrix(this->rows, this->columns);
            int originalMultiplier = 1;
            int multiplier = 1;

            for (int row = 0; row < this->rows; row++) {
                multiplier = originalMultiplier;

                for (int column = 0; column < this->columns; column++) {
                    result->set(column, row, multiplier * this->get(column, row));
                    multiplier *= -1;
                }

                originalMultiplier *= -1;
            }

            return result;
        }

        /**
         * Calculates the inverse of this matrix.
         * @returns A pointer to the inverse of this matrix.
         */
        Matrix* inverse() {
            return this->minorMatrix()->cofactorMatrix()->transpose()->multiply(1 / determinant(this));
        }

        /**
         * Find the dot product of two vector matrices.
         * @returns The dot product.
         */
        float dot(Matrix* b) {
            if (this->columns != 1 || b->columns != 1) {
                throw std::invalid_argument("At least one of the inputs is not a vector matrix.");
            }

            if (this->rows != b->rows) {
                throw std::invalid_argument("Input matrices must have the same dimensions.");
            }

            float result = 0;

            for (int i = 0; i < this->rows; i++) {
                result += this->get(i) * b->get(i);
            }

            return result;
        }        
};
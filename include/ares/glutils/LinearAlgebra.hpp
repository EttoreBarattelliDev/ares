/******************************************************************************/
/*!
 * @file
 * @author Ettore Barattelli
 * @copyright
 * This file is part of ARES, distributed under MIT license
 * \n\n
 * MIT License
 * \n\n
 * Copyright (c) 2023 Ettore Barattelli
 * \n\n
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * \n\n
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * \n\n
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

#ifndef LINEARALGEBRA_HPP_INCLUDED
#define LINEARALGEBRA_HPP_INCLUDED

#include <cstdint>
#include <cstring>
#include <cmath>
#include <vector>

namespace ares
{

namespace glutils
{
    /*!
     * @brief Template class for vector operations
     * 
     * This class implements a vector of float of up to 4 dimensions.
     * It implements common operators and methods for vector operations
     * like dot product, length, normalization.
     */
    template<size_t N>
    class Vec
    {
    public:
        /*!
         * @brief Default constructor. Initializes all entries to 0
         */
        Vec()
        {
            memset(m_data, 0, sizeof(m_data));
        }

        /*!
         * @brief Constructor to initialize 1 entry
         */
        Vec(float v0)
        {
            memset(m_data, 0, sizeof(m_data));
            m_data[0] = v0;
        }

        /*!
         * @brief Constructor to initialize 2 entries
         */
        Vec(float v0, float v1)
        {
            memset(m_data, 0, sizeof(m_data));
            m_data[0] = v0;
            m_data[1] = v1;
        }

        /*!
         * @brief Constructor to initialize 3 entries
         */
        Vec(float v0, float v1, float v2)
        {
            memset(m_data, 0, sizeof(m_data));
            m_data[0] = v0;
            m_data[1] = v1;
            m_data[2] = v2;
        }

        /*!
         * @brief Constructor to initialize 4 entries
         */
        Vec(float v0, float v1, float v2, float v3)
        {
            memset(m_data, 0, sizeof(m_data));
            m_data[0] = v0;
            m_data[1] = v1;
            m_data[2] = v2;
            m_data[3] = v3;
        }

        /*!
         * @brief Copy constructor
         */
        Vec(const Vec& rhs)
        {
            memcpy(m_data, rhs.m_data, sizeof(m_data));
        }

        /*!
         * @brief Assignment operator
         */
        Vec& operator=(const Vec& rhs)
        {
            memcpy(m_data, rhs.m_data, sizeof(m_data));
            return *this;
        }

        /*!
         * @brief Entry getter
         * 
         * @param[in] i - Index of entry
         * @return Value of requested entry
         */
        float& operator[](size_t i) { return m_data[i]; }

        /*!
         * @brief Entry getter
         * 
         * @param[in] i - Index of entry
         * @return Value of requested entry
         */
        const float& operator[](size_t i) const { return m_data[i]; }

        /*!
         * @brief Operator to sum 2 vectors
         * 
         * @param[in] rhs - Vector to sum
         * @return Result of the sum
         */
        Vec operator+(const Vec& rhs) const
        {
            Vec res;
            for (size_t i = 0; i < N; ++i)
            {
                res[i] = (m_data[i] + rhs.m_data[i]);
            }
            return res;
        }

        /*!
         * @brief Operator to sum and assign vector
         * 
         * @param[in] rhs - Vector to sum
         * @return Result of the sum
         */
        Vec& operator+=(const Vec& rhs)
        {
            for (size_t i = 0; i < N; ++i)
            {
                m_data[i] += rhs.m_data[i];
            }
            return *this;
        }

        /*!
         * @brief Operator to subtract 2 vectors
         * 
         * @param[in] rhs - Vector to subtract
         * @return Result of the subtraction
         */
        Vec operator-(const Vec& rhs) const
        {
            Vec res;
            for (size_t i = 0; i < N; ++i)
            {
                res[i] = (m_data[i] - rhs.m_data[i]);
            }
            return res;
        }

        /*!
         * @brief Operator to subtract and assign vector
         * 
         * @param[in] rhs - Vector to subtract
         * @return Result of the subtraction
         */
        Vec& operator-=(const Vec& rhs)
        {
            for (size_t i = 0; i < N; ++i)
            {
                m_data[i] -= rhs.m_data[i];
            }
            return *this;
        }

        /*!
         * @brief Operator to multiply 2 vectors
         * 
         * @param[in] rhs - Vector to multiply
         * @return Result of the multiplication
         */
        Vec operator*(const Vec& rhs) const
        {
            Vec res;
            for (size_t i = 0; i < N; ++i)
            {
                res[i] = (m_data[i] * rhs.m_data[i]);
            }
            return res;
        }

        /*!
         * @brief Operator to multiply and assign vector
         * 
         * @param[in] rhs - Vector to multiply
         * @return Result of the multiplication
         */
        Vec& operator*=(const Vec& rhs)
        {
            for (size_t i = 0; i < N; ++i)
            {
                m_data[i] *= rhs.m_data[i];
            }
            return *this;
        }

        /*!
         * @brief Operator to multiply vectors by a scalar
         * 
         * @param[in] rhs - Scalar to multiply
         * @return Result of the multiplication
         */
        Vec operator*(float rhs) const
        {
            Vec res;
            for (size_t i = 0; i < N; ++i)
            {
                res[i] = (m_data[i] * rhs);
            }
            return res;
        }

        /*!
         * @brief Operator to multiply by a scalar and assign vector
         * 
         * @param[in] rhs - Scalar to multiply
         * @return Result of the multiplication
         */
        Vec& operator*=(float rhs)
        {
            for (size_t i = 0; i < N; ++i)
            {
                m_data[i] *= rhs;
            }
            return *this;
        }
        /*!
         * @brief Operator to divide 2 vectors
         * 
         * @param[in] rhs - Vector to divide
         * @return Result of the division
         */
        Vec operator/(const Vec& rhs) const
        {
            Vec res;
            for (size_t i = 0; i < N; ++i)
            {
                res[i] = (m_data[i] / rhs.m_data[i]);
            }
            return res;
        }

        /*!
         * @brief Operator to divide and assign vector
         * 
         * @param[in] rhs - Vector to divide
         * @return Result of the division
         */
        Vec& operator/=(const Vec& rhs)
        {
            for (size_t i = 0; i < N; ++i)
            {
                m_data[i] /= rhs.m_data[i];
            }
            return *this;
        }

        /*!
         * @brief Operator to divide vectors by a scalar
         * 
         * @param[in] rhs - Scalar to divide
         * @return Result of the division
         */
        Vec operator/(float rhs) const
        {
            Vec res;
            for (size_t i = 0; i < N; ++i)
            {
                res[i] = (m_data[i] / rhs);
            }
            return res;
        }

        /*!
         * @brief Operator to divide by a scalar and assign vector
         * 
         * @param[in] rhs - Scalar to divide
         * @return Result of the division
         */
        Vec& operator/=(float rhs)
        {
            for (size_t i = 0; i < N; ++i)
            {
                m_data[i] /= rhs;
            }
            return *this;
        }

        /*!
         * @brief Calculates dot product with provided vector
         * 
         * @param[in] rhs - Vector on which to perform dot product
         * @return Result of the dot product
         */
        float dot(const Vec& rhs) const
        {
            float res = 0.f;
            for (size_t i = 0; i < N; ++i)
            {
                res += (m_data[i] * rhs.m_data[i]);
            }
            return res;
        }

        /*!
         * @brief Calculates vector length/magnitude
         * 
         * @return Length/magnitude of the vector
         */
        float length() const
        {
            return sqrtf(dot(*this));
        }

        /*!
         * @brief Normalizes the vector
         */
        void normalize()
        {
            float len = length();
            for (size_t i = 0; i < N; ++i)
            {
                m_data[i] /= len;
            }
        }

        /*!
         * @brief Raw data getter
         * 
         * @return Raw data
         */
        const float* const_data() const { return m_data; }

        /*!
         * @brief Raw data getter
         * 
         * @return Raw data
         */
        float* data() { return m_data; }

    private:
        /*! Vector data */
        float m_data[N];
    };

    using Vec2 = Vec<2>;
    using Vec3 = Vec<3>;
    using Vec4 = Vec<4>;


    /*!
     * @brief Template class for matrix operations
     * 
     * This class implements a square matrix of float of up to 4 dimensions.
     * It implements common operators and methods for vector operations
     * like product, transpose, etc.
     * The matrix is represented as column-major, as per common OpenGL usage.
     */
    template<size_t N>
    class Mat
    {
    public:
        /*!
         * @brief Default constructor. Initializes all entries to 0
         */
        Mat()
        {
            memset(m_data, 0, sizeof(m_data));
        }

        /*!
         * @brief Constructor from raw data
         * 
         * @param[in] m - Raw matrix to copy. **NOTE** the raw data is considered row-major
         */
        Mat(const float* m)
        {
            memset(m_data, 0, sizeof(m_data));
            for (size_t c = 0; c < N; ++c)
            {
                for (size_t r = 0; r < N; ++r)
                {
                    m_data[c][r] = m[r * N + c];
                }
            }
        }

        /*!
         * @brief Constructor from raw data
         * 
         * @param[in] m - Raw matrix to copy. **NOTE** the raw data is considered row-major
         */
        Mat(const float m[N][N])
        {
            memset(m_data, 0, sizeof(m_data));
            for (size_t c = 0; c < N; ++c)
            {
                for (size_t r = 0; r < N; ++r)
                {
                    m_data[c][r] = m[r][c];
                }
            }
        }

        /*!
         * @brief Constructor from raw data
         * 
         * @param[in] m - Raw matrix to copy. **NOTE** the raw data is considered row-major
         */
        Mat(const std::vector<double> m)
        {
            memset(m_data, 0, sizeof(m_data));
            for (size_t c = 0; c < N; ++c)
            {
                for (size_t r = 0; r < N; ++r)
                {
                    m_data[c][r] = m[r * N + c];
                }
            }
        }

        /*!
         * @brief Copy constructor
         */
        Mat(const Mat& rhs)
        {
            memcpy(m_data, rhs.m_data, sizeof(m_data));
        }

        /*!
         * @brief Assignment operator
         */
        Mat& operator=(const Mat& rhs)
        {
            memcpy(m_data, rhs.m_data, sizeof(m_data));
            return *this;
        }

        /*!
         * @brief Assignment operator from raw data
         * 
         * @param[in] m - Raw matrix to copy. **NOTE** the raw data is considered row-major
         */
        Mat& operator=(const float* m)
        {
            memset(m_data, 0, sizeof(m_data));
            for (size_t c = 0; c < N; ++c)
            {
                for (size_t r = 0; r < N; ++r)
                {
                    m_data[c][r] = m[r * N + c];
                }
            }
            return *this;
        }

        /*!
         * @brief Assignment operator from raw data
         * 
         * @param[in] m - Raw matrix to copy. **NOTE** the raw data is considered row-major
         */
        Mat& operator=(const float m[N][N])
        {
            memset(m_data, 0, sizeof(m_data));
            for (size_t c = 0; c < N; ++c)
            {
                for (size_t r = 0; r < N; ++r)
                {
                    m_data[c][r] = m[r][c];
                }
            }
            return *this;
        }

        /*!
         * @brief Row getter
         * 
         * @param[in] i - Index of row
         * @return Vector with the requested row data
         */
        Vec<N> row(size_t i) const
        {
            Vec<N> res;
            for (size_t c = 0; c < N; ++c)
            {
                res[c] = m_data[c][i];
            }
            return res;
        }

        /*!
         * @brief Column getter
         * 
         * @param[in] i - Index of column
         * @return Vector with the requested column data
         */
        Vec<N> column(size_t i) const
        {
            Vec<N> res;
            for (size_t r = 0; r < N; ++r)
            {
                res[r] = m_data[i][r];
            }
            return res;
        }

        /*!
         * @brief Sets the matrix as an identity matrix
         */
        void setIdentity()
        {
            for (size_t c = 0; c < N; ++c)
            {
                for (size_t r = 0; r < N; ++r)
                {
                    m_data[c][r] = (c == r) ? (1.f) : (0.f);
                }
            }
        }

        /*!
         * @brief Entry setter
         * 
         * @param[in] r - Row index
         * @param[in] c - Column index
         * @param[in] v - Value to set
         */
        void set(size_t r, size_t c, float v)
        {
            m_data[c][r] = v;
        }

        /*!
         * @brief Operator to sum a matrix
         * 
         * @param[in] rhs - Matrix to sum
         * @return Result of the sum
         */
        Mat operator+(const Mat& rhs) const
        {
            Mat res;
            for (size_t c = 0; c < N; ++c)
            {
                for (size_t r = 0; r < N; ++r)
                {
                    res.m_data[c][r] = (m_data[c][r] + rhs.m_data[c][r]);
                }
            }
            return res;
        }

        /*!
         * @brief Operator to sum and assign matrix
         * 
         * @param[in] rhs - Matrix to sum
         * @return Result of the sum
         */
        Mat& operator+=(const Mat& rhs)
        {
            for (size_t c = 0; c < N; ++c)
            {
                for (size_t r = 0; r < N; ++r)
                {
                    m_data[c][r] += rhs.m_data[c][r];
                }
            }
            return *this;
        }

        /*!
         * @brief Operator to subtract a matrix
         * 
         * @param[in] rhs - Matrix to subtract
         * @return Result of the subtraction
         */
        Mat operator-(const Mat& rhs) const
        {
            Mat res;
            for (size_t c = 0; c < N; ++c)
            {
                for (size_t r = 0; r < N; ++r)
                {
                    res.m_data[c][r] = (m_data[c][r] - rhs.m_data[c][r]);
                }
            }
            return res;
        }

        /*!
         * @brief Operator to subtract and assign matrix
         * 
         * @param[in] rhs - Matrix to subtract
         * @return Result of the subtraction
         */
        Mat& operator-=(const Mat& rhs)
        {
            for (size_t c = 0; c < N; ++c)
            {
                for (size_t r = 0; r < N; ++r)
                {
                    m_data[c][r] -= rhs.m_data[c][r];
                }
            }
            return *this;
        }

        /*!
         * @brief Operator to multiply a matrix by a vector
         * 
         * @param[in] rhs - Matrix to multiply
         * @return Result of the multiplication
         */
        Vec<N> operator*(const Vec<N>& rhs) const
        {
            Vec<N> res;
            for (size_t r = 0; r < N; ++r)
            {
                for (size_t c = 0; c < N; ++c)
                {
                    res[r] += (m_data[c][r] * rhs[c]);
                }
            }
            return res;
        }

        /*!
         * @brief Operator to multiply a matrix
         * 
         * @param[in] rhs - Matrix to multiply
         * @return Result of the multiplication
         */
        Mat operator*(const Mat& rhs) const
        {
            Mat res;
            for (size_t c = 0; c < N; ++c)
            {
                for (size_t r = 0; r < N; ++r)
                {
                    for (size_t i = 0; i < N; i++)
                    {
                        res.m_data[c][r] += (m_data[i][r] * rhs.m_data[c][i]);
                    }
                }
            }
            return res;
        }

        /*!
         * @brief Operator to multiply and assign matrix
         * 
         * @param[in] rhs - Matrix to multiply
         * @return Result of the multiplication
         */
        Mat& operator*=(const Mat& rhs)
        {
            Mat tmp(*this);
            memset(m_data, 0, sizeof(m_data));
            for (size_t c = 0; c < N; ++c)
            {
                for (size_t r = 0; r < N; ++r)
                {
                    for (size_t i = 0; i < N; i++)
                    {
                        m_data[c][r] += (tmp.m_data[i][r] * rhs.m_data[c][i]);
                    }
                }
            }
            return *this;
        }

        /*!
         * @brief Transposes the matrix
         * 
         * @return Transposed matrix
         */
        Mat& transpose()
        {
            Mat tmp(*this);
            for (size_t c = 0; c < N; ++c)
            {
                for (size_t r = 0; r < N; ++r)
                {
                    m_data[c][r] = tmp.m_data[r][c];
                }
            }
            return *this;
        }

        /*!
         * @brief Inverts the matrix
         * 
         * NOTE: only available for Mat4 template
         * 
         * @return Inverted matrix
         */
        Mat& invert();

        /*!
         * @brief Gets the translation information from the matrix
         * 
         * NOTE: only available for Mat4 template
         * 
         * @return Translation vector
         */
        Vec3 translation() const;

        /*!
         * @brief Matrix translation
         * 
         * NOTE: only available for Mat4 template
         * 
         * @param[in] x - X position
         * @param[in] y - Y position
         * @param[in] z - Z position
         */
        void translate(float x, float y, float z);

        /*!
         * @brief Matrix translation in local coordinate system on the XZ plane
         * 
         * NOTE: only available for Mat4 template
         * 
         * @param[in] x - X position
         * @param[in] z - Z position
         */
        void translateLocalXZ(float x, float z);

        /*!
         * @brief Matrix rotation around X axis
         * 
         * NOTE: only available for Mat4 template
         * 
         * @param[in] x - rotation angle (rad)
         */
        void rotateX(float x);

        /*!
         * @brief Matrix rotation around Y axis
         * 
         * NOTE: only available for Mat4 template
         * 
         * @param[in] y - rotation angle (rad)
         */
        void rotateY(float y);

        /*!
         * @brief Matrix rotation around Z axis
         * 
         * NOTE: only available for Mat4 template
         * 
         * @param[in] z - rotation angle (rad)
         */
        void rotateZ(float z);

        /*!
         * @brief Matrix rotation around X, Y, Z axis
         * 
         * NOTE: only available for Mat4 template
         * 
         * @param[in] x - X rotation angle (rad)
         * @param[in] y - Y rotation angle (rad)
         * @param[in] z - Z rotation angle (rad)
         */
        void rotateXYZ(float x, float y, float z);

        /*!
         * @brief Matrix rotation using quaternions
         * 
         * NOTE: only available for Mat4 template
         * 
         * @param[in] x - Quaternion X
         * @param[in] y - Quaternion Y
         * @param[in] z - Quaternion Z
         * @param[in] w - Quaternion W
         */
        void rotateXYZW(float x, float y, float z, float w);

        /*!
         * @brief Matrix scaling
         * 
         * NOTE: only available for Mat4 template
         * 
         * @param[in] x - X scaling
         * @param[in] y - Y scaling
         * @param[in] z - Z scaling
         */
        void scale(float x, float y, float z);

        /*!
         * @brief Raw data getter
         * 
         * @return Raw data
         */
        const float* const_data() const { return reinterpret_cast<const float*>(m_data); }

        /*!
         * @brief Raw data getter
         * 
         * @return Raw data
         */
        float* data() { return reinterpret_cast<float*>(m_data); }

    private:
        /*! Matrix data */
        float m_data[N][N];
    };

    using Mat2 = Mat<2>;
    using Mat3 = Mat<3>;
    using Mat4 = Mat<4>;

    /*!
     * @brief Conversion function from Euler angles to quaternion
     * 
     * @param[in] euler - Euler angles
     * @return Quaternion for the input angle
     */
    Vec4 eulerToQuaternion(const Vec3& euler);
}

}

#endif

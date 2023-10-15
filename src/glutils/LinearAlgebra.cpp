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

#include "ares/glutils/LinearAlgebra.hpp"

namespace ares
{

namespace glutils
{
    template<>
    Vec3 Mat4::translation() const
    {
        return Vec3(m_data[3][0], m_data[3][1], m_data[3][2]);
    }

    template<>
    void Mat4::translate(float x, float y, float z)
    {
        /* Transform matrix */
        float m[4][4] =
        {
            { 1.F, 0.F, 0.F, x   },
            { 0.F, 1.F, 0.F, y   },
            { 0.F, 0.F, 1.F, z   },
            { 0.F, 0.F, 0.F, 1.F }
        };

        /* Apply */
        Mat4 tmp(m);
        *this = tmp * *this;
    }

    template<>
    void Mat4::translateLocalXZ(float x, float z)
    {
        /* Transform matrix */
        float m[4][4] =
        {
            { 1.F, 0.F, 0.F, x   },
            { 0.F, 1.F, 0.F, 0.f },
            { 0.F, 0.F, 1.F, z   },
            { 0.F, 0.F, 0.F, 1.F }
        };

        /* Apply and preserve Y */
        Mat4 tmp(m);
        float y = m_data[3][1];
        *this = *this * tmp;
        m_data[3][1] = y;
    }

    template<>
    void Mat4::rotateX(float x)
    {
        float cosx = cosf(x);
        float sinx = sinf(x);

        /* Transform matrix */
        float m[4][4] =
        {
            { 1.F, 0.F , 0.F  , 0.F },
            { 0.F, cosx, -sinx, 0.F },
            { 0.F, sinx, cosx , 0.F },
            { 0.F, 0.F , 0.F  , 1.F }
        };

        /* Apply */
        Mat4 tmp(m);
        *this = tmp * *this;
    }

    template<>
    void Mat4::rotateY(float y)
    {
        float cosy = cosf(y);
        float siny = sinf(y);

        /* Transform matrix */
        float m[4][4] =
        {
            { cosy , 0.F, siny, 0.F },
            { 0.F  , 1.F, 0.F , 0.F },
            { -siny, 0.F, cosy, 0.F },
            { 0.F  , 0.F, 0.F , 1.F }
        };

        /* Apply */
        Mat4 tmp(m);
        *this = tmp * *this;
    }

    template<>
    void Mat4::rotateZ(float z)
    {
        float cosz = cosf(z);
        float sinz = sinf(z);

        /* Transform matrix */
        float m[4][4] =
        {
            { cosz, -sinz, 0.F, 0.F },
            { sinz, cosz , 0.F, 0.F },
            { 0.F , 0.F  , 1.F, 0.F },
            { 0.F , 0.F  , 0.F, 1.F }
        };

        /* Apply */
        Mat4 tmp(m);
        *this = tmp * *this;
    }

    template<>
    void Mat4::rotateXYZ(float x, float y, float z)
    {
        float cx = cosf(x);
        float sx = sinf(x);
        float cy = cosf(y);
        float sy = sinf(y);
        float cz = cosf(z);
        float sz = sinf(z);

        /* Transform matrix */
        float m[4][4] =
        {
            { cz * cy               , cz * sy * sx - sz * cx, cz * sy * cx + sz * sx, 0.F },
            { sz * cy               , sz * sy * sx + cz * cx, sz * sy * cx - cz * sx, 0.F },
            { -sy                   , cy * sx               , cy * cx               , 0.F },
            { 0.F                   , 0.F                   , 0.F                   , 1.F }
        };

        /* Apply */
        Mat4 tmp(m);
        *this = tmp * *this;

    }

    template<>
    void Mat4::rotateXYZW(float x, float y, float z, float w)
    {
        float x2 = x * x;
        float y2 = y * y;
        float z2 = z * z;
        float w2 = w * w;

        /* Transform matrix */
        float m[4][4] =
        {
            { 1 - 2 * y2 - 2 * z2   , 2 * x * y - 2 * z * w, 2 * x * z + 2 * y * w, 0.F },
            { 2 * x * y + 2 * z * w , 1 - 2 * x2 - 2 * z2  , 2 * y * z - 2 * x * w, 0.F },
            { 2 * x * z - 2 * y * w , 2 * y * z + 2 * x * w, 1 - 2 * x2 -  2 * y2 , 0.F },
            { 0.F                   , 0.F                  , 0.F                  , 1.F }
        };

        /* Apply */
        Mat4 tmp(m);
        //tmp.transpose();
        *this = tmp * *this;

    }

    template<>
    void Mat4::scale(float x, float y, float z)
    {
        /* Transform matrix */
        float m[4][4] =
        {
            { x  , 0.F, 0.F, 0.F },
            { 0.F, y  , 0.F, 0.F },
            { 0.F, 0.F, z  , 0.F },
            { 0.F, 0.F, 0.F, 1.F }
        };

        /* Apply */
        Mat4 tmp(m);
        *this = tmp * *this;
    }

    template<>
    Mat4& Mat4::invert()
    {
        Mat4 res;

        res.m_data[0][0] = m_data[1][1] * m_data[2][2] * m_data[3][3] - 
                    m_data[1][1] * m_data[2][3] * m_data[3][2] - 
                    m_data[2][1] * m_data[1][2] * m_data[3][3] + 
                    m_data[2][1] * m_data[1][3] * m_data[3][2] +
                    m_data[3][1] * m_data[1][2] * m_data[2][3] - 
                    m_data[3][1] * m_data[1][3] * m_data[2][2];

        res.m_data[1][0] = -m_data[1][0] * m_data[2][2] * m_data[3][3] + 
                     m_data[1][0] * m_data[2][3] * m_data[3][2] + 
                     m_data[2][0] * m_data[1][2] * m_data[3][3] - 
                     m_data[2][0] * m_data[1][3] * m_data[3][2] - 
                     m_data[3][0] * m_data[1][2] * m_data[2][3] + 
                     m_data[3][0] * m_data[1][3] * m_data[2][2];

        res.m_data[2][0] = m_data[1][0] * m_data[2][1] * m_data[3][3] - 
                    m_data[1][0] * m_data[2][3] * m_data[3][1] - 
                    m_data[2][0] * m_data[1][1] * m_data[3][3] + 
                    m_data[2][0] * m_data[1][3] * m_data[3][1] + 
                    m_data[3][0] * m_data[1][1] * m_data[2][3] - 
                    m_data[3][0] * m_data[1][3] * m_data[2][1];

        res.m_data[3][0] = -m_data[1][0] * m_data[2][1] * m_data[3][2] + 
                     m_data[1][0] * m_data[2][2] * m_data[3][1] +
                     m_data[2][0] * m_data[1][1] * m_data[3][2] - 
                     m_data[2][0] * m_data[1][2] * m_data[3][1] - 
                     m_data[3][0] * m_data[1][1] * m_data[2][2] + 
                     m_data[3][0] * m_data[1][2] * m_data[2][1];

        res.m_data[0][1] = -m_data[0][1] * m_data[2][2] * m_data[3][3] + 
                     m_data[0][1] * m_data[2][3] * m_data[3][2] + 
                     m_data[2][1] * m_data[0][2] * m_data[3][3] - 
                     m_data[2][1] * m_data[0][3] * m_data[3][2] - 
                     m_data[3][1] * m_data[0][2] * m_data[2][3] + 
                     m_data[3][1] * m_data[0][3] * m_data[2][2];

        res.m_data[1][1] = m_data[0][0] * m_data[2][2] * m_data[3][3] - 
                    m_data[0][0] * m_data[2][3] * m_data[3][2] - 
                    m_data[2][0] * m_data[0][2] * m_data[3][3] + 
                    m_data[2][0] * m_data[0][3] * m_data[3][2] + 
                    m_data[3][0] * m_data[0][2] * m_data[2][3] - 
                    m_data[3][0] * m_data[0][3] * m_data[2][2];

        res.m_data[2][1] = -m_data[0][0] * m_data[2][1] * m_data[3][3] + 
                     m_data[0][0] * m_data[2][3] * m_data[3][1] + 
                     m_data[2][0] * m_data[0][1] * m_data[3][3] - 
                     m_data[2][0] * m_data[0][3] * m_data[3][1] - 
                     m_data[3][0] * m_data[0][1] * m_data[2][3] + 
                     m_data[3][0] * m_data[0][3] * m_data[2][1];

        res.m_data[3][1] = m_data[0][0] * m_data[2][1] * m_data[3][2] - 
                    m_data[0][0] * m_data[2][2] * m_data[3][1] - 
                    m_data[2][0] * m_data[0][1] * m_data[3][2] + 
                    m_data[2][0] * m_data[0][2] * m_data[3][1] + 
                    m_data[3][0] * m_data[0][1] * m_data[2][2] - 
                    m_data[3][0] * m_data[0][2] * m_data[2][1];

        res.m_data[0][2] = m_data[0][1] * m_data[1][2] * m_data[3][3] - 
                    m_data[0][1] * m_data[1][3] * m_data[3][2] - 
                    m_data[1][1] * m_data[0][2] * m_data[3][3] + 
                    m_data[1][1] * m_data[0][3] * m_data[3][2] + 
                    m_data[3][1] * m_data[0][2] * m_data[1][3] - 
                    m_data[3][1] * m_data[0][3] * m_data[1][2];

        res.m_data[1][2] = -m_data[0][0] * m_data[1][2] * m_data[3][3] + 
                     m_data[0][0] * m_data[1][3] * m_data[3][2] + 
                     m_data[1][0] * m_data[0][2] * m_data[3][3] - 
                     m_data[1][0] * m_data[0][3] * m_data[3][2] - 
                     m_data[3][0] * m_data[0][2] * m_data[1][3] + 
                     m_data[3][0] * m_data[0][3] * m_data[1][2];

        res.m_data[2][2] = m_data[0][0] * m_data[1][1] * m_data[3][3] - 
                    m_data[0][0] * m_data[1][3] * m_data[3][1] - 
                    m_data[1][0] * m_data[0][1] * m_data[3][3] + 
                    m_data[1][0] * m_data[0][3] * m_data[3][1] + 
                    m_data[3][0] * m_data[0][1] * m_data[1][3] - 
                    m_data[3][0] * m_data[0][3] * m_data[1][1];

        res.m_data[3][2] = -m_data[0][0] * m_data[1][1] * m_data[3][2] + 
                     m_data[0][0] * m_data[1][2] * m_data[3][1] + 
                     m_data[1][0] * m_data[0][1] * m_data[3][2] - 
                     m_data[1][0] * m_data[0][2] * m_data[3][1] - 
                     m_data[3][0] * m_data[0][1] * m_data[1][2] + 
                     m_data[3][0] * m_data[0][2] * m_data[1][1];

        res.m_data[0][3] = -m_data[0][1] * m_data[1][2] * m_data[2][3] + 
                     m_data[0][1] * m_data[1][3] * m_data[2][2] + 
                     m_data[1][1] * m_data[0][2] * m_data[2][3] - 
                     m_data[1][1] * m_data[0][3] * m_data[2][2] - 
                     m_data[2][1] * m_data[0][2] * m_data[1][3] + 
                     m_data[2][1] * m_data[0][3] * m_data[1][2];

        res.m_data[1][3] = m_data[0][0] * m_data[1][2] * m_data[2][3] - 
                    m_data[0][0] * m_data[1][3] * m_data[2][2] - 
                    m_data[1][0] * m_data[0][2] * m_data[2][3] + 
                    m_data[1][0] * m_data[0][3] * m_data[2][2] + 
                    m_data[2][0] * m_data[0][2] * m_data[1][3] - 
                    m_data[2][0] * m_data[0][3] * m_data[1][2];

        res.m_data[2][3] = -m_data[0][0] * m_data[1][1] * m_data[2][3] + 
                     m_data[0][0] * m_data[1][3] * m_data[2][1] + 
                     m_data[1][0] * m_data[0][1] * m_data[2][3] - 
                     m_data[1][0] * m_data[0][3] * m_data[2][1] - 
                     m_data[2][0] * m_data[0][1] * m_data[1][3] + 
                     m_data[2][0] * m_data[0][3] * m_data[1][1];

        res.m_data[3][3] = m_data[0][0] * m_data[1][1] * m_data[2][2] - 
                    m_data[0][0] * m_data[1][2] * m_data[2][1] - 
                    m_data[1][0] * m_data[0][1] * m_data[2][2] + 
                    m_data[1][0] * m_data[0][2] * m_data[2][1] + 
                    m_data[2][0] * m_data[0][1] * m_data[1][2] - 
                    m_data[2][0] * m_data[0][2] * m_data[1][1];

        float det = m_data[0][0] * res.m_data[0][0] + m_data[0][1] * res.m_data[1][0] + m_data[0][2] * res.m_data[2][0] + m_data[0][3] * res.m_data[3][0];

        if (det != 0.F)
        {
            det = 1.F / det;
            for (size_t c = 0; c < 4; ++c)
            {
                for (size_t r = 0; r < 4; ++r)
                {
                    m_data[c][r] = res.m_data[c][r] * det;
                }
            }
        }

        return *this;
    }

    Vec4 eulerToQuaternion(const Vec3& euler)
    {
        Vec4 retval;

        float halfX = euler[0] / 2.F;
        float halfY = euler[1] / 2.F;
        float halfZ = euler[2] / 2.F;
        float cosHalfX = cosf(halfX);
        float cosHalfY = cosf(halfY);
        float cosHalfZ = cosf(halfZ);
        float sinHalfX = sinf(halfX);
        float sinHalfY = sinf(halfY);
        float sinHalfZ = sinf(halfZ);

        retval[3] = cosHalfX * cosHalfY * cosHalfZ + sinHalfX * sinHalfY * sinHalfZ;
        retval[0] = sinHalfX * cosHalfY * cosHalfZ - cosHalfX * sinHalfY * sinHalfZ;
        retval[1] = cosHalfX * sinHalfY * cosHalfZ + sinHalfX * cosHalfY * sinHalfZ;
        retval[2] = cosHalfX * cosHalfY * sinHalfZ - sinHalfX * sinHalfY * cosHalfZ;

        return retval;
    }
}

}

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

#include "ares/core/PerspectiveCamera.hpp"

#include <cmath>

namespace ares
{

namespace core
{
    PerspectiveCamera::PerspectiveCamera(float aspectRatio, float yfov, float znear, float zfar)
        : Camera()
        , m_aspectRatio(aspectRatio)
        , m_yfov(yfov)
        , m_znear(znear)
        , m_zfar(zfar)
    {
        /* Update projection matrix */
        updateProjectionMatrix();
    }

    void PerspectiveCamera::updateProjectionMatrix()
    {
        if (m_zfar > 0)
        {
            /* Finite perspective */
            float tanHalfFov = tanf(m_yfov * .5F);
            float nminusf = m_znear - m_zfar;

            /* Projection matrix definition */
            float m[4][4] =
            {
                { 1.F / (m_aspectRatio * tanHalfFov), 0.F             , 0.F                         , 0.F                                },
                { 0.F                               , 1.F / tanHalfFov, 0.F                         , 0.F                                },
                { 0.F                               , 0.F             , (m_zfar + m_znear) / nminusf, (2.F * m_zfar * m_znear) / nminusf },
                { 0.F                               , 0.F             , -1.F                        , 0.F                                }
            };

            /* Store matrix */
            m_projectionMatrix = m;
        }
        else
        {
            /* Infinite perspective */
            float tanHalfFov = tanf(m_yfov * .5F);

            /* Projection matrix definition */
            float m[4][4] =
            {
                { 1.F / (m_aspectRatio * tanHalfFov), 0.F             , 0.F , 0.F            },
                { 0.F                               , 1.F / tanHalfFov, 0.F , 0.F            },
                { 0.F                               , 0.F             , -1.F, -2.F * m_znear },
                { 0.F                               , 0.F             , -1.F, 0.F            }
            };

            /* Store matrix */
            m_projectionMatrix = m;
        }
    }
}

}

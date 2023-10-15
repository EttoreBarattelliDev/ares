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

#include "ares/glutils/PngLoader.hpp"

#include <stdexcept>
#include <fstream>
#include <png.h>

namespace ares
{

namespace glutils
{

namespace PngLoader
{

    static Image::Format loc_pngFormatToImageFormat(int32_t pngFormat)
    {
        /* Assume failure */
        Image::Format retval = Image::Format::Invalid;

        /* Map png format to Image format */
        switch (pngFormat)
        {
            case PNG_COLOR_TYPE_RGB:
                retval = Image::Format::RGB;
                break;
            case PNG_COLOR_TYPE_RGBA:
                retval = Image::Format::RGBA;
                break;
            default:
                throw std::runtime_error("Unsupported PNG image format");
        }

        return retval;
    }

    ImagePtr loadPng(const std::string& filename, bool flip)
    {
        /* Assume failure */
        ImagePtr retval = nullptr;

        /* Open file */
        FILE *fp = fopen(filename.c_str(), "rb");
        if (nullptr == fp)
        {
            throw std::runtime_error("[PngLoader::loadPng] File " + filename + " could not be opened for reading");
        }

        /* Read header and check it's a PNG file */
        uint8_t header[8];
        fread(header, 1, 8, fp);
        if (png_sig_cmp((png_const_bytep)header, 0, 8))
        {
            throw std::runtime_error("[PngLoader::loadPng] File " + filename + " is not recognized as a PNG file");
        }

        /* Create png read struct */
        png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (nullptr == pngPtr)
        {
            throw std::runtime_error("[PngLoader::loadPng] png_create_read_struct failed");
        }

        /* Create info struct */
        png_infop infoPtr = png_create_info_struct(pngPtr);
        if (nullptr == infoPtr)
        {
            throw std::runtime_error("[PngLoader::loadPng] png_create_info_struct failed");
        }

        /* Set error handler */
        if (setjmp(png_jmpbuf(pngPtr)))
        {
            png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
            throw std::runtime_error("[PngLoader::loadPng] Error during init_io");
        }

        /* Initialize and get info */
        png_init_io(pngPtr, fp);
        png_set_sig_bytes(pngPtr, 8);
        png_read_info(pngPtr, infoPtr);

        int32_t width     = png_get_image_width(pngPtr, infoPtr);
        int32_t height    = png_get_image_height(pngPtr, infoPtr);
        int32_t colorType = png_get_color_type(pngPtr, infoPtr);
        int32_t bitDepth  = png_get_bit_depth(pngPtr, infoPtr);

        /* Check for valid bit depth */
        if (bitDepth != 8)
        {
            throw std::runtime_error("[PngLoader::loadPng] Unsupported bit depth");
        }

        /* Create buffers for row and image data */
        int32_t nbytes = png_get_rowbytes(pngPtr, infoPtr);
        png_bytep rowPtr = (png_bytep)png_malloc(pngPtr, nbytes);
        std::vector<uint8_t> imgData(nbytes * height);

        /* Set error handler */
        if (setjmp(png_jmpbuf(pngPtr)))
        {
            /* Free resources */
            png_free(pngPtr, rowPtr);
            png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
            fclose(fp);
            throw std::runtime_error("[PngLoader::loadPng] Error during image reading");
        }

        /* Read all image data */
        for (int32_t r = 0; r < height; ++r)
        {
            /* Read row */
            png_read_row(pngPtr, rowPtr, NULL);
            for (int32_t c = 0; c < nbytes; c++)
            {
                int32_t rowIdx = (flip) ? (height - r - 1) : (r);
                imgData[rowIdx * nbytes + c] = rowPtr[c];
            }
        }

        /* Free row buffer */
        png_free(pngPtr, rowPtr);

        /* Read the rest of the file */
        png_read_end(pngPtr, infoPtr);
        
        /* Clean up and free resources */
        png_destroy_read_struct(&pngPtr, &infoPtr, NULL);

        /* Close the file */
        fclose(fp);

        /* Create image object */
        retval = std::make_shared<Image>(imgData, loc_pngFormatToImageFormat(colorType), width, height);
        return retval;
    }

}

}

}

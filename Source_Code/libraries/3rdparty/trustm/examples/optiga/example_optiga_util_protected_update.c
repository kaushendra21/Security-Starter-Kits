/**
* \copyright
* MIT License
*
* Copyright (c) 2019 Infineon Technologies AG
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE
*
* \endcopyright
*
* \author Infineon Technologies AG
*
* \file example_optiga_util_protected_update.c
*
* \brief   This file provides the example for util protected update operation using #optiga_util_protected_update_start,
*           #optiga_util_protected_update_continue & #optiga_util_protected_update_final.
*
* \ingroup grOptigaExamples
*
* @{
*/

#include "optiga/optiga_util.h"
#include "optiga_example.h"

/**
 * Callback when optiga_util_xxxx operation is completed asynchronously
 */
static volatile optiga_lib_status_t optiga_lib_status;
//lint --e{818} suppress "argument "context" is not used in the sample provided"
static void optiga_util_callback(void * context, optiga_lib_status_t return_status)
{
    optiga_lib_status = return_status;
    if (NULL != context)
    {
        // callback to upper layer here
    }
}

/**
 * Sample Trust Anchor with ECC-256 based certificate
 */
static uint8_t trust_anchor [] = 
{
    0x30, 0x82, 0x02, 0x58, 0x30, 0x82, 0x01, 0xFF, 0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x01, 0x2F,
    0x30, 0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x02, 0x30, 0x56, 0x31, 0x0B,
    0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x49, 0x4E, 0x31, 0x0D, 0x30, 0x0B, 0x06,
    0x03, 0x55, 0x04, 0x0A, 0x0C, 0x04, 0x49, 0x46, 0x49, 0x4E, 0x31, 0x0C, 0x30, 0x0A, 0x06, 0x03,
    0x55, 0x04, 0x0B, 0x0C, 0x03, 0x43, 0x43, 0x53, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04,
    0x03, 0x0C, 0x0A, 0x49, 0x6E, 0x74, 0x43, 0x41, 0x20, 0x50, 0x32, 0x35, 0x36, 0x31, 0x15, 0x30,
    0x13, 0x06, 0x03, 0x55, 0x04, 0x2E, 0x13, 0x0C, 0x54, 0x72, 0x75, 0x73, 0x74, 0x20, 0x41, 0x6E,
    0x63, 0x68, 0x6F, 0x72, 0x30, 0x1E, 0x17, 0x0D, 0x31, 0x36, 0x30, 0x35, 0x32, 0x36, 0x30, 0x38,
    0x30, 0x31, 0x33, 0x37, 0x5A, 0x17, 0x0D, 0x31, 0x37, 0x30, 0x36, 0x30, 0x35, 0x30, 0x38, 0x30,
    0x31, 0x33, 0x37, 0x5A, 0x30, 0x5A, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13,
    0x02, 0x49, 0x4E, 0x31, 0x0D, 0x30, 0x0B, 0x06, 0x03, 0x55, 0x04, 0x0A, 0x0C, 0x04, 0x49, 0x46,
    0x49, 0x4E, 0x31, 0x0C, 0x30, 0x0A, 0x06, 0x03, 0x55, 0x04, 0x0B, 0x0C, 0x03, 0x43, 0x43, 0x53,
    0x31, 0x17, 0x30, 0x15, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C, 0x0E, 0x65, 0x6E, 0x64, 0x45, 0x6E,
    0x74, 0x69, 0x74, 0x79, 0x20, 0x50, 0x32, 0x35, 0x36, 0x31, 0x15, 0x30, 0x13, 0x06, 0x03, 0x55,
    0x04, 0x2E, 0x13, 0x0C, 0x54, 0x72, 0x75, 0x73, 0x74, 0x20, 0x41, 0x6E, 0x63, 0x68, 0x6F, 0x72,
    0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02, 0x01, 0x06, 0x08, 0x2A,
    0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0x19, 0xB5, 0xB2, 0x17, 0x0D,
    0xF5, 0x98, 0x5E, 0xD4, 0xD9, 0x72, 0x16, 0xEF, 0x61, 0x39, 0x3F, 0x14, 0x58, 0xAF, 0x5C, 0x02,
    0x78, 0x07, 0xCA, 0x48, 0x8F, 0x2A, 0xE3, 0x90, 0xB9, 0x03, 0xA1, 0xD2, 0x46, 0x20, 0x09, 0x21,
    0x52, 0x98, 0xDC, 0x8E, 0x88, 0x84, 0x67, 0x8E, 0x83, 0xD1, 0xDE, 0x0F, 0x1C, 0xE5, 0x19, 0x1D,
    0x0C, 0x74, 0x60, 0x41, 0x58, 0x5B, 0x36, 0x55, 0xF8, 0x3D, 0xAB, 0xA3, 0x81, 0xB9, 0x30, 0x81,
    0xB6, 0x30, 0x09, 0x06, 0x03, 0x55, 0x1D, 0x13, 0x04, 0x02, 0x30, 0x00, 0x30, 0x1D, 0x06, 0x03,
    0x55, 0x1D, 0x0E, 0x04, 0x16, 0x04, 0x14, 0xB5, 0x97, 0xFD, 0xAB, 0x36, 0x1A, 0xA0, 0xA2, 0x23,
    0xA7, 0x68, 0x25, 0x25, 0xFB, 0x82, 0x55, 0xD0, 0x4F, 0xCF, 0xB8, 0x30, 0x7A, 0x06, 0x03, 0x55,
    0x1D, 0x23, 0x04, 0x73, 0x30, 0x71, 0x80, 0x14, 0x1A, 0xBB, 0x56, 0x44, 0x65, 0x8C, 0x4D, 0x4F,
    0xCD, 0x29, 0xA2, 0x3F, 0x4C, 0xC6, 0xBC, 0xA8, 0x8B, 0xA4, 0x0A, 0xDA, 0xA1, 0x56, 0xA4, 0x54,
    0x30, 0x52, 0x31, 0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x49, 0x4E, 0x31,
    0x0D, 0x30, 0x0B, 0x06, 0x03, 0x55, 0x04, 0x0A, 0x0C, 0x04, 0x49, 0x46, 0x49, 0x4E, 0x31, 0x0C,
    0x30, 0x0A, 0x06, 0x03, 0x55, 0x04, 0x0B, 0x0C, 0x03, 0x43, 0x43, 0x53, 0x31, 0x0F, 0x30, 0x0D,
    0x06, 0x03, 0x55, 0x04, 0x03, 0x0C, 0x06, 0x52, 0x6F, 0x6F, 0x74, 0x43, 0x41, 0x31, 0x15, 0x30,
    0x13, 0x06, 0x03, 0x55, 0x04, 0x2E, 0x13, 0x0C, 0x54, 0x72, 0x75, 0x73, 0x74, 0x20, 0x41, 0x6E,
    0x63, 0x68, 0x6F, 0x72, 0x82, 0x01, 0x2E, 0x30, 0x0E, 0x06, 0x03, 0x55, 0x1D, 0x0F, 0x01, 0x01,
    0xFF, 0x04, 0x04, 0x03, 0x02, 0x00, 0x81, 0x30, 0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D,
    0x04, 0x03, 0x02, 0x03, 0x47, 0x00, 0x30, 0x44, 0x02, 0x20, 0x68, 0xFD, 0x9C, 0x8F, 0x35, 0x33,
    0x0B, 0xB8, 0x32, 0x8C, 0xAF, 0x1C, 0x81, 0x4E, 0x41, 0x29, 0x26, 0xCB, 0xB7, 0x10, 0xA0, 0x75,
    0xFC, 0x89, 0xAE, 0xC5, 0x1D, 0x92, 0x8E, 0x72, 0xEF, 0x5C, 0x02, 0x20, 0x7D, 0xC1, 0xEB, 0x58,
    0x21, 0xF1, 0xFD, 0xFB, 0x5E, 0xD7, 0xDE, 0x06, 0xC9, 0xB4, 0xFF, 0x59, 0x8D, 0x37, 0x8C, 0x7A,
    0x48, 0xCD, 0x2D, 0x99, 0x74, 0x77, 0x58, 0x9D, 0x95, 0x51, 0x8F, 0x5D
};

/**
 * Manifest for ECC-256
 */
static uint8_t manifest[] = 
{
    // COSE Sign1 Trust
    0x84,
        // Protected signed header trust
        0x43,
            // Trust Sign Alogorithm
            0xA1, 
            // ECDSA
            0x01, 0x26, 
        // Unprotected signed header trust
        0xA1, 
            // Root of trust
            0x04, 0x42, 
            // Trust Anchor OID
            0xE0, 0xE3, 
        // Payload info Byte string of single byte length
        0x58, 
            // Byte string length of manifest            
            0x3D, 
            // Trust manifest, array of 6 entries
            0x86, 
            // Version(Major Type 0)
            0x01,
            // NULL
            0xF6,
            // NULL
            0xF6,
            // Resources, array of 4 entries
            0x84, 
                // Trust Payload Type
                0x20,
                // 2 byte length
                0x19,
                // Payload Length
                0x02, 0x92, 
                // Trust Payload version
                0x03, 
                // Trust Add info data 
                0x82, 
                    // Offset( Major Type 0)
                    0x00, 
                    // Trust add info write type
                    0x01, 
                // Trust Processors, array of 2 entries 
                0x82, 
                    // Processing step integrity, array of 2 entries
                    0x82, 
                        // Process( Major Type 1)
                        0x20, 
                        // Parameters, byte string with single byte length
                        0x58, 
                        // Byte string length 
                        0x25, 
                        // IFX Digest info, array of 2 entries
                        0x82, 
                        // Digest Algorithm
                        0x18,
                        // SHA-256
                        0x29, 
                        // Digest
                        0x58, 
                        // Byte string length
                        0x20, 
                        // Digest data
                        0xA0, 0xAE, 0xD2, 0x75, 0x75, 0xB8, 0x77, 0xED, 
                        0x0F, 0xEA, 0xB6, 0x3C, 0x74, 0x35, 0x58, 0xEA,
                        0xE3, 0xA2, 0x26, 0x4C, 0x8C, 0xEC, 0xD5, 0x8F,
                        0x8F, 0x4E, 0x12, 0xAD, 0xA0, 0xDB, 0x73, 0x9A, 
                    // NULL
                    0xF6, 
                // Trsust Target
                0x82, 
                    // Component identifier
                    0x40, 
                    // Storage identifier
                    0x42, 
                    // Optiga target OID
                    0xE0, 0xE1, 
        // Signature info, byte string of single byte length
        0x58, 
            // Byte string length for ECC 256
            0x40, 
            // Signature data
            0x8B, 0x87, 0xAE, 0x23, 0x11, 0x4D, 0x44, 0xC4, 0xE8, 0x93, 0xFA, 0x70, 0x99, 0xD0, 0x32, 0xFE,
            0x70, 0x9D, 0xF9, 0x7C, 0x81, 0x98, 0x05, 0x73, 0xA9, 0x61, 0x8A, 0x3D, 0xD7, 0xCE, 0x8B, 0xA4,
            0xC8, 0xC2, 0x70, 0x19, 0x8E, 0x74, 0xE8, 0x58, 0xDC, 0x22, 0x63, 0x9E, 0x38, 0x52, 0x8C, 0x7D,
            0x95, 0xE2, 0x5E, 0x28, 0xC7, 0x71, 0xED, 0xDF, 0xFE, 0x79, 0xC4, 0x62, 0x77, 0xB8, 0xC6, 0x5C                     

};

/**
 * Fragment array for continue with 608 bytes of payload and 32 bytes of digest of next fragment
 */
static uint8_t continue_fragment_array[] = 
{
    // Payload of 608 bytes
    0x21, 0x40, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
    0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x3B,
    0x0D, 0x0A, 0x21, 0x40, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,
    0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,
    0x3E, 0x3F, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E,
    0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E,
    0x5F, 0x3B, 0x0D, 0x0A, 0x21, 0x40, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B,
    0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
    0x3C, 0x3D, 0x3E, 0x3F, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C,
    0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C,
    0x5D, 0x5E, 0x5F, 0x3B, 0x0D, 0x0A, 0x21, 0x40, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
    0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
    0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A,
    0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A,
    0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x3B, 0x0D, 0x0A, 0x21, 0x40, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
    0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x3B, 0x0D, 0x0A, 0x21, 0x40, 0x22, 0x23, 0x24, 0x25,
    0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
    0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
    0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56,
    0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x3B, 0x0D, 0x0A, 0x21, 0x40, 0x22, 0x23,
    0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33,
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x41, 0x42, 0x43, 0x44,
    0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54,
    0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x3B, 0x0D, 0x0A, 0x21, 0x40,
    0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31,
    0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x41, 0x42,
    0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52,
    0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x3B, 0x0D, 0x0A,
    0x21, 0x40, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
    0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x3B,
    0x0D, 0x0A, 0x21, 0x40, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,
    // Digest of final fragment data    
    0xDF, 0x34, 0x57, 0x42, 0xB8, 0xB8, 0xB3, 0xC1, 0x21, 0xCC, 0x4D, 0x06, 0x88, 0x6F, 0xFE, 0x09,
    0x1F, 0xBC, 0xA0, 0x07, 0xD5, 0xDA, 0x81, 0xB4, 0x2D, 0xDC, 0xE0, 0x50, 0xCE, 0xBA, 0x1E, 0x13
};

/**
 * Fragment array for final with 50 bytes of payload
 */
static uint8_t final_fragment_array[] = 
{
    // Payload of 50 bytes  
    0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,
    0x3E, 0x3F, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E,
    0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E,
    0x5F, 0x3B    
};

/**
 * Sample metadata of 0xE0E1 to change access condition to integrity protected and reseting the version tag
 */
static uint8_t E0E1_metadata[] = { 0x20, 0x09, 0xD0, 0x03, 0x21, 0xE0, 0xE3, 0xC1, 0x02, 0x00, 0x00 };

/**
 * Sample metadata of 0xE0E3 for execute access condition
 */
static uint8_t E0E3_metadata[] = { 0x20, 0x06, 0xD3, 0x01, 0x00, 0xe8, 0x01, 0x11 };

/**
 * Local functions prototype
 */
static optiga_lib_status_t write_metadata ( optiga_util_t * me);
static optiga_lib_status_t write_ecc_256_certificate ( optiga_util_t * me);

/**
 * The below example demonstrates the protected update of data using #optiga_util_protected_update_start,
 * optiga_util_protected_update_continue & optiga_util_protected_update_final
 *
 * This example uses OID 0xE0E3 as Trust Anchor and OID 0xE0E1 as Target OID. Signature algorithm used is ECC-256.
 *
 * Example for #optiga_util_protected_update_start,#optiga_util_protected_update_continue
 *              & #optiga_util_protected_update_final.
 *
 */
void example_optiga_util_protected_update(void)
{
    optiga_lib_status_t return_status = 0;

    optiga_util_t * me = NULL;
    OPTIGA_EXAMPLE_LOG_MESSAGE(__FUNCTION__);

    do
    {
        /**
         *  Create OPTIGA util Instance
         *
         */
        me = optiga_util_create(0, optiga_util_callback, NULL);
        if (NULL == me)
        {
            break;
        }
        OPTIGA_EXAMPLE_LOG_MESSAGE("optiga_util_create--finish\n");
        /**
        *  Precondition 1 : Write Metadata for 0xE0E1 and 0xE0E3
         */
        return_status = write_metadata( me);
        if (OPTIGA_LIB_SUCCESS != return_status)
        {
            break;
        }
        OPTIGA_EXAMPLE_LOG_MESSAGE("write_metadata--finish\n");
        /**
        *  Precondition 2 : Write ECC-256 certificate for 0xE0E3
         */
        return_status = write_ecc_256_certificate( me);
        if (OPTIGA_LIB_SUCCESS != return_status)
        {
            break;
        }
        OPTIGA_EXAMPLE_LOG_MESSAGE("write_ecc_256_certificate--finish\n");
        /**
        *   Send the manifest using optiga_util_protected_update_start
        */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_protected_update_start(me,
                                                           0x01,
                                                           manifest,
                                                           sizeof(manifest));

        if (OPTIGA_LIB_SUCCESS != return_status)
        {
            break;
        }

        while (OPTIGA_LIB_BUSY == optiga_lib_status)
        {
            //Wait until the optiga_util_write_data operation is completed
        }

        if (OPTIGA_LIB_SUCCESS != optiga_lib_status)
        {
            //writing data to a data object failed.
            return_status = optiga_lib_status;
            break;
        }
        OPTIGA_EXAMPLE_LOG_MESSAGE("optiga_util_protected_update_start--finish\n");
        /**
        *   Send the first fragment using optiga_util_protected_update_continue
        */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_protected_update_continue( me,
                                                               continue_fragment_array,
                                                               sizeof(continue_fragment_array));

        if (OPTIGA_LIB_SUCCESS != return_status)
        {
            break;
        }

        while (OPTIGA_LIB_BUSY == optiga_lib_status)
        {
            //Wait until the optiga_util_write_data operation is completed
        }

        if (OPTIGA_LIB_SUCCESS != optiga_lib_status)
        {
            //writing data to a data object failed.
            return_status = optiga_lib_status;
            break;
        }
        OPTIGA_EXAMPLE_LOG_MESSAGE("optiga_util_protected_update_continue--finish\n");

        /**
        *   Send the last fragment using optiga_util_protected_update_final
        */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        return_status = optiga_util_protected_update_final(me,
                                                           final_fragment_array,
                                                           sizeof(final_fragment_array));

        if (OPTIGA_LIB_SUCCESS != return_status)
        {
            break;
        }

        while (OPTIGA_LIB_BUSY == optiga_lib_status)
        {
            //Wait until the optiga_util_write_data operation is completed
        }

        if (OPTIGA_LIB_SUCCESS != optiga_lib_status)
        {
            //writing data to a data object failed.
            return_status = optiga_lib_status;
            break;
        }
        OPTIGA_EXAMPLE_LOG_MESSAGE("optiga_util_protected_update_final--finish\n");

        return_status = OPTIGA_LIB_SUCCESS;
    } while (FALSE);
    OPTIGA_EXAMPLE_LOG_STATUS(return_status);
    
    if (me)
    {
        //Destroy the instance after the completion of usecase if not required.
        return_status = optiga_util_destroy(me);
        if(OPTIGA_LIB_SUCCESS != return_status)
        {
            //lint --e{774} suppress This is a generic macro
            OPTIGA_EXAMPLE_LOG_STATUS(return_status);
        }
    }
}

static optiga_lib_status_t write_metadata ( optiga_util_t * me)
{
    optiga_lib_status_t return_status = 0;
    uint16_t optiga_oid;

    do
    {
        /**
        *   set meta data "0x20 0x09 0xD0 0x03 0x21 0xE0 0xE8 0xC1 0x02 0x00 0x00" for oids 0xE0E1
        */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        optiga_oid = 0xE0E1;
        return_status = optiga_util_write_metadata(me,
                                                   optiga_oid,
                                                   E0E1_metadata,
                                                   sizeof(E0E1_metadata));

        if (OPTIGA_LIB_SUCCESS != return_status)
        {
            break;
        }

        while (OPTIGA_LIB_BUSY == optiga_lib_status)
        {
            //Wait until the optiga_util_write_metadata operation is completed
        }

        if (OPTIGA_LIB_SUCCESS != optiga_lib_status)
        {
            //writing metadata to a data object failed.
            break;
        }

        /**
        *   set meta data "0x20 0x03 0xD3 0x01 0x00" for oid 0xE0E3
        */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        optiga_oid = 0xE0E3;
        return_status = optiga_util_write_metadata(me,
                                                   optiga_oid,
                                                   E0E3_metadata,
                                                   sizeof(E0E3_metadata));

        if (OPTIGA_LIB_SUCCESS != return_status)
        {
            break;
        }

        while (OPTIGA_LIB_BUSY == optiga_lib_status)
        {
            //Wait until the optiga_util_write_metadata operation is completed
        }

        if (OPTIGA_LIB_SUCCESS != optiga_lib_status)
        {
            //writing metadata to a data object failed.
            break;
        }
    } while (FALSE);

    return (return_status);
}

static optiga_lib_status_t write_ecc_256_certificate ( optiga_util_t * me)
{
    optiga_lib_status_t return_status = 0;
    uint16_t optiga_oid;
    uint16_t offset;

    do
    {
        /**
        *   valid ECC-256 with sha256 certificate is available in oid 0xE0E3
        */
        optiga_lib_status = OPTIGA_LIB_BUSY;
        optiga_oid = 0xE0E3;
        offset = 0x00;
        return_status = optiga_util_write_data(me,
                                               optiga_oid,
                                               OPTIGA_UTIL_ERASE_AND_WRITE,
                                               offset,
                                               trust_anchor,
                                               sizeof(trust_anchor));

        if (OPTIGA_LIB_SUCCESS != return_status)
        {
            break;
        }

        while (OPTIGA_LIB_BUSY == optiga_lib_status)
        {
            //Wait until the optiga_util_write_data operation is completed
        }

        if (OPTIGA_LIB_SUCCESS != optiga_lib_status)
        {
            //writing data to a data object failed.
            break;
        }
    } while (FALSE);

    return (return_status);

}

/**
* @}
*/
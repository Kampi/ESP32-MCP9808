 /*
 * mcp9808_defs.h
 *
 *  Copyright (C) Daniel Kampert, 2022
 *	Website: www.kampis-elektroecke.de
 *  File info: MCP9808 driver for ESP32.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Errors and commissions should be reported to DanielKampert@kampis-elektroecke.de
 */

#ifndef MCP9808_ERRORS_H_
#define MCP9808_ERRORS_H_

typedef int MCP9808_Error_t;

/** @brief      Generic error check macro.
 *  @param Func Function that should be checked
 */
#define MCP9808_ERROR_CHECK(Func)                               do                                                                                                              \
                                                                {                                                                                                               \
                                                                    MCP9808_Error_t Error = Func;                                                                              \
                                                                    if(Error != MCP9808_ERR_OK)                                                                                \
                                                                    {                                                                                                           \
                                                                        ESP_LOGE("MCP9808", "Error check failed in (%s) at line (%u): 0x%X!", __FUNCTION__, __LINE__, Error);  \
                                                                        return Error;                                                                                           \
                                                                    }                                                                                                           \
                                                                } while(0);

/** @brief MCP9808 error base.
 */
#define MCP9808_BASE                                           0x7000

/** @brief Everything okay.
 */
#define MCP9808_ERR_OK                                         (MCP9808_BASE + 0)

/** @brief Invalid function parameter.
 */
#define MCP9808_ERR_INVALID_ARG                                (MCP9808_BASE + 1)

/** @brief Invalid response from sensor.
 */
#define MCP9808_ERR_INVALID_RESPONSE                           (MCP9808_BASE + 2)

/** @brief Device is locked.
 */
#define MCP9808_ERR_LOCKED                                      (MCP9808_BASE + 3)

/** @brief No device found.
 */
#define MCP9808_ERR_NO_DEVICE                                   (MCP9808_BASE + 4)

/** @brief Device not initialized.
 */
#define MCP9808_ERR_NOT_INITIALIZED                             (MCP9808_BASE + 5)

/** @brief Device is in shutdown mode and not active.
 */
#define MCP9808_ERR_NOT_ACTIVE                                  (MCP9808_BASE + 6)

#endif /* MCP9808_ERRORS_H_ */
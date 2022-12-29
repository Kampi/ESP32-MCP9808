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

#ifndef MCP9808_DEFS_H_
#define MCP9808_DEFS_H_

#include <cstddef>
#include <stdint.h>
#include <stdbool.h>

/** @brief              I2C master write function definition.
 *  @param Address      Device address
 *  @param p_Data       Pointer to data buffer
 *  @param Size         Data length
 *  @param p_Instance   Pointer to interface instance
 *  @return             0 when successful, -1 when failed
 */
typedef int8_t (*MCP9808_Write_t)(uint8_t Address, const uint8_t* p_Data, size_t Size, void* p_Instance);

/** @brief              I2C master read function definition.
 *  @param Address      Device address
 *  @param p_Data       Pointer to data buffer
 *  @param Size         Data length
 *  @param p_Instance   Pointer to interface instance
 *  @return             0 when successful, -1 when failed
 */
typedef int8_t (*MCP9808_Read_t)(uint8_t Address, uint8_t* p_Data, size_t Size, void* p_Instance);

/** @brief              Alarm callback definition.
 *  @param Temperature  Last temperature measurement.
 */
typedef void (*MCP9808_Alarm_t)(int8_t Temperature);

/** @brief MCP9808 hysteresis definitions.
 */
typedef enum
{
    MCP9808_HYSTERESIS_0 = 0,                               /**< Hysteresis 0°C. */
    MCP9808_HYSTERESIS_15,                                  /**< Hysteresis +-1.5°C. */
    MCP9808_HYSTERESIS_3,                                   /**< Hysteresis +-3°C. */
    MCP9808_HYSTERESIS_6,                                   /**< Hysteresis +-6°C. */
} MCP9808_Hysteresis_t;

/** @brief MCP9808 resolution definitions.
 */
typedef enum
{
    MCP9808_RESOLUTION_05 = 0,                              /**< Resolution +-0.5°C. */
    MCP9808_RESOLUTION_025,                                 /**< Resolution +-0.25°C. */
    MCP9808_RESOLUTION_0125,                                /**< Resolution +-0.125°C. */
    MCP9808_RESOLUTION_00625,                               /**< Resolution +-0.0625°C. */
} MCP9808_Resolution_t;

/** @brief MCP9808 device object definition.
 */
typedef struct
{
    uint8_t ID;                                             /**< Device ID. */
    uint8_t Revision;                                       /**< Device revision. */
    MCP9808_Alarm_t TCrit_Alarm;                            /**< Ambient temperature is greater than TCRIT (TA >= TCRIT). */
    MCP9808_Alarm_t TUpper_Alarm;                           /**< Ambient temperature is greater than TUPPER (TA > TUPPER). */
    MCP9808_Alarm_t TLower_Alarm;                           /**< Ambient temperature is lower than TLOWER  (TA < TLOWER). */
    struct
    {
        MCP9808_Write_t Write;                              /**< Platform specific I2C write function. */
        MCP9808_Read_t Read;                                /**< Platform specific I2C read function. */
        uint8_t Address;                                    /**< I2C address of the device. */
    } I2C;
    struct
    {
        bool isInitialized;                                 /**< Device was initialized successfully.
                                                                 NOTE: Managed by the driver. */
        bool isShutdown;                                    /**< Device is in shutdown mode.
                                                                 NOTE: Managed by the driver. */
    } Internal;
} MCP9808_t;

#endif /* MCP9808_DEFS_H_ */
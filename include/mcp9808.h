 /*
 * mcp9808.h
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

#ifndef MCP9808_H_
#define MCP9808_H_

#include "MCP9808_defs.h"
#include "MCP9808_errors.h"

/** @brief  Get the shutdown status of the device.
 *  @return #true when the device is in shutdown mode
 */
inline __attribute__((always_inline)) bool MCP9808_isShutdown(MCP9808_t* const p_Device)
{
    if(p_Device == NULL)
    {
        return true;
    }

    return p_Device->Internal.isShutdown;
}

/** @brief          Initialize the MCP9808 sensor driver and the device.
 *  @param p_Device Pointer to device object
 *  @return         MCP9808_ERR_OK when successful
 */
MCP9808_Error_t MCP9808_Init(MCP9808_t* const p_Device);

/** @brief          Enable / Disable the shutdown mode.
 *  @param p_Device Pointer to device object
 *  @param Enable   Shutdown enable / disable
 *  @return         MCP9808_ERR_OK when successful
 */
MCP9808_Error_t MCP9808_SwitchShutdown(MCP9808_t* const p_Device, bool Enable);

/** @brief                  Get the temperature from the device.
 *  @param p_Device         Pointer to device object
 *  @param p_Temperature    Pointer to ambient temperature in °C
 *  @return                 MCP9808_ERR_OK when successful
 */
MCP9808_Error_t MCP9808_GetTemperature(MCP9808_t* const p_Device, int8_t* p_Temperature);

/** @brief          Set the upper temperature limit.
 *  @param p_Device Pointer to device object
 *  @param Limit    Temperature limit
 *  @return         MCP9808_ERR_OK when successful
 */
MCP9808_Error_t MCP9808_SetTupper(MCP9808_t* const p_Device, int16_t Limit);

/** @brief          Get the upper temperature limit.
 *  @param p_Device Pointer to device object
 *  @param p_Limit  Pointer to temperature limit
 *  @return         MCP9808_ERR_OK when successful
 */
MCP9808_Error_t MCP9808_GetTupper(MCP9808_t* const p_Device, int16_t* p_Limit);

/** @brief          Set the lower temperature limit.
 *  @param p_Device Pointer to device object
 *  @param Limit    Temperature limit
 *  @return         MCP9808_ERR_OK when successful
 */
MCP9808_Error_t MCP9808_SetTlower(MCP9808_t* const p_Device, int16_t Limit);

/** @brief          Get the lower temperature limit.
 *  @param p_Device Pointer to device object
 *  @param p_Limit  Pointer to temperature limit
 *  @return         MCP9808_ERR_OK when successful
 */
MCP9808_Error_t MCP9808_GetTlower(MCP9808_t* const p_Device, int16_t* p_Limit);

/** @brief          Set the critical temperature limit.
 *  @param p_Device Pointer to device object
 *  @param Limit    Temperature limit
 *  @return         MCP9808_ERR_OK when successful
 */
MCP9808_Error_t MCP9808_SetTcrit(MCP9808_t* const p_Device, int16_t Limit);

/** @brief          Get the critical temperature limit.
 *  @param p_Device Pointer to device object
 *  @param p_Limit  Pointer to temperature limit
 *  @return         MCP9808_ERR_OK when successful
 */
MCP9808_Error_t MCP9808_GetTcrit(MCP9808_t* const p_Device, int16_t* p_Limit);

/** @brief              Set the hysteresis of the device.
 *  @param p_Device     Pointer to device object
 *  @param Hysteresis   Sensor gysteresis
 *  @return             MCP9808_ERR_OK when successful
 */
MCP9808_Error_t MCP9808_SetHysteresis(MCP9808_t* const p_Device, MCP9808_Hysteresis_t Hysteresis);

/** @brief              Get the hysteresis of the device.
 *  @param p_Device     Pointer to device object
 *  @param p_Hysteresis Pointer to sensor gysteresis
 *  @return             MCP9808_ERR_OK when successful
 */
MCP9808_Error_t MCP9808_GetHysteresis(MCP9808_t* const p_Device, MCP9808_Hysteresis_t* p_Hysteresis);

/** @brief              Set the resolution of the device.
 *  @param p_Device     Pointer to device object
 *  @param Resolution   Sensor resolution
 *  @return             MCP9808_ERR_OK when successful
 */
MCP9808_Error_t MCP9808_SetResolution(MCP9808_t* const p_Device, MCP9808_Resolution_t Resolution);

/** @brief              Get the resolution of the device.
 *  @param p_Device     Pointer to device object
 *  @param p_Resolution Pointer to sensor resolution
 *  @return             MCP9808_ERR_OK when successful
 */
MCP9808_Error_t MCP9808_GetResolution(MCP9808_t* const p_Device, MCP9808_Resolution_t* p_Resolution);

#endif /* MCP9808_H_ */
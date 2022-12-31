 /*
 * mcp9808.cpp
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

#include <esp_log.h>

#include "mcp9808.h"

#define MCP9808_REG_POINTER                 0x00
#define MCP9808_REG_CONFIG                  0x01
#define MCP9808_REG_TEMP_UPPER              0x02
#define MCP9808_REG_TEMP_LOWER              0x03
#define MCP9808_REG_TEMP_CRIT               0x04
#define MCP9808_REG_TEMP_AMBIENT            0x05
#define MCP9808_REG_MANUFACTURER_ID         0x06
#define MCP9808_REG_DEVICE_ID               0x07
#define MCP9808_REG_RESOLUTION              0x08

#define MCP9808_BIT_SIGN                    0x04
#define MCP9808_BIT_TLOWER                  0x05
#define MCP9808_BIT_TUPPER                  0x06
#define MCP9808_BIT_WIN_LOCK                0x06
#define MCP9808_BIT_TCRIT                   0x07
#define MCP9808_BIT_CRIT_LOCK               0x07

#define MCP9808_DEVICE_ID                   0x04
#define MCP9808_MANUFACTURER_ID             0x0054

static const char* TAG                      = "MCP9808";

/** @brief          Change the given bits in a register.
 *  @param p_Device Pointer to device object
 *  @param Register Register address
 *  @param Mask     Bit mask
 *  @param Value    New value for the masked bits
 *  @return         MCP9808_ERR_OK when successful
 */
static MCP9808_Error_t MCP9808_ModifyBits(MCP9808_t& p_Device, uint8_t Register, uint16_t Mask, uint16_t Value)
{
    uint8_t Temp[2];

    if(p_Device.I2C.Read(MCP9808_REG_CONFIG, Temp, 2, &p_Device.I2C.Address) != 0)
    {
        return MCP9808_ERR_INVALID_RESPONSE;
    }

    Temp[0] &= ~(Mask >> 0x08);
    Temp[0] |= Value >> 0x08;

    Temp[1] &= ~(Mask & 0xFF);
    Temp[1] |= Value & 0xFF;

    if(p_Device.I2C.Write(MCP9808_REG_CONFIG, Temp, 2, &p_Device.I2C.Address) != 0)
    {
        return MCP9808_ERR_INVALID_RESPONSE;
    }

    return MCP9808_ERR_OK;
}

MCP9808_Error_t MCP9808_Init(MCP9808_t& p_Device)
{
    uint16_t ID;
    uint8_t Temp[2];

    if((p_Device.I2C.Read == NULL) || (p_Device.I2C.Write == NULL) || (p_Device.I2C.Address < 0x18))
    {
        return MCP9808_ERR_INVALID_ARG;
    }
    else if(p_Device.Internal.isInitialized)
    {
        return MCP9808_ERR_OK;
    }

    p_Device.Internal.isInitialized = false;

    if(p_Device.I2C.Read(MCP9808_REG_MANUFACTURER_ID, Temp, 2, &p_Device.I2C.Address) != 0)
    {
        return MCP9808_ERR_INVALID_RESPONSE;
    }

    ID = ((static_cast<uint16_t>(Temp[0])) << 0x08) | Temp[1];
    if(ID != MCP9808_MANUFACTURER_ID)
    {
        return MCP9808_ERR_NO_DEVICE;
    }

    if(p_Device.I2C.Read(MCP9808_REG_DEVICE_ID, Temp, 2, &p_Device.I2C.Address) != 0)
    {
        return MCP9808_ERR_INVALID_RESPONSE;
    }

    if(p_Device.I2C.Read(MCP9808_REG_CONFIG, Temp, 2, &p_Device.I2C.Address) != 0)
    {
        return MCP9808_ERR_INVALID_RESPONSE;
    }

    p_Device.ID = Temp[0];
    p_Device.Revision = Temp[1];

    // Check if the device is locked.
    if((Temp[1] & ((0x01 << MCP9808_BIT_CRIT_LOCK) | (0x01 << MCP9808_BIT_WIN_LOCK))) == ((0x01 << MCP9808_BIT_CRIT_LOCK) | (0x01 << MCP9808_BIT_WIN_LOCK)))
    {
        ESP_LOGE(TAG, "Device locked!");

        return MCP9808_ERR_LOCKED;
    }

    ESP_LOGI(TAG, "Initialization successful!");
    ESP_LOGI(TAG, " Device ID: 0x%X", p_Device.ID);
    ESP_LOGI(TAG, " Device Revision: %u", p_Device.Revision);

    p_Device.Internal.isShutdown = false;
    p_Device.Internal.isInitialized = true;

    return MCP9808_ERR_OK;
}

MCP9808_Error_t MCP9808_SwitchShutdown(MCP9808_t& p_Device, bool Enable)
{
    if(p_Device.Internal.isInitialized == false)
    {
        return MCP9808_ERR_NOT_INITIALIZED;
    }
    else if(p_Device.Internal.isShutdown == Enable)
    {
        return MCP9808_ERR_OK;
    }

    MCP9808_ERROR_CHECK(MCP9808_ModifyBits(p_Device, MCP9808_REG_CONFIG, (0x01 << 0x08), (Enable << 0x08)));

    p_Device.Internal.isShutdown = Enable;

    return MCP9808_ERR_OK;
}

MCP9808_Error_t MCP9808_GetTemperature(MCP9808_t& p_Device, int8_t* const p_Temperature)
{
    int8_t Temperature;
    uint8_t Temp[2];

    if(p_Temperature == NULL)
    {
        return MCP9808_ERR_INVALID_ARG;
    }
    else if(p_Device.Internal.isInitialized == false)
    {
        return MCP9808_ERR_NOT_INITIALIZED;
    }
    else if(p_Device.Internal.isShutdown)
    {
        return MCP9808_ERR_NOT_ACTIVE;
    }

    if(p_Device.I2C.Read(MCP9808_REG_TEMP_AMBIENT, Temp, 2, &p_Device.I2C.Address) != 0)
    {
        return MCP9808_ERR_INVALID_RESPONSE;
    }

    // TA < 0°C
    if((Temp[0] & (0x01 << MCP9808_BIT_SIGN)) >> MCP9808_BIT_SIGN)
    {
        Temperature = 256 - (((Temp[0] & 0x0F) << 0x04) + (Temp[1] >> 0x04));
    }
    // TA > 0°C
    else
    {
        Temperature = (((Temp[0] & 0x0F) << 0x04) + (Temp[1] >> 0x04));
    }

    // TA > TCRIT
    if((Temp[0] & (0x01 << MCP9808_BIT_TCRIT)) >> MCP9808_BIT_TCRIT)
    {
        ESP_LOGW(TAG, "TA > Tcrit!");

        if(p_Device.TCrit_Alarm != NULL)
        {
            p_Device.TCrit_Alarm(Temperature);
        }
    }

    // TA > TUPPER
    if((Temp[0] & (0x01 << MCP9808_BIT_TUPPER)) >> MCP9808_BIT_TUPPER)
    {
        ESP_LOGW(TAG, "TA > Tupper!");

        if(p_Device.TUpper_Alarm != NULL)
        {
            p_Device.TUpper_Alarm(Temperature);
        }
    }

    // TA < TLOWER
    if((Temp[0] & (0x01 << MCP9808_BIT_TLOWER)) >> MCP9808_BIT_TLOWER)
    {
        ESP_LOGW(TAG, "TA < Tlower!");

        if(p_Device.TLower_Alarm != NULL)
        {
            p_Device.TLower_Alarm(Temperature);
        }
    }

    *p_Temperature = Temperature;

    return MCP9808_ERR_OK;
}

MCP9808_Error_t MCP9808_SetTupper(MCP9808_t& p_Device, int16_t Limit)
{
    uint8_t Temp[2];

    if(p_Device.Internal.isInitialized == false)
    {
        return MCP9808_ERR_NOT_INITIALIZED;
    }

    Temp[0] = static_cast<uint8_t>(Limit >> 0x08);
    Temp[1] = static_cast<uint8_t>(Limit & 0xFF);

    if(p_Device.I2C.Write(MCP9808_REG_TEMP_UPPER, Temp, 2, &p_Device.I2C.Address) != 0)
    {
        return MCP9808_ERR_INVALID_RESPONSE;
    }

    return MCP9808_ERR_OK;
}

MCP9808_Error_t MCP9808_GetTupper(MCP9808_t& p_Device, int16_t* const p_Limit)
{
    uint8_t Temp[2];

    if(p_Limit == NULL)
    {
        return MCP9808_ERR_INVALID_ARG;
    }
    else if(p_Device.Internal.isInitialized == false)
    {
        return MCP9808_ERR_NOT_INITIALIZED;
    }

    if(p_Device.I2C.Read(MCP9808_REG_TEMP_UPPER, Temp, 2, &p_Device.I2C.Address) != 0)
    {
        return MCP9808_ERR_INVALID_RESPONSE;
    }

    *p_Limit = (static_cast<int16_t>(Temp[0]) << 0x08) | Temp[1];

    return MCP9808_ERR_OK;
}

MCP9808_Error_t MCP9808_SetTlower(MCP9808_t& p_Device, int16_t Limit)
{
    uint8_t Temp[2];

    if(p_Device.Internal.isInitialized == false)
    {
        return MCP9808_ERR_NOT_INITIALIZED;
    }

    Temp[0] = static_cast<uint8_t>(Limit >> 0x08);
    Temp[1] = static_cast<uint8_t>(Limit & 0xFF);

    if(p_Device.I2C.Write(MCP9808_REG_TEMP_LOWER, Temp, 2, &p_Device.I2C.Address) != 0)
    {
        return MCP9808_ERR_INVALID_RESPONSE;
    }

    return MCP9808_ERR_OK;
}

MCP9808_Error_t MCP9808_GetTlower(MCP9808_t& p_Device, int16_t* const p_Limit)
{
    uint8_t Temp[2];

    if(p_Limit == NULL)
    {
        return MCP9808_ERR_INVALID_ARG;
    }
    else if(p_Device.Internal.isInitialized == false)
    {
        return MCP9808_ERR_NOT_INITIALIZED;
    }

    if(p_Device.I2C.Read(MCP9808_REG_TEMP_LOWER, Temp, 2, &p_Device.I2C.Address) != 0)
    {
        return MCP9808_ERR_INVALID_RESPONSE;
    }

    *p_Limit = (static_cast<int16_t>(Temp[0]) << 0x08) | Temp[1];

    return MCP9808_ERR_OK;
}

MCP9808_Error_t MCP9808_SetTcrit(MCP9808_t& p_Device, int16_t Limit)
{
    uint8_t Temp[2];

    if(p_Device.Internal.isInitialized == false)
    {
        return MCP9808_ERR_NOT_INITIALIZED;
    }

    Temp[0] = static_cast<uint8_t>(Limit >> 0x08);
    Temp[1] = static_cast<uint8_t>(Limit & 0xFF);

    if(p_Device.I2C.Write(MCP9808_REG_TEMP_CRIT, Temp, 2, &p_Device.I2C.Address) != 0)
    {
        return MCP9808_ERR_INVALID_RESPONSE;
    }

    return MCP9808_ERR_OK;
}

MCP9808_Error_t MCP9808_GetTcrit(MCP9808_t& p_Device, int16_t* const p_Limit)
{
    uint8_t Temp[2];

    if(p_Limit == NULL)
    {
        return MCP9808_ERR_INVALID_ARG;
    }
    else if(p_Device.Internal.isInitialized == false)
    {
        return MCP9808_ERR_NOT_INITIALIZED;
    }

    if(p_Device.I2C.Read(MCP9808_REG_TEMP_CRIT, Temp, 2, &p_Device.I2C.Address) != 0)
    {
        return MCP9808_ERR_INVALID_RESPONSE;
    }

    *p_Limit = (static_cast<int16_t>(Temp[0]) << 0x08) | Temp[1];

    return MCP9808_ERR_OK;
}

MCP9808_Error_t MCP9808_SetHysteresis(MCP9808_t& p_Device, MCP9808_Hysteresis_t Hysteresis)
{
    return MCP9808_ModifyBits(p_Device, MCP9808_REG_CONFIG, 0x03 << 0x09, static_cast<uint16_t>(Hysteresis) << 0x09);
}

MCP9808_Error_t MCP9808_GetHysteresis(MCP9808_t& p_Device, MCP9808_Hysteresis_t* const p_Hysteresis)
{
    uint8_t Temp[2];

    if(p_Hysteresis == NULL)
    {
        return MCP9808_ERR_INVALID_ARG;
    }
    else if(p_Device.Internal.isInitialized == false)
    {
        return MCP9808_ERR_NOT_INITIALIZED;
    }

    if(p_Device.I2C.Read(MCP9808_REG_CONFIG, Temp, 2, &p_Device.I2C.Address) != 0)
    {
        return MCP9808_ERR_INVALID_RESPONSE;
    }

    *p_Hysteresis = static_cast<MCP9808_Hysteresis_t>((Temp[0] >> 0x01) & 0x03);

    return MCP9808_ERR_OK;
}

MCP9808_Error_t MCP9808_SetResolution(MCP9808_t& p_Device, MCP9808_Resolution_t Resolution)
{
    uint8_t Temp;

    if(p_Device.Internal.isInitialized == false)
    {
        return MCP9808_ERR_NOT_INITIALIZED;
    }

    Temp = Resolution;
    if(p_Device.I2C.Write(MCP9808_REG_RESOLUTION, &Temp, 1, &p_Device.I2C.Address) != 0)
    {
        return MCP9808_ERR_INVALID_RESPONSE;
    }

    return MCP9808_ERR_OK;
}

MCP9808_Error_t MCP9808_GetResolution(MCP9808_t& p_Device, MCP9808_Resolution_t* const p_Resolution)
{
    uint8_t Temp;

    if(p_Resolution == NULL)
    {
        return MCP9808_ERR_INVALID_ARG;
    }
    else if(p_Device.Internal.isInitialized == false)
    {
        return MCP9808_ERR_NOT_INITIALIZED;
    }

    if(p_Device.I2C.Read(MCP9808_REG_RESOLUTION, &Temp, 1, &p_Device.I2C.Address) != 0)
    {
        return MCP9808_ERR_INVALID_RESPONSE;
    }

    *p_Resolution = static_cast<MCP9808_Resolution_t>(Temp);

    return MCP9808_ERR_OK;
}
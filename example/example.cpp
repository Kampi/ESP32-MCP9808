#include <esp_log.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>

#include <stdint.h>

#include "mcp9808.h"

MCP9808_t MCP9808;

static const char* TAG 							= "main";

extern "C" void app_main(void)
{
    MCP9808.I2C.Address = 0x18;
    MCP9808.I2C.Read = I2CM_Read;
    MCP9808.I2C.Write = I2CM_Write;

    if((MCP9808_Init(MCP9808) == MCP9808_ERR_OK) && (MCP9808_SetResolution(MCP9808, MCP9808_RESOLUTION_00625) == MCP9808_ERR_OK))
    {
        while(true)
        {
            int8_t Temperature;

            if(MCP9808_GetTemperature(MCP9808, &Temperature) == MCP9808_ERR_OK)
            {
                ESP_LOGI(TAG, "Temperature: %i", Temperature);
            }
            else
            {
                ESP_LOGE(TAG, "Cannot read temperature!");
            }

            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
    else
    {
        ESP_LOGE(TAG, "Cannot initialize MCP9808!");
    }

    while(true)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
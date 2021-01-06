#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#define LCD_SMALL_FONT        Font16 //Small font constant
// main() runs in its own thread in the OS
int main()
{
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    while (true) {
        BSP_LCD_Clear(LCD_COLOR_BLACK);
        BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
        BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_DisplayStringAt(0, 100, (uint8_t *)"ONSDAGS PROJEKT", CENTER_MODE);
        BSP_LCD_DrawEllipse(233, 110, 170, 65);
        BSP_LCD_SetFont(&LCD_SMALL_FONT);
        BSP_LCD_DisplayStringAt(0, 250, (uint8_t *)"Version 0.0.1", CENTER_MODE);
        HAL_Delay(2000);
/*
        BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
        BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
        BSP_LCD_DisplayStringAt(0, 100, (uint8_t *)"DISCOVERY STM32F746NG", CENTER_MODE);
        HAL_Delay(2000);

        BSP_LCD_Clear(LCD_COLOR_GREEN);
        BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
        BSP_LCD_DrawRect(10, 20, 50, 50);
        BSP_LCD_SetTextColor(LCD_COLOR_BROWN);
        BSP_LCD_DrawCircle(80, 80, 50);
        BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
        BSP_LCD_DrawEllipse(150, 150, 50, 100);
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        BSP_LCD_FillCircle(200, 200, 40);
        HAL_Delay(2000);

        BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
        BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
        BSP_LCD_DisplayStringAt(0, 6, (uint8_t *)"HAVE FUN !!!", RIGHT_MODE);
        HAL_Delay(2000);
        */
    }
}


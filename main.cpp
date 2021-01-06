#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include <cstdio>
#define LCD_SMALL_FONT        Font16 //Small font constant
#define LCD_MEDIUM_FONT        Font20 //Small font constant
// main() runs in its own thread in the OS

AnalogIn ain(A0);
DigitalOut myLedD2(D2);
DigitalOut myLedD3(D3);

uint8_t percentString[25] = {0};
uint8_t counterString[15] = {0};

int counter = 1;


void startScreen() {
    BSP_LCD_Clear(LCD_COLOR_DARKCYAN);
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_DARKCYAN);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, 100, (uint8_t *)"ONSDAGS PROJEKT", CENTER_MODE);
    BSP_LCD_DrawEllipse(233, 110, 170, 65);
    BSP_LCD_SetFont(&LCD_SMALL_FONT);
    BSP_LCD_DisplayStringAt(0, 250, (uint8_t *)"Version 0.0.1", CENTER_MODE);
    HAL_Delay(2000);

}

void homeScreen() {
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&LCD_MEDIUM_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, 5, (uint8_t *)"ONSDAGS PROJEKT", CENTER_MODE);
    //BSP_LCD_DisplayStringAt(0, 100, (uint8_t *)percentString, CENTER_MODE);
}

void updatePercentString() {
    BSP_LCD_SetFont(&LCD_MEDIUM_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, 100, (uint8_t *)percentString, CENTER_MODE);
}

void updateCounter() {
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, 130, (uint8_t *)counterString, CENTER_MODE);

}


int main()
{
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);

    startScreen();
    homeScreen();
    while (true) {
        printf("percentage: %3.3f%%\n", ain.read() * 100.0f);
        printf("Counter: %4d\n",counter);
        sprintf((char *) percentString, "Power: %3.0f%%", ain.read() * 100.0f);
        sprintf((char *) counterString,"Counter: %4d",counter);
        updatePercentString();
        updateCounter();
        if(counter < 9999) {
            counter++;
        } else {
            counter = 1;
        }

        if (ain > 0.9f) {
            myLedD3 = !myLedD3;
            myLedD2 = !myLedD3;
        } else {
            myLedD3 = 0;
            myLedD2 = 0;
        }
        wait_us(250000);
    }
}


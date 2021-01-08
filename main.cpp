#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
//#include <cstdio>
#include "Grove_temperature.h"
#define LCD_SMALL_FONT        Font16 //Small font constant
#define LCD_MEDIUM_FONT        Font20 //Medium font constant
#define LCD_XSMALL_FONT         Font12 //Extra small font constant
// main() runs in its own thread in the OS

AnalogIn ain(A0);
Grove_temperature tempSensor(A1);
DigitalOut myLedD2(D2);
DigitalOut myLedD3(D3);
InterruptIn exButton(D4);
TS_StateTypeDef TS_State; // Based on MBED Template

uint8_t percentString[25] = {0};
uint8_t counterString[30] = {0};
uint8_t clickerString[25] = {0};
uint16_t celciusString[25] = {0};
uint16_t fahrenheitString[25] = {0};
uint16_t kelvinString[25] = {0};
uint16_t x, y;
//uint8_t text[30];
uint8_t status;
uint8_t idx;
uint8_t cleared = 0;
uint8_t prev_nb_touches = 0;



float counter = 0.0;
float temperature;
int screenNum = 1;

//Creates the homescreen on start on the LCD
void startScreen() {
    BSP_LCD_Clear(LCD_COLOR_DARKCYAN);
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_DARKCYAN);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, 100, (uint8_t *)"ONSDAGS PROJEKT", CENTER_MODE);
    BSP_LCD_DrawEllipse(233, 110, 170, 65);
    BSP_LCD_SetFont(&LCD_SMALL_FONT);
    BSP_LCD_DisplayStringAt(0, 250, (uint8_t *)"Version 0.0.1", CENTER_MODE);
    HAL_Delay(1000);
    if (status != TS_OK) {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        BSP_LCD_DisplayStringAt(0, 200, (uint8_t *)"TOUCHSCREEN INIT FAIL", CENTER_MODE);
    } else {
        BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
        BSP_LCD_DisplayStringAt(0, 200, (uint8_t *)"TOUCHSCREEN INIT OK", CENTER_MODE);
    }
    HAL_Delay(1000);

}
//Updates the counter on the LCD
void updateCounter() {
    BSP_LCD_SetFont(&LCD_XSMALL_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_DARKCYAN);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(20, 50, (uint8_t *)counterString, LEFT_MODE);

}


//Updates the click counter on the LCD
void updateClickCounter() {
    BSP_LCD_SetFont(&LCD_XSMALL_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_DARKCYAN);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(20, 50, (uint8_t *)clickerString, RIGHT_MODE);
}
//Updates the power percentage on the LCD
void updatePercentString() {
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, 90, (uint8_t *)"POWER", CENTER_MODE);

    //Changes color based on percentage
    if(ain > 0.49 && ain < 0.70) {
        BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    } else if(ain > 0.69 && ain < 0.90) {
        BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
    } else if (ain > 0.89) {
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
    } else {
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    }
       BSP_LCD_SetFont(&LCD_MEDIUM_FONT);
    BSP_LCD_DisplayStringAt(0, 150, (uint8_t *)percentString, CENTER_MODE);
}

//Updates the temperature on the LCD
void updateTemperature() {
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, 80, (uint8_t *)"TEMPERATURE", CENTER_MODE);
    BSP_LCD_SetFont(&LCD_MEDIUM_FONT);
    BSP_LCD_DisplayStringAt(0, 120, (uint8_t *)celciusString, CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, 140, (uint8_t *)fahrenheitString, CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)kelvinString, CENTER_MODE);
}

//Updates the menubuttons on the LCD
void updateMenuButtons() {
    BSP_LCD_SetFont(&LCD_SMALL_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_DARKCYAN);
    BSP_LCD_DrawRect(20,BSP_LCD_GetYSize()-65, 180,50);
    BSP_LCD_DrawRect(BSP_LCD_GetXSize()-200,BSP_LCD_GetYSize()-65, 180,50);
    switch(screenNum) {
        case 1:
            BSP_LCD_FillRect (20,BSP_LCD_GetYSize()-65, 180,50);
            BSP_LCD_SetBackColor(LCD_COLOR_DARKCYAN);
            BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
            BSP_LCD_DisplayStringAt(85, 225, (uint8_t *)"Power", LEFT_MODE);
            BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
            BSP_LCD_SetTextColor(LCD_COLOR_DARKCYAN);
            BSP_LCD_DisplayStringAt(310, 225, (uint8_t *)"Temperature", LEFT_MODE);
            break;
        case 2: 
            BSP_LCD_FillRect(BSP_LCD_GetXSize()-200,BSP_LCD_GetYSize()-65, 180,50);
            BSP_LCD_SetBackColor(LCD_COLOR_DARKCYAN);
            BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
            BSP_LCD_DisplayStringAt(310, 225, (uint8_t *)"Temperature", LEFT_MODE);
            BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
            BSP_LCD_SetTextColor(LCD_COLOR_DARKCYAN);
            BSP_LCD_DisplayStringAt(85, 225, (uint8_t *)"Power", LEFT_MODE);
    }


}




//Template from mbed API reference to update int on butten press
class CheckedCount {
    public:
    CheckedCount(PinName pin) : _interrupt(pin)                     // create the InterruptIn on the pin specified to Counter
    {
        _interrupt.rise(callback(this, &CheckedCount::increment));  // attach increment function of this counter instance
        
    }

    void increment()
    {
        _count++;
        _interrupt.disable_irq();
        wait_us(250000);
        _interrupt.enable_irq();

    }

    int read()
    {
        return _count;
    }

    void resetCount() {
        _count = 0;
    }

    private:
    InterruptIn _interrupt;
    volatile int _count = 0;
};

//Only updates input values on the LCD
void updateValuesOnLCD() {
    updateCounter();
    updateClickCounter();
    updateMenuButtons();
    switch (screenNum) {
    case 1: 
        updatePercentString();
        break;
    case 2: 
        updateTemperature();
        break;
    }
}

//Updates the homescreen on the LCD
void homeScreen() {
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&LCD_MEDIUM_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_DARKCYAN);
    BSP_LCD_SetTextColor(LCD_COLOR_DARKCYAN);
    BSP_LCD_DrawRect (0,0, BSP_LCD_GetXSize()-2,BSP_LCD_GetYSize()-2);
    BSP_LCD_FillRect(0,0, BSP_LCD_GetXSize()-2,70);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, 5, (uint8_t *)"ONSDAGS PROJEKT", CENTER_MODE);
    updateValuesOnLCD();
}

int main()
{
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
    CheckedCount checkedCounter(D4);
    startScreen();
    homeScreen();
    status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

    while (true) {

        temperature =  tempSensor.getTemperature();
        
        // *** Uncomment to show in terminal ***
        //printf("percentage: %3.3f%%\n", ain.read() * 100.0f);
        //printf("Counter: %4d\n",counter);
        //printf("Checked today: %d\n", checkedCounter.read());
        //printf("Temp: %3.3f\n", temperature);

        //Creates Char arrays to shwow on LCD
        sprintf((char *) percentString, "Power: %3.0f%%", ceil(ain.read() * 100.0f));
        sprintf((char *) counterString,"Time online: %.0fsec",counter);
        sprintf((char *) clickerString,"Times checked: %4d",checkedCounter.read());
        sprintf((char *) celciusString,"%4.2fC",temperature);
        sprintf((char *) fahrenheitString,"%4.2fF",temperature*1.8+32);
        sprintf((char *) kelvinString,"%4.2fK",temperature+273.15);

        updateValuesOnLCD();

        //Increments the counter by 1 every second
        if(counter < 9999) {
            counter+=0.25;
        } else {
            counter = 1;
        }

        //Flashes LCD lights if percentage from Rotary Angle Sensor
        if (ain > 0.9f) {
            myLedD3 = !myLedD3;
            myLedD2 = !myLedD3;
        } else {
            myLedD3 = 0;
            myLedD2 = 0;
        }

        //Handles touch input on the display
        BSP_TS_GetState(&TS_State);
        if (TS_State.touchDetected) {
            for (idx = 0; idx < TS_State.touchDetected; idx++) {
                x = TS_State.touchX[idx];
                y = TS_State.touchY[idx];
                printf("Touch %d: x=%d y=%d    \n", idx+1, x, y);
                
            }
            //Touch on click counter
            if((x >= 310 && x <= 480) && (y >= 45 && y <= 75)) {
                checkedCounter.resetCount();
            }

            //Touch on Power Button
            if((x >= 20 && x <= 200) && (y >= BSP_LCD_GetYSize()-65 && y <= BSP_LCD_GetYSize()-15)) {
                screenNum = 1;
                homeScreen();
            }

            //Touch on Temperature Button
            if((x >= BSP_LCD_GetXSize()-200 && x <= BSP_LCD_GetXSize()-20) && (y >= BSP_LCD_GetYSize()-65 && y <= BSP_LCD_GetYSize()-15)) {
                screenNum = 2;
                homeScreen();
            }
        } else {
            if (!cleared) {
                printf("Touches: 0\n");
                cleared = 1;
            }
        }

        wait_us(250000);
    }
}


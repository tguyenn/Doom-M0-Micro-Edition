#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/SPI.h"
#include "../inc/ST7735.h"
#include "Logo.h"
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    Clock_Init40MHz();
    LaunchPad_Init();
    ST7735_InitR(INITR_REDTAB); //SPI HiLetgo ST7735R
    ST7735_FillScreen(ST7735_BLACK);
    uint16_t highcolor = ST7735_Color565(0x00, 0x00, 0x00);
    uint16_t lowcolor = ST7735_Color565(0x00, 0x00, 0x00);
    uint8_t x, y = 0;
    while(1) {
        while(x < 126) {
            ST7735_DrawFastVLine(x, 0, 159, highcolor);
            highcolor =   ST7735_Color565(x, x, x);
//            Clock_Delay1ms(10);  // might need this because hardware slower? <- p sure SPI.c has countermeasure against this
            x++;
        }
        x = 0;
        while(x <126) {
            ST7735_DrawFastVLine(x, y, 159, lowcolor);
//            Clock_Delay1ms(10);
            x++;
        }
       x = 0;
    }
}


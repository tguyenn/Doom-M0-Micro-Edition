#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/SPI.h"
#include "../inc/ST7735.h"
#include "Logo.h"
#include <stdlib.h>
#include <stdio.h>

// my own function that sweeps a gradient across screen
int maina(void) {
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

// my own function that draws a square sliding around
int mainb(void) {
    Clock_Init40MHz();
    LaunchPad_Init();
    ST7735_InitR(INITR_REDTAB); //SPI HiLetgo ST7735R
    ST7735_FillScreen(ST7735_BLACK);
    uint8_t x1 = 0;
    uint8_t y1 = 70;


    while(1) {
        ST7735_FillRect(x1, y1, 5, 5, ST7735_Color565(0xFF, 0x00, 0x00));
        Clock_Delay1ms(10);
        ST7735_FillRect(x1, y1, 5, 5, ST7735_Color565(0x00, 0x00, 0x00));
        x1++;

        y1 =  (int) (y1+4) % 159;
        if(x1 > 159) {
            x1 = 0;
        }
    }
}

// use user input to move square
int mainc(void) {
    Clock_Init40MHz();
    LaunchPad_Init();
    ST7735_InitR(INITR_REDTAB); //SPI HiLetgo ST7735R
    ST7735_FillScreen(ST7735_BLACK);
    IOMUX->SECCFG.PINCM[11] = (uint32_t) 0x00040081; //PB0
    uint8_t x1 = 0;
    uint16_t color = 0;
    while(1) {
        if((GPIOB->DIN31_0 & 0b1) > 0) {
            ST7735_FillRect(x1, 70, 5, 5, ST7735_Color565(0x50, 0x50, 0x50));
            Clock_Delay1ms(10);
            ST7735_FillRect(x1, 70, 5, 5, ST7735_Color565(0x00, 0x00, 0x00));
            x1++;
            if(x1 > 159) {
                x1 = 0;
            }
        }
        ST7735_FillRect(x1, 70, 5, 5, ST7735_Color565(0x00, 0x50, 0x50)); // prevent disappear
        color++;
    }

}

// my own function to sweep a vertical line across display
int maind(void) {
    Clock_Init40MHz();
    LaunchPad_Init();
    ST7735_InitR(INITR_REDTAB); //SPI HiLetgo ST7735R
    ST7735_FillScreen(ST7735_BLACK);
    uint16_t color = ST7735_Color565(0xFF, 0xFF, 0xFF);
    uint8_t x, y;
    while(1) {
        ST7735_DrawFastVLine(x, y, y, color);
//        Clock_Delay1ms(100);  // might need this because hardware slower? <- p sure SPI.c has countermeasure against this
        x++;
        y++;
        if(x > 126) {
            x = 0;
        }
        if(y > 159) {
            y = 0;
        }
    }
}

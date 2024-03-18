// Dump.c
// Your solution to ECE319K Lab 3 Spring 2024
// Author: Toby Nguyen
// Last Modified: 2/11/2024


#include <ti/devices/msp/msp.h>
#include "../inc/Timer.h"
#define MAXBUF 50
uint32_t DataBuffer[MAXBUF];
uint32_t TimeBuffer[MAXBUF];
uint32_t DebugCnt; // 0 to MAXBUF (0 is empty, MAXBUF is full)
uint32_t g_last_data;

// *****Debug_Init******
// Initializes your index or pointer.
// Input: none
// Output:none
void Debug_Init(void){
// students write this for Lab 3
// This function should also initialize Timer G12, call TimerG12_Init.

TimerG12_Init();
DebugCnt = 0;

}

// *****Debug_Dump******
// Records one data and one time into the two arrays.
// Input: data is value to store in DataBuffer
// Output: 1 for success, 0 for failure (buffers full)
uint32_t Debug_Dump(uint32_t data){
// students write this for Lab 3
// The software simply reads TIMG12->COUNTERREGS.CTR to get the current time in bus cycles.
    if(DebugCnt > 49) {
        return 0;
    }
//    Clock_Delay(10000000);
//    if(DebugCnt == 49) {
//        Clock_Delay(1000000000);
//    }
    else {
        DataBuffer[DebugCnt] = data;
        TimeBuffer[DebugCnt] = TIMG12->COUNTERREGS.CTR;
        DebugCnt++;
    }
  return 1; // success
}


// *****Debug_Dump2******
// Always record data and time on the first call to Debug_Dump2
// However, after the first call
//    Records one data and one time into the two arrays, only if the data is different from the previous call.
//    Do not record data or time if the data is the same as the data from the previous call
// Input: data is value to store in DataBuffer
// Output: 1 for success (saved or skipped), 0 for failure (buffers full)

// optional for Lab 3
// The software simply reads TIMG12->COUNTERREGS.CTR to get the current time in bus cycles.

uint32_t Debug_Dump2(uint32_t data){

    // check if buffers are full, if so then return 0
    if(DebugCnt >= MAXBUF) {
        return(0);
    }
//
//    // check if data is different from last time
//    // if data is NOT different, then return 1 for skip
//    // if data is different, then append to arrays, increment DebugCnt, and save data for next pass
    if(data == g_last_data) {
        return(1);
    }
    else {
        DataBuffer[DebugCnt] = data;
        TimeBuffer[DebugCnt] = TIMG12->COUNTERREGS.CTR;
        DebugCnt++;
        g_last_data = data;
    }

  return(1); // success
}


// *****Debug_Period******
// Calculate period of the recorded data using mask
// Input: mask specifies which bit(s) to observe
// Output: period in bus cycles
// Period is defined as rising edge (low to high) to the next rising edge.
// Return 0 if there is not enough collected data to calculate period .
uint32_t Debug_Period(uint32_t mask){
// students write this for Lab 3
// This function should not alter the recorded data.
// AND each recorded data with mask,
//    if nonzero the signal is considered high.
//    if zero, the signal is considered low.
    g_last_data = 0; // instead of doing DataBuffer[i-1] to avoid out of bounds issue
    uint32_t last_time = 0;
    uint32_t res = 0;
    int8_t rise_count = -1;


// for every entry in the arrays:
    // compare current data buffer and previous data buffers' bit specified by mask
    // (if current data buffer = 1 && previous data = 0, then rising edge found)

    // if rising edge, then save associated time
        // if last_time = 0, then check next data because that means the data was first rising edge
        // last time - current time... add to running total (res)
        // increment a counter for rising edges
    // if NO rising edge, then check next data

// put average of periods into res and return



// DO NOT TOUCH
    for(uint8_t i=0; i < MAXBUF; i++) {
        if(((g_last_data & mask) == 0) && ((DataBuffer[i] & mask) != 0)) {
            if(last_time == 0) { // only execute once
                last_time = TimeBuffer[i];
                continue;
            }
            else{
                res = res + (last_time - TimeBuffer[i]); // running total of interval between rises
                last_time = TimeBuffer[i];
                rise_count++;
            }
        }
        g_last_data = DataBuffer[i];
    }
//    if(mask == 1) {
//        res -= 160000; // correct for case specific error
//    }

  return res/rise_count; // average period
}


// *****Debug_Duty******
// Calculate duty cycle of the recorded data using mask
// Input: mask specifies which bit(s) to observe
// Output: period in percent (0 to 100)
// Period is defined as rising edge (low to high) to the next rising edge.
// High is defined as rising edge (low to high) to the next falling edge.
// Duty cycle is (100*High)/Period
// Return 0 if there is not enough collected data to calculate duty cycle.
uint32_t Debug_Duty(uint32_t mask){
// optional for Lab 3
// This function should not alter the recorded data.
// AND each recorded data with mask,
//    if nonzero the signal is considered high.
//    if zero, the signal is considered low.


    // count clock cycles when high and clock cycles when low
    // H/(H+L) = duty cycle in .00 format.. need to multiply by 100 to get percent


        g_last_data = 0; // instead of doing DataBuffer[i-1] to avoid out of bounds issue
        uint32_t last_time = 0;
        uint32_t high = 0;
        uint32_t low = 0;
        uint32_t res;
//        int8_t rise_count = -1;

        for(uint8_t i=0; i < MAXBUF; i++) {
                if(((g_last_data & mask) == 0) && ((DataBuffer[i] & mask) != 0)) { // rising edge
                    if(last_time == 0) { // only execute once
                        last_time = TimeBuffer[i];
                        continue;
                    }
                    else {
                        high = high + (last_time - TimeBuffer[i]); // running total of high time
                        last_time = TimeBuffer[i];
                    }
                }

                if(((g_last_data & mask) != 0) && ((DataBuffer[i] & mask) == 0)) { // falling edge detect
                    low = low + (last_time - TimeBuffer[i]); // running total of low time
                    last_time = TimeBuffer[i];

                }
                g_last_data = DataBuffer[i];
            }

        res = (high*100)/(low+high); // need 100 to keep res from falling into range of 1<x<0
        res = (100 - res);
  return res;
}

// Lab2 specific debugging code
uint32_t Theperiod;
uint32_t Theduty;
void Dump(void){
  uint32_t out = GPIOB->DOUT31_0&0x0070000; // PB18-PB16 outputs
  uint32_t in = GPIOB->DIN31_0&0x0F;        // PB3-PB0 inputs
  uint32_t data = out|in;                   // PB18-PB16, PB3-PB0
  uint32_t result = Debug_Dump(data);       // calls your Lab3 function
  if(result == 0){ // 0 means full
    Theduty = Debug_Duty(1<<18); // extra credit function
    Theperiod = Debug_Period(1<<18);        // calls your Lab3 function
 //  __asm volatile("bkpt; \n"); // breakpoint here
// observe Theperiod
  }
}



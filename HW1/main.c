/* 
 * File:   main.c
 * Author: Ana
 *
 * Created on March 28, 2017, 5:04 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro

// DEVCFG0 (data sheet page 241-242)
#pragma config DEBUG = 11 // no debugging
#pragma config JTAGEN = 0 // no jtag
#pragma config ICESEL = 11 // use PGED1 and PGEC1
#pragma config PWP = 111111111 // no write protect
#pragma config BWP = 0 // no boot write protect
#pragma config CP = 1 // no code protect

// DEVCFG1 (data sheet page 243-244)
#pragma config FNOSC = 011 // use primary oscillator with pll
#pragma config FSOSCEN = 0 // turn off secondary oscillator
#pragma config IESO = 0 // no switching clocks
#pragma config POSCMOD = 10 // high speed crystal mode
#pragma config OSCIOFNC = 1 // free up secondary osc pins
#pragma config FPBDIV = 00 // divide CPU freq by 1 for peripheral bus clock
#pragma config FCKSM = 11 // do not enable clock switch
#pragma config WDTPS = 10100 // slowest wdt
#pragma config WINDIS = 1 // no wdt window
#pragma config FWDTEN = 0 // wdt off by default
#pragma config FWDTWINSZ = 11 // wdt window at 25%

// DEVCFG2 - get the CPU clock to 48MHz (data sheet page 245-246)
#pragma config FPLLIDIV = 001 // divide input clock to be in range 4-5MHz (starts at 8, divide by 2)
#pragma config FPLLMUL = 111 // multiply clock after FPLLIDIV (multiply by 24 to get 96)
#pragma config FPLLODIV = 001 // divide clock after FPLLMUL to get 48MHz (divide by 2 to get 48)
#pragma config UPLLIDIV = 001 // divider for the 8MHz input clock, then multiply by 12 to get 48MHz for USB (divide by 2?)
#pragma config UPLLEN = 0 // USB clock on

// DEVCFG3 (data sheet page 247)
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = 0 // allow multiple reconfigurations
#pragma config IOL1WAY = 0 // allow multiple reconfigurations
#pragma config FUSBIDIO = 1 // USB pins controlled by USB module
#pragma config FVBUSONIO = 1 // USB BUSON controlled by USB module


int main(int argc, char** argv) {

    __builtin_disable_interrupts();

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // do your TRIS and LAT commands here
    TRISBbits.TRISB4 = 1; // make push button an input
    TRISAbits.TRISA4 = 0; // make green LED an output
    LATAbits.LATA4 = 1; // set green LED initially high

    __builtin_enable_interrupts();

    while(1) {
        
        while(PORTBbits.RB4 == 0){ // while button is pressed
            // do nothing
        }
        
        _CP0_SET_COUNT(0); // set the Core Timer to 0
              
        while (_CP0_GET_COUNT() < 12000) { // if count less than 0.5ms 
            // do nothing
        }
                  
        if (PORTAbits.RA4 == 1) { // if LED is on
            LATAbits.LATA4 = 0; // turn off LED
        }
        else { // if LED is off
            LATAbits.LATA4 = 1; // turn on LED
        }
}
}


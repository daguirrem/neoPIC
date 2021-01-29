/* 
 * Archivo : main.c
 * Autor   : David A. Aguirre Morales
 * Contacto: daguirre.m@outlook.com
 * 
 * Fecha de creación:    09/01/2021
 * Última modificiación: 27/01/2021
 *
 * Descripción: 
 */

/* Documentos:
 * Neopixel (WS2812b) datasheet:
 * https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf 
 */

/* XC 2.31
 * OPTIMIZACIÓN -O2
 * MPLAB 5.45
 * 16F877A @20MHz
 */

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = ON
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#include "system.h"
#include <stdint.h>
#include <xc.h>

#include "neopixel.h"

void main(void)
{
    /* Salida: (RC2) */
    PORTC = 0;
    TRISCbits.TRISC2 = 0;
    
    /* (Para proteus) */
    __delay_ms(10);

    /* Datos RGB del LED */
    /* { G, R, B } */
    uint8_t leds[3][3] = {
        { 255, 0, 0 },
        { 0, 255, 0 },
        { 0, 0, 255 }
    }; 
    
    neoPixel_send_led(&leds[0][0]);
    neoPixel_send_led(&leds[1][0]);
    neoPixel_send_led(&leds[2][0]);
    
    //neoPixel_update(&leds[0][0], 3);
    /* (Proteus delay) */
    __delay_ms(10);
    
    while (1) {
        
    }
}

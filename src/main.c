/* 
 * Archivo : main.c
 * Autor   : David A. Aguirre Morales
 * Contacto: daguirre.m@outlook.com
 * 
 * Fecha de creación:    09/01/2021
 * Última modificiación: 09/01/2021
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
 * 16F877A, solo pruebas, el 16F877A NO SOPORTA 32MHz 
 * alternativa (16F171x, Ej. 9) 
 */

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = ON
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#include <stdint.h>
#include "system.h"
#include <xc.h>

void main(void)
{
    /* Salida: (RC2) */
    PORTC = 0;
    TRISCbits.TRISC2 = 0;
    
    /* (Para proteus) */
    __delay_ms(10);
    
    /* Datos RGB del LED */
    uint8_t rgb[3] = {0,0,0xCA}; /* { B, G, R } */
    
    /* PRUEBAS--------------------------------------------------------------- */
    /* Selección de banco de registros PORTx (No aplica 18Fxxxx)*/
    asm(
        "BCF STATUS, 5  \n"
        "BCF STATUS, 6"
    );
    
    /* Para cada color, realice la secuencia de envío de byte */
    for (uint8_t x = 0; x < 3; x++) {
        volatile uint8_t byte = rgb[x];
        volatile uint8_t i = 8;
        
        /* Para cada bit en /byte, compruebe y genere la forma de onda
         * correspondiente, se envía de menos a más significativo */
        asm(
            /* 1 */
            "TESTBIT:                       \n" // Compruebe byte [0:0]
            "BTFSC  main@byte, 0            \n" 
            "GOTO   CODE1                   \n"
        
            "CODE0:                         \n" // Si byte [0:0] es '0'
            "BSF PORTC, 2;                  \n" // 125nS        ON
            "NOP;                           \n" // 125nS
            "NOP;                           \n" // 125nS
            "NOP;                           \n" // 125nS = ~0.5uS
            "BCF PORTC, 2;                  \n" // OFF -----------
            "GOTO CORRIMIENTO               \n" // 125nS
        
            "CODE1:                         \n" // Si byte [0:0] es '1'
            "BSF PORTC, 2;                  \n" // 125nS        ON
            "NOP;                           \n" // 125nS 
            "NOP;                           \n" // 125nS 
            "NOP;                           \n" // 125nS 
            "NOP;                           \n" // 125nS 
            "NOP;                           \n" // 125nS = ~0.75uS
            "BCF PORTC, 2;                  \n" // OFF -----------
        
            "CORRIMIENTO:                   \n"
            "BCF    STATUS, 0               \n"
            "RRF    main@byte, 1            \n" // byte >>= 1;
            "DECFSZ main@i, F               \n" // i -= 1;
            "GOTO   TESTBIT                 \n" // if (i != 0) GOTO TESTBIT;
        );
    }
    
    /* (Proteus delay) */
    __delay_ms(1);
    
    while (1) {
        
    }
}

/* NOTAS:
 * 1. Periodo de instrucción:
 *  4 ciclos por instrucción
 *  1 NOP a Fosc:32MHz = 125nS (4/32E6)
 */

// OBSOLETO --------------------------------------------------------------------

//inline void __attribute__((always_inline)) send_bit (void)
//{
//}

//void __interrupt() ISR_handler()
//{
//    PORTCbits.RC2 ^= 1;
//    PIR1bits.TMR1IF = 0;
//    TMR1 = 0xFFFF - 2;
//}

//    for (uint8_t x = 0; x < 3; x++) {
//        
//        for (uint8_t i = 0; i < 8; i++) {
//
//            if (byte & 0x1) {
//                /* 1 CODE*/
//                asm(
//                    "BSF PORTC, 2;  \n" // 125nS
//                    "NOP;           \n" // 125nS 
//                    "NOP;           \n" // 125nS 
//                    "NOP;           \n" // 125nS 
//                    "NOP;           \n" // 125nS 
//                    "NOP;           \n" // 125nS = ~0.75uS
//                    "BCF PORTC, 2;  \n" // 125nS 
//                    "NOP;           \n" // 125nS
//                    "NOP;           \n" // 125nS
//                    "NOP;"              // 125nS = ~0.5uS
//                );
//
//            } else {
//                /* 0 CODE*/
//                asm(
//                    "BSF PORTC, 2;  \n" // 125nS
//                    "NOP;           \n" // 125nS
//                    "NOP;           \n" // 125nS
//                    "NOP;           \n" // 125nS = ~0.5uS
//                    "BCF PORTC, 2;  \n" // 125nS
//                    "NOP;           \n" // 125nS 
//                    "NOP;           \n" // 125nS 
//                    "NOP;           \n" // 125nS 
//                    "NOP;           \n" // 125nS 
//                    "NOP;"              // 125nS = ~0.75uS
//                );
//            }
//            byte >>= 1;
//        }
//    }

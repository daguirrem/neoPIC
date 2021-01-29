/* 
 * Archivo : neopixel.c
 * Autor   : David A. Aguirre Morales
 * Contacto: daguirre.m@outlook.com
 * 
 * Fecha de creación:    27/01/2021
 * Última modificiación: 27/01/2021
 *
 * Descripción: 
 */
#include <xc.h>
#include "neopixel.h"

void neoPixel_send_led(uint8_t *rgb)
{
    /* PRUEBAS--------------------------------------------------------------- */
    /* Selección de banco de registros PORTx (No aplica 18Fxxxx)*/
    asm(
        "BCF STATUS, 5;  \n"
        "BCF STATUS, 6;"
    );
    volatile uint8_t rgbc = (uint8_t)rgb; // ANTI optimización del compilador
    
    /* Para cada color, realice la secuencia de envío de byte */
    //for (uint8_t x = 0; x < 3; x++) {
    volatile uint8_t x = 3;
    asm(
        "MOVF   neoPixel_send_led@rgbc,W;\n" // Direccionamiento indirecto de 
        "MOVWF  FSR;                    \n"  // rgb (apuntador) byte = &rgb[0]
    );
    
    asm("SEND_COLOR:");
    volatile uint8_t i = 0;
        
    asm(
        "MOVLW  0x8;                    \n"
        "MOVWF  neoPixel_send_led@i;    \n"
    );
    
    /* Para cada bit en / *byte, compruebe y genére la forma de onda
     * correspondiente; Se envía de más a menos significativo.
     * notación:
     * *byte = INDF (contenido puntero)
     *  byte = FSR (dirección apuntador) 
     */
    asm(
        /* 1 */
        "TESTBIT:                       \n" // Compruebe *byte[7:7]
        "BTFSC  INDF, 7;                \n" 
        "GOTO   CODE1;                  \n"

        "CODE0:                         \n" // Si *byte[7:7] es '0'
        "BSF    NPORT, NPIN;            \n" // 200nS        ON
        "NOP;                           \n" // 200nS = ~0.4uS
        "BCF    NPORT, NPIN;            \n" // OFF -----------
        "GOTO   CORRIMIENTO;            \n" 

        "CODE1:                         \n" // Si *byte[7:7] es '1'
        "BSF    NPORT, NPIN;            \n" // 200nS        ON
        "NOP;                           \n" // 200nS 
        "NOP;                           \n" // 200nS 
        "NOP;                           \n" // 200nS = ~0.8uS
        "BCF    NPORT, NPIN;            \n" // OFF -----------

        "CORRIMIENTO:                   \n"
        "BCF    STATUS, 0;              \n"
        "RLF    INDF, 1;                \n" // *byte <<= 1;
        "DECFSZ neoPixel_send_led@i;    \n" // i -= 1;
        "GOTO   TESTBIT;                \n" // if (i > 0) GOTO TESTBIT;
    );
    
    asm (
        "INCF   FSR, F;                 \n" // byte += 1; (siguiente dirección)
        "DECFSZ neoPixel_send_led@x, F; \n" // a apuntar
        "GOTO   SEND_COLOR;"                // if (x > 0) goto SEND_COLOR;
    );
}

void neoPixel_update(uint8_t *leds, uint16_t size)
{
    do {
        neoPixel_send_led(leds);
        leds += 3;
    } while (--size);
    __delay_us(50);
}

/* NOTAS:
 * 1. Periodo de instrucción:
 *  4 ciclos por instrucción
 *  1 NOP a Fosc:20MHz = 200nS (4/20E6)
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

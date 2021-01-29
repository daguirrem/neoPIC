/* 
 * Archivo : neopixel.h
 * Autor   : David A. Aguirre Morales
 * Contacto: daguirre.m@outlook.com
 * 
 * Fecha de creación:    27/01/2021
 * Última modificiación: 27/01/2021
 *
 * Descripción: 
 */
#ifndef _NEOPIXEL_H_
#define _NEOPIXEL_H_

#include <stdint.h> 
#include "system.h"

asm(
    "NPORT EQU PORTC    \n"
    "NPIN  EQU 2"
);

void neoPixel_send_led(uint8_t *rgb);
void neoPixel_update(uint8_t *leds, uint16_t size);

#endif /* _NEOPIXEL_H_ */
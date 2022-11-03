/******************************************************************
Universidad del Valle de Guatemala
IE2023 Programación de Microcontroladores
Autor: Fernando Donis
Compilador: PIC-AS (v2.36), MPLAB X IDE (v6.00)
Proyecto: Laboratorio 4
Hardware: PIC16F887
Creado: 24/08/2022
Última Modificación: 17/09/2022
******************************************************************/
#include "oscilador.h"

void setupINTOSC(uint8_t IRCF){
    
    
    if(IRCF == 7){
        OSCCONbits.IRCF = 0b111;       // 8 MHz
    }
    if(IRCF == 6){
        OSCCONbits.IRCF = 0b110;       // 4 MHz
    }
    
    OSCCONbits.SCS = 1;
}


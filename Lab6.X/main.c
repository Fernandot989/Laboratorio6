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


// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


#include <xc.h>
#include <pic16f887.h>
#include <stdint.h>
#include "oscilador.h"

#define _XTAL_FREQ 8000000

//******************************************************************************
// Variables
//******************************************************************************
int i = 0, conversion;
unsigned int ADC;
char recibido;
//******************************************************************************
// Prototipos de funciones
//******************************************************************************
void setup(void);
void setupADC(void);
void initUART(void);
void cambio(int pot);
void mandar(unsigned char *cadena);

//******************************************************************************
// Interrupciones
//******************************************************************************

void __interrupt() isr (void){
    if(PIR1bits.ADIF){
        RB7 = 1;
        ADIF = 0;
    TXIF = 0;
    }
}

//******************************************************************************
// Código Principal
//******************************************************************************
void main(void) {
    
    setup();
    setupINTOSC(7);         // 8 MHz
    setupADC();
    initUART();
    
    while(1){
         //Prelaboratorio
        //Puerto B aumentarà su valor]
            PORTD++;
        //Si TMRT es igual a 1, esta instrucciòn se ejecutarà
            if(TXSTAbits.TRMT == 1){
        //TXREG toma el valor que PORTD tenga y lo proyectarà en la pantalla
            TXREG = PORTD;
        }
        //Si RCIF es igual a 1 esta instrucciòn de ejecutarà
        if(PIR1bits.RCIF == 1){
        //PORTB toma el valor que se le ingresè a la terminal
            PORTB = RCREG;
            PIR1bits.RCIF = 0;
        }
        __delay_ms(500);
        
        //Laboratorio
        //mandar("Hola mundo. Hello world");
        //__delay_ms(10000);
        /* //Postlaboratorio
        mandar("\rs para leer potenciometro \r");
        mandar("\rn enviar ASCII \r");
        while(RCIF == 0){;}
        ADCON0bits.CHS = 0b0001;
        __delay_us(100);
        ADCON0bits.GO = 1;
        while(ADCON0bits.GO == 1){;}
        ADC = ADRESH;
        cambio(ADC);
        CCPR1L = conversion;
        __delay_us(100);
        recibido = RCREG;
        
        if(recibido == 's'){
            mandar("\rvalor \r");
            TXREG = CCPR1L;
        }
        else if (recibido == 'n'){
            mandar("\rvalor ASCII \r");
            while(PIR1bits.RCIF == 0){;}
            PORTB = CCPR1L;
        }
        else{
            mandar("ingresar valor solicitado"); 
        }  */
    }
}
//Aqui se hace la conversion del ADC]
void cambio(int pot){
    conversion = (unsigned short)(48+((float)(207)/(255))*(pot-0));
}
//Aqui se manda el texto del pic a la computadora
void mandar(unsigned char *cadena){
    while(*cadena != '\0'){
        while(TXIF != 1);
        TXREG = *cadena;
        *cadena++;
    }
}
//******************************************************************************
// Función para configurar GPIOs
//******************************************************************************
void setup(void){
    TRISB = 0;
    TRISD = 0;
    
    PORTB = 0;
    PORTD = 0;
    
    GIE = 1;
    ADIE = 1;
    TMR0IE = 1;
    ADIF = 0;
}

//******************************************************************************
// Función para configurar UART
//******************************************************************************
void initUART(void){
    // Paso 1: configurar velocidad baud rate
    SPBRG = 12;
    // Paso 2:
    TXSTAbits.SYNC = 0;         // Trabajaremos de forma asincrona
    RCSTAbits.SPEN = 1;         // habilitamos módulo UART
    // Paso 3:
    // Saltamos porque no usamos 9 bits
    // Paso 4:
    TXSTAbits.TXEN = 1;         // Habilitamos la transmision
    PIR1bits.TXIF = 0;
    RCSTAbits.CREN = 1;         // Habilitamos la recepcion
}

void setupADC(void){
    TRISAbits.TRISA1 = 1;
    ANSELbits.ANS1 = 1;
    
    ADCON0bits.ADCS1 = 0;
    ADCON0bits.ADCS0 = 1;
    
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.VCFG0 = 0;
    
    ADCON1bits.ADFM = 0;
    ADCON0bits.CHS = 0b0001;
    ADCON0bits.ADON = 1;
    __delay_us(100);
}


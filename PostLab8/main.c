#include <stdint.h>

#include <stdbool.h>

#include "inc/hw_ints.h"

#include "inc/hw_types.h"

#include "inc/hw_memmap.h"

#include "driverlib/debug.h"

#include "driverlib/fpu.h"

#include "driverlib/gpio.h"

#include "driverlib/interrupt.h"

#include "driverlib/sysctl.h"

#include "driverlib/uart.h"

#include "driverlib/rom.h"

#include "grlib/grlib.h"

#include "inc/tm4c123gh6pm.h"

#include "driverlib/timer.h"

//-----------------------------------------------------------------------------
//                                 Variables
//-----------------------------------------------------------------------------

uint8_t contador = 0;
uint8_t LED = 2;
uint32_t period;

unsigned char letra;

//-----------------------------------------------------------------------------
//                          Prototipos de Funciones
//-----------------------------------------------------------------------------

void main(void);
void semaforo(void);
void InitTMR0A(void);
void InitUART0(void);
void Timer0IntHandler(void);
void UART0IntHandler(void);

//-----------------------------------------------------------------------------
//                               Código Principal
//-----------------------------------------------------------------------------

void main(void){

    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_INT); //Configuramos el oscilador interno (PLL) a 40MHz

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); //Habilitamos el puerto B
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //Habilitamos el puerto F

    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2); //Configuramos el pin 2 del puerto B como salida
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);  //Configuramos los pines 1,2 y 3 del puerto F como salida

    IntMasterEnable(); //Habilitamos las interrupciones globales

    InitTMR0A();
    InitUART0();

    while(1) //Loop principal
    {
        ;
    }

}

//-----------------------------------------------------------------------------
//                                Funciones
//-----------------------------------------------------------------------------

void InitTMR0A(void){

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); //Habilitamos el TMR0 (16/32 bit) Normal
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC); //Configuramso el TMR0 como temporizador de ancho completo y periódico

    period = (SysCtlClockGet()/2); //Cálculo del período del TMR0
    TimerLoadSet(TIMER0_BASE, TIMER_A, period-1); // Establecemos el período del TMR0

    IntEnable(INT_TIMER0A); //Habilitamos la interrupción del TMR0A
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); //Especificamos que la interrupción ocurra al acabarse el tiempo
    TimerEnable(TIMER0_BASE, TIMER_A); // Habilitamos el TIMER0A

}

void InitUART0(void){

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //Habilitamos los periféricos del puerto A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); //Habilitamos los periféricos para el UART0
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0|GPIO_PIN_1); //Habilitamos los pines 0 y 1 del puerto A para el UART

    // Configuración de lo parámetros para el UART0 = (Module UART0, Baud Rate 115200, 8 data bits, 1 stop bit, None parity)
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE) );

    IntEnable(INT_UART0); //Habilitamos la interrupción del UART0
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); //Especificamos que la interrupción ocurra al recibir datos

}

//-----------------------------------------------------------------------------
//                                 Handlers
//-----------------------------------------------------------------------------

void Timer0IntHandler(void){

    //Limpiamos la interrupción del Timer0
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Toggle de las GPIO del puerto B
    if (!GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_2)){
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0x04);
    }

    else{
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0x00);
    }

    // Toggle del LED
    if (contador == 0){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
    }
    else if (contador == 1){ //Si enviamos una "r"
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1)){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00); //Si la LED estaba encendida, apagarla
        }
        else{
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x02); //Si la LED estaba apagada, encenderla
        }
    }
    else if (contador == 2){ //Si enviamos una "g"
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2)){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00); //Si la LED estaba encendida, apagarla
        }
        else{
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x04); //Si la LED estaba apagada, encenderla
        }
    }
    else if (contador == 3){ //Si enviamos una "b"
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3)){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00); //Si la LED estaba encendida, apagarla
        }
        else{
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x08); //Si la LED estaba apagada, encenderla
        }
    }
    else{
        ;
    }

}
void UART0IntHandler(void){

    UARTIntClear(UART0_BASE, UART_INT_RX | UART_INT_RT); //Limpiamos los puertos de recepción

    letra = UARTCharGet(UART0_BASE); //Guardamos el carácter recibido

    if (letra == 'r'){
        if (contador == 1){
            contador = 0;
        }
        else{
            contador = 1;
        }
    }

    else if (letra == 'b'){
        if (contador == 2){
            contador = 0;
        }
        else{
            contador = 2;
        }
    }

    else if (letra == 'g'){
        if (contador == 3){
            contador = 0;
        }
        else{
            contador = 3;
        }
    }

    else{
        ;
    }

}

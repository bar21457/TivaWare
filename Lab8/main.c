#include <stdint.h>

#include <stdbool.h>

#include "inc/hw_types.h"

#include "inc/hw_memmap.h"

#include "driverlib/sysctl.h"

#include "driverlib/pin_map.h"

#include "driverlib/debug.h"

#include "driverlib/gpio.h"

#define LED_RED GPIO_PIN_1 // Defininmos el pin de la LED roja
#define LED_BLUE GPIO_PIN_2 // Defininmos el pin de la LED azul
#define LED_GREEN GPIO_PIN_3 // Defininmos el pin de la LED verde
#define BUTTON GPIO_PIN_4 // Defininmos el pin del botón

void initLEDs(void) // Función para configurar los pines de la LED como salidas
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Habilitamos el puerto F
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)); // Esperamos a que el puerto F esté listo
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED|LED_BLUE|LED_GREEN); // Configuramos los pines de la LED como salidas
}

void initButton(void) // Función para configurar el pin del botón como entrada
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Habilitamos el puerto F
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)); // Esperamos a que el puerto F esté listo
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, BUTTON); // Configuramos el pin del botón como entrada
    GPIOPadConfigSet(GPIO_PORTF_BASE, BUTTON, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU); // Activamos la resistencia de pull-up del botón
}

uint8_t readButton(void) // Función para leer el estado del botón
{
    if(GPIOPinRead(GPIO_PORTF_BASE, BUTTON) == 0) // Si el botón está presionado
    {
        SysCtlDelay(20000); // Esperamos un tiempo para evitar rebote
        if(GPIOPinRead(GPIO_PORTF_BASE, BUTTON) == 0) // Verificamos de nuevo si el botón está presionado
        {
            return 0; // Devolvemos un 0 si el botón está presionado
        }
    }
    return 1; // Si el botón no está presionado, devolvemos un 1
}

int main(void)
{
    initLEDs(); // Configuramos los pines de la LED
    initButton(); // Configuramos el pin del botón

    while(1) // Loop principal
    {
        if(readButton() == 0) // Si el botón está presionado
        {

            // Semáforo en Verde
            GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN|LED_BLUE|LED_RED, 0b1000);
            SysCtlDelay(10000000);

            // Semáforo en Verde parpadeante
            GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN|LED_BLUE|LED_RED, 0b1000);
            SysCtlDelay(2000000);
            GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN|LED_BLUE|LED_RED, 0b0000);
            SysCtlDelay(2000000);
            GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN|LED_BLUE|LED_RED, 0b1000);
            SysCtlDelay(2000000);
            GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN|LED_BLUE|LED_RED, 0b0000);
            SysCtlDelay(2000000);
            GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN|LED_BLUE|LED_RED, 0b1000);
            SysCtlDelay(2000000);
            GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN|LED_BLUE|LED_RED, 0b0000);
            SysCtlDelay(2000000);
            GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN|LED_BLUE|LED_RED, 0b1000);
            SysCtlDelay(2000000);
            GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN|LED_BLUE|LED_RED, 0b0000);
            SysCtlDelay(2000000);
            GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN|LED_BLUE|LED_RED, 0b1000);
            SysCtlDelay(2000000);
            GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN|LED_BLUE|LED_RED, 0b0000);
            SysCtlDelay(2000000);

            // Semáforo en Amarillo
            GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN|LED_BLUE|LED_RED, 0b1010);
            SysCtlDelay(10000000);

            // Semáforo en Rojo
            GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN|LED_BLUE|LED_RED, 0b0010);
            SysCtlDelay(10000000);

        }
        else // Si el botón no está presionado
        {
            GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN|LED_BLUE|LED_RED, 0b0000); // Apagamos todas las LEDs
        }
    }
}

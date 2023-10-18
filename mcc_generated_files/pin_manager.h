/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F46K42
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above
        MPLAB 	          :  MPLAB X 6.00	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set RB1 procedures
#define RB1_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define RB1_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define RB1_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define RB1_GetValue()              PORTBbits.RB1
#define RB1_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define RB1_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define RB1_SetPullup()             do { WPUBbits.WPUB1 = 1; } while(0)
#define RB1_ResetPullup()           do { WPUBbits.WPUB1 = 0; } while(0)
#define RB1_SetAnalogMode()         do { ANSELBbits.ANSELB1 = 1; } while(0)
#define RB1_SetDigitalMode()        do { ANSELBbits.ANSELB1 = 0; } while(0)

// get/set RB2 procedures
#define RB2_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define RB2_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define RB2_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define RB2_GetValue()              PORTBbits.RB2
#define RB2_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define RB2_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define RB2_SetPullup()             do { WPUBbits.WPUB2 = 1; } while(0)
#define RB2_ResetPullup()           do { WPUBbits.WPUB2 = 0; } while(0)
#define RB2_SetAnalogMode()         do { ANSELBbits.ANSELB2 = 1; } while(0)
#define RB2_SetDigitalMode()        do { ANSELBbits.ANSELB2 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()              PORTCbits.RC6
#define RC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetPullup()             do { WPUCbits.WPUC6 = 1; } while(0)
#define RC6_ResetPullup()           do { WPUCbits.WPUC6 = 0; } while(0)
#define RC6_SetAnalogMode()         do { ANSELCbits.ANSELC6 = 1; } while(0)
#define RC6_SetDigitalMode()        do { ANSELCbits.ANSELC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()              PORTCbits.RC7
#define RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetPullup()             do { WPUCbits.WPUC7 = 1; } while(0)
#define RC7_ResetPullup()           do { WPUCbits.WPUC7 = 0; } while(0)
#define RC7_SetAnalogMode()         do { ANSELCbits.ANSELC7 = 1; } while(0)
#define RC7_SetDigitalMode()        do { ANSELCbits.ANSELC7 = 0; } while(0)

// get/set LED_1 aliases
#define LED_1_TRIS                 TRISDbits.TRISD0
#define LED_1_LAT                  LATDbits.LATD0
#define LED_1_PORT                 PORTDbits.RD0
#define LED_1_WPU                  WPUDbits.WPUD0
#define LED_1_OD                   ODCONDbits.ODCD0
#define LED_1_ANS                  ANSELDbits.ANSELD0
#define LED_1_SetHigh()            do { LATDbits.LATD0 = 1; } while(0)
#define LED_1_SetLow()             do { LATDbits.LATD0 = 0; } while(0)
#define LED_1_Toggle()             do { LATDbits.LATD0 = ~LATDbits.LATD0; } while(0)
#define LED_1_GetValue()           PORTDbits.RD0
#define LED_1_SetDigitalInput()    do { TRISDbits.TRISD0 = 1; } while(0)
#define LED_1_SetDigitalOutput()   do { TRISDbits.TRISD0 = 0; } while(0)
#define LED_1_SetPullup()          do { WPUDbits.WPUD0 = 1; } while(0)
#define LED_1_ResetPullup()        do { WPUDbits.WPUD0 = 0; } while(0)
#define LED_1_SetPushPull()        do { ODCONDbits.ODCD0 = 0; } while(0)
#define LED_1_SetOpenDrain()       do { ODCONDbits.ODCD0 = 1; } while(0)
#define LED_1_SetAnalogMode()      do { ANSELDbits.ANSELD0 = 1; } while(0)
#define LED_1_SetDigitalMode()     do { ANSELDbits.ANSELD0 = 0; } while(0)

// get/set LED_2 aliases
#define LED_2_TRIS                 TRISDbits.TRISD1
#define LED_2_LAT                  LATDbits.LATD1
#define LED_2_PORT                 PORTDbits.RD1
#define LED_2_WPU                  WPUDbits.WPUD1
#define LED_2_OD                   ODCONDbits.ODCD1
#define LED_2_ANS                  ANSELDbits.ANSELD1
#define LED_2_SetHigh()            do { LATDbits.LATD1 = 1; } while(0)
#define LED_2_SetLow()             do { LATDbits.LATD1 = 0; } while(0)
#define LED_2_Toggle()             do { LATDbits.LATD1 = ~LATDbits.LATD1; } while(0)
#define LED_2_GetValue()           PORTDbits.RD1
#define LED_2_SetDigitalInput()    do { TRISDbits.TRISD1 = 1; } while(0)
#define LED_2_SetDigitalOutput()   do { TRISDbits.TRISD1 = 0; } while(0)
#define LED_2_SetPullup()          do { WPUDbits.WPUD1 = 1; } while(0)
#define LED_2_ResetPullup()        do { WPUDbits.WPUD1 = 0; } while(0)
#define LED_2_SetPushPull()        do { ODCONDbits.ODCD1 = 0; } while(0)
#define LED_2_SetOpenDrain()       do { ODCONDbits.ODCD1 = 1; } while(0)
#define LED_2_SetAnalogMode()      do { ANSELDbits.ANSELD1 = 1; } while(0)
#define LED_2_SetDigitalMode()     do { ANSELDbits.ANSELD1 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/
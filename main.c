/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F46K42
        Driver Version    :  2.00
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

#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/examples/i2c1_master_example.h"
#include "mcc_generated_files/i2c1_master.h"
#include "stdio.h"
#include "string.h"

#define MPU6050_ADDRESS 0x68 // Dado que AD0 est� conectado a GND
#define MPU6050_PWR_MGMT_1 0x6B

#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_ZOUT_H 0x3F

#define MPU6050_GYRO_XOUT_H  0x43
#define MPU6050_GYRO_YOUT_H  0x45
#define MPU6050_GYRO_ZOUT_H  0x47

#define EEPROM1_ADDRESS 0x50
#define EEPROM2_ADDRESS 0x54
#define MAX_BUFFER_SIZE 32

#define TEST_MEMORY_ADDRESS 0x0010 // Direcci�n de memoria arbitraria para la prueba


void UART_SendString(const char* str) {
    while (*str) {
        UART1_Write(*str);
        str++;
    }
}


void MPU6050_Init() {
    uint8_t data = 0x00; // Configuraci�n para despertar el MPU6050
    I2C1_Write1ByteRegister(MPU6050_ADDRESS, MPU6050_PWR_MGMT_1, data);
}


void MPU6050_ReadSensorData(float* ax, float* ay, float* az, float* gx, float* gy, float* gz) {
    uint8_t buffer[12];
    I2C1_ReadDataBlock(MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_H, buffer, 12);
    
    int16_t ax_raw = (buffer[0] << 8) | buffer[1];
    int16_t ay_raw = (buffer[2] << 8) | buffer[3];
    int16_t az_raw = (buffer[4] << 8) | buffer[5];
    int16_t gx_raw = (buffer[6] << 8) | buffer[7];
    int16_t gy_raw = (buffer[8] << 8) | buffer[9];
    int16_t gz_raw = (buffer[10] << 8) | buffer[11];

    *ax = ax_raw / 16384.0f;
    *ay = ay_raw / 16384.0f;
    *az = az_raw / 16384.0f;
    *gx = gx_raw / 131.0f;
    *gy = gy_raw / 131.0f;
    *gz = gz_raw / 131.0f;
}


void EEPROM_WriteByte(uint8_t deviceAddress, uint16_t memoryAddress, uint8_t data) {
    uint8_t buffer[3];
    buffer[0] = (memoryAddress >> 8) & 0xFF;  // Address High Byte
    buffer[1] = memoryAddress & 0xFF;         // Address Low Byte
    buffer[2] = data;                         // Data
    
    I2C1_WriteNBytes(deviceAddress, buffer, 3);
    
    __delay_ms(5);  // Espera para que la EEPROM complete la escritura
}


uint8_t EEPROM_ReadByte(uint8_t deviceAddress, uint16_t memoryAddress) {
    uint8_t addressBuffer[2];
    addressBuffer[0] = (memoryAddress >> 8) & 0xFF;  // Address High Byte
    addressBuffer[1] = memoryAddress & 0xFF;         // Address Low Byte

    I2C1_WriteNBytes(deviceAddress, addressBuffer, 2); // Send address to EEPROM

    uint8_t data = 0;
    I2C1_ReadNBytes(deviceAddress, &data, 1);  // Read one byte

    return data;
}


//main de solo eeprom
void main(void) {
    SYSTEM_Initialize();

    while(1) {
        
        // Escribir un byte en la EEPROM
        EEPROM_WriteByte(EEPROM1_ADDRESS, TEST_MEMORY_ADDRESS, 0xA5);
        UART_SendString("Dato Escrito en la EEPROM 1\n");
        
        // Leer inmediatamente despu�s de escribir
        uint8_t readData = EEPROM_ReadByte(EEPROM1_ADDRESS, TEST_MEMORY_ADDRESS);
        
        if(readData == 0xA5) {
            UART_SendString("Escritura 1 exitosa.\n");
        } else {
            UART_SendString("Escritura 1 fallida.\n");
        }
        
        
        // Enviar el dato le�do a trav�s de UART
        char buffer[50];
        sprintf(buffer, "Dato 1 leido: %X\n", readData);
        UART_SendString(buffer);
        __delay_ms(50);
        ////////////////////////////////////////////////////
        
        EEPROM_WriteByte(EEPROM2_ADDRESS, TEST_MEMORY_ADDRESS, 0xB6);
        UART_SendString("Dato Escrito en la EEPROM 2\n");
        
        // Leer inmediatamente despu�s de escribir
        uint8_t readData2 = EEPROM_ReadByte(EEPROM2_ADDRESS, TEST_MEMORY_ADDRESS);
        
        if(readData2 == 0xB6) {
            UART_SendString("Escritura 2 exitosa.\n");
        } else {
            UART_SendString("Escritura 2 fallida.\n");
        }
        char buffer2[50];
        sprintf(buffer2, "Dato 2 leido: %X\n", readData2);
        UART_SendString(buffer2);
        
        
        UART_SendString("Programa sigue corriendo...\n\n\n");
        __delay_ms(1000); // Esperar 1 segundo
    }
}

/*void main(void) {
    SYSTEM_Initialize();

    while(1) {
        
        // Escribir un byte en la EEPROM1
        EEPROM_WriteByte(EEPROM1_ADDRESS, TEST_MEMORY_ADDRESS, 0xA5);
        UART_SendString("Data written to EEPROM1.\n");
        
        // Leer inmediatamente despu�s de escribir de la EEPROM1
        uint8_t checkData1 = EEPROM_ReadByte(EEPROM1_ADDRESS, TEST_MEMORY_ADDRESS);
        if(checkData1 == 0xA5) {
            UART_SendString("EEPROM1 Write operation successful.\n");
        } else {
            UART_SendString("EEPROM1 Write operation failed.\n");
        }
        
        // Escribir un byte en la EEPROM2
        EEPROM_WriteByte(EEPROM2_ADDRESS, TEST_MEMORY_ADDRESS, 0xB6);
        UART_SendString("Data written to EEPROM2.\n");
        
        // Leer inmediatamente despu�s de escribir de la EEPROM2
        uint8_t checkData2 = EEPROM_ReadByte(EEPROM2_ADDRESS, TEST_MEMORY_ADDRESS);
        if(checkData2 == 0xB6) {
            UART_SendString("EEPROM2 Write operation successful.\n");
        } else {
            UART_SendString("EEPROM2 Write operation failed.\n");
        }
        
        // Leer el byte de la EEPROM1 para mostrarlo
        uint8_t readData1 = EEPROM_ReadByte(EEPROM1_ADDRESS, TEST_MEMORY_ADDRESS);
        char buffer1[50];
        sprintf(buffer1, "EEPROM1 Read Data: %X\n", readData1);
        UART_SendString(buffer1);
        
        // Leer el byte de la EEPROM2 para mostrarlo
        uint8_t readData2 = EEPROM_ReadByte(EEPROM2_ADDRESS, TEST_MEMORY_ADDRESS);
        char buffer2[50];
        sprintf(buffer2, "EEPROM2 Read Data: %X\n", readData2);
        UART_SendString(buffer2);
        
        UART_SendString("Program is running...\n");
        __delay_ms(1000); // Esperar 1 segundo
    }
}/*

//supuesto main completo
/*void main(void) {
    SYSTEM_Initialize();
    MPU6050_Init();

    char buffer[200];
    uint8_t eepromBuffer[24];
    float ax, ay, az, gx, gy, gz;

    while(1) {
        // Read data from MPU6050
        MPU6050_ReadSensorData(&ax, &ay, &az, &gx, &gy, &gz);

        // Format the data as a string
        sprintf(buffer, "BEFORE EEPROM - AX: %.2f, AY: %.2f, AZ: %.2f, GX: %.2f, GY: %.2f, GZ: %.2f\n", ax, ay, az, gx, gy, gz);
        UART_SendString(buffer);

        // Store the data in the eepromBuffer
        memcpy(&eepromBuffer[0], &ax, sizeof(float));
        memcpy(&eepromBuffer[4], &ay, sizeof(float));
        memcpy(&eepromBuffer[8], &az, sizeof(float));
        memcpy(&eepromBuffer[12], &gx, sizeof(float));
        memcpy(&eepromBuffer[16], &gy, sizeof(float));
        memcpy(&eepromBuffer[20], &gz, sizeof(float));

        // Write accelerometer data to EEPROM1
        EEPROM_Write(EEPROM1_ADDRESS, 0x0000, eepromBuffer, 12);
        // Write gyroscope data to EEPROM2
        EEPROM_Write(EEPROM2_ADDRESS, 0x0000, &eepromBuffer[12], 12);

        // Read accelerometer data from EEPROM1
        EEPROM_Read(EEPROM1_ADDRESS, 0x0000, eepromBuffer, 12);
        // Read gyroscope data from EEPROM2
        EEPROM_Read(EEPROM2_ADDRESS, 0x0000, &eepromBuffer[12], 12);

        // Extract the values from the eepromBuffer
        memcpy(&ax, &eepromBuffer[0], sizeof(float));
        memcpy(&ay, &eepromBuffer[4], sizeof(float));
        memcpy(&az, &eepromBuffer[8], sizeof(float));
        memcpy(&gx, &eepromBuffer[12], sizeof(float));
        memcpy(&gy, &eepromBuffer[16], sizeof(float));
        memcpy(&gz, &eepromBuffer[20], sizeof(float));

        // Format the data as a string
        sprintf(buffer, "AFTER EEPROM - AX: %.2f, AY: %.2f, AZ: %.2f, GX: %.2f, GY: %.2f, GZ: %.2f\n", ax, ay, az, gx, gy, gz);
        UART_SendString(buffer);

        __delay_ms(1000);
    }
}*/





//main que solo recibe del sensor
/*void main(void) {
    SYSTEM_Initialize();
    MPU6050_Init();

    char buffer[200];
    float ax, ay, az, gx, gy, gz;

    while(1) {
        MPU6050_ReadSensorData(&ax, &ay, &az, &gx, &gy, &gz);

        sprintf(buffer, "AX: %.2f, AY: %.2f, AZ: %.2f, GX: %.2f, GY: %.2f, GZ: %.2f\n", ax, ay, az, gx, gy, gz);
        UART_SendString(buffer);

        __delay_ms(1000);
    }
}*/

/**
 End of File
*/
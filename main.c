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
#include "C:\Users\mesas\MPLABXProjects\Practica3Intento2.X\mcc_generated_files\examples\i2c1_master_example.h"
#include "stdio.h"
#include "string.h"

#define MPU6050_ADDRESS 0x68 // Dado que AD0 está conectado a GND
#define MPU6050_PWR_MGMT_1   0x6B

#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_ZOUT_H 0x3F

#define MPU6050_GYRO_XOUT_H  0x43
#define MPU6050_GYRO_YOUT_H  0x45
#define MPU6050_GYRO_ZOUT_H  0x47

#define EEPROM1_ADDRESS 0x50
#define EEPROM2_ADDRESS 0x54
#define MAX_BUFFER_SIZE 32


void MPU6050_Init() {
    uint8_t data = 0x00; // Configuración para despertar el MPU6050
    I2C1_Write1ByteRegister(MPU6050_ADDRESS, MPU6050_PWR_MGMT_1, data);
}

void MPU6050_ReadBytes(uint8_t registerAddress, uint8_t* buffer, size_t length) {
    uint8_t startBuffer[2] = { MPU6050_ADDRESS << 1, registerAddress };

    I2C1_SetBuffer(startBuffer, 2);
    I2C1_MasterWrite();

    I2C1_SetBuffer(buffer, length);
    I2C1_MasterRead();
}

void EEPROM_Write(uint8_t deviceAddress, uint16_t memoryAddress, uint8_t* data, size_t length) {
    uint8_t buffer[MAX_BUFFER_SIZE + 3];
    buffer[0] = (uint8_t)(deviceAddress << 1);
    buffer[1] = (memoryAddress >> 8) & 0xFF;
    buffer[2] = memoryAddress & 0xFF;
    memcpy(&buffer[3], data, length);

    I2C1_SetBuffer(buffer, length + 3);
    I2C1_MasterWrite();

    __delay_ms(5); // Espera para que la EEPROM complete la escritura
}

void EEPROM_Read(uint8_t deviceAddress, uint16_t memoryAddress, uint8_t* data, size_t length) {
    uint8_t startBuffer[3] = { (uint8_t)(deviceAddress << 1), (memoryAddress >> 8) & 0xFF, memoryAddress & 0xFF };

    I2C1_SetBuffer(startBuffer, 3);
    I2C1_MasterWrite();

    I2C1_SetBuffer(data, length);
    I2C1_MasterRead();
}

void MPU6050_ReadAccelerometer(float* ax, float* ay, float* az) {
    uint8_t buffer[6];
    I2C1_ReadDataBlock(MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_H, buffer, 6);
    
    int16_t ax_raw = (buffer[0] << 8) | buffer[1];
    int16_t ay_raw = (buffer[2] << 8) | buffer[3];
    int16_t az_raw = (buffer[4] << 8) | buffer[5];

    *ax = ax_raw / 16384.0f;
    *ay = ay_raw / 16384.0f;
    *az = az_raw / 16384.0f;
}

void MPU6050_ReadGyroscope(float* gx, float* gy, float* gz) {
    uint8_t buffer[6];
    I2C1_ReadDataBlock(MPU6050_ADDRESS, MPU6050_GYRO_XOUT_H, buffer, 6);
    
    int16_t gx_raw = (buffer[0] << 8) | buffer[1];
    int16_t gy_raw = (buffer[2] << 8) | buffer[3];
    int16_t gz_raw = (buffer[4] << 8) | buffer[5];

    *gx = gx_raw / 131.0f;
    *gy = gy_raw / 131.0f;
    *gz = gz_raw / 131.0f;
}

void UART_SendString(const char* str) {
    while (*str) {
        UART1_Write(*str);
        str++;
    }
}

void main(void) {
    SYSTEM_Initialize();
    MPU6050_Init();

    char buffer[200];
    float ax, ay, az, gx, gy, gz;

    while(1) {
        MPU6050_ReadAccelerometer(&ax, &ay, &az);
        MPU6050_ReadGyroscope(&gx, &gy, &gz);

        sprintf(buffer, "AX: %.2f, AY: %.2f, AZ: %.2f, GX: %.2f, GY: %.2f, GZ: %.2f\n", ax, ay, az, gx, gy, gz);
        UART_SendString(buffer);

        __delay_ms(1000);
    }
}




/*void main(void){
    // Initialize the device
    SYSTEM_Initialize();
    MPU6050_Init();

    // Buffer para almacenar los datos formateados como string
    char buffer[200];

    // Variables para almacenar los datos del MPU6050
    float ax, ay, az, gx, gy, gz;

    // Buffer para almacenar los datos antes de escribir en la EEPROM
    uint8_t eepromBuffer[24];

    while(1){
        
        LED_1_Toggle();
        // Lee datos del acelerómetro
        MPU6050_ReadAccelerometer(&ax, &ay, &az);

        // Lee datos del giroscopio
        MPU6050_ReadGyroscope(&gx, &gy, &gz);

        // Almacena los datos en el eepromBuffer
        memcpy(&eepromBuffer[0], &ax, sizeof(float));
        memcpy(&eepromBuffer[4], &ay, sizeof(float));
        memcpy(&eepromBuffer[8], &az, sizeof(float));
        memcpy(&eepromBuffer[12], &gx, sizeof(float));
        memcpy(&eepromBuffer[16], &gy, sizeof(float));
        memcpy(&eepromBuffer[20], &gz, sizeof(float));

        // Escribe los datos del acelerómetro en la EEPROM1
        EEPROM_Write(EEPROM1_ADDRESS, 0x0000, eepromBuffer, 12);

        // Escribe los datos del giroscopio en la EEPROM2
        EEPROM_Write(EEPROM2_ADDRESS, 0x0000, &eepromBuffer[12], 12);

        // Lee los datos del acelerómetro de la EEPROM1
        EEPROM_Read(EEPROM1_ADDRESS, 0x0000, eepromBuffer, 12);

        // Lee los datos del giroscopio de la EEPROM2
        EEPROM_Read(EEPROM2_ADDRESS, 0x0000, &eepromBuffer[12], 12);

        // Extrae los valores desde el eepromBuffer
        memcpy(&ax, &eepromBuffer[0], sizeof(float));
        memcpy(&ay, &eepromBuffer[4], sizeof(float));
        memcpy(&az, &eepromBuffer[8], sizeof(float));
        memcpy(&gx, &eepromBuffer[12], sizeof(float));
        memcpy(&gy, &eepromBuffer[16], sizeof(float));
        memcpy(&gz, &eepromBuffer[20], sizeof(float));

        // Formatea los datos como string
        sprintf(buffer, "AX: %.2f, AY: %.2f, AZ: %.2f, GX: %.2f, GY: %.2f, GZ: %.2f\n", ax, ay, az, gx, gy, gz);

        // Envía los datos a través de UART
        UART_SendString(buffer);

        // Espera antes de la próxima lectura (puedes ajustar este valor según tus necesidades)
        //__delay_ms(1000);
    }
}*/
/**
 End of File
*/
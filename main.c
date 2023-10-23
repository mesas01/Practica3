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

//librerias necesarias
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/examples/i2c1_master_example.h"
#include "mcc_generated_files/i2c1_master.h"
#include "stdio.h"
#include "string.h"

// Definiciones para MPU6050 y direcciones EEPROM
#define MPU6050_ADDRESS 0x68
#define MPU6050_PWR_MGMT_1 0x6B

#define MPU6050_ACCEL_XOUT_H 0x3B

#define EEPROM1_ADDRESS 0x50
#define EEPROM2_ADDRESS 0x51
#define TEST_MEMORY_ADDRESS 0x0010 // Dirección de memoria arbitraria para probar

#define BLOCK_SIZE 24
#define EEPROM_SIZE 32768
#define TOTAL_BLOCKS (EEPROM_SIZE / BLOCK_SIZE)


// Variables globales para rastrear bloques actuales
uint16_t currentBlockEEPROM1 = 0x0000;
uint16_t currentBlockEEPROM2 = 0x0000;

uint32_t Ndat = 0;  // Número de datos a almacenar
uint32_t Tm = 0;    // Tiempo de muestreo en milisegundos
bool logging = false;  // Flag para indicar si se está registrando

float ax, ay, az, gx, gy, gz;
uint8_t eepromBuffer[BLOCK_SIZE];  // Buffer temporal para almacenar los datos antes de escribir en la EEPROM

// Función para enviar cadenas a través de UART
void UART_SendString(const char* str){
    while (*str){
        UART1_Write(*str);
        str++;
    }
}

//inicializar el mpu
void MPU6050_Init(){
    uint8_t data = 0x00; // Configuración para despertar el MPU6050
    I2C1_Write1ByteRegister(MPU6050_ADDRESS, MPU6050_PWR_MGMT_1, data);
}

// Leer datos del MPU6050 y convertir a valores flotantes
void MPU6050_ReadSensorData(float* ax, float* ay, float* az, float* gx, float* gy, float* gz){
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

// Escribir un byte en la EEPROM
void EEPROM_WriteByte(uint8_t deviceAddress, uint16_t memoryAddress, uint8_t data){
    uint8_t buffer[3];
    buffer[0] = (memoryAddress >> 8) & 0xFF;  // Address High Byte
    buffer[1] = memoryAddress & 0xFF;         // Address Low Byte
    buffer[2] = data;                         // Data
    
    I2C1_WriteNBytes(deviceAddress, buffer, 3);
    
    __delay_ms(5);  // Espera para que la EEPROM complete la escritura
}

// Leer un byte de la EEPROM
uint8_t EEPROM_ReadByte(uint8_t deviceAddress, uint16_t memoryAddress){
    uint8_t addressBuffer[2];
    addressBuffer[0] = (memoryAddress >> 8) & 0xFF;  // Address High Byte
    addressBuffer[1] = memoryAddress & 0xFF;         // Address Low Byte
    
    I2C1_WriteNBytes(deviceAddress, addressBuffer, 2); // Send address to EEPROM

    uint8_t data = 0;
    I2C1_ReadNBytes(deviceAddress, &data, 1);  // Read one byte
    
    return data;
}


// Función para escribir un bloque en la EEPROM
void EEPROM_WriteBlock(uint8_t deviceAddress, uint16_t memoryAddress, uint8_t* data, uint8_t size){
    for (uint8_t i = 0; i < size; i++){
        EEPROM_WriteByte(deviceAddress, memoryAddress + i, data[i]);
    }
}

// Leer un bloque de datos de la EEPROM
void EEPROM_ReadBlock(uint8_t deviceAddress, uint16_t memoryAddress, uint8_t* data, uint8_t size){
    for (uint8_t i = 0; i < size; i++) {
        data[i] = EEPROM_ReadByte(deviceAddress, memoryAddress + i);
    }
}

// Recibir una cadena de texto a través de UA
char UART_ReceiveString(char* receivedString, uint8_t maxLength){
    char data;
    uint8_t index = 0;
    
    while (index < maxLength - 1){
        data = UART1_Read();   // Lee un byte del UART
        
        // Ignora caracteres de retorno de carro y salto de línea al principio
        if (index == 0 && (data == '\r' || data == '\n')) {
            continue;
        }
        
        if (data == '\r') {   // Verifica si es un retorno de carro
            break;
        } else {
            receivedString[index] = data;
            index++;
        }
    }
    
    receivedString[index] = '\0';  // Termina la cadena con un carácter nulo
    return index;  // Retorna la longitud de la cadena
}

// Función de retardo personalizada
void custom_delay_ms(uint32_t milliseconds) {
    while(milliseconds--) {
        __delay_ms(1);
    }
}

// Leer datos grabados en EEPROM y enviarlos por UART
void ReadAndSendEEPROMData(uint32_t Ndat, uint16_t startAddressEEPROM1, uint16_t startAddressEEPROM2) {

    uint16_t readBlockEEPROM1 = startAddressEEPROM1;
    uint16_t readBlockEEPROM2 = startAddressEEPROM2;
    uint8_t eepromReadBuffer[BLOCK_SIZE];
    float ax_read, ay_read, az_read, gx_read, gy_read, gz_read;  // Variables para almacenar datos leídos de la EEPROM
    char buffer[200];

    for (uint32_t i = 0; i < Ndat; i++) {
        // Leer y convertir los datos del acelerómetro de la EEPROM1
        EEPROM_ReadBlock(EEPROM1_ADDRESS, readBlockEEPROM1, eepromReadBuffer, 12);
        memcpy(&ax_read, &eepromReadBuffer[0], sizeof(float));
        memcpy(&ay_read, &eepromReadBuffer[4], sizeof(float));
        memcpy(&az_read, &eepromReadBuffer[8], sizeof(float));

        // Leer y convertir los datos del giroscopio de la EEPROM2
        EEPROM_ReadBlock(EEPROM2_ADDRESS, readBlockEEPROM2, &eepromReadBuffer[12], 12);
        memcpy(&gx_read, &eepromReadBuffer[12], sizeof(float));
        memcpy(&gy_read, &eepromReadBuffer[16], sizeof(float));
        memcpy(&gz_read, &eepromReadBuffer[20], sizeof(float));

        // Mostrar los datos leídos de la EEPROM en UART
        sprintf(buffer, "EEPROM - AX: %.2f, AY: %.2f, AZ: %.2f, GX: %.2f, GY: %.2f, GZ: %.2f\n", ax_read, ay_read, az_read, gx_read, gy_read, gz_read);
        UART_SendString(buffer);

        // Incrementa los índices de los bloques para la siguiente iteración
        readBlockEEPROM1 = (readBlockEEPROM1 + 12) % EEPROM_SIZE;
        readBlockEEPROM2 = (readBlockEEPROM2 + 12) % EEPROM_SIZE;
    }
}

// Iniciar proceso de grabación de datos del sensor en EEPROM
void StartLogging() {
    uint32_t loggedDataCount = 0;
    char buffer[200];
    while (loggedDataCount < Ndat) {
        // Realiza una lectura del MPU6050
        MPU6050_ReadSensorData(&ax, &ay, &az, &gx, &gy, &gz);

        // Convertir los datos float a bytes y almacenarlos en eepromBuffer
        memcpy(&eepromBuffer[0], &ax, sizeof(float));
        memcpy(&eepromBuffer[4], &ay, sizeof(float));
        memcpy(&eepromBuffer[8], &az, sizeof(float));
        memcpy(&eepromBuffer[12], &gx, sizeof(float));
        memcpy(&eepromBuffer[16], &gy, sizeof(float));
        memcpy(&eepromBuffer[20], &gz, sizeof(float));
        
        UART_SendString("Escribiendo en EEPROM1 en direccion: ");
        sprintf(buffer, "0x%X", currentBlockEEPROM1);
        UART_SendString(buffer);
        UART_SendString("\n");

        // Escribir los datos del acelerómetro (ax, ay, az) en la EEPROM1
        EEPROM_WriteBlock(EEPROM1_ADDRESS, currentBlockEEPROM1, eepromBuffer, 12);
        
        
        // Antes de escribir en la EEPROM2
        UART_SendString("Escribiendo en EEPROM2 en direccion: ");
        sprintf(buffer, "0x%X", currentBlockEEPROM2);
        UART_SendString(buffer);
        UART_SendString("\n");
        // Escribir los datos del giroscopio (gx, gy, gz) en la EEPROM2
        EEPROM_WriteBlock(EEPROM2_ADDRESS, currentBlockEEPROM2, &eepromBuffer[12], 12);

        // Incrementar los índices de los bloques y verificar si se ha alcanzado el final de la EEPROM
        currentBlockEEPROM1 = (currentBlockEEPROM1 + 12) % EEPROM_SIZE;
        currentBlockEEPROM2 = (currentBlockEEPROM2 + 12) % EEPROM_SIZE;

        loggedDataCount++;
        custom_delay_ms(Tm);
    }

    if (loggedDataCount == Ndat) {
    UART_SendString("°°°°°°°°°°°°LOG_OK°°°°°°°°°°°°\n");
    
    unsigned long startReadAddressEEPROM1 = (currentBlockEEPROM1 == 0 ? EEPROM_SIZE : currentBlockEEPROM1) - (Ndat * 12);
    unsigned long startReadAddressEEPROM2 = (currentBlockEEPROM2 == 0 ? EEPROM_SIZE : currentBlockEEPROM2) - (Ndat * 12);

    for (uint32_t i = 0; i < Ndat; i++) {
        // Mostrar la dirección de lectura actual de la EEPROM1
        UART_SendString("Leyendo de EEPROM1 desde direccion: ");
        sprintf(buffer, "0x%X", startReadAddressEEPROM1);
        UART_SendString(buffer);
        UART_SendString("\n");
        
        // Incrementar la dirección de lectura de la EEPROM1
        startReadAddressEEPROM1 = (startReadAddressEEPROM1 + 12) % EEPROM_SIZE;

        // Mostrar la dirección de lectura actual de la EEPROM2
        UART_SendString("Leyendo de EEPROM2 desde direccion: ");
        sprintf(buffer, "0x%X", startReadAddressEEPROM2);
        UART_SendString(buffer);
        UART_SendString("\n");
        
        // Incrementar la dirección de lectura de la EEPROM2
        startReadAddressEEPROM2 = (startReadAddressEEPROM2 + 12) % EEPROM_SIZE;
    }
    // Recuperar las direcciones de inicio originales
    startReadAddressEEPROM1 = (currentBlockEEPROM1 == 0 ? EEPROM_SIZE : currentBlockEEPROM1) - (Ndat * 12);
    startReadAddressEEPROM2 = (currentBlockEEPROM2 == 0 ? EEPROM_SIZE : currentBlockEEPROM2) - (Ndat * 12);
    
    ReadAndSendEEPROMData(Ndat, (uint16_t)startReadAddressEEPROM1, (uint16_t)startReadAddressEEPROM2);  // Leer y enviar datos de las EEPROMs
    
    } else {
        UART_SendString("LOG_ERR\n");
    }
    logging = false;  // Reset the logging flag
}

// Analizar la entrada del usuario y configurar parámetros para el registro de datos
bool ParseUserInput(const char* input, uint32_t* Tm, uint32_t* Ndat) {
    uint32_t frequencyHz; // Frecuencia en Hz
    // Intentamos analizar la entrada en el formato deseado
    if (sscanf(input, "LOG(%lu,%lu)\r\n", &frequencyHz, Ndat) == 2 || sscanf(input, "LOG(%lu,%lu)\n", &frequencyHz, Ndat) == 2) {
        
        // Verifica si la frecuencia supera los 400,000 Hz
        if (frequencyHz > 400000) {
            frequencyHz = 400000;
            UART_SendString("Frecuencia excedida. Ajustada a 400kHz.\n");
        }

        *Tm = 1000 / frequencyHz; // Calcula Tm en milisegundos a partir de la frecuencia

        return true;
    }
    return false;
}


void main(void){
    SYSTEM_Initialize();
    MPU6050_Init();
    
    char buffer[200];
    float ax_read, ay_read, az_read, gx_read, gy_read, gz_read;  // Variables para almacenar datos leídos de la EEPROM
    
    char userInput[20];  // Buffer para almacenar la entrada del usuario

    // Bucle principal
    while(1){
        // Esperamos la entrada del usuario para el comando LOG
        UART_SendString("Ingrese la frecuencia (max 400000) y muestras (ejemplo: LOG(400000,100)): ");
        // Limpiar el buffer userInput antes de recibir una nueva entrada
        memset(userInput, 0, sizeof(userInput));
        
        UART_ReceiveString(userInput, sizeof(userInput));
        
        // Impresión de depuración para mostrar lo que se recibe en userInput
        UART_SendString("Recibido: ");
        UART_SendString(userInput);
        UART_SendString("\n");

        if (ParseUserInput(userInput, &Tm, &Ndat)) {
            // Si se analizó correctamente, inicia el registro
            StartLogging();
        } else {
            // En caso contrario, envía un mensaje de error
            UART_SendString("Entrada no valida. Intente de nuevo.\n");
        }

        // Si no se está registrando, simplemente leemos el MPU y mostramos los datos
        if (!logging) {
            MPU6050_ReadSensorData(&ax, &ay, &az, &gx, &gy, &gz);
        }
    }
}
/*void main(void){
    SYSTEM_Initialize();
    MPU6050_Init();
    
    char buffer[200];
    float ax, ay, az, gx, gy, gz;
    float ax_read, ay_read, az_read, gx_read, gy_read, gz_read;  // Variables para almacenar datos leídos de la EEPROM
    uint8_t eepromBuffer[BLOCK_SIZE];  // Buffer temporal para almacenar los datos antes de escribir en la EEPROM
    
    char userInput[10];  // Buffer para almacenar la entrada del usuario
    uint32_t samplingRate;  // Tasa de muestreo en Hz
    uint32_t delayTime;  // Tiempo de retraso en ms entre muestras
    
    UART_SendString("Ingrese la frecuencia de muestreo (max 400kHz = 400000): ");
    UART_ReceiveString(userInput, sizeof(userInput));
    samplingRate = (uint32_t)atoi(userInput);  // Convierte la cadena a un valor numérico
    
    if (samplingRate > 400000){
        UART_SendString("Frecuencia ingresada es muy alta. Se establecera en 400kHz.\n");
        samplingRate = 400000;
    }
    delayTime = 1000 / samplingRate;  // Calcula el tiempo de retraso en ms


    while(1){
        MPU6050_ReadSensorData(&ax, &ay, &az, &gx, &gy, &gz);

        // Convertir los datos float a bytes y almacenarlos en eepromBuffer
        memcpy(&eepromBuffer[0], &ax, sizeof(float));
        memcpy(&eepromBuffer[4], &ay, sizeof(float));
        memcpy(&eepromBuffer[8], &az, sizeof(float));
        memcpy(&eepromBuffer[12], &gx, sizeof(float));
        memcpy(&eepromBuffer[16], &gy, sizeof(float));
        memcpy(&eepromBuffer[20], &gz, sizeof(float));

        // Escribir los datos del acelerómetro (ax, ay, az) en la EEPROM1
        EEPROM_WriteBlock(EEPROM1_ADDRESS, currentBlockEEPROM1, eepromBuffer, 12);
        
        // Leer y convertir los datos del acelerómetro de la EEPROM1
        EEPROM_ReadBlock(EEPROM1_ADDRESS, currentBlockEEPROM1, eepromBuffer, 12);
        memcpy(&ax_read, &eepromBuffer[0], sizeof(float));
        memcpy(&ay_read, &eepromBuffer[4], sizeof(float));
        memcpy(&az_read, &eepromBuffer[8], sizeof(float));

        // Escribir los datos del giroscopio (gx, gy, gz) en la EEPROM2
        EEPROM_WriteBlock(EEPROM2_ADDRESS, currentBlockEEPROM2, &eepromBuffer[12], 12);
        
        // Leer y convertir los datos del giroscopio de la EEPROM2
        EEPROM_ReadBlock(EEPROM2_ADDRESS, currentBlockEEPROM2, &eepromBuffer[12], 12);
        memcpy(&gx_read, &eepromBuffer[12], sizeof(float));
        memcpy(&gy_read, &eepromBuffer[16], sizeof(float));
        memcpy(&gz_read, &eepromBuffer[20], sizeof(float));

        // Incrementar los índices de los bloques y verificar si se ha alcanzado el final de la EEPROM
        currentBlockEEPROM1 = (currentBlockEEPROM1 + 1) % TOTAL_BLOCKS;
        currentBlockEEPROM2 = (currentBlockEEPROM2 + 1) % TOTAL_BLOCKS;
        
        // Mostrar los datos originales leídos en UART
        sprintf(buffer, "Original - AX: %.2f, AY: %.2f, AZ: %.2f, GX: %.2f, GY: %.2f, GZ: %.2f\n", ax, ay, az, gx, gy, gz);
        UART_SendString(buffer);

        // Mostrar los datos leídos de la EEPROM en UART
        sprintf(buffer, "____Read - AX: %.2f, AY: %.2f, AZ: %.2f, GX: %.2f, GY: %.2f, GZ: %.2f\n", ax_read, ay_read, az_read, gx_read, gy_read, gz_read);
        UART_SendString(buffer);

         custom_delay_ms(delayTime);
    }
}*/


/*void I2C_Scanner(void) {
    UART_SendString("Scanning I2C bus...\n");
    for(uint8_t address = 0; address < 128; address++){
        EEPROM_WriteByte(address, TEST_MEMORY_ADDRESS, 0xA5);
        __delay_ms(10);
        // Intenta leer de la dirección
        uint8_t data = EEPROM_ReadByte(address, TEST_MEMORY_ADDRESS);
        if(data == 0xA5){
            char buffer[50];
            sprintf(buffer, "I2C device found at address: 0x%X\n", address);
            UART_SendString(buffer);
        }
    }
    UART_SendString("Scan complete.\n");
}*/

    //timestamp
    //main que puede detectar la direccion de las memorias
    /*void main(void) {
        SYSTEM_Initialize();

        while(1) {
            I2C_Scanner();
            __delay_ms(5000); // Espera 5 segundos antes de escanear nuevamente
        }
    }*/

    /**
     End of File
    */
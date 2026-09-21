#include "i2c.h"

void Config_I2C1(void) {
    RCC->APB1ENR |= (0b1 << 21);
    I2C1->CR1 |= (0b1 << 15);   // SWRST
    I2C1->CR1 &= ~(0b1 << 15);  // Clear SWRST
    I2C1->CR2 = 16;             // APB1 = 16 MHz
    I2C1->CCR = 80;             // 100 kHz
    I2C1->TRISE = 17;           // Max rise time
    I2C1->CR1 |= 0b1;           // PE enable
}

void I2C1_Init_DS1621(void) {
    // --- 1. Configurer DS1621 en mode CONTINUOUS (1SHOT = 0) ---
    I2C1->CR1 |= (1 << 8);               // START
    while (!(I2C1->SR1 & (1 << 0)));     // Wait SB
    I2C1->DR = 0x90;                     // Addr + Write
    while (!(I2C1->SR1 & (1 << 1)));     // Wait ADDR
    (void)I2C1->SR2;                     // Clear ADDR
    I2C1->DR = 0xAC;                     // Access Config command
    while (!(I2C1->SR1 & (1 << 7)));     // Wait TXE
    I2C1->DR = 0x00;                     // 1SHOT=0, POL=0 ? mode continu
    while (!(I2C1->SR1 & (1 << 7)));     // Wait TXE
    while (!(I2C1->SR1 & (1 << 2)));     // Wait BTF
    I2C1->CR1 |= (1 << 9);               // STOP

    // Petit délai
    for(volatile int i = 0; i < 10000; i++);

    // --- 2. Démarrer la conversion (Start Convert T: 0xEE) ---
    I2C1->CR1 |= (1 << 8);               // START
    while (!(I2C1->SR1 & (1 << 0)));     // Wait SB
    I2C1->DR = 0x90;                     // Addr + Write
    while (!(I2C1->SR1 & (1 << 1)));     // Wait ADDR
    (void)I2C1->SR2;                     // Clear ADDR
    I2C1->DR = 0xEE;                     // Start Convert T
    while (!(I2C1->SR1 & (1 << 7)));     // Wait TXE
    while (!(I2C1->SR1 & (1 << 2)));     // Wait BTF
    I2C1->CR1 |= (1 << 9);               // STOP
}

int8_t I2C1_Read_Temp(void) {
    int8_t msb = 0;
    int8_t lsb = 0;

    // --- 1. Pointer sur le registre température (0xAA) ---
    I2C1->CR1 |= (1 << 8);               // START
    while (!(I2C1->SR1 & (1 << 0)));     // Wait SB
    I2C1->DR = 0x90;                     // Addr + Write
    while (!(I2C1->SR1 & (1 << 1)));     // Wait ADDR
    (void)I2C1->SR2;                     // Clear ADDR
    I2C1->DR = 0xAA;                     // Read Temperature command
    while (!(I2C1->SR1 & (1 << 7)));     // Wait TXE
    while (!(I2C1->SR1 & (1 << 2)));     // Wait BTF

    // --- 2. Repeated Start + Lecture ---
    I2C1->CR1 |= (1 << 8);               // Repeated START
    while (!(I2C1->SR1 & (1 << 0)));     // Wait SB
    I2C1->DR = 0x91;                     // Addr + Read
    while (!(I2C1->SR1 & (1 << 1)));     // Wait ADDR
    (void)I2C1->SR2;                     // Clear ADDR

    // --- 3. Lire MSB (1er octet) avec ACK ---
    I2C1->CR1 |= (1 << 10);              // ACK = 1
    while (!(I2C1->SR1 & (1 << 6)));     // Wait RXNE
    msb = (int8_t)I2C1->DR;

    // --- 4. Lire LSB (2ème octet) avec NACK ---
    I2C1->CR1 &= ~(1 << 10);             // ACK = 0
    while (!(I2C1->SR1 & (1 << 6)));     // Wait RXNE
    lsb = (int8_t)I2C1->DR;

    // --- 5. STOP ---
    I2C1->CR1 |= (1 << 9);               // STOP

    // DS1621 : LSB bit7 = 0.5°C
    if (lsb & 0x80) {
        return msb + (msb >= 0 ? 1 : -1);  // Arrondi
    }
    return msb;
}
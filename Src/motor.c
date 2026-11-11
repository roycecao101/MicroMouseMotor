#include <stdint.h>
#include "motor.h"

// ==============================
// RCC
// ==============================

#define RCC_AHB1ENR (*(volatile uint32_t *)0x40023830)

// ==============================
// GPIOA
// ==============================

#define GPIOA_MODER (*(volatile uint32_t *)0x40020000)
#define GPIOA_ODR   (*(volatile uint32_t *)0x40020014)

// ==============================
// GPIOB
// ==============================

#define GPIOB_MODER (*(volatile uint32_t *)0x40020400)
#define GPIOB_ODR   (*(volatile uint32_t *)0x40020414)

// ==============================
// GPIOC
// ==============================

#define GPIOC_MODER (*(volatile uint32_t *)0x40020800)
#define GPIOC_ODR   (*(volatile uint32_t *)0x40020814)


// Configure one GPIO pin as output
static void setOutput(volatile uint32_t *moder, int pin)
{
    *moder &= ~(3U << (pin * 2));
    *moder |=  (1U << (pin * 2));
}


void motorInit(void)
{
    // Enable GPIOA, GPIOB, GPIOC clocks
    RCC_AHB1ENR |= (1 << 0);
    RCC_AHB1ENR |= (1 << 1);
    RCC_AHB1ENR |= (1 << 2);


    // ==================================================
    // TB6612 #1
    // ==================================================

    // A0 = PA0 = AIN1
    setOutput(&GPIOA_MODER, 0);

    // A1 = PA1 = AIN2
    setOutput(&GPIOA_MODER, 1);

    // A2 = PA4 = STBY #1
    setOutput(&GPIOA_MODER, 4);

    // A3 = PB0 = PWMA
    setOutput(&GPIOB_MODER, 0);

    // D2 = PA10 = BIN1
    setOutput(&GPIOA_MODER, 10);

    // D3 = PB3 = BIN2
    setOutput(&GPIOB_MODER, 3);

    // D4 = PB5 = PWMB
    setOutput(&GPIOB_MODER, 5);


    // ==================================================
    // TB6612 #2
    // ==================================================

    // D7 = PA8 = AIN1
    setOutput(&GPIOA_MODER, 8);

    // D8 = PA9 = AIN2
    setOutput(&GPIOA_MODER, 9);

    // D5 = PB4 = PWMA
    setOutput(&GPIOB_MODER, 4);

    // D9 = PC7 = BIN1
    setOutput(&GPIOC_MODER, 7);

    // D10 = PB6 = BIN2
    setOutput(&GPIOB_MODER, 6);

    // D6 = PB10 = PWMB
    setOutput(&GPIOB_MODER, 10);

    // D11 = PA7 = STBY #2
    setOutput(&GPIOA_MODER, 7);


    // ==================================================
    // ENABLE BOTH TB6612 BOARDS
    // ==================================================

    GPIOA_ODR |= (1 << 4);   // Board #1 STBY HIGH
    GPIOA_ODR |= (1 << 7);   // Board #2 STBY HIGH


    // ==================================================
    // FULL SPEED FOR ALL FOUR MOTOR CHANNELS
    // ==================================================

    // Board #1
    GPIOB_ODR |= (1 << 0);   // PWMA HIGH
    GPIOB_ODR |= (1 << 5);   // PWMB HIGH

    // Board #2
    GPIOB_ODR |= (1 << 4);   // PWMA HIGH
    GPIOB_ODR |= (1 << 10);  // PWMB HIGH

    stopMotors();
}


void moveForward(void)
{
    // Front-left: Board #1 Motor A
    GPIOA_ODR |=  (1 << 0);
    GPIOA_ODR &= ~(1 << 1);

    // Front-right: Board #1 Motor B
    GPIOA_ODR &= ~(1 << 10);
    GPIOB_ODR |=  (1 << 3);

    // Rear-left: Board #2 Motor A
    GPIOA_ODR |=  (1 << 8);
    GPIOA_ODR &= ~(1 << 9);

    // Rear-right: Board #2 Motor B
    GPIOC_ODR &= ~(1 << 7);
    GPIOB_ODR |=  (1 << 6);
}


void moveBackward(void)
{
    // Front-left
    GPIOA_ODR &= ~(1 << 0);
    GPIOA_ODR |=  (1 << 1);

    // Front-right
    GPIOA_ODR |=  (1 << 10);
    GPIOB_ODR &= ~(1 << 3);

    // Rear-left
    GPIOA_ODR &= ~(1 << 8);
    GPIOA_ODR |=  (1 << 9);

    // Rear-right
    GPIOC_ODR |=  (1 << 7);
    GPIOB_ODR &= ~(1 << 6);
}


void turnLeft(void)
{
    // Left side backward
    GPIOA_ODR &= ~(1 << 0);
    GPIOA_ODR |=  (1 << 1);

    GPIOA_ODR &= ~(1 << 8);
    GPIOA_ODR |=  (1 << 9);

    // Right side forward
    GPIOA_ODR &= ~(1 << 10);
    GPIOB_ODR |=  (1 << 3);

    GPIOC_ODR &= ~(1 << 7);
    GPIOB_ODR |=  (1 << 6);
}


void turnRight(void)
{
    // Left side forward
    GPIOA_ODR |=  (1 << 0);
    GPIOA_ODR &= ~(1 << 1);

    GPIOA_ODR |=  (1 << 8);
    GPIOA_ODR &= ~(1 << 9);

    // Right side backward
    GPIOA_ODR |=  (1 << 10);
    GPIOB_ODR &= ~(1 << 3);

    GPIOC_ODR |=  (1 << 7);
    GPIOB_ODR &= ~(1 << 6);
}


void stopMotors(void)
{
    // Board #1 Motor A
    GPIOA_ODR &= ~(1 << 0);
    GPIOA_ODR &= ~(1 << 1);

    // Board #1 Motor B
    GPIOA_ODR &= ~(1 << 10);
    GPIOB_ODR &= ~(1 << 3);

    // Board #2 Motor A
    GPIOA_ODR &= ~(1 << 8);
    GPIOA_ODR &= ~(1 << 9);

    // Board #2 Motor B
    GPIOC_ODR &= ~(1 << 7);
    GPIOB_ODR &= ~(1 << 6);
}

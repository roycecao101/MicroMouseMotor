#include <stdint.h>
#include "motor.h"

// RCC
#define RCC_AHB1ENR (*(volatile uint32_t *)0x40023830)

// GPIOA
#define GPIOA_MODER (*(volatile uint32_t *)0x40020000)
#define GPIOA_ODR   (*(volatile uint32_t *)0x40020014)

// GPIOB
#define GPIOB_MODER (*(volatile uint32_t *)0x40020400)
#define GPIOB_ODR   (*(volatile uint32_t *)0x40020414)

void motorInit(void)
{
    // Enable GPIOA and GPIOB clocks
    RCC_AHB1ENR |= (1 << 0);   // GPIOA
    RCC_AHB1ENR |= (1 << 1);   // GPIOB


    // -------------------------
    // Motor A control pins
    // -------------------------

    // PA0 = AIN1
    GPIOA_MODER &= ~(3 << (0 * 2));
    GPIOA_MODER |=  (1 << (0 * 2));

    // PA1 = AIN2
    GPIOA_MODER &= ~(3 << (1 * 2));
    GPIOA_MODER |=  (1 << (1 * 2));

    // PA4 = STBY
    GPIOA_MODER &= ~(3 << (4 * 2));
    GPIOA_MODER |=  (1 << (4 * 2));

    // PB0 = PWMA
    GPIOB_MODER &= ~(3 << (0 * 2));
    GPIOB_MODER |=  (1 << (0 * 2));


    // -------------------------
    // Motor B control pins
    // -------------------------

    // PA10 = BIN1
    GPIOA_MODER &= ~(3 << (10 * 2));
    GPIOA_MODER |=  (1 << (10 * 2));

    // PB3 = BIN2
    GPIOB_MODER &= ~(3 << (3 * 2));
    GPIOB_MODER |=  (1 << (3 * 2));

    // PB5 = PWMB
    GPIOB_MODER &= ~(3 << (5 * 2));
    GPIOB_MODER |=  (1 << (5 * 2));


    // Enable TB6612
    GPIOA_ODR |= (1 << 4);   // STBY HIGH

    // Full speed for both motors
    GPIOB_ODR |= (1 << 0);   // PWMA HIGH
    GPIOB_ODR |= (1 << 5);   // PWMB HIGH

    // Start stopped
    stopMotors();
}


void moveForward(void)
{
    // Motor A
    GPIOA_ODR |=  (1 << 0);   // AIN1 HIGH
    GPIOA_ODR &= ~(1 << 1);   // AIN2 LOW

    // Motor B
    // Opposite electrical direction because
    // motor is mounted on opposite side
    GPIOA_ODR &= ~(1 << 10);  // BIN1 LOW
    GPIOB_ODR |=  (1 << 3);   // BIN2 HIGH
}


void moveBackward(void)
{
    // Motor A
    GPIOA_ODR &= ~(1 << 0);   // AIN1 LOW
    GPIOA_ODR |=  (1 << 1);   // AIN2 HIGH

    // Motor B
    GPIOA_ODR |=  (1 << 10);  // BIN1 HIGH
    GPIOB_ODR &= ~(1 << 3);   // BIN2 LOW
}


void turnLeft(void)
{
    // Pivot turn:
    // left-side motor backward
    // right-side motor forward

    // Motor A backward
    GPIOA_ODR &= ~(1 << 0);
    GPIOA_ODR |=  (1 << 1);

    // Motor B forward
    GPIOA_ODR &= ~(1 << 10);
    GPIOB_ODR |=  (1 << 3);
}


void turnRight(void)
{
    // Pivot turn:
    // left-side motor forward
    // right-side motor backward

    // Motor A forward
    GPIOA_ODR |=  (1 << 0);
    GPIOA_ODR &= ~(1 << 1);

    // Motor B backward
    GPIOA_ODR |=  (1 << 10);
    GPIOB_ODR &= ~(1 << 3);
}


void stopMotors(void)
{
    // Stop Motor A
    GPIOA_ODR &= ~(1 << 0);
    GPIOA_ODR &= ~(1 << 1);

    // Stop Motor B
    GPIOA_ODR &= ~(1 << 10);
    GPIOB_ODR &= ~(1 << 3);
}

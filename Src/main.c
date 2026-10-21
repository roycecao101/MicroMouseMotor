#include <stdint.h>

#define RCC_AHB1ENR (*(volatile uint32_t *)0x40023830)

#define GPIOA_MODER (*(volatile uint32_t *)0x40020000)
#define GPIOA_ODR   (*(volatile uint32_t *)0x40020014)

#define GPIOB_MODER (*(volatile uint32_t *)0x40020400)
#define GPIOB_ODR   (*(volatile uint32_t *)0x40020414)

int main(void)
{
    // Enable GPIOA and GPIOB clocks
    RCC_AHB1ENR |= (1 << 0);
    RCC_AHB1ENR |= (1 << 1);

    // A0 = PA0 = AIN1
    GPIOA_MODER &= ~(3 << (0 * 2));
    GPIOA_MODER |=  (1 << (0 * 2));

    // A1 = PA1 = AIN2
    GPIOA_MODER &= ~(3 << (1 * 2));
    GPIOA_MODER |=  (1 << (1 * 2));

    // A2 = PA4 = STBY
    GPIOA_MODER &= ~(3 << (4 * 2));
    GPIOA_MODER |=  (1 << (4 * 2));

    // A3 = PB0 = PWMA
    GPIOB_MODER &= ~(3 << (0 * 2));
    GPIOB_MODER |=  (1 << (0 * 2));

    // D2 = PA10 = BIN1
    GPIOA_MODER &= ~(3 << (10 * 2));
    GPIOA_MODER |=  (1 << (10 * 2));

    // D3 = PB3 = BIN2
    GPIOB_MODER &= ~(3 << (3 * 2));
    GPIOB_MODER |=  (1 << (3 * 2));

    // D4 = PB5 = PWMB
    GPIOB_MODER &= ~(3 << (5 * 2));
    GPIOB_MODER |=  (1 << (5 * 2));

    // Enable motor driver
    GPIOA_ODR |= (1 << 4);   // STBY HIGH

    // Full speed
    GPIOB_ODR |= (1 << 0);   // PWMA HIGH
    GPIOB_ODR |= (1 << 5);   // PWMB HIGH

    // Motor A
    GPIOA_ODR |=  (1 << 0);  // AIN1 HIGH
    GPIOA_ODR &= ~(1 << 1);  // AIN2 LOW

    // Motor B
    GPIOA_ODR &= ~(1 << 10); // BIN1 LOW
    GPIOB_ODR |=  (1 << 3);  // BIN2 HIGH

    while (1)
    {
        // Both wheels keep spinning continuously
    }
}

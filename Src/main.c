#include <stdint.h>

#define RCC_AHB1ENR (*(volatile uint32_t *)0x40023830)

#define GPIOA_MODER (*(volatile uint32_t *)0x40020000)
#define GPIOA_ODR   (*(volatile uint32_t *)0x40020014)

#define GPIOB_MODER (*(volatile uint32_t *)0x40020400)
#define GPIOB_ODR   (*(volatile uint32_t *)0x40020414)

void delay(void)
{
    for (volatile int i = 0; i < 3000000; i++);
}

int main(void)
{
    // Enable GPIOA clock
    RCC_AHB1ENR |= (1 << 0);

    // Enable GPIOB clock
    RCC_AHB1ENR |= (1 << 1);

    // ----------------------------
    // A0 = PA0 = AIN1
    // ----------------------------
    GPIOA_MODER &= ~(3 << (0 * 2));
    GPIOA_MODER |=  (1 << (0 * 2));

    // ----------------------------
    // A1 = PA1 = AIN2
    // ----------------------------
    GPIOA_MODER &= ~(3 << (1 * 2));
    GPIOA_MODER |=  (1 << (1 * 2));

    // ----------------------------
    // A2 = PA4 = STBY
    // ----------------------------
    GPIOA_MODER &= ~(3 << (4 * 2));
    GPIOA_MODER |=  (1 << (4 * 2));

    // ----------------------------
    // A3 = PB0 = PWMA
    // ----------------------------
    GPIOB_MODER &= ~(3 << (0 * 2));
    GPIOB_MODER |=  (1 << (0 * 2));

    // Enable motor driver
    GPIOA_ODR |= (1 << 4);   // STBY HIGH

    // Full speed
    GPIOB_ODR |= (1 << 0);   // PWMA HIGH

    while (1)
    {
        // Forward
        GPIOA_ODR |=  (1 << 0);   // AIN1 HIGH
        GPIOA_ODR &= ~(1 << 1);   // AIN2 LOW

        delay();

        // Stop
        GPIOA_ODR &= ~(1 << 0);
        GPIOA_ODR &= ~(1 << 1);

        delay();

        // Reverse
        GPIOA_ODR &= ~(1 << 0);   // AIN1 LOW
        GPIOA_ODR |=  (1 << 1);   // AIN2 HIGH

        delay();

        // Stop
        GPIOA_ODR &= ~(1 << 0);
        GPIOA_ODR &= ~(1 << 1);

        delay();
    }
}

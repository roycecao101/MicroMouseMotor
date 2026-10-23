#include "motor.h"

void delay(void)
{
    for (volatile int i = 0; i < 3000000; i++);
}

int main(void)
{
    motorInit();

    while (1)
    {
        moveForward();
        delay();

        stopMotors();
        delay();

        turnLeft();
        delay();

        stopMotors();
        delay();

        turnRight();
        delay();

        stopMotors();
        delay();
    }
}

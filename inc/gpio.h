#ifndef GPIO_H
#define GPIO_H

#define GPIOA 0x40020000
#define GPIOB 0x40020400
#define GPIOC 0x40020800
#define GPIOD 0x40020c00
#define GPIOE 0x40021000
#define GPIOF 0x40021400
#define GPIOG 0x40021800
#define GPIOH 0x40021c00
#define GPIOI 0x40022000
#define GPIOJ 0x40022400
#define GPIOK 0x40022800

#define GPIO_MODER   0x00
#define GPIO_OTYPER  0x04
#define GPIO_OSPEEDR 0x08
#define GPIO_PUPDR   0x0c
#define GPIO_ODR     0x14

#endif

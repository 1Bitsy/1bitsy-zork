#include "led-ping.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "systick.h"

#define LED_PORT            GPIOA
#define LED_PIN             GPIO8
#define RCC_LED_PORT    RCC_GPIOA

#define BUTTON_PORT         GPIOC
#define BUTTON_PIN          GPIO1
#define RCC_BUTTON_PORT RCC_GPIOC

#define TRIGGER_DURATION      100

static uint32_t trigger_count;

static void update_blink(uint32_t msec_time)
{
    bool on = gpio_get(GPIOC, GPIO1) ? false : true;
    if (trigger_count) {
        on = true;
        trigger_count--;
    }
    if (msec_time & 0x21)
        on = false;
    if (on)
        gpio_clear(GPIOA, GPIO8); // low is LED on.
    else
        gpio_set(GPIOA, GPIO8);
}

void LED_ping_init(void)
{
    rcc_periph_clock_enable(RCC_LED_PORT);
    rcc_periph_clock_enable(RCC_BUTTON_PORT);
    gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
    gpio_mode_setup(BUTTON_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, BUTTON_PIN);
    register_systick_handler(update_blink);
}

void trigger_LED_ping(void)
{
    trigger_count = TRIGGER_DURATION;
}


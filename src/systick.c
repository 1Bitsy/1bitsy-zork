#include "systick.h"

#include <stddef.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#define MAX_HANDLERS 4

static volatile uint32_t milliseconds;
static systick_handler *handlers[MAX_HANDLERS];
static size_t handler_count = 0;

void sys_tick_handler(void)
{
    milliseconds++;
    for (size_t i = 0; i < handler_count; i++)
        (*handlers[i])(milliseconds);
}

void systick_init(void)
{
    /* clock rate / 1000 to get 1mS interrupt rate */
    systick_set_reload(168000);
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_counter_enable();
    /* this done last */
    systick_interrupt_enable();
}

uint32_t time_millis(void)
{
    return milliseconds;
}

void delay(uint32_t millis)
{
    uint32_t wake = milliseconds + millis;
    while ((int32_t)wake - (int32_t)milliseconds > 0)
        continue;
}

void register_systick_handler(systick_handler *h)
{
    if (handler_count < MAX_HANDLERS)
        handlers[handler_count++] = h;
}

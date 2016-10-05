#ifndef SYSTICK_included
#define SYSTICK_included

#include <stdint.h>

typedef void systick_handler(uint32_t msec_time);

extern void systick_init(void);
extern uint32_t time_millis(void);
extern void delay(uint32_t millis);

extern void register_systick_handler(systick_handler *);

#endif /* !SYSTICK_included */

#ifndef _LED_TASK_H_
#define _LED_TASK_H_

typedef enum
{
    LED_ON,
    LED_OFF,
} LED_STATUS_enum;

void led_init(void);
void led_task(void);
void led_control(LED_STATUS_enum state);
void led_toggle(void);

#endif


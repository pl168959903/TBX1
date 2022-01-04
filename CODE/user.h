#ifndef __USER_H
#define __USER_H

#include "TBX1.h"

#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define LED_B PB7
#define LED_G PB8
#define LED_R PB9
#define PW_STDBY PA10
#define PW_CHARGE PA11

typedef enum{
    discharging,
    charging,
    fullyCharge
}power_state;


typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}color;

typedef struct led_pwm_t {
    uint8_t value;

    uint8_t val;
    counter count;

    void ( *LED_on )( void );
    void ( *LED_off )( void );

    void ( *LED_step )( struct led_pwm_t *self );
} led_pwm;

uint8_t LED_timer_handler(void);
void LED_rgb_init( void );
void LED_start( void );
void LED_stop( void );
void LED_updataColor( color c ) ;

void PW_charge_handler( void );

#endif  

#include "user.h"

void LED_R_on( void ) {
    LED_R = 0;
}

void LED_R_off( void ) {
    LED_R = 1;
}

void LED_G_on( void ) {
    LED_G = 0;
}

void LED_G_off( void ) {
    LED_G = 1;
}

void LED_B_on( void ) {
    LED_B = 0;
}

void LED_B_off( void ) {
    LED_B = 1;
}

void LED_step( led_pwm *self ) {
    if ( self->val > self->count.GetCounter( &( self->count ) ) )
        self->LED_on();
    else
        self->LED_off();

    self->count.Trigger( &( self->count ) );
    if ( self->count.GetCounter( &( self->count ) ) > 15 ) {
        self->count.SetCounter( &( self->count ), 0 );
        self->val = self->value;
    }
}

void led_pwm_init( led_pwm *st ) {
    st->val = st->value;
    counter_init( &( st->count ) );
    st->LED_step = LED_step;
}

led_pwm led_r, led_g, led_b;
uint8_t led_ctrl_en = 0;

void LED_start( void ) {
    TIMER_Start( TIMER0 );
    led_ctrl_en = 1;
}

void LED_stop( void ) {
    // TIMER_Start( TIMER0 );
    led_ctrl_en = 0;
    led_r.LED_off();
    led_g.LED_off();
    led_b.LED_off();
}

void LED_updataColor( color c ) {
    led_r.value = c.red;
    led_g.value = c.green;
    led_b.value = c.blue;
}

void LED_rgb_init( void ) {

    led_r = (led_pwm){ .value = 0, .LED_on = LED_R_on, .LED_off = LED_R_off };
    led_g = (led_pwm){ .value = 0, .LED_on = LED_G_on, .LED_off = LED_G_off };
    led_b = (led_pwm){ .value = 0, .LED_on = LED_B_on, .LED_off = LED_B_off };
    led_pwm_init( &led_r);
    led_pwm_init( &led_g);
    led_pwm_init( &led_b);
}

uint8_t LED_timer_handler( void ) {
    if ( led_ctrl_en == 1 ) {
        led_r.LED_step( &led_r );
        led_g.LED_step( &led_g );
        led_b.LED_step( &led_b );
        return 1;
    }
    else {
        return 0;
    }
}

void debug_functions( void ) {
    printf( "LED debug start.\n" );


    // while ( 1 ) {
    //     Timer_delay( 500 );
    //     printf("Red = %d, Green = %d, Blue = %d\n", c.red, c.green, c.blue);
    //     __NOP();
    // };
}

/****************************************************************************
 * @file     main.c
 * @version  V1.01
 * $Revision: 1 $
 * $Date: 2019/05/02 8:05p $
 * @brief    TBX1 Sample
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2011 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/

#include "TBX1.h"

#if defined( __GNUC__ ) && !defined( __ARMCC_VERSION ) && defined( OS_USE_SEMIHOSTING )
extern void initialise_monitor_handles( void );
#endif

extern void Periph_Init( void );
extern void Clock_Init( void );
extern void Pin_Init( void );

void debug_functions( void );

int __user_cmpxchg_4( unsigned int *ptr, unsigned int old, unsigned int new ) {
    if ( *ptr == old ) {
        *ptr = new;
        return 0;
    }
    else {
        return 1;
    }
}

void System_Init( void ) {
    /* Unlock protected registers */
    SYS_UnlockReg();

    Clock_Init();
    Pin_Init();
    Periph_Init();

    /* Lock protected registers */
    SYS_LockReg();
}

extern void ( *ext_key_timer_start_handle )( void );
extern void ( *ext_key_timer_stop_handle )( void );

void key_timer_start( void ) {
    TIMER_Start( TIMER0 );
    return;
}
void key_timer_stop( void ) {
    // TIMER_Stop( TIMER0 );
    return;
}

counter delayCount;

uint8_t Timer_delay_Handeler( void ) {
    if ( delayCount.en == 1 ) {
        delayCount.Trigger( &delayCount );
        return 1;
    }
    else {
        return 0;
    }
}

void Timer_delay( uint32_t ms ) {
    delayCount.SetCounter(&delayCount, 0);
    delayCount.Enable( &delayCount );
    TIMER_Start( TIMER0 );
    while ( ms >= delayCount.count ) { __NOP(); };
    delayCount.Disable( &delayCount );
}

void Enable_v33( void ) {
    EN_V33 = 1;
    printf( "Enable V33\n" );
}
void Disable_v33( void ) {
    EN_V33 = 0;
    printf( "Disnable V33\n" );
    LED_stop();
    while(1){};
}
void nop_funciton(void){
    __NOP();
}

int32_t main( void ) {
    /* Init System, IP clock, multi-function I/O and Peripheral*/

    ext_key_timer_start_handle = key_timer_start;
    ext_key_timer_stop_handle  = key_timer_stop;

    System_Init();
    Key_init();
    LED_rgb_init();
    LED_start();
    counter_init( &delayCount );

    if ( KEY0 == 1 ) Key_down();

    printf( "*** Init Done, User add operation code***\n" );
    // Timer_delay( 1000 );

    Key_capture_until( &( key_event_capture ){ .event = keepPress, .clearHistoryEvent = 1, .Timeout = 0, .handler_ptr = Enable_v33 } );
    LED_updataColor((color){.red = 16, .green = 16, .blue = 16});
    Key_capture_until( &( key_event_capture ){ .event = longPress, .clearHistoryEvent = 1, .Timeout = 0, .handler_ptr = nop_funciton } );
    Key_add_capture( &( key_event_capture ){ .event = keepPress, .clearHistoryEvent = 1, .Timeout = 0, .handler_ptr = Disable_v33 } );

    printf( "start debug function.\n" );
    debug_functions();
    printf( "End debug function.\n" );
    while ( 1 ){
        Timer_delay( 1000 );
        printf("In while loop.\n");
    };
}

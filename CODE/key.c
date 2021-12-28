#include "key.h"

static  uint8_t           keyCodeBufArray[ SW_BUF_CREATE_BYTE( KEY_CODE_BUFFER_SIZE, sizeof( key_code ) ) ] = { 0 };
uint8_t           keyEventArray[ SW_BUF_CREATE_BYTE( KEY_EVENT_BUFFER_SIZE, sizeof( key_event ) ) ] = { 0 };
sw_buf           *keyCodeBuf;
sw_buf           *keyEventBuf;
counter           key0Counter;
counter           keyCaptureTimerCounter;
key_event_capture key_event_capture_array[ 8 ] = { 0 };

void ( *ext_key_timer_start_handle )( void );
void ( *ext_key_timer_stop_handle )( void );

void Key_code_decoder( void ) {
    static uint8_t node = 0;
    while ( keyCodeBuf->bufControl.units_c > 0 ) {
        key_code t;
        keyCodeBuf->Data_output( keyCodeBuf, &t );

        switch ( node ) {
        case 0:
            if ( t.keyCodeClass == keyDown )
                node = 1;
            else
                node = 0;
            break;
        case 1:
            if ( t.keyCodeClass == keyUp ) {
                keyEventBuf->Data_input( keyEventBuf, ( key_event[] ){ shortPress } );
                node = 0;
            }
            else if ( t.keyCodeClass == keyKeep ) {
                node = 2;
            }
            else if ( t.keyCodeClass == keyDown ) {
                node = 1;
            }
            else
                node = 0;
            break;
        case 2:
            if ( t.keyCodeClass == keyUp ) {
                keyEventBuf->Data_input( keyEventBuf, ( key_event[] ){ shortPress } );
                node = 0;
            }
            else if ( t.keyCodeClass == keyKeep ) {
                node = 3;
            }
            else
                node = 0;
            break;
        case 3:
            if ( t.keyCodeClass == keyUp ) {
                keyEventBuf->Data_input( keyEventBuf, ( key_event[] ){ shortPress } );
                node = 0;
            }
            else if ( t.keyCodeClass == keyKeep ) {
                node = 4;
            }
            else
                node = 0;
            break;
        case 4:
            if ( t.keyCodeClass == keyUp ) {
                keyEventBuf->Data_input( keyEventBuf, ( key_event[] ){ longPress } );
                node = 0;
            }
            else if ( t.keyCodeClass == keyKeep ) {
                keyEventBuf->Data_input( keyEventBuf, ( key_event[] ){ keepPress } );
                node = 4;
            }
            else
                node = 0;
            break;
        default:
            break;
        }
    }
}
static __inline void Key_start_timer_counter( void ) {
    ext_key_timer_start_handle();
    key0Counter.Enable( &key0Counter );
}
static __inline void Key_stop_timer_counter( void ) {
    ext_key_timer_stop_handle();
    key0Counter.Disable( &key0Counter );
}

void Data_input( sw_buf *self, void *data );
void Key_down( void ) {
    key_code t = { .keyCodeClass = keyDown, .time_ms = 11 };
	// Data_input(keyCodeBuf, &t);
    keyCodeBuf->Data_input( keyCodeBuf, &t );
    Key_code_decoder();
	Key_start_timer_counter();
}
void Key_up( void ) {
    key_code t = { .keyCodeClass = keyUp, .time_ms = key0Counter.GetCounter( &key0Counter ) };
    key0Counter.SetCounter( &key0Counter, 0 );
    keyCodeBuf->Data_input( keyCodeBuf, &t );
    Key_code_decoder();
		Key_stop_timer_counter();
}
void Key_keep( void ) {
    if ( key0Counter.count >= KEY_KEEP_TIME_STEP_MS ) {
        key_code t = { .keyCodeClass = keyKeep, .time_ms = key0Counter.GetCounter( &key0Counter ) };
        key0Counter.SetCounter( &key0Counter, 0 );
        keyCodeBuf->Data_input( keyCodeBuf, &t );
        Key_code_decoder();
    }
}
uint8_t KeyTimer_Handler( void ) {
    uint8_t retval = 0;
    if ( key0Counter.en == 1 ) {
        key0Counter.Trigger( &key0Counter );
        Key_keep();
        retval = 1;
    }
    return retval;
}
void Key_init( void ) {
    counter_init( &key0Counter );
    keyCodeBuf  = sw_buf_init( keyCodeBufArray, KEY_CODE_BUFFER_SIZE, sizeof( key_code ), FIFO );
    keyEventBuf = sw_buf_init( keyEventArray, KEY_EVENT_BUFFER_SIZE, sizeof( key_event ), FIFO );
}

void key_event_free( void ) {
    while ( keyEventBuf->bufControl.units_c > 0 ) {
        volatile key_event env;
        keyEventBuf->Data_output( keyEventBuf, ( key_event * )&env );
    }
}

key_event_capture* Key_add_capture( key_event_capture *cap ) {
    for ( int i = 0; i < 8; i++ ) {
        if ( key_event_capture_array[ i ].en == 0 ) {
            memcpy( &key_event_capture_array[ i ], cap, sizeof( key_event_capture ) );

            key_event_capture_array[ i ].en = 1;
            if ( key_event_capture_array[ i ].clearHistoryEvent == 1 ) key_event_free();
            if ( key_event_capture_array[ i ].Timeout > 0 ) {
                counter_init( &( key_event_capture_array[ i ].keyCaptureTimerCounter ) );
                ext_key_timer_start_handle();
            }
            key_event_capture_array[ i ].result = undone;
            return &(key_event_capture_array[ i ]);
        }
    }
    return NULL;
}

uint8_t KeyCaptureTimer_Handler( void ) {
    uint8_t retval = 0;
    for ( int i = 0; i < 8; i++ ) {
        if ( key_event_capture_array[ i ].en == 1 ) {
            if ( key_event_capture_array[ i ].keyCaptureTimerCounter.count < key_event_capture_array[ i ].Timeout ) {
                key_event_capture_array[ i ].keyCaptureTimerCounter.Trigger( &( key_event_capture_array[ i ].keyCaptureTimerCounter ) );
                retval = 1;
            }
        }
    }
    return retval;
}

void Key_capture_start( void ) {
    while ( keyEventBuf->bufControl.units_c > 0 ) {
        key_event env;
        keyEventBuf->Data_output( keyEventBuf, &env );

        for ( int i = 0; i < 8; i++ ) {
            if ( key_event_capture_array[ i ].en == 1 ) {
                if ( key_event_capture_array[ i ].Timeout != 0 && key_event_capture_array[ i ].Timeout <= key_event_capture_array[ i ].keyCaptureTimerCounter.count ) {
                    key_event_capture_array[ i ].en = 0;
                    key_event_capture_array[ i ].result = timeout;
                }
                else {
                    if ( key_event_capture_array[ i ].event == env ) {
                        key_event_capture_array[ i ].handler_ptr();
                        key_event_capture_array[ i ].en = 0;
                        key_event_capture_array[ i ].result = done;
                    }
                }
            }
        }
    }
}


void nop_funciton(void);
Key_capture_result Key_capture_until( key_event_capture *cap ){
    key_event_capture *env = Key_add_capture( cap );
    while(env->en == 1){nop_funciton();}
    return env->result;
}

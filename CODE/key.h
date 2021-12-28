#ifndef __KEY_H
#define __KEY_H

#include <stdint.h>
#include <string.h>
#include "sw_buf.h"
#include "counter.h"

#define KEY_KEEP_TIME_STEP_MS 500
#define KEY_CODE_BUFFER_SIZE 32
#define KEY_EVENT_BUFFER_SIZE 32

typedef enum {
    shortPress,
    longPress,
    keepPress
} key_event;

typedef enum { 
    keyDown, keyUp, keyKeep 
} key_code_class;

typedef struct {
    key_code_class keyCodeClass;
    uint32_t time_ms;
} key_code;

typedef enum {
    timeout,
    done,
    undone
}Key_capture_result;

typedef struct {
    uint8_t en;
    uint8_t clearHistoryEvent;
    uint32_t Timeout;
    key_event event;
    counter keyCaptureTimerCounter;
    Key_capture_result result;
    void (*handler_ptr)(void);
}key_event_capture;



void Key_init(void);
void Key_code_decoder( void );
void Key_down( void );
void Key_up( void );
void key_keep( void );
uint8_t KeyTimer_Handler( void );
uint8_t KeyCaptureTimer_Handler( void );
void Key_capture_start( void );
key_event_capture* Key_add_capture( key_event_capture *cap );
Key_capture_result Key_capture_until( key_event_capture *cap );

#endif 

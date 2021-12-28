#ifndef __COUNTER_H
#define __COUNTER_H

#include <stdint.h>

typedef struct counter_t {
    uint32_t count;
    uint8_t  en;

    void ( *Trigger )( struct counter_t *self );
    void ( *Disable )( struct counter_t *self );
    void ( *Enable )( struct counter_t *self );
    uint32_t ( *GetCounter )( struct counter_t *self );
    void ( *SetCounter )( struct counter_t *self, uint32_t value );
} counter;


void counter_init(counter *st);

#endif

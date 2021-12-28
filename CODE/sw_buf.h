#ifndef __SW_BUF_H
#define __SW_BUF_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SW_ALIGN( n )              ( ( n + ( sizeof( size_t ) - 1 ) ) & ~( sizeof( size_t ) - 1 ) )
#define SW_BUF_CREATE_BYTE( n, s ) ( SW_ALIGN( sizeof( sw_buf ) ) + SW_ALIGN( n * s ) )

typedef enum { FIFO, STACK } bufferType;

typedef struct sw_buf_s {
    uint8_t    lock;
    uint32_t   units;
    uint32_t   unitSize;
    bufferType type;

    struct {
        void    *head;
        void    *tail;
        uint32_t units_c;
    } bufControl;

    void ( *Data_input )( struct sw_buf_s *self, void *addr );
    void ( *Data_output )( struct sw_buf_s *self, void *addr );

    void* data;

} sw_buf;

sw_buf *sw_buf_init( void *baseAddress, uint32_t units, uint32_t unitSize, bufferType type );

#endif

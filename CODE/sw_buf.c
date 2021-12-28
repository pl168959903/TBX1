#include "sw_buf.h"

static __inline uint32_t calc_align( uint32_t n ) {
    return ( n + ( sizeof( size_t ) - 1 ) ) & ~( sizeof( size_t ) - 1 );
}

static __inline uint32_t GetBufCfgDataSize( void ) {
    return calc_align( sizeof( sw_buf ) );
}

static __inline uint32_t GetBufDataArraySize( sw_buf *buf ) {
    return calc_align( buf->units * buf->unitSize );
}

static __inline void *GetBufStartAddress( sw_buf *buf ) {
    return ( void * )( buf + GetBufCfgDataSize() );
}

static __inline void *GetBufEndAddress( sw_buf *buf ) {
    return ( void * )( ( uint32_t )GetBufStartAddress( buf ) + GetBufDataArraySize( buf ) );
}

void Data_input( sw_buf *self, void *data ) {
    if ( self->bufControl.units_c < self->units ) {
        if ( self->type == FIFO ) {
            memcpy( self->bufControl.tail, data, self->unitSize );
            if ( __sync_add_and_fetch( ( uint32_t * )&( self->bufControl.tail ), self->unitSize ) == ( uint32_t )GetBufEndAddress( self ) ) { 
                self->bufControl.tail = GetBufStartAddress( self ); 
            }
        }
        else if ( self->type == STACK ) {
            memcpy( self->bufControl.tail, data, self->unitSize );
            __sync_add_and_fetch( ( uint32_t * )&( self->bufControl.tail ), self->unitSize );
        }
        __sync_add_and_fetch( &( self->bufControl.units_c ), 1 );
    }
    else {
        printf("error Data_output.\n");
        while(1){};
        return;
    }
}
static void Data_output( sw_buf *self, void *data ) {
    if ( self->bufControl.units_c > 0 ) {
        if ( self->type == FIFO ) {
            memcpy( data, self->bufControl.head, self->unitSize );
            if ( __sync_add_and_fetch( ( uint32_t * )&( self->bufControl.head ), self->unitSize ) == ( uint32_t )GetBufEndAddress( self ) ) { self->bufControl.head = GetBufStartAddress( self ); }
        }
        else if ( self->type == STACK ) {
            memcpy( data, ( void * )( ( uint32_t )self->bufControl.tail - self->unitSize ), self->unitSize );
            __sync_sub_and_fetch( ( uint32_t * )&( self->bufControl.tail ), self->unitSize );
        }
        __sync_sub_and_fetch( &( self->bufControl.units_c ), 1 );
    }
    else {
        printf("error Data_output.\n");
        return;
    }
}

sw_buf *sw_buf_init( void *baseAddress, uint32_t units, uint32_t unitSize, bufferType type ) {
    sw_buf *buf   = ( sw_buf   *)baseAddress;
    buf->lock     = 0;
    buf->units    = units;
    buf->unitSize = unitSize;
    buf->type     = type;

    buf->bufControl.head    = GetBufStartAddress( buf );
    buf->bufControl.tail    = GetBufStartAddress( buf );
    buf->bufControl.units_c = 0;

    buf->Data_input  = Data_input;
    buf->Data_output = Data_output;

    return buf;
}

// void debug_sw_printf(sw_buf *buf){
//      printf("lock = 0x%X\n", buf->lock);
//      printf("units = 0x%X\n", buf->units);
//      printf("unitSize = 0x%X\n", buf->unitSize);
//      printf("type = 0x%X\n", buf->type);

//      printf("head = 0x%X\n", (uint32_t)buf->bufControl.head);
//      printf("tail = 0x%X\n", (uint32_t)buf->bufControl.tail);
//      printf("units_c = 0x%X\n", (uint32_t)buf->bufControl.units_c);

//      printf("Data_input = 0x%X\n", (uint32_t)buf->Data_input);
//      printf("Data_output = 0x%X\n", (uint32_t)buf->Data_output);
//      printf("------------------------------\n");
// }

// uint8_t bArray[SW_BUF_CREATE_BYTE(10, sizeof(uint32_t))] = {0};

// void debug_functions( void ) {
//     // printf("%d\n", sizeof( sw_buf ) );
//     // printf("%d\n", SW_ALIGN( sizeof( sw_buf ) ));
//     // printf("%d\n", SW_ALIGN( 10 * sizeof(uint32_t) ));
//     sw_buf *fifoBuf = sw_buf_init(bArray, 10, sizeof(uint32_t), STACK);
//     uint32_t outputData = 0;

//     for(uint32_t i = 0; i < 10; i++) {
//         printf("!Data in!\n");
//         fifoBuf->Data_input(fifoBuf, &i);
//         debug_sw_printf(fifoBuf);
//     }

//     printf("!Data out!\n");
//     fifoBuf->Data_output(fifoBuf, &outputData);
//     printf("outputData = 0x%X\n", outputData);
//     debug_sw_printf(fifoBuf);

//     printf("!Data in!\n");
//     fifoBuf->Data_input(fifoBuf, (uint32_t[]){0x12345});
//     debug_sw_printf(fifoBuf);

//     for(uint32_t i = 0; i < 10; i++) {
//         printf("!Data out!\n");
//         fifoBuf->Data_output(fifoBuf, &outputData);
//         printf("outputData = 0x%X\n", outputData);
//         debug_sw_printf(fifoBuf);
//     }
// }

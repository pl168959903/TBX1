#include "counter.h"

void Trigger( counter *self ) {
    __sync_add_and_fetch( ( uint32_t * )&( self->count ), 1 );
}
void Disable ( counter *self ){
    self->en = 0;
}
void Enable( counter *self ){
    self->en = 1;
}
uint32_t GetCounter( counter *self ){
    return self->count;
}
void SetCounter ( counter *self, uint32_t value ){
    self->count = value;
}

void counter_init(counter *st){
    st->count = 0;
    st->en = 0;

    st->Trigger = Trigger;
    st->Disable = Disable;
    st->Enable = Enable;
    st->GetCounter = GetCounter;
    st->SetCounter = SetCounter;
}


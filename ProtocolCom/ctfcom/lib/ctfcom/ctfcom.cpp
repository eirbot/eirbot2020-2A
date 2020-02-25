#include "ctfcom.h"
#include <mbed.h>

Ctfcom::Ctfcom(PinName Tx, PinName Rx, int baud) : _com(Tx,Rx,baud)
{
// Setup a serial interrupt function to receive data
    _com.attach(callback(this, &Ctfcom::it_handler), SerialBase::RxIrq);
}

Ctfcom::~Ctfcom()
{

}

void Ctfcom::send(const uint8_t *data, uint32_t len){
    for (size_t i = 0; i < len; i++)
    {
        _com.putc(data[i]);
    }
    
}

void Ctfcom::send_pos(float x, float y, float angle){
    generic_send(get_pos, x, y, angle);
}

void Ctfcom::it_handler(void){
    list_buffer.push_back(_com.getc());
}

uint8_t Ctfcom::get_recived_data(){
    uint8_t b = list_buffer.front();
    list_buffer.pop_front();
    return b;
}
size_t Ctfcom::get_len_recived_data(){
    return list_buffer.size();
}

void Ctfcom::build_data_frame_recursive(uint8_t *buffer, uint16_t index){
    // C'est une feuille du template on touche pas !
    return;
}

void Ctfcom::decode(){
    // C'est une feuille du template on touche pas !
    return;
}

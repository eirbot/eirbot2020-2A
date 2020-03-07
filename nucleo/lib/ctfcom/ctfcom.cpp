#include "ctfcom.h"
#include <mbed.h>

Ctfcom::Ctfcom(Serial *const com) : _com(com)
{
    _com->attach(callback(this, &Ctfcom::it_handler), SerialBase::RxIrq);
}
Ctfcom::Ctfcom(PinName Tx, PinName Rx)
{
    _com = new Serial(Tx, Rx, 115200);
    // Setup a serial interrupt function to receive data
    _com->attach(callback(this, &Ctfcom::it_handler), SerialBase::RxIrq);
    read_lock = 0;
    write_lock = 0;
}

Ctfcom::Ctfcom(PinName Tx, PinName Rx, int baud)
{
    _com = new Serial(Tx, Rx, baud);
    // Setup a serial interrupt function to receive data
    _com->attach(callback(this, &Ctfcom::it_handler), SerialBase::RxIrq);
    read_lock = 0;
    write_lock = 0;
}

Ctfcom::Ctfcom(PinName Tx, PinName Rx, int baud, uint16_t read_timout_in_ms)
{
    _com = new Serial(Tx, Rx, baud);
    // Setup a serial interrupt function to receive data
    _com->attach(callback(this, &Ctfcom::it_handler), SerialBase::RxIrq);
    read_lock = 0;
    write_lock = 0;
    read_timout_ms = read_timout_in_ms;
}

Ctfcom::~Ctfcom()
{
}

void Ctfcom::send(const uint8_t *data, uint32_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        _com->putc(data[i]);
    }
}

int Ctfcom::answer_get_pos(float x, float y, float angle)
{
    // TODO gestion ack
    generic_send(get_pos, x, y, angle);
    return 0;
}

int Ctfcom::answer_set_pos(float *x, float *y, float *angle)
{
    Timer t;
    t.start();
    uint8_t nb_expected_args = sizeof(&x) + sizeof(&y) + sizeof(&angle);
    while (get_len_recived_data() < nb_expected_args)
    {
        if (t.read_ms() > read_timout_ms)
        {
            // ERROR
            return -1;
        }
    }
    decode(x, y, angle);
    return 0;
}

void Ctfcom::it_handler(void)
{
    list_buffer.push_back(_com->getc());
}

uint8_t Ctfcom::get_recived_data()
{
    uint8_t b = list_buffer.front();
    list_buffer.pop_front();
    return b;
}
size_t Ctfcom::get_len_recived_data()
{
    return list_buffer.size();
}

void Ctfcom::build_data_frame_recursive(uint8_t *buffer, uint16_t index)
{
    // C'est une feuille du template on touche pas !
    return;
}

void Ctfcom::decode()
{
    // C'est une feuille du template on touche pas !
    return;
}

void Ctfcom::lock_write_ressource(void)
{
    while (write_lock)
    {
    }
    write_lock = 1;
}

void Ctfcom::lock_read_ressource(void)
{
    while (read_lock)
    {
    }
    read_lock = 1;
}

void Ctfcom::unlock_write_ressource(void)
{
    write_lock = 1;
}

void Ctfcom::unlock_read_ressource(void)
{
    read_lock = 1;
}

void Ctfcom::generic_send(COMMAND cmd)
{
    send((uint8_t *)&cmd, 1);
}
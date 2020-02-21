#ifndef CTFCOM_H
#define CTFCOM_H
#include <mbed.h>
#include <list>

enum COMMAND{
    get_pos,
    set_pos,
    go_to,
    stop,
    panic,
    get_gp2
};


class Ctfcom
{
private:
    RawSerial _com;
    list<uint8_t> list_buffer;
    void send(const uint8_t *data, int32_t len);
    void float_to_table(float f, uint8_t *buff);
public:
    Ctfcom(PinName Tx, PinName Rx, int baud);
    ~Ctfcom();
    uint8_t recive(void);
    void send_pos(float x, float y, float angle);
    void it_handler(void);
    uint8_t get_recived_data();
    uint8_t get_len_recived_data();
};



#endif
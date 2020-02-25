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
    void send(const uint8_t *data, uint32_t len);
    void float_to_table(float f, uint8_t *buff);

    template<typename... NoTypeDataPack, typename NoTypeData>
    void build_data_frame_recursive(uint8_t *buffer, uint16_t index, NoTypeData arg, NoTypeDataPack... args);
    void build_data_frame_recursive(uint8_t *buffer, uint16_t index);
    template<typename... NoTypeDataPack>
    void generic_send(COMMAND cmd, NoTypeDataPack... args);
public:
    Ctfcom(PinName Tx, PinName Rx, int baud);
    ~Ctfcom();
    uint8_t recive(void);
    void send_pos(float x, float y, float angle);
    void it_handler(void);
    uint8_t get_recived_data();
    uint8_t get_len_recived_data();
};

template<typename... NoTypeDataPack>
void Ctfcom::generic_send(COMMAND cmd, NoTypeDataPack... args){
    const uint32_t len = (sizeof(args) + ...) +1;
    //uint8_t *buffer = (uint8_t *)malloc(len);
    uint8_t buffer[len];
    buffer[0] = get_pos;
    build_data_frame_recursive(buffer, 1, args...);
    send(buffer, len);
    //free(buffer);
}

template<typename... NoTypeDataPack, typename NoTypeData>
void Ctfcom::build_data_frame_recursive(uint8_t *buffer, uint16_t index, NoTypeData arg, NoTypeDataPack... args){

    uint8_t calculated_arg_len = sizeof(arg);
    for( uint8_t var_index = 0; var_index < calculated_arg_len; var_index ++ ){
            buffer[index] = ((uint8_t *)&arg)[var_index];
            index += 1;
    }
    build_data_frame_recursive(buffer, index, args...);
}



#endif

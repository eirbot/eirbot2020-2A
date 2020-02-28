#ifndef CTFCOM_H
#define CTFCOM_H
#include <mbed.h>
#include <list>

#define DEFAULT_TIMOUT_MS 50

enum COMMAND{
    get_pos,
    set_pos,
    go_to,
    stop,
    panic,
    get_gp2,
    ack = 254,
    nack = 255
};

class Ctfcom
{
private:
    RawSerial _com;
    list<uint8_t> list_buffer;
    uint16_t read_timout_ms = DEFAULT_TIMOUT_MS;

    void send(const uint8_t *data, uint32_t len);
    template<typename... NoTypeDataPack, typename NoTypeData>
    void build_data_frame_recursive(uint8_t *buffer, uint16_t index, NoTypeData arg, NoTypeDataPack... args);
    void build_data_frame_recursive(uint8_t *buffer, uint16_t index); //leaf
    void decode(); // leaf
    uint8_t read_lock;
    uint8_t write_lock;
    void lock_write_ressource(void);
    void lock_read_ressource(void);
    void unlock_write_ressource(void);
    void unlock_read_ressource(void);


public:
    Ctfcom(PinName Tx, PinName Rx);
    Ctfcom(PinName Tx, PinName Rx, int baud);
    Ctfcom(PinName Tx, PinName Rx, int baud, uint16_t read_timout_in_ms);
    ~Ctfcom();
    uint8_t recive(void);
    int answer_get_pos(float x, float y, float angle);
    int answer_set_pos(float *x, float *y, float *angle);
    void it_handler(void);
    uint8_t get_recived_data();
    size_t get_len_recived_data();
    template<typename... NoTypeDataPack, typename NoTypeData>
    void decode(NoTypeData *arg, NoTypeDataPack * ...args);
    template<typename... NoTypeDataPack>
    void generic_send(COMMAND cmd, NoTypeDataPack... args);
    void generic_send(COMMAND cmd);

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


template<typename... NoTypeDataPack, typename NoTypeData>
void Ctfcom::decode(NoTypeData *arg, NoTypeDataPack * ...args){
    uint8_t arg_size = sizeof(*arg);
    while (get_len_recived_data() < arg_size){}
    for (uint8_t i = 0; i < arg_size; i++)
    {
        ((uint8_t *)arg)[i] = get_recived_data();
    }
    decode(args...);
}


#endif

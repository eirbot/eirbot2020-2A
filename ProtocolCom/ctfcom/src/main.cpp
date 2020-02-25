#include <mbed.h>
#include "ctfcom.h"

Ctfcom com(SERIAL_TX, SERIAL_RX, 115200);

float x,y,angle;

int main() {

  // put your setup code here, to run once:
  //com.it_handler();
  //com.send_pos(0.2,0.3,0.4);
  uint8_t recived_order;
  x = 0.0f;
  y = 1.0f;
  angle = 0.365f;
  volatile size_t frames_recived = 0;
  while(1) {
    frames_recived = com.get_len_recived_data();
    if (frames_recived >= 1)
    {
      recived_order = com.get_recived_data();
      if (recived_order == get_pos)
      {
        com.send_pos(0.2,0.3,0.4);
      }else if(recived_order == set_pos){
        com.decode(&x,&y,&angle);
      }
    }
    
  }
}
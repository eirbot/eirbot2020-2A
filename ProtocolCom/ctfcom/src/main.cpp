#include <mbed.h>
#include "ctfcom.h"

Ctfcom com(SERIAL_TX, SERIAL_RX, 115200);

int main() {

  // put your setup code here, to run once:
  //com.it_handler();
  //com.send_pos(0.2,0.3,0.4);
  while(1) {
    if (com.get_len_recived_data() >= 1)
    {
      if (com.get_recived_data() == get_pos)
      {
        //com.send_pos(0.2,0.3,0.4);
        com.generic_send(get_pos, 3, 0.2, 0.3, 0.4);
      }
      
    }
    
  }
}
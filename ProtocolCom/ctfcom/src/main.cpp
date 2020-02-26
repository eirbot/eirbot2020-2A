#include <mbed.h>
#include "ctfcom.h"

Ctfcom com(SERIAL_TX, SERIAL_RX, 115200);

float x,y,angle;

void rpi_command_middleware(void){
  uint8_t recived_order;
  uint8_t frame_recived_is_known = 1;
  size_t frames_recived = com.get_len_recived_data();
  if (frames_recived >= 1)
  {
    recived_order = com.get_recived_data();
    if (recived_order == get_pos)
    {
      com.send_pos(0.2,0.3,0.4);
    }else if(recived_order == set_pos){
      //while (com.get_len_recived_data() < 12){}
      com.decode(&x,&y,&angle);
    }else if(recived_order == go_to){
      // com.decode(&x,&y,&angle);
    }else if(recived_order == stop){
      // Stop the robot
    }else if(recived_order == panic){
      // The rpi is dead, go to the end
    }else if(recived_order == get_gp2){
      // com.generic_send(get_gp2,OUI);
    }else{
      frame_recived_is_known = 0;
    }

    if(frame_recived_is_known){
      com.generic_send(ack);
    }else
    {
      com.generic_send(nack);
      // traiter l'erreur
    }
    
  }
}


int main() {

  // put your setup code here, to run once:
  //com.it_handler();
  //com.send_pos(0.2,0.3,0.4);
  x = 0.0f;
  y = 1.0f;
  angle = 0.365f;

  while(1) {
    rpi_command_middleware();
    
  }
}
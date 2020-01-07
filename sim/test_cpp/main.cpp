#include <string.h>
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

extern "C" {
#include "extApi.h"
}

int main(void)
{
  simxInt client_id = simxStart("127.0.0.1", 3000, true, true, 1000, 10);
  if(client_id == -1) {
    cerr << "ERROR : Cannot connect to simulation" << endl;
    return -1;
  }

  simxInt left_motor_handle = -1;
  simxInt right_motor_handle = -1;

  simxInt left_encoder_handle = -1;
  simxInt right_encoder_handle = -1;

  const char* gp2_names[] = {
    "gp2_1",
    "gp2_2",
    "gp2_3",
    "gp2_4",
    "gp2_5",
    "gp2_6",
    "gp2_7",
    "gp2_8",
    "gp2_9",
    "gp2_10",
    "gp2_11",
    "gp2_12",
    "gp2_13",
    "gp2_14",
    "gp2_15",
    "gp2_16",
    "gp2_17",
    "gp2_18",
    "gp2_19",
    "gp2_20",
    "gp2_21",
    "gp2_22",
    "gp2_23",
    "gp2_24",
  };

  simxInt gp2[sizeof(gp2_names)/sizeof(gp2_names[0])] = {};
  simxFloat gp2_pos[sizeof(gp2_names)/sizeof(gp2_names[0])][3] = {};

  if(simxGetObjectHandle(client_id, "left_motor_joint", &left_motor_handle, simx_opmode_oneshot_wait) != simx_return_ok) {
    cerr << "ERROR : Could not find left_motor_joint" << endl;
    return -1;
  }

  if(simxGetObjectHandle(client_id, "right_motor_joint", &right_motor_handle, simx_opmode_oneshot_wait) != simx_return_ok) {
    cerr << "ERROR : Could not find right_motor_joint" << endl;
    return -1;
  }

  if(simxGetObjectHandle(client_id, "left_encoder_joint", &left_encoder_handle, simx_opmode_oneshot_wait) != simx_return_ok) {
    cerr << "ERROR : Could not find left_encoder_joint" << endl;
    return -1;
  }

  if(simxGetObjectHandle(client_id, "right_encoder_joint", &right_encoder_handle, simx_opmode_oneshot_wait) != simx_return_ok) {
    cerr << "ERROR : Could not find right_encoder_joint" << endl;
    return -1;
  }

  for(int i = 0 ; i < sizeof(gp2)/sizeof(gp2[0]) ; i++) {
    if(simxGetObjectHandle(client_id, gp2_names[i], &gp2[i], simx_opmode_oneshot_wait) != simx_return_ok) {
      cerr << "ERROR : Could not find " << gp2_names[i] << endl;
      return -1;
    }
  }

  for(int i = 0 ; i < sizeof(gp2)/sizeof(gp2[0]) ; i++) {
    simxReadProximitySensor(client_id, gp2[i], NULL, NULL, NULL, NULL, simx_opmode_streaming);
    simxGetObjectPosition (client_id, gp2[i], -1, gp2_pos[i], simx_opmode_streaming);
  }

  while(1) {
    simxFloat detect[3] = {};
    simxUChar state = 0;
    for(int i = 0 ; i < sizeof(gp2)/sizeof(gp2[0]) ; i++) {
      simxReadProximitySensor(client_id, gp2[i], &state, detect, NULL, NULL, simx_opmode_buffer);
      simxGetObjectPosition (client_id, gp2[i], -1, gp2_pos[i], simx_opmode_buffer);
      if (state) {
        double dist = sqrt(detect[0]*detect[0]+detect[1]*detect[1]+detect[2]*detect[2]);
        int d = dist * 1000;
        cout << right << setw(4) << d << ";";
      }
      else {
        cout << "NONE;";
      }
    }
    cout << endl;
  }

  simxFinish(client_id);
  return 0;
}

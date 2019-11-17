#include <string.h>
#include <iostream>
#include <iomanip>
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

  {
    simxFloat v = 0;
    cout << "Enter motors speed : " << endl;
    cin >> v;
    simxSetJointTargetVelocity(client_id, left_motor_handle, v*0.01745322, simx_opmode_oneshot_wait);
    simxSetJointTargetVelocity(client_id, right_motor_handle, v*0.01745322, simx_opmode_oneshot_wait);
  }

  for(int i = 0 ; i < 10 ; i++) {
    simxFloat lp = 0;
    simxFloat rp = 0;
    simxGetJointPosition(client_id, left_encoder_handle, &lp, simx_opmode_oneshot_wait);
    simxGetJointPosition(client_id, right_encoder_handle, &rp, simx_opmode_oneshot_wait);
    cout << left << std::setw(10) << lp << " ; " << std::setw(10) << rp << endl;
  }

  simxFinish(client_id);
  return 0;
}

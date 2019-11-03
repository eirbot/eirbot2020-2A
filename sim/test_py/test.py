#! /usr/bin/python3

# Copy needed files before importing vrep
import shutil
import os

VREP_PATH = os.getenv("VREP_PATH", "/mnt/c/Program Files/V-REP3/V-REP_PRO_EDU")
VREP_PYTHON_PATH = os.path.join(VREP_PATH, "programming/remoteApiBindings/python/python")
VREP_LIB_PATH = os.path.join(VREP_PATH, "programming/remoteApiBindings/lib/lib/Linux/64Bit")

if not os.path.exists("vrep.py"):
    shutil.copy(os.path.join(VREP_PYTHON_PATH, "vrep.py"), os.path.dirname(__file__))

if not os.path.exists("vrepConst.py"):
    shutil.copy(os.path.join(VREP_PYTHON_PATH, "vrepConst.py"), os.path.dirname(__file__))

if not os.path.exists("remoteApi.so"):
    shutil.copy(os.path.join(VREP_LIB_PATH, "remoteApi.so"), os.path.dirname(__file__))

# Models path
MODEL_PATH = os.getenv("MODEL_PATH", "D:/work/eirbot2020-2A/sim/model")

# Now we can talk
import vrep

client_id = vrep.simxStart('127.0.0.1',19997,True,True,5000,5)
if client_id == -1:
    print("ERROR : Cannot connect to simulation")
    exit(0)

res, model_id = vrep.simxLoadModel(client_id, os.path.join(MODEL_PATH, "ceres.ttm"), 0, vrep.simx_opmode_blocking)
if res != vrep.simx_return_ok:
    print("ERROR : Cannot load model (" + str(res) + ")")

vrep.simxFinish(client_id)

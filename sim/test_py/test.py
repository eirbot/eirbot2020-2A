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

# Launch vrep
import subprocess
import atexit

vrep_process = subprocess.Popen([VREP_PATH+"/vrep.exe"])

atexit.register(lambda: vrep_process.terminate())

# Connect
import vrep

client_id = -1

while client_id == -1:
    client_id = vrep.simxStart('127.0.0.1',19997,True,True,5000,50)
    if client_id == -1:
        print("ERROR : Cannot connect to simulation")

# Remove floor
res, floor_id = vrep.simxGetObjectHandle(client_id, "ResizableFloor_5_25", vrep.simx_opmode_blocking)
if res != vrep.simx_return_ok:
    print("ERROR : Cannot get handle (" + str(res) + ")")

res = vrep.simxRemoveObject(client_id, floor_id, vrep.simx_opmode_blocking)
if res != vrep.simx_return_ok:
    print("ERROR : Cannot remove object (" + str(res) + ")")

res, floor_id = vrep.simxGetObjectHandle(client_id, "ResizableFloor_5_25_element", vrep.simx_opmode_blocking)
if res != vrep.simx_return_ok:
    print("ERROR : Cannot get handle (" + str(res) + ")")

res = vrep.simxRemoveObject(client_id, floor_id, vrep.simx_opmode_blocking)
if res != vrep.simx_return_ok:
    print("ERROR : Cannot remove object (" + str(res) + ")")

res, floor_id = vrep.simxGetObjectHandle(client_id, "ResizableFloor_5_25_visibleElement", vrep.simx_opmode_blocking)
if res != vrep.simx_return_ok:
    print("ERROR : Cannot get handle (" + str(res) + ")")

res = vrep.simxRemoveObject(client_id, floor_id, vrep.simx_opmode_blocking)
if res != vrep.simx_return_ok:
    print("ERROR : Cannot remove object (" + str(res) + ")")

# Add sim models
res, ceres_id = vrep.simxLoadModel(client_id, os.path.join(MODEL_PATH, "ceres.ttm"), 0, vrep.simx_opmode_blocking)
if res != vrep.simx_return_ok:
    print("ERROR : Cannot load model (" + str(res) + ")")

res, table_id = vrep.simxLoadModel(client_id, os.path.join(MODEL_PATH, "table.ttm"), 0, vrep.simx_opmode_blocking)
if res != vrep.simx_return_ok:
    print("ERROR : Cannot load model (" + str(res) + ")")

# Get Camera
res, cam_id = vrep.simxGetObjectHandle(client_id, "Vision_sensor", vrep.simx_opmode_blocking)
if res != vrep.simx_return_ok:
    print("ERROR : Cannot get handle (" + str(res) + ")")

# Start simulation
vrep.simxStartSimulation(client_id, vrep.simx_opmode_blocking)

# Get video stream
import matplotlib.pyplot as plt
import numpy as np

err, resolution, image = vrep.simxGetVisionSensorImage(client_id, cam_id, 0, vrep.simx_opmode_streaming_split)
while (vrep.simxGetConnectionId(client_id) != -1):
    err, resolution, image = vrep.simxGetVisionSensorImage(client_id, cam_id, 0, vrep.simx_opmode_buffer)
    if res != vrep.simx_return_ok:
        print("ERROR : simxGetVisionSensorImage (" + str(res) + ")")
        pass
    else:
        print (len(image))
        if len(image) != 0:
            img = np.array(image,dtype=np.uint8)
            img.resize([resolution[1],resolution[0],3])
            plt.imshow(img, origin='lower')
        plt.pause(0.05)

plt.show()

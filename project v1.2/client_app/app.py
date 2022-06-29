from websocket import create_connection
import json
import time
from datetime import datetime
import tzlocal

#### Data section

## Autorization on VEGA server
autreq = {"cmd":"auth_req", # Don't change!
"login": "root", # Login name
"password": "123456" # password
}
## Get information from connected server
srvinfo = {"cmd":"server_info_req"} # Don't change!

## Get device list w/attributes
devalist = {"cmd":"get_device_appdata_req"} # Don't change!

##Get data from devices
datadev = {"cmd":"get_data_req",
            "devEui":"AC1F09FFFE015304"
    } # Don't change!

##Get reg users
reguser = {"cmd": "get_users_req"} # Don't change!

### ============
### Code section

##Connection to VEGA server
ws = create_connection("ws://192.168.0.38:8002/") # Start connection. IP Address:Port VEGA server
ws.send(json.dumps(autreq))                    # Get Autorization command
autresp = ws.recv()                         #Status (responce) of execute command

print(autresp)
print("==========================")

## Get server info
ws.send(json.dumps(srvinfo))                    # Get command
srvinfresp = ws.recv()
infresp_dict = json.loads(srvinfresp)
time_serv_now = infresp_dict["time"]["utc"]/1000
local_timezone = tzlocal.get_localzone()
serv_time = datetime.fromtimestamp(time_serv_now, local_timezone)

print(srvinfresp)
print(serv_time.strftime("%Y-%m-%d %H:%M:%S"))
print("==========================")

## Get dev list w/attributes
ws.send(json.dumps(devalist))                    # Get command
devalistresp = ws.recv()

print(devalistresp)
print("==========================")

## Get registered users
ws.send(json.dumps(reguser))                    # Get command
reguserresponse = ws.recv()

print(reguserresponse)
print("==========================")

while(True):
    ## Get save data drom devices
    ws.send(json.dumps(datadev))                    # Get command
    datadevresp = ws.recv()
    data_dict = json.loads(datadevresp)
    keys = data_dict.keys()
    keys1 = str(keys)
    if keys1 == "dict_keys(['appEui', 'cmd', 'data_list', 'devEui', 'direction', 'status', 'totalNum'])":
        data = [{"devEui": data_dict["devEui"]}, {"data": data_dict["data_list"][0]["data"]}, {"type": data_dict["data_list"][0]["type"]}]
        data_temp = str(data)
        print(data_temp)
        realtime = data_dict["data_list"][0]["ts"]/1000
        packet_time = datetime.fromtimestamp(realtime, local_timezone)
        print(packet_time.strftime("%Y-%m-%d %H:%M:%S"))
    else:
        print(datadevresp)
    print("==========================")
    time.sleep(5)

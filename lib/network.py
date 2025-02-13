from time import sleep

data = []
now = [0, 0]
old = [0, 0]
interfaces = ["eth","wlan"]

def get_data():
    with open("/proc/net/dev", mode="r") as f:
        for l in f.readlines():
            for interface in interfaces:
              if(interface in l):
                  data = [e for e in l.split(" ") if e != ""]
    return [int(data[1]), int(data[9])]
  
print(get_data())
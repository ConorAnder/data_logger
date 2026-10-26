from datetime import datetime
from serial import Serial

dt = datetime.today()
msg = f'{dt.year} {dt.month} {dt.day} {dt.hour} {dt.minute} {dt.second}'
print(msg)

s = Serial('/dev/ttyUSB0', 115200)
s.write(msg.encode('UTF-8'))

print(s.readline().decode().strip())
s.close()

import serial
import time
import datetime
import serial.tools.list_ports

print('-----------------------------------------------------------------------')
ports = serial.tools.list_ports.comports()
for port, desc, hwid in sorted(ports):
    print("{}: {} [{}]".format(port, desc, hwid))

ser = serial.Serial('COM4')
print(ser.name)  # check which port was really used
ser.baudrate = 115200
ser.close()
ser.open()
ser.write(b'CLRS ')
time.sleep(1)
n = 100000
while n > 0:
    n -= 1

    current_time = datetime.datetime.now()

    year = current_time.strftime("%Y")
    print("year:", year)

    month = current_time.strftime("%m")
    print("month:", month)

    day = current_time.strftime("%d")
    print("day:", day)

    vfd_time = current_time.strftime("%H:%M:%S")
    print("time:", vfd_time)
    vfd_buf = str.encode('ASCH ' + '      ' + year + ' ' + month + ' ' + day + '    ' + vfd_time + '       ')
    print(vfd_buf)
    ser.write(vfd_buf)
    #560 bytes for full screen
    #VFD28016_RAM = bytearray([0xAA]*560)
    #VFD28016_RAM[559] = 0xff
    #VFD28016_RAM[558] = 0xAA
    #vfd_buf = str.encode('WRAM ')
    #print(vfd_buf)
    #print(VFD28016_RAM)
    #ser.write(vfd_buf)
    #ser.write(VFD28016_RAM)
    time.sleep(0.1)

# ser.write(b'ASCH  Sep 28 2020 Monday         PM     ')
ser.close()
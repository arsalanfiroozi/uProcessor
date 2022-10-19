import serial
import serial.tools.list_ports as port_list
import time

Port = 'COM5'

print('List of available ports:')
ports = list(port_list.comports())
for p in ports:
    print(p)
print('-----------------------')
# a=0;
# for i in range(10):
#     in_hex = input("Enter a hex value(0 -> f): ");
#     a=a+1;
# print(a)

ser = serial.Serial(Port, baudrate=9600, timeout=1, bytesize=8, stopbits=serial.STOPBITS_ONE);
print('Connecting to ' + ser.name)         # check which port was really used

for i in range(100):
    in_hex = input("Enter a hex value(0 -> f): ")
    ser.write(bytes(in_hex, encoding='utf8'));
    x = ser.read(4)
    # a = ser.read() 
    # b = ser.read()   
    # c = ser.read()   
    # d = ser.read()   
    print('Input: ' + str(int(in_hex,16)) + ' ==> CRC32: ' + str(hex(int.from_bytes(x, byteorder='little'))))
    # print(str(int.from_bytes(a, byteorder='little')))
    # print(str(int.from_bytes(b, byteorder='little')))
    # print(str(int.from_bytes(c, byteorder='little')))
    # print(str(int.from_bytes(d, byteorder='little')))
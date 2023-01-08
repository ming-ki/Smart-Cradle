from bluetooth import *

socket = BluetoothSocket( RFCOMM )
socket.connect(("00:20:10:08:D3:65", 1))
print("bluetooth connected!")

msg = input("send message : ")
socket.send(msg)

print("finished")
socket.close()

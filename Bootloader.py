# -*- coding: utf-8 -*-
"""
Created on Thu Sep 17 14:39:54 2020

@author: Diaa Eldeen
"""


import sys, serial, binascii, time, timeit

from intelhex import IntelHex


print("Sending the image")


filename = "BlinkLED_test/main.hex"

ih = IntelHex()  
ih.loadhex(filename)


startAddr = ih.minaddr()
maxAddr = ih.maxaddr()
content = ih.todict()

addr = startAddr

COMMAND_PROG = b'PROG '
COMMAND_PROG_DONE = b'Download complete\n'
COMMAND_JUMP = b'JUMP '


# open the port with those settings and close it after the with block
with serial.Serial(port = "COM3", baudrate=921600,
            bytesize=8, timeout=2, 
            stopbits=serial.STOPBITS_ONE) as serialPort:

    startTm = time.time()

    serialPort.write(b'\n')     # End any previous line

    serialPort.write(COMMAND_PROG) 
    serialPort.write(bytes(str(startAddr) + ' ', 'utf-8')) # Address
    serialPort.write(bytes(str(maxAddr-startAddr+1) + '\n', 'utf-8')) # Size
    serialPort.flush()
    message = serialPort.readline()
    
    if(message != b'Receiving the application image\n'):
        print("Error sending the program command")
        sys.exit(0)
    
    
    while(addr<=maxAddr):
        try:
            data = content[addr]
        except KeyError:    # empty address
            data = 0xff
        
        #send data
        serialPort.write([data])
        serialPort.flush()
        addr +=1
    
    # Verify receiving the image
    message = serialPort.readline()
    if(message == COMMAND_PROG_DONE):
        print("Image sent successfully")
    else:
        print(message)
        print("Error executing the program command")
        sys.exit(0)
    
    endTm = time.time()
    
    # Jump to the written image    
    serialPort.write(COMMAND_JUMP)
    serialPort.write(bytes(str(startAddr) + '\n', 'utf-8')) # Address
    print("Jumped to the new image")




print("Time taken: %f seconds" %(endTm - startTm))
print("Size: %d bytes" %(maxAddr-startAddr+1))
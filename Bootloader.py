# -*- coding: utf-8 -*-
"""
Created on Wed Nov  4 13:37:18 2020

@author: Diaa Eldeen
"""


import sys, serial, io, binascii, time, timeit

from intelhex import IntelHex


print("Booting")


filename = "BlinkLED_test/main.hex"

ih = IntelHex()  
ih.loadhex(filename)


startAddr = ih.minaddr()
maxAddr = ih.maxaddr()
content = ih.todict()

addr = startAddr

COMMAND_PROG = 'PROG '
COMMAND_PROG_DONE = 'Download complete\n'
COMMAND_JUMP = 'JUMP '


# open the port with those settings and close it after the with block
with serial.Serial(port = "COM3", baudrate=921600,
            bytesize=8, timeout=2, 
            stopbits=serial.STOPBITS_ONE) as serialPort:

    startTm = time.time()

    sio = io.TextIOWrapper(io.BufferedRWPair(serialPort, serialPort))

    sio.write('\n')     # End any previous line

    sio.write(COMMAND_PROG) 
    sio.write(str(startAddr) + ' ') # Address
    sio.write(str(maxAddr-startAddr+1) + '\n') # Size
    
    sio.flush()
    
    message = sio.readline()
    
    if(message != 'Receiving the application image\n'):
        print("Error sending the program command")
        sys.exit(0)
    
    
    while(addr<=maxAddr):
        try:
            data = content[addr]
        except KeyError:    # empty address
            data = 0xff
        
        #send data
        sio.write([data])
        sio.flush()
        addr +=1
    
    # Verify receiving the image
    message = sio.readline()
    if(message == COMMAND_PROG_DONE):
        print("Image sent successfully")
    else:
        print(message)
        print("Error executing the program command")
        sys.exit(0)
    
    endTm = time.time()
    
    # Jump to the written image    
    sio.write(COMMAND_JUMP)
    sio.write(str(startAddr) + '\n') # Address
    sio.flush()
    print("Jumped to the new image")




print("Time taken: %f seconds" %(endTm - startTm))
print("Size: %d bytes" %(maxAddr-startAddr+1))
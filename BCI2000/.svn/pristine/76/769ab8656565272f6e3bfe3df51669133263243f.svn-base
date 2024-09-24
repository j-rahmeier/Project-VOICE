#! /usr/bin/env python3

import socket
from select import select
from multiprocessing import shared_memory
import matplotlib.pyplot as plt
import numpy as np
import io
import traceback
import platform
from enum import Enum

class Object(object):
    pass

def waitForRead(sock):
    """polling wait for data on the socket so we may react to a keyboard interrupt"""
    pollingIntervalSeconds = 0.1
    ready, _, _ = select([sock], [], [], pollingIntervalSeconds)
    while not ready:
        try:
            ready, _, _ = select([sock], [], [], pollingIntervalSeconds)
        except KeyboardInterrupt:
            print('Keyboard interrupt, exiting')
            quit()
    

def readLine(stream, terminator = b'\n'):
    """read a line from a stream up to terminator character"""
    chars = []
    c = stream.read(1)
    while c != terminator and c != b'':
        chars.append(c)
        c = stream.read(1)
    return str(b''.join(chars), 'utf-8')

class BciDescSupp(Enum):
    """BCI2000 descriptor and supplement for relevant messages"""
    SignalProperties = b'\x04\x03'
    SignalData = b'\x04\x01'

def readBciLengthField(stream, fieldSize):
    """read a length field of specified size from a stream"""
    # read fieldSize bytes that make up a little-endian number
    b = stream.read(fieldSize)
    if b == b'':
        raise EOFError()
    if len(b) != fieldSize:
        raise RuntimeError('Could not read size field')
    n = int.from_bytes(b, 'little')
    # if all bytes are 0xff, ignore them and read the field value as a string
    if n == (1 << (fieldSize * 8)) - 1:
        n = int(readLine(stream, b'\x00'))
    return n

def readBciIndexCount(stream):
    """read a channel or element index, ignoring the actual indices"""
    s = readLine(stream, b' ')
    if s == '{':
        n = 0
        s = readLine(stream, b' ')
        while s != '}':
            n += 1
            s = readLine(stream, b' ')
    else:
        n = int(s)
    return n

def readBciPhysicalUnit(stream):
    """read the members of a physical unit from a stream"""
    pu = Object()
    pu.offset = float(readLine(stream, b' '))
    pu.gain = float(readLine(stream, b' '))
    pu.unit = readLine(stream, b' ')
    pu.rawMin = float(readLine(stream, b' '))
    pu.rawMax = float(readLine(stream, b' '))
    return pu

def readBciSourceIdentifier(stream):
    """read a BCI2000 source identifier from a stream"""
    b = stream.read(1)
    if b != b'\xff':
        return str(b[0])
    return readLine(stream, b'\x00')

def readBciRawMessage(stream):
    """read a full raw BCI2000 message from a stream"""
    descsupp = stream.read(2) # get descriptor and descriptor supplement
    if descsupp == b'':
        raise EOFError()
    if len(descsupp) != 2:
        raise RuntimeError('Could not read descriptor fields')
    messageLength = readBciLengthField(stream, 2)
    chunks = []
    bytesRead = 0
    while bytesRead < messageLength:
        chunk = stream.read(min(messageLength - bytesRead, 2048))
        if chunk == b'':
            raise EOFError()
        chunks.append(chunk)
        bytesRead = bytesRead + len(chunk)
    return descsupp, b''.join(chunks)

def parseBciSignalProperties(stream):
    """parse a raw signal properties message into an object"""
    sp = Object()
    sp.kind = 'signal properties'
    sp.sourceID = readBciSourceIdentifier(stream)
    sp.name = readLine(stream, b' ')
    sp.channels = readBciIndexCount(stream)
    sp.elements = readBciIndexCount(stream)
    sp.type = readLine(stream, b' ')
    sp.channelUnit = readBciPhysicalUnit(stream)
    sp.elementUnit = readBciPhysicalUnit(stream)
    return sp

def parseBciSignalData(stream):
    """parse a raw signal data message into an object"""
    signal = Object()
    signal.kind = 'signal'
    signal.sourceID = readBciSourceIdentifier(stream)
    signal.type = ord(stream.read(1))
    signal.channels = readBciLengthField(stream, 2)
    signal.elements = readBciLengthField(stream, 2)
    signal.shm = readLine(stream, b'\x00')

    if signal.channels != 0 and signal.elements != 0:
        if signal.type & 64 == 0:
            raise RuntimeError('Signal data not located in shared memory')
        signal.type = signal.type & ~64
        if signal.type == 0:
            signal.type = 'int16'
        elif signal.type == 1:
            signal.type = 'float24'
        elif signal.type == 2:
            signal.type = 'float32'
        elif signal.type == 3:
            signal.type = 'int32'
        else:
            raise RuntimeError('Invalid signal type')
        if platform.system() == 'Windows':
              signal.shm = signal.shm.split("/")[1]

    return signal

def receiveBciMessage(stream):
    """read and parse a single BCI2000 message from a stream"""
    descsupp, data = readBciRawMessage(stream)
    stream2 = io.BytesIO(data)
    if descsupp == BciDescSupp.SignalProperties.value:
        return parseBciSignalProperties(stream2)
    elif descsupp == BciDescSupp.SignalData.value:
        return parseBciSignalData(stream2)
    else:
        raise RuntimeError('Unexpected BCI2000 message type')

#user input
HOST, PORT = "localhost", 1879

#initialize variables
CHANNELS = -1
ELEMENTS = -1
setProps = False
chNames = []
memoryName = ""

figure, ax = plt.subplots(figsize=(10, 8))
ax.set_xlim(-2,2)
ax.set_ylim(-2,2)
figure.set_facecolor((0,0,0,1))
ax.set_axis_off()
ax.set_frame_on(0)
figure.canvas.draw()

#listen for connection on specified port
try:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen(1)
        while True:
            print("Waiting for BCI2000 on %s at port %i" %(HOST, PORT))
            waitForRead(s)
            conn, addr = s.accept()
            print('Connected by', addr)
            try:
                stream = conn.makefile('rb')
                while True: #go until we receive an EOFError exception
                    waitForRead(conn)
                    msg = receiveBciMessage(stream)
                        
                    if msg.kind == 'signal properties':
                        CHANNELS = msg.channels
                        chNames = range(1,CHANNELS+1)
                        ELEMENTS = msg.elements
                        #initialize variables once we have channels and elements
                        phi = np.zeros((CHANNELS, ELEMENTS))
                        bla = np.zeros((ELEMENTS,1))
                        lineArr = list(range(CHANNELS))
                        for i in range(0,CHANNELS):
                            lineArr[i], = ax.plot(bla, bla)

                        print("Properties: Channels: %i, Elements: %i" %(CHANNELS, ELEMENTS))

                    elif msg.kind == 'signal':
                        if msg.channels == 0 and msg.elements == 0:
                            continue;

                        if msg.channels != CHANNELS:
                            raise RuntimeError('Mismatch in number of channels')
                        if msg.elements != ELEMENTS:
                            raise RuntimeError('Mismatch in number of elements')

                        if memoryName != msg.shm:
                            # update shared memory object
                            memoryName = msg.shm
                            mem = shared_memory.SharedMemory(memoryName)
                            print(f"Connected to shared memory: {memoryName}")
                            print("Visualizing data...")
                        
                        #update visualization with new data
                        data = np.ndarray((CHANNELS,ELEMENTS),dtype=np.double, buffer=mem.buf)
                        for ch in range(0,CHANNELS):
                            for el in range(0, ELEMENTS):
                                phi[ch, el] = el*2*np.pi/(ELEMENTS-1)

                            xdata = np.multiply(1+0.003*data[ch,:],np.cos(phi[ch,:]))
                            ydata = np.multiply(1+0.003*data[ch,:],np.sin(phi[ch,:]))
                            #update plots
                            lineArr[ch].set_xdata(xdata)
                            lineArr[ch].set_ydata(ydata)

                        #update figure
                        figure.canvas.draw()
                        plt.pause(0.01) #render update

                    else:
                        raise RuntimeException('Unexpected message')

            except EOFError:
                print('disconnected')
                continue;

            except Exception:
                traceback.print_exc()

except KeyboardInterrupt:
    print('aborted by user')



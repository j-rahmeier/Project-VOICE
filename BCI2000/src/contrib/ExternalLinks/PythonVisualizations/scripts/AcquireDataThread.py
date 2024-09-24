import threading
import socket
from select import select
from multiprocessing import shared_memory
import numpy as np
import io
import traceback
import platform
from enum import Enum
#
# Data acquisition thread from BCI2000's shared memory
#

class Object(object):
    pass

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
def readBciIndexList(stream):
    """obtain the list of values"""
    v = []
    s = readLine(stream, b' ')
    if s == '{':
        s = readLine(stream, b' ')
        while s != '}':
            v.append(s.replace('%20', ' '))
            s = readLine(stream, b' ')
    else:
        v = range(1, int(s)+1)
    return v

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
    #sp.channels = readBciIndexCount(stream)
    sp.chNames = readBciIndexList(stream)
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


class AcquireDataThread(threading.Thread):
    def __init__(self, address, log):
        super(AcquireDataThread, self).__init__()
        self.print = log
        self.chNames = []
        self.newData = False
        self.go = True
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.bind((address[0], address[1]))
        self.s.listen(1)
        self.print("Waiting for BCI2000 on %s at port %i" %(address[0], address[1]))
        self.s.settimeout(0.1)

        self.propsInit = False
        self.CHANNELS = -1
        self.ELEMENTS = -1

    def waitForRead(self, sock):
        """polling wait for data on the socket so we may react to a keyboard interrupt"""
        pollingIntervalSeconds = 0.1
        ready, _, _ = select([sock], [], [], pollingIntervalSeconds)
        while not ready and self.go:
            try:
                ready, _, _ = select([sock], [], [], pollingIntervalSeconds)
            except KeyboardInterrupt:
                self.print('Keyboard interrupt, exiting')
                self.go = False
                quit()

    def run(self):
      #listen for connection on specified port
      memoryName = ""
      while self.go:
        try:
          self.waitForRead(self.s)
          conn, addr = self.s.accept()
          self.print(f"Connected by {addr}")
          stream = conn.makefile('rb')
          while self.go: #go until we receive an EOFError exception
            self.waitForRead(conn)
            msg = receiveBciMessage(stream)
           # print("Received: " + msg.kind)
                
            if msg.kind == 'signal properties':
              chs = len(msg.chNames)
              self.chNames = msg.chNames
              #self.chNames = [str(x) for x in range(1,self.CHANNELS+1)]

              els = msg.elements
              self.rawData = np.full((chs,els), 1, dtype=np.double)

              if self.CHANNELS == -1 and self.ELEMENTS == -1:
                #first initialization
                self.print("Properties: Channels: %i, Elements: %i" %(chs, els))
              else:
                if self.CHANNELS != chs:
                  self.print(f"Changed to {chs} channels")
                if self.ELEMENTS != els:
                  self.print(f"Changed to {els} elements")
              self.CHANNELS = chs
              self.ELEMENTS = els
              self.newData = True

            elif msg.kind == 'signal':
              if msg.channels == 0 and msg.elements == 0:
                #flag for end of connection
                self.propsInit = False
                continue

              if msg.channels != self.CHANNELS:
                raise RuntimeError('Mismatch in number of channels')
              if msg.elements != self.ELEMENTS:
                raise RuntimeError('Mismatch in number of elements')

              if memoryName != msg.shm:
                # update shared memory object
                memoryName = msg.shm
                mem = shared_memory.SharedMemory(memoryName)
                #self.print(f"Connected to shared memory: {memoryName}")
              
              #update visualization with new data
              data = np.ndarray((self.CHANNELS,self.ELEMENTS),dtype=np.double, buffer=mem.buf)
              for ch in range(0,self.CHANNELS):
                self.rawData[ch] = data[ch,0:self.ELEMENTS]
              self.newData = True

            else:
              raise RuntimeError('Unexpected message')

        except EOFError:
          #self.print('disconnected')
          #s.close()
          continue

        except socket.timeout:
          pass

        except Exception:
          traceback.print_exc()
        



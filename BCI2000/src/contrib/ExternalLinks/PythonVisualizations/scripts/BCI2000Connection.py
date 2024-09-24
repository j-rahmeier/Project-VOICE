import sys
import os
import importlib
import threading
import time

def BCI2000Instance(progPath):
    #set directory to where the python file was run in case path is relative
    myDir = os.path.dirname(sys.argv[0])
    if myDir != '':
      os.chdir(myDir)
    sys.path.append(progPath) #BCI2000 prog path
    BCI2000Remote = importlib.import_module('BCI2000Remote') #in prog folder
    return BCI2000Remote.BCI2000Remote() #init BCI2000 remote

class BCI2000Thread(threading.Thread):
  def __init__(self, bci, initBCI2000, bciLock, win):
    super(BCI2000Thread, self).__init__() #thread parent constructor
    self.bci = bci
    self.initBCI2000 = initBCI2000
    self.bciLock = bciLock
    self.win = win
    self.connected = False
  def run(self):
    #self.win.print(" BCI2000...")
    self.win.print("Waiting to connect to BCI2000...")
    self.bci.Connect()
    self.bci.Execute('Wait for Idle|Suspended|Resting|Initialization|ParamsModified 2')
    while (self.bci.Result == "false"):
      self.win.print(self.bci.GetSystemState())
      self.win.print("Waiting for the run to be stopped")
      print(self.bci.GetSystemState())
      print("Waiting for the run to be stopped")
      self.bci.Execute('Wait for Idle|Suspended|Resting|Initialization|ParamsModified 5')
    #print(self.bci.GetSystemState())
    if self.bci.GetSystemState() == "Idle":
      self.bci.WindowVisible = True
      self.initBCI2000(self.bci)
    else:
      #self.win.print("BCI2000 IS ALREADY INITIALIZED, NOT LOADING NEW PARAMETERS")
      self.win.print("BCI2000 is already initialized- not loading new parameters")
    setRequiredVariables(self, self.bci) #required for pipeline
    self.connected = True
    #self.bciLock.release() #bci2000 is initialized

def getDataFileName(bci):
    #gets absolute path from BCI2000 parameters and Operator Path
    try:
        d = bci.GetParameter('DataDirectory')
        s = bci.GetParameter('DataFile')
    except:
        time.sleep(1)
        return getDataFileName(bci)
    pNames = s.split("${")
    for i in range(len(pNames)):
        if '}' in pNames[i]:
            #get parameter name
            names = pNames[i].split("}")
            value = bci.GetParameter(names[0])
            names[0] = value
            newP = ''.join(names)
            pNames[i] = newP
    newS = ''.join(pNames)
    absPath = bci.OperatorPath
    relPath = d + '/' + newS
    absPath = absPath.replace('Operator.exe', relPath)
    return absPath

def setRequiredVariables(bciThr, bci):
    #setSharingAddress(bci, address)
    bciThr.savePath = getDataFileName(bci)

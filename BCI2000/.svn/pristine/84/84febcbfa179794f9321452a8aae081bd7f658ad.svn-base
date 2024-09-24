
from scripts.BCI2000Connection import *

from scripts.PACVisualization import PACWindow
from scripts.CCEPVisualization import CCEPWindow
from scripts.AcquireDataThread import AcquireDataThread
from scripts.DefaultVisualization import DefaultWindow
from scripts.SharedVisualization import AbstractWindow
import pyqtgraph as pg
from enum import Enum
from pyqtgraph.Qt import QtCore
#
# Helper file to combine data thread, BCI2000 connection, and visualization
#

#bci instance if none is given, contains required commands
def defaultBCI(bci):
    bci.WindowVisible = False
    bci.Execute('Wait for Connected')
#main function, first one run
def main(bciConfig = defaultBCI, path="../../prog", winSize = (1600, 1200), timerUpdate=30 ):
    bciInst = BCI2000Instance(path)
    #start gui
    pg.mkQApp() #init app
    win = AbstractWindow()

    bciLock = threading.Lock()
    bciThread = BCI2000Thread(bciInst, bciConfig, bciLock, win)

    bciLock.acquire()
    bciThread.start()
    #bciLock.acquire()
    MainWindow(bciThread, winSize, timerUpdate, win) #start the rest of the visualization


#define all available python visualization types here
#name must be equivalent to the Filter name in the Signal Processing pipeline
class VisTypes(Enum):
    Error       = 0
    Default     = 1
    PACFilter   = 2
    CCEPFilter  = 3

class MainWindow():
    def __init__(self, bciThread, winSize, timerUpdate, win):
        self.window = win
        self.bciThread = bciThread
        self.visType = VisTypes.Error
        self.winSize = winSize
        self.timerUpdate = timerUpdate

        self.t1 = QtCore.QTimer()
        self.t1.setInterval(100)
        self.t1.timeout.connect(self.getVisualizationType)
        self.t1.start()

        sys.exit(pg.mkQApp().exec_())

    #find which filter we are visualizing
    def getVisualizationType(self):
      shared = False
      if self.bciThread.connected == True:
        self.window.print("Connected to BCI2000!")
        #should only enter this once
        self.bciThread.bci.Execute( "LIST PARAMETERS ")
        res = self.bciThread.bci.Result
        p = res.split("\n")
        for r in p:
          pStr = r.split(" ")
          #st = r.split(" ")[0]
          if pStr[0] == "SignalSharing:Processing%20Stages":
            addyStr = pStr[3]
            #addyStr = self.bciThread.bci.GetParameter(a)
            if addyStr != "%":
              #shared
              shared = True
              addy = addyStr.split(':')
              if len(addy) != 2:
                self.window.print("Fix the address given for " + self.visType.name)
                continue
              addy[1] = int(addy[1]) #convert to int

              #try to find unique filter
              f = pStr[2].replace("Share","")
              f = f.replace("=","")
              for t in VisTypes:
                if f == t.name:
                  #found unique filter
                  self.visType = t
                  break

        if shared and self.visType == VisTypes.Error:
          self.visType = VisTypes.Default #use default vis if signal is simply shared
        
        if shared:
          self.window.print("Initializing %s Visualization" %(self.visType.name))
          print("\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%")
          print("Initializing %s Visualization" %(self.visType.name))
          print("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n")

          #start to get data
          self.acqThr = AcquireDataThread(addy, self.window.print)
          self.acqThr.start()

          #initialize window
          self.visualize(self.bciThread, self.acqThr, self.visType, self.timerUpdate)
        else:
          self.window.print("BCI2000 data is not available")
          self.window.print("Please specify a parameter in the SignalSharing tab")

        self.t1.stop() #stop the timer

    def updatePlots(self):
      #this function is continually called
      try:
        bci2000State = self.bciThread.bci.GetSystemState()
        #each case should be true sparingly
        if self.initialized and not self.acqThr.propsInit:
          #print("State: resetting")
          self.initialized = False
        if bci2000State == "Resting" and not self.initialized and self.acqThr.CHANNELS != -1:
          if self.win.saveImages and self.win.localConfig.saveFigs:
            self.win.saveFigures()
            self.win.saveImages = False

          #set config
          self.win.setConfig()
          self.initialized = True
          self.acqThr.propsInit = True
        elif bci2000State == "Running" and self.acqThr.newData and self.initialized:
          #pass along new data if everything is good to go
          #print("State: plotting")
          self.acqThr.newData = False
          self.win.plot()
          self.win.saveImages = True #only save images if there is data to be shown
        elif bci2000State == "":
          #we have disconnected?
          self.win.quitAll()

      except:
        self.win.quitAll()

    def visualize(self, bciThread, acqThr, visType, timerUpdate):
      #change visualization based on our signal processing pipeline
      if visType == VisTypes.PACFilter:
        self.win = PACWindow(self.window)
      elif visType == VisTypes.CCEPFilter:
        self.win = CCEPWindow(self.window)
      else:
        self.win = DefaultWindow(self.window)

      #init and pass other threads
      self.win.setThreads(bciThread, acqThr)
      self.window.setChild(self.win)
      self.win.initialize()
      self.window.print("Waiting for the configuration to be set...")

      self.initialized = False
      self.window.timer = QtCore.QTimer()
      self.window.timer.setInterval(timerUpdate)
      self.window.timer.timeout.connect(self.updatePlots)
      self.window.timer.start()

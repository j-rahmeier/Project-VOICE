from PyQt5.QtCore import QSettings, QByteArray
from pyqtgraph.dockarea import *

class SharedSettings():
  def __init__(self, name):
    self.settings = QSettings("BCI2000",name)
    
  #init dock geometry once dock is initialized
  def initDockArea(self, win):
    win.restoreGeometry(self.settings.value("geometry", QByteArray()))
    defDock = {'main': None, 'float': []}
    if self.settings.value("dockConfig", defDock) != defDock:
      try:
        win.area.restoreState(self.settings.value("dockConfig", defDock)) #default dock
      except:
        pass
  def save(self, win):
    self.settings.setValue("geometry", win.saveGeometry())
    #self.settings.setValue("windowState", win.saveState())
    self.settings.setValue("dockConfig", win.area.saveState())
    

class CCEPSettings(SharedSettings):
  def __init__(self, name):
    super().__init__(name)
    
    self.stds = self.settings.value("stds", 5)
    self.sortChs = eval(self.settings.value("sortChs", "False").lower().capitalize()) #take care of string
    self.saveFigs = eval(self.settings.value("saveFigs", "False").lower().capitalize())
    self.avgPlots = eval(self.settings.value("avgPlots", "False").lower().capitalize())
    self.maskStart = self.settings.value("maskStart", -5)
    self.maskEnd = self.settings.value("maskEnd", 15)
    self.visBegin = self.settings.value("visBegin", 0)
    self.maxWindows = self.settings.value("maxWindows", 25)
    self.maxPlots = self.settings.value("maxPlots", 0)

  def save(self, win):
    super().save(win)
    
    self.settings.setValue("stds", self.stds)
    self.settings.setValue("sortChs", bool(self.sortChs))
    self.settings.setValue("saveFigs", bool(self.saveFigs))
    self.settings.setValue("avgPlots", bool(self.avgPlots))
    self.settings.setValue("maskStart", self.maskStart)
    self.settings.setValue("maskEnd", self.maskEnd)
    self.settings.setValue("visBegin", self.visBegin)
    self.settings.setValue("maxWindows", self.maxWindows)
    self.settings.setValue("maxPlots", self.maxPlots)

        

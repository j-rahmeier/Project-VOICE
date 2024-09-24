# importing various libraries
import pyqtgraph as pg
import pyqtgraph.exporters
from pyqtgraph.dockarea import *
import numpy as np
import os
import sys
from PyQt5 import QtWidgets, QtGui
from pyqtgraph.dockarea import *
from scripts.localConfig import SharedSettings

#-----VISUALIZATION ABSTRACTION------#
class AbstractWindow(QtWidgets.QMainWindow):
  def __init__(self):
    super().__init__()
    self.child = None
    #set icon
    iconPath = '../../doc/htmlhelp/rsrc/bci2000logo.svg'
    if os.path.isfile(iconPath):
      self.setWindowIcon(QtGui.QIcon(iconPath))
    self.setWindowTitle("BCI2000 Python Visualization") #can be renamed by specific visualization later

    #init dock area
    self.area = DockArea()
    self.setCentralWidget(self.area)
    #create log to inform user as we set things up
    self.output = TextOutput()
    self.area.addDock(Dock("Log", widget=self.output))

    self.show()

  def setChild(self, child):
    self.child = child
  def closeEvent(self, event): #overrides QMainWindow closeEvent
    if self.child != None:
      self.child.closeEvent(event)
  #print to app log
  def print(self, text):
    self.output.append(">>" + text)
    self.output.moveCursor(QtGui.QTextCursor.End)

class AbstractVisualization():
  def __init__(self, win):
    self.win = win #QMainWindow that we are using
    print(self.__class__.__name__)
    self.localConfig = SharedSettings(self.__class__.__name__)
    pass

  def resizeEvent(self, event):
    QtWidgets.QMainWindow.resizeEvent(self, event)
    pass
  
  def setThreads(self, bciThread, acqThr):
    self.bciThread = bciThread
    self.acqThr = acqThr
    self.saveImages = False
    self.setStyles()
    self.windows = 0
    #load any local settings
    #self.localConfig = SharedSettings(name)
    #self.localConfig.setPreferences()
    #self.localConfig = config(name)
    pass
  def initialize(self):
    pass
  def setConfig(self):
    if hasattr(self.localConfig, 'maxWindows'):
      self.windows = min(self.localConfig.maxWindows, self.acqThr.CHANNELS)
    else:
      self.windows = self.acqThr.CHANNELS
    self.windows = max(self.windows, 0)
    if self.windows == 0:
      self.numRows = 0
      self.numColumns = 0
    else:
      self.numColumns = int(np.floor(np.sqrt(self.windows)))
      self.numRows = int(np.ceil(self.windows / self.numColumns))
    pass
  def plot(self):
    pass
  def saveFigures(self):
    pass

  def closeEvent(self, event): #overrides QMainWindow closeEvent
    #save local config
    self.localConfig.save(self.win)

    #save images
    # if self.saveImages:
    #     self.saveFigures()
    event.accept()
    self.quitAll()

  def quitAll(self):
    try:
      #stop everything
      self.win.timer.stop()
      self.acqThr.go = False
      self.acqThr.conn.close()
      self.acqThr.join()
      #self.bciThread.bci.Execute("Quit")
      #self.bciThread.bci.Execute("CLOSE CONNECTION")
      self.bciThread.join()
    except:
      pass
    finally:
      self.win.close()
      sys.exit()

  def setStyles(self):
    darkBackground = "#1D1D1F"
    darkEmphasis = "#0E0E0F"

    self.tableStyle = f"""
QHeaderView {{
  background-color: {darkBackground};
  color: #f0f0f0;    
}}
QHeaderView::section {{
  background-color: {darkEmphasis};
  color: #f0f0f0;    
}}
QTableView {{
    background-color: {darkBackground};
    border: 1px solid #32414B;
    color: #f0f0f0;
    gridline-color: #8faaff;
    outline : 0;
}}
QTableView::disabled {{
    background-color: {darkBackground};
    border: 1px solid #32414B;
    color: #656565;
    gridline-color: #656565;
    outline : 0;
}}
QTableView::item:hover {{
    background-color: "#26264f";
    color: #f0f0f0;
}}
QTableView::item:selected {{
    background-color: #1a1b1c;
    border: 2px solid #4969ff;
    color: #F0F0F0;
}}
QTableView::item:selected:disabled {{
    background-color: #1a1b1c;
    border: 2px solid #525251;
    color: #656565;
}}
QTableCornerButton::section {{
    background-color: #505050;
    color: #fff;
}}
"""
    self.buttonStyle = f"""QPushButton {{
                            color: white;
                            background-color: {darkEmphasis}; 
                            border-style: outset; 
                            border-width: 2px; 
                            border-radius: 10px; 
                            border-color: white; 
                            font: bold 16px; 
                            min-width: 5.5em; 
                            min-height: 1em;
                            padding: 5px;
                            margin: 15px;
                            }}
                            QPushButton:hover {{
                                background-color: grey;
                                margin: 5px;
                        }}"""
    self.plotStyle = f"""
                        * > QWidget, QMainWindow {{
                            background-color: {darkEmphasis};
                        }}
                        """
    self.titleDockStyle = f"""
                        Dock > QWidget {{
                            background-color: {darkBackground};
                        }}
                        """
    self.labelStyle = f"""
                        QLabel {{
                            font: bold 24px; 
                            color: white;
                            padding: 1px;
                        }}"""
    self.spinBoxStyle = f"""
                        SpinBox {{
                            background-color: white;
                            color: {darkBackground};
                            font: 16px;
                            border-width: 4px; 
                            border-radius: 2px; 
                            border-color: black;
                            min-width: 1em;
                            min-height: 1.2em;
                            margin-bottom: 10px;
                        }}"""
    self.spinLabelStyle = f"""
                        QLabel {{
                            font: 16px; 
                            color: white;
                            margin-top: 10px;
                        }}"""
    self.checkBoxStyle = f"""
                        QCheckBox {{
                            font: 16px;
                            color: white;
                        }}
                        """
    self.resetButtonStyle = f"""QPushButton {{
                            color: white;
                            background-color: {darkEmphasis}; 
                            border-style: outset; 
                            border-width: 1px; 
                            border-radius: 6px; 
                            border-color: white; 
                            font: 16px; 
                            min-width: 2em; 
                            min-height: 1em;
                            padding: 3px;
                            margin: 10px;
                            }}
                            QPushButton:hover {{
                                background-color: orange;
                                    margin: 5px;
                        }}"""
    #self.setStyleSheet("""""")
    pg.setConfigOption("background", pg.mkColor(darkBackground))
        
class TextOutput(QtWidgets.QTextEdit):
  def __init__(self):
    super().__init__()
    self.setReadOnly(True)
    #self.setTextBackgroundColor(pg.QtGui.QColor(29,29,31))
    #self.setTextColor(pg.QtGui.QColor(255, 255, 255))
    self.setFontPointSize(11)
    self.ensureCursorVisible()


#-----SAVING FIGURE------#
def saveFigure(myPrint, path, graphicsLayoutObj, suffix, ext='.png', antialias=True):
    newPath = _getPath(path)
    myPrint("Saving image at " + newPath + suffix + ext)
    if ext == '.svg':
        exporter = pg.exporters.SVGExporter(graphicsLayoutObj.ci)
    else:
        exporter = pg.exporters.ImageExporter(graphicsLayoutObj.ci)
        exporter.parameters()['antialias'] = antialias
    #double size to make image quality better
    #exporter.parameters()['width'] = exporter.parameters()['width'] * 2
    #exporter.parameters()['height'] = exporter.parameters()['height'] * 2
    exporter.export(_nonExistantFileName(newPath + suffix, ext) + ext)

########################
#### HELPER METHODS ####
########################
def _getPath(path):
    base = path
    #remove file type
    for i in range(len(path)-1, 0, -1):
        if base[i] == '.':
            base = base[:i]
            break
    #print(base)
    newPath = _nonExistantFileName(base, '.dat')
    #print(newPath)
    return newPath

#return file path with name, without an extension
def _nonExistantFileName(path, ext):
    #print(path)
    if path[-2:] == '00':
        path = path[:-1] + '1' #BCI2000 run numbers are never 00
    oldPath = path
    while os.path.isfile(path + ext):
        #print(path + ext + " is a file")
        #increment run number until we are not overwriting
        oldPath = path
        path = _changeName(path, -1)
    if ext == '.dat':
        path = oldPath #bc current dat file exists, but we want that number
    return path

def _changeName(path, index):
    #tries to match BCI2000 dat file name
    def getSuffix(path, index):
        if index+1 == 0:
            suf = ''
        else:
            suf = path[index+1:]
        return suf
    def replaceStr(path, index, v):
        newPath = path[:index] + str(v) + getSuffix(path, index)
        return newPath
    if path[index].isnumeric():
        if path[index] == '9':
            path = replaceStr(path, index, 0)
            path = _changeName(path, index - 1)
        else:
            path = replaceStr(path, index, int(path[index]) + 1)
    else:
        if index == -1:
            pre = path[::]
        else:
            pre = path[:index+1]
        path = pre + '1' + getSuffix(path, index) #add new digit
        #print(path)
    return path
# importing various libraries
import pyqtgraph as pg
from PyQt5 import QtWidgets
from pyqtgraph.Qt import QtCore
from pyqtgraph.dockarea import *
import pyqtgraph.exporters

import numpy as np
from scipy import stats
from .SharedVisualization import saveFigure, AbstractVisualization
#
# Uses PyQtGraph to visualize phase-amplitude coupling real-time
#
class PolarPlot(pg.PlotItem):
  def __init__(self, phis, data, title):
    super().__init__(title=title)
    self.phis = phis
    self.rawData = data

    axView = self.getViewBox()
    axView.setLimits(yMin=-1, yMax=1, xMin=-1, xMax=1)
    #axView.setRange(yRange=(-0.5, 0.5), xRange=(-0.5, 0.5))
    axView.setAspectLocked()
    # Add polar grid lines
    self.addLine(x=0, pen=0.2)
    self.addLine(y=0, pen=0.2)
    n = 5
    for r1 in range(1, n+1):
      r2 = r1/n*0.5
      circle = pg.QtWidgets.QGraphicsEllipseItem(-r2, -r2, r2 * 2, r2 * 2)
      circle.setPen(pg.mkPen(0.2))
      self.addItem(circle)

    #plot line
    self.line = pg.PlotDataItem(x=[0, 0], y=[0, 0])
    self.line.setZValue(9998)
    self.addItem(self.line)
    #plot dot
    self.data = pg.ScatterPlotItem(x=[0], y=[0], pen=None)
    self.data.setZValue(9999)
    self.addItem(self.data)

  def plot(self):
    zData = self.rawData #stats.zscore(self.rawData)
    a = np.multiply(zData, np.exp(1j*self.phis))
    self.zMod = np.sum(a)/(2*len(a))
    if not np.isnan(self.zMod):
      self.data.setData(x=[self.zMod.real], y=[self.zMod.imag])
      self.line.setData(x=[0, self.zMod.real], y=[0, self.zMod.imag])

  def setLink(self, pastPlot):
    if pastPlot != None:
      self.getViewBox().setXLink(pastPlot.getViewBox())
      self.getViewBox().setYLink(pastPlot.getViewBox())
  
  def refreshPlot(self, c):
    #aggregate dots
    newDot = pg.ScatterPlotItem(x=[self.zMod.real], y=[self.zMod.imag], symbol='x', brush=c, pen=None)
    self.addItem(newDot)

  def changeColor(self, c):
    self.data.setBrush(c)

class BinsPlot(pg.PlotItem):
  def __init__(self, phis, ax, data, title):
    super().__init__(axisItems=ax, title=title)
    self.phis = phis
    self.rawData = data
    
    #FIGURE 1
    axView = self.getViewBox()
    axView.setXRange(-np.pi - 0.3, np.pi + 0.3)
    axView.setMouseEnabled(x=False, y=True)
    axView.setYRange(-3, 3)

    #zData = stats.zscore(self.rawData)
    self.data = self.plot_centroid(self, self.phis, np.zeros(self.rawData.shape), clear=False, _callSync='off')

  def setYLink(self, pastPlot):
    if pastPlot != None:
      self.getViewBox().setYLink(pastPlot.getViewBox())

  #for making a stem plot
  def plot_centroid(self, plot, x, y, **kwargs):
    return plot.plot(x=np.repeat(x, 2), y=np.dstack((np.zeros(y.shape[0]), y)).flatten(), connect='pairs', **kwargs)

  # gets called every interval set by timer
  def plotBins(self):
    zData = self.rawData #stats.zscore(self.rawData)
    oldData = self.data.getData()
    newY = oldData[1]
    newY[1::2] = zData
    self.data.setData(x=oldData[0], y=newY)

  def changeColor(self, qc):
    #keep hue, make saturation and light the same
    c = pg.ColorMap(pos=(0.0, 0.5, 1.0), color=[qc, 'w', qc])
    self.data.setPen(width=3)
    p = c.getPen(span=(-2.0, 2.0))
    #p.setWidth(3)
    #print(p.color())
    self.data.setPen(p)

class PACWindow(AbstractVisualization):
  def __init__(self, win):
    super().__init__(win)
  def newTrial(self):
    for r in range(self.numRows):
      for c in range(self.numColumns):
        ch = r*self.numColumns+c
        if ch < self.acqThr.CHANNELS:
          q = self.polarGrid.getItem(r,c)
          q.refreshPlot(self.color)
        
    #reset bci2000 variables
    try:
      oldState = self.bciThread.bci.GetStateVariable("PAC_TrialNumber")
      newState = str(oldState.value+1)
      self.bciThread.bci.Execute("SET STATE PAC_TrialNumber " + newState)
    except:
      print("Could not connect to BCI2000")
  def saveFigures(self):
    #TODO: lines don't show up if you do svg b/c of the gradient pen
    saveFigure(self.bciThread.savePath, self.binsGrid, '_PAC_bins', ext='.png')
    saveFigure(self.bciThread.savePath, self.polarGrid, '_PAC_polar', antialias=False, ext='.svg')

  def setTrialNum(self, spin, i):
    self.color = pg.intColor(i, hues=self.maxTrials+1, sat=100, minHue=20, maxHue=360)
    for g in self.polarPlots:
      g.changeColor(self.color)
    for g in self.binPlots:
      g.changeColor(self.color)

  def setTrialTypeState(self, spin):
    try:
      self.bciThread.bci.Execute("SET STATE PAC_TrialType " + str(spin.value()))
    except:
      print("Could not connect to BCI2000")

  def getParameterList(self, paramName):
    ans = []
    self.bciThread.bci.Execute( "LIST PARAMETERS %s" %(paramName))
    a = self.bciThread.bci.Result
    b = a.split("\n")
    for f in b:
      h = f.split(" ")[3]
      ans.append(h) 
    return ans
  def configureTitle(self):
    #get parameters
    hP = self.getParameterList("HighPassCorner")
    lP = self.getParameterList("LowPassCorner")
    outType = self.getParameterList("OutputSignal")
    outType = [int(numeric_string) for numeric_string in outType] #convert to int

    angleI = outType[0] != 2
    magI = not angleI

    figTitle = "Phase-Amplitude coupling- Phase: %s - %s, Amplitude: %s - %s" %(hP[angleI], lP[angleI], hP[magI], lP[magI])
    return figTitle
  
  def plot(self):
    for p in self.polarPlots:
      p.plot()
    for b in self.binPlots:
      b.plotBins()
    #self.saveImages = True #only save images if there is data to be shown

  def setConfig(self):
    super().setConfig()
    elWidth = np.pi/self.acqThr.ELEMENTS
    phis = np.linspace(-np.pi+elWidth, np.pi-elWidth, self.acqThr.ELEMENTS)

    self.initialized = True

    pastBinsPlot = None
    pastPolarPlot = None
    self.polarGrid.clear()
    self.binsGrid.clear()
    self.polarPlots = list(range(self.acqThr.CHANNELS))
    self.binPlots = list(range(self.acqThr.CHANNELS))
    for r in range(self.numRows):
      for c in range(self.numColumns):
        ch = r*self.numColumns+c
        if ch < self.acqThr.CHANNELS:
          #print(self.acqThr.rawData[ch])
          self.polarPlots[ch] = PolarPlot(phis, data=self.acqThr.rawData[ch],
                           title=self.acqThr.chNames[ch])
          self.polarGrid.addItem(self.polarPlots[ch])
          self.polarGrid.getItem(r,c).setLink(pastPolarPlot) #connect axes of plots
          pastPolarPlot = self.polarGrid.getItem(r,c)

          ax = pg.AxisItem(orientation='bottom')
          ticks = [-np.pi, -np.pi/2, 0, np.pi/2, np.pi]
          strTicks = ["-π", "-π/2", "0", "π/2", "π"]
          ax.setTicks([[(v, str(s)) for (v, s) in zip(ticks, strTicks) ]])
          self.binPlots[ch] = BinsPlot(phis, {'bottom': ax}, data=self.acqThr.rawData[ch], 
                        title=self.acqThr.chNames[ch])
          self.binsGrid.addItem(self.binPlots[ch])
          self.binsGrid.getItem(r,c).setYLink(pastBinsPlot) #connect axes of plots
          pastBinsPlot = self.binsGrid.getItem(r,c)

      self.polarGrid.nextRow()
      self.binsGrid.nextRow()
    self.setTrialNum(None, 0)
    self.titleLab.setText(self.configureTitle())

  def initialize(self):
    self.win.setWindowTitle("Phase-Amplitude Coupling")
    self.maxTrials = 5
    self.cm = range(0, 360, round(360/(self.maxTrials+1)))

    button = QtWidgets.QPushButton('New Trial')
    button.clicked.connect(self.newTrial)
    saveButton = QtWidgets.QPushButton('Save Figures')
    saveButton.clicked.connect(self.saveFigures)
    spinWin = pg.SpinBox(int=True, compactHeight=True)
    spinWin.setMaximum(self.maxTrials)
    spinWin.setMinimum(0)
    spinWin.sigValueChanging.connect(self.setTrialNum)
    spinWin.sigValueChanged.connect(self.setTrialTypeState)
    spinLab = QtWidgets.QLabel("Trial type")

    polarTitle = "Polar Plot"
    self.polarGrid = pg.GraphicsLayoutWidget(title=polarTitle)
    self.binsGrid = pg.GraphicsLayoutWidget(title="Bins Plot")

    titleD = Dock("Trial Configuration", size=(1,1))
    self.titleLab = QtWidgets.QLabel(self.configureTitle())
    self.titleLab.setAlignment(QtCore.Qt.AlignHCenter)
    self.titleLab.setWordWrap(True)
    self.titleLab.setMinimumWidth(100)
    titleD.addWidget(self.titleLab, row=0, colspan=4)
    titleD.addWidget(button, row=1, col=0)
    titleD.addWidget(spinLab, row=1, col=1)
    titleD.addWidget(spinWin, row=1, col=2)
    titleD.addWidget(saveButton, row=1, col=3)
    titleD.hideTitleBar()
    
    self.polarD = Dock(polarTitle, widget=self.polarGrid)
    self.binsD = Dock("Bins Plot", widget=self.binsGrid)
    self.win.area.addDock(titleD, 'top')
    self.win.area.addDock(self.polarD, 'bottom')
    self.win.area.addDock(self.binsD, 'left', self.polarD)

    #style
    titleD.setStyleSheet(self.titleDockStyle)
    button.setStyleSheet(self.buttonStyle)
    saveButton.setStyleSheet(self.buttonStyle)
    self.titleLab.setStyleSheet(self.labelStyle)
    spinWin.setStyleSheet(self.spinBoxStyle)
    spinLab.setStyleSheet(self.spinLabelStyle)

    #set arrangement
    self.localConfig.initDockArea(self.win)


# driver code
if __name__ == '__main__':
  print("Choose a BCI2000 python file to run, or choose connect_batch.py to connect to your own batch file")
  input("Press enter to quit")
  
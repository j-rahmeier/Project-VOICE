# importing various libraries
import pyqtgraph as pg
from pyqtgraph.dockarea import *
from .SharedVisualization import saveFigure, AbstractVisualization

class DefaultPlot(pg.PlotItem):
    def __init__(self, data, title):
        super().__init__(title=title)
        self.rawData = data
        self.data = self.plot(data)

    def plotData(self):
        self.data.setData(self.rawData)

class DefaultWindow(AbstractVisualization):
    def __init__(self, win):
        super().__init__(win)

    def initialize(self):
        self.gridPlots = pg.GraphicsLayoutWidget()
        self.win.area.addDock(Dock("Demo", widget=self.gridPlots))
        #set arrangement
        self.localConfig.initDockArea(self.win)

    def setConfig(self):
        super().setConfig()
        self.gridPlots.clear()
        self.chPlot = list(range(self.acqThr.CHANNELS))

        for r in range(self.numRows):
            for c in range(self.numColumns):
                ch = r*self.numColumns+c
                if ch < self.acqThr.CHANNELS:
                    self.chPlot[ch] = DefaultPlot(data=self.acqThr.rawData[ch], 
                                                  title=self.acqThr.chNames[ch])
                    self.gridPlots.addItem(self.chPlot[ch])
            self.gridPlots.nextRow()

    def plot(self):
        for r in range(self.numRows):
            for c in range(self.numColumns):
                ch = r*self.numColumns+c
                if ch < self.acqThr.CHANNELS:
                    self.chPlot[ch].plotData()
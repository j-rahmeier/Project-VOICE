# importing various libraries
import numpy as np
from PyQt5 import QtWidgets
from pyqtgraph.Qt import QtCore
import pyqtgraph as pg
from pyqtgraph.dockarea import *
from .SharedVisualization import saveFigure, AbstractVisualization
from scripts.localConfig import CCEPSettings

backgroundColor = (14, 14, 14)
highlightColor = (60, 60, 40)

class CCEPWindow(AbstractVisualization):
    def __init__(self, win):
      super().__init__(win)
      self.localConfig = CCEPSettings(self.__class__.__name__)

    def plot(self):
        try:
          newVal = int(self.bciThread.bci.GetStateVariable("CCEPTriggered").value)
        except:
          newVal = 0

        if newVal != self.oldVal:
          #print("plotting")
          #get stim ch if we can
          try:
            chBits = int(self.bciThread.bci.GetStateVariable("StimulatingChannel").value)
          except:
            chBits = 0
          if chBits != 0:
            self.stimChs.clear()
            chBinary = str("{0:b}".format(chBits))
            for b in range(len(chBinary)): #32 bit state
              if chBinary[len(chBinary) - b - 1] == '1':
                #print(self.acqThr.chNames[b] + " at " + str(b))
                self.stimChs.append(self.acqThr.chNames[b]) #append ch name

          aocs = []
          for ch in self.chTable.values():
            ch.plotData() #compute data
            aocs.append(ch.auc)
          t = np.std(aocs) * self.localConfig.stds/10 #we scale by 10 cause slider can only do ints
          for ch in self.chTable.values():
            ch.totalChanged(ch.auc > t) #update table with new data
          #sort table with updated numbers
          self.table.sortItems(0, QtCore.Qt.DescendingOrder)
          for i in range(0, self.windows):
            chName = self.table.item(i, 0).text()
            self.chPlot[i].plotData(chName, self.chTable[chName])

          self.oldVal = newVal

    def changeBackgroundColor(self, row, emph):
      if row >= self.windows:
        return
      c = backgroundColor
      if emph:
        c = highlightColor
      self.chPlot[row].vb.setBackgroundColor(c)
      chName = self.table.item(row,0).text()
      self.chPlot[row].selected = emph
      self.chTable[chName].selected = emph
        
    def itemChanged(self):
        items = self.table.selectedItems()
        newRows = []
        for p in items:
            if p.row() not in self.selectedRows:
                self.changeBackgroundColor(p.row(),True)
            newRows.append(p.row())
        for oldS in self.selectedRows:
            if oldS not in newRows:
                self.changeBackgroundColor(oldS, False)
        self.selectedRows = newRows                
    
    def initialize(self):
        self.win.setWindowTitle("BCI2000 CCEPs")
        
        #FIGURE 1
        #self.pens = [pg.mkPen(x) for x in np.linspace((0, 0, 0), (256, 256, 256), 256)] #create all the pens we could ever need
        self.pens = [pg.mkPen(x) for x in np.linspace(0, 1, 256)] #create all the pens we could ever need
        self.gridPlots = pg.GraphicsLayoutWidget(title="CCEPS")
        #self.ccepTable = CCEPTable(title="CCEP Aggregate")
        self.gridNums = pg.GraphicsLayoutWidget(title="CCEP Aggregate")
        #self.table = CCEPTable()
        self.table = QtWidgets.QTableWidget()
        #self.table = pg.TableWidget(sortable=False)
        #self.table.setSortMode(0,'index')
        #self.table.setData([{"Name": "", "Count": ""}])

        #title dock widgets
        clearButton = QtWidgets.QPushButton('Clear Figures')
        clearButton.clicked.connect(self.clearFigures)
        saveButton = QtWidgets.QPushButton('Save Figures')
        saveButton.clicked.connect(self.saveFigures)

        #settings dock widgets
        #choose std dev threshold
        self.stdSpin = QtWidgets.QSlider(QtCore.Qt.Horizontal)
        #self.stdSpin = pg.SpinBox(int=True, compactHeight=True)
        self.stdSpin.setMaximum(10*10)
        self.stdSpin.setMinimum(0)
        self.stdSpin.setValue(self.localConfig.stds)
        self.stdSpin.valueChanged.connect(self.setStdDevState)
        self.stdSpin.setToolTip(str(self.localConfig.stds/10))
        stdLab = QtWidgets.QLabel("Threshold (STD)")
        stdLab.setToolTip("Standard Deviations for threshold, calculated from baseline")
        #specifies where to start visualizing the signal
        baseSpin = pg.SpinBox(int=True, compactHeight=True)
        baseSpin.setValue(self.localConfig.visBegin)
        baseSpin.sigValueChanged.connect(self.setBaselineBegin)
        baseSpin.setToolTip("First value of x axis")
        baseLab = QtWidgets.QLabel('x<sub>0</sub>')
        baseLab.setToolTip("First value of x axis")
        #max window number
        maxSpin = pg.SpinBox(int=True, compactHeight=True)
        maxSpin.setMaximum(100)
        maxSpin.setMinimum(0)
        maxSpin.setValue(self.localConfig.maxWindows)
        maxSpin.sigValueChanged.connect(self.setMaxWindows)
        maxSpin.setToolTip("Maximum number of windows to show")
        maxLab = QtWidgets.QLabel("Max Windows")
        maxLab.setToolTip("Maximum number of windows to show")
        #max plots
        holdSpin = pg.SpinBox(int=True, compactHeight=True)
        holdSpin.setMaximum(100)
        holdSpin.setMinimum(0)
        holdSpin.setValue(self.localConfig.maxPlots)
        holdSpin.sigValueChanged.connect(self.setMaxPlots)
        holdSpin.setToolTip("Maximum plots to hold (0 to hold all)")
        holdLab = QtWidgets.QLabel("Hold Plots")
        holdLab.setToolTip("Maximum plots to hold (0 to hold all)")
        #toggle to average every existing plot
        avgBut = QtWidgets.QCheckBox("Average CCEPs")
        avgBut.setChecked(self.localConfig.avgPlots)
        avgBut.stateChanged.connect(self.setAvgPlots)
        avgBut.setToolTip("New plot will be average of all CCEPs")
        #sorting toggle
        sortBut = QtWidgets.QCheckBox("Sort channels")
        sortBut.setChecked(self.localConfig.sortChs)
        sortBut.stateChanged.connect(self.setSortChs)
        sortBut.setToolTip("Sort by number of CCEPs detected")
        #automatically saving figures toggle
        saveFigBut = QtWidgets.QCheckBox("Save figures on refresh")
        saveFigBut.setChecked(self.localConfig.saveFigs)
        saveFigBut.stateChanged.connect(self.setSaveFigs)
        saveFigBut.setToolTip("Automatically save a .svg of the CCEPs when they are cleared/refreshed")
        #reset onset period
        self.onsetSpin = pg.SpinBox(int=True, compactHeight=True)
        self.onsetSpin.setMaximum(2**8 - 1)
        self.onsetSpin.setMinimum(1)
        #onsetSpin.setValue(self.localConfig.maxPlots)
        self.onsetSpin.sigValueChanged.connect(self.setOnset)
        self.onsetSpin.setToolTip("Frequency of triggers that are displayed (e.g. 2 = every other trigger)")
        onsetLab = QtWidgets.QLabel("Onset Period")
        onsetLab.setToolTip("Frequency of triggers that are displayed (e.g. 2 = every other trigger)")

        settingsD = Dock("Settings")
        settingsLab = QtWidgets.QLabel("Settings")
        settingsLab.setWordWrap(True)
        figuresLab = QtWidgets.QLabel("Figures")
        #settingsD.addWidget(figuresLab, row=0, col=0,)
        settingsD.addWidget(saveButton, row=1, col=0)

        settingsD.addWidget(settingsLab, row=3, col=0, colspan=2)
        settingsD.addWidget(stdLab, row=4, col=0)
        settingsD.addWidget(self.stdSpin, row=5, col=0)
        settingsD.addWidget(maxLab, row=6, col=0)
        settingsD.addWidget(maxSpin, row=7, col=0)
        settingsD.addWidget(baseLab, row=8, col=0)
        settingsD.addWidget(baseSpin, row=9, col=0)
        settingsD.addWidget(holdLab, row=10, col=0)
        settingsD.addWidget(holdSpin, row=11, col=0)
        settingsD.addWidget(avgBut, row=12, col=0)
        settingsD.addWidget(sortBut, row=13, col=0)
        settingsD.addWidget(saveFigBut, row=14, col=0)
        settingsD.addWidget(onsetLab, row=15, col=0)
        settingsD.addWidget(self.onsetSpin, row=16, col=0)
        settingsD.addWidget(clearButton, row=17, col=0)


        d1 = Dock("CCEPs", widget=self.gridPlots)
        d2 = Dock("Total CCEPs", widget=self.table)
        self.win.area.addDock(settingsD)
        self.win.area.addDock(d1, 'right')
        self.win.area.addDock(d2, 'left', d1)
        #resize according to config
        # if self.localConfig.dockConfig is not None:
        #     self.area.restoreState(self.localConfig.dockConfig)
        # else:
        #     self.resize(self.size[0], self.size[1])
        settingsD.setWindowTitle("Settings")

        #-----style-----
        #buttons
        clearButton.setStyleSheet(self.buttonStyle)
        saveButton.setStyleSheet(self.buttonStyle)
        #settings dock
        settingsD.setStyleSheet(self.titleDockStyle)
        settingsLab.setStyleSheet(self.labelStyle)
        figuresLab.setStyleSheet(self.labelStyle)
        avgBut.setStyleSheet(self.checkBoxStyle)
        sortBut.setStyleSheet(self.checkBoxStyle)
        saveFigBut.setStyleSheet(self.checkBoxStyle)
        #spinners and labels
        self.stdSpin.setStyleSheet(self.spinBoxStyle)
        stdLab.setStyleSheet(self.spinLabelStyle)
        baseSpin.setStyleSheet(self.spinBoxStyle)
        baseLab.setStyleSheet(self.spinLabelStyle)
        maxSpin.setStyleSheet(self.spinBoxStyle)
        maxLab.setStyleSheet(self.spinLabelStyle)
        holdSpin.setStyleSheet(self.spinBoxStyle)
        holdLab.setStyleSheet(self.spinLabelStyle)
        self.onsetSpin.setStyleSheet(self.spinBoxStyle)
        onsetLab.setStyleSheet(self.spinLabelStyle)

        self.table.setStyleSheet(self.tableStyle)
        self.gridPlots.setStyleSheet(self.plotStyle)
        #self.gridPlots.setStyleSheet(self.graphStyle)

        #set arrangement
        self.localConfig.initDockArea(self.win)
    
    def setConfig(self):
        super().setConfig()

        self.gridNums.clear()
        self.gridPlots.clear()

        self.chPlot = list(range(self.acqThr.CHANNELS))
        self.chOrder = list(range(self.acqThr.CHANNELS))
        #self.chPlot = {}
        self.chTable = {}
        self.regs = list(range(self.acqThr.CHANNELS))
        #init variables
        self.oldVal = 0
        self.baselineLength = self.getParameterValue("BaselineEpochLength") #for now, assume ms
        self.latStart = 0
        self.latStartSamples = self.localConfig.maskStart
        self.ccepLength = self.getParameterValue("CCEPEpochLength")
        self.sr = self.getParameterValue("SamplingRate")
        if self.sr < 30:
            self.sr = self.sr * 1000 #TODO: ugly hack if sampling rate is in kHz
        self.baseSamples = self.msToSamples(self.baselineLength)
        self.trigSamples = self.localConfig.maskEnd 
        self.trigLatLength = self.trigSamples * 1000.0 / self.sr
        
        self.x = np.linspace(-self.baselineLength, self.ccepLength, self.acqThr.ELEMENTS)

        #to visualize stimulating channels if we can
        self.stimChs = []
        try:
            onsetPeriod = int(self.bciThread.bci.GetParameter("OnsetPeriod"))
            self.onsetSpin.setValue(onsetPeriod)
        except:
            pass

        #go thru all channels for table
        self.tableArray = []
        count = 0
        for chName in self.acqThr.chNames:
            sub1 = self.gridNums.addLayout()
            sub1.addLabel("<b>%s"%(chName), size='20pt', bold=True)
            #print(self.tableArray)
            self.tableArray.append({
                "Name": chName,
                "Count": 0,
                "AUC": 0
            })
            sub1.nextRow()
            self.chTable[chName] = CCEPCalc(self, data=self.acqThr.rawData[count], ch=count, title=chName)
            count = count + 1

        #only initialize plots up to max number 
        for r in range(self.numRows):
          for c in range(self.numColumns):
            ch = r*self.numColumns+c
            #print(self.chPlot)
            if ch < self.windows:
              chName = self.acqThr.chNames[ch]
              self.chOrder[ch] = chName
              self.chPlot[ch] = CCEPPlot(self, title=chName, row=self.chTable[chName])
              self.gridPlots.addItem(self.chPlot[ch])
              if ch != 0:
                self.chPlot[ch].setLink(self.chPlot[ch-1])
          self.gridPlots.nextRow()
        
        if self.windows > 1:
          self.chPlot[0].friend = self.chPlot[self.windows-1] #give first plot a friend
        
        #table
        self.table.setRowCount(self.acqThr.CHANNELS)
        heads = ["Name", "Sig?", "AUC"]
        self.table.setColumnCount(len(heads))
        self.table.setHorizontalHeaderLabels(heads)
        for i, name in enumerate(self.acqThr.chNames):
          n = MyTableWidgetItem(name, self.acqThr.CHANNELS - i, self.acqThr.CHANNELS) #save order as rank
          s = MyTableWidgetItem(0)
          a = MyTableWidgetItem(0)
          self.table.setItem(i, 0, n)
          self.table.setItem(i, 1, s)
          self.table.setItem(i, 2, a)
        self.table.resizeColumnsToContents()
        #self.table.setData(self.tableArray)
        #self.table.setSortMode(0,'index')
        self.table.sortItems(0, QtCore.Qt.DescendingOrder)
        count = 0
        for chName in self.acqThr.chNames:
          self.chTable[chName].setTableItem(count)
          count = count + 1
        self.setSortChs(self.localConfig.sortChs)
        #make sure user can't change sorting
        for i in range(self.table.columnCount()):
          h = self.table.horizontalHeaderItem(i)
          h.setFlags(h.flags() & ~QtCore.Qt.ItemFlag.ItemIsEditable)
          self.table.setHorizontalHeaderItem(i, h)
        # print(self.table.horizontalHeaderItem(1).text())
        self.selectedRows = []
        #self.table.itemClicked.connect(self.tableItemClickedCallback)
        #self.table.itemSelectionChanged.connect(self.itemChanged)
        
      #save dock config
      #self.localConfig.dockConfig = self.area.saveState()

    def setStdDevState(self, value):
      self.localConfig.stds = value
      self.stdSpin.setToolTip(str(value/10))
    def setMaxWindows(self, spin):
      self.localConfig.maxWindows = spin.value()
    def setMaxPlots(self, spin):
      self.localConfig.maxPlots = spin.value()
    def setAvgPlots(self, state):
      self.localConfig.avgPlots = state
    def setSortChs(self, state):
      self.localConfig.sortChs = state
    def setSaveFigs(self, state):
      self.localConfig.saveFigs = state

    def setBaselineBegin(self, spin):
      self.localConfig.visBegin = spin.value()
      for i in range(0, self.windows):
        #xEnd = self.chPlot[i].getViewBox().viewRange()[0][1]
        self.chPlot[i].getViewBox().setXRange(spin.value(), self.ccepLength)
      # try:
      #     self.bciThread.bci.Execute("SET PARAMETER StdDevThreshold " + str(spin.value()))
      # except:
      #     print("Could not connect to BCI2000")
    def setOnset(self, spin):
        try:
            self.bciThread.bci.Execute("SET STATE ResetOnsetPeriod " + str(spin.value()))
        except:
            print("Could not connect to BCI2000")
    def resetOnsetPeriod(self):
        try:
            self.bciThread.bci.Execute("SET STATE ResetOnsetPeriod 1")
        except:
            print("Could not connect to BCI2000")
        
    def msToSamples(self, lengthMs):
        return int(lengthMs * self.sr/1000.0)
       
    def getParameterValue(self, pName):
        try:
            pString = self.bciThread.bci.GetParameter(pName)
        except:
            print("Parameter " + pName + " does not exist!!")
        finally:
            if pString == 'auto':
                return float(1)
            else:
                return float(''.join(c for c in pString if c.isdigit()))  

    def updateParameter(self, latStart, newLat):
      if newLat != self.trigLatLength:
        self.trigLatLength = newLat
        self.trigSamples = self.msToSamples(newLat)
        if round(newLat) >= 0:
          latP = round(newLat)
        else:
          latP = 0
        #self.bciThread.bci.SetParameter("TriggerLatencyLength", str(latP)+"ms")
      if latStart != self.latStart:
        self.latStart = latStart
        self.latStartSamples = self.msToSamples(latStart)
        self.localConfig.maskStart = self.latStartSamples
    
    def clearFigures(self):
      if self.localConfig.saveFigs:
        self.saveFigures()
      for i in range(0, self.windows):
        for child in self.chPlot[i].listDataItems():
          self.chPlot[i].removeItem(child)
      # for chName in self.acqThr.chNames:
      #     for child in self.chPlot[chName].listDataItems():
      #         self.chPlot[chName].removeItem(child)
      for t in self.chTable.values():
        t.totalCCEPs = 0
        t.totalChanged(0)
        t.database = []

    def saveFigures(self):
      saveFigure(self.win.print, self.bciThread.savePath, self.gridPlots, '_CCEPs', '.svg')

class MyTableWidgetItem(QtWidgets.QTableWidgetItem):
  def __init__(self, parent=None, rank=0, maxVal=0):
    QtWidgets.QTableWidgetItem.__init__(self, parent)
    self.rank = rank
    self.sig = 0
    self.max = maxVal
  #define less than (<) operator for table sorting
  def __lt__(self, b):
    #return (self.sig*self.max) < (b.sig*b.max)
    return (self.rank + self.sig*self.max) < (b.rank + b.sig*b.max)
    try:
      return int(self.text().split('-')[0]) < int(otherItem.text().split('-')[0])
    except ValueError:
      return self.text() < otherItem.text()
        

class CCEPPlot(pg.PlotItem):
  def __init__(self, parent, title, row):
    super().__init__(title=title)
    self.p = parent
    self.name = title
    self.link = row
    axView = self.getViewBox()
    axView.disableAutoRange()
    axView.setMouseEnabled(x=False, y=True)
    axView.setDefaultPadding(0)
    xLim = self.p.localConfig.visBegin
    yLim = self.p.ccepLength
    axView.setXRange(xLim, yLim, padding=0)
    axView.setYRange(-1000, 1000)

    self.latReg = pg.LinearRegionItem(values=(self.p.latStartSamples*1000.0/self.p.sr, self.p.trigLatLength), movable=True, brush=(9, 24, 80, 100), 
                                      pen=pg.mkPen(color=(9, 24, 80), width=1, style=QtCore.Qt.DotLine), bounds=[xLim, yLim])
    self.latReg.setZValue(1000) #make sure its in front of all plots
    #callbacks
    self.latReg.sigRegionChanged.connect(self.regionChanged)
    #axView.sigRangeChangedManually.connect(self.chgManual)
    #axView.sigYRangeChanged.connect(self.rngChanged)

    self.addItem(self.latReg)

    self.threshHigh = 0
    self.threshLow = 0
    self.latHigh = self.p.trigLatLength
    self.latLow = 0
    self.selected = False
    self.iChanged = False

    self.totalCCEPs = 0
    #self.backgroundC = (14, 14, 14)
    self.vb.setBackgroundColor(backgroundColor)

  def setLink(self, plt):
    self.friend = plt #each plot gets one friend they affect
    self.getViewBox().setYLink(self.friend.getViewBox())

  def regionChanged(self, reg):
    newReg = reg.getRegion()
    self.latHigh = newReg[1]
    self.latLow = newReg[0]
    if self.latHigh != self.friend.latHigh or self.latLow != self.friend.latLow:
      self.p.updateParameter(self.latLow, self.latHigh) 
      self.friend.latReg.setRegion(reg.getRegion())

  #plot: new name and link is only considered if we are dynamically sorting
  def plotData(self, name, link):
    #have we changed channels
    if self.name != name:
      self.setTitle(name)
      self.name = name
      self.link = link #update link
      
      #channel has changed, need to re-plot everything
      for child in self.listDataItems():
        self.removeItem(child)
      #now add
      expColors = [(255) * (1 - 2**(-x)) for x in np.linspace(0+1/(len(self.link.database)+1), 1-1/(len(self.link.database)+1), len(self.link.database))]
      #TODO: use multiDataPlot for more efficient plotting, figure out pens
      #self.multiDataPlot(x=self.p.x, y=self.link.database)
      for d, p in zip(self.link.database, expColors):
        self.plot(x=self.p.x, y=d, useCache=True, pen=self.p.pens[int(p)])
      #change background based on selected
      if self.selected and not self.link.selected:
        self.vb.setBackgroundColor(backgroundColor)
        self.selected = False
      elif not self.selected and self.link.selected:
        self.vb.setBackgroundColor(highlightColor)
        self.selected = True
    else:
      #update colors for old plots
      children = self.listDataItems()
      if self.link.averaged and np.size(children) > 0:
        #remove average plot
        self.removeItem(children[-1]) #average should be most recent one
        self.link.averaged = False
      if self.p.localConfig.maxPlots > 0:
        extra = len(children) - self.p.localConfig.maxPlots + 1  #account for 1 more we will plot
        for i in range(extra):
          self.removeItem(children[0])
          children.pop(0)
      expColors = [(255) * (1 - 2**(-x)) for x in np.linspace(0+1/(len(children)+1), 1-1/(len(children)+1), len(children))]
      for child, c in zip(children, expColors):
        child.setPen(self.p.pens[int(c)])

    if self.link.significant:
      p = pg.mkPen('y', width=1.5) #ccep!
    else:
      p = pg.mkPen('w')
    if self.name in self.p.stimChs:
      p = pg.mkPen('c')
    
    #plot new data
    if self.p.localConfig.avgPlots and len(self.link.database) > 1:
      #if averaged, plot most recent ccep before average
      p2 = 255*(1-2.5**(-1*(1-1/(len(self.link.database)+1))))
      self.plot(x=self.p.x, y=self.link.database[-1], useCache=True, pen=self.p.pens[int(p2)])
      self.link.averaged = True
    self.plot(x=self.p.x, y=self.link.data, useCache=True, pen=p)

class CCEPCalc():
  def __init__(self, parent, data, ch, title):
    self.p = parent
    self.rawData = data
    self.ch = ch

    self.threshHigh = 0
    self.threshLow = 0
    self.latLow = 0

    self.totalCCEPs = 0
    self.significant = False
    self.selected = False
    self.database = []
    self.averaged = False #if average plot exists or not
    self.auc = 0

  def getActiveData(self, data):
    p1 = data[:self.p.baseSamples+self.p.latStartSamples]
    p2 = data[self.p.baseSamples+self.p.trigSamples:]
    d = np.concatenate((p1, p2))
    #print(np.shape(d))
    return p2

  def setTableItem(self, ch):
    self.tableItem = self.p.table.item(ch,0)

  #t = boolean, if significant or not
  def totalChanged(self, t):
    self.tableItem.sig = t
    r = self.p.table.row(self.tableItem) #find new row we are at
    self.p.table.item(r,1).setData(QtCore.Qt.DisplayRole, int(t)) #change number at that row
    self.p.table.item(r,2).setData(QtCore.Qt.DisplayRole, int(self.auc))
    self.significant = t

  def plotData(self):        
    #new data, normalize amplitude with baseline data
    if self.p.baseSamples == 0:
      self.data = self.rawData
      #stdBase = 0
    else:
      avBase = np.mean(self.rawData[:self.p.baseSamples])
      #stdBase = np.std(self.rawData[:self.p.baseSamples], dtype=np.float64)
      self.data = np.subtract(self.rawData, avBase)

    #store data
    self.database.append(self.data)

    #possibly change to average, before we detect ccep
    if self.p.localConfig.avgPlots:
      #calculate average of plots
      self.data = np.mean(self.database, axis=0)
    
    #get area under the curve
    ccepData = self.getActiveData(self.data)
    normData = ccepData - np.mean(ccepData)
    self.auc = np.trapz(abs(normData))/1e3
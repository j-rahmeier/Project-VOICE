from scripts.main import main

#configure to customize your BCI2000 instance
def initBCI2000(bci):
    bci.WindowTitle = 'BCI2000: Phase-Amplitude coupling'
    
    bci.Execute('cd ${BCI2000LAUNCHDIR}')
    bci.StartupModules(('SignalGenerator', 
                        'PAC', 
                        'DummyApplication'))
    
    bci.SubjectID = 'pacDemo'
    bci.LoadParametersRemote('../parms/PAC/pac_pipeline.prm')
    bci.Execute('Set Parameter SamplingRate 1000Hz')
    bci.Execute('Set Parameter SampleBlockSize 50')

if __name__ == '__main__':
    main(initBCI2000)

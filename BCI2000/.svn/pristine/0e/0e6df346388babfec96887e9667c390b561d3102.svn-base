from scripts.main import main

#configure to customize your BCI2000 instance
def initBCI2000(bci):
    bci.WindowTitle = 'BCI2000: CCEP Filter'
    
    bci.Execute('cd ${BCI2000LAUNCHDIR}')
    bci.StartupModules(('SignalGenerator --local --LogKeyboard=1 --LogMouse=1', 
                        'CCEPFilter', 
                        'DummyApplication'))

    bci.SubjectID = 'ccepDemo'
    bci.LoadParametersRemote('../parms/CCEP/demo_mouse_settings.prm')
    bci.Execute('VISUALIZE WATCH MouseKeys')
    bci.Execute('VISUALIZE WATCH CCEPTriggered')
    #bci.Execute('SET PARAMETER OnsetExpression KeyDown>0')

if __name__ == '__main__':
    main(initBCI2000)

classdef SCIT_sourceCode < matlab.apps.AppBase

    % Properties that correspond to app components
    properties (Access = public)
        UIFigure                        matlab.ui.Figure
        GridLayout                      matlab.ui.container.GridLayout
        LeftPanel                       matlab.ui.container.Panel
        StimulationConfigurationLabel   matlab.ui.control.Label
        NeurostimulatorDropDown         matlab.ui.control.DropDown
        RecordingChannelsButtonGroup    matlab.ui.container.ButtonGroup
        LFPButton                       matlab.ui.control.RadioButton
        ECOGLFButton                    matlab.ui.control.RadioButton
        SEGButton                       matlab.ui.control.RadioButton
        SPKButton                       matlab.ui.control.RadioButton
        ECOGHFButton                    matlab.ui.control.RadioButton
        RAWButton                       matlab.ui.control.RadioButton
        OrderButtonGroup                matlab.ui.container.ButtonGroup
        AnodeFirstButton                matlab.ui.control.RadioButton
        CathodeFirstButton              matlab.ui.control.RadioButton
        NumberofpulsesSpinner           matlab.ui.control.Spinner
        PolarityButtonGroup             matlab.ui.container.ButtonGroup
        SteadyButton                    matlab.ui.control.RadioButton
        AlternatingButton               matlab.ui.control.RadioButton
        Phase1delaymsEditField          matlab.ui.control.NumericEditField
        Phase1ampuAEditField            matlab.ui.control.NumericEditField
        Phase2ampuAEditField            matlab.ui.control.NumericEditField
        Phase1durationusEditField       matlab.ui.control.NumericEditField
        InterphasedurationusEditField   matlab.ui.control.NumericEditField
        Phase2durationusEditField       matlab.ui.control.NumericEditField
        NumberofpulsesSpinnerLabel      matlab.ui.control.Label
        FrequencyHzEditField            matlab.ui.control.NumericEditField
        TrainDurationsEditField         matlab.ui.control.NumericEditField
        StimulationDurationsEditField   matlab.ui.control.NumericEditField
        TrainFrequencyHzEditField       matlab.ui.control.NumericEditField
        TriggerExpressionEditField      matlab.ui.control.EditField
        DropDown                        matlab.ui.control.DropDown
        Spinner                         matlab.ui.control.Spinner
        AbortExpressionLabel            matlab.ui.control.Label
        AbortExpressionEditField        matlab.ui.control.EditField
        AbortDropDown                   matlab.ui.control.DropDown
        AbortSpinner                    matlab.ui.control.Spinner
        TriggerExpressionLabel          matlab.ui.control.Label
        ReturnChannelDropDown           matlab.ui.control.DropDown
        ReturnChannelDropDownLabel      matlab.ui.control.Label
        StimulationDurationsEditFieldLabel  matlab.ui.control.Label
        Phase1delaymsEditFieldLabel     matlab.ui.control.Label
        InterphasedurationusEditFieldLabel  matlab.ui.control.Label
        NeurostimulatorDropDownLabel    matlab.ui.control.Label
        ChannelDropDown                 matlab.ui.control.DropDown
        ChannelDropDownLabel            matlab.ui.control.Label
        TrainFrequencyHzEditFieldLabel  matlab.ui.control.Label
        TrainDurationsEditFieldLabel    matlab.ui.control.Label
        FrequencyHzEditFieldLabel       matlab.ui.control.Label
        Phase2durationusEditFieldLabel  matlab.ui.control.Label
        Phase1durationusEditFieldLabel  matlab.ui.control.Label
        Phase2ampuAEditFieldLabel       matlab.ui.control.Label
        Phase1ampuAEditFieldLabel       matlab.ui.control.Label
        SaveConfigurationtoTablesButton  matlab.ui.control.Button
        CenterPanel                     matlab.ui.container.Panel
        ExperimentConditionsLabel       matlab.ui.control.Label
        ButtonGroup                     matlab.ui.container.ButtonGroup
        BUILDCONFIGButton               matlab.ui.control.ToggleButton
        TrainAxes                       matlab.ui.control.UIAxes
        ChannelAxes                     matlab.ui.control.UIAxes
        MultPulsesAxes                  matlab.ui.control.UIAxes
        SinglePulseAxes                 matlab.ui.control.UIAxes
        RightPanel                      matlab.ui.container.Panel
        ExporttoparameterfileButton     matlab.ui.control.Button
        PlayStimulationButton           matlab.ui.control.Button
        StimulationOnLamp               matlab.ui.control.Lamp
        StimulationOnLampLabel          matlab.ui.control.Label
        DeleteTriggerButton             matlab.ui.control.Button
        DeleteConfigurationButton       matlab.ui.control.Button
        DeleteTriggerDropDown           matlab.ui.control.DropDown
        DeleteChannelDropDown           matlab.ui.control.DropDown
        StimulationConfigurationsLabel  matlab.ui.control.Label
        StimulationTriggersLabel        matlab.ui.control.Label
        TableTriggers                   matlab.ui.control.Table
        TableConfigurations             matlab.ui.control.Table
        FileMenu                        matlab.ui.container.Menu
        LoadParameterFileMenu           matlab.ui.container.Menu
        PreferencesMenu                 matlab.ui.container.Menu
        AutoincrementTriggerExpressionMenu  matlab.ui.container.Menu
        IncreaseFontSizeMenu            matlab.ui.container.Menu
        DecreaseFontSizeMenu            matlab.ui.container.Menu
        ColorContextMenu                matlab.ui.container.ContextMenu
        ChangeColorsMenu                matlab.ui.container.Menu
        BackgroundMenu                  matlab.ui.container.Menu
        Plot1Menu                       matlab.ui.container.Menu
        Plot2Menu                       matlab.ui.container.Menu
        Plot3Menu                       matlab.ui.container.Menu
        RestoreDefaultColorsMenu        matlab.ui.container.Menu
    end

    % Properties that correspond to apps with auto-reflow
    properties (Access = private)
        onePanelWidth = 576;
        twoPanelWidth = 768;
    end

    
    properties (Access = private)
        %define list of stimulation device names
        % stimDeviceName = ["Cerestim", "NeuroOmega", "g.Estim"];
        cereStim = "CereStim";
        neuroOmega = "NeuroOmega";
        gEstim = "g.Estim";
        cortec = "Cortec BIC";
        %mapping for channel names to numbers
        %if you change the order of the buttons, you must change this as well
        chMapping = {"LFP",     10000:10009;
                     "ECOG LF", 10016:10047;
                     "SEG",     10128:10132;
                     "SPK",     10256:10260;
                     "ECOG HF", 10272:10303;
                     "RAW",     10384:10393};
        %for easy access to see if we should auto-update
        showTestConfig = false;
        lastStimName %to backtrack in case we don't want to change
        dy; % initialize spacing between elements
        defaultBackgroundColor;
        defaultColors; % Default colors to restore
        firstButtonText; % Description
        testColor = 0.2*ones(1,3); % Description
        sourceChChanged = true; % changed on channel callback, source vs return
    end
    
    methods (Access = private)
        
        function deleteConfig(app, index)
            labels = app.TableConfigurations.ColumnName;
            data = app.TableConfigurations.Data;
            opts = app.DeleteChannelDropDown.Items;
            %allNodes = app.ButtonGroup.Children;
            %account for "test values" node, which should be 1
            %reverses order from channel drop down list
            %delete(allNodes(length(allNodes) - index));
            %newNodes = app.ButtonGroup.Children;
            updateGraphs(app)
            labels(index) = [];
            data(:,index) = [];
            opts(index) = [];
            if isempty(labels) || ischar(labels)
                labels = [];
                data = [];
                opts = {};
                app.Spinner.Value = 1;
            else
                for i2=index:size(labels,1)
                    %shift to the left by 1
                    newName = "Configuration"+i2;
                    labels(i2) = {newName};
                    opts(i2) = cellstr(newName);
                    %if i2 ~= index
                        %newNodes(i2+1).Text = newName;
                    %end
                end
            end
            app.TableConfigurations.ColumnName = labels;
            app.TableConfigurations.Data = data;
            app.DeleteChannelDropDown.Items = opts;
            %column width
            w = app.TableConfigurations.ColumnWidth;
            if iscell(w)
                w(1) = [];
                if isempty(w)
                    w = 'auto';
                elseif w{1} == 'auto'
                    w = 'auto';
                end
            else
                w = 'auto';
            end
            if isempty(app.TableConfigurations.Data)
                w = 'auto';
            end
            app.TableConfigurations.ColumnWidth = w;
        end
        
        function deleteTrigger(app, i, configShift, expShift)
            labels = app.TableTriggers.ColumnName;
            data = app.TableTriggers.Data;
            opts = app.DeleteTriggerDropDown.Items;

            %delete from condition list if no other equiv expressions
            allNodes = app.ButtonGroup.Buttons;
            sameExp = false;
            for j=1:size(data,2)
                if i == j
                    continue
                end
                if strcmp(data(1,i), data(1,j))
                    sameExp = true;
                    break                    
                end
            end
            if ~sameExp
                for j=1:length(allNodes)
                    if strcmp(data(1,i),allNodes(j).Text)
                        delete(allNodes(j));
                        positionButtons(app);
                        break
                    end
                end
            end

            %account for "test values" node, which should be 1
            %reverses order from channel drop down list


            labels(i) = [];
            data(:,i) = [];
            opts(i) = [];
            if isempty(labels) || ischar(labels)
                labels = [];
                data = [];
                opts = {};
                app.Spinner.Value = 1;
            else
                for i2=i:size(labels,1)
                    %shift to the left by 1
                    newName = "Trigger"+i2;
                    labels(i2) = {newName};
                    opts(i2) = cellstr(newName);
                    if configShift
                        oldI = str2double(data(2,i2));
                        if oldI > i
                            data(2,i2) = {string(oldI-1)};
                        end
                    end
                end
            end
            app.TableTriggers.ColumnName = labels;
            app.TableTriggers.Data = data;
            app.DeleteTriggerDropDown.Items = opts;
            %width
            w = app.TableTriggers.ColumnWidth;
            if iscell(w)
                w(1) = [];
                if isempty(w)
                    w = 'auto';
                elseif w{1} == 'auto'
                    w = 'auto';
                end
            else
                w = 'auto';
            end
            if isempty(app.TableTriggers.Data)
                w = 'auto';
            end
            app.TableTriggers.ColumnWidth = w;
            
            updateGraphs(app);
        end
        
        function setNeuroOmegaChannels(app)
            if strcmp(app.NeurostimulatorDropDown.Value, app.neuroOmega)
                selectedButton = app.RecordingChannelsButtonGroup.SelectedObject;
                for i=1:size(app.chMapping, 1)
                    if app.chMapping{i,1} == selectedButton.Text
                        app.ChannelDropDown.Items = [string(app.chMapping{i,2}) ""];
                        app.ReturnChannelDropDown.Items = string([-1, app.chMapping{i,2}]);
                        break
                    end
                end
            end
        end

        function createConfig(app, newData)
            %add visualize row
            oldData = app.TableConfigurations.Data;
            oldColumnNames = app.TableConfigurations.ColumnName;
            newConfigName = string.empty;
            app.TableConfigurations.Data = [oldData newData];
            for i=1:size(newData,2)
                newConfigName(end+1) = "Configuration"+(int8(size(oldColumnNames,1))+i);
            end
            if ~isempty(oldColumnNames)
                app.TableConfigurations.ColumnName = [oldColumnNames; newConfigName'];
            else
                app.TableConfigurations.ColumnName = newConfigName;
            end
            %add new random color if we have used all available colors
            while size(app.SinglePulseAxes.ColorOrder,1) < size(app.TableConfigurations.ColumnName,1)
                c = rand(1,3);
                updateColors(app, c)
            end
            
            %set delete button
            oldOpts = app.DeleteChannelDropDown.Items;
            app.DeleteChannelDropDown.Items = [oldOpts, newConfigName];
            
            %update width
            for i=1:size(newData,2)
                addWidth(app, app.TableConfigurations, 40);
            end
        end
        
        function createTrigger(app, newData)
            oldData = app.TableTriggers.Data;
            %column name
            oldColumnNames = app.TableTriggers.ColumnName;
            newConfigName = string.empty;
            app.TableTriggers.Data = [oldData newData];
            for i=1:size(newData,2)
                newConfigName(end+1) = "Trigger"+(int8(size(oldColumnNames,1))+i);
            end
            if ~isempty(oldColumnNames)
                app.TableTriggers.ColumnName = [oldColumnNames; newConfigName'];
            else
                app.TableTriggers.ColumnName = newConfigName;
            end
            
            %set delete button
            oldOpts = app.DeleteTriggerDropDown.Items;
            app.DeleteTriggerDropDown.Items = [oldOpts, newConfigName];

            %add to experiment conditions list
            for i=1:size(newData,2)
                %make sure it isn't in there already
                newTrigger = true;
                for j=1:size(oldData,2)
                    if strcmp(newData(1,i), oldData(1,j))
                        newTrigger = false;
                    end
                end
                %check if the new data has already added the trig
                for j=1:i-1
                    if strcmp(newData(1,i), newData(1,j))
                        newTrigger = false;
                    end
                end
                %add new button
                if newTrigger
                    uitogglebutton(app.ButtonGroup, 'Text', newData(1,i));
                end
            end
            %switch selected button to visualize new trigger
            %just do first column if there are multiple new triggers
            selectThisButton(app,newData(1,1));

            %button positioning
            positionButtons(app);

            %update width
            for i=1:size(newData,2)
                addWidth(app, app.TableTriggers, 40);
            end

            %perhaps increment
            if app.AutoincrementTriggerExpressionMenu.Checked
                app.Spinner.Value = app.Spinner.Value + 1;
            end
        end

        function addWidth(app, tableObj, LIMIT)
            %change width of each entry
            newLength = length(tableObj.ColumnName);
            w = tableObj.ColumnWidth;
            if ~iscell(w)
                if newLength ~= 1
                    w = {'auto', 'auto'};
                end
            else
                for i=1:length(w)
                    newW = w(i);
                    iWidth = newW{1};
                    if iWidth ~= 'auto'
                        if iWidth > LIMIT
                            newWidth = iWidth - newLength*iWidth/8;
                            if (newWidth > LIMIT)
                                w(i) = {newWidth};
                            else
                                w(i) = {LIMIT};
                            end
                        end
                    else
                        sWidth= 110 - newLength*10;
                        if sWidth > LIMIT
                            w(i) = {sWidth};
                        else
                            w(i) = {LIMIT};
                        end
                    end
                end
                while length(w) < newLength
                    w{end+1} = 'auto';
                end
            end
            tableObj.ColumnWidth = w;
        end
        
        function [rNames, cNames, lineArr] = getLabels(app, lineArr)
            lineArr = lineArr(4:end);
            [rNames, lineArr] = getLabel(app, lineArr);
            [cNames, lineArr] = getLabel(app, lineArr);
        end

        function [names, lineArr] = getLabel(app, lineArr)
            %assume beginning is start of label
            n=lineArr{1};
            lineArr = lineArr(2:end);
            names= string.empty;
            if strcmp(n, '{')
                %row names
                while ~strcmp(lineArr(1), '}')
                    oldString = string(lineArr{1});
                    names(end+1) = replace(oldString,"%20"," ");
                    lineArr = lineArr(2:end);
                end
                lineArr = lineArr(2:end);
            else
                %number of rows
                names = 1:str2double(n);
            end
        end

        function ret = clearAllData(app)
            if ~isempty(app.TableConfigurations.Data)
                fig = uifigure;
                selection = uiconfirm(fig,'Continuing will delete the current values in the table. Proceed?', ...
                                            'Confirm Switch','Icon','warning');
                delete(fig)
                if selection ~= "OK"
                    app.NeurostimulatorDropDown.Value = app.lastStimName;
                    ret = false;
                    return
                end
            end

            %clear data from past stimulator
            app.TableConfigurations.Data = [];
            app.TableConfigurations.ColumnName = [];
            %app.TableConfigurations.RowName = [];
            app.TableConfigurations.ColumnWidth = 'auto';
            app.DeleteChannelDropDown.Items = {};
            app.TableTriggers.Data = [];
            app.TableTriggers.ColumnName = [];
            %app.TableTriggers.RowName = [];
            app.TableTriggers.ColumnWidth = 'auto';
            app.DeleteTriggerDropDown.Items = {};
            app.Spinner.Value = 1;
            %cla(app.ChannelAxes)
            %assume testing values is the first, which we keep
            delete(app.ButtonGroup.Children(1:end-1));
            updateGraphs(app);
            ret = true;
        end

        function initializeStimulator(app)
            value = app.NeurostimulatorDropDown.Value;
            %check if anything actually changed
            if strcmp(app.lastStimName, value)
                return
            end
            if ~clearAllData(app)
                return
            end

            cla(app.ChannelAxes)
            
            %dy = 33;
            %all elements
            elements = {
                app.NumberofpulsesSpinner, app.NumberofpulsesSpinnerLabel;                  %1
                app.Phase1delaymsEditField, app.Phase1delaymsEditFieldLabel;                %2
                app.Phase1ampuAEditField, app.Phase1ampuAEditFieldLabel;                    %3
                app.Phase2ampuAEditField, app.Phase2ampuAEditFieldLabel;                    %4
                app.Phase1durationusEditField, app.Phase1durationusEditFieldLabel;          %5
                app.InterphasedurationusEditField, app.InterphasedurationusEditFieldLabel;  %6
                app.Phase2durationusEditField, app.Phase2durationusEditFieldLabel;          %7
                app.FrequencyHzEditField, app.FrequencyHzEditFieldLabel;                    %8
                app.StimulationDurationsEditField, app.StimulationDurationsEditFieldLabel;  %9
                app.TrainDurationsEditField, app.TrainDurationsEditFieldLabel;              %10
                app.TrainFrequencyHzEditField, app.TrainFrequencyHzEditFieldLabel;          %11
                app.ChannelDropDown, app.ChannelDropDownLabel;                              %12
                app.OrderButtonGroup, '';                                                   %13
                app.ReturnChannelDropDown, app.ReturnChannelDropDownLabel;                  %14
                app.RecordingChannelsButtonGroup, '';                                       %15
                app.AbortDropDown, app.AbortExpressionEditField;
                    app.AbortExpressionLabel, app.AbortSpinner;                             %16 - 17
                app.PolarityButtonGroup, '';                                                %18
            };
            
            if strcmp(value, app.cereStim)
                visEls = [1 3:8 10:13];

                %change
                app.CathodeFirstButton.Text = "Cathode First";
                app.AnodeFirstButton.Text = "Anode First";
                app.OrderButtonGroup.Title = "Order";
                app.Phase1ampuAEditFieldLabel.Text = "Phase 1 amp (uA)";
                app.Phase1ampuAEditField.ValueDisplayFormat = "%11.4g uA";
                app.Phase1ampuAEditField.Limits = [100, 10000];
                app.Phase1ampuAEditField.Value = 200;
                app.Phase2ampuAEditFieldLabel.Text = "Phase 2 amp (uA)";
                app.Phase2ampuAEditField.ValueDisplayFormat = "%11.4g uA";
                app.Phase2ampuAEditField.Limits = [100, 10000];
                app.Phase2ampuAEditField.Value = 100;
                app.Phase1durationusEditFieldLabel.Text = "Phase 1 duration (us)";
                app.Phase1durationusEditField.ValueDisplayFormat = "%11.4g us";
                app.Phase1durationusEditField.Limits = [1, 65535];
                app.Phase1durationusEditField.Value = 250;
                app.InterphasedurationusEditFieldLabel.Text = "Interphase duration (us)";
                app.InterphasedurationusEditField.ValueDisplayFormat = "%11.4g us";
                app.InterphasedurationusEditField.Limits = [53, 65535];
                app.InterphasedurationusEditField.Value = 53;
                app.Phase2durationusEditFieldLabel.Text = "Phase 2 duration (us)";
                app.Phase2durationusEditField.ValueDisplayFormat = "%11.4g us";
                app.Phase2durationusEditField.Limits = [1, 65535];
                app.Phase2durationusEditField.Value = 500;
                app.FrequencyHzEditField.Limits = [4, 5000];
                app.FrequencyHzEditField.Value = 30;
                app.ChannelDropDown.Items = [string(1:96) ""];
                app.TrainDurationsEditFieldLabel.Text = "Train Duration (s)";
                app.TrainDurationsEditField.ValueDisplayFormat = "%11.4g s";
                app.ChannelAxes.Title.String = "Channels";

                if app.lastStimName == app.neuroOmega
                    shiftForNeuroOmega(app, -app.dy);
                elseif app.lastStimName == app.gEstim
                    shiftForGEstim(app, -app.dy);
                elseif strcmp(app.lastStimName, app.cortec)
                    shiftForCortec(app, app.dy);
                end

                %table
                app.TableConfigurations.RowName = ["Cathode First", "Number of pulses", "Phase 1 amp (uA)", "Phase 2 amp (uA)", ...
                                                    "Phase 1 duration (us)","Phase 2 duration (us)","Frequency (Hz)", ...
                                                    "Interphase duration (us)", "Train Duration (s)", "Train Frequency (Hz)"];
                app.TableTriggers.RowName = ["Expression", "Config ID", "Electrode(s)"];

                %figure
                xlabel(app.MultPulsesAxes, "Time (us)", 'Color', 'black')
                ylabel(app.MultPulsesAxes, "Current (uA)")
                xlabel(app.SinglePulseAxes, "Time (us)", 'Color', 'black')
                ylabel(app.SinglePulseAxes, "Current (uA)")

                %channel visualization
                n = 96;
                r = 4;
                c = 24;
                xlim(app.ChannelAxes, [0 c])
                ylim(app.ChannelAxes, [0 r])
                axis(app.ChannelAxes, 'auto y')
                axis(app.ChannelAxes,'off')
                axis(app.ChannelAxes,'tight')
                axis(app.ChannelAxes, 'equal')
                hold(app.ChannelAxes, "on")
                for i = 1:r
                    for j = c:-1:1
                        rectangle(app.ChannelAxes,'Position',[j-1 i-1 1 1],'Curvature',[1 1], ...
                            'FaceColor', app.CenterPanel.BackgroundColor, 'ButtonDownFcn', @app.channelClickedCallback)
                    end
                end
                %update channel visualization
                circI = getCircleIndexFromChannel(app, str2double(app.ChannelDropDown.Value));
                if ~isnan(circI)
                    app.ChannelAxes.Children(circI).FaceColor = app.testColor;
                end

            %cortec
            elseif strcmp(value, app.cortec)
                visEls = [1 3 5 6 8 10:12 14];

                %change parameter bounds
                app.Phase1ampuAEditFieldLabel.Text = "Pulse amplitude (uA)";
                app.Phase1ampuAEditField.ValueDisplayFormat = "%11.4g uA";
                app.Phase1ampuAEditField.Limits = [0 6120];
                app.Phase1durationusEditFieldLabel.Text = "Pulse duration (us)";
                app.Phase1durationusEditField.ValueDisplayFormat = "%11.4g us";
                app.Phase1durationusEditField.Limits = [10 2550];
                app.InterphasedurationusEditFieldLabel.Text = "Interphase duration (us)";
                app.InterphasedurationusEditField.ValueDisplayFormat = "%11.4g us";
                app.InterphasedurationusEditField.Limits = [10 2550];
                app.FrequencyHzEditField.Limits = [43.57 50000]; %limits calculated from dead zone 0 + dead zone 1 specs
                app.TrainDurationsEditFieldLabel.Text = "Number of Trains";
                app.TrainDurationsEditField.ValueDisplayFormat = "%11.4g";
                app.ChannelDropDown.Items = [string(1:32) ""];
                app.ReturnChannelDropDown.Items = [string(1:32) ""];
                app.ReturnChannelDropDown.Value = "2";
                app.ChannelAxes.Title.String = "Channels";

                %table
                app.TableConfigurations.RowName = ["Pulse amplitude (uA)", "Pulse duration (us)", ...
                                                    "Dead zone 0 (us)", "Dead zone 1 (us)", ...
                                                    "Number of Pulses", "Train Frequency (Hz)", "Number of Trains"];
                app.TableTriggers.RowName = ["Trigger", "PulseID", "Source ch", "Destination ch"];
                %figure
                xlabel(app.MultPulsesAxes, "Time (us)", 'Color', 'black')
                ylabel(app.MultPulsesAxes, "Current (uA)")
                xlabel(app.SinglePulseAxes, "Time (us)", 'Color', 'black')
                ylabel(app.SinglePulseAxes, "Current (uA)")

                %shift
                if app.lastStimName == app.neuroOmega
                    shiftForNeuroOmega(app, -app.dy);
                elseif app.lastStimName == app.gEstim
                    shiftForGEstim(app, -app.dy);
                end
                shiftForCortec(app, -app.dy);

                %channel visualization
                n = 32;
                r = 2;
                c = 16;
                xlim(app.ChannelAxes, [0 c])
                ylim(app.ChannelAxes, [0 r])
                axis(app.ChannelAxes,'tight')
                axis(app.ChannelAxes, 'equal')
                %axis(app.ChannelAxes, 'auto y')
                axis(app.ChannelAxes,'off')
                %axis(app.ChannelAxes, 'equal')
                hold(app.ChannelAxes, "on")
                for i = 1:r
                    for j = c:-1:1
                        rectangle(app.ChannelAxes,'Position',[j-1 i-1 1 1],'Curvature',[1 1], ...
                            'FaceColor', app.CenterPanel.BackgroundColor, 'ButtonDownFcn', @app.channelClickedCallback)
                    end
                end
                %color initial channel selection
                circI = getCircleIndexFromChannel(app, str2double(app.ChannelDropDown.Value));
                if ~isnan(circI)
                    app.ChannelAxes.Children(circI).FaceColor = app.testColor;
                end
                circI = getCircleIndexFromChannel(app, str2double(app.ReturnChannelDropDown.Value));
                if ~isnan(circI)
                    app.ChannelAxes.Children(circI).FaceColor = app.testColor;
                    app.ChannelAxes.Children(circI).Curvature = [0 0];
                end

            %gEstim
            elseif strcmp(value,app.gEstim)
                visEls = [1 3 5 6 8 10 11 13 16 17 18];

                %change parameter bounds
                app.CathodeFirstButton.Text = "Biphasic";
                app.AnodeFirstButton.Text = "Monophasic";
                app.OrderButtonGroup.Title = "Modularity";
                app.Phase1ampuAEditFieldLabel.Text = "Magnitude (mA)";
                app.Phase1ampuAEditField.ValueDisplayFormat = "%11.4g mA";
                app.Phase1ampuAEditField.Limits = [-0.2 15];
                app.Phase1durationusEditFieldLabel.Text = "Phase duration (ms)";
                app.Phase1durationusEditField.ValueDisplayFormat = "%11.4g ms";
                app.Phase1durationusEditField.Limits = [0.1 1.0];
                app.InterphasedurationusEditFieldLabel.Text = "Interphase duration (ms)";
                app.InterphasedurationusEditField.ValueDisplayFormat = "%11.4g ms";
                app.InterphasedurationusEditField.Limits = [0 inf]; %these are unknown
                app.InterphasedurationusEditField.Value = 0.1;
                app.FrequencyHzEditField.Limits = [2 100];
                app.FrequencyHzEditField.Value = 20;
                app.TrainDurationsEditFieldLabel.Text = "Number of Trains";
                app.TrainDurationsEditField.ValueDisplayFormat = "%11.4g";
                app.ChannelAxes.Title.String = "";
                
                %shift
                if strcmp(app.lastStimName, app.neuroOmega)
                    shiftForNeuroOmega(app, -app.dy);
                elseif strcmp(app.lastStimName, app.cortec)
                    shiftForCortec(app, app.dy);
                end
                shiftForGEstim(app, app.dy);
                
                %table
                app.TableConfigurations.RowName = ["Modularity", "Polarity", "Phase duration (ms)", ...
                                                    "Interphase duration (ms)", "Magnitude (mA)", "Frequency (Hz)", ...
                                                    "Number Of Pulses", "Train Frequency (Hz)", "Number Of Trains"];
                app.TableTriggers.RowName = ["Trigger Expression", "Config ID", "Abort Expression"];

                %figure
                xlabel(app.MultPulsesAxes, "Time (ms)", 'Color', 'black')
                ylabel(app.MultPulsesAxes, "Current (mA)")
                xlabel(app.SinglePulseAxes, "Time (ms)", 'Color', 'black')
                ylabel(app.SinglePulseAxes, "Current (mA)")
            %NeuroOmega
            elseif strcmp(value, app.neuroOmega)
                visEls = [2:11 12 14:15];
                
                %change
                app.Phase1ampuAEditFieldLabel.Text = "Phase 1 amp (mA)";
                app.Phase1ampuAEditField.ValueDisplayFormat = "%11.4g mA";
                app.Phase1ampuAEditField.Limits = [-15 15];
                app.Phase2ampuAEditFieldLabel.Text = "Phase 2 amp (mA)";
                app.Phase2ampuAEditField.ValueDisplayFormat = "%11.4g mA";
                app.Phase2ampuAEditField.Limits = [-15 15];
                app.Phase2ampuAEditField.Value = -15;
                app.Phase1durationusEditFieldLabel.Text = "Phase 1 duration (ms)";
                app.Phase1durationusEditField.ValueDisplayFormat = "%11.4g ms";
                app.Phase1durationusEditField.Limits = [0.02 0.5];
                app.InterphasedurationusEditFieldLabel.Text = "Phase 2 delay (ms)";
                app.InterphasedurationusEditField.ValueDisplayFormat = "%11.4g ms";
                app.InterphasedurationusEditField.Limits = [0 99];
                app.InterphasedurationusEditField.Value = 0.05;
                app.Phase2durationusEditFieldLabel.Text = "Phase 2 duration (ms)";
                app.Phase2durationusEditField.ValueDisplayFormat = "%11.4g ms";
                app.Phase2durationusEditField.Limits = [0.02 0.5];
                app.FrequencyHzEditField.Limits = [0 300];
                app.FrequencyHzEditField.Value = 3;
                app.TrainDurationsEditFieldLabel.Text = "Train Duration (s)";
                app.TrainDurationsEditField.ValueDisplayFormat = "%11.4g s";
                app.ChannelAxes.Title.String = "Channels";
                setNeuroOmegaChannels(app)

                if app.lastStimName == app.gEstim
                    shiftForGEstim(app, -app.dy);
                elseif strcmp(app.lastStimName, app.cortec)
                    shiftForCortec(app, app.dy);
                end
                shiftForNeuroOmega(app, app.dy);
                
                %table
                app.TableConfigurations.RowName = ["Phase 1 Delay (ms)", "Phase 1 Amp (mA)", "Phase 1 Width (ms)", ...
                                                    "Phase 2 Delay (ms)", "Phase 2 Amp (mA)", "Phase 2 Width (ms)", ...
                                                    "Frequency (Hz)","Stimulation Duration (s)","Train Duration (s)","Train Frequency (Hz)"];
                app.TableTriggers.RowName = ["Expression", "Config ID", "Channel(s)", "Return Channel"];
                
                %figure
                xlabel(app.MultPulsesAxes, "Time (ms)", 'Color', 'black')
                ylabel(app.MultPulsesAxes, "Current (mA)")
                xlabel(app.SinglePulseAxes, "Time (ms)", 'Color', 'black')
                ylabel(app.SinglePulseAxes, "Current (mA)")

                %channel visualization
                n = 96;
                r = size(app.chMapping,1);
                c = 32;
                xlim(app.ChannelAxes, [0 c])
                ylim(app.ChannelAxes, [0 r])
                axis(app.ChannelAxes,'tight')
                axis(app.ChannelAxes, 'equal')
                %axis(app.ChannelAxes, 'auto y')
                axis(app.ChannelAxes,'off')
                hold(app.ChannelAxes, "on")
                for i = 1:size(app.chMapping,1)
                    for j = size(app.chMapping{i,2},2):-1:1
                        rectangle(app.ChannelAxes,'Position',[j-1 i-1 1 1],'Curvature',[1 1], ...
                            'FaceColor', app.CenterPanel.BackgroundColor, 'ButtonDownFcn', @app.channelClickedCallback)
                    end
                end
                %update channel visualization
                circI = getCircleIndexFromChannel(app, str2double(app.ChannelDropDown.Value));
                if ~isnan(circI)
                    app.ChannelAxes.Children(circI).FaceColor = app.testColor;
                end
                circI = getCircleIndexFromChannel(app, str2double(app.ReturnChannelDropDown.Value));
                if ~isnan(circI)
                    app.ChannelAxes.Children(circI).FaceColor = app.testColor;
                    app.ChannelAxes.Children(circI).Curvature = [0 0];
                end

            else
                errordlg('This stimulator is not fully implemented. Please choose another stimulator','Stimulator Error');
            end
            %update visibility
            changeParameterVisibility(app, elements, visEls);


            %updateGraphs
            if app.showTestConfig
                updateGraphs(app)
            end
            app.lastStimName = app.NeurostimulatorDropDown.Value;


        end

        function updateGraphs(app)  
            nodeName = app.ButtonGroup.SelectedObject.Text;
            app.showTestConfig = false;
            if strcmp(nodeName, app.firstButtonText)
                app.showTestConfig = true;
            end
            count = 0;
            pulsesLegend = string.empty;
            trainPulses = string.empty;
            myLegend = string.empty;
            %use cell cause they will be different lengths
            d1 = {};
            d2 = {};
            d3 = {};
            lim = [];
            allLim = ones(2,2,3);
            allLim(:,1,:) = allLim(:,1,:)*Inf;
            allLim(:,2,:) = allLim(:,2,:)*-Inf;
            if app.showTestConfig
                count = 1;
                cIDs = 1;
                colors = app.testColor;
                %define vars
                p1Amp = app.Phase1ampuAEditField.Value;
                p2Amp = app.Phase2ampuAEditField.Value;
                p1Dur = app.Phase1durationusEditField.Value;
                p2Dur = app.Phase2durationusEditField.Value;
                freq = app.FrequencyHzEditField.Value;
                p2Del = app.InterphasedurationusEditField.Value;
                trainDur = app.TrainDurationsEditField.Value;
                trainFreq = app.TrainFrequencyHzEditField.Value;
                numPulses = app.NumberofpulsesSpinner.Value;
                %each device just needs to define "config"
                if strcmp(app.NeurostimulatorDropDown.Value, app.cereStim)
                    cathFirst = app.CathodeFirstButton.Value;
                    config = [cathFirst, numPulses, p1Amp, p2Amp, p1Dur, p2Dur, freq, p2Del]; 
                    config(end+1:end+2) = [trainDur, trainFreq];
                elseif strcmp(app.NeurostimulatorDropDown.Value, app.cortec)
                    deadzone1 = 1/freq*1e6;
                    config = [p1Amp, p1Dur, p2Del, deadzone1, numPulses];
                    config(end+1:end+2) = [trainFreq, trainDur];
                elseif strcmp(app.NeurostimulatorDropDown.Value, app.gEstim)
                    %trainDur = 1/trainFreq * trainDur; % we are really given number of pulses, not train duration
                    modul = app.CathodeFirstButton.Value; %cathode = biphasic, anode = monophasic
                    polarity = app.AlternatingButton.Value;
                    config = [modul, polarity, p1Dur, p2Del, p1Amp, freq, numPulses];
                    config(end+1:end+2) = [trainFreq, trainDur];
                elseif strcmp(app.NeurostimulatorDropDown.Value, app.neuroOmega)
                    p1Del = app.Phase1delaymsEditField.Value;
                    stimWidth = app.StimulationDurationsEditField.Value;
                    config = [p1Del, p1Amp, p1Dur, p2Del, p2Amp, p2Dur, freq, stimWidth];
                    config(end+1:end+2) = [trainDur, trainFreq];
                end
    
                [d1{count}, lim(:,:,1), d2{count}, lim(:,:,2), d3{count}, lim(:,:,3), numPulses, trainPulses(count)] = createGraphs(app, config);
                myLegend(end+1) = app.firstButtonText;
                pulsesLegend(end+1) = numPulses;
                allLim = [min(lim(:,1,:), allLim(:,1,:)), ...
                          max(lim(:,2,:), allLim(:,2,:))];
            
            else
                %configNames = app.TableConfigurations.ColumnName;
                triggerNames = app.TableTriggers.Data(1,:);
                configNames = app.TableTriggers.Data(1,:);
%                 if ~iscell(triggerNames)
%                     triggerNames = {triggerNames};
%                 end
                colors = app.SinglePulseAxes.ColorOrder;
                cData = app.TableConfigurations.Data;
                cIDs = [];
                for i=1:length(configNames)
                    if strcmp(nodeName, triggerNames(i))
                        configID = str2double(app.TableTriggers.Data(2,i));
                        count = count + 1;
                        cIDs(end+1) = configID;
                        [d1{count}, lim(:,:,1), d2{count}, lim(:,:,2), d3{count}, lim(:,:,3), numPulses, trainPulses(count)] = createGraphs(app, cData(:,configID));
                        myLegend(end+1) = "Config " + app.TableTriggers.Data(2,i);
                        pulsesLegend(end+1) = numPulses;
                        allLim = [min(lim(:,1,:), allLim(:,1,:)), ...
                                  max(lim(:,2,:), allLim(:,2,:))];
                    end
                end
            end
%             if count == 0
%                 cla(app.SinglePulseAxes)
%                 cla(app.MultPulsesAxes)
%                 cla(app.TrainAxes)
%                 legend(app.SinglePulseAxes, 'hide')
%                 legend(app.MultPulsesAxes, 'hide')
%                 legend(app.TrainAxes, 'hide')
%                 return
%             end
            trainLegend = string.empty;
            useFig1 = false;
            lineWidth = 2;
            for i=1:count
                plot(app.SinglePulseAxes, d1{i}(:,1), d1{i}(:,2), 'LineWidth',lineWidth, 'Color',colors(cIDs(i),:))
                hold(app.SinglePulseAxes, "on")
                plot(app.MultPulsesAxes, d2{i}(:,1), d2{i}(:,2), 'LineWidth',lineWidth, 'Color',colors(cIDs(i),:))
                hold(app.MultPulsesAxes, "on")
                if ~isempty(d3{i})
                    trainLegend(end+1) = myLegend(i)+ ": "+ trainPulses(i);
                    plot(app.TrainAxes, d3{i}(:,1), d3{i}(:,2), 'LineWidth',lineWidth, 'Color',colors(cIDs(i),:))
                    hold(app.TrainAxes, "on")
                else
                    useFig1 = true;
                end
            end
            %if stim trains have diff number
            if useFig1
                legend(app.SinglePulseAxes, myLegend)
                %use 2 legends cause the colors will be different
                legend(app.TrainAxes, trainLegend)
            else
                legend(app.SinglePulseAxes, 'hide')
                legend(app.TrainAxes, trainLegend)
            end
            %no trains
            if all(allLim(1,:,3) == 0)
                cla(app.TrainAxes)
                allLim(1,:,3) = [0 1];
                legend(app.TrainAxes, 'hide')
            end
            %if no stim amplitude
            if all(allLim(2,:,1) == 0)
                allLim(2,:,1) = [-1 1];
                allLim(2,:,2) = [-1 1];
            end
            legend(app.MultPulsesAxes, pulsesLegend)
            xlim(app.SinglePulseAxes, allLim(1,:,1))
            ylim(app.SinglePulseAxes, allLim(2,:,1))
            xlim(app.MultPulsesAxes, allLim(1,:,2))
            ylim(app.MultPulsesAxes, allLim(2,:,2))
            xlim(app.TrainAxes, allLim(1,:,3))
            ylim(app.TrainAxes, allLim(2,:,3))
            hold(app.SinglePulseAxes, "off")
            hold(app.MultPulsesAxes, "off")
            hold(app.TrainAxes, "off")
        end
        
        function [d1, lim1, d2, lim2, d3, lim3, numPulses, numTrainPulses] = createGraphs(app, config)
            polarity = false;
            %for each device define:
                %p1Amp, p1Dur, p1Del
                %p2Amp, p2Dur, p2Del
                %stimWidth, freq_us (in units the other time scales use)
                %trainDur, trainFreq
                %numPulses
            if strcmp(app.NeurostimulatorDropDown.Value,app.cereStim)
                cathFirst = config(1);
                numPulses = config(2);
                p1Amp = config(3);
                p2Amp = config(4);
                p1Dur = config(5);
                p2Dur = config(6);
                freq = config(7);
                p2Del = config(8); 
                trainDur = config(9);
                trainFreq = config(10);        
                
                %change based on cathFirst
                if (cathFirst == 1)
                    p1Amp = -p1Amp;
                else
                    p2Amp = -p2Amp;
                end
                p1Del = 0;
                stimWidth = (numPulses-1)/freq + (p1Del+p1Dur+p2Del+p2Dur)*1e-6; %s
                freq_us = freq/1e6;   
            elseif strcmp(app.NeurostimulatorDropDown.Value, app.cortec)
                p1Amp = config(1);
                p1Dur = config(2);
                p2Del = config(3);
                deadZone1 = config(4);
                numPulses = config(5);
                trainFreq = config(6);
                trainDur = config(7);

                p1Del = 0;
                p2Amp = -1/4*p1Amp;
                p2Dur = 4*p1Dur;
                trainDur = 1/trainFreq * trainDur; %translate from num of trains to actual duration
                freq_us = 1/(p2Del + deadZone1); %already in us
                stimWidth = (numPulses-1)/freq_us*1e-6 + (p1Del+p1Dur+p2Del+p2Dur)*1e-6; %s

            elseif strcmp(app.NeurostimulatorDropDown.Value,app.gEstim)
                modul = config(1);
                polarity = config(2);
                p1Dur = config(3);
                p2Del = config(4);
                p1Amp = config(5);
                freq = config(6);
                numPulses = config(7);
                trainFreq = config(8);
                trainDur = config(9);

                p2Amp = -1 *p1Amp;
                if modul
                    %biphasic
                    p2Dur = p1Dur;
                else
                    %monophasic, no second pulse
                    p2Dur = 0;
                end
                p1Del = 0;
                stimWidth = (numPulses-1)/freq + (p1Del+p1Dur+p2Del+p2Dur)*1e-3; %s
                trainDur = 1/trainFreq * trainDur; %+ stimWidth; %translate from num of trains to actual duration
                freq_us = freq/1e3; %ms units
            elseif strcmp(app.NeurostimulatorDropDown.Value,app.neuroOmega)
                p1Del = config(1);
                p1Amp = config(2);
                p1Dur = config(3);
                p2Del = config(4);
                p2Amp = config(5);
                p2Dur = config(6);
                freq = config(7);
                stimWidth = config(8);
                trainDur = config(9);
                trainFreq = config(10);

                numPulses = floor(freq*(stimWidth-(p1Del+p1Dur+p2Del+p2Dur)*1e-3))+1;
                freq_us = freq/1e3; %units in ms instead
            end
            
            %create piecewise fn
            xEnd = 2*p1Dur+p1Del+p2Del+p2Dur;
            xStart = 0-p1Dur;
            dX = (xEnd - xStart)/1000;
            x = xStart:dX:xEnd;
            y(x<=p1Del) = 0;
            y(p1Del < x & x < p1Dur+p1Del) = p1Amp;
            y(p1Del+p1Dur < x & x < p1Del+p1Dur+p2Del) = 0;
            y(p1Del+p1Dur+p2Del < x & x < p1Del+p1Dur+p2Del+p2Dur) = p2Amp;
            y(x > p1Del+p1Dur+p2Del+p2Dur) = 0;
            
            %plot 1
            minY = 11/8*min([p1Amp,p2Amp,0]); %do 11/8 just to make it look nice with padding on top and bottom
            maxY = 11/8*max([p1Amp,p2Amp,0]);
            if (minY == 0)
                minY = -maxY/8; %again, to make it look nice
            end
            if (maxY == 0)
                maxY = -minY/8;
            end
            lim1 = [0-p1Dur, 2*p1Dur+p1Del+p2Del+p2Dur;
                    minY, maxY];

            %create mult pulse plot
            %set bounds
            if numPulses > 1
                if strcmp(app.NeurostimulatorDropDown.Value,app.cereStim) ...
                || strcmp(app.NeurostimulatorDropDown.Value,app.cortec)
                    xEnd1 = 1/freq_us/2+(numPulses-1)/freq_us+p1Del+p1Dur+p2Del+p2Dur;
                else
                    xEnd1 = max(stimWidth*1e3,1/freq_us/2+(numPulses-1)/freq_us+p1Del+p1Dur+p2Del+p2Dur);
                end
                xStart = 0-1/freq_us/2;
            else
                %draw the bounds differently if there is only one pulse
                if strcmp(app.NeurostimulatorDropDown.Value,app.cereStim) ...
                || strcmp(app.NeurostimulatorDropDown.Value,app.cortec)
                    xEnd1 = 2*p1Dur+p1Del+p2Del+p2Dur;
                else
                    xEnd1 = max(stimWidth*1e3,2*p1Dur+p1Del+p2Del+p2Dur);
                end
                xStart = 0-p1Dur;
            end
            timeArr = [p1Del, p1Dur, p2Del, p2Dur];
            %find smallest dx we need to use
            dX = min(timeArr(timeArr>0))/2;
            %dX = (xEnd1 - xStart)/1000000;
            x1 = xStart:dX:xEnd1;  
            y1=zeros(size(x1));
            %loop thru each pulse
            for i=0:numPulses-1
                if polarity % would only be possibly set true with gEstim
                    alternate = 2*mod(i,2) - 1;
                    p1Amp = -abs(p1Amp) * alternate;
                    p2Amp = abs(p2Amp) * alternate;
                end
                lastSt = (i-1)/freq_us;
                st = i/freq_us;
                y1(p1Del+p1Dur+p2Del+p2Dur+lastSt < x1 & x1 <= p1Del + st) = 0;
                y1(st+p1Del < x1 & x1 <= st+p1Del+p1Dur) = p1Amp;
                y1(p1Del+p1Dur+st < x1 & x1 <= p1Del+p1Dur+p2Del+st) = 0;
                y1(p1Del+p1Dur+p2Del+st < x1 & x1 <= p1Del+p1Dur+p2Del+p2Dur+st) = p2Amp;
                y1(x1 > p1Del+p1Dur+p2Del+p2Dur+st) = 0;
            end
            if (p1Del+p1Dur+p2Del+p2Dur) > 1/freq_us
                xlabel(app.MultPulsesAxes, ["FREQ IS TOO FAST FOR THE"; "DESIRED STIMULATION DURATION"], ...
                    'FontWeight','bold', 'Color', 'r')
            else
                if strcmp(app.NeurostimulatorDropDown.Value,app.cereStim) ...
                || strcmp(app.NeurostimulatorDropDown.Value,app.cortec) %us
                    xlabel(app.MultPulsesAxes, "Time (us)", 'Color', 'black')
                else %neuroomega and gestim are ms
                    xlabel(app.MultPulsesAxes, "Time (ms)", 'Color', 'black')
                end
            end
            
            %plot 2
            lim2 = [xStart, xEnd1;
                    minY, maxY];

            %create train plot
            spacing = trainDur/8;
            xEnd2 = trainDur+spacing;
            xStart = 0-spacing;
            dX = (xEnd2 - xStart)/10000;
            x2 = xStart:dX:xEnd2;  

            numTrainPulses = trainFreq*trainDur;

            if (~isinteger(numTrainPulses))
                if (stimWidth <= trainDur - floor(numTrainPulses)/trainFreq)
                    %enough time for the full train
                    numTrainPulses = ceil(numTrainPulses);
                else
                    %not enough time, no partial trains
                    numTrainPulses = floor(numTrainPulses);
                end
            end
            y2(x2 > 0) = 0;
            for i=0:numTrainPulses-1
                lastSt = 1/trainFreq*(i-1);
                st = 1/trainFreq*i;
                y2(lastSt+stimWidth < x2 & x2 <= st) = 0;
                y2(st < x2 & x2 < st + stimWidth) = 1;
                y2(x2 > st + stimWidth) = 0;  
            end
            if (stimWidth >= 1/trainFreq)
                if strcmp(app.NeurostimulatorDropDown.Value,app.cereStim)
                    xlabel(app.TrainAxes, ["STIM BURSTS ARE TOO LONG"; "DECREASE THE NUM OF PULSES OR THE TRAIN FREQUENCY"], ...
                    'FontWeight','bold', 'Color', 'r')
                else
                    xlabel(app.TrainAxes, ["STIM BURSTS ARE TOO LONG"; "DECREASE THE STIM DURATION OR THE TRAIN FREQUENCY"], ...
                    'FontWeight','bold', 'Color', 'r')
                end
            else
                xlabel(app.TrainAxes, "Time (s)", 'Color', 'black')
            end
            if (trainDur~=0 && trainFreq==0)||(trainDur==0 && trainFreq~=0)
                xlabel(app.TrainAxes, "Train duration and train frequency must both be zero, or both non-zero!", ...
                    'FontWeight','bold', 'Color', 'r')
            end
            
            %plot 3
            lim3 = [xStart, xEnd2;
                    -0.4, 1.4];

            d1 = [x', y'];
            d2 = [x1', y1'];
            d3 = [x2', y2'];
        end
        
        function changeParameterVisibility(app, elements, visEls)
            count = 1;
            for i = visEls
                while i ~= count %count needs to catch up, and turn off things
                    changeVisibility(count, 'off');
                    count = count + 1;
                end
                changeVisibility(i, 'on');
                count = count + 1;
            end
            extraEls = length(elements) - visEls(end) - 1;
            if extraEls > 0
                startI = visEls(end) + 1;
                for i = startI: startI + extraEls
                    changeVisibility(i, 'off');
                end
            end

            function changeVisibility(i, toggleStr)
                for j = 1:2
                    if ~isempty(elements{i,j})
                        if elements{i, j}.Visible ~= toggleStr
                            elements{i, j}.Visible = toggleStr;
                        end
                    end
                end
            end
        end
        
        %cerestim set up is the default
        function shiftForNeuroOmega(app, shiftDown) 
            els = [app.TrainDurationsEditField, app.TrainDurationsEditFieldLabel, app.TrainFrequencyHzEditField, app.TrainFrequencyHzEditFieldLabel];
            shiftElements(app, els, shiftDown);
        end
        function shiftForGEstim(app, shiftDown) 
            doubleShift = [app.Phase1ampuAEditField, app.Phase1ampuAEditFieldLabel, app.NumberofpulsesSpinner, app.NumberofpulsesSpinnerLabel];
            els = [app.InterphasedurationusEditField, app.InterphasedurationusEditFieldLabel, app.Phase1durationusEditField, app.Phase1durationusEditFieldLabel, doubleShift];
            shiftElements(app, els, shiftDown);
            shiftElements(app, doubleShift, shiftDown);
        end
        function shiftForCortec(app, shiftDown)
            bigShift = [app.NumberofpulsesSpinner, app.NumberofpulsesSpinnerLabel, app.Phase1durationusEditField, app.Phase1durationusEditFieldLabel, app.FrequencyHzEditField, app.FrequencyHzEditFieldLabel];
            smallShift = [app.Phase1ampuAEditField, app.Phase1ampuAEditFieldLabel, app.InterphasedurationusEditField, app.InterphasedurationusEditFieldLabel, app.TrainDurationsEditField, app.TrainDurationsEditFieldLabel];
            shiftElements(app, bigShift, shiftDown*3/2);
            shiftElements(app, smallShift, shiftDown);
            shiftElements(app, [app.TrainFrequencyHzEditField, app.TrainFrequencyHzEditFieldLabel], shiftDown*1/2);
        end

        function shiftElements(app, els, y)
            for el = els
                el.Position(2) = el.Position(2) - y;
            end
        end
        
        function expres = createExpression(app, trig)
            if trig
                expresStart = convertCharsToStrings(app.TriggerExpressionEditField.Value);
                expresOp = convertCharsToStrings(app.DropDown.Value);
                expresEnd = int2str(app.Spinner.Value);
            else
                expresStart = convertCharsToStrings(app.AbortExpressionEditField.Value);
                expresOp = convertCharsToStrings(app.AbortDropDown.Value);
                expresEnd = int2str(app.AbortSpinner.Value);
            end
            expres = expresStart + expresOp + expresEnd;
        end
        
        function errorSameConfiguration(app)
            errordlg(['Same exact configuration is already in tables, will not add a duplicate.'] ...
                , 'Configuration Error');
        end
        
        function results = formatImportedParameter(app, p)
            results = p;
            results = removeStr(app, results, "%20"); %space operator
            [results, chang1] = removeStr(app, results, "mus");
            [results, chang2] = removeStr(app, results, "us");
            if any(isstrprop(results, 'alpha')) %any letters
                resChars = convertStringsToChars(results);
                results = convertCharsToStrings(resChars(~isstrprop(results, 'alpha'))); %take only numbers
            end
            if chang1 || chang2
                results = string(str2double(results)/1000); %convert to ms
            end
        end

        function [fullStr, changed] = removeStr(app, fullStr, subStr)
            if contains(fullStr, subStr)
                fullStr = erase(fullStr, subStr);
                changed = true;
            else
                changed = false;
            end
        end
        
        function positionButtons(app)
            %first resize button group window to match displayed center panel if needed
%             dispCenterPanelWidth = app.UIFigure.Position(3) - app.LeftPanel.Position(3) - app.RightPanel.Position(3);
%                 app.ButtonGroup.Position(3) = dispCenterPanelWidth - 1;
%             if dispCenterPanelWidth < app.ButtonGroup.Position(3)
%             end
            buttons = app.ButtonGroup.Buttons;
            padding = 2;
            for i=2:length(buttons) %test values is first position
                startingPos = buttons(i-1).Position(1) + buttons(i-1).Position(3);
                %adjust width based on number of characters
                buttonWidth = length(buttons(i).Text)*9; %estimate
                buttons(i).Position(3) = buttonWidth;
                %y coord
                if app.ButtonGroup.Position(3) < startingPos + buttonWidth*9/8 %2x spacing in case we are making window smaller
                    %move to next row
                    startingPos = buttons(1).Position(1) + buttons(1).Position(3);
                    buttons(i).Position(2) = buttons(i-1).Position(2) - buttons(i).Position(4);
                else
                    %keep on same row
                    buttons(i).Position(2) = buttons(i-1).Position(2)+buttons(i-1).Position(4)-buttons(i).Position(4);
                end
                buttons(i).Position(1) = startingPos + padding; %adjust left location
            end
        end
        
        function selectThisButton(app, triggerExp)
            buts = app.ButtonGroup.Buttons;
            %find where expression matches button
            for j=1:length(buts)
                if strcmp(buts(j).Text, triggerExp)
                    buts(j).Value = true;
                    break %only should be true once
                end
            end
            updateChannelVisualization(app);
            updateGraphs(app);
        end
        
        function changeColor(app, index)
            plots = [app.SinglePulseAxes, app.MultPulsesAxes, app.TrainAxes];
            if index == 0
                %background color
                c = uisetcolor(plots(1).Color, "Background color");
                for i=1:length(plots)
                    plots(i).Color = c;
                end
            else
                c = uisetcolor(plots(1).ColorOrder(index,:), sprintf("Plot %d color",index));
                for i=1:length(plots)
                    plots(i).ColorOrder(index,:) = c;
                end
            end
        end
        
        %resets all circles
        function updateChannelVisualization(app)
            chs = string.empty;
            cIDs = string.empty;
            retChs = string.empty;
            %gEstim doesn't have channels
            if ~strcmp(app.NeurostimulatorDropDown.Value,app.gEstim)
                if app.ButtonGroup.Buttons(1).Value
                    %test values
                    for c = 1:length(app.ChannelAxes.Children)
                        %reset all circles
                        if ~strcmp(app.ChannelAxes.Children(c).FaceColor,app.CenterPanel.BackgroundColor)
                            app.ChannelAxes.Children(c).FaceColor = app.CenterPanel.BackgroundColor;
                        end
                        if app.ChannelAxes.Children(c).Curvature ~= [1 1]
                            app.ChannelAxes.Children(c).Curvature = [1 1];
                        end
                    end
                    ch = convertCharsToStrings(app.ChannelDropDown.Value);
                    if ~strcmp(ch, "")
                        cI = getCircleIndexFromChannel(app,str2double(ch));
                        app.ChannelAxes.Children(cI).FaceColor = app.testColor;
                    else
                        cI = 0; %no meaning
                    end
                    %return channel if it exists
                    if app.ReturnChannelDropDown.Visible
                        retCh = convertCharsToStrings(app.ReturnChannelDropDown.Value);
                        if ~strcmp(retCh, "-1") && ~strcmp(retCh, "")
                            retI = getCircleIndexFromChannel(app,str2double(retCh));
                            %check if it is the same as source channel
                            if retI == cI
                                if app.sourceChChanged
                                if strcmp(app.NeurostimulatorDropDown.Value, app.neuroOmega)
                                    app.ReturnChannelDropDown.Value = "-1";
                                elseif strcmp(app.NeurostimulatorDropDown.Value, app.cortec)
                                    app.ReturnChannelDropDown.Value = "";
                                end
                                else
                                    app.ChannelDropDown.Value = "";
                                    app.ChannelAxes.Children(retI).Curvature = [0 0];
                                end
                            else
                                app.ChannelAxes.Children(retI).Curvature = [0 0];
                            end
                            app.ChannelAxes.Children(retI).FaceColor = app.testColor;
                        end
                    end
                    return
                else
                    d = app.TableTriggers.Data;
                    triggerExp = app.ButtonGroup.SelectedObject.Text;
                    for i = 1:size(d,2)
                        if strcmp(triggerExp, d(1,i))
                            %grab channels
                            chList = split(d(3,i),", ");
                            chLength = length(chs);
                            chs(chLength+1:chLength+length(chList),1) = chList;
                            %store config id
                            cIDs(chLength+1:chLength+length(chList),1) = d(2,i);
                            %store if is a return channel
                            retChs(chLength+1:chLength+length(chList),1) = 0;
                            if strcmp(app.NeurostimulatorDropDown.Value, app.neuroOmega)
                                %get return channel
                                r = d(4,i);
                                if ~strcmp(r,"") && ~strcmp(r, "-1")
                                    chs(end+1,1) = r;
                                    %store config id
                                    cIDs(end+1,1) = d(2,i);
                                    %store if is a return channel
                                    retChs(end+1,1) = 1;
                                end
                            elseif strcmp(app.NeurostimulatorDropDown.Value, app.cortec)
                                retChList = split(d(4,i), ", ");
                                chLength = length(chs);
                                chs(chLength+1:chLength+length(retChList),1) = retChList;
                                %store config id
                                cIDs(chLength+1:chLength+length(retChList),1) = d(2,i);
                                %store if is a return channel
                                retChs(chLength+1:chLength+length(retChList),1) = 1;

                            end
                        end
                    end
                end

                %visualize channels
                i = 1;
                [oChs, I] = sort(str2double(chs));
                oCfgs = str2double(cIDs(I));
                oRetChs = str2double(retChs(I));
                ordChs = [oChs, oCfgs, oRetChs];
                colors = app.SinglePulseAxes.ColorOrder;
                for ch = 1:length(app.ChannelAxes.Children)
                    chColor = app.ChannelAxes.Children(ch).FaceColor;
                    chName = getChannelFromCircleIndex(app,ch); %device specific function
                    if app.ChannelAxes.Children(ch).Curvature ~= [1 1]
                        app.ChannelAxes.Children(ch).Curvature = [1 1];
                    end
                    if i <= size(ordChs,1) && chName == ordChs(i,1)
                        if ~isequal(chColor, colors(ordChs(i,2),:))
                            chColor = colors(ordChs(i,2),:);
                        end
                        %change shape if return channel
                        if ordChs(i,3)
                            app.ChannelAxes.Children(ch).Curvature = [0 0];
                        end
                        %move counter to next channel
                        i = i + 1;
                    elseif ~strcmp(chColor,app.CenterPanel.BackgroundColor)
                        chColor = app.CenterPanel.BackgroundColor;
                    end
                    app.ChannelAxes.Children(ch).FaceColor = chColor;
                end
            end
        end
        
        %should only deal with affected channels to maintain the quick speed
        function channelClickedCallback(app,src,~)
            expName = app.ButtonGroup.SelectedObject.Text;
            %get channel number
            for i = 1:length(app.ChannelAxes.Children)
                if isequal(app.ChannelAxes.Children(i), src)
                    e = getChannelFromCircleIndex(app, i);
                    break
                end
            end

            %if testing values, just color it
            if expName == app.firstButtonText
                %un-color the last selected value
                oldCh = str2double(app.ChannelDropDown.Value);
                oldRetCh = str2double(app.ReturnChannelDropDown.Value);

                %set recording channels group (for neuroomega)
                if strcmp(app.NeurostimulatorDropDown.Value, app.neuroOmega)
                    recButI = src.Position(2) + 1;
                    if ~isequal(app.RecordingChannelsButtonGroup.SelectedObject, app.RecordingChannelsButtonGroup.Children(recButI))
                        %clear old return channel
                        if oldRetCh ~= -1
                            oldRet = getCircleIndexFromChannel(app, str2double(app.ReturnChannelDropDown.Value));
                            app.ChannelAxes.Children(oldRet).FaceColor = app.CenterPanel.BackgroundColor;
                            app.ChannelAxes.Children(oldRet).Curvature = [1 1];
                        end

                        app.RecordingChannelsButtonGroup.SelectedObject = app.RecordingChannelsButtonGroup.Children(recButI);
                        setNeuroOmegaChannels(app);
                    end
                end

                if  (oldRetCh == e) || (~isnan(oldCh) && oldCh == e)
                    if app.ReturnChannelDropDown.Visible
                        %unless we want to make it return channel
                        if src.Curvature == [1 1]
                            %make return channel
                            src.Curvature = [0 0];
                            app.ChannelDropDown.Value = ""; %clear channel

                            %clear old return
                            if ~isnan(oldRetCh) && oldRetCh ~= -1
                                oldRet = getCircleIndexFromChannel(app, str2double(app.ReturnChannelDropDown.Value));
                                app.ChannelAxes.Children(oldRet).FaceColor = app.CenterPanel.BackgroundColor;
                                app.ChannelAxes.Children(oldRet).Curvature = [1 1];
                            end
                            app.ReturnChannelDropDown.Value = string(e);
                        else
                            %already a return channel
                            src.FaceColor = app.CenterPanel.BackgroundColor;
                            src.Curvature = [1 1];
                            if strcmp(app.NeurostimulatorDropDown.Value, app.neuroOmega)
                                app.ReturnChannelDropDown.Value = "-1";
                            else
                                %cortec
                                oldRet = getCircleIndexFromChannel(app, str2double(app.ReturnChannelDropDown.Value));
                                app.ChannelAxes.Children(oldRet).FaceColor = app.CenterPanel.BackgroundColor;
                                app.ChannelAxes.Children(oldRet).Curvature = [1 1];
                                app.ReturnChannelDropDown.Value = "";
                            end
                        end
                    end
                else
                    %reset old one
                    if ~isnan(oldCh)
                        oldC = getCircleIndexFromChannel(app, oldCh);
                        app.ChannelAxes.Children(oldC).FaceColor = app.CenterPanel.BackgroundColor;
                    end
                    %color new one
                    src.FaceColor = app.testColor;
                    app.ChannelDropDown.Value = string(e);
                end
                return
            end

            %find config id from current color of channel
            configFound = false;
            for i = 1:length(app.SinglePulseAxes.ColorOrder)
                if isequal(app.SinglePulseAxes.ColorOrder(i,:), src.FaceColor)
                    n = i;
                    configFound = true;
                    break
                end
            end
            d = app.TableTriggers.Data;
            expCols = find(strcmp(d(1,:),expName));
            returnCh = false;
            
            %available config ids for this expression
            cIDs = [];
            for i = 1:size(d,2)
                if strcmp(expName, d(1,i))
                    cIDs(end+1) = str2double(d(2,i));
                end
            end
            cIDs = sort(cIDs);
            colCounter = 1;

            if configFound
                %find where all expressions are true
                nextIDExists = false;
                chFound = 0;
                %array locs of same configs
                repConfigs = find(cIDs==n);
                for i = 1:length(expCols)
                    cID = str2double(d(2,expCols(i)));
                    if cID == n && chFound==0
                        %return ch or normal ch?
                        if src.Curvature == [1 1]
                            %normal ch
                            %channel might be in there
                            chList = split(d(3,expCols(i)),", ");
                            for j = 1:length(chList)
                                if str2double(chList(j)) == e
                                    chList(j) = []; %remove from list
                                    app.TableTriggers.Data(3,expCols(i)) = formatChList(app,chList);
                                    chFound = i;
                                    break
                                end
                            end
                        else
                            %return ch
                            returnCh = true;
                            if strcmp(app.NeurostimulatorDropDown.Value, app.neuroOmega)
                                if str2double(d(4,expCols(i))) == e
                                    %back to default
                                    app.TableTriggers.Data(4,expCols(i)) = "-1";
                                    chFound = i;
                                end
                            else
                                %cortec
                                retChList = split(d(4,expCols(i)),", ");
                                for j = 1:length(retChList)
                                    if str2double(retChList(j)) == e
                                        retChList(j) = []; %remove from list
                                        app.TableTriggers.Data(4,expCols(i)) = formatChList(app,retChList);
                                        chFound = i;
                                        break
                                    end
                                end
                            end
                            
                        end
                    elseif configFound && cID == n && colCounter < length(repConfigs)
                        %for multiple columns with same config (neuroomega)
                        colCounter = colCounter + 1;
                        nextIDExists = true;
                        newConfigID = cID;
                    elseif cID == n+1 && ~nextIDExists
                        %color channel of next id
                        nextIDExists = true;
                        newConfigID = cID;
                    end
                end
                if ~nextIDExists
                    if strcmp(app.NeurostimulatorDropDown.Value, app.neuroOmega) ...
                    || strcmp(app.NeurostimulatorDropDown.Value, app.cortec)
                        %toggle return channel
                        if returnCh
                            src.Curvature = [1 1];
                            src.FaceColor = app.CenterPanel.BackgroundColor;
                            return
                        else
                            src.Curvature = [0 0];
                            colCounter = 1;
                            newConfigID = cIDs(colCounter);
                        end
                        returnCh = ~returnCh;
                    else
                        src.FaceColor = app.CenterPanel.BackgroundColor;
                        return
                    end
                end
            else
                %want to add channel to table
                newConfigID = cIDs(1);
            end
            tempCounter = 1;
            for i = 1:length(expCols)
                if newConfigID == str2double(d(2,expCols(i)))
                    if tempCounter < colCounter
                        %skip this column
                        tempCounter = tempCounter + 1;
                        continue;
                    end
                    if returnCh
                        %reset old return channel (only for neuro omega)
                        if strcmp(app.NeurostimulatorDropDown.Value, app.neuroOmega)
                            oldChNum = str2double(d(4,expCols(i)));
                            if ~isnan(oldChNum) && oldChNum ~= -1
                                oldCircI = getCircleIndexFromChannel(app,str2double(d(4,expCols(i))));
                                app.ChannelAxes.Children(oldCircI).FaceColor = app.CenterPanel.BackgroundColor;
                                app.ChannelAxes.Children(oldCircI).Curvature = [1 1];
                            end
                            app.TableTriggers.Data(4,expCols(i)) = string(e);
                        else
                            oldChList = split(d(4,expCols(i)), ", ");
                            app.TableTriggers.Data(4,expCols(i)) = formatChList(app,[oldChList; string(e)]);
                        end
                        %color new return channel
                        src.FaceColor = app.SinglePulseAxes.ColorOrder(newConfigID,:);
                    else
                        %add to column
                        oldChList = split(d(3,expCols(i)), ", ");
                        chList = [oldChList; string(e)];
                        ch = expCols(i);
                        src.FaceColor = app.SinglePulseAxes.ColorOrder(newConfigID,:);
                        app.TableTriggers.Data(3,ch) = formatChList(app,chList);
                    end
                    break
                end
            end
        end
        
        function newChList = formatChList(app, chList)
            if isempty(chList)
                newChList = "";
                return
            end
            orderedChannels = string(sort(str2double(chList)));
            newChList = orderedChannels(1);
            for i=2:length(orderedChannels)
                if ~ismissing(orderedChannels(i))
                    newChList = newChList + ", " + orderedChannels(i);
                end
            end
        end
        
        function circI = getCircleIndexFromChannel(app, ch)
            if ~strcmp(app.NeurostimulatorDropDown.Value, app.neuroOmega)
                circI = ch;
            else
                %make sure it is valid channel
                if isnan(ch) || ch == -1
                    circI = nan;
                    return
                end
                for i = 1:size(app.chMapping, 1) %6
                    startVal = app.chMapping{i,2}(1);
                    n = ch - startVal;
                    if n >= 0 && n <= 32 %max length of channel group
                        %make sure it doesn't belong in next group
                        if i + 1 <= size(app.chMapping, 1)
                            if ch - app.chMapping{i+1,2}(1) < 0
                                %found group
                                g = i;
                                break
                            end
                        else
                            %found group
                            g = i;
                            break
                        end
                    end
                end
                x = n;
                y = g - 1;
                for i = 1:length(app.ChannelAxes.Children)
                    if app.ChannelAxes.Children(i).Position == [x y 1 1]
                        circI = i;
                        break
                    end
                end
            end
            
        end
        
        function ch = getChannelFromCircleIndex(app, cI)
            if ~strcmp(app.NeurostimulatorDropDown.Value, app.neuroOmega)
                ch = cI;
            else
                circ = app.ChannelAxes.Children(cI);
                x = circ.Position(1);
                y = circ.Position(2);
                g = app.chMapping{y+1, 2}(1);
                ch = g + x;
            end
        end
        
        function updateColors(app, color)
            plots = [app.SinglePulseAxes, app.MultPulsesAxes, app.TrainAxes];
            for i=1:length(plots)
                plots(i).ColorOrder = [plots(i).ColorOrder; color];
            end
        end
        
        function changeFontSize(app, el, diff)
            if isprop(el, "Children")
                %get all children
                elChildren = el.Children;
                for i = 1:length(elChildren)
                    changeFontSize(app, elChildren(i), diff); %recursive swag
                end
            elseif isprop(el, "FontSize")
                el.FontSize = el.FontSize + diff;
            end
        end
    end
    

    % Callbacks that handle component events
    methods (Access = private)

        % Code that executes after component creation
        function startupFcn(app)
            app.lastStimName = 'none';
            app.dy = abs(app.TrainFrequencyHzEditField.Position(2) - app.TrainDurationsEditField.Position(2));
            app.defaultBackgroundColor = app.SinglePulseAxes.Color;
            app.defaultColors = app.SinglePulseAxes.ColorOrder;

            app.SinglePulseAxes.Toolbar.Visible = 'off';
            disableDefaultInteractivity(app.SinglePulseAxes)
            app.MultPulsesAxes.Toolbar.Visible = 'off';
            disableDefaultInteractivity(app.MultPulsesAxes)
            app.TrainAxes.Toolbar.Visible = 'off';
            disableDefaultInteractivity(app.TrainAxes)
            app.ChannelAxes.Toolbar.Visible = 'off';

            app.firstButtonText = string(app.BUILDCONFIGButton.Text);

            initializeStimulator(app);
            updateGraphs(app)
        end

        % Button pushed function: SaveConfigurationtoTablesButton
        function SaveConfigurationtoTablesButtonPushed(app, event)
            %define vars
            
            p1Amp = app.Phase1ampuAEditField.Value;
            p2Amp = app.Phase2ampuAEditField.Value;
            p1Dur = app.Phase1durationusEditField.Value;
            p2Dur = app.Phase2durationusEditField.Value;
            freq = app.FrequencyHzEditField.Value/1e6;
            interDur = app.InterphasedurationusEditField.Value;
            trainDur = app.TrainDurationsEditField.Value;
            trainFreq = app.TrainFrequencyHzEditField.Value;
            expres = createExpression(app, true);
            ch = app.ChannelDropDown.Value;
            retCh = app.ReturnChannelDropDown.Value;
            numPulses = app.NumberofpulsesSpinner.Value;

            %stimulation configuration
            oldConfigData = app.TableConfigurations.Data;
            sizeConfigData = size(oldConfigData);
            %stimulation triggers
            oldTrigData = app.TableTriggers.Data;
            sizeOldTrigData = size(oldTrigData);
            
            %must define newConfigData and newTrigData for each device
            %current required syntax for each device:
            % Trigger table rows:
            % 1 = trigger expression
            % 2 = configuration ID
            % 3 = source channel (except gEstim currently doesn't have channels)
            if strcmp(app.NeurostimulatorDropDown.Value,app.cereStim)
                %cerestim
                cathFirst = app.CathodeFirstButton.Value;
                newConfigData = [cathFirst; numPulses; p1Amp; p2Amp;
                                p1Dur; p2Dur; freq*1e6; interDur; 
                                trainDur; trainFreq];
                newTrigData = [expres; -1; ch];
            elseif strcmp(app.NeurostimulatorDropDown.Value, app.cortec)
                %cortec
                deadzone1 = 1/freq;
                newConfigData = [p1Amp; p1Dur; interDur; deadzone1; numPulses; trainFreq; trainDur];
                newTrigData = [expres; -1; ch; retCh];
            elseif strcmp(app.NeurostimulatorDropDown.Value,app.gEstim)
                %trainDur = 1/trainFreq * trainDur; % we are really given number of pulses, not train duration
                abortExp = createExpression(app, false);
                modul = app.CathodeFirstButton.Value; %cathode = biphasic
                polarity = app.AlternatingButton.Value;
                newConfigData = [modul; polarity; p1Dur; interDur; p1Amp; freq*1e6; numPulses; trainFreq; trainDur];
                newTrigData = [expres; -1; abortExp];
            elseif strcmp(app.NeurostimulatorDropDown.Value,app.neuroOmega)
                %neuro omega
                p1Delay = app.Phase1delaymsEditField.Value;
                stimDur = app.StimulationDurationsEditField.Value;
                newConfigData = [p1Delay; p1Amp; p1Dur; 
                                interDur; p2Amp; p2Dur; freq*1e6;
                                stimDur; trainDur; trainFreq];
                newTrigData = [expres; -1; ch; retCh];
            end

            %check channels are valid
            if strcmp(ch, "") ...
            || ((strcmp(app.NeurostimulatorDropDown.Value,app.neuroOmega) || strcmp(app.NeurostimulatorDropDown.Value,app.cortec)) && strcmp(retCh, ""))
                errordlg(['Specify a channel for stimulation.'],'Channel Error');
                return
            end

            %check if configuration already exists
            diffConfig = true;
            colIndex = 0;
            for j=1:sizeConfigData(2)
                %go across columns first
                diffConfig = false;
                for i=1:sizeConfigData(1)
                    if (oldConfigData(i,j) ~= newConfigData(i))
                        diffConfig = true;
                        break
                    end
                end
                if (diffConfig == false)
                    colIndex = j;
                    break
                end
            end

            %check to make sure channels aren't being triggered at same time
            for i=1:sizeOldTrigData(2)
                if strcmp(expres, oldTrigData(1,i))
                    %same expression
                    if strcmp(app.NeurostimulatorDropDown.Value,app.gEstim)
                        %can't have same expression
                        errordlg(['Choose a different trigger expression. g.Estim cannot excute multiple configurations' ...
                            ' at the same time.'],'Trigger expression Error');
                                return
                    else
                        chs = oldTrigData(3,i);
                        chList = split(chs,", ");
                        if strcmp(app.NeurostimulatorDropDown.Value,app.neuroOmega) ...
                        || strcmp(app.NeurostimulatorDropDown.Value,app.cortec)
                            %also need to look at return channels
                            retChs = oldTrigData(4,i);
                            chList = [chList; retChs];
                        end
                        for j =1:length(chList)
                            if strcmp(chList(j), ch) || strcmp(chList(j), retCh)
                                errordlg(['The specified channel is already' ...
                                    ' being stimulated with the same expression. ' ...
                                    'Choose a different channel or trigger expression'],'Configuration Error');
                                return
                            end
                        end  
                    end
                end
            end

            %if config is new, adding trigger is simple
            if diffConfig
                newTrigData(2) = sizeConfigData(2) + 1;
                createConfig(app, newConfigData);
                createTrigger(app, newTrigData);
                return
            end
            
            %config is same. don't add new config, but might need to add trig
            %store which trigs have the id of the config
            sameConfigCols = [];
            sameExp = false;
            for i=1:sizeOldTrigData(2)
                %expression and config ID are the same
                if strcmp(expres, oldTrigData(1,i)) && colIndex == str2double(oldTrigData(2,i))
                    sameConfigCols(end+1) = i;
                    sameExp = true;
                end
            end
            
            addChannelToList = false;
            if sameExp
                if strcmp(app.NeurostimulatorDropDown.Value,app.neuroOmega)
                    %neuro omega
                    %need to see if ch and return ch are in there
                    %go thru each trig column where they have the config
                    for i=1:length(sameConfigCols)
                        col = sameConfigCols(i);
                        %column has same config
                        if (retCh == oldTrigData(4,col))
                            %return ch is the same
                            addChannelToList = true;
                            trigIndex = col;
                            break
                        else
                            %diff return ch, need new trigger
                            sameExp = false;
                        end
                    end
                elseif strcmp(app.NeurostimulatorDropDown.Value,app.cortec)
                    %cortec
                    %both ch and return channels can have lists
                    addChannelToList = true;
                    trigIndex = sameConfigCols(1);
                elseif strcmp(app.NeurostimulatorDropDown.Value,app.cereStim)
                    %cerestim
                    addChannelToList = true;
                    %cerestim should only have 1 trig column with same expression
                    trigIndex = sameConfigCols(1);
                elseif strcmp(app.NeurostimulatorDropDown.Value,app.gEstim)
                    %gEstim
                    %need to see if abort exp is the same
                    for i=1:length(sameConfigCols)
                        col = sameConfigCols(i);
                        if strcmp(abortExp, oldTrigData(3,col))
                            %abort exp is the same
                            %don't want to add anything
                            errorSameConfiguration(app);
                            return
                        end
                    end
                    sameExp = false;
                end
            end

            %same config, same trig
            %add channel to list, don't add new config or trig
            if addChannelToList
                channels = oldTrigData(3,trigIndex);
                chList = split(channels,", ");
%                 for c=1:length(chList)
%                     %only add to list if it isn't there already
%                     if (str2double(ch) == str2double(chList(c)))
%                         errorSameConfiguration(app);
%                         return
%                     end
%                 end
                oldTrigData(3,trigIndex) = formatChList(app,[chList; ch]);
                if strcmp(app.NeurostimulatorDropDown.Value,app.cortec)
                    %cortec
                    retChannels = oldTrigData(4,trigIndex);
                    retChList = split(retChannels,", ");
                    oldTrigData(4,trigIndex) = formatChList(app,[retChList; retCh]);
                end
                app.TableTriggers.Data = oldTrigData;

                %update button selection
                selectThisButton(app, oldTrigData(1,trigIndex));
                return
            end

            %same config, new trigger
            if ~sameExp
                %add new trigger, not new config
                newTrigData(2) = colIndex;
                createTrigger(app, newTrigData);
                return
            end

            errordlg('Unexpected software error. Please restart','Software Error');
               
        end

        % Button pushed function: ExporttoparameterfileButton
        function ExporttoparameterfileButtonPushed(app, event)
            %sanity checks
            configs = app.TableConfigurations.Data;
            cRowLabels = app.TableConfigurations.RowName;
            numConfigs = size(configs,2);
            triggers = app.TableTriggers.Data;
            tRowLabels = app.TableTriggers.RowName;
            numTriggers = size(triggers,2);
            if numConfigs == 0
                errordlg('Please add data to the tables first, then save your parameter file. ', 'Parameter tables are empty');
                return
            end
            if strcmp(app.NeurostimulatorDropDown.Value,app.gEstim)
                %gEstim can only export one column at the moment
                if numConfigs ~= 1 || numTriggers ~= 1
                    errordlg(['BCI2000 can only load one configuration for g.Estim. ' ...
                        'Please make your configuration and trigger tables one column before exporting. '], 'One configuration constraint');
                    return
                end
            elseif strcmp(app.NeurostimulatorDropDown.Value, app.cortec)
                %will have same number of triggers, might have diff num of configs
                for t = 1:numTriggers
                    configID = str2double(triggers(2,t));
                    %move data over
                    triggers(5:7, t) = configs(5:7, configID); %reps, train freq, train reps
                    %data that is left over
                    pulse = configs(1:4, configID);
                    for c = 1:numConfigs
                        if all(isequal(pulse, configs(1:4,c)))
                            %use this config ID instead
                            triggers(2,t) = string(c);
                            break
                        end
                    end
                end
                %get rid of repeats and train configs
                configs(5:7, :) = [];
                %delete any extra configs, now that we moved IDs over
                i = 1;
                while (i <= numConfigs)
                    j = i+1;
                    while (j < numConfigs)
                        if all(isequal(configs(:,i),configs(:,j)))
                            configs(:,j) = [];
                            numConfigs = numConfigs - 1;
                        else
                            j = j+1;
                        end
                    end
                    i = i+1;
                end
                pulseIDs = 1:numConfigs;
                configs = [pulseIDs; configs]; %add pulse IDs

                %update labels
                app.TableConfigurations.ColumnName = "Configuration" + string(1:(size(configs,2)+1));
                app.TableConfigurations.ColumnName(end) = [];
                tRowLabels(end+1:end+3) = cRowLabels(end-2:end);
                cRowLabels = ["PulseID"; cRowLabels];
                cRowLabels(end-2:end) = [];
                app.TableTriggers.ColumnName = "Trigger" + string(1:(size(triggers,2)+1));
                app.TableTriggers.ColumnName(end) = [];

            end


            %create parameter file
            [file, path] = uiputfile('*.prm');
            fPath = fullfile(path, file);
            if file ~=0
                settings.parm_filename = fPath;
            else
                return
            end
            
            %stimulation configuration
            columnLabels = app.TableConfigurations.ColumnName;
            rowSize = length(cRowLabels);
            if strcmp(app.NeurostimulatorDropDown.Value,app.gEstim)
                %add expressions
                trigs = app.TableTriggers.Data;
                configs = [trigs(1); trigs(3); configs];
                %format units for parameters
                configs(5) = configs(5) + " ms";
                configs(6) = configs(6) + " ms";
                configs(8) = configs(8) + " Hz";
                configs(10) = configs(10) + " Hz";
                %format of gEstim is different, separate each parameter
                fields = ["StimulationExpression", "AbortExpression", "Modularity", ...
                            "Polarity", "PhaseDuration", "InterphaseDuration", ...
                            "Magnitude", "PulseFrequency", "NumberOfPulses", ...
                            "FrequencyOfTrains", "NumberOfTrains"];
                numParams = length(fields);
                for i=1:numParams
                    if i < 3
                        param.(fields(i)).Type = 'string';
                    else
                        param.(fields(i)).Type = 'float';
                    end
                    param.(fields(i)).DefaultValue = '';
                    param.(fields(i)).LowRange = '';
                    param.(fields(i)).HighRange = '';
                    param.(fields(i)).Value = cellstr(configs(i));
                    param.(fields(i)).Section = 'gEstim:Stimulation';
                    param.(fields(i)).Comment = 'gEstim Stim Config';
                end

            else
                if strcmp(app.NeurostimulatorDropDown.Value, app.cortec)
                    configName = "StimulationPulses";
                else
                    configName = "StimulationConfigurations";
                end
                param.(configName).Type = 'matrix';
                param.(configName).DefaultValue = '';
                param.(configName).LowRange = '';
                param.(configName).HighRange = '';
                param.(configName).Value = cell(rowSize, numConfigs);
                param.(configName).RowLabels = cell(rowSize,1);
                param.(configName).ColumnLabels = cell(1, numConfigs);
                if strcmp(app.NeurostimulatorDropDown.Value,app.cereStim)
                    param.(configName).Section = 'CereStim:Waveform definition';
                    param.StimulationTriggers.Section = param.(configName).Section;
                    param.(configName).Comment = 'Configurations for CereStim';
                    param.StimulationTriggers.Comment = 'Triggers for CereStim';
                elseif strcmp(app.NeurostimulatorDropDown.Value,app.neuroOmega)
                    param.(configName).Section = 'Application:NeuroOmegaStimulation';
                    param.StimulationTriggers.Section = param.(configName).Section;
                    param.(configName).Comment = 'Configurations for NeuroOmega';
                    param.StimulationTriggers.Comment = 'Triggers for NeuroOmega';
                elseif strcmp(app.NeurostimulatorDropDown.Value, app.cortec)
                    param.(configName).Section = 'Stimulation:Stimulation';
                    param.StimulationTriggers.Section = param.(configName).Section;
                    param.(configName).Comment = 'Configurations for Cortec';
                    param.StimulationTriggers.Comment = 'Triggers for Cortec';
                end
                
                for i=1:rowSize
                    param.(configName).RowLabels{i} = char(cRowLabels(i));
                end
                for i=1:size(configs,2)
                    if size(configs,2) > 1
                        param.(configName).ColumnLabels{i} = char(columnLabels(i));
                    else
                        if iscell(columnLabels)
                            param.(configName).ColumnLabels = columnLabels;
                        else
                            c{1} = columnLabels;
                            param.(configName).ColumnLabels = c;
                        end
                    end
                end
                for j=1:numConfigs
                    for i=1:rowSize
                        if numConfigs > 1
                            param.(configName).Value{i, j} = num2str(configs(i,j));
                        else
                            param.(configName).Value{i, j} = sprintf('%d', configs(i,j));
                        end
                    end
                end
                
                %stimulation triggers
                columnLabels = app.TableTriggers.ColumnName;
                rowSize = length(tRowLabels);
                param.StimulationTriggers.Type = 'matrix';
                param.StimulationTriggers.DefaultValue = '';
                param.StimulationTriggers.LowRange = '';
                param.StimulationTriggers.HighRange = '';
                param.StimulationTriggers.Value = cell(rowSize, numTriggers);
                param.StimulationTriggers.RowLabels = cell(rowSize,1);
                param.StimulationTriggers.ColumnLabels = cell(1, numTriggers);
                
                for i=1:rowSize
                    param.StimulationTriggers.RowLabels{i} = char(tRowLabels(i));
                end
                for i=1:numTriggers
                    if numTriggers > 1
                        param.StimulationTriggers.ColumnLabels{i} = char(columnLabels(i));
                    else
                        if iscell(columnLabels)
                            param.StimulationTriggers.ColumnLabels = columnLabels;
                        else
                            c{1} = columnLabels;
                            param.StimulationTriggers.ColumnLabels = c;
                        end
                    end
                end
                for j=1:numTriggers
                    %param.StimulationTriggers.Value{1, j} = sprintf('%s',triggers(1,j));
                    %sprintf('%s',triggers(1,j))
                    for i=1:rowSize
                            a = triggers(i,j);
                            param.StimulationTriggers.Value{i, j} = a{1};
                            %convert ch to list
                            if (i == 3 || i == 4)
                                if contains(triggers(i,j), ",")
                                    chList = split(triggers(i,j), ", ");
                                    %chList = erase(triggers(i,j), ",")
                                    %f = "{ matrix " + string(nList) + " 1 " + chList + " }"
                                    %param.StimulationTriggers.Value{i, j} = "{ matrix " + string(nList+1) + " 1 " + chList + " }";
    %                             channel = triggers(i,j);
                                    chanMat = cell(length(chList),1);
                                    for k=1:length(chanMat)
                                        chanMat(k,1) = {num2str(chList(k))};
                                    end
                                    param.StimulationTriggers.Value{i, j} = chanMat;
    %                             chanMat
                                end
                            end                        
                    end
                end
            end
            
            %write the param struct to a bci2000 parameter file
            try
                parameter_lines = convert_bciprm( param );
                fid = fopen(settings.parm_filename, 'w');
                
                for i=1:length(parameter_lines)
                    fprintf( fid, '%s', parameter_lines{i} );
                    fprintf( fid, '\r\n' );
                end
                fclose(fid);    
            catch ME
                report = getReport(ME);
                fig = uifigure;
                uialert(fig,report,'Error Message');
%                 uialert(fig,"There is no 'convert_bciprm' mex file in your path. " + ...
%                     newline + newline + "Use the 'Add MEX files to path!' button to add to path, or go to " + ...
%                     "https://www.bci2000.org/mediawiki/index.php/User_Reference:Matlab_MEX_Files for more information. ",'Error Message');
            end
            
            
            
        end

        % Callback function
        function AddMEXfilestopathButtonPushed(app, event)
            %let user navigate to folder containing convert_bciprm
            selpath = uigetdir();
            if selpath ~= 0
                addpath(selpath)
            end
        end

        % Callback function
        function BrowseButtonPushed(app, event)
            selpath = uiputfile('*.prm');
            app.ParamFilePathEditField.Value = selpath;
        end

        % Button pushed function: DeleteConfigurationButton
        function DeleteConfigurationButtonPushed(app, event)
            badConfig = app.DeleteChannelDropDown.Value;
            
            labels = app.TableConfigurations.ColumnName;
            for i=1:length(labels)
                if iscell(labels)
                    l = labels(i);
                    newL = l{1};
                else
                    newL = labels;
                end
                if badConfig == newL
                    %delete config
                    deleteConfig(app, i);
                    %might want to delete multiple triggers
                    trigData = app.TableTriggers.Data;
                    deleteThese = [];
                    for j=1:size(trigData,2) 
                        if (i == str2double(trigData(2,j)))
                            deleteThese(end+1) = j;
                        end
                    end
                    sortedA = sort(deleteThese, 'descend');
                    for j=1:length(sortedA)
                        if j==length(sortedA) %only shift on last deletion
                            deleteTrigger(app, sortedA(j), true, true);
                        else
                            deleteTrigger(app, sortedA(j), false, true);
                        end
                    end
                    break
                end
            end
            updateChannelVisualization(app);
        end

        % Button pushed function: DeleteTriggerButton
        function DeleteTriggerButtonPushed(app, event)
            badConfig = app.DeleteTriggerDropDown.Value;
            
            labels = app.TableTriggers.ColumnName;
            for i=1:length(labels)
                if iscell(labels)
                    l = labels(i);
                    newL = l{1};
                else
                    newL = labels;
                end
                if strcmp(badConfig, newL)
                    %make sure no other ids are correlated
                    trigData = app.TableTriggers.Data;
                    canDelete = true;
                    for j=1:size(trigData,2)
                        if ((i ~= j) && strcmp(trigData(2,i),trigData(2,j)))
                            canDelete = false;
                        end
                    end
                    if canDelete
                        deleteConfig(app, str2double(trigData(2,i)));
                    end
                    deleteTrigger(app, i, canDelete, true);
                    break
                end
            end
            updateChannelVisualization(app);
        end

        % Value changed function: NeurostimulatorDropDown
        function NeurostimulatorDropDownValueChanged(app, event)
            initializeStimulator(app);
        end

        % Selection changed function: RecordingChannelsButtonGroup
        function RecordingChannelsButtonGroupSelectionChanged(app, event)
            setNeuroOmegaChannels(app)
            updateChannelVisualization(app);
            selectThisButton(app, app.firstButtonText);
        end

        % Menu selected function: LoadParameterFileMenu
        function LoadParameterFileMenuSelected(app, event)
            [file, path] = uigetfile('*.prm');
            updatedTables = false;
            if file~=0
                fileID = fopen(fullfile(path, file));
                %define gEstim parameter names
                configNames = ["Modularity", "Polarity", "PhaseDuration", ...
                    "InterphaseDuration", "Magnitude", "PulseFrequency", ...
                    "NumberOfPulses", "FrequencyOfTrains", "NumberOfTrains"];
                configNames = strcat(configNames, "="); %add "=" to parameter names
                cTable = [];
                tTable = string.empty;
                while ~feof(fileID)
                    line = fgetl(fileID);
                    lineArr = strsplit(line);
                    if length(lineArr) < 3
                        continue
                    end
                    %gEstim has different parameter syntax
                    %hopefully follows parameter syntax
                    if contains(lineArr(1), "gEstim")
                        app.NeurostimulatorDropDown.Value = app.gEstim;
                        parameterName = lineArr(3);
                        value = convertCharsToStrings(lineArr{4});
                        if strcmp(value, "%") %empty parameter
                            value = "";
                        end
                        if strcmp(parameterName,"StimulationExpression=")
                            tTable(1,1) = value;
                        elseif strcmp(parameterName,"AbortExpression=")
                            tTable(3,1) = value;
                        else
                            for i=1:length(configNames)
                                if strcmp(parameterName, configNames(i))
                                    cTable(i,1) = formatImportedParameter(app, value);
                                    break
                                end
                            end
                            tTable(2,1) = '1'; %set multiple times, shouldn't matter
                        end
                        updatedTables = true;
                    
                    %hopefully true for both cerestim and neuro omega
                    elseif lineArr(3)=="StimulationConfigurations=" ...
                        || lineArr(3)=="StimulationPulses=" %cortec
                        %send full lineArr
                        [rNames, cNames, lineArr] = getLabels(app, lineArr);
                        cTable = [];
                        for r=1:length(rNames)
                            for count=1:length(cNames)
                                cTable(r, count) = str2double(lineArr{count});
                            end
                            lineArr = lineArr(length(cNames)+1:end);
                        end
                        updatedTables = true;
                        %app.TableConfigurations.RowName = rNames;
                        %app.TableConfigurations.ColumnName = cNames;
                    elseif lineArr(3)=="StimulationTriggers="
                        %send full lineArr
                        [rNames, cNames, lineArr] = getLabels(app, lineArr);
                        app.lastStimName = app.NeurostimulatorDropDown.Value;
                        if (length(rNames) == 4)
                            %neuro omega
                            app.NeurostimulatorDropDown.Value = app.neuroOmega;
                        elseif (length(rNames) == 3)
                            %cerestim
                            app.NeurostimulatorDropDown.Value = app.cereStim;
                        elseif (length(rNames) == 7) || (length(rNames) == 5)
                            %cortec
                            app.NeurostimulatorDropDown.Value = app.cortec;
                        end
                        tTable = string.empty;
                        for r=1:length(rNames)
                            %for updating the table correctly
                            count=1;
                            for col=1:length(cNames)
                                if strcmp(lineArr{count}, '{')
                                    if strcmp(lineArr{count+1}, 'matrix')
                                        %matrix
                                        rSize = str2double(lineArr{count+2});
                                        cSize = str2double(lineArr{count+3});
                                        cellString = "";
                                        %cellString = string(lineArr{c+3});
                                        for i=1:rSize
                                            for j=1:cSize
                                                lineArr{count+3+cSize*(i-1)+j};
                                                if j*i == 1
                                                    %don't want a comma b4 the first one
                                                    cellString = lineArr{count+3+cSize*(i-1)+j};
                                                else
                                                    cellString = cellString + ", " + lineArr{count+3+cSize*(i-1)+j};
                                                end
                                            end
                                        end
                                        tTable(r,col) = cellString;
                                        %gotta update the counter irregularly
                                        count = count + cSize + rSize + 4;
                                    elseif strcmp(lineArr{count+1}, 'list')
                                        %list
                                        cSize = str2double(lineArr{count+2});
                                        for i=1:cSize
                                            if i == 1
                                                %don't want a comma b4 the first one
                                                cellString = lineArr{count+2+i};
                                            else
                                                cellString = cellString + ", " + lineArr{count+2+i};
                                            end
                                        end
                                        tTable(r,col) = cellString;
                                        %gotta account for '{', 'list', 'cSize', '}'
                                        count = count + cSize + 4;
                                    end
                                    if r==2
                                        errordlg(['Loading parameter files with Config IDs as a list is not supported. ' ...
                                        'Please make all Config IDs in StimulationTriggers scalar values.'],'StimulationTriggers Error');
                                        fclose(fileID);
                                        return
                                    end
                                else
                                    tTable(r, col) = string(lineArr{count});
                                    %update counter like normal
                                    count = count + 1;
                                end
                            end
                            lineArr = lineArr(count:end);
                        end
                        updatedTables = true;
                        %app.TableTriggers.RowName = rNames;
                        %app.TableTriggers.ColumnName = cNames;
                        %app.TableTriggers.Data = t;
                    end
                end
                fclose(fileID);

                if ~updatedTables
                    errordlg('No StimulationConfigurations or StimulationTriggers were found.','Loading Parameter File Error'); 
                    return
                end

                %configure for cortec
                if strcmp(app.NeurostimulatorDropDown.Value, app.cortec)
                    %add repetitions and train configuration to pulses in config table
                    cID = 1;
                    numLoadedIDs = size(cTable,2);
                    for i=1:size(tTable, 2)
                        pulseID = tTable(2,i);
                        for j = 1:numLoadedIDs
                            if cTable(1,j)== str2double(pulseID)
                                %create new column
                                for k=1:8
                                    if k < 6
                                        cTable(k,numLoadedIDs + cID) = cTable(k,j);
                                    else
                                        cTable(k,numLoadedIDs + cID) = tTable(k-1,i);
                                    end
                                end
                                tTable(2,i) = string(cID); %update pulseID
                                cID = cID + 1;
                                break
                            end
                        end
                    end
                    %get rid of extra table rows, all of them should be moved over to configuration
                    tTable(5:7,:) = [];
                    %get rid of original configuration columns
                    cTable(:, 1:numLoadedIDs) = [];
                    %get rid of "PulseID" column in Configuration table
                    cTable(1,:) = [];
                end


                %check to see if there are any repeat configurations
                repeatedC = [];
                %only check first half, second half will be repeats
                for c=1:ceil(size(cTable,2)/2)
                    for i=1:size(cTable,2)
                        if i ~= c && all(cTable(:,c) == cTable(:,i))
                            repeatedC(end+1,:) = [c i];
                        end
                    end
                end
                if ~isempty(repeatedC)
                    fig = uifigure;
                    selection = uiconfirm(fig,['Repeated configurations are not supported. ' ...
                        'Would you like to automatically convert the Config IDs?'],'Confirm Load',...
                                            'Icon','warning');
                    delete(fig)
                    if selection ~= "OK"
                        return
                    else
                        %convert config ids in trigger table, then delete repeat configs
                        for num=1:size(repeatedC,1)
                            %remove i, keep c
                            for j=1:size(tTable,2)
                                tI = str2double(tTable(2,j));
                                if tI == repeatedC(num,2)
                                    tTable(2,j) = string(repeatedC(num,1));
                                %gotta move it down cause we delete a config
                                elseif tI > repeatedC(num,2)
                                    tTable(2,j) = string(tI - 1);
                                end
                            end
                        end
                        for num=size(repeatedC,1):-1:1
                            %now delete from end to front
                            cTable(:,repeatedC(num,2)) = [];
                        end
                    end
                end

                %if there are no train specified
                if size(cTable,1) == 8
                    cTable(9:10,:) = zeros(size(cTable(1:2,:)));
                end

                if ~clearAllData(app)
                    return
                end
                %init tables
                initializeStimulator(app);
            
                %update tables
                if ~isempty(cTable)
                    createConfig(app, cTable);
                end
                if ~isempty(tTable)
                    createTrigger(app, tTable);
                end
            end
        end

        % Value changed function: Phase1ampuAEditField
        function Phase1ampuAEditFieldValueChanged(app, event)
            selectThisButton(app, app.firstButtonText);
        end

        % Value changed function: Phase2ampuAEditField
        function Phase2ampuAEditFieldValueChanged(app, event)
            selectThisButton(app, app.firstButtonText);
        end

        % Value changed function: Phase1durationusEditField
        function Phase1durationusEditFieldValueChanged(app, event)
            selectThisButton(app, app.firstButtonText);
        end

        % Value changed function: Phase2durationusEditField
        function Phase2durationusEditFieldValueChanged(app, event)
            selectThisButton(app, app.firstButtonText);
        end

        % Value changed function: FrequencyHzEditField
        function FrequencyHzEditFieldValueChanged(app, event)
            selectThisButton(app, app.firstButtonText);
        end

        % Value changed function: TrainDurationsEditField
        function TrainDurationsEditFieldValueChanged(app, event)
            selectThisButton(app, app.firstButtonText);
        end

        % Value changed function: TrainFrequencyHzEditField
        function TrainFrequencyHzEditFieldValueChanged(app, event)
            selectThisButton(app, app.firstButtonText);
        end

        % Value changed function: NumberofpulsesSpinner
        function NumberofpulsesSpinnerValueChanged(app, event)
            selectThisButton(app, app.firstButtonText);
        end

        % Value changed function: InterphasedurationusEditField
        function InterphasedurationusEditFieldValueChanged(app, event)
            selectThisButton(app, app.firstButtonText);
        end

        % Value changed function: Phase1delaymsEditField
        function Phase1delaymsEditFieldValueChanged(app, event)
            selectThisButton(app, app.firstButtonText);
        end

        % Selection changed function: OrderButtonGroup
        function OrderButtonGroupSelectionChanged(app, event)
            selectThisButton(app, app.firstButtonText);
        end

        % Value changed function: StimulationDurationsEditField
        function StimulationDurationsEditFieldValueChanged(app, event)
            selectThisButton(app, app.firstButtonText);
        end

        % Menu selected function: AutoincrementTriggerExpressionMenu
        function AutoincrementTriggerExpressionMenuSelected(app, event)
            app.AutoincrementTriggerExpressionMenu.Checked = ~app.AutoincrementTriggerExpressionMenu.Checked;
        end

        % Display data changed function: TableConfigurations
        function TableConfigurationsDisplayDataChanged(app, event)
            updateGraphs(app)
        end

        % Selection changed function: PolarityButtonGroup
        function PolarityButtonGroupSelectionChanged(app, event)
            selectThisButton(app, app.firstButtonText);
        end

        % Selection changed function: ButtonGroup
        function ButtonGroupSelectionChanged(app, event)
            updateGraphs(app)
            updateChannelVisualization(app);
        end

        % Size changed function: ButtonGroup
        function ButtonGroupSizeChanged(app, event)
            positionButtons(app);
        end

        % Menu selected function: BackgroundMenu
        function BackgroundMenuSelected(app, event)
            changeColor(app,0)
        end

        % Menu selected function: Plot1Menu
        function Plot1MenuSelected(app, event)
            changeColor(app,1);
        end

        % Menu selected function: Plot2Menu
        function Plot2MenuSelected(app, event)
            changeColor(app,2);
        end

        % Menu selected function: Plot3Menu
        function Plot3MenuSelected(app, event)
            changeColor(app,3);
        end

        % Menu selected function: RestoreDefaultColorsMenu
        function RestoreDefaultColorsMenuSelected(app, event)
            plots = [app.SinglePulseAxes, app.MultPulsesAxes, app.TrainAxes];
            for i=1:length(plots)
                plots(i).Color = app.defaultBackgroundColor;
                plots(i).ColorOrder = app.defaultColors;
            end
        end

        % Button pushed function: PlayStimulationButton
        function PlayStimulationButtonPushed(app, event)
            for i=1:length(app.TrainAxes.Children)
                if strcmp(app.TrainAxes.Children(i).Type,'line')
                    %grab plot
                    p = app.TrainAxes.Children(i);

                    %init marker
                    hold(app.TrainAxes, "on")
                    c = xline(app.TrainAxes, 0, 'LineWidth',3, 'Color', [1 1 1]);
                    hold(app.TrainAxes, "off")

                    %init lamp
                    app.StimulationOnLamp.Color = [1 1 0];
                    app.SinglePulseAxes.Color = [0 0 0];
                    app.MultPulsesAxes.Color = [0 0 0];

                    %get data points of existing train
                    times = zeros(500,1);
                    count = 0;
                    for j = 1:20:length(p.XData)
                        count = count + 1;
                        tic
                        c.Value =  p.XData(j);
                        drawnow

                        %update lamp
%                         if toc >= p.XData(j) %time has passed
                            if p.YData(j)==1 && app.StimulationOnLamp.Color(1)
                                app.StimulationOnLamp.Color = [0 1 0];
                                app.SinglePulseAxes.Color = [1 1 1];
                                app.MultPulsesAxes.Color = [1 1 1];
                            elseif p.YData(j)==0 && ~app.StimulationOnLamp.Color(1)
                                app.StimulationOnLamp.Color = [1 1 0];
                                app.SinglePulseAxes.Color = [0 0 0];
                                app.MultPulsesAxes.Color = [0 0 0];
                            end
%                         else
%                             'skipping'
%                         end
                        times(count) = toc;
                    end
                    delete(c);
                    histogram(times);

                    %turn off lamp
                    app.StimulationOnLamp.Color = [1 0 0];
                end
            end
        end

        % Value changed function: ChannelDropDown
        function ChannelDropDownValueChanged(app, event)
            app.sourceChChanged = true;
            selectThisButton(app, app.firstButtonText);
        end

        % Value changed function: ReturnChannelDropDown
        function ReturnChannelDropDownValueChanged(app, event)
            app.sourceChChanged = false;
            selectThisButton(app, app.firstButtonText);
        end

        % Cell selection callback: TableTriggers
        function TableTriggersCellSelection(app, event)
            indices = event.Indices;
            %find the selected column
            if ~isempty(indices)
                c = indices(1,2);
                if all(indices(:,2)==c)
                    exp = app.TableTriggers.Data(1,c);
                    %change if we are not there already
                    if ~strcmp(exp,app.ButtonGroup.SelectedObject.Text)
                        selectThisButton(app, exp);
                    end
                end
            end            
        end

        % Menu selected function: IncreaseFontSizeMenu
        function IncreaseFontSizeMenuSelected(app, event)
            changeFontSize(app, app.UIFigure, 1)
        end

        % Menu selected function: DecreaseFontSizeMenu
        function DecreaseFontSizeMenuSelected(app, event)
            changeFontSize(app, app.UIFigure, -1)
        end

        % Changes arrangement of the app based on UIFigure width
        function updateAppLayout(app, event)
            currentFigureWidth = app.UIFigure.Position(3);
            if(currentFigureWidth <= app.onePanelWidth)
                % Change to a 3x1 grid
                app.GridLayout.RowHeight = {608, 608, 608};
                app.GridLayout.ColumnWidth = {'1x'};
                app.CenterPanel.Layout.Row = 1;
                app.CenterPanel.Layout.Column = 1;
                app.LeftPanel.Layout.Row = 2;
                app.LeftPanel.Layout.Column = 1;
                app.RightPanel.Layout.Row = 3;
                app.RightPanel.Layout.Column = 1;
            elseif (currentFigureWidth > app.onePanelWidth && currentFigureWidth <= app.twoPanelWidth)
                % Change to a 2x2 grid
                app.GridLayout.RowHeight = {608, 608};
                app.GridLayout.ColumnWidth = {'1x', '1x'};
                app.CenterPanel.Layout.Row = 1;
                app.CenterPanel.Layout.Column = [1,2];
                app.LeftPanel.Layout.Row = 2;
                app.LeftPanel.Layout.Column = 1;
                app.RightPanel.Layout.Row = 2;
                app.RightPanel.Layout.Column = 2;
            else
                % Change to a 1x3 grid
                app.GridLayout.RowHeight = {'1x'};
                app.GridLayout.ColumnWidth = {283, '1x', 396};
                app.LeftPanel.Layout.Row = 1;
                app.LeftPanel.Layout.Column = 1;
                app.CenterPanel.Layout.Row = 1;
                app.CenterPanel.Layout.Column = 2;
                app.RightPanel.Layout.Row = 1;
                app.RightPanel.Layout.Column = 3;
            end
        end
    end

    % Component initialization
    methods (Access = private)

        % Create UIFigure and components
        function createComponents(app)

            % Create UIFigure and hide until all components are created
            app.UIFigure = uifigure('Visible', 'off');
            app.UIFigure.AutoResizeChildren = 'off';
            app.UIFigure.Color = [0.0588 0 0.702];
            app.UIFigure.Colormap = [0.2431 0.149 0.6588;0.2431 0.1529 0.6745;0.2471 0.1569 0.6863;0.2471 0.1608 0.698;0.251 0.1647 0.7059;0.251 0.1686 0.7176;0.2549 0.1725 0.7294;0.2549 0.1765 0.7412;0.2588 0.1804 0.749;0.2588 0.1843 0.7608;0.2627 0.1882 0.7725;0.2627 0.1922 0.7843;0.2627 0.1961 0.7922;0.2667 0.2 0.8039;0.2667 0.2039 0.8157;0.2706 0.2078 0.8235;0.2706 0.2157 0.8353;0.2706 0.2196 0.8431;0.2745 0.2235 0.851;0.2745 0.2275 0.8627;0.2745 0.2314 0.8706;0.2745 0.2392 0.8784;0.2784 0.2431 0.8824;0.2784 0.2471 0.8902;0.2784 0.2549 0.898;0.2784 0.2588 0.902;0.2784 0.2667 0.9098;0.2784 0.2706 0.9137;0.2784 0.2745 0.9216;0.2824 0.2824 0.9255;0.2824 0.2863 0.9294;0.2824 0.2941 0.9333;0.2824 0.298 0.9412;0.2824 0.3059 0.9451;0.2824 0.3098 0.949;0.2824 0.3137 0.9529;0.2824 0.3216 0.9569;0.2824 0.3255 0.9608;0.2824 0.3294 0.9647;0.2784 0.3373 0.9686;0.2784 0.3412 0.9686;0.2784 0.349 0.9725;0.2784 0.3529 0.9765;0.2784 0.3569 0.9804;0.2784 0.3647 0.9804;0.2745 0.3686 0.9843;0.2745 0.3765 0.9843;0.2745 0.3804 0.9882;0.2706 0.3843 0.9882;0.2706 0.3922 0.9922;0.2667 0.3961 0.9922;0.2627 0.4039 0.9922;0.2627 0.4078 0.9961;0.2588 0.4157 0.9961;0.2549 0.4196 0.9961;0.251 0.4275 0.9961;0.2471 0.4314 1;0.2431 0.4392 1;0.2353 0.4431 1;0.2314 0.451 1;0.2235 0.4549 1;0.2196 0.4627 0.9961;0.2118 0.4667 0.9961;0.2078 0.4745 0.9922;0.2 0.4784 0.9922;0.1961 0.4863 0.9882;0.1922 0.4902 0.9882;0.1882 0.498 0.9843;0.1843 0.502 0.9804;0.1843 0.5098 0.9804;0.1804 0.5137 0.9765;0.1804 0.5176 0.9725;0.1804 0.5255 0.9725;0.1804 0.5294 0.9686;0.1765 0.5333 0.9647;0.1765 0.5412 0.9608;0.1765 0.5451 0.9569;0.1765 0.549 0.9529;0.1765 0.5569 0.949;0.1725 0.5608 0.9451;0.1725 0.5647 0.9412;0.1686 0.5686 0.9373;0.1647 0.5765 0.9333;0.1608 0.5804 0.9294;0.1569 0.5843 0.9255;0.1529 0.5922 0.9216;0.1529 0.5961 0.9176;0.149 0.6 0.9137;0.149 0.6039 0.9098;0.1451 0.6078 0.9098;0.1451 0.6118 0.9059;0.1412 0.6196 0.902;0.1412 0.6235 0.898;0.1373 0.6275 0.898;0.1373 0.6314 0.8941;0.1333 0.6353 0.8941;0.1294 0.6392 0.8902;0.1255 0.6471 0.8902;0.1216 0.651 0.8863;0.1176 0.6549 0.8824;0.1137 0.6588 0.8824;0.1137 0.6627 0.8784;0.1098 0.6667 0.8745;0.1059 0.6706 0.8706;0.102 0.6745 0.8667;0.098 0.6784 0.8627;0.0902 0.6824 0.8549;0.0863 0.6863 0.851;0.0784 0.6902 0.8471;0.0706 0.6941 0.8392;0.0627 0.698 0.8353;0.0549 0.702 0.8314;0.0431 0.702 0.8235;0.0314 0.7059 0.8196;0.0235 0.7098 0.8118;0.0157 0.7137 0.8078;0.0078 0.7176 0.8;0.0039 0.7176 0.7922;0 0.7216 0.7882;0 0.7255 0.7804;0 0.7294 0.7765;0.0039 0.7294 0.7686;0.0078 0.7333 0.7608;0.0157 0.7333 0.7569;0.0235 0.7373 0.749;0.0353 0.7412 0.7412;0.051 0.7412 0.7373;0.0627 0.7451 0.7294;0.0784 0.7451 0.7216;0.0902 0.749 0.7137;0.102 0.7529 0.7098;0.1137 0.7529 0.702;0.1255 0.7569 0.6941;0.1373 0.7569 0.6863;0.1451 0.7608 0.6824;0.1529 0.7608 0.6745;0.1608 0.7647 0.6667;0.1686 0.7647 0.6588;0.1725 0.7686 0.651;0.1804 0.7686 0.6471;0.1843 0.7725 0.6392;0.1922 0.7725 0.6314;0.1961 0.7765 0.6235;0.2 0.7804 0.6157;0.2078 0.7804 0.6078;0.2118 0.7843 0.6;0.2196 0.7843 0.5882;0.2235 0.7882 0.5804;0.2314 0.7882 0.5725;0.2392 0.7922 0.5647;0.251 0.7922 0.5529;0.2588 0.7922 0.5451;0.2706 0.7961 0.5373;0.2824 0.7961 0.5255;0.2941 0.7961 0.5176;0.3059 0.8 0.5059;0.3176 0.8 0.498;0.3294 0.8 0.4863;0.3412 0.8 0.4784;0.3529 0.8 0.4667;0.3686 0.8039 0.4549;0.3804 0.8039 0.4471;0.3922 0.8039 0.4353;0.4039 0.8039 0.4235;0.4196 0.8039 0.4118;0.4314 0.8039 0.4;0.4471 0.8039 0.3922;0.4627 0.8 0.3804;0.4745 0.8 0.3686;0.4902 0.8 0.3569;0.5059 0.8 0.349;0.5176 0.8 0.3373;0.5333 0.7961 0.3255;0.5451 0.7961 0.3176;0.5608 0.7961 0.3059;0.5765 0.7922 0.2941;0.5882 0.7922 0.2824;0.6039 0.7882 0.2745;0.6157 0.7882 0.2627;0.6314 0.7843 0.251;0.6431 0.7843 0.2431;0.6549 0.7804 0.2314;0.6706 0.7804 0.2235;0.6824 0.7765 0.2157;0.698 0.7765 0.2078;0.7098 0.7725 0.2;0.7216 0.7686 0.1922;0.7333 0.7686 0.1843;0.7451 0.7647 0.1765;0.7608 0.7647 0.1725;0.7725 0.7608 0.1647;0.7843 0.7569 0.1608;0.7961 0.7569 0.1569;0.8078 0.7529 0.1529;0.8157 0.749 0.1529;0.8275 0.749 0.1529;0.8392 0.7451 0.1529;0.851 0.7451 0.1569;0.8588 0.7412 0.1569;0.8706 0.7373 0.1608;0.8824 0.7373 0.1647;0.8902 0.7373 0.1686;0.902 0.7333 0.1765;0.9098 0.7333 0.1804;0.9176 0.7294 0.1882;0.9255 0.7294 0.1961;0.9373 0.7294 0.2078;0.9451 0.7294 0.2157;0.9529 0.7294 0.2235;0.9608 0.7294 0.2314;0.9686 0.7294 0.2392;0.9765 0.7294 0.2431;0.9843 0.7333 0.2431;0.9882 0.7373 0.2431;0.9961 0.7412 0.2392;0.9961 0.7451 0.2353;0.9961 0.7529 0.2314;0.9961 0.7569 0.2275;0.9961 0.7608 0.2235;0.9961 0.7686 0.2196;0.9961 0.7725 0.2157;0.9961 0.7804 0.2078;0.9961 0.7843 0.2039;0.9961 0.7922 0.2;0.9922 0.7961 0.1961;0.9922 0.8039 0.1922;0.9922 0.8078 0.1922;0.9882 0.8157 0.1882;0.9843 0.8235 0.1843;0.9843 0.8275 0.1804;0.9804 0.8353 0.1804;0.9765 0.8392 0.1765;0.9765 0.8471 0.1725;0.9725 0.851 0.1686;0.9686 0.8588 0.1647;0.9686 0.8667 0.1647;0.9647 0.8706 0.1608;0.9647 0.8784 0.1569;0.9608 0.8824 0.1569;0.9608 0.8902 0.1529;0.9608 0.898 0.149;0.9608 0.902 0.149;0.9608 0.9098 0.1451;0.9608 0.9137 0.1412;0.9608 0.9216 0.1373;0.9608 0.9255 0.1333;0.9608 0.9333 0.1294;0.9647 0.9373 0.1255;0.9647 0.9451 0.1216;0.9647 0.949 0.1176;0.9686 0.9569 0.1098;0.9686 0.9608 0.1059;0.9725 0.9686 0.102;0.9725 0.9725 0.0941;0.9765 0.9765 0.0863;0.9765 0.9843 0.0824];
            app.UIFigure.Position = [100 100 1166 608];
            app.UIFigure.Name = 'MATLAB App';
            app.UIFigure.SizeChangedFcn = createCallbackFcn(app, @updateAppLayout, true);

            % Create GridLayout
            app.GridLayout = uigridlayout(app.UIFigure);
            app.GridLayout.ColumnWidth = {283, '1x', 396};
            app.GridLayout.RowHeight = {'1x'};
            app.GridLayout.ColumnSpacing = 0;
            app.GridLayout.RowSpacing = 0;
            app.GridLayout.Padding = [0 0 0 0];
            app.GridLayout.Scrollable = 'on';

            % Create LeftPanel
            app.LeftPanel = uipanel(app.GridLayout);
            app.LeftPanel.BackgroundColor = [0.949 0.9804 1];
            app.LeftPanel.Layout.Row = 1;
            app.LeftPanel.Layout.Column = 1;
            app.LeftPanel.FontName = 'Montserrat';

            % Create SaveConfigurationtoTablesButton
            app.SaveConfigurationtoTablesButton = uibutton(app.LeftPanel, 'push');
            app.SaveConfigurationtoTablesButton.ButtonPushedFcn = createCallbackFcn(app, @SaveConfigurationtoTablesButtonPushed, true);
            app.SaveConfigurationtoTablesButton.BackgroundColor = [0.9882 0.8745 0.6627];
            app.SaveConfigurationtoTablesButton.FontName = 'Montserrat SemiBold';
            app.SaveConfigurationtoTablesButton.FontSize = 14;
            app.SaveConfigurationtoTablesButton.Position = [29 38 225 26];
            app.SaveConfigurationtoTablesButton.Text = 'Save Configuration to Tables';

            % Create Phase1ampuAEditFieldLabel
            app.Phase1ampuAEditFieldLabel = uilabel(app.LeftPanel);
            app.Phase1ampuAEditFieldLabel.HorizontalAlignment = 'right';
            app.Phase1ampuAEditFieldLabel.FontName = 'Montserrat';
            app.Phase1ampuAEditFieldLabel.Position = [6 427 149 22];
            app.Phase1ampuAEditFieldLabel.Text = 'Phase 1 amp (uA)';

            % Create Phase2ampuAEditFieldLabel
            app.Phase2ampuAEditFieldLabel = uilabel(app.LeftPanel);
            app.Phase2ampuAEditFieldLabel.HorizontalAlignment = 'right';
            app.Phase2ampuAEditFieldLabel.FontName = 'Montserrat';
            app.Phase2ampuAEditFieldLabel.Position = [6 394 149 22];
            app.Phase2ampuAEditFieldLabel.Text = 'Phase 2 amp (uA)';

            % Create Phase1durationusEditFieldLabel
            app.Phase1durationusEditFieldLabel = uilabel(app.LeftPanel);
            app.Phase1durationusEditFieldLabel.HorizontalAlignment = 'right';
            app.Phase1durationusEditFieldLabel.FontName = 'Montserrat';
            app.Phase1durationusEditFieldLabel.Position = [6 360 149 22];
            app.Phase1durationusEditFieldLabel.Text = 'Phase 1 duration (us)';

            % Create Phase2durationusEditFieldLabel
            app.Phase2durationusEditFieldLabel = uilabel(app.LeftPanel);
            app.Phase2durationusEditFieldLabel.HorizontalAlignment = 'right';
            app.Phase2durationusEditFieldLabel.FontName = 'Montserrat';
            app.Phase2durationusEditFieldLabel.Position = [6 289 149 22];
            app.Phase2durationusEditFieldLabel.Text = 'Phase 2 duration (us)';

            % Create FrequencyHzEditFieldLabel
            app.FrequencyHzEditFieldLabel = uilabel(app.LeftPanel);
            app.FrequencyHzEditFieldLabel.HorizontalAlignment = 'right';
            app.FrequencyHzEditFieldLabel.FontName = 'Montserrat';
            app.FrequencyHzEditFieldLabel.Position = [6 254 149 22];
            app.FrequencyHzEditFieldLabel.Text = 'Frequency (Hz)';

            % Create TrainDurationsEditFieldLabel
            app.TrainDurationsEditFieldLabel = uilabel(app.LeftPanel);
            app.TrainDurationsEditFieldLabel.HorizontalAlignment = 'right';
            app.TrainDurationsEditFieldLabel.FontName = 'Montserrat';
            app.TrainDurationsEditFieldLabel.Position = [46 219 109 22];
            app.TrainDurationsEditFieldLabel.Text = 'Train Duration (s)';

            % Create TrainFrequencyHzEditFieldLabel
            app.TrainFrequencyHzEditFieldLabel = uilabel(app.LeftPanel);
            app.TrainFrequencyHzEditFieldLabel.HorizontalAlignment = 'right';
            app.TrainFrequencyHzEditFieldLabel.FontName = 'Montserrat';
            app.TrainFrequencyHzEditFieldLabel.Position = [6 186 149 22];
            app.TrainFrequencyHzEditFieldLabel.Text = 'Train Frequency (Hz)';

            % Create ChannelDropDownLabel
            app.ChannelDropDownLabel = uilabel(app.LeftPanel);
            app.ChannelDropDownLabel.HorizontalAlignment = 'right';
            app.ChannelDropDownLabel.FontName = 'Montserrat';
            app.ChannelDropDownLabel.Position = [62 97 56 22];
            app.ChannelDropDownLabel.Text = 'Channel';

            % Create ChannelDropDown
            app.ChannelDropDown = uidropdown(app.LeftPanel);
            app.ChannelDropDown.Items = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24', '25', '26', '27', '28', '29', '30', ''};
            app.ChannelDropDown.ValueChangedFcn = createCallbackFcn(app, @ChannelDropDownValueChanged, true);
            app.ChannelDropDown.FontName = 'Montserrat';
            app.ChannelDropDown.Position = [133 97 100 22];
            app.ChannelDropDown.Value = '1';

            % Create NeurostimulatorDropDownLabel
            app.NeurostimulatorDropDownLabel = uilabel(app.LeftPanel);
            app.NeurostimulatorDropDownLabel.HorizontalAlignment = 'right';
            app.NeurostimulatorDropDownLabel.FontName = 'Montserrat SemiBold';
            app.NeurostimulatorDropDownLabel.Position = [8 555 107 22];
            app.NeurostimulatorDropDownLabel.Text = 'Neurostimulator';

            % Create InterphasedurationusEditFieldLabel
            app.InterphasedurationusEditFieldLabel = uilabel(app.LeftPanel);
            app.InterphasedurationusEditFieldLabel.HorizontalAlignment = 'right';
            app.InterphasedurationusEditFieldLabel.FontName = 'Montserrat';
            app.InterphasedurationusEditFieldLabel.Position = [6 324 150 22];
            app.InterphasedurationusEditFieldLabel.Text = 'Interphase duration (us)';

            % Create Phase1delaymsEditFieldLabel
            app.Phase1delaymsEditFieldLabel = uilabel(app.LeftPanel);
            app.Phase1delaymsEditFieldLabel.HorizontalAlignment = 'right';
            app.Phase1delaymsEditFieldLabel.FontName = 'Montserrat';
            app.Phase1delaymsEditFieldLabel.Visible = 'off';
            app.Phase1delaymsEditFieldLabel.Position = [40 458 115 22];
            app.Phase1delaymsEditFieldLabel.Text = 'Phase 1 delay (ms)';

            % Create StimulationDurationsEditFieldLabel
            app.StimulationDurationsEditFieldLabel = uilabel(app.LeftPanel);
            app.StimulationDurationsEditFieldLabel.HorizontalAlignment = 'right';
            app.StimulationDurationsEditFieldLabel.FontName = 'Montserrat';
            app.StimulationDurationsEditFieldLabel.Visible = 'off';
            app.StimulationDurationsEditFieldLabel.Position = [6 219 149 22];
            app.StimulationDurationsEditFieldLabel.Text = 'Stimulation Duration (s)';

            % Create ReturnChannelDropDownLabel
            app.ReturnChannelDropDownLabel = uilabel(app.LeftPanel);
            app.ReturnChannelDropDownLabel.HorizontalAlignment = 'right';
            app.ReturnChannelDropDownLabel.FontName = 'Montserrat';
            app.ReturnChannelDropDownLabel.Visible = 'off';
            app.ReturnChannelDropDownLabel.Position = [16 68 101 22];
            app.ReturnChannelDropDownLabel.Text = 'Return Channel';

            % Create ReturnChannelDropDown
            app.ReturnChannelDropDown = uidropdown(app.LeftPanel);
            app.ReturnChannelDropDown.Items = {'-1', '10272', '10273', '10274', '10275', '10276', '10277', '10278', '10279', '10280', '10281', '10282', '10283', '10284', '10285', '10286', '10287'};
            app.ReturnChannelDropDown.ValueChangedFcn = createCallbackFcn(app, @ReturnChannelDropDownValueChanged, true);
            app.ReturnChannelDropDown.Visible = 'off';
            app.ReturnChannelDropDown.FontName = 'Montserrat';
            app.ReturnChannelDropDown.Position = [132 68 100 22];
            app.ReturnChannelDropDown.Value = '-1';

            % Create TriggerExpressionLabel
            app.TriggerExpressionLabel = uilabel(app.LeftPanel);
            app.TriggerExpressionLabel.HorizontalAlignment = 'center';
            app.TriggerExpressionLabel.FontName = 'Montserrat';
            app.TriggerExpressionLabel.Position = [1 116 85 38];
            app.TriggerExpressionLabel.Text = {'Trigger'; 'Expression'};

            % Create AbortSpinner
            app.AbortSpinner = uispinner(app.LeftPanel);
            app.AbortSpinner.FontName = 'Montserrat';
            app.AbortSpinner.Visible = 'off';
            app.AbortSpinner.Position = [232 89 47 22];

            % Create AbortDropDown
            app.AbortDropDown = uidropdown(app.LeftPanel);
            app.AbortDropDown.Items = {'==', '>=', '<=', '!='};
            app.AbortDropDown.Visible = 'off';
            app.AbortDropDown.FontName = 'Montserrat';
            app.AbortDropDown.Position = [184 89 47 22];
            app.AbortDropDown.Value = '==';

            % Create AbortExpressionEditField
            app.AbortExpressionEditField = uieditfield(app.LeftPanel, 'text');
            app.AbortExpressionEditField.FontName = 'Montserrat';
            app.AbortExpressionEditField.FontSize = 11;
            app.AbortExpressionEditField.Visible = 'off';
            app.AbortExpressionEditField.Placeholder = 'Valid BCI2000 Exp';
            app.AbortExpressionEditField.Position = [92 89 89 22];
            app.AbortExpressionEditField.Value = 'StimulusCode';

            % Create AbortExpressionLabel
            app.AbortExpressionLabel = uilabel(app.LeftPanel);
            app.AbortExpressionLabel.HorizontalAlignment = 'center';
            app.AbortExpressionLabel.FontName = 'Montserrat';
            app.AbortExpressionLabel.Visible = 'off';
            app.AbortExpressionLabel.Position = [2 81 85 38];
            app.AbortExpressionLabel.Text = {'Abort'; 'Expression'};

            % Create Spinner
            app.Spinner = uispinner(app.LeftPanel);
            app.Spinner.FontName = 'Montserrat';
            app.Spinner.Position = [231 124 47 22];
            app.Spinner.Value = 1;

            % Create DropDown
            app.DropDown = uidropdown(app.LeftPanel);
            app.DropDown.Items = {'==', '>=', '<=', '!='};
            app.DropDown.FontName = 'Montserrat';
            app.DropDown.Position = [183 124 47 22];
            app.DropDown.Value = '==';

            % Create TriggerExpressionEditField
            app.TriggerExpressionEditField = uieditfield(app.LeftPanel, 'text');
            app.TriggerExpressionEditField.FontName = 'Montserrat';
            app.TriggerExpressionEditField.FontSize = 11;
            app.TriggerExpressionEditField.Placeholder = 'Valid BCI2000 Exp';
            app.TriggerExpressionEditField.Position = [91 124 89 22];
            app.TriggerExpressionEditField.Value = 'StimulusCode';

            % Create TrainFrequencyHzEditField
            app.TrainFrequencyHzEditField = uieditfield(app.LeftPanel, 'numeric');
            app.TrainFrequencyHzEditField.Limits = [0 Inf];
            app.TrainFrequencyHzEditField.ValueDisplayFormat = '%11.4g Hz';
            app.TrainFrequencyHzEditField.ValueChangedFcn = createCallbackFcn(app, @TrainFrequencyHzEditFieldValueChanged, true);
            app.TrainFrequencyHzEditField.FontName = 'Montserrat';
            app.TrainFrequencyHzEditField.Position = [162 186 105 22];
            app.TrainFrequencyHzEditField.Value = 2;

            % Create StimulationDurationsEditField
            app.StimulationDurationsEditField = uieditfield(app.LeftPanel, 'numeric');
            app.StimulationDurationsEditField.Limits = [0 Inf];
            app.StimulationDurationsEditField.ValueDisplayFormat = '%11.4g s';
            app.StimulationDurationsEditField.ValueChangedFcn = createCallbackFcn(app, @StimulationDurationsEditFieldValueChanged, true);
            app.StimulationDurationsEditField.FontName = 'Montserrat';
            app.StimulationDurationsEditField.Visible = 'off';
            app.StimulationDurationsEditField.Position = [162 219 104 22];
            app.StimulationDurationsEditField.Value = 0.01;

            % Create TrainDurationsEditField
            app.TrainDurationsEditField = uieditfield(app.LeftPanel, 'numeric');
            app.TrainDurationsEditField.Limits = [0 Inf];
            app.TrainDurationsEditField.ValueDisplayFormat = '%11.4g s';
            app.TrainDurationsEditField.ValueChangedFcn = createCallbackFcn(app, @TrainDurationsEditFieldValueChanged, true);
            app.TrainDurationsEditField.FontName = 'Montserrat';
            app.TrainDurationsEditField.Position = [162 219 105 22];
            app.TrainDurationsEditField.Value = 2;

            % Create FrequencyHzEditField
            app.FrequencyHzEditField = uieditfield(app.LeftPanel, 'numeric');
            app.FrequencyHzEditField.Limits = [4 5000];
            app.FrequencyHzEditField.ValueDisplayFormat = '%11.4g Hz';
            app.FrequencyHzEditField.ValueChangedFcn = createCallbackFcn(app, @FrequencyHzEditFieldValueChanged, true);
            app.FrequencyHzEditField.FontName = 'Montserrat';
            app.FrequencyHzEditField.Position = [162 254 105 22];
            app.FrequencyHzEditField.Value = 100;

            % Create NumberofpulsesSpinnerLabel
            app.NumberofpulsesSpinnerLabel = uilabel(app.LeftPanel);
            app.NumberofpulsesSpinnerLabel.HorizontalAlignment = 'right';
            app.NumberofpulsesSpinnerLabel.FontName = 'Montserrat';
            app.NumberofpulsesSpinnerLabel.Position = [42 458 113 22];
            app.NumberofpulsesSpinnerLabel.Text = 'Number of pulses';

            % Create Phase2durationusEditField
            app.Phase2durationusEditField = uieditfield(app.LeftPanel, 'numeric');
            app.Phase2durationusEditField.Limits = [1 65535];
            app.Phase2durationusEditField.ValueDisplayFormat = '%11.4g us';
            app.Phase2durationusEditField.ValueChangedFcn = createCallbackFcn(app, @Phase2durationusEditFieldValueChanged, true);
            app.Phase2durationusEditField.FontName = 'Montserrat';
            app.Phase2durationusEditField.Position = [162 289 105 22];
            app.Phase2durationusEditField.Value = 500;

            % Create InterphasedurationusEditField
            app.InterphasedurationusEditField = uieditfield(app.LeftPanel, 'numeric');
            app.InterphasedurationusEditField.Limits = [53 65535];
            app.InterphasedurationusEditField.ValueDisplayFormat = '%11.4g us';
            app.InterphasedurationusEditField.ValueChangedFcn = createCallbackFcn(app, @InterphasedurationusEditFieldValueChanged, true);
            app.InterphasedurationusEditField.FontName = 'Montserrat';
            app.InterphasedurationusEditField.Position = [162 324 105 22];
            app.InterphasedurationusEditField.Value = 53;

            % Create Phase1durationusEditField
            app.Phase1durationusEditField = uieditfield(app.LeftPanel, 'numeric');
            app.Phase1durationusEditField.Limits = [1 65535];
            app.Phase1durationusEditField.ValueDisplayFormat = '%11.4g us';
            app.Phase1durationusEditField.ValueChangedFcn = createCallbackFcn(app, @Phase1durationusEditFieldValueChanged, true);
            app.Phase1durationusEditField.FontName = 'Montserrat';
            app.Phase1durationusEditField.Position = [162 360 105 22];
            app.Phase1durationusEditField.Value = 250;

            % Create Phase2ampuAEditField
            app.Phase2ampuAEditField = uieditfield(app.LeftPanel, 'numeric');
            app.Phase2ampuAEditField.Limits = [100 10000];
            app.Phase2ampuAEditField.ValueDisplayFormat = '%11.4g uA';
            app.Phase2ampuAEditField.ValueChangedFcn = createCallbackFcn(app, @Phase2ampuAEditFieldValueChanged, true);
            app.Phase2ampuAEditField.FontName = 'Montserrat';
            app.Phase2ampuAEditField.Position = [162 394 105 22];
            app.Phase2ampuAEditField.Value = 100;

            % Create Phase1ampuAEditField
            app.Phase1ampuAEditField = uieditfield(app.LeftPanel, 'numeric');
            app.Phase1ampuAEditField.Limits = [100 10000];
            app.Phase1ampuAEditField.ValueDisplayFormat = '%11.4g uA';
            app.Phase1ampuAEditField.ValueChangedFcn = createCallbackFcn(app, @Phase1ampuAEditFieldValueChanged, true);
            app.Phase1ampuAEditField.FontName = 'Montserrat';
            app.Phase1ampuAEditField.Position = [162 427 105 22];
            app.Phase1ampuAEditField.Value = 200;

            % Create Phase1delaymsEditField
            app.Phase1delaymsEditField = uieditfield(app.LeftPanel, 'numeric');
            app.Phase1delaymsEditField.Limits = [0 99];
            app.Phase1delaymsEditField.ValueDisplayFormat = '%11.4g ms';
            app.Phase1delaymsEditField.ValueChangedFcn = createCallbackFcn(app, @Phase1delaymsEditFieldValueChanged, true);
            app.Phase1delaymsEditField.FontName = 'Montserrat';
            app.Phase1delaymsEditField.Visible = 'off';
            app.Phase1delaymsEditField.Position = [162 458 105 22];

            % Create PolarityButtonGroup
            app.PolarityButtonGroup = uibuttongroup(app.LeftPanel);
            app.PolarityButtonGroup.SelectionChangedFcn = createCallbackFcn(app, @PolarityButtonGroupSelectionChanged, true);
            app.PolarityButtonGroup.Title = 'Polarity';
            app.PolarityButtonGroup.Visible = 'off';
            app.PolarityButtonGroup.BackgroundColor = [1 0.9686 0.902];
            app.PolarityButtonGroup.FontName = 'Montserrat';
            app.PolarityButtonGroup.Position = [28 432 223 54];

            % Create AlternatingButton
            app.AlternatingButton = uiradiobutton(app.PolarityButtonGroup);
            app.AlternatingButton.Text = 'Alternating';
            app.AlternatingButton.FontName = 'Montserrat';
            app.AlternatingButton.Position = [11 7 94 22];
            app.AlternatingButton.Value = true;

            % Create SteadyButton
            app.SteadyButton = uiradiobutton(app.PolarityButtonGroup);
            app.SteadyButton.Text = 'Steady';
            app.SteadyButton.FontName = 'Montserrat';
            app.SteadyButton.Position = [126 7 83 22];

            % Create NumberofpulsesSpinner
            app.NumberofpulsesSpinner = uispinner(app.LeftPanel);
            app.NumberofpulsesSpinner.Limits = [1 255];
            app.NumberofpulsesSpinner.ValueChangedFcn = createCallbackFcn(app, @NumberofpulsesSpinnerValueChanged, true);
            app.NumberofpulsesSpinner.FontName = 'Montserrat';
            app.NumberofpulsesSpinner.Position = [185 458 59 22];
            app.NumberofpulsesSpinner.Value = 10;

            % Create OrderButtonGroup
            app.OrderButtonGroup = uibuttongroup(app.LeftPanel);
            app.OrderButtonGroup.SelectionChangedFcn = createCallbackFcn(app, @OrderButtonGroupSelectionChanged, true);
            app.OrderButtonGroup.Title = 'Order';
            app.OrderButtonGroup.BackgroundColor = [1 0.9686 0.902];
            app.OrderButtonGroup.FontName = 'Montserrat';
            app.OrderButtonGroup.Position = [28 494 222 54];

            % Create CathodeFirstButton
            app.CathodeFirstButton = uiradiobutton(app.OrderButtonGroup);
            app.CathodeFirstButton.Text = 'Cathode First';
            app.CathodeFirstButton.FontName = 'Montserrat';
            app.CathodeFirstButton.Position = [11 7 103 22];
            app.CathodeFirstButton.Value = true;

            % Create AnodeFirstButton
            app.AnodeFirstButton = uiradiobutton(app.OrderButtonGroup);
            app.AnodeFirstButton.Text = 'Anode First';
            app.AnodeFirstButton.FontName = 'Montserrat';
            app.AnodeFirstButton.Position = [114 7 91 22];

            % Create RecordingChannelsButtonGroup
            app.RecordingChannelsButtonGroup = uibuttongroup(app.LeftPanel);
            app.RecordingChannelsButtonGroup.SelectionChangedFcn = createCallbackFcn(app, @RecordingChannelsButtonGroupSelectionChanged, true);
            app.RecordingChannelsButtonGroup.BorderType = 'none';
            app.RecordingChannelsButtonGroup.TitlePosition = 'centertop';
            app.RecordingChannelsButtonGroup.Title = 'Recording Channels';
            app.RecordingChannelsButtonGroup.BackgroundColor = [1 0.9686 0.902];
            app.RecordingChannelsButtonGroup.FontName = 'Montserrat';
            app.RecordingChannelsButtonGroup.Position = [6 487 273 65];

            % Create RAWButton
            app.RAWButton = uiradiobutton(app.RecordingChannelsButtonGroup);
            app.RAWButton.Text = 'RAW';
            app.RAWButton.Position = [11 20 58 22];

            % Create ECOGHFButton
            app.ECOGHFButton = uiradiobutton(app.RecordingChannelsButtonGroup);
            app.ECOGHFButton.Text = 'ECOG HF';
            app.ECOGHFButton.Position = [92 20 77 22];
            app.ECOGHFButton.Value = true;

            % Create SPKButton
            app.SPKButton = uiradiobutton(app.RecordingChannelsButtonGroup);
            app.SPKButton.Text = 'SPK';
            app.SPKButton.Position = [192 20 65 22];

            % Create SEGButton
            app.SEGButton = uiradiobutton(app.RecordingChannelsButtonGroup);
            app.SEGButton.Text = 'SEG';
            app.SEGButton.Position = [11 -1 65 22];

            % Create ECOGLFButton
            app.ECOGLFButton = uiradiobutton(app.RecordingChannelsButtonGroup);
            app.ECOGLFButton.Text = 'ECOG LF';
            app.ECOGLFButton.Position = [92 -1 75 22];

            % Create LFPButton
            app.LFPButton = uiradiobutton(app.RecordingChannelsButtonGroup);
            app.LFPButton.Text = 'LFP';
            app.LFPButton.Position = [192 -1 58 22];

            % Create NeurostimulatorDropDown
            app.NeurostimulatorDropDown = uidropdown(app.LeftPanel);
            app.NeurostimulatorDropDown.Items = {'CereStim', 'NeuroOmega', 'g.Estim', 'Cortec BIC'};
            app.NeurostimulatorDropDown.ValueChangedFcn = createCallbackFcn(app, @NeurostimulatorDropDownValueChanged, true);
            app.NeurostimulatorDropDown.FontName = 'Montserrat ExtraBold';
            app.NeurostimulatorDropDown.BackgroundColor = [1 0.9333 0.6902];
            app.NeurostimulatorDropDown.Position = [130 555 131 22];
            app.NeurostimulatorDropDown.Value = 'CereStim';

            % Create StimulationConfigurationLabel
            app.StimulationConfigurationLabel = uilabel(app.LeftPanel);
            app.StimulationConfigurationLabel.FontName = 'Montserrat';
            app.StimulationConfigurationLabel.FontWeight = 'bold';
            app.StimulationConfigurationLabel.Position = [68 581 170 22];
            app.StimulationConfigurationLabel.Text = 'Stimulation Configuration';

            % Create CenterPanel
            app.CenterPanel = uipanel(app.GridLayout);
            app.CenterPanel.BackgroundColor = [0.949 0.9804 1];
            app.CenterPanel.Layout.Row = 1;
            app.CenterPanel.Layout.Column = 2;
            app.CenterPanel.FontName = 'Montserrat';

            % Create SinglePulseAxes
            app.SinglePulseAxes = uiaxes(app.CenterPanel);
            title(app.SinglePulseAxes, 'Single pulse')
            xlabel(app.SinglePulseAxes, 'Time (us)')
            ylabel(app.SinglePulseAxes, 'Current (uA)')
            app.SinglePulseAxes.FontName = 'Montserrat';
            app.SinglePulseAxes.Colormap = [0.2431 0.149 0.6588;0.2431 0.1529 0.6745;0.2471 0.1569 0.6863;0.2471 0.1608 0.698;0.251 0.1647 0.7059;0.251 0.1686 0.7176;0.2549 0.1725 0.7294;0.2549 0.1765 0.7412;0.2588 0.1804 0.749;0.2588 0.1843 0.7608;0.2627 0.1882 0.7725;0.2627 0.1922 0.7843;0.2627 0.1961 0.7922;0.2667 0.2 0.8039;0.2667 0.2039 0.8157;0.2706 0.2078 0.8235;0.2706 0.2157 0.8353;0.2706 0.2196 0.8431;0.2745 0.2235 0.851;0.2745 0.2275 0.8627;0.2745 0.2314 0.8706;0.2745 0.2392 0.8784;0.2784 0.2431 0.8824;0.2784 0.2471 0.8902;0.2784 0.2549 0.898;0.2784 0.2588 0.902;0.2784 0.2667 0.9098;0.2784 0.2706 0.9137;0.2784 0.2745 0.9216;0.2824 0.2824 0.9255;0.2824 0.2863 0.9294;0.2824 0.2941 0.9333;0.2824 0.298 0.9412;0.2824 0.3059 0.9451;0.2824 0.3098 0.949;0.2824 0.3137 0.9529;0.2824 0.3216 0.9569;0.2824 0.3255 0.9608;0.2824 0.3294 0.9647;0.2784 0.3373 0.9686;0.2784 0.3412 0.9686;0.2784 0.349 0.9725;0.2784 0.3529 0.9765;0.2784 0.3569 0.9804;0.2784 0.3647 0.9804;0.2745 0.3686 0.9843;0.2745 0.3765 0.9843;0.2745 0.3804 0.9882;0.2706 0.3843 0.9882;0.2706 0.3922 0.9922;0.2667 0.3961 0.9922;0.2627 0.4039 0.9922;0.2627 0.4078 0.9961;0.2588 0.4157 0.9961;0.2549 0.4196 0.9961;0.251 0.4275 0.9961;0.2471 0.4314 1;0.2431 0.4392 1;0.2353 0.4431 1;0.2314 0.451 1;0.2235 0.4549 1;0.2196 0.4627 0.9961;0.2118 0.4667 0.9961;0.2078 0.4745 0.9922;0.2 0.4784 0.9922;0.1961 0.4863 0.9882;0.1922 0.4902 0.9882;0.1882 0.498 0.9843;0.1843 0.502 0.9804;0.1843 0.5098 0.9804;0.1804 0.5137 0.9765;0.1804 0.5176 0.9725;0.1804 0.5255 0.9725;0.1804 0.5294 0.9686;0.1765 0.5333 0.9647;0.1765 0.5412 0.9608;0.1765 0.5451 0.9569;0.1765 0.549 0.9529;0.1765 0.5569 0.949;0.1725 0.5608 0.9451;0.1725 0.5647 0.9412;0.1686 0.5686 0.9373;0.1647 0.5765 0.9333;0.1608 0.5804 0.9294;0.1569 0.5843 0.9255;0.1529 0.5922 0.9216;0.1529 0.5961 0.9176;0.149 0.6 0.9137;0.149 0.6039 0.9098;0.1451 0.6078 0.9098;0.1451 0.6118 0.9059;0.1412 0.6196 0.902;0.1412 0.6235 0.898;0.1373 0.6275 0.898;0.1373 0.6314 0.8941;0.1333 0.6353 0.8941;0.1294 0.6392 0.8902;0.1255 0.6471 0.8902;0.1216 0.651 0.8863;0.1176 0.6549 0.8824;0.1137 0.6588 0.8824;0.1137 0.6627 0.8784;0.1098 0.6667 0.8745;0.1059 0.6706 0.8706;0.102 0.6745 0.8667;0.098 0.6784 0.8627;0.0902 0.6824 0.8549;0.0863 0.6863 0.851;0.0784 0.6902 0.8471;0.0706 0.6941 0.8392;0.0627 0.698 0.8353;0.0549 0.702 0.8314;0.0431 0.702 0.8235;0.0314 0.7059 0.8196;0.0235 0.7098 0.8118;0.0157 0.7137 0.8078;0.0078 0.7176 0.8;0.0039 0.7176 0.7922;0 0.7216 0.7882;0 0.7255 0.7804;0 0.7294 0.7765;0.0039 0.7294 0.7686;0.0078 0.7333 0.7608;0.0157 0.7333 0.7569;0.0235 0.7373 0.749;0.0353 0.7412 0.7412;0.051 0.7412 0.7373;0.0627 0.7451 0.7294;0.0784 0.7451 0.7216;0.0902 0.749 0.7137;0.102 0.7529 0.7098;0.1137 0.7529 0.702;0.1255 0.7569 0.6941;0.1373 0.7569 0.6863;0.1451 0.7608 0.6824;0.1529 0.7608 0.6745;0.1608 0.7647 0.6667;0.1686 0.7647 0.6588;0.1725 0.7686 0.651;0.1804 0.7686 0.6471;0.1843 0.7725 0.6392;0.1922 0.7725 0.6314;0.1961 0.7765 0.6235;0.2 0.7804 0.6157;0.2078 0.7804 0.6078;0.2118 0.7843 0.6;0.2196 0.7843 0.5882;0.2235 0.7882 0.5804;0.2314 0.7882 0.5725;0.2392 0.7922 0.5647;0.251 0.7922 0.5529;0.2588 0.7922 0.5451;0.2706 0.7961 0.5373;0.2824 0.7961 0.5255;0.2941 0.7961 0.5176;0.3059 0.8 0.5059;0.3176 0.8 0.498;0.3294 0.8 0.4863;0.3412 0.8 0.4784;0.3529 0.8 0.4667;0.3686 0.8039 0.4549;0.3804 0.8039 0.4471;0.3922 0.8039 0.4353;0.4039 0.8039 0.4235;0.4196 0.8039 0.4118;0.4314 0.8039 0.4;0.4471 0.8039 0.3922;0.4627 0.8 0.3804;0.4745 0.8 0.3686;0.4902 0.8 0.3569;0.5059 0.8 0.349;0.5176 0.8 0.3373;0.5333 0.7961 0.3255;0.5451 0.7961 0.3176;0.5608 0.7961 0.3059;0.5765 0.7922 0.2941;0.5882 0.7922 0.2824;0.6039 0.7882 0.2745;0.6157 0.7882 0.2627;0.6314 0.7843 0.251;0.6431 0.7843 0.2431;0.6549 0.7804 0.2314;0.6706 0.7804 0.2235;0.6824 0.7765 0.2157;0.698 0.7765 0.2078;0.7098 0.7725 0.2;0.7216 0.7686 0.1922;0.7333 0.7686 0.1843;0.7451 0.7647 0.1765;0.7608 0.7647 0.1725;0.7725 0.7608 0.1647;0.7843 0.7569 0.1608;0.7961 0.7569 0.1569;0.8078 0.7529 0.1529;0.8157 0.749 0.1529;0.8275 0.749 0.1529;0.8392 0.7451 0.1529;0.851 0.7451 0.1569;0.8588 0.7412 0.1569;0.8706 0.7373 0.1608;0.8824 0.7373 0.1647;0.8902 0.7373 0.1686;0.902 0.7333 0.1765;0.9098 0.7333 0.1804;0.9176 0.7294 0.1882;0.9255 0.7294 0.1961;0.9373 0.7294 0.2078;0.9451 0.7294 0.2157;0.9529 0.7294 0.2235;0.9608 0.7294 0.2314;0.9686 0.7294 0.2392;0.9765 0.7294 0.2431;0.9843 0.7333 0.2431;0.9882 0.7373 0.2431;0.9961 0.7412 0.2392;0.9961 0.7451 0.2353;0.9961 0.7529 0.2314;0.9961 0.7569 0.2275;0.9961 0.7608 0.2235;0.9961 0.7686 0.2196;0.9961 0.7725 0.2157;0.9961 0.7804 0.2078;0.9961 0.7843 0.2039;0.9961 0.7922 0.2;0.9922 0.7961 0.1961;0.9922 0.8039 0.1922;0.9922 0.8078 0.1922;0.9882 0.8157 0.1882;0.9843 0.8235 0.1843;0.9843 0.8275 0.1804;0.9804 0.8353 0.1804;0.9765 0.8392 0.1765;0.9765 0.8471 0.1725;0.9725 0.851 0.1686;0.9686 0.8588 0.1647;0.9686 0.8667 0.1647;0.9647 0.8706 0.1608;0.9647 0.8784 0.1569;0.9608 0.8824 0.1569;0.9608 0.8902 0.1529;0.9608 0.898 0.149;0.9608 0.902 0.149;0.9608 0.9098 0.1451;0.9608 0.9137 0.1412;0.9608 0.9216 0.1373;0.9608 0.9255 0.1333;0.9608 0.9333 0.1294;0.9647 0.9373 0.1255;0.9647 0.9451 0.1216;0.9647 0.949 0.1176;0.9686 0.9569 0.1098;0.9686 0.9608 0.1059;0.9725 0.9686 0.102;0.9725 0.9725 0.0941;0.9765 0.9765 0.0863;0.9765 0.9843 0.0824];
            app.SinglePulseAxes.LineWidth = 1;
            app.SinglePulseAxes.ColorOrder = [0.502 0.6902 0.8314;0.9804 0.502 0.451;0.549 0.8314 0.7804;0.9882 0.7098 0.3804;0.702 0.8706 0.4118;0.749 0.7294 0.851;0.502 0.502 0.502];
            app.SinglePulseAxes.GridColor = [0 1 0];
            app.SinglePulseAxes.MinorGridColor = [0.1294 0.1216 0.1216];
            app.SinglePulseAxes.Box = 'on';
            app.SinglePulseAxes.Position = [3 245 233 250];

            % Create MultPulsesAxes
            app.MultPulsesAxes = uiaxes(app.CenterPanel);
            title(app.MultPulsesAxes, 'Burst of pulses')
            xlabel(app.MultPulsesAxes, 'Time (us)')
            ylabel(app.MultPulsesAxes, 'Current (uA)')
            app.MultPulsesAxes.FontName = 'Montserrat';
            app.MultPulsesAxes.Colormap = [0.2431 0.149 0.6588;0.2431 0.1529 0.6745;0.2471 0.1569 0.6863;0.2471 0.1608 0.698;0.251 0.1647 0.7059;0.251 0.1686 0.7176;0.2549 0.1725 0.7294;0.2549 0.1765 0.7412;0.2588 0.1804 0.749;0.2588 0.1843 0.7608;0.2627 0.1882 0.7725;0.2627 0.1922 0.7843;0.2627 0.1961 0.7922;0.2667 0.2 0.8039;0.2667 0.2039 0.8157;0.2706 0.2078 0.8235;0.2706 0.2157 0.8353;0.2706 0.2196 0.8431;0.2745 0.2235 0.851;0.2745 0.2275 0.8627;0.2745 0.2314 0.8706;0.2745 0.2392 0.8784;0.2784 0.2431 0.8824;0.2784 0.2471 0.8902;0.2784 0.2549 0.898;0.2784 0.2588 0.902;0.2784 0.2667 0.9098;0.2784 0.2706 0.9137;0.2784 0.2745 0.9216;0.2824 0.2824 0.9255;0.2824 0.2863 0.9294;0.2824 0.2941 0.9333;0.2824 0.298 0.9412;0.2824 0.3059 0.9451;0.2824 0.3098 0.949;0.2824 0.3137 0.9529;0.2824 0.3216 0.9569;0.2824 0.3255 0.9608;0.2824 0.3294 0.9647;0.2784 0.3373 0.9686;0.2784 0.3412 0.9686;0.2784 0.349 0.9725;0.2784 0.3529 0.9765;0.2784 0.3569 0.9804;0.2784 0.3647 0.9804;0.2745 0.3686 0.9843;0.2745 0.3765 0.9843;0.2745 0.3804 0.9882;0.2706 0.3843 0.9882;0.2706 0.3922 0.9922;0.2667 0.3961 0.9922;0.2627 0.4039 0.9922;0.2627 0.4078 0.9961;0.2588 0.4157 0.9961;0.2549 0.4196 0.9961;0.251 0.4275 0.9961;0.2471 0.4314 1;0.2431 0.4392 1;0.2353 0.4431 1;0.2314 0.451 1;0.2235 0.4549 1;0.2196 0.4627 0.9961;0.2118 0.4667 0.9961;0.2078 0.4745 0.9922;0.2 0.4784 0.9922;0.1961 0.4863 0.9882;0.1922 0.4902 0.9882;0.1882 0.498 0.9843;0.1843 0.502 0.9804;0.1843 0.5098 0.9804;0.1804 0.5137 0.9765;0.1804 0.5176 0.9725;0.1804 0.5255 0.9725;0.1804 0.5294 0.9686;0.1765 0.5333 0.9647;0.1765 0.5412 0.9608;0.1765 0.5451 0.9569;0.1765 0.549 0.9529;0.1765 0.5569 0.949;0.1725 0.5608 0.9451;0.1725 0.5647 0.9412;0.1686 0.5686 0.9373;0.1647 0.5765 0.9333;0.1608 0.5804 0.9294;0.1569 0.5843 0.9255;0.1529 0.5922 0.9216;0.1529 0.5961 0.9176;0.149 0.6 0.9137;0.149 0.6039 0.9098;0.1451 0.6078 0.9098;0.1451 0.6118 0.9059;0.1412 0.6196 0.902;0.1412 0.6235 0.898;0.1373 0.6275 0.898;0.1373 0.6314 0.8941;0.1333 0.6353 0.8941;0.1294 0.6392 0.8902;0.1255 0.6471 0.8902;0.1216 0.651 0.8863;0.1176 0.6549 0.8824;0.1137 0.6588 0.8824;0.1137 0.6627 0.8784;0.1098 0.6667 0.8745;0.1059 0.6706 0.8706;0.102 0.6745 0.8667;0.098 0.6784 0.8627;0.0902 0.6824 0.8549;0.0863 0.6863 0.851;0.0784 0.6902 0.8471;0.0706 0.6941 0.8392;0.0627 0.698 0.8353;0.0549 0.702 0.8314;0.0431 0.702 0.8235;0.0314 0.7059 0.8196;0.0235 0.7098 0.8118;0.0157 0.7137 0.8078;0.0078 0.7176 0.8;0.0039 0.7176 0.7922;0 0.7216 0.7882;0 0.7255 0.7804;0 0.7294 0.7765;0.0039 0.7294 0.7686;0.0078 0.7333 0.7608;0.0157 0.7333 0.7569;0.0235 0.7373 0.749;0.0353 0.7412 0.7412;0.051 0.7412 0.7373;0.0627 0.7451 0.7294;0.0784 0.7451 0.7216;0.0902 0.749 0.7137;0.102 0.7529 0.7098;0.1137 0.7529 0.702;0.1255 0.7569 0.6941;0.1373 0.7569 0.6863;0.1451 0.7608 0.6824;0.1529 0.7608 0.6745;0.1608 0.7647 0.6667;0.1686 0.7647 0.6588;0.1725 0.7686 0.651;0.1804 0.7686 0.6471;0.1843 0.7725 0.6392;0.1922 0.7725 0.6314;0.1961 0.7765 0.6235;0.2 0.7804 0.6157;0.2078 0.7804 0.6078;0.2118 0.7843 0.6;0.2196 0.7843 0.5882;0.2235 0.7882 0.5804;0.2314 0.7882 0.5725;0.2392 0.7922 0.5647;0.251 0.7922 0.5529;0.2588 0.7922 0.5451;0.2706 0.7961 0.5373;0.2824 0.7961 0.5255;0.2941 0.7961 0.5176;0.3059 0.8 0.5059;0.3176 0.8 0.498;0.3294 0.8 0.4863;0.3412 0.8 0.4784;0.3529 0.8 0.4667;0.3686 0.8039 0.4549;0.3804 0.8039 0.4471;0.3922 0.8039 0.4353;0.4039 0.8039 0.4235;0.4196 0.8039 0.4118;0.4314 0.8039 0.4;0.4471 0.8039 0.3922;0.4627 0.8 0.3804;0.4745 0.8 0.3686;0.4902 0.8 0.3569;0.5059 0.8 0.349;0.5176 0.8 0.3373;0.5333 0.7961 0.3255;0.5451 0.7961 0.3176;0.5608 0.7961 0.3059;0.5765 0.7922 0.2941;0.5882 0.7922 0.2824;0.6039 0.7882 0.2745;0.6157 0.7882 0.2627;0.6314 0.7843 0.251;0.6431 0.7843 0.2431;0.6549 0.7804 0.2314;0.6706 0.7804 0.2235;0.6824 0.7765 0.2157;0.698 0.7765 0.2078;0.7098 0.7725 0.2;0.7216 0.7686 0.1922;0.7333 0.7686 0.1843;0.7451 0.7647 0.1765;0.7608 0.7647 0.1725;0.7725 0.7608 0.1647;0.7843 0.7569 0.1608;0.7961 0.7569 0.1569;0.8078 0.7529 0.1529;0.8157 0.749 0.1529;0.8275 0.749 0.1529;0.8392 0.7451 0.1529;0.851 0.7451 0.1569;0.8588 0.7412 0.1569;0.8706 0.7373 0.1608;0.8824 0.7373 0.1647;0.8902 0.7373 0.1686;0.902 0.7333 0.1765;0.9098 0.7333 0.1804;0.9176 0.7294 0.1882;0.9255 0.7294 0.1961;0.9373 0.7294 0.2078;0.9451 0.7294 0.2157;0.9529 0.7294 0.2235;0.9608 0.7294 0.2314;0.9686 0.7294 0.2392;0.9765 0.7294 0.2431;0.9843 0.7333 0.2431;0.9882 0.7373 0.2431;0.9961 0.7412 0.2392;0.9961 0.7451 0.2353;0.9961 0.7529 0.2314;0.9961 0.7569 0.2275;0.9961 0.7608 0.2235;0.9961 0.7686 0.2196;0.9961 0.7725 0.2157;0.9961 0.7804 0.2078;0.9961 0.7843 0.2039;0.9961 0.7922 0.2;0.9922 0.7961 0.1961;0.9922 0.8039 0.1922;0.9922 0.8078 0.1922;0.9882 0.8157 0.1882;0.9843 0.8235 0.1843;0.9843 0.8275 0.1804;0.9804 0.8353 0.1804;0.9765 0.8392 0.1765;0.9765 0.8471 0.1725;0.9725 0.851 0.1686;0.9686 0.8588 0.1647;0.9686 0.8667 0.1647;0.9647 0.8706 0.1608;0.9647 0.8784 0.1569;0.9608 0.8824 0.1569;0.9608 0.8902 0.1529;0.9608 0.898 0.149;0.9608 0.902 0.149;0.9608 0.9098 0.1451;0.9608 0.9137 0.1412;0.9608 0.9216 0.1373;0.9608 0.9255 0.1333;0.9608 0.9333 0.1294;0.9647 0.9373 0.1255;0.9647 0.9451 0.1216;0.9647 0.949 0.1176;0.9686 0.9569 0.1098;0.9686 0.9608 0.1059;0.9725 0.9686 0.102;0.9725 0.9725 0.0941;0.9765 0.9765 0.0863;0.9765 0.9843 0.0824];
            app.MultPulsesAxes.LineWidth = 1;
            app.MultPulsesAxes.ColorOrder = [0.502 0.6902 0.8314;0.9804 0.502 0.451;0.549 0.8314 0.7804;0.9882 0.7098 0.3804;0.702 0.8706 0.4118;0.749 0.7294 0.851;0.502 0.502 0.502];
            app.MultPulsesAxes.Box = 'on';
            app.MultPulsesAxes.Position = [248 243 231 250];

            % Create ChannelAxes
            app.ChannelAxes = uiaxes(app.CenterPanel);
            title(app.ChannelAxes, 'Channels')
            zlabel(app.ChannelAxes, 'Z')
            app.ChannelAxes.FontName = 'Montserrat';
            app.ChannelAxes.Colormap = [0.2431 0.149 0.6588;0.2431 0.1529 0.6745;0.2471 0.1569 0.6863;0.2471 0.1608 0.698;0.251 0.1647 0.7059;0.251 0.1686 0.7176;0.2549 0.1725 0.7294;0.2549 0.1765 0.7412;0.2588 0.1804 0.749;0.2588 0.1843 0.7608;0.2627 0.1882 0.7725;0.2627 0.1922 0.7843;0.2627 0.1961 0.7922;0.2667 0.2 0.8039;0.2667 0.2039 0.8157;0.2706 0.2078 0.8235;0.2706 0.2157 0.8353;0.2706 0.2196 0.8431;0.2745 0.2235 0.851;0.2745 0.2275 0.8627;0.2745 0.2314 0.8706;0.2745 0.2392 0.8784;0.2784 0.2431 0.8824;0.2784 0.2471 0.8902;0.2784 0.2549 0.898;0.2784 0.2588 0.902;0.2784 0.2667 0.9098;0.2784 0.2706 0.9137;0.2784 0.2745 0.9216;0.2824 0.2824 0.9255;0.2824 0.2863 0.9294;0.2824 0.2941 0.9333;0.2824 0.298 0.9412;0.2824 0.3059 0.9451;0.2824 0.3098 0.949;0.2824 0.3137 0.9529;0.2824 0.3216 0.9569;0.2824 0.3255 0.9608;0.2824 0.3294 0.9647;0.2784 0.3373 0.9686;0.2784 0.3412 0.9686;0.2784 0.349 0.9725;0.2784 0.3529 0.9765;0.2784 0.3569 0.9804;0.2784 0.3647 0.9804;0.2745 0.3686 0.9843;0.2745 0.3765 0.9843;0.2745 0.3804 0.9882;0.2706 0.3843 0.9882;0.2706 0.3922 0.9922;0.2667 0.3961 0.9922;0.2627 0.4039 0.9922;0.2627 0.4078 0.9961;0.2588 0.4157 0.9961;0.2549 0.4196 0.9961;0.251 0.4275 0.9961;0.2471 0.4314 1;0.2431 0.4392 1;0.2353 0.4431 1;0.2314 0.451 1;0.2235 0.4549 1;0.2196 0.4627 0.9961;0.2118 0.4667 0.9961;0.2078 0.4745 0.9922;0.2 0.4784 0.9922;0.1961 0.4863 0.9882;0.1922 0.4902 0.9882;0.1882 0.498 0.9843;0.1843 0.502 0.9804;0.1843 0.5098 0.9804;0.1804 0.5137 0.9765;0.1804 0.5176 0.9725;0.1804 0.5255 0.9725;0.1804 0.5294 0.9686;0.1765 0.5333 0.9647;0.1765 0.5412 0.9608;0.1765 0.5451 0.9569;0.1765 0.549 0.9529;0.1765 0.5569 0.949;0.1725 0.5608 0.9451;0.1725 0.5647 0.9412;0.1686 0.5686 0.9373;0.1647 0.5765 0.9333;0.1608 0.5804 0.9294;0.1569 0.5843 0.9255;0.1529 0.5922 0.9216;0.1529 0.5961 0.9176;0.149 0.6 0.9137;0.149 0.6039 0.9098;0.1451 0.6078 0.9098;0.1451 0.6118 0.9059;0.1412 0.6196 0.902;0.1412 0.6235 0.898;0.1373 0.6275 0.898;0.1373 0.6314 0.8941;0.1333 0.6353 0.8941;0.1294 0.6392 0.8902;0.1255 0.6471 0.8902;0.1216 0.651 0.8863;0.1176 0.6549 0.8824;0.1137 0.6588 0.8824;0.1137 0.6627 0.8784;0.1098 0.6667 0.8745;0.1059 0.6706 0.8706;0.102 0.6745 0.8667;0.098 0.6784 0.8627;0.0902 0.6824 0.8549;0.0863 0.6863 0.851;0.0784 0.6902 0.8471;0.0706 0.6941 0.8392;0.0627 0.698 0.8353;0.0549 0.702 0.8314;0.0431 0.702 0.8235;0.0314 0.7059 0.8196;0.0235 0.7098 0.8118;0.0157 0.7137 0.8078;0.0078 0.7176 0.8;0.0039 0.7176 0.7922;0 0.7216 0.7882;0 0.7255 0.7804;0 0.7294 0.7765;0.0039 0.7294 0.7686;0.0078 0.7333 0.7608;0.0157 0.7333 0.7569;0.0235 0.7373 0.749;0.0353 0.7412 0.7412;0.051 0.7412 0.7373;0.0627 0.7451 0.7294;0.0784 0.7451 0.7216;0.0902 0.749 0.7137;0.102 0.7529 0.7098;0.1137 0.7529 0.702;0.1255 0.7569 0.6941;0.1373 0.7569 0.6863;0.1451 0.7608 0.6824;0.1529 0.7608 0.6745;0.1608 0.7647 0.6667;0.1686 0.7647 0.6588;0.1725 0.7686 0.651;0.1804 0.7686 0.6471;0.1843 0.7725 0.6392;0.1922 0.7725 0.6314;0.1961 0.7765 0.6235;0.2 0.7804 0.6157;0.2078 0.7804 0.6078;0.2118 0.7843 0.6;0.2196 0.7843 0.5882;0.2235 0.7882 0.5804;0.2314 0.7882 0.5725;0.2392 0.7922 0.5647;0.251 0.7922 0.5529;0.2588 0.7922 0.5451;0.2706 0.7961 0.5373;0.2824 0.7961 0.5255;0.2941 0.7961 0.5176;0.3059 0.8 0.5059;0.3176 0.8 0.498;0.3294 0.8 0.4863;0.3412 0.8 0.4784;0.3529 0.8 0.4667;0.3686 0.8039 0.4549;0.3804 0.8039 0.4471;0.3922 0.8039 0.4353;0.4039 0.8039 0.4235;0.4196 0.8039 0.4118;0.4314 0.8039 0.4;0.4471 0.8039 0.3922;0.4627 0.8 0.3804;0.4745 0.8 0.3686;0.4902 0.8 0.3569;0.5059 0.8 0.349;0.5176 0.8 0.3373;0.5333 0.7961 0.3255;0.5451 0.7961 0.3176;0.5608 0.7961 0.3059;0.5765 0.7922 0.2941;0.5882 0.7922 0.2824;0.6039 0.7882 0.2745;0.6157 0.7882 0.2627;0.6314 0.7843 0.251;0.6431 0.7843 0.2431;0.6549 0.7804 0.2314;0.6706 0.7804 0.2235;0.6824 0.7765 0.2157;0.698 0.7765 0.2078;0.7098 0.7725 0.2;0.7216 0.7686 0.1922;0.7333 0.7686 0.1843;0.7451 0.7647 0.1765;0.7608 0.7647 0.1725;0.7725 0.7608 0.1647;0.7843 0.7569 0.1608;0.7961 0.7569 0.1569;0.8078 0.7529 0.1529;0.8157 0.749 0.1529;0.8275 0.749 0.1529;0.8392 0.7451 0.1529;0.851 0.7451 0.1569;0.8588 0.7412 0.1569;0.8706 0.7373 0.1608;0.8824 0.7373 0.1647;0.8902 0.7373 0.1686;0.902 0.7333 0.1765;0.9098 0.7333 0.1804;0.9176 0.7294 0.1882;0.9255 0.7294 0.1961;0.9373 0.7294 0.2078;0.9451 0.7294 0.2157;0.9529 0.7294 0.2235;0.9608 0.7294 0.2314;0.9686 0.7294 0.2392;0.9765 0.7294 0.2431;0.9843 0.7333 0.2431;0.9882 0.7373 0.2431;0.9961 0.7412 0.2392;0.9961 0.7451 0.2353;0.9961 0.7529 0.2314;0.9961 0.7569 0.2275;0.9961 0.7608 0.2235;0.9961 0.7686 0.2196;0.9961 0.7725 0.2157;0.9961 0.7804 0.2078;0.9961 0.7843 0.2039;0.9961 0.7922 0.2;0.9922 0.7961 0.1961;0.9922 0.8039 0.1922;0.9922 0.8078 0.1922;0.9882 0.8157 0.1882;0.9843 0.8235 0.1843;0.9843 0.8275 0.1804;0.9804 0.8353 0.1804;0.9765 0.8392 0.1765;0.9765 0.8471 0.1725;0.9725 0.851 0.1686;0.9686 0.8588 0.1647;0.9686 0.8667 0.1647;0.9647 0.8706 0.1608;0.9647 0.8784 0.1569;0.9608 0.8824 0.1569;0.9608 0.8902 0.1529;0.9608 0.898 0.149;0.9608 0.902 0.149;0.9608 0.9098 0.1451;0.9608 0.9137 0.1412;0.9608 0.9216 0.1373;0.9608 0.9255 0.1333;0.9608 0.9333 0.1294;0.9647 0.9373 0.1255;0.9647 0.9451 0.1216;0.9647 0.949 0.1176;0.9686 0.9569 0.1098;0.9686 0.9608 0.1059;0.9725 0.9686 0.102;0.9725 0.9725 0.0941;0.9765 0.9765 0.0863;0.9765 0.9843 0.0824];
            app.ChannelAxes.Color = [0.949 0.9804 1];
            app.ChannelAxes.TitleHorizontalAlignment = 'right';
            app.ChannelAxes.GridColor = [1 1 1];
            app.ChannelAxes.Box = 'on';
            app.ChannelAxes.Position = [4 1 476 114];

            % Create TrainAxes
            app.TrainAxes = uiaxes(app.CenterPanel);
            title(app.TrainAxes, 'Complete stimulation train')
            xlabel(app.TrainAxes, 'Time (s)')
            ylabel(app.TrainAxes, 'On / Off')
            app.TrainAxes.FontName = 'Montserrat';
            app.TrainAxes.Colormap = [0.2431 0.149 0.6588;0.2431 0.1529 0.6745;0.2471 0.1569 0.6863;0.2471 0.1608 0.698;0.251 0.1647 0.7059;0.251 0.1686 0.7176;0.2549 0.1725 0.7294;0.2549 0.1765 0.7412;0.2588 0.1804 0.749;0.2588 0.1843 0.7608;0.2627 0.1882 0.7725;0.2627 0.1922 0.7843;0.2627 0.1961 0.7922;0.2667 0.2 0.8039;0.2667 0.2039 0.8157;0.2706 0.2078 0.8235;0.2706 0.2157 0.8353;0.2706 0.2196 0.8431;0.2745 0.2235 0.851;0.2745 0.2275 0.8627;0.2745 0.2314 0.8706;0.2745 0.2392 0.8784;0.2784 0.2431 0.8824;0.2784 0.2471 0.8902;0.2784 0.2549 0.898;0.2784 0.2588 0.902;0.2784 0.2667 0.9098;0.2784 0.2706 0.9137;0.2784 0.2745 0.9216;0.2824 0.2824 0.9255;0.2824 0.2863 0.9294;0.2824 0.2941 0.9333;0.2824 0.298 0.9412;0.2824 0.3059 0.9451;0.2824 0.3098 0.949;0.2824 0.3137 0.9529;0.2824 0.3216 0.9569;0.2824 0.3255 0.9608;0.2824 0.3294 0.9647;0.2784 0.3373 0.9686;0.2784 0.3412 0.9686;0.2784 0.349 0.9725;0.2784 0.3529 0.9765;0.2784 0.3569 0.9804;0.2784 0.3647 0.9804;0.2745 0.3686 0.9843;0.2745 0.3765 0.9843;0.2745 0.3804 0.9882;0.2706 0.3843 0.9882;0.2706 0.3922 0.9922;0.2667 0.3961 0.9922;0.2627 0.4039 0.9922;0.2627 0.4078 0.9961;0.2588 0.4157 0.9961;0.2549 0.4196 0.9961;0.251 0.4275 0.9961;0.2471 0.4314 1;0.2431 0.4392 1;0.2353 0.4431 1;0.2314 0.451 1;0.2235 0.4549 1;0.2196 0.4627 0.9961;0.2118 0.4667 0.9961;0.2078 0.4745 0.9922;0.2 0.4784 0.9922;0.1961 0.4863 0.9882;0.1922 0.4902 0.9882;0.1882 0.498 0.9843;0.1843 0.502 0.9804;0.1843 0.5098 0.9804;0.1804 0.5137 0.9765;0.1804 0.5176 0.9725;0.1804 0.5255 0.9725;0.1804 0.5294 0.9686;0.1765 0.5333 0.9647;0.1765 0.5412 0.9608;0.1765 0.5451 0.9569;0.1765 0.549 0.9529;0.1765 0.5569 0.949;0.1725 0.5608 0.9451;0.1725 0.5647 0.9412;0.1686 0.5686 0.9373;0.1647 0.5765 0.9333;0.1608 0.5804 0.9294;0.1569 0.5843 0.9255;0.1529 0.5922 0.9216;0.1529 0.5961 0.9176;0.149 0.6 0.9137;0.149 0.6039 0.9098;0.1451 0.6078 0.9098;0.1451 0.6118 0.9059;0.1412 0.6196 0.902;0.1412 0.6235 0.898;0.1373 0.6275 0.898;0.1373 0.6314 0.8941;0.1333 0.6353 0.8941;0.1294 0.6392 0.8902;0.1255 0.6471 0.8902;0.1216 0.651 0.8863;0.1176 0.6549 0.8824;0.1137 0.6588 0.8824;0.1137 0.6627 0.8784;0.1098 0.6667 0.8745;0.1059 0.6706 0.8706;0.102 0.6745 0.8667;0.098 0.6784 0.8627;0.0902 0.6824 0.8549;0.0863 0.6863 0.851;0.0784 0.6902 0.8471;0.0706 0.6941 0.8392;0.0627 0.698 0.8353;0.0549 0.702 0.8314;0.0431 0.702 0.8235;0.0314 0.7059 0.8196;0.0235 0.7098 0.8118;0.0157 0.7137 0.8078;0.0078 0.7176 0.8;0.0039 0.7176 0.7922;0 0.7216 0.7882;0 0.7255 0.7804;0 0.7294 0.7765;0.0039 0.7294 0.7686;0.0078 0.7333 0.7608;0.0157 0.7333 0.7569;0.0235 0.7373 0.749;0.0353 0.7412 0.7412;0.051 0.7412 0.7373;0.0627 0.7451 0.7294;0.0784 0.7451 0.7216;0.0902 0.749 0.7137;0.102 0.7529 0.7098;0.1137 0.7529 0.702;0.1255 0.7569 0.6941;0.1373 0.7569 0.6863;0.1451 0.7608 0.6824;0.1529 0.7608 0.6745;0.1608 0.7647 0.6667;0.1686 0.7647 0.6588;0.1725 0.7686 0.651;0.1804 0.7686 0.6471;0.1843 0.7725 0.6392;0.1922 0.7725 0.6314;0.1961 0.7765 0.6235;0.2 0.7804 0.6157;0.2078 0.7804 0.6078;0.2118 0.7843 0.6;0.2196 0.7843 0.5882;0.2235 0.7882 0.5804;0.2314 0.7882 0.5725;0.2392 0.7922 0.5647;0.251 0.7922 0.5529;0.2588 0.7922 0.5451;0.2706 0.7961 0.5373;0.2824 0.7961 0.5255;0.2941 0.7961 0.5176;0.3059 0.8 0.5059;0.3176 0.8 0.498;0.3294 0.8 0.4863;0.3412 0.8 0.4784;0.3529 0.8 0.4667;0.3686 0.8039 0.4549;0.3804 0.8039 0.4471;0.3922 0.8039 0.4353;0.4039 0.8039 0.4235;0.4196 0.8039 0.4118;0.4314 0.8039 0.4;0.4471 0.8039 0.3922;0.4627 0.8 0.3804;0.4745 0.8 0.3686;0.4902 0.8 0.3569;0.5059 0.8 0.349;0.5176 0.8 0.3373;0.5333 0.7961 0.3255;0.5451 0.7961 0.3176;0.5608 0.7961 0.3059;0.5765 0.7922 0.2941;0.5882 0.7922 0.2824;0.6039 0.7882 0.2745;0.6157 0.7882 0.2627;0.6314 0.7843 0.251;0.6431 0.7843 0.2431;0.6549 0.7804 0.2314;0.6706 0.7804 0.2235;0.6824 0.7765 0.2157;0.698 0.7765 0.2078;0.7098 0.7725 0.2;0.7216 0.7686 0.1922;0.7333 0.7686 0.1843;0.7451 0.7647 0.1765;0.7608 0.7647 0.1725;0.7725 0.7608 0.1647;0.7843 0.7569 0.1608;0.7961 0.7569 0.1569;0.8078 0.7529 0.1529;0.8157 0.749 0.1529;0.8275 0.749 0.1529;0.8392 0.7451 0.1529;0.851 0.7451 0.1569;0.8588 0.7412 0.1569;0.8706 0.7373 0.1608;0.8824 0.7373 0.1647;0.8902 0.7373 0.1686;0.902 0.7333 0.1765;0.9098 0.7333 0.1804;0.9176 0.7294 0.1882;0.9255 0.7294 0.1961;0.9373 0.7294 0.2078;0.9451 0.7294 0.2157;0.9529 0.7294 0.2235;0.9608 0.7294 0.2314;0.9686 0.7294 0.2392;0.9765 0.7294 0.2431;0.9843 0.7333 0.2431;0.9882 0.7373 0.2431;0.9961 0.7412 0.2392;0.9961 0.7451 0.2353;0.9961 0.7529 0.2314;0.9961 0.7569 0.2275;0.9961 0.7608 0.2235;0.9961 0.7686 0.2196;0.9961 0.7725 0.2157;0.9961 0.7804 0.2078;0.9961 0.7843 0.2039;0.9961 0.7922 0.2;0.9922 0.7961 0.1961;0.9922 0.8039 0.1922;0.9922 0.8078 0.1922;0.9882 0.8157 0.1882;0.9843 0.8235 0.1843;0.9843 0.8275 0.1804;0.9804 0.8353 0.1804;0.9765 0.8392 0.1765;0.9765 0.8471 0.1725;0.9725 0.851 0.1686;0.9686 0.8588 0.1647;0.9686 0.8667 0.1647;0.9647 0.8706 0.1608;0.9647 0.8784 0.1569;0.9608 0.8824 0.1569;0.9608 0.8902 0.1529;0.9608 0.898 0.149;0.9608 0.902 0.149;0.9608 0.9098 0.1451;0.9608 0.9137 0.1412;0.9608 0.9216 0.1373;0.9608 0.9255 0.1333;0.9608 0.9333 0.1294;0.9647 0.9373 0.1255;0.9647 0.9451 0.1216;0.9647 0.949 0.1176;0.9686 0.9569 0.1098;0.9686 0.9608 0.1059;0.9725 0.9686 0.102;0.9725 0.9725 0.0941;0.9765 0.9765 0.0863;0.9765 0.9843 0.0824];
            app.TrainAxes.LineWidth = 1;
            app.TrainAxes.TitleHorizontalAlignment = 'right';
            app.TrainAxes.ColorOrder = [0.502 0.6902 0.8314;0.9804 0.502 0.451;0.549 0.8314 0.7804;0.9882 0.7098 0.3804;0.702 0.8706 0.4118;0.749 0.7294 0.851;0.502 0.502 0.502];
            app.TrainAxes.Box = 'on';
            app.TrainAxes.Position = [9 105 475 131];

            % Create ButtonGroup
            app.ButtonGroup = uibuttongroup(app.CenterPanel);
            app.ButtonGroup.AutoResizeChildren = 'off';
            app.ButtonGroup.SelectionChangedFcn = createCallbackFcn(app, @ButtonGroupSelectionChanged, true);
            app.ButtonGroup.SizeChangedFcn = createCallbackFcn(app, @ButtonGroupSizeChanged, true);
            app.ButtonGroup.FontName = 'Montserrat';
            app.ButtonGroup.Scrollable = 'on';
            app.ButtonGroup.Position = [9 502 470 84];

            % Create BUILDCONFIGButton
            app.BUILDCONFIGButton = uitogglebutton(app.ButtonGroup);
            app.BUILDCONFIGButton.Text = 'BUILD CONFIG';
            app.BUILDCONFIGButton.WordWrap = 'on';
            app.BUILDCONFIGButton.FontName = 'Montserrat';
            app.BUILDCONFIGButton.Position = [9 7 61 70];
            app.BUILDCONFIGButton.Value = true;

            % Create ExperimentConditionsLabel
            app.ExperimentConditionsLabel = uilabel(app.CenterPanel);
            app.ExperimentConditionsLabel.HorizontalAlignment = 'right';
            app.ExperimentConditionsLabel.WordWrap = 'on';
            app.ExperimentConditionsLabel.FontName = 'Montserrat Medium';
            app.ExperimentConditionsLabel.FontSize = 13.5;
            app.ExperimentConditionsLabel.FontWeight = 'bold';
            app.ExperimentConditionsLabel.Position = [9 585 471 22];
            app.ExperimentConditionsLabel.Text = 'Experiment Conditions';

            % Create RightPanel
            app.RightPanel = uipanel(app.GridLayout);
            app.RightPanel.BackgroundColor = [0.949 0.9804 1];
            app.RightPanel.Layout.Row = 1;
            app.RightPanel.Layout.Column = 3;
            app.RightPanel.FontName = 'Montserrat';

            % Create TableConfigurations
            app.TableConfigurations = uitable(app.RightPanel);
            app.TableConfigurations.ColumnName = '';
            app.TableConfigurations.RowName = {'Cathode First'; 'Number of pulses'; 'Phase 1 amp (uA)'; 'Phase 2 amp (uA)'; 'Phase 1 duration (us)'; 'Phase 2 duration (us)'; 'Frequency (Hz)'; 'Interphase duration (us)'; 'Train Duration (s)'; 'Train Frequency (Hz)'};
            app.TableConfigurations.DisplayDataChangedFcn = createCallbackFcn(app, @TableConfigurationsDisplayDataChanged, true);
            app.TableConfigurations.Position = [12 286 364 296];

            % Create TableTriggers
            app.TableTriggers = uitable(app.RightPanel);
            app.TableTriggers.ColumnName = '';
            app.TableTriggers.RowName = {'Expression'; 'Config ID'; 'Electrode(s)'};
            app.TableTriggers.CellSelectionCallback = createCallbackFcn(app, @TableTriggersCellSelection, true);
            app.TableTriggers.Position = [12 110 364 156];

            % Create StimulationTriggersLabel
            app.StimulationTriggersLabel = uilabel(app.RightPanel);
            app.StimulationTriggersLabel.HorizontalAlignment = 'center';
            app.StimulationTriggersLabel.FontName = 'Montserrat SemiBold';
            app.StimulationTriggersLabel.Position = [7 265 384 22];
            app.StimulationTriggersLabel.Text = 'StimulationTriggers';

            % Create StimulationConfigurationsLabel
            app.StimulationConfigurationsLabel = uilabel(app.RightPanel);
            app.StimulationConfigurationsLabel.HorizontalAlignment = 'center';
            app.StimulationConfigurationsLabel.FontName = 'Montserrat SemiBold';
            app.StimulationConfigurationsLabel.Position = [7 582 384 20];
            app.StimulationConfigurationsLabel.Text = 'StimulationConfigurations';

            % Create DeleteChannelDropDown
            app.DeleteChannelDropDown = uidropdown(app.RightPanel);
            app.DeleteChannelDropDown.Items = {};
            app.DeleteChannelDropDown.FontName = 'Montserrat';
            app.DeleteChannelDropDown.Position = [187 79 142 22];
            app.DeleteChannelDropDown.Value = {};

            % Create DeleteTriggerDropDown
            app.DeleteTriggerDropDown = uidropdown(app.RightPanel);
            app.DeleteTriggerDropDown.Items = {};
            app.DeleteTriggerDropDown.FontName = 'Montserrat';
            app.DeleteTriggerDropDown.Position = [187 50 141 22];
            app.DeleteTriggerDropDown.Value = {};

            % Create DeleteConfigurationButton
            app.DeleteConfigurationButton = uibutton(app.RightPanel, 'push');
            app.DeleteConfigurationButton.ButtonPushedFcn = createCallbackFcn(app, @DeleteConfigurationButtonPushed, true);
            app.DeleteConfigurationButton.FontName = 'Montserrat';
            app.DeleteConfigurationButton.Position = [43 78 142 23];
            app.DeleteConfigurationButton.Text = 'Delete Configuration';

            % Create DeleteTriggerButton
            app.DeleteTriggerButton = uibutton(app.RightPanel, 'push');
            app.DeleteTriggerButton.ButtonPushedFcn = createCallbackFcn(app, @DeleteTriggerButtonPushed, true);
            app.DeleteTriggerButton.FontName = 'Montserrat';
            app.DeleteTriggerButton.Position = [84 50 101 23];
            app.DeleteTriggerButton.Text = 'Delete Trigger';

            % Create StimulationOnLampLabel
            app.StimulationOnLampLabel = uilabel(app.RightPanel);
            app.StimulationOnLampLabel.HorizontalAlignment = 'right';
            app.StimulationOnLampLabel.FontName = 'Montserrat Black';
            app.StimulationOnLampLabel.Visible = 'off';
            app.StimulationOnLampLabel.Position = [243 306 73 38];
            app.StimulationOnLampLabel.Text = 'Stimulation On';

            % Create StimulationOnLamp
            app.StimulationOnLamp = uilamp(app.RightPanel);
            app.StimulationOnLamp.Visible = 'off';
            app.StimulationOnLamp.Position = [315 289 62 62];
            app.StimulationOnLamp.Color = [1 0 0];

            % Create PlayStimulationButton
            app.PlayStimulationButton = uibutton(app.RightPanel, 'push');
            app.PlayStimulationButton.ButtonPushedFcn = createCallbackFcn(app, @PlayStimulationButtonPushed, true);
            app.PlayStimulationButton.WordWrap = 'on';
            app.PlayStimulationButton.FontName = 'Montserrat Black';
            app.PlayStimulationButton.FontSize = 22;
            app.PlayStimulationButton.Visible = 'off';
            app.PlayStimulationButton.Position = [203 63 174 67];
            app.PlayStimulationButton.Text = 'Play Stimulation';

            % Create ExporttoparameterfileButton
            app.ExporttoparameterfileButton = uibutton(app.RightPanel, 'push');
            app.ExporttoparameterfileButton.ButtonPushedFcn = createCallbackFcn(app, @ExporttoparameterfileButtonPushed, true);
            app.ExporttoparameterfileButton.BackgroundColor = [0 0.0549 0.251];
            app.ExporttoparameterfileButton.FontName = 'Montserrat Black';
            app.ExporttoparameterfileButton.FontSize = 18;
            app.ExporttoparameterfileButton.FontColor = [1 1 1];
            app.ExporttoparameterfileButton.Position = [73 9 254 30];
            app.ExporttoparameterfileButton.Text = 'Export to parameter file';

            % Create FileMenu
            app.FileMenu = uimenu(app.UIFigure);
            app.FileMenu.Text = 'File';

            % Create LoadParameterFileMenu
            app.LoadParameterFileMenu = uimenu(app.FileMenu);
            app.LoadParameterFileMenu.MenuSelectedFcn = createCallbackFcn(app, @LoadParameterFileMenuSelected, true);
            app.LoadParameterFileMenu.Separator = 'on';
            app.LoadParameterFileMenu.Text = 'Load Parameter File';

            % Create PreferencesMenu
            app.PreferencesMenu = uimenu(app.UIFigure);
            app.PreferencesMenu.Text = 'Preferences';

            % Create AutoincrementTriggerExpressionMenu
            app.AutoincrementTriggerExpressionMenu = uimenu(app.PreferencesMenu);
            app.AutoincrementTriggerExpressionMenu.MenuSelectedFcn = createCallbackFcn(app, @AutoincrementTriggerExpressionMenuSelected, true);
            app.AutoincrementTriggerExpressionMenu.Text = 'Auto-increment Trigger Expression?';

            % Create IncreaseFontSizeMenu
            app.IncreaseFontSizeMenu = uimenu(app.PreferencesMenu);
            app.IncreaseFontSizeMenu.MenuSelectedFcn = createCallbackFcn(app, @IncreaseFontSizeMenuSelected, true);
            app.IncreaseFontSizeMenu.Accelerator = '=';
            app.IncreaseFontSizeMenu.Text = 'Increase Font Size';

            % Create DecreaseFontSizeMenu
            app.DecreaseFontSizeMenu = uimenu(app.PreferencesMenu);
            app.DecreaseFontSizeMenu.MenuSelectedFcn = createCallbackFcn(app, @DecreaseFontSizeMenuSelected, true);
            app.DecreaseFontSizeMenu.Accelerator = '-';
            app.DecreaseFontSizeMenu.Text = 'Decrease Font Size';

            % Create ColorContextMenu
            app.ColorContextMenu = uicontextmenu(app.UIFigure);

            % Create ChangeColorsMenu
            app.ChangeColorsMenu = uimenu(app.ColorContextMenu);
            app.ChangeColorsMenu.Text = 'Change Colors';

            % Create BackgroundMenu
            app.BackgroundMenu = uimenu(app.ChangeColorsMenu);
            app.BackgroundMenu.MenuSelectedFcn = createCallbackFcn(app, @BackgroundMenuSelected, true);
            app.BackgroundMenu.Text = 'Background';

            % Create Plot1Menu
            app.Plot1Menu = uimenu(app.ChangeColorsMenu);
            app.Plot1Menu.MenuSelectedFcn = createCallbackFcn(app, @Plot1MenuSelected, true);
            app.Plot1Menu.Text = 'Plot 1';

            % Create Plot2Menu
            app.Plot2Menu = uimenu(app.ChangeColorsMenu);
            app.Plot2Menu.MenuSelectedFcn = createCallbackFcn(app, @Plot2MenuSelected, true);
            app.Plot2Menu.Text = 'Plot 2';

            % Create Plot3Menu
            app.Plot3Menu = uimenu(app.ChangeColorsMenu);
            app.Plot3Menu.MenuSelectedFcn = createCallbackFcn(app, @Plot3MenuSelected, true);
            app.Plot3Menu.Text = 'Plot 3';

            % Create RestoreDefaultColorsMenu
            app.RestoreDefaultColorsMenu = uimenu(app.ColorContextMenu);
            app.RestoreDefaultColorsMenu.MenuSelectedFcn = createCallbackFcn(app, @RestoreDefaultColorsMenuSelected, true);
            app.RestoreDefaultColorsMenu.Text = 'Restore Default Colors';
            
            % Assign app.ColorContextMenu
            app.SinglePulseAxes.ContextMenu = app.ColorContextMenu;
            app.MultPulsesAxes.ContextMenu = app.ColorContextMenu;
            app.TrainAxes.ContextMenu = app.ColorContextMenu;

            % Show the figure after all components are created
            app.UIFigure.Visible = 'on';
        end
    end

    % App creation and deletion
    methods (Access = public)

        % Construct app
        function app = SCIT_sourceCode

            % Create UIFigure and components
            createComponents(app)

            % Register the app with App Designer
            registerApp(app, app.UIFigure)

            % Execute the startup function
            runStartupFcn(app, @startupFcn)

            if nargout == 0
                clear app
            end
        end

        % Code that executes before app deletion
        function delete(app)

            % Delete UIFigure when app is deleted
            delete(app.UIFigure)
        end
    end
end
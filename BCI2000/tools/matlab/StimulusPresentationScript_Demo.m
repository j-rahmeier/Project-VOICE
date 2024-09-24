% BCI2000 Stimulus Presentation Demo Script
% 
% StimulusPresentationScript_Demo creates a parameter fragment that can be
% loaded into BCI2000 to create a stimulus presentation experiment.
% 
% This demo script will take the image files located in the BCI2000 prog
% directory and create a stimuli matrix containing these images, variable
% duration fixation cross stimuli, instructions, and a sync pulse. 
% 
% Change the n_rows and n_stimuli variables to store more information with
% the stimuli or add additional stimuli. Best practice is to separate
% stimuli into banks (e.g. 1-25, 101-125, etc) for easy evaluation later. 
% 
% Note that every stimulus needs to have an index for every row desired,
% even if that row label is not meaningful for the stimulus.
% 
% A sequence is created to alternate the fixation cross stimuli with the
% image stimuli.
% 
% The stimuli and meaningful parameters are written into a param
% variable and stored as a *.prm file using the convert_bciprm function.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Author: James Swift <swift@neurotechcenter.org>
%%
%% $BEGIN_BCI2000_LICENSE$
%% 
%% This file is part of BCI2000, a platform for real-time bio-signal research.
%% [ Copyright (C) 2000-2021: BCI2000 team and many external contributors ]
%% 
%% BCI2000 is free software: you can redistribute it and/or modify it under the
%% terms of the GNU General Public License as published by the Free Software
%% Foundation, either version 3 of the License, or (at your option) any later
%% version.
%% 
%% BCI2000 is distributed in the hope that it will be useful, but
%%                         WITHOUT ANY WARRANTY
%% - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
%% A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
%% 
%% You should have received a copy of the GNU General Public License along with
%% this program.  If not, see <http://www.gnu.org/licenses/>.
%% 
%% $END_BCI2000_LICENSE$
%% http://www.bci2000.org 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Set the path of the BCI2000 main directory here
prompt               = {'Enter path to BCI2000 root directory: '};
dlgtitle             = 'Enter BCI2000 path';
fieldsize            = [1 45];
definput             = {fullfile('C:','bci2000.x64')};
userinput            = inputdlg(prompt,dlgtitle,fieldsize,definput);
settings.BCI2000path = userinput{1};


% Add BCI2000 tools to path
addpath(genpath(fullfile(settings.BCI2000path,'tools')))


%% Settings
settings.SamplingRate          = '256Hz'; % device sampling rate
settings.SampleBlockSize       = '8';     % number of samples in a block

settings.PreRunDuration        = '2s';
settings.PostRunDuration       = '0.5s';
settings.TaskDuration          = '2s';
settings.InstructionDuration   = '30s';
settings.SyncPulseDuration     = '1s';
settings.BaselineMinDuration   = '0.5s';
settings.BaselineMaxDuration   = '1.5s';
settings.NumberOfSequences     = '1';
settings.StimulusWidth         = '30';
settings.WindowTop             = '0';
settings.WindowLeft            = '0';
settings.WindowWidth           = '640';
settings.WindowHeight          = '480';
settings.BackgroundColor       = '0x000000';
settings.CaptionColor          = '0xFFFFFF';
settings.CaptionSwitch         = '1';
settings.WindowBackgroundColor = '0x000000';
settings.ISIMinDuration        = '0s';
settings.ISIMaxDuration        = '0s';
settings.SubjectName           = 'BCI';
settings.DataDirectory         = fullfile('..','data');
settings.SubjectSession        = 'auto';
settings.SubjectRun            = '01';
settings.parm_filename         = fullfile(settings.BCI2000path,'parms','demo_parms.prm');
settings.UserComment           = 'Enter user comment here';

settings.InstructCaption   = {'Stimulus Presentation Task. Press space to continue'; 'End of task.'};

%% Get task images
task_images = dir(fullfile(settings.BCI2000path,'prog','images','*.bmp'));

%% Set up the different stimuli so they are represented by unique stimulus codes, separated into banks for easy evaluation later
n_stimuli = 301; % Total events
n_rows    = 7;

% break down into blocks for easier analysis later
% 1-50:    image stimuli
% 101-150: inter-stimulus interval (variable duration)
% 201:     instructions
% 301:     sync pulse


% Set up Stimuli
param.Stimuli.Section         = 'Application';
param.Stimuli.Type            = 'matrix';
param.Stimuli.DefaultValue    = '';
param.Stimuli.LowRange        = '';
param.Stimuli.HighRange       = '';
param.Stimuli.Comment         = 'captions and icons to be displayed, sounds to be played for different stimuli';
param.Stimuli.Value           = cell(n_rows,n_stimuli);
param.Stimuli.Value(:)        = {''};
param.Stimuli.RowLabels       = cell(n_rows,1);
param.Stimuli.RowLabels(:)    = {''};
param.Stimuli.ColumnLabels    = cell(1,n_stimuli);
param.Stimuli.ColumnLabels(:) = {''};

param.Stimuli.RowLabels{1}  = 'caption';
param.Stimuli.RowLabels{2}  = 'icon';
param.Stimuli.RowLabels{3}  = 'audio';
param.Stimuli.RowLabels{4}  = 'StimulusDuration';
param.Stimuli.RowLabels{5}  = 'AudioVolume';
param.Stimuli.RowLabels{6}  = 'Category';
param.Stimuli.RowLabels{7}  = 'EarlyOffsetExpression';

%% Study images 1-50
for idx = 1:length(task_images)
    param.Stimuli.ColumnLabels{idx} = sprintf('%d',idx);
    param.Stimuli.Value{1,idx}      = '';
    param.Stimuli.Value{2,idx}      = sprintf('%s',fullfile('..','prog','images',task_images(idx).name));
    param.Stimuli.Value{3,idx}      = '';
    param.Stimuli.Value{4,idx}      = settings.TaskDuration;
    param.Stimuli.Value{5,idx}      = '0';      
    param.Stimuli.Value{6,idx}      = 'image'; 
    param.Stimuli.Value{7,idx}      = ''; 
end 


%% inter-stimulus interval (fixation cross) 101-150
% variable duration from 0.5-1.5s
SamplingRate = str2double(settings.SamplingRate(1:end-2));
BlockSize    = str2double(settings.SampleBlockSize);
MinDuration  = str2double(settings.BaselineMinDuration(1:end-1));
MaxDuration  = str2double(settings.BaselineMaxDuration(1:end-1));
for idx = 101:100+length(task_images)
    blockvals = MinDuration:BlockSize/SamplingRate:MaxDuration;
    randval   = randi(length(blockvals));
    duration  = blockvals(randval);
    
    param.Stimuli.ColumnLabels{idx} = sprintf('%d',idx);
    param.Stimuli.Value{1,idx}      = '+';
    param.Stimuli.Value{2,idx}      = '';
    param.Stimuli.Value{3,idx}      = '';
    param.Stimuli.Value{4,idx}      = strcat(num2str(duration,7),'s');
    param.Stimuli.Value{5,idx}      = '0';      
    param.Stimuli.Value{6,idx}      = 'fixation'; 
    param.Stimuli.Value{7,idx}      = ''; 
end


%% Instructions 201-202
idx_iter = 1;
for idx = 201:200+length(settings.InstructCaption)
    param.Stimuli.ColumnLabels{idx} = sprintf('%d',idx);
    param.Stimuli.Value{1,idx}      = settings.InstructCaption{idx_iter};
    param.Stimuli.Value{2,idx}      = '';
    param.Stimuli.Value{3,idx}      = '';
    param.Stimuli.Value{4,idx}      = settings.InstructionDuration;
    param.Stimuli.Value{5,idx}      = '0';    
    param.Stimuli.Value{6,idx}      = 'instruction'; 
    param.Stimuli.Value{7,idx}      = 'KeyDown == 32'; % space key 
    
    idx_iter = idx_iter + 1;
end 

%% Sync pulse 301
idx = 301;
param.Stimuli.ColumnLabels{idx} = sprintf('%d',idx);
param.Stimuli.Value{1,idx}      = '';
param.Stimuli.Value{2,idx}      = '';
param.Stimuli.Value{3,idx}      = '';
param.Stimuli.Value{4,idx}      = settings.SyncPulseDuration;
param.Stimuli.Value{5,idx}      = '0';      
param.Stimuli.Value{6,idx}      = 'sync'; 
param.Stimuli.Value{7,idx}      = '';     


%% Sequence
% 1-50:    image stimuli
% 101-150: inter-stimulus interval (variable duration)
% 201:     instructions
% 301:     sync pulse

randOrder = randperm(length(task_images));
taskseq   = [];
for i = 1:length(task_images)
    currentImage = randOrder(i);
    taskseq      = [taskseq (100+currentImage) currentImage];
end

seq = [301 201 taskseq 202 301]';

param.Sequence.Section      = 'Application';
param.Sequence.Type         = 'intlist';
param.Sequence.DefaultValue = '1';
param.Sequence.LowRange     = '1';
param.Sequence.HighRange    = '';
param.Sequence.Comment      = 'Sequence in which stimuli are presented (deterministic mode)/ Stimulus frequencies for each stimulus (random mode)';
param.Sequence.Value        = cellfun(@num2str, num2cell(seq), 'un',0);
param.Sequence.NumericValue = seq;

%% UserComment
param.UserComment.Section         = 'Application';
param.UserComment.Type            = 'string';
param.UserComment.DefaultValue    = '';
param.UserComment.LowRange        = '';
param.UserComment.HighRange       = '';
param.UserComment.Comment         = 'User comments for a specific run';
param.UserComment.Value           = {settings.UserComment};

%%
param.SamplingRate.Section         = 'Source';
param.SamplingRate.Type            = 'int';
param.SamplingRate.DefaultValue    = '256Hz';
param.SamplingRate.LowRange        = '1';
param.SamplingRate.HighRange       = '';
param.SamplingRate.Comment         = 'sample rate';
param.SamplingRate.Value           = {settings.SamplingRate};

%%
param.SampleBlockSize.Section         = 'Source';
param.SampleBlockSize.Type            = 'int';
param.SampleBlockSize.DefaultValue    = '8';
param.SampleBlockSize.LowRange        = '1';
param.SampleBlockSize.HighRange       = '';
param.SampleBlockSize.Comment         = 'number of samples transmitted at a time';
param.SampleBlockSize.Value           = {settings.SampleBlockSize};

%%
param.NumberOfSequences.Section         = 'Application';
param.NumberOfSequences.Type            = 'int';
param.NumberOfSequences.DefaultValue    = '1';
param.NumberOfSequences.LowRange        = '0';
param.NumberOfSequences.HighRange       = '';
param.NumberOfSequences.Comment         = 'number of sequence repetitions in a run';
param.NumberOfSequences.Value           = {settings.NumberOfSequences};

%%
param.StimulusWidth.Section         = 'Application';
param.StimulusWidth.Type            = 'int';
param.StimulusWidth.DefaultValue    = '0';
param.StimulusWidth.LowRange        = '';
param.StimulusWidth.HighRange       = '';
param.StimulusWidth.Comment         = 'StimulusWidth in percent of screen width (zero for original pixel size)';
param.StimulusWidth.Value           = {settings.StimulusWidth};

%%
param.SequenceType.Section              = 'Application';
param.SequenceType.Type                 = 'int';
param.SequenceType.DefaultValue         = '0';
param.SequenceType.LowRange             = '0';
param.SequenceType.HighRange            = '1';
param.SequenceType.Comment              = 'Sequence of stimuli is 0 deterministic, 1 random (enumeration)';
param.SequenceType.Value                = {'0'};

%%
param.StimulusDuration.Section           = 'Application';
param.StimulusDuration.Type              = 'float';
param.StimulusDuration.DefaultValue      = '40ms';
param.StimulusDuration.LowRange          = '0';
param.StimulusDuration.HighRange         = '';
param.StimulusDuration.Comment           = 'stimulus duration';
param.StimulusDuration.Value             = {};

%%
param.ISIMaxDuration.Section       = 'Application';
param.ISIMaxDuration.Type          = 'float';
param.ISIMaxDuration.DefaultValue  = '80ms';
param.ISIMaxDuration.LowRange      = '0';
param.ISIMaxDuration.HighRange     = '';
param.ISIMaxDuration.Comment       = 'maximum duration of inter-stimulus interval';
param.ISIMaxDuration.Value         = {settings.ISIMaxDuration};

%%
param.ISIMinDuration.Section       = 'Application';
param.ISIMinDuration.Type          = 'float';
param.ISIMinDuration.DefaultValue  = '80ms';
param.ISIMinDuration.LowRange      = '0';
param.ISIMinDuration.HighRange     = '';
param.ISIMinDuration.Comment       = 'minimum duration of inter-stimulus interval';
param.ISIMinDuration.Value         = {settings.ISIMinDuration};

%%
param.PreSequenceDuration.Section       = 'Application';
param.PreSequenceDuration.Type          = 'float';
param.PreSequenceDuration.DefaultValue  = '2s';
param.PreSequenceDuration.LowRange      = '0';
param.PreSequenceDuration.HighRange     = '';
param.PreSequenceDuration.Comment       = 'pause preceding sequences/sets of intensifications';
param.PreSequenceDuration.Value         = {'0s'};

%%
param.PostSequenceDuration.Section       = 'Application';
param.PostSequenceDuration.Type          = 'float';
param.PostSequenceDuration.DefaultValue  = '2s';
param.PostSequenceDuration.LowRange      = '0';
param.PostSequenceDuration.HighRange     = '';
param.PostSequenceDuration.Comment       = 'pause following sequences/sets of intensifications';
param.PostSequenceDuration.Value         = {'0s'};

%%
param.PreRunDuration.Section       = 'Application';
param.PreRunDuration.Type          = 'float';
param.PreRunDuration.DefaultValue  = '2000ms';
param.PreRunDuration.LowRange      = '0';
param.PreRunDuration.HighRange     = '';
param.PreRunDuration.Comment       = 'pause preceding first sequence';
param.PreRunDuration.Value         = {settings.PreRunDuration};

%%
param.PostRunDuration.Section       = 'Application';
param.PostRunDuration.Type          = 'float';
param.PostRunDuration.DefaultValue  = '2000ms';
param.PostRunDuration.LowRange      = '0';
param.PostRunDuration.HighRange     = '';
param.PostRunDuration.Comment       = 'pause following last squence';
param.PostRunDuration.Value         = {settings.PostRunDuration};


%%
param.BackgroundColor.Section      = 'Application';
param.BackgroundColor.Type         = 'string';
param.BackgroundColor.DefaultValue = '0x00FFFF00';
param.BackgroundColor.LowRange     = '0x00000000';
param.BackgroundColor.HighRange    = '0x00000000';
param.BackgroundColor.Comment      = 'Color of stimulus background (color)';
param.BackgroundColor.Value        = {settings.BackgroundColor};

%%
param.CaptionColor.Section      = 'Application';
param.CaptionColor.Type         = 'string';
param.CaptionColor.DefaultValue = '0x00FFFF00';
param.CaptionColor.LowRange     = '0x00000000';
param.CaptionColor.HighRange    = '0x00000000';
param.CaptionColor.Comment      = 'Color of stimulus caption text (color)';
param.CaptionColor.Value        = {settings.CaptionColor};

%%
param.WindowBackgroundColor.Section      = 'Application';
param.WindowBackgroundColor.Type         = 'string';
param.WindowBackgroundColor.DefaultValue = '0x00FFFF00';
param.WindowBackgroundColor.LowRange     = '0x00000000';
param.WindowBackgroundColor.HighRange    = '0x00000000';
param.WindowBackgroundColor.Comment      = 'background color (color)';
param.WindowBackgroundColor.Value        = {settings.WindowBackgroundColor};

%%
param.IconSwitch.Section          = 'Application';
param.IconSwitch.Type             = 'int';
param.IconSwitch.DefaultValue     = '1';
param.IconSwitch.LowRange         = '0';
param.IconSwitch.HighRange        = '1';
param.IconSwitch.Comment          = 'Present icon files (boolean)';
param.IconSwitch.Value            = {'1'};

%%
param.AudioSwitch.Section         = 'Application';
param.AudioSwitch.Type            = 'int';
param.AudioSwitch.DefaultValue    = '1';
param.AudioSwitch.LowRange        = '0';
param.AudioSwitch.HighRange       = '1';
param.AudioSwitch.Comment         = 'Present audio files (boolean)';
param.AudioSwitch.Value           = {'0'};

%%
param.CaptionSwitch.Section       = 'Application';
param.CaptionSwitch.Type          = 'int';
param.CaptionSwitch.DefaultValue  = '1';
param.CaptionSwitch.LowRange      = '0';
param.CaptionSwitch.HighRange     = '1';
param.CaptionSwitch.Comment       = 'Present captions (boolean)';
param.CaptionSwitch.Value         = {settings.CaptionSwitch};

%%
param.WindowHeight.Section        = 'Application';
param.WindowHeight.Type           = 'int';
param.WindowHeight.DefaultValue   = '480';
param.WindowHeight.LowRange       = '0';
param.WindowHeight.HighRange      = '';
param.WindowHeight.Comment        = 'height of application window';
param.WindowHeight.Value          = {settings.WindowHeight};

%%
param.WindowWidth.Section        = 'Application';
param.WindowWidth.Type           = 'int';
param.WindowWidth.DefaultValue   = '480';
param.WindowWidth.LowRange       = '0';
param.WindowWidth.HighRange      = '';
param.WindowWidth.Comment        = 'width of application window';
param.WindowWidth.Value          = {settings.WindowWidth};

%%
param.WindowLeft.Section        = 'Application';
param.WindowLeft.Type           = 'int';
param.WindowLeft.DefaultValue   = '0';
param.WindowLeft.LowRange       = '';
param.WindowLeft.HighRange      = '';
param.WindowLeft.Comment        = 'screen coordinate of application window''s left edge';
param.WindowLeft.Value          = {settings.WindowLeft};

%%
param.WindowTop.Section        = 'Application';
param.WindowTop.Type           = 'int';
param.WindowTop.DefaultValue   = '0';
param.WindowTop.LowRange       = '';
param.WindowTop.HighRange      = '';
param.WindowTop.Comment        = 'screen coordinate of application window''s top edge';
param.WindowTop.Value          = {settings.WindowTop};

%%
param.CaptionHeight.Section      = 'Application';
param.CaptionHeight.Type         = 'int';
param.CaptionHeight.DefaultValue = '0';
param.CaptionHeight.LowRange     = '0';
param.CaptionHeight.HighRange    = '100';
param.CaptionHeight.Comment      = 'Height of stimulus caption text in percent of screen height';
param.CaptionHeight.Value        = {'5'};

%%
param.WarningExpression.Section      = 'Filtering';
param.WarningExpression.Type         = 'string';
param.WarningExpression.DefaultValue = '';
param.WarningExpression.LowRange     = '';
param.WarningExpression.HighRange    = '';
param.WarningExpression.Comment      = 'expression that results in a warning when it evaluates to true';
param.WarningExpression.Value        = {''};

%%
param.Expressions.Section      = 'Filtering';
param.Expressions.Type         = 'matrix';
param.Expressions.DefaultValue = '';
param.Expressions.LowRange     = '';
param.Expressions.HighRange    = '';
param.Expressions.Comment      = 'expressions used to compute the output of the ExpressionFilter';
param.Expressions.Value        = {''};

%%
param.SubjectName.Section      = 'Storage';
param.SubjectName.Type         = 'string';
param.SubjectName.DefaultValue = 'Name';
param.SubjectName.LowRange     = '';
param.SubjectName.HighRange    = '';
param.SubjectName.Comment      = 'subject alias';
param.SubjectName.Value        = {settings.SubjectName};

%%
param.DataDirectory.Section      = 'Storage';
param.DataDirectory.Type         = 'string';
param.DataDirectory.DefaultValue = strcat('..\data');
param.DataDirectory.LowRange     = '';
param.DataDirectory.HighRange    = '';
param.DataDirectory.Comment      = 'path to top level data directory (directory)';
param.DataDirectory.Value        = {settings.DataDirectory};

%%
param.SubjectRun.Section      = 'Storage';
param.SubjectRun.Type         = 'string';
param.SubjectRun.DefaultValue = '00';
param.SubjectRun.LowRange     = '';
param.SubjectRun.HighRange    = '';
param.SubjectRun.Comment      = 'two-digit run number';
param.SubjectRun.Value        = {settings.SubjectRun};

%%
param.SubjectSession.Section      = 'Storage';
param.SubjectSession.Type         = 'string';
param.SubjectSession.DefaultValue = '00';
param.SubjectSession.LowRange     = '';
param.SubjectSession.HighRange    = '';
param.SubjectSession.Comment      = 'three-digit session number';
param.SubjectSession.Value        = {settings.SubjectSession};


%% write the param struct to a bci2000 parameter file
parameter_lines = convert_bciprm( param );
fid = fopen(settings.parm_filename, 'w');

for i=1:length(parameter_lines)
    fprintf( fid, '%s', parameter_lines{i} );
    fprintf( fid, '\r\n' );
end
fclose(fid);
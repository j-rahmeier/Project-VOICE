from convert_bciprm import *
import os
import random

def float_range( start, stop, step ):
    l = []
    i = start
    q = 0
    while i < stop:
        l.append( i )
        q += 1
        i = start + q * step
    return l

print( "----Loading Settings----" )

settings = BCISettings() 

settings[ "BCI2000Path" ] = os.path.join( os.getcwd(), "..", ".." )
print( "BCI2000Path: ", settings.BCI2000Path )

settings[ "SamplingRate         " ] = '256Hz' # device sampling rate
settings[ "SampleBlockSize      " ] = '8'     # number of samples in a block

settings[ "PreRunDuration       " ] = '2s'
settings[ "PostRunDuration      " ] = '0.5s'
settings[ "TaskDuration         " ] = '2s'
settings[ "InstructionDuration  " ] = '30s'
settings[ "SyncPulseDuration    " ] = '1s'
settings[ "BaselineMinDuration  " ] = '0.5s'
settings[ "BaselineMaxDuration  " ] = '1.5s'
settings[ "NumberOfSequences    " ] = '1'
settings[ "StimulusWidth        " ] = '30'
settings[ "WindowTop            " ] = '0'
settings[ "WindowLeft           " ] = '0'
settings[ "WindowWidth          " ] = '640'
settings[ "WindowHeight         " ] = '480'
settings[ "BackgroundColor      " ] = '0x000000'
settings[ "CaptionColor         " ] = '0xFFFFFF'
settings[ "CaptionSwitch        " ] = '1'
settings[ "WindowBackgroundColor" ] = '0x000000'
settings[ "ISIMinDuration       " ] = '0s'
settings[ "ISIMaxDuration       " ] = '0s'
settings[ "SubjectName          " ] = 'BCI'
settings[ "DataDirectory        " ] = "../data"
settings[ "SubjectSession       " ] = 'auto'
settings[ "SubjectRun           " ] = '01'
settings[ "parm_filename        " ] = os.path.join( settings[ "BCI2000Path" ], "parms", "demo_parms_py.prm" )
settings[ "UserComment          " ] = 'Enter user comment here'

settings.InstructionsCaption   = [ 'Stimulus Presentation Task. Press space to continue', 'End of task.' ]


print( "----Done Loading Settings----" )

print( "----Reading Default Params----" )

param = read_bciprm( "default.prm" )

print( "----Done Reading Params----" )

print( "----Setting Params----" )

n_stimuli = 301
n_rows = 7

param[ 'Application' ]['Stimuli']['Stimuli']['Section        '] = 'Application'
param[ 'Application' ]['Stimuli']['Stimuli']['Type           '] = 'matrix'
param[ 'Application' ]['Stimuli']['Stimuli']['DefaultValue   '] = ''
param[ 'Application' ]['Stimuli']['Stimuli']['LowRange       '] = ''
param[ 'Application' ]['Stimuli']['Stimuli']['HighRange      '] = ''
param[ 'Application' ]['Stimuli']['Stimuli']['Comment        '] = 'captions and icons to be displayed, sounds to be played for different stimuli'
param[ 'Application' ]['Stimuli']['Stimuli']['Value          '] = bcimatrix( n_rows, n_stimuli )
param[ 'Application' ]['Stimuli']['Stimuli']['RowLabels      '] = bcilist( n_rows )
param[ 'Application' ]['Stimuli']['Stimuli']['ColumnLabels   '] = bcilist( n_stimuli )

param.Application.Stimuli.Stimuli.RowLabels[0]  = 'caption'
param.Application.Stimuli.Stimuli.RowLabels[1]  = 'icon'
param.Application.Stimuli.Stimuli.RowLabels[2]  = 'audio'
param.Application.Stimuli.Stimuli.RowLabels[3]  = 'StimulusDuration'
param.Application.Stimuli.Stimuli.RowLabels[4]  = 'AudioVolume'
param.Application.Stimuli.Stimuli.RowLabels[5]  = 'Category'
param.Application.Stimuli.Stimuli.RowLabels[6]  = 'EarlyOffsetExpression'

task_images = os.listdir( os.path.join( settings[ "BCI2000Path" ], "prog", "images" ) )
for i in task_images:
    if not ".bmp" in i:
        task_images.remove( i )
random.shuffle( task_images )

# Study images 1-50
for idx in range( 0, len( task_images ) ):
    param.Application.Stimuli.Stimuli.ColumnLabels[ idx ]    = str( idx + 1 )
    param.Application.Stimuli.Stimuli.Value[ 0 ][ idx ]      = ''
    param.Application.Stimuli.Stimuli.Value[ 1 ][ idx ]      = os.path.join( "..", "prog", "images", task_images[ idx ] )
    param.Application.Stimuli.Stimuli.Value[ 2 ][ idx ]      = ''
    param.Application.Stimuli.Stimuli.Value[ 3 ][ idx ]      = settings.TaskDuration
    param.Application.Stimuli.Stimuli.Value[ 4 ][ idx ]      = '0'      
    param.Application.Stimuli.Stimuli.Value[ 5 ][ idx ]      = 'image' 
    param.Application.Stimuli.Stimuli.Value[ 6 ][ idx ]      = '' 


# inter-stimulus interval (fixation cross) 101-150
# variable duration from 0.5-1.5s
SamplingRate = float(settings.SamplingRate[ : -2 ] )
BlockSize    = float(settings.SampleBlockSize)
MinDuration  = float(settings.BaselineMinDuration[ : -1 ] )
MaxDuration  = float(settings.BaselineMaxDuration[ : -1 ] )
for idx in range( 100, 100 + len( task_images ) ):
    blockvals = float_range( MinDuration, MaxDuration, BlockSize / SamplingRate )
    randval   = random.randint( 0, len(blockvals) - 1 )
    duration  = blockvals[ randval ]
    
    param.Application.Stimuli.Stimuli.ColumnLabels[ idx ]    = str( idx + 1 )
    param.Application.Stimuli.Stimuli.Value[ 0 ][ idx ]      = '+'
    param.Application.Stimuli.Stimuli.Value[ 1 ][ idx ]      = ''
    param.Application.Stimuli.Stimuli.Value[ 2 ][ idx ]      = ''
    param.Application.Stimuli.Stimuli.Value[ 3 ][ idx ]      = str( duration ) + "s"
    param.Application.Stimuli.Stimuli.Value[ 4 ][ idx ]      = '0' 
    param.Application.Stimuli.Stimuli.Value[ 5 ][ idx ]      = 'fixation'
    param.Application.Stimuli.Stimuli.Value[ 6 ][ idx ]      = ''


# Instructions 201-202
idx_iter = 0
for idx in range( 200, 200 + len( settings.InstructionsCaption ) ):
    param.Application.Stimuli.Stimuli.ColumnLabels[ idx ] = str( idx )
    param.Application.Stimuli.Stimuli.Value[ 0 ][ idx ]      = settings.InstructionsCaption[ idx_iter ]
    param.Application.Stimuli.Stimuli.Value[ 1 ][ idx ]      = ''
    param.Application.Stimuli.Stimuli.Value[ 2 ][ idx ]      = ''
    param.Application.Stimuli.Stimuli.Value[ 3 ][ idx ]      = settings.InstructionDuration
    param.Application.Stimuli.Stimuli.Value[ 4 ][ idx ]      = '0'    
    param.Application.Stimuli.Stimuli.Value[ 5 ][ idx ]      = 'instruction' 
    param.Application.Stimuli.Stimuli.Value[ 6 ][ idx ]      = 'KeyDown == 32' # space key 
    
    idx_iter = idx_iter + 1

# Sync pulse 301
idx = 300
param.Application.Stimuli.Stimuli.ColumnLabels[ idx ] = str( idx + 1 )
param.Application.Stimuli.Stimuli.Value[ 0 ][ idx ]      = ''
param.Application.Stimuli.Stimuli.Value[ 1 ][ idx ]      = ''
param.Application.Stimuli.Stimuli.Value[ 2 ][ idx ]      = ''
param.Application.Stimuli.Stimuli.Value[ 3 ][ idx ]      = settings.SyncPulseDuration
param.Application.Stimuli.Stimuli.Value[ 4 ][ idx ]      = '0'      
param.Application.Stimuli.Stimuli.Value[ 5 ][ idx ]      = 'sync' 
param.Application.Stimuli.Stimuli.Value[ 6 ][ idx ]      = ''     


# Sequence
taskseq = []
for img in range( 1, len(task_images) + 1 ):
    taskseq += [ 100 + img, img ]

seq = [ 301, 201, *taskseq, 202, 301 ]

param.Application.Sequencing.Sequence.Section      = 'Application'
param.Application.Sequencing.Sequence.Type         = 'intlist'
param.Application.Sequencing.Sequence.DefaultValue = '1'
param.Application.Sequencing.Sequence.LowRange     = '1'
param.Application.Sequencing.Sequence.HighRange    = ''
param.Application.Sequencing.Sequence.Comment      = 'Sequence in which stimuli are presented (deterministic mode)/ Stimulus frequencies for each stimulus (random mode)'
param.Application.Sequencing.Sequence.Value        = [ str( i ) for i in seq ]
#param.Application.Sequencing.Sequence.RowLabels = [ i + 1 for i in range( len( seq ) ) ]
#
# param.Application.Sequencing.Sequence.NumericValue = seq 

# UserComment
param.Application.Sequencing.UserComment.Section         = 'Application'
param.Application.Sequencing.UserComment.Type            = 'string'
param.Application.Sequencing.UserComment.DefaultValue    = ''
param.Application.Sequencing.UserComment.LowRange        = ''
param.Application.Sequencing.UserComment.HighRange       = ''
param.Application.Sequencing.UserComment.Comment         = 'User comments for a specific run'
param.Application.Sequencing.UserComment.Value           = settings.UserComment

#
param.Source[ "Signal Properties" ].SamplingRate.Section         = 'Source'
param.Source[ "Signal Properties" ].SamplingRate.Type            = 'int'
param.Source[ "Signal Properties" ].SamplingRate.DefaultValue    = '256Hz'
param.Source[ "Signal Properties" ].SamplingRate.LowRange        = '1'
param.Source[ "Signal Properties" ].SamplingRate.HighRange       = ''
param.Source[ "Signal Properties" ].SamplingRate.Comment         = 'sample rate'
param.Source[ "Signal Properties" ].SamplingRate.Value           = settings.SamplingRate

#
param.Source[ "Signal Properties" ].SampleBlockSize.Section         = 'Source'
param.Source[ "Signal Properties" ].SampleBlockSize.Type            = 'int'
param.Source[ "Signal Properties" ].SampleBlockSize.DefaultValue    = '8'
param.Source[ "Signal Properties" ].SampleBlockSize.LowRange        = '1'
param.Source[ "Signal Properties" ].SampleBlockSize.HighRange       = ''
param.Source[ "Signal Properties" ].SampleBlockSize.Comment         = 'number of samples transmitted at a time'
param.Source[ "Signal Properties" ].SampleBlockSize.Value           = settings.SampleBlockSize

#
param.Application.Sequencing.NumberOfSequences.Section         = 'Application'
param.Application.Sequencing.NumberOfSequences.Type            = 'int'
param.Application.Sequencing.NumberOfSequences.DefaultValue    = '1'
param.Application.Sequencing.NumberOfSequences.LowRange        = '0'
param.Application.Sequencing.NumberOfSequences.HighRange       = ''
param.Application.Sequencing.NumberOfSequences.Comment         = 'number of sequence repetitions in a run'
param.Application.Sequencing.NumberOfSequences.Value           = settings.NumberOfSequences

#
param.Application.Stimuli.StimulusWidth.Section         = 'Application'
param.Application.Stimuli.StimulusWidth.Type            = 'int'
param.Application.Stimuli.StimulusWidth.DefaultValue    = '0'
param.Application.Stimuli.StimulusWidth.LowRange        = ''
param.Application.Stimuli.StimulusWidth.HighRange       = ''
param.Application.Stimuli.StimulusWidth.Comment         = 'StimulusWidth in percent of screen width (zero for original pixel size)'
param.Application.Stimuli.StimulusWidth.Value           = settings.StimulusWidth

#
param.Application.Sequencing.SequenceType.Section              = 'Application'
param.Application.Sequencing.SequenceType.Type                 = 'int'
param.Application.Sequencing.SequenceType.DefaultValue         = '0'
param.Application.Sequencing.SequenceType.LowRange             = '0'
param.Application.Sequencing.SequenceType.HighRange            = '1'
param.Application.Sequencing.SequenceType.Comment              = 'Sequence of stimuli is 0 deterministic, 1 random (enumeration)'
param.Application.Sequencing.SequenceType.Value                = '0'

#
param.Application.Sequencing.StimulusDuration.Section           = 'Application'
param.Application.Sequencing.StimulusDuration.Type              = 'float'
param.Application.Sequencing.StimulusDuration.DefaultValue      = '40ms'
param.Application.Sequencing.StimulusDuration.LowRange          = '0'
param.Application.Sequencing.StimulusDuration.HighRange         = ''
param.Application.Sequencing.StimulusDuration.Comment           = 'stimulus duration'
param.Application.Sequencing.StimulusDuration.Value             = ''

#
param.Application.Sequencing.ISIMaxDuration.Section       = 'Application'
param.Application.Sequencing.ISIMaxDuration.Type          = 'float'
param.Application.Sequencing.ISIMaxDuration.DefaultValue  = '80ms'
param.Application.Sequencing.ISIMaxDuration.LowRange      = '0'
param.Application.Sequencing.ISIMaxDuration.HighRange     = ''
param.Application.Sequencing.ISIMaxDuration.Comment       = 'maximum duration of inter-stimulus interval'
param.Application.Sequencing.ISIMaxDuration.Value         = [ settings.ISIMaxDuration ]

#
param.Application.Sequencing.ISIMinDuration.Section       = 'Application'
param.Application.Sequencing.ISIMinDuration.Type          = 'float'
param.Application.Sequencing.ISIMinDuration.DefaultValue  = '80ms'
param.Application.Sequencing.ISIMinDuration.LowRange      = '0'
param.Application.Sequencing.ISIMinDuration.HighRange     = ''
param.Application.Sequencing.ISIMinDuration.Comment       = 'minimum duration of inter-stimulus interval'
param.Application.Sequencing.ISIMinDuration.Value         = [ settings.ISIMinDuration ]

#
param.Application.Sequencing.PreSequenceDuration.Section       = 'Application'
param.Application.Sequencing.PreSequenceDuration.Type          = 'float'
param.Application.Sequencing.PreSequenceDuration.DefaultValue  = '2s'
param.Application.Sequencing.PreSequenceDuration.LowRange      = '0'
param.Application.Sequencing.PreSequenceDuration.HighRange     = ''
param.Application.Sequencing.PreSequenceDuration.Comment       = 'pause preceding sequences/sets of intensifications'
param.Application.Sequencing.PreSequenceDuration.Value         = [ '0s' ]

#
param.Application.Sequencing.PostSequenceDuration.Section       = 'Application'
param.Application.Sequencing.PostSequenceDuration.Type          = 'float'
param.Application.Sequencing.PostSequenceDuration.DefaultValue  = '2s'
param.Application.Sequencing.PostSequenceDuration.LowRange      = '0'
param.Application.Sequencing.PostSequenceDuration.HighRange     = ''
param.Application.Sequencing.PostSequenceDuration.Comment       = 'pause following sequences/sets of intensifications'
param.Application.Sequencing.PostSequenceDuration.Value         = [ '0s' ]

#
param.Application.Sequencing.PreRunDuration.Section       = 'Application'
param.Application.Sequencing.PreRunDuration.Type          = 'float'
param.Application.Sequencing.PreRunDuration.DefaultValue  = '2000ms'
param.Application.Sequencing.PreRunDuration.LowRange      = '0'
param.Application.Sequencing.PreRunDuration.HighRange     = ''
param.Application.Sequencing.PreRunDuration.Comment       = 'pause preceding first sequence'
param.Application.Sequencing.PreRunDuration.Value         = [ settings.PreRunDuration ]

#
param.Application.Sequencing.PostRunDuration.Section       = 'Application'
param.Application.Sequencing.PostRunDuration.Type          = 'float'
param.Application.Sequencing.PostRunDuration.DefaultValue  = '2000ms'
param.Application.Sequencing.PostRunDuration.LowRange      = '0'
param.Application.Sequencing.PostRunDuration.HighRange     = ''
param.Application.Sequencing.PostRunDuration.Comment       = 'pause following last squence'
param.Application.Sequencing.PostRunDuration.Value         = [ settings.PostRunDuration ]


#
param.Application.Stimuli.BackgroundColor.Section      = 'Application'
param.Application.Stimuli.BackgroundColor.Type         = 'string'
param.Application.Stimuli.BackgroundColor.DefaultValue = '0x00FFFF00'
param.Application.Stimuli.BackgroundColor.LowRange     = '0x00000000'
param.Application.Stimuli.BackgroundColor.HighRange    = '0x00000000'
param.Application.Stimuli.BackgroundColor.Comment      = 'Color of stimulus background (color)'
param.Application.Stimuli.BackgroundColor.Value        = [ settings.BackgroundColor ]

#
param.Application.Stimuli.CaptionColor.Section      = 'Application'
param.Application.Stimuli.CaptionColor.Type         = 'string'
param.Application.Stimuli.CaptionColor.DefaultValue = '0x00FFFF00'
param.Application.Stimuli.CaptionColor.LowRange     = '0x00000000'
param.Application.Stimuli.CaptionColor.HighRange    = '0x00000000'
param.Application.Stimuli.CaptionColor.Comment      = 'Color of stimulus caption text (color)'
param.Application.Stimuli.CaptionColor.Value        = [ settings.CaptionColor ]

#
param.Application[ "Application Window" ].WindowBackgroundColor.Section      = 'Application'
param.Application[ "Application Window" ].WindowBackgroundColor.Type         = 'string'
param.Application[ "Application Window" ].WindowBackgroundColor.DefaultValue = '0x00FFFF00'
param.Application[ "Application Window" ].WindowBackgroundColor.LowRange     = '0x00000000'
param.Application[ "Application Window" ].WindowBackgroundColor.HighRange    = '0x00000000'
param.Application[ "Application Window" ].WindowBackgroundColor.Comment      = 'background color (color)'
param.Application[ "Application Window" ].WindowBackgroundColor.Value        = [ settings.WindowBackgroundColor ]

#
param.Application.Stimuli.IconSwitch.Section          = 'Application'
param.Application.Stimuli.IconSwitch.Type             = 'int'
param.Application.Stimuli.IconSwitch.DefaultValue     = '1'
param.Application.Stimuli.IconSwitch.LowRange         = '0'
param.Application.Stimuli.IconSwitch.HighRange        = '1'
param.Application.Stimuli.IconSwitch.Comment          = 'Present icon files (boolean)'
param.Application.Stimuli.IconSwitch.Value            = [ '1' ]

#
param.Application.Stimuli.AudioSwitch.Section         = 'Application'
param.Application.Stimuli.AudioSwitch.Type            = 'int'
param.Application.Stimuli.AudioSwitch.DefaultValue    = '1'
param.Application.Stimuli.AudioSwitch.LowRange        = '0'
param.Application.Stimuli.AudioSwitch.HighRange       = '1'
param.Application.Stimuli.AudioSwitch.Comment         = 'Present audio files (boolean)'
param.Application.Stimuli.AudioSwitch.Value           = [ '0' ]

#
param.Application.Stimuli.CaptionSwitch.Section       = 'Application'
param.Application.Stimuli.CaptionSwitch.Type          = 'int'
param.Application.Stimuli.CaptionSwitch.DefaultValue  = '1'
param.Application.Stimuli.CaptionSwitch.LowRange      = '0'
param.Application.Stimuli.CaptionSwitch.HighRange     = '1'
param.Application.Stimuli.CaptionSwitch.Comment       = 'Present captions (boolean)'
param.Application.Stimuli.CaptionSwitch.Value         = [ settings.CaptionSwitch ]

#
param.Application[ "Application Window" ].WindowHeight.Section        = 'Application'
param.Application[ "Application Window" ].WindowHeight.Type           = 'int'
param.Application[ "Application Window" ].WindowHeight.DefaultValue   = '480'
param.Application[ "Application Window" ].WindowHeight.LowRange       = '0'
param.Application[ "Application Window" ].WindowHeight.HighRange      = ''
param.Application[ "Application Window" ].WindowHeight.Comment        = 'height of application window'
param.Application[ "Application Window" ].WindowHeight.Value          = [ settings.WindowHeight ]

#
param.Application[ "Application Window" ].WindowWidth.Section        = 'Application'
param.Application[ "Application Window" ].WindowWidth.Type           = 'int'
param.Application[ "Application Window" ].WindowWidth.DefaultValue   = '480'
param.Application[ "Application Window" ].WindowWidth.LowRange       = '0'
param.Application[ "Application Window" ].WindowWidth.HighRange      = ''
param.Application[ "Application Window" ].WindowWidth.Comment        = 'width of application window'
param.Application[ "Application Window" ].WindowWidth.Value          = [ settings.WindowWidth ]

#
param.Application[ "Application Window" ].WindowLeft.Section        = 'Application'
param.Application[ "Application Window" ].WindowLeft.Type           = 'int'
param.Application[ "Application Window" ].WindowLeft.DefaultValue   = '0'
param.Application[ "Application Window" ].WindowLeft.LowRange       = ''
param.Application[ "Application Window" ].WindowLeft.HighRange      = ''
param.Application[ "Application Window" ].WindowLeft.Comment        = 'screen coordinate of application window''s left edge'
param.Application[ "Application Window" ].WindowLeft.Value          = [ settings.WindowLeft ]

#
param.Application[ "Application Window" ].WindowTop.Section        = 'Application'
param.Application[ "Application Window" ].WindowTop.Type           = 'int'
param.Application[ "Application Window" ].WindowTop.DefaultValue   = '0'
param.Application[ "Application Window" ].WindowTop.LowRange       = ''
param.Application[ "Application Window" ].WindowTop.HighRange      = ''
param.Application[ "Application Window" ].WindowTop.Comment        = 'screen coordinate of application window''s top edge'
param.Application[ "Application Window" ].WindowTop.Value          = [ settings.WindowTop ]

#
param.Application.Stimuli.CaptionHeight.Section      = 'Application'
param.Application.Stimuli.CaptionHeight.Type         = 'int'
param.Application.Stimuli.CaptionHeight.DefaultValue = '0'
param.Application.Stimuli.CaptionHeight.LowRange     = '0'
param.Application.Stimuli.CaptionHeight.HighRange    = '100'
param.Application.Stimuli.CaptionHeight.Comment      = 'Height of stimulus caption text in percent of screen height'
param.Application.Stimuli.CaptionHeight.Value        = [ '5' ]

#
#param.Filtering.ExpressionFilter.WarningExpression.Section      = 'Filtering'
#param.Filtering.ExpressionFilter.WarningExpression.Type         = 'string'
#param.Filtering.ExpressionFilter.WarningExpression.DefaultValue = ''
#param.Filtering.ExpressionFilter.WarningExpression.LowRange     = ''
#param.Filtering.ExpressionFilter.WarningExpression.HighRange    = ''
#param.Filtering.ExpressionFilter.WarningExpression.Comment      = 'expression that results in a warning when it evaluates to true'
#param.Filtering.ExpressionFilter.WarningExpression.Value        = [ '' ]
#
##
#param.Filtering.ExpressionFilter.Expressions.Section      = 'Filtering'
#param.Filtering.ExpressionFilter.Expressions.Type         = 'matrix'
#param.Filtering.ExpressionFilter.Expressions.DefaultValue = ''
#param.Filtering.ExpressionFilter.Expressions.LowRange     = ''
#param.Filtering.ExpressionFilter.Expressions.HighRange    = ''
#param.Filtering.ExpressionFilter.Expressions.Comment      = 'expressions used to compute the output of the ExpressionFilter'
#param.Filtering.ExpressionFilter.Expressions.Value        = [ '' ]

#
param.Storage.Session.SubjectName.Section      = 'Storage'
param.Storage.Session.SubjectName.Type         = 'string'
param.Storage.Session.SubjectName.DefaultValue = 'Name'
param.Storage.Session.SubjectName.LowRange     = ''
param.Storage.Session.SubjectName.HighRange    = ''
param.Storage.Session.SubjectName.Comment      = 'subject alias'
param.Storage.Session.SubjectName.Value        = [ settings.SubjectName ]

#
param.Storage[ "Data Location" ].DataDirectory.Section      = 'Storage'
param.Storage[ "Data Location" ].DataDirectory.Type         = 'string'
param.Storage[ "Data Location" ].DataDirectory.DefaultValue = os.path.join( "..", "data" )
param.Storage[ "Data Location" ].DataDirectory.LowRange     = ''
param.Storage[ "Data Location" ].DataDirectory.HighRange    = ''
param.Storage[ "Data Location" ].DataDirectory.Comment      = 'path to top level data directory (directory)'
param.Storage[ "Data Location" ].DataDirectory.Value        = [ settings.DataDirectory ]

param.Storage.Session.SubjectRun.Section      = 'Storage'
param.Storage.Session.SubjectRun.Type         = 'string'
param.Storage.Session.SubjectRun.DefaultValue = '00'
param.Storage.Session.SubjectRun.LowRange     = ''
param.Storage.Session.SubjectRun.HighRange    = ''
param.Storage.Session.SubjectRun.Comment      = 'two-digit run number'
param.Storage.Session.SubjectRun.Value        = [ settings.SubjectRun ]

#
param.Storage.Session.SubjectSession.Section      = 'Storage'
param.Storage.Session.SubjectSession.Type         = 'string'
param.Storage.Session.SubjectSession.DefaultValue = '00'
param.Storage.Session.SubjectSession.LowRange     = ''
param.Storage.Session.SubjectSession.HighRange    = ''
param.Storage.Session.SubjectSession.Comment      = 'three-digit session number'
param.Storage.Session.SubjectSession.Value        = [ settings.SubjectSession ]

print( "----Done Setting Params----" )
print( "----Converting Params----" )

convert_bciprm( param, settings.parm_filename )

print( f"----Params Saved to { settings.parm_filename }----" )


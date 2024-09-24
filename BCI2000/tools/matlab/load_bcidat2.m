function s = load_bcidat2(varargin)
% LOAD_BCIDAT2    Load a BCI2000 data file.
% 
% LOAD_BCIDAT2 wraps the mex-file LOAD_BCIDAT for the sake of a simpler
% syntax: it only takes a list of data files, and has a single return
% value, which is a struct with fields .States, .Signal, and .Parms.
% Also, it always returns calibrated data (with SourcheChOffset
% and SourceChGain applied).
% 
% S = LOAD_BCIDAT2(FILENAME, FILENAME2, ...)
%   where S is a struct
%         S.States is a nested struct that contains state values
%         S.Signal contains the numeric signal (samples-by-channels)
%         S.Parms will be a struct containing parameter definitions
% 
% LOAD_BCIDAT2 has the following dependencies:
% 
%  Mex-files: load_bcidat

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% $Id: load_bcidat2.m 7465 2023-06-30 15:23:43Z mellinger $ 
%% Author: mellinger@neurotechcenter.org
%%
%% $BEGIN_BCI2000_LICENSE$
%% 
%% This file is part of BCI2000, a platform for real-time bio-signal research.
%% [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
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

if isempty(varargin)
    error('Provide one or more filenames.');
end

[s.Signal, s.States, s.Parms] = load_bcidat('-calibrated', varargin{:});


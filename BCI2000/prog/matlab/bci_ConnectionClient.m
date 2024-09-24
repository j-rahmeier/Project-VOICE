function bci_ConnectionClient

% This function opens a connection to BCI2000 and executes commands it
% receives. It is a replacement for the Matlab engine, avoiding the
% intransparency and confusion that arise from the Matlab engine's COM
% approach on Windows.
%
% BCI2000 filter interface for Matlab
% mellinger@neurotechcenter.org, 2020
% $BEGIN_BCI2000_LICENSE$
% 
% This file is part of BCI2000, a platform for real-time bio-signal research.
% [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
% 
% BCI2000 is free software: you can redistribute it and/or modify it under the
% terms of the GNU General Public License as published by the Free Software
% Foundation, either version 3 of the License, or (at your option) any later
% version.
% 
% BCI2000 is distributed in the hope that it will be useful, but
%                         WITHOUT ANY WARRANTY
% - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
% A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
% 
% You should have received a copy of the GNU General Public License along with
% this program.  If not, see <http://www.gnu.org/licenses/>.
% 
% $END_BCI2000_LICENSE$

import java.net.Socket
import java.io.*

bci_Debug = false;

bci_Socket = Socket('127.0.0.1', 3997);
bci_Reader = BufferedReader(InputStreamReader(bci_Socket.getInputStream));
bci_Writer = OutputStreamWriter(bci_Socket.getOutputStream);

fprintf(1, 'Processing input from BCI2000 ...\n');

bci_Done = false;
bci_Count = 0;
while ~bci_Done
    try
        bci_Line = char(bci_Reader.readLine());
        bci_Count = bci_Count + 1;
    catch, java.net.SocketException;
        bci_Line = [];
    end
    if isempty(bci_Line) % other side closed connection
        bci_Command = 'q:';
        bci_Arg = [];
    else
        bci_Command = bci_Line(1:2);
        bci_Arg = bci_Line(3:end);
    end
    if bci_Debug
        fprintf(1, '%s -- %s\n', bci_Command, bci_Arg);
    end
    if mod(bci_Count, 80) == 0
        fprintf(1, '\n%d\n', bci_Count);
    end
    fprintf(1, '.');
    switch bci_Command
        case 'x:' % execute
            eval(bci_Arg);
        case 'g:' % get integer matrix
            bci_Ans = eval(bci_Arg);
            bci_Ans = sprintf('%d ', size(bci_Ans), bci_Ans);
            if bci_Debug
                fprintf(1, '-> %s\n', bci_Ans);
            end
            bci_Writer.write(bci_Ans);
            bci_Writer.flush();
        case 'p:' % pause
            fprintf(1, '\bP');
            bci_Socket.setSoTimeout(100);
            bci_Line = [];
            while isempty(bci_Line)
                try
                    bci_Line = char(bci_Reader.readLine());
                catch, java.net.SocketTimeoutException;
                    drawnow limitrate
                end
            end
            bci_Socket.setSoTimeout(0);
            assert(strcmp(bci_Line, 'c:'), ['bci_Line: ' bci_Line]);
        case 'c:' % continue
        case 'q:' % quit processing loop
            bci_Done = true;
            fprintf(1, '\nConnection closed.\n');
        case 'Q:' % quit Matlab
            exit();
        otherwise
            error(['unexpected command: ' bci_Command]);
    end
    drawnow limitrate;
end

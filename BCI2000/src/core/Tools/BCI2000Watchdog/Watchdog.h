//////////////////////////////////////////////////////////////////////
// $Id$
// Authors: mellinger@neurotechcenter.org
// Description: A class for watching a BCI2000 instance.
//
// $BEGIN_BCI2000_LICENSE$
//
// This file is part of BCI2000, a platform for real-time bio-signal research.
// [ Copyright (C) 2000-2023: BCI2000 team and many external contributors ]
//
// BCI2000 is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// BCI2000 is distributed in the hope that it will be useful, but
//                         WITHOUT ANY WARRANTY
// - without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.
//
// $END_BCI2000_LICENSE$
///////////////////////////////////////////////////////////////////////
#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <string>
#include <vector>

class Watchdog
{
public:
    Watchdog();
    ~Watchdog();

    Watchdog(const Watchdog&) = delete;
    Watchdog& operator=(const Watchdog&) = delete;

    enum StateEnum
    {
        Idle = 0,
        UserStartup = 1,
        Watching = 2,
        Triggered = 3,
        UserTerminating = 4,
        TriggeredTerminating = 5,
        TriggeredStartup = 6,
        Error = 7,
    };
    StateEnum State() const;

    enum MessageType
    {
        LogMessage,
        Log2Message,
        ErrorMessage,
        TriggeredMessage,
    };

    struct Listener
    {
        virtual ~Listener() {}
        virtual void OnLogMessage(MessageType, const std::string&) = 0;
    };
    void SetListener(Listener*);
    Listener* GetListener() const;

    struct Watch
    {
        std::string expression;
        double initialTimeoutSeconds, timeoutSeconds;
    };
    void SetWatches(const std::vector<Watch>&);
    const std::vector<Watch>& Watches() const;

    void SetBatchfile(const std::string&);
    const std::string& Batchfile() const;

    void SetRunningTimeout(double seconds);
    double RunningTimeout() const;

    void SetProcessesTimeout(double seconds);
    double ProcessesTimeout() const;

    void StartBCI2000();
    void TerminateBCI2000();
    void Reset();

private:
    struct Private;
    Private* p;
};

#endif // WATCHDOG_H

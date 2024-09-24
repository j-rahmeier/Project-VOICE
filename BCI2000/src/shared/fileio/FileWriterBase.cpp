////////////////////////////////////////////////////////////////////////////////
// $Id: FileWriterBase.cpp 8071 2024-04-24 18:34:36Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A base class that implements functionality common to all
//              file writer classes that output into a file.
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
////////////////////////////////////////////////////////////////////////////////
#include "FileWriterBase.h"

#include "BCIStream.h"
#include "ClassName.h"
#include "FileUtils.h"
#include "RunManager.h"
#include "BCIEvent.h"

#include <fstream>
#include <iomanip>
#include <iostream>

static const int bufferSize = 65536;
static const char *bciParameterExtension = ".prm";

static std::string ParameterFile(const std::string &inDataFile)
{
    return FileUtils::ExtractDirectory(inDataFile) + FileUtils::ExtractBase(inDataFile) + bciParameterExtension;
}

FileWriterBase::FileWriterBase(GenericOutputFormat &inOutputFormat)
    : mrOutputFormat(inOutputFormat), mStreambuf(0, bufferSize), mOutputFile(&mStreambuf),
      mFilePart(1)
{
}

FileWriterBase::~FileWriterBase()
{
    Halt();
}

void FileWriterBase::Publish()
{
    mrOutputFormat.Publish();

    std::string ext = mrOutputFormat.DataFileExtension();
    size_t i = 0;
    while (i < ext.length() && ::ispunct(ext[i]))
        ++i;
    std::string formatName = ext.substr(i);

    if (Parameters->Exists("/FileFormat"))
        Parameters->Delete("/FileFormat");
    std::string def = "Storage string /FileFormat= " + formatName + " % % % // format of data file (readonly)";
    BEGIN_PARAMETER_DEFINITIONS
      def.c_str(),
    END_PARAMETER_DEFINITIONS

    if (OptionalParameter("SavePrmFile") != 0)
    {
        BEGIN_PARAMETER_DEFINITIONS
            "Storage:Documentation int SavePrmFile= 0 1 0 1 "
               "// save additional parameter file for each run (0=no, 1=yes) (boolean)",
        END_PARAMETER_DEFINITIONS
    }

    BEGIN_PARAMETER_DEFINITIONS
        "Storage:Documentation string /StorageTime= % % % % "
          "// time of beginning of data storage",

        "Storage:FileSplitting string FileSplittingCondition= % % % % "
          "// when to split files, e.g. \"1:25:00\" or \"1.5GB\"",
    END_PARAMETER_DEFINITIONS

    BEGIN_EVENT_DEFINITIONS
        "FilePart 32 1",
    END_EVENT_DEFINITIONS
}

void FileWriterBase::Preflight(const SignalProperties &Input, SignalProperties &Output) const
{
    mrOutputFormat.Preflight(Input, *Statevector);

    // Test file splitting condition.
    std::string error = FileSplittingCondition().Compile(Parameter("FileSplittingCondition")).Error();
    if (!error.empty())
        bcierr << "Invalid FileSplittingCondition: " << error;

    // State availability.
    State("SourceTime");

    // File accessibility.
    std::string dataFile = RunManager()->CurrentRun();
    if (OptionalParameter("SavePrmFile") == 1)
    {
        std::string paramFile = ParameterFile(dataFile);
        if (FileUtils::Exists(paramFile))
            bcierr << "Parameter file \"" << paramFile << "\" already exists, "
                   << "will not be touched.";
        else if (!FileUtils::IsWritableLocation(paramFile))
            bcierr << "Cannot write to file location \"" << paramFile << "\"";
    }

    if (!std::string(Parameter("StorageTime")).empty())
        bciout << "The StorageTime parameter will be overwritten with the"
               << " recording's actual date and time";

    Output = SignalProperties(0, 0);
}

void FileWriterBase::Initialize(const SignalProperties &Input, const SignalProperties & /*Output*/)
{
    mFileSplittingCondition.Compile(Parameter("FileSplittingCondition"));
    mrOutputFormat.Initialize(Input, *Statevector);
}

void FileWriterBase::BeginFile()
{
    // This will be called from a separate thread, so we need to set up context explicitly.
    ErrorContext("BeginFile", this);
    time_t now = ::time(nullptr);
    struct tm* timeinfo = ::localtime(&now);
    char buffer[20];
    ::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", timeinfo);

    std::lock_guard<std::mutex> lock(mMutex);
    mParameters("/StorageTime") = buffer;

    mOutputFile.clear();
    mStreambuf.Reset();
    auto& fileObject = RunManager()->CurrentFile();
    mStreambuf.SetOutput(&fileObject.Output());
    mrOutputFormat.BeginFile(mOutputFile, mParameters, mStates);
    ErrorContext("");
}

void FileWriterBase::EndFile()
{
    // This will be called from a separate thread, so we need to set up context explicitly.
    ErrorContext("EndFile", this);
    mrOutputFormat.EndFile(mOutputFile);
    mOutputFile.flush();
    mStreambuf.SetOutput(nullptr);
    mOutputFile.clear();
    RunManager()->CurrentFile().Close();
    ErrorContext("");
}

void FileWriterBase::StartRun()
{
    if (OptionalParameter("SavePrmFile") == 1)
    {
        std::string paramFile = ParameterFile(RunManager()->CurrentRun());
        std::ofstream file(paramFile);
        if (!(file << *Parameters << std::flush))
            bcierr << "Error writing parameters to file " << paramFile;
    }

    {
        std::lock_guard<std::mutex> lock(mMutex);
        mParameters = *Parameters; // create a copy that can be used inside the writer thread
        mStates = *States; // dito
    }

    mFilePart = 1;
    BeginFile();
    mQueue.Clear();
    Thread::Start();
}

void FileWriterBase::StopRun()
{
    Thread::Terminate();
    if (!mQueue.Empty())
        bcierr << "Nonempty buffering queue";
    EndFile();
}

void FileWriterBase::Halt()
{
    Thread::Terminate();
}

void FileWriterBase::Write(const GenericSignal &Signal, const StateVector &Statevector)
{
    mQueue.Produce(std::make_pair(Signal, Statevector));
}

int FileWriterBase::OnExecute()
{
    std::string fileName = RunManager()->CurrentRun();
    TimeUtils::Time startTime = TimeUtils::SystemTimeUTC();

    Queue::Consumable c;
    while (mOutputFile && (c = mQueue.AwaitConsumption(Time::Forever())))
    {   // When a data block with incremented FilePart event arrives, begin a new file.
        // By interposing a FilePart event, we allow other parts of the system to
        // begin new files in a synchronized manner.
        // To catch all changes in the FilePart event, evaluate it at the last sample
        // of the state vector.
        Assert(c->second.Samples() > 0);
        int lastSample = c->second.Samples() - 1;
        int nextFilePart = c->second.StateValue("FilePart", lastSample);

        if (nextFilePart > mFilePart)
        {
            // Close the current file.
            EndFile();
            mFilePart = nextFilePart;
            startTime = TimeUtils::SystemTimeUTC();
            // Ask the RunManager to create the next file.
            fileName = RunManager()->BeginNextFilePart();
            // Set everything up for the new file, and write the header.
            BeginFile();
        }

        int64_t pos1 = mOutputFile.tellp();
        mrOutputFormat.Write(mOutputFile, c->first, c->second);
        int64_t blockSize = mOutputFile.tellp() - pos1;
        mOutputFile.flush();

        TimeUtils::TimeInterval timeElapsed = TimeUtils::SystemTimeUTC() - startTime;
        // When evaluating file size, we need to take account for two blocks of delay
        // between broadcasting an event, and its arrival at the write queue.
        int64_t fileSize = mOutputFile.tellp() + 2 * blockSize;
        if (mFileSplittingCondition.Evaluate(timeElapsed, fileSize))
        {
            // When the file splitting condition evaluates to true, do not act directly but
            // broadcast an increment of the FilePart event.
            if (!mOutputFile)
                bcierr << "Error writing to file \"" << fileName << "\"";
            // When sending the event, make sure to set it at a block boundary.
            // This is achieved by associating it with the current block's SourceTime stamp.
            PrecisionTime timeStamp = c->second.StateValue("SourceTime");
            BCIEvent(timeStamp) << "FilePart " << mFilePart + 1;
        }
    }
    while ((c = mQueue.Consume()))
        mrOutputFormat.Write(mOutputFile, c->first, c->second);
    mOutputFile.flush();
    if (!mOutputFile)
        bcierr << "Error writing to file \"" << fileName << "\"";
    return 0;
}

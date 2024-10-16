/////////////////////////////////////////////////////////////////////////////
// $Id: SecondaryBCI2000Controller.cpp 8335 2024-09-02 16:39:42Z mellinger $
// Author: mellinger@neurotechcenter.org
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
/////////////////////////////////////////////////////////////////////////////
#include "SecondaryBCI2000Controller.h"
#include "FileUtils.h"
#include "RunManager.h"
#include <thread>

Extension( SecondaryBCI2000Controller );

SecondaryBCI2000Controller::SecondaryBCI2000Controller()
: mEnabled(false), mConnected(false), mFilePart(0)
{
    PublishEnabler("SecondaryBCI2000");
}

SecondaryBCI2000Controller::~SecondaryBCI2000Controller()
{
}

void SecondaryBCI2000Controller::Publish()
{
  mEnabled = (OptionalParameter("SecondaryBCI2000", 0) != 0);
  if (!mEnabled)
    return;

  BEGIN_PARAMETER_DEFINITIONS
    "Source:Secondary%20BCI2000 int /SecondaryBCI2000= 1 0 0 1"
      " // Control a second BCI2000 instance (boolean)",

    "Source:Secondary%20BCI2000 int SecondaryBCI2000TelnetPort= 4999 0 0 65535"
      " // Telnet port for communication with Secondary instance",

    "Source:Secondary%20BCI2000 int SecondaryBCI2000StartupScript= % % %"
      " // Script file to start up secondary Secondary instance (file)",

  END_PARAMETER_DEFINITIONS
}

void SecondaryBCI2000Controller::AutoConfig()
{
  if (!mEnabled)
    return;

  std::string operatorPath = FileUtils::ParentDirectory(FileUtils::ExecutablePath()) + "Operator";
  mConnection.OperatorPath(operatorPath);
  mConnection.OperatorOptions("--AllowMultipleInstances --InstancePrefix Secondary");
  mConnection.TelnetAddress("localhost:" + std::string(Parameter("SecondaryBCI2000TelnetPort")));
  mConnection.WindowVisible(false);
  mConnection.Connect();
  mConnected = mConnection.Connected();
  if (!mConnected)
    bcierr << "Could not start up or connect to secondary BCI2000 instance: " << mConnection.Result();
  if (mConnected)
  {
    std::string script = Parameter("SecondaryBCI2000StartupScript");
    std::string command = "EXECUTE SCRIPT \"" + script + "\"";
    int result = 0;
    mConnection.Execute(command, &result);
    if (result != 0)
    {
        bcierr << "Error when executing startup script: " << mConnection.Result();
        mConnection.Disconnect();
        mConnected = false;
    }
  }
}

void SecondaryBCI2000Controller::Preflight() const
{
  if (!mEnabled || !mConnected)
    return;

  for (auto p : { "SubjectName", "SubjectSession", "SubjectRun", "DataDirectory", "DataFile", "DataFileShm" })
  {
      std::string value = Parameter(p);
      mConnection.SetParameter(p, value);
  }

  // Make sure the secondary system does not do file splitting by itself. Instead, we will send
  // updates to the FilePart event, which in turn will cause the next file part to begin.
  mConnection.SetParameter("FileSplittingCondition", "");

  // Make sure current run is updated before SetConfig() is called in secondary instance.
  RunManager()->CurrentRun();
  if (!mConnection.SetConfig())
    bcierr << "Could not set configuration on secondary instance: " << mConnection.Result();

  State("FilePart");
}

void SecondaryBCI2000Controller::Initialize()
{
  if (!mEnabled || !mConnected)
    return;
}

void SecondaryBCI2000Controller::StartRun()
{
  if (!mEnabled || !mConnected)
    return;

  mFilePart = 1;

  if (!mConnection.Start())
    bcierr << "Could not start run on secondary instance: " << mConnection.Result();
}

void SecondaryBCI2000Controller::StopRun()
{
  if (!mEnabled || !mConnected)
    return;

  mFilePart = 0;

  if (!mConnection.Stop())
    bcierr << "Could not stop run on secondary instance: " << mConnection.Result();
}

void SecondaryBCI2000Controller::Process()
{
  if (!mEnabled || !mConnected || mFilePart < 1)
    return;

  int nextFilePart = State("FilePart")(Statevector->Samples() - 1);
  if (nextFilePart > mFilePart)
  {
    mFilePart = nextFilePart;
    if (!mConnection.SetEventVariable("FilePart", nextFilePart))
      bcierr << "Could not set file part on secondary instance: " << mConnection.Result();
  }
}

void SecondaryBCI2000Controller::Halt()
{
  if (!mEnabled || !mConnected)
    return;

  mConnection.Stop();
  mConnection.Disconnect();
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

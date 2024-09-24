////////////////////////////////////////////////////////////////////////////////
// $Id: MatlabWrapper.h 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: Wrapper classes for convenient creation and manipulation of
//              Matlab workspace variables, and calling of Matlab functions.
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
#ifndef MatlabWrapperH
#define MatlabWrapperH

#include "GenericSignal.h"
#include "Param.h"
#include "Sockets.h"
#include <string>
#include <vector>

class MatlabConnection
{
  public:
    class IntMatrix : public std::vector<std::vector<int>>
    {
      public:
        IntMatrix()
        {
        }
        IntMatrix(int i) : std::vector<std::vector<int>>(1, std::vector<int>(1, i))
        {
        }
        IntMatrix(int rows, int cols);
        int rows() const;
        int cols() const;

        static IntMatrix fromSignalProperties(const SignalProperties &);
        SignalProperties toSignalProperties() const;
    };

    class StringMatrix : public std::vector<std::vector<std::string>>
    {
      public:
        StringMatrix()
        {
        }
        StringMatrix(const std::string &s) : std::vector<std::vector<std::string>>(1, std::vector<std::string>(1, s))
        {
        }
        StringMatrix(int rows, int cols);
        int rows() const;
        int cols() const;

        static StringMatrix fromParam(const Param &);
    };

  protected:
    MatlabConnection();
    virtual ~MatlabConnection();

  public:
    static std::string FindMatlab();

    static void Open(const std::string &executable);
    static bool IsOpen();
    static void Close(bool closeMatlab);

    static bool Pause();
    static bool Continue();

    static bool Execute(const std::string &commands);
    static bool Execute(const std::string &commands, std::string &error);

    static bool CreateGlobal(const std::string &name);
    static bool ClearVariable(const std::string &name);
    static bool ClearObject(const std::string &name);

    static std::string GetString(const std::string &expr);
    static bool PutString(const std::string &expr, const std::string &value);

    static int GetIntScalar(const std::string &expr);
    static bool PutIntScalar(const std::string &expr, int value);

    static IntMatrix GetIntMatrix(const std::string &expr);
    static bool PutIntMatrix(const std::string &expr, const IntMatrix &value);

    static StringMatrix GetCells(const std::string &expr);
    static bool PutCells(const std::string &expr, const StringMatrix &value);

  protected:
    static Socket *spMatlabSocket;
    static int sNumInstances;
};

class MatlabFunction : private MatlabConnection
{
  public:
    MatlabFunction(const std::string &);
    ~MatlabFunction();

  private:
    MatlabFunction(const MatlabFunction &);
    MatlabFunction &operator=(const MatlabFunction &);

  public:
    MatlabFunction &InputArgument(const std::string &);
    MatlabFunction &OutputArgument(const std::string &);
    MatlabFunction &CodePre(const std::string &);
    MatlabFunction &CodePost(const std::string &);
    const std::string &Name() const
    {
        return mName;
    }
    bool Exists() const
    {
        return mExists;
    }
    std::string Execute() const;

  private:
    std::vector<std::string> mInputArguments, mOutputArguments;
    std::string mName, mCodePre, mCodePost;
    bool mExists;
};

#endif // MatlabWrapperH

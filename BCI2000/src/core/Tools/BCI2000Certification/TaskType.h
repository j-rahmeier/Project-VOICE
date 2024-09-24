////////////////////////////////////////////////////////////////////////////////
// $Id: TaskType.h 7520 2023-08-04 17:40:46Z mellinger $
// Authors: Adam Wilson, juergen.mellinger@neurotechcenter.org
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
#ifndef TaskTypeH
#define TaskTypeH

#include "Property.h"

template <class T> struct List : std::vector<T>
{
    List()
    {
    }
    explicit List(const std::string &s)
    {
        std::istringstream iss(s);
        unserialize(iss);
    }
    std::ostream &serialize(std::ostream &os) const
    {
        if (!std::vector<T>::empty())
            os << std::vector<T>::front();
        for (size_t i = 1; i < std::vector<T>::size(); ++i)
            os << " " << (*this)[i];
        return os;
    }
    std::istream &unserialize(std::istream &is)
    {
        std::vector<T>::clear();
        T t;
        while (is >> t)
            std::vector<T>::push_back(t);
        if (is.eof() && !std::vector<T>::empty())
            is.clear(is.eofbit);
        return is;
    }
};

template <class T> static std::ostream &operator<<(std::ostream &os, const List<T> &t)
{
    return t.serialize(os);
}
template <class T> static std::istream &operator>>(std::istream &is, List<T> &t)
{
    return t.unserialize(is);
}

//=====================================

class TaskType : public Property::Holder<TaskType>
{
  public:
    TaskType();
    ~TaskType();

    bool operator==(const TaskType &) const;

    PropertyMember(Name, Tiny::String);
    PropertyMember(ParmFiles, List<Tiny::String>);
    PropertyMember(Source, Tiny::String);
    PropertyMember(SigProc, Tiny::String);
    PropertyMember(App, Tiny::String);
    PropertyMember(VidStateName, Tiny::String);
    PropertyMember(AudStateName, Tiny::String);
    PropertyMember(VidStateValues, List<int>);
    PropertyMember(AudStateValues, List<int>);
    PropertyMember(BlockSize, int);
    PropertyMember(SampleRate, float);
    PropertyMember(VidFrameTest, bool);
    PropertyMember(Skip, bool);
    PropertyMembersEnd();

  private:
    void validateName(const Tiny::String *);
    void validateStateName(const Tiny::String *);
    void validateStateValues(const List<int> *);
    void validateBlockSize(const int *);
    void validateSampleRate(const float *);
    void validateExecutable(const Tiny::String *);
    void validateExecutableNonempty(const Tiny::String *);

  public:
    void addParm(const Tiny::String &);
    void delParm(int);
    const List<Tiny::String> &getParmFilesDisp()
    {
        return mParmFilesDisp;
    };
    bool hasErrors() const;
    std::string toInputScript() const;
    std::ostream &serialize(std::ostream &) const;
    std::istream &unserialize(std::istream &);

  private:
    List<Tiny::String> mParmFilesDisp;
};

inline std::ostream &operator<<(std::ostream &os, const TaskType &t)
{
    return t.serialize(os);
}
inline std::istream &operator>>(std::istream &is, TaskType &t)
{
    return t.unserialize(is);
}

class Config : public Property::Holder<Config>
{
  public:
    Config();
    ~Config();

    PropertyMember(Tasks, Property::Container<TaskType>);
    PropertyMember(RootDir, Tiny::String);
    PropertyMember(DataDir, Tiny::String);
    PropertyMember(DefaultSource, Tiny::String);
    PropertyMember(WindowLeft, int);
    PropertyMember(WindowTop, int);
    PropertyMember(WindowWidth, int);
    PropertyMember(WindowHeight, int);
    PropertyMember(AppAddArguments, Tiny::String);
    PropertyMember(ExecuteBefore, Tiny::String);
    PropertyMember(ExecuteAfter, Tiny::String);
    PropertyMember(AutoCfg, bool);
    PropertyMember(AutoStart, bool);
    PropertyMember(AutoQuit, bool);
    PropertyMember(AmpChannel, int);
    PropertyMember(AmpStream, Tiny::String);
    PropertyMember(VidChannel, int);
    PropertyMember(VidStream, Tiny::String);
    PropertyMember(AudChannel, int);
    PropertyMember(AudStream, Tiny::String);
    PropertyMembersEnd();

  private:
    void validateTaskName(const Tiny::String *);
    void validateTasks(const Property::Container<TaskType> *);
    void validateRootDir(const Tiny::String *);
    void validateDataDir(const Tiny::String *);
    void validateExecutable(const Tiny::String *);
    void validateWindowSize(const int *);
    void validateChannel(const int*);
    void validateStream(const Tiny::String*);

  public:
    bool readFromFile(const Tiny::String &);
    bool writeToFile(const Tiny::String &);
    std::string toInputScript() const;
    const Tiny::String &fileName() const
    {
        return mFileName;
    }

    TaskType *taskByName(const Tiny::String &);
    void remove(TaskType *);
    void move(TaskType *, int);
    bool tasksModified() const;
    int activeTasks() const;
    bool isValidExecutable(const Tiny::String &) const;

  private:
    Tiny::String mFileName;
};

#undef PROPERTY

//---------------------------------------------------------------------------
#endif // TaskTypeH

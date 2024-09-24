// clang-format off
#include "ExceptionCatcher.h"
#include "ProcessUtils.h"
#include "QtMain.h"
#include "configdialog.h"
#include "CmdLine.h"
// clang-format on

int P300ClassifierMain(int, char **);

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(configdialog);

    ProcessUtils::InitializeDesktopIntegration("org.bci2000.Tools");
    ProcessUtils::InitializeShellIntegration("../../prog/BCI2000ShellExtension");
    QtApplication app(argc, argv);
    FunctionCall<int(int, char **)> call(P300ClassifierMain, argc, argv);
    bool finished = ExceptionCatcher().SetMessage("Aborting").Run(call);
    return finished ? call.Result() : -1;
}

int P300ClassifierMain(int argc, char **argv)
{
    ConfigDialog dialog;
    dialog.show();
    CCmdLine cmdLine;
    cmdLine.SplitLine(argc, argv);

    QStringList arg_TrainingDataFilesList;
    if (cmdLine.HasSwitch("-TrainingDataFiles"))
    {
        for (int i = 0; i < cmdLine.GetArgumentCount("-TrainingDataFiles"); i++)
        {
            QString arg = QString::fromLocal8Bit(cmdLine.GetArgument("-TrainingDataFiles", i).c_str());
            arg_TrainingDataFilesList.push_back(arg);
        }
    }

    QStringList arg_TestingDataFilesList;
    if (cmdLine.HasSwitch("-TestingDataFiles"))
    {
        for (int i = 0; i < cmdLine.GetArgumentCount("-TestingDataFiles"); i++)
        {
            QString arg = QString::fromLocal8Bit(cmdLine.GetArgument("-TestingDataFiles", i).c_str());
            arg_TestingDataFilesList.push_back(arg);
        }
    }

    QString arg_inicfg;
    if (cmdLine.HasSwitch("-inicfg"))
    {
        arg_inicfg = QString::fromLocal8Bit(cmdLine.GetArgument("-inicfg", 0).c_str());
    }

    QString classifierOutputFile;
    if (cmdLine.HasSwitch("-ClassifierOutputFile"))
    {
        classifierOutputFile = QString::fromLocal8Bit(cmdLine.GetArgument("-ClassifierOutputFile", 0).c_str());
    }
    dialog.SetFiles(arg_TrainingDataFilesList, arg_TestingDataFilesList, arg_inicfg, classifierOutputFile);

    return dialog.exec();
}

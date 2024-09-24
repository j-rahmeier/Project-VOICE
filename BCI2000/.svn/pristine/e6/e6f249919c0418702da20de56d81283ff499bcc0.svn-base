#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include "pages.h"
#include <QDialog>
#include <QString>

class QListWidget;
class QListWidgetItem;
class QStackedWidget;

class ConfigDialog : public QDialog
{
    Q_OBJECT

  public:
    ConfigDialog();

  public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
    void SetFiles(const QStringList &trainingDataFiles, const QStringList &testingDataFiles, 
                  const QString &inicfgFile, const QString &classifierOutputFile);

  private:
    void createIcons();

    QListWidget *mpContentsWidget;
    QStackedWidget *mpPagesWidget;

    DataPage *mpDataPage;
    ParametersPage *mpParametersPage;
    DetailsPage *mpDetailsPage;
};

#endif

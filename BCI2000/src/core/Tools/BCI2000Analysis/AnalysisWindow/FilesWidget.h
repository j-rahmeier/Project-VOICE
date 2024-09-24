////////////////////////////////////////////////////////////////////////////////
// $Id: FilesWidget.h 7463 2023-06-30 14:59:17Z mellinger $
// Author: mellinger@neurotechcenter.org
// Description: A window containing a BCI2000 data file list.
//   Data files are tested for consistency.
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
#include "BCI2000FileReader.h"
#include <QWidget>
#include <vector>

class FilesWidget : public QWidget
{
    Q_OBJECT

  public:
    FilesWidget(QWidget *);
    ~FilesWidget();
    FilesWidget(FilesWidget &) = delete;
    FilesWidget &operator=(FilesWidget &) = delete;

    std::vector<BCI2000FileReader *> data() const;
    typedef std::vector<std::pair<std::string, std::string>> FilesList;
    FilesList files() const;
    void setFiles(const FilesList &);
    void setReferenceFile(const BCI2000FileReader *);
    const BCI2000FileReader *referenceFile() const;

    enum FileClassification
    {
        Undefined,
        Unknown,
        SMR, SMR2,
        ERP
    };
    static FileClassification classifyFile(const BCI2000FileReader *);

  signals:
    void dataChanged() const;

  public slots:
    void addFile();
    void addFiles(const QStringList&);

  private:
    struct Private;
    Private *p;
};

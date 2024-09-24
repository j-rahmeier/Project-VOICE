////////////////////////////////////////////////////////////////////////////////
// $Id$
// Author: mellinger@neurotechcenter.org
// Description: An AVSource implementation for Windows Media Foundation devices.
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
#ifndef AV_SOURCE_MF_H
#define AV_SOURCE_MF_H

#include "AVSource.h"
#include "AVSourceEnumerator.h"
#include "VideoMediaType.h"
#include "ComPtr.h"

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

class AVSourceMF : public AVSource
{
private:
    friend class AVSourceEnumeratorMF;

    AVSourceMF();

    bool Init(com::Ptr<IMFSourceReader>);

public:
    ~AVSourceMF();

    const struct Properties& Properties() const override;
    bool WaitForData(std::vector<uint8_t>&) override;
    void AbortWaiting() override;

private:
    struct Private;
    Private* p;
};

#endif // AV_SOURCE_MF_H

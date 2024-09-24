////////////////////////////////////////////////////////////////////////////////
// $Id: Errors.h 7731 2023-11-23 16:43:40Z mellinger $
// Author: mellinger@neurotechcenter.com
// Description: Error variables that throw on error, keeping code clean of error
//   handling overhead.
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
#ifndef ERRORS_H
#define ERRORS_H

#include "Exception.h"
#include "SysError.h"

struct HRSucceedOrThrow
{
  HRSucceedOrThrow(HRESULT hr = S_OK) : hr(hr)
  {
    if (FAILED(hr))
      throw std_runtime_error << SysError(hr).Message();
  }
  operator HRESULT() const
  {
    return hr;
  }
private:
  HRESULT hr;
};

#endif // ERRORS_H

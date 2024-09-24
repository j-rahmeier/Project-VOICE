////////////////////////////////////////////////////////////////////////////////
// $Id: FeedbackScene2D.h 7195 2023-02-07 18:31:03Z mellinger $
// Author: juergen.mellinger@uni-tuebingen.de
// Description: A 2D feedback scene implemented using 2D GraphObjects.
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
#ifndef FEEDBACK_SCENE_2D_H
#define FEEDBACK_SCENE_2D_H

#include "DisplayWindow.h"
#include "FeedbackScene.h"
#include "Shapes.h"

class FeedbackScene2D : public FeedbackScene
{
  public:
    FeedbackScene2D(GUI::DisplayWindow &);
    virtual ~FeedbackScene2D();

  protected:
    void OnInitialize() override;
    void OnSetCursorPosition(const Vector3D &) override;

    float CursorRadius() const override;
    void SetCursorVisible(bool) override;
    void SetCursorColor(RGBColor) override;

    int NumTargets() const override;
    bool TargetHit(int) const override;
    float CursorTargetDistance(int) const override;
    void SetTargetVisible(bool, int) override;
    void SetTargetColor(RGBColor, int) override;

  private:
    void ClearObjects();
    enum
    {
        point,
        vector
    };
    void SceneToObjectCoords(GUI::Point &, int kind) const;
    void ObjectToSceneCoords(GUI::Point &, int kind) const;

    GUI::DisplayWindow &mDisplay;
    float mScalingX, mScalingY;
    EllipticShape *mpCursor;
    RectangularShape *mpBoundary;
    std::vector<RectangularShape *> mTargets;
};

#endif // FEEDBACK_SCENE_2D_H

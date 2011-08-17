//
////  libavg - Media Playback Engine. 
////  Copyright (C) 2003-2011 Ulrich von Zadow
////
////  This library is free software; you can redistribute it and/or
////  modify it under the terms of the GNU Lesser General Public
////  License as published by the Free Software Foundation; either
////  version 2 of the License, or (at your option) any later version.
////
////  This library is distributed in the hope that it will be useful,
////  but WITHOUT ANY WARRANTY; without even the implied warranty of
////  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
////  Lesser General Public License for more details.
////
////  You should have received a copy of the GNU Lesser General Public
////  License along with this library; if not, write to the Free Software
////  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
////
////  Current versions can be found at www.libavg.de
////

#ifndef _HueSatFXNode_H_
#define _HueSatFXNode_H_

#include "../api.h"

#include "FXNode.h"
#include "../graphics/GPUHueSatFilter.h"

#include <boost/shared_ptr.hpp>
#include <boost/python.hpp>
#include <string>

using namespace std;

namespace avg {
class SDLDisplayEngine;

class AVG_API HueSatFXNode : public FXNode {
public:
    HueSatFXNode(int hue=0, int saturation=0, int lightness=0,
            bool tint=false);
    virtual ~HueSatFXNode();
    virtual void disconnect();

    void setHue(int hue);
    void setSaturation(int saturation);
    void setLightnessOffset(int lightnessOffset);
    void setColorizing(bool colorize);
    int getHue();
    int getSaturation();
    int getLightnessOffset();
    bool isColorizing();

    std::string toString();

private:
    virtual GPUFilterPtr createFilter(const IntPoint& size);
    void setFilterParams();
    int clamp(int val, int min, int max);

    GPUHueSatFilterPtr filterPtr;

    int m_fHue;
    int m_fLightnessOffset;
    int m_fSaturation;
    bool m_bColorize;
};

typedef boost::shared_ptr<HueSatFXNode> HueSatFXNodePtr;
} //end namespace avg

#endif

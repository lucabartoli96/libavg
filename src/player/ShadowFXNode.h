//
//  libavg - Media Playback Engine. 
//  Copyright (C) 2003-2014 Ulrich von Zadow
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  Current versions can be found at www.libavg.de
//

#ifndef _ShadowFXNode_H_
#define _ShadowFXNode_H_

#include "../api.h"

#include "FXNode.h"
#include "../graphics/GPUShadowFilter.h"
#include "../graphics/Color.h"
#include "../base/GLMHelper.h"

#include <boost/shared_ptr.hpp>
#include <string>

namespace avg {

class AVG_API ShadowFXNode: public FXNode {
public:
    ShadowFXNode(glm::vec2 offset=glm::vec2(0,0), float radius=1.f, float opacity=1.f,
            Color color=Color("FFFFFF"));
    virtual ~ShadowFXNode();

    virtual void connect();
    virtual void disconnect();

    void setOffset(const glm::vec2& offset);
    glm::vec2 getOffset() const;
    void setRadius(float radius);
    float getRadius() const;
    void setOpacity(float opacity);
    float getOpacity() const;
    void setColor(const Color& sColor);
    Color getColor() const;

private:
    virtual GPUFilterPtr createFilter(const IntPoint& size);
    void updateFilter();

    GPUShadowFilterPtr m_pFilter;

    glm::vec2 m_Offset;
    float m_StdDev;
    float m_Opacity;
    Color m_Color;
};

typedef boost::shared_ptr<ShadowFXNode> ShadowFXNodePtr;

}

#endif


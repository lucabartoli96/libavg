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

#ifndef _StateAnim_H_
#define _StateAnim_H_

#include "../api.h"

#include "Anim.h"

#include <vector>
#include <map>

namespace avg {

struct AVG_API AnimState {
    AnimState(const UTF8String& sName, AnimPtr pAnim, const UTF8String& sNextName = "");
    AnimState();

    UTF8String m_sName;
    AnimPtr m_pAnim;
    UTF8String m_sNextName;
};

class AVG_API StateAnim: public Anim {
public:
    StateAnim(const std::vector<AnimState>& states);
    virtual ~StateAnim();

    virtual void abort();

    virtual void setState(const UTF8String& sName, bool bKeepAttr=false);
    const UTF8String& getState() const;
    void setDebug(bool bDebug);
    
    virtual bool step();

private:
    void switchToNewState(const UTF8String& sName, bool bKeepAttr);

    std::map<UTF8String, AnimState> m_States;
    bool m_bDebug;
    UTF8String m_sCurStateName;
};

}

#endif 




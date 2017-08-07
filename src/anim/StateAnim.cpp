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

#include "StateAnim.h"

#include "../base/Exception.h"
#include "../player/Player.h"

using namespace boost::python;
using namespace std;

namespace avg {

AnimState::AnimState(const UTF8String& sName, AnimPtr pAnim, const UTF8String& sNextName)
    : m_sName(sName),
      m_pAnim(pAnim),
      m_sNextName(sNextName)
{
}

AnimState::AnimState()
{
}

StateAnim::StateAnim(const vector<AnimState>& states)
    : Anim(object(), object()),
      m_bDebug(false)
{
    vector<AnimState>::const_iterator it;
    for (it=states.begin(); it != states.end(); ++it) {
        m_States[(*it).m_sName] = *it;
        it->m_pAnim->setHasParent();
    }
}

StateAnim::~StateAnim()
{
    setState("");
}

void StateAnim::abort()
{
    setState("");
}

void StateAnim::setState(const UTF8String& sName, bool bKeepAttr)
{
    if (m_sCurStateName == sName) {
        return;
    }
    if (!(Player::get()->isPlaying())) {
        throw(Exception(AVG_ERR_UNSUPPORTED,
                "Animation playback can only be started when the player is running."));
    }

    if (!m_sCurStateName.empty()) {
        m_States[m_sCurStateName].m_pAnim->abort();
    }
    switchToNewState(sName, bKeepAttr);
}

const UTF8String& StateAnim::getState() const
{
    return m_sCurStateName;
}

void StateAnim::setDebug(bool bDebug)
{
    m_bDebug = bDebug;
}
    
bool StateAnim::step()
{
    // Make sure the object isn't deleted until the end of the method.
    AnimPtr tempThis = shared_from_this();  

    if (!m_sCurStateName.empty()) {
        const AnimState& curState = m_States[m_sCurStateName];
        AnimPtr pAnim = curState.m_pAnim;
        bool bDone;
        if (pAnim->isRunning()) {
            bDone = curState.m_pAnim->step();
        } else {
            // Special case: AttrAnim stopped because other animation hijacked it.
            bDone = true;
        }
        if (bDone) {
            switchToNewState(curState.m_sNextName, false);
        }
    }
    return false;
}

void StateAnim::switchToNewState(const UTF8String& sName, bool bKeepAttr)
{
    if (m_bDebug) {
        cerr << this << " State change: '" << m_sCurStateName << "' --> '" << sName 
                << "'" << endl;
    }
    UTF8String sOldStateName = m_sCurStateName;
    m_sCurStateName = sName;
    if (!sName.empty()) {
        map<UTF8String, AnimState>::iterator it = m_States.find(sName);
        if (it == m_States.end()) {
            throw Exception(AVG_ERR_INVALID_ARGS, "StateAnim: State "+sName+" unknown.");
        } else {
            it->second.m_pAnim->start(bKeepAttr);
        }
        if (sOldStateName == "") {
            Anim::start(false);
        }
    } else {
        Anim::setStopped();
    }
}

}

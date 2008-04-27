//
//  libavg - Media Playback Engine. 
//  Copyright (C) 2003-2006 Ulrich von Zadow
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

#ifndef _Sound_H_
#define _Sound_H_

// Python docs say python.h should be included before any standard headers (!)
#include "WrapPython.h" 

#include "Node.h"

#include "../base/IFrameListener.h"
#include "../audio/IAudioSource.h"

namespace avg {

class IVideoDecoder;

class Sound : public Node, IFrameListener, IAudioSource
{
    public:
        static NodeDefinition getNodeDefinition();

        Sound (const ArgList& Args, Player * pPlayer);
        virtual ~Sound ();

        virtual void setRenderingEngines(DisplayEngine * pDisplayEngine, 
                AudioEngine * pAudioEngine);
        virtual void disconnect();

        void play();
        void stop();
        void pause();

        const std::string& getHRef() const;
        void setHRef(const std::string& href);
        void setVolume(double Volume);
        void checkReload();

        long long getDuration() const;
        long long getCurTime() const;
        void seekToTime(long long Time);
        bool getLoop() const;
        void setEOFCallback(PyObject * pEOFCallback);

        virtual std::string getTypeStr();

        virtual void onFrameEnd();
        virtual void setAudioEnabled(bool bEnabled);

        virtual int fillAudioBuffer(AudioBufferPtr pBuffer);

    private:
        void seek(long long DestTime);
        void onEOF();

        typedef enum SoundState {Unloaded, Paused, Playing};
        void changeSoundState(SoundState NewSoundState);
        void open();
        void close();

        std::string m_href;
        std::string m_Filename;
        bool m_bLoop;
        PyObject * m_pEOFCallback;
        bool m_bAudioEnabled;

        long long m_StartTime;
        long long m_PauseTime;
        long long m_PauseStartTime;

        IVideoDecoder * m_pDecoder;
        double m_Volume;
        SoundState m_State;
};

}

#endif 


// TestAudiere.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <conio.h>
using namespace std;

#include "audiere.h"
using namespace audiere;

//#define TEST_MIDI

int _tmain(int argc, _TCHAR* argv[])
{

    RefPtr<AudioDevice> device=OpenDevice();
    if (!device) 
    {
        cout<<"初始化audiere设备失败！"<<endl;
        return 0;
    }

    /*
    * If OpenSound is called with the last parameter = false, then
    * Audiere tries to load the sound into memory.  If it can't do
    * that, it will just stream it.
    */
    RefPtr<OutputStream> sound=OpenSound(device, "walk_grass.wav", false);
    if (!sound)
    {
        // failure
        cout<<"加载声音失败!"<<endl;
        return 0;
    }

    /*
    * Since this file is background music, we don't need to load the
    * whole thing into memory.
    */
    RefPtr<OutputStream> stream=OpenSound(device, "Battle001.ogg", true);
    if (!stream) 
    {
        // failure
        cout<<"加载北京音乐失败!"<<endl;
        return 0;
    }

    // let's start the background music first
    bool bMidi=false;
    stream->setRepeat(true);
    stream->setVolume(0.5f);
    stream->play();

    /*
    MIDIDevice使用windows的sendCommand实现，故加载速度较慢。
    */
    cout<<"正在加载MIDI..."<<endl;
    RefPtr<MIDIDevice> midiDevice=OpenMIDIDevice("");
    if (!midiDevice) 
    {
        cout<<"初始化MIDI设备失败！"<<endl;
        return 0;
    }
    RefPtr<MIDIStream> midiStream=midiDevice->openStream("007.mid");
    if (!midiStream) 
    {
        // failure
        cout<<"加载midi失败!"<<endl;
        return 0;
    }

    cout<<"q-退出，e-切换背景音乐，其他-播放音效"<<endl;
    while(true)
    {
        char t;
        t=_getch();
        if (t == 'q')
        {
            break;
        }
        else if(t == 'e')//切换音乐
        {
            bMidi = !bMidi;
            if (bMidi)
            {
                cout<<"midi播放中..."<<endl;
                stream->stop();
                midiStream->play();
            }
            else
            {
                cout<<"背景音乐播放中..."<<endl;
                midiStream->stop();
                stream->play();
            }
        }
        else
        {
            // now play a sound effect
            sound->play();
        }
    }
    //stream->stop();
    return 0;
}


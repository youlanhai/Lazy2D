// TestAudiere.cpp : �������̨Ӧ�ó������ڵ㡣
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
        cout<<"��ʼ��audiere�豸ʧ�ܣ�"<<endl;
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
        cout<<"��������ʧ��!"<<endl;
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
        cout<<"���ر�������ʧ��!"<<endl;
        return 0;
    }

    // let's start the background music first
    bool bMidi=false;
    stream->setRepeat(true);
    stream->setVolume(0.5f);
    stream->play();

    /*
    MIDIDeviceʹ��windows��sendCommandʵ�֣��ʼ����ٶȽ�����
    */
    cout<<"���ڼ���MIDI..."<<endl;
    RefPtr<MIDIDevice> midiDevice=OpenMIDIDevice("");
    if (!midiDevice) 
    {
        cout<<"��ʼ��MIDI�豸ʧ�ܣ�"<<endl;
        return 0;
    }
    RefPtr<MIDIStream> midiStream=midiDevice->openStream("007.mid");
    if (!midiStream) 
    {
        // failure
        cout<<"����midiʧ��!"<<endl;
        return 0;
    }

    cout<<"q-�˳���e-�л��������֣�����-������Ч"<<endl;
    while(true)
    {
        char t;
        t=_getch();
        if (t == 'q')
        {
            break;
        }
        else if(t == 'e')//�л�����
        {
            bMidi = !bMidi;
            if (bMidi)
            {
                cout<<"midi������..."<<endl;
                stream->stop();
                midiStream->play();
            }
            else
            {
                cout<<"�������ֲ�����..."<<endl;
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


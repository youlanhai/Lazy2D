#pragma once

#ifdef SOUND_ENGINE_DLL
#   ifdef MYSOUND_EXPORTS
#       define MYSOUND_API __declspec(dllexport)
#   else
#       define MYSOUND_API __declspec(dllimport)
#   endif
#else
#   define MYSOUND_API
#endif //SOUND_ENGINE_DLL

#include <Windows.h>

///////////////////////////////////
MYSOUND_API int InitMusic(HWND hWnd,int max_file=10);
MYSOUND_API int LoadMid(int index,char *filename);
MYSOUND_API int PlayMid(int index,bool loop=false);
MYSOUND_API int IsPlaying(int index);
MYSOUND_API int StopMid(int index);
MYSOUND_API int StopAllMid();
MYSOUND_API int DeleteMid(int index);
MYSOUND_API int DeleteAllMid();
MYSOUND_API int DestroyMusic();
///////////////////////////
MYSOUND_API int InitSound(HWND hWnd,int max_file=50);
MYSOUND_API int LoadWave(int index,char *filename);
MYSOUND_API int CopySoundBuffer(int index);
MYSOUND_API int PlaySound(int index,bool loop=false,bool bfromhead=false);
MYSOUND_API int IsSounPlaying(int index);
MYSOUND_API int SetFrequncy(int index,long fre);
MYSOUND_API int SetPan(int index,long lpan);
MYSOUND_API int SetVolum(int index,long volume);
MYSOUND_API int StopSound(int index);
MYSOUND_API int StopAllSound();
MYSOUND_API int DeleteSound(int index);
MYSOUND_API int DeleteAllSound();
MYSOUND_API int DestroySound();
/////////////////////////////

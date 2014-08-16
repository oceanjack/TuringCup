#include "sound.h"
#include "../var/musicVar.h"
Sound::Sound(char *p)
{
	loop = true;
	path = p;
}
Sound::~Sound()
{
	//Sound_Release();
}
//增加音乐音量
void Sound::Sound_addVal()
{
	if(musicVar <= 90)
		musicVar += 10;
	hge->Channel_SetVolume(snd, musicVar);
	return;
}
//降低音乐音量
void Sound::Sound_decVal()
{
	if(musicVar >= 10)
		musicVar -= 10;
	hge->Channel_SetVolume(snd, musicVar);
	return;
}
bool Sound::Sound_Initiate()
{
	//音效
	snd = hge->Stream_Load(path);
	if(!snd)
	{
		MessageBox(NULL, hge->System_GetErrorMessage(), "snd_Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return false;
	}
	//字体
	Fnt = new hgeFont("../font/font2.fnt");
	if (!Fnt)
		return false;
	return true;
}
bool Sound::Sound_Frame()
{
	hge->Stream_Play(snd, loop, musicVar);
	return false;
}
bool Sound::Sound_Render()
{
	Fnt->printf(10, 30, HGETEXT_LEFT, "Val:%d\n", musicVar);
	return false;
}
void Sound::Sound_Release()
{
	if(Fnt)
		delete Fnt;
	if(snd)
		hge->Stream_Free(snd);
	return;
}
void Sound::Sound_setPath(char *p)
{
	path = p;
}
void Sound::Sound_stop()
{
	hge->Channel_Pause(snd);
}
void Sound::Sound_play()
{
	hge->Channel_Resume(snd);
}
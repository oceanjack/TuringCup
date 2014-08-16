#include "effect.h"
#include "../var/musicVar.h"
Effect::Effect(char *p)
{
	pan = 0;
	pitch = 1.0;
	loop = false;
	path = p;
}
Effect::~Effect()
{
	Effect_Release();
}
void Effect::add()
{
	if(effectVar <= 90)
		effectVar += 10;
}
void Effect::dec()
{
	if(effectVar >= 10)
		effectVar -= 10;
}
bool Effect::Effect_Initiate()
{
	snd = hge->Effect_Load(path);
	if(!snd)
	{
		MessageBox(NULL, hge->System_GetErrorMessage(), "effect_Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return false;
	}
	return true;
}
bool Effect::Effect_Frame()
{
	hge->Effect_PlayEx(snd, effectVar, pan, pitch, loop);
	return false;
}
bool Effect::Effect_Render()
{
	return false;
}
void Effect::Effect_Release()
{
	if(snd)
	{
		hge->Effect_Free(snd);
		snd = 0;
	}
	return;
}
void Effect::Effect_ReSet(char *p)
{
	path = p;
}
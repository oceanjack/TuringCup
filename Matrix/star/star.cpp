#include "./star.h"
#include <time.h>
#include <stdlib.h>
Star::Star(float _x, float _y)
{
	act = true;
	x = _x;
	y = _y;
}
Star::~Star()
{
	//Star_Release();
}
void Star::setPos(float _x, float _y)
{
	x = _x;
	y = _y;
}
void Star::setAct(bool f)
{
	act = f;
}
bool Star::Star_Initiate()
{
	tex = hge->Texture_Load("../psi/particles.png");
	if(!tex)
	{
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return false;
	}

	//纹理
	spr = new hgeSprite(tex, 0, 96, 32, 32);
	spr->SetHotSpot(0,0);

	//粒子系统
	par = new hgeParticleSystem("../psi/star.psi", spr);

	return true;
}
bool Star::Star_Frame()
{
	par->Fire();
	par->info.nEmission = 4;
	par->MoveTo(x + rand() % 20, y + rand() % 20);
	par->Update(hge->Timer_GetDelta());
	return false;
}
bool Star::Star_Render()
{
	if(act)
		par->Render();
	return false;
}
void Star::Star_Release()
{
	if(par)
		delete par;
	if(spr)
		delete spr;
	if(tex)
		hge->Texture_Free(tex);
	return;
}
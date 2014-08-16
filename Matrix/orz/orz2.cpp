#include "./orz2.h"
#include "../Info.h"
ORZ2::ORZ2(float _x, float _y, float _dx, float _dy)
{
	x = _x;
	y = _y;
	dx = _dx;
	dy = _dy;
}
ORZ2::~ORZ2()
{
	//ORZ_Release();
}
bool ORZ2::ORZ2_Initiate()
{
	c = hge->Random_Int(-200, 200);
	t = abs(dy - y) / 7 + abs(dx - x) / 7 + 10;
	l = t;
	tex = hge->Texture_Load("../psi/particles.png");
	if(!tex)
	{
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return false;
	}

	//纹理句柄，x，y，w，h
	spr = new hgeSprite(tex, 0, 96, 32, 32);
	spr->SetHotSpot(0, 0); //设置锚点

	//粒子系统描述文件名，粒子系统指向有效hgeSprite指针
	par = new hgeParticleSystem("../psi/star.psi", spr);
	
	return true;
}
bool ORZ2::ORZ2_Frame()
{
	if(t > 0)
		--t;
	else
		return true;
	par->Fire();
	par->info.nEmission = 20;
	par->info.fSizeStart = 1;
	par->info.fSizeEnd = 0;
	par->FireAt(x + (dx - x)/l*(l-t), y + (dy - y)/l*(l-t));
	par->MoveTo(x + (dx - x)/l*(l-t), y + (dy - y)/l*(l-t),true);
	par->Update(hge->Timer_GetDelta());
	return false;
}
bool ORZ2::ORZ2_Render()
{
	par->Render();
	return false;
}
void ORZ2::ORZ2_Release()
{
	if(par)
	{
		delete par;
		par = 0;
	}
	if(spr)
	{
		delete spr;
		spr = 0;
	}
	if(tex)
	{
		hge->Texture_Free(tex);
		tex = 0;
	}
	return;
}
void ORZ2::setXY(int _dx, int _dy)
{
	dx = _dx;
	dy = _dy;
}
#include "cursor.h"
#include <math.h>
Cursor::Cursor(char *p, char *p2)
{
	path = p;
	path2 = p2;
}
Cursor::~Cursor()
{
	//Cursor_Release();
}
bool Cursor::Cursor_Initiate()
{
	tex = hge->Texture_Load(path);
	if(!tex)
	{
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return false;
	}

	//纹理句柄，x，y，w，h
	spr = new hgeSprite(tex, 0, 0, 32, 32);
	spr->SetHotSpot(0, 0); //设置锚点

	//spr替代
	bgQuad = new hgeQuad();
	bgQuad->tex = tex;
	//屏幕想x，y设置
	bgQuad->v[0].x = 0, bgQuad->v[0].y = 0;
	bgQuad->v[1].x = 0, bgQuad->v[1].y = 0;
	bgQuad->v[2].x = 0, bgQuad->v[2].y = 0;
	bgQuad->v[3].x = 0, bgQuad->v[3].y = 0;
	for(int i = 0; i < 4; ++i)
	{
		bgQuad->v[i].z = 0;
		bgQuad->v[i].col = 0xffffffff;
	}
	//纹理x，y坐标
	bgQuad->v[0].tx = 0, bgQuad->v[0].ty = 0;
	bgQuad->v[1].tx = 1, bgQuad->v[1].ty = 0;
	bgQuad->v[2].tx = 1, bgQuad->v[2].ty = 1;
	bgQuad->v[3].tx = 0, bgQuad->v[3].ty = 1;
	bgQuad->blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;

	//粒子系统描述文件名，粒子系统指向有效hgeSprite指针
	par = new hgeParticleSystem(path2, spr);
	
	return true;
}
bool Cursor::Cursor_Frame()
{
	hge->Input_GetMousePos(&x, &y);
	//移动监测
	isRender = fabs(x - cx) > 1e-1 || fabs(y - cy) > 1e-1;
	cx = x;
	cy = y;
	//轨迹追踪效果
	if(isRender)
	{	
		par->Fire();
		par->info.nEmission = 20;
		par->MoveTo(x, y);
	}
	else
		par->Stop();
	par->Update(hge->Timer_GetDelta());
	return false;
}
bool Cursor::Cursor_Render()
{
	par->Render();
	bgQuad->v[0].x = x, bgQuad->v[0].y = y;
	bgQuad->v[1].x = x + 32.0f, bgQuad->v[1].y = y;
	bgQuad->v[2].x = x + 32.0f, bgQuad->v[2].y = y + 32.0f;
	bgQuad->v[3].x = x, bgQuad->v[3].y = y + 32.0f;
	hge->Gfx_RenderQuad(bgQuad);
	return false;
}
void Cursor::Cursor_Release()
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
	if(bgQuad)
	{
		delete bgQuad;
		bgQuad = 0;
	}
	if(tex)
	{
		hge->Texture_Free(tex);
		tex = 0;
	}
	return;
}

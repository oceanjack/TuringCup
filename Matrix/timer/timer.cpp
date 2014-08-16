#include "./timer.h"
#include "../Info.h"
Timer::Timer()
{
}
Timer::~Timer()
{
	//Timer_Release();
}
bool Timer::Timer_Initiate()
{
	id = 3;
	t = 0;

	texs[0] = hge->Texture_Load("../timer/0.png");
	texs[1] = hge->Texture_Load("../timer/1.png");
	texs[2] = hge->Texture_Load("../timer/2.png");
	texs[3] = hge->Texture_Load("../timer/3.png");
	for(int i = 0; i < 4; ++i)
	{
		if(!texs[i])
		{
			MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
			return false;
		}
		Quad[i] = new hgeQuad();
		Quad[i]->tex = texs[i];
	}

	return true;
}
bool Timer::Timer_Frame()
{
	if(id < 0)
		return true;
	float size = 1100 - t*10.0f;
	float _x = 345;
	float _y = 230;
	Quad[id]->v[0].x = _x - size, Quad[id]->v[0].y = _y - size;
	Quad[id]->v[1].x = _x + size, Quad[id]->v[1].y = _y - size;
	Quad[id]->v[2].x = _x + size, Quad[id]->v[2].y = _y + size;
	Quad[id]->v[3].x = _x - size, Quad[id]->v[3].y = _y + size;
	Quad[id]->v[0].tx = 0, Quad[id]->v[0].ty = 0;
	Quad[id]->v[1].tx = 1, Quad[id]->v[1].ty = 0;
	Quad[id]->v[2].tx = 1, Quad[id]->v[2].ty = 1;
	Quad[id]->v[3].tx = 0, Quad[id]->v[3].ty = 1;
	for(int j = 0; j < 4; ++j)
	{
		Quad[id]->v[j].col = 0xffffffff;
		Quad[id]->v[j].z = 0;
	}
	Quad[id]->blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;
	++t;
	if(t >= 100)
	{
		t = 0;
		--id;
	}
	return false;
}
bool Timer::Timer_Render()
{
	if(id >= 0)
		hge->Gfx_RenderQuad(Quad[id]);
	return false;
}
void Timer::Timer_Release()
{
	for(int i = 0; i < 4; ++i)
	{
		if(Quad[i])
		{
			delete Quad[i];
			Quad[i] = 0;
		}
		if(texs[i])
		{
			hge->Texture_Free(texs[i]);
			texs[i] = 0;
		}
	}
	return;
}
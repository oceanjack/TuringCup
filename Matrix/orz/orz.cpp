#include "./orz.h"
#include "../Info.h"
#include <string.h>
ORZ::ORZ(int _lt, int _x, int _y, int _h, int _w, int _l, char *_ip, char *_mp)
{
	l_t = _lt;
	x = _x;
	y = _y;
	h = _h;
	w = _w;
	len = _l;
	img_path = _ip;
	music_path = _mp;
}
ORZ::~ORZ()
{
	//ORZ_Release();
}
bool ORZ::ORZ_Initiate()
{
	id = 0;
	t = 0;
	for(int i = 0; i < len; ++i)
	{
		char num[2];
		char tmp[50];
		strcpy(tmp, img_path);
		strcat(tmp, " (");
		num[0] = i + 1 + '0';
		num[1] = '\0';
		strcat(tmp, num);
		strcat(tmp, ").png");
		texs[i] = hge->Texture_Load(tmp);

		if(!texs[i])
		{
			MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
			return false;
		}
		Quad[i] = new hgeQuad();
		Quad[i]->tex = texs[i];
		for(int j = 0; j < 4; ++j)
		{
			Quad[i]->v[j].col = 0xffffffff;
			Quad[i]->v[j].z = 0;
		}
		Quad[i]->v[0].tx = 0, Quad[i]->v[0].ty = 0;
		Quad[i]->v[1].tx = 1, Quad[i]->v[1].ty = 0;
		Quad[i]->v[2].tx = 1, Quad[i]->v[2].ty = 1;
		Quad[i]->v[3].tx = 0, Quad[i]->v[3].ty = 1;
		Quad[i]->blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;
	}

	return true;
}
bool ORZ::ORZ_Frame()
{
	if(id >= len)
		return true;
	++t;
	if(t >= l_t)
	{
		t = 0;
		++id;
		if(id >= len)
			return true;
	}
	Quad[id]->v[0].x = x - w, Quad[id]->v[0].y = y - h;
	Quad[id]->v[1].x = x + w, Quad[id]->v[1].y = y - h;
	Quad[id]->v[2].x = x + w, Quad[id]->v[2].y = y + h;
	Quad[id]->v[3].x = x - w, Quad[id]->v[3].y = y + h;
	return false;
}
bool ORZ::ORZ_Render()
{
	if(id < len)
		hge->Gfx_RenderQuad(Quad[id]);
	return false;
}
void ORZ::ORZ_Release()
{
	for(int i = 0; i < len; ++i)
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
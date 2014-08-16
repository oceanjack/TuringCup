#include "./Stars.h"
#include <fstream>
Stars::Stars()
{
	up = 170;
	down = 70;
}
Stars::~Stars()
{
	//Stars_Release();
}
bool Stars::Stars_Initiate()
{
	tt = down;
	dt = 0.0002f;
	dtt = 0.1f;
	bgtex = hge->Texture_Load("../background/star.png");
	if(!bgtex)
	{
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return false;
	}

	//矩形框设计
	bgd.tex = bgtex;
	bgr.tex = bgtex;

	bgd.v[0].x = 0,   bgd.v[0].y = 0;
	bgd.v[1].x = 800, bgd.v[1].y = 0;
	bgd.v[2].x = 800, bgd.v[2].y = 600;
	bgd.v[3].x = 0,   bgd.v[3].y = 600;

	bgr.v[0].x = 0,   bgr.v[0].y = 0;
	bgr.v[1].x = 800, bgr.v[1].y = 0;
	bgr.v[2].x = 800, bgr.v[2].y = 600;
	bgr.v[3].x = 0,   bgr.v[3].y = 600;

	for(int i = 0; i < 4; ++i)
	{
		bgd.v[i].z = 0;
		bgd.v[i].col = 0xffffffff;
	}
	bgd.blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;

	for(int i = 0; i < 4; ++i)
	{
		bgr.v[i].z = 0;
		bgr.v[i].col = 0xffffffff;
	}
	bgr.blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;
	
	return true;

}
bool Stars::Stars_Frame()
{
	t += dt;
	if(t > 100 || t < -100)
		dt = -dt;

	/*调参数时用
	if(hge->Input_GetKey() == HGEK_F12)
	{
		std::ifstream files;
		files.open("E:/Matrix/in.txt");
		files>>up>>down>>dt>>dtt;
		files.close();

		if(tt > up || tt < down)
			tt = down;
	}
	//*/

	tt += dtt;
	if((int)tt >= up || tt <= down)
		dtt = -dtt;

	//透明控制
	for(int i = 0; i < 4; ++i)
	{
		bgd.v[i].col = ARGB((int)tt, 0xff, 0xff, 0xff);
		bgr.v[i].col = ARGB(up + down - (int)tt, 0xff, 0xff, 0xff);
	}

	//移动
	ty = t;
	tx = t * 0.9 + 1.0;
	
	bgd.v[0].tx = tx * 0.3;		  bgd.v[0].ty = ty;
	bgd.v[1].tx = tx * 0.3 + 0.8; bgd.v[1].ty = ty;
	bgd.v[2].tx = tx * 0.3 + 0.8; bgd.v[2].ty = ty + 0.8;
	bgd.v[3].tx = tx * 0.3;		  bgd.v[3].ty = ty + 0.8;

	bgr.v[0].tx = tx;		bgr.v[0].ty = ty * -0.2;
	bgr.v[1].tx = tx + 0.8; bgr.v[1].ty = ty * -0.2;
	bgr.v[2].tx = tx + 0.8; bgr.v[2].ty = ty * -0.2 + 0.8;
	bgr.v[3].tx = tx;		bgr.v[3].ty = ty * -0.2 + 0.8;
	return false;
}
bool Stars::Stars_Render()
{
	hge->Gfx_RenderQuad(&bgd);
	hge->Gfx_RenderQuad(&bgr);
	return false;
}
void Stars::Stars_Release()
{
	if(bgtex)
		hge->Texture_Free(bgtex);
}
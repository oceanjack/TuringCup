#include "./autoblack.h"
AutoBlack::AutoBlack(int t, int d)
{
	time = t;
	dir = d;
	switch(dir)
	{
		case M_1:
			cval = 0xff;
			break;
		case M_2:
			cval = 0;
			break;
	}
	dval = (0xff) / (t*1.0);
	//暂停
	stop = false;
}
AutoBlack::~AutoBlack()
{
	//AutoBlack_Release();
}
void AutoBlack::reSet(int t, int d)
{
	time = t;
	dir = d;
	switch(dir)
	{
		case M_1:
			cval = 0xff;
			break;
		case M_2:
			cval = 0;
			break;
	}
	dval = (0xff) / (t*1.0);
	for(int i = 0; i < 4; ++i)
	{
		int rval = int(cval);
		bgQuad.v[i].col = ARGB(rval,0x00,0x00,0x00);
	}
	//暂停
	stop = false;
}
bool AutoBlack::AutoBlack_Initiate()
{
	//屏幕想x，y设置
	bgQuad.v[0].x = 0, bgQuad.v[0].y = 0;
	bgQuad.v[1].x = 800, bgQuad.v[1].y = 0;
	bgQuad.v[2].x = 800, bgQuad.v[2].y = 600;
	bgQuad.v[3].x = 0, bgQuad.v[3].y = 600;
	for(int i = 0; i < 4; ++i)
	{
		bgQuad.v[i].z = 0;
		bgQuad.v[i].col = 0xff000000;
	}
	//纹理x，y坐标
	bgQuad.v[0].tx = 0, bgQuad.v[0].ty = 0;
	bgQuad.v[1].tx = 1, bgQuad.v[1].ty = 0;
	bgQuad.v[2].tx = 1, bgQuad.v[2].ty = 1;
	bgQuad.v[3].tx = 0, bgQuad.v[3].ty = 1;
	bgQuad.blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;
	//暂停
	stop = false;
	return true;
}
bool AutoBlack::AutoBlack_Frame()
{
	if(stop)
		time = -1;
	if(time >= 0)
		--time;
	if(time <= 0)
		return true;
	switch(dir)
	{
		case M_1:
			cval -= dval;
			if(cval <= 0)
				cval = 0;
			break;
		case M_2:
			cval += dval;
			if(cval >= 0xff)
				cval = 0xff;
			break;
	}
	for(int i = 0; i < 4; ++i)
	{
		int rval = int(cval);
		bgQuad.v[i].col = ARGB(rval,0x00,0x00,0x00);
	}
	return false;
}
bool AutoBlack::AutoBlack_Render()
{
	if(time >= 0 || stop)
		hge->Gfx_RenderQuad(&bgQuad);
	return false;
}
void AutoBlack::AutoBlack_Release()
{
	time = -1;
	return;
}
void AutoBlack::setStop(int _val)
{
	stop = true;
	time = -1;
	for(int i = 0; i < 4; ++i)
	{
		bgQuad.v[i].col = ARGB(_val,0,0,0);
	}
}
bool AutoBlack::getStop()
{
	return stop;
}
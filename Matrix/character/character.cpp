#include "character.h"
#include <string.h>
#include <math.h>
Character::Character(char *p, char *p2, int _x, int _y)
{
	path = p;
	path2 = p2;
	id = 0;
	num = 0;
	cx = _x;
	cy = _y;
	sx = 0;
	sy = 0;
	speed = 1;
	size = 1.0f;
	move = 0;
	c_size = 70;
}
Character::~Character()
{
	//Character_Release();
}
void Character::setPos(int _x, int _y)
{
	id = 0;
	num = 0;
	cx = _x;
	cy = _y;
	sx = 0;
	sy = 0;
	speed = 1;
	size = 1.0f;
	move = 0;
}
bool Character::Character_Initiate()
{
	no_show = false;
	sorce = 0;
	life = 404;
	num_g = 0;
	state = -1;
	state_t = 0;
	dead_cd = 0;
	len = 8;
	small_skill_cd = 0;
	round_time = 5000;
	death_time = 0;
	say = 0;
	say_time = 0;
	char tmps[100] = "../character/";
	strcat(tmps, path);
	strcat(tmps, "/");
	strcat(tmps, path);
	//下
	for(int i = 0; i < 8; ++i)
	{
		char tmp[100];
		strcpy(tmp, tmps);
		strcat(tmp, "3/");
		strcat(tmp, path);
		char num[10] = "3 (0).png";
		num[3] = '1' + i;
		strcat(tmp, num);
		texs[i] = hge->Texture_Load(tmp);

		bgQuad[i] = new hgeQuad();
		bgQuad[i]->tex = texs[i];
		//屏幕想x，y设置
		bgQuad[i]->v[0].x = x - c_size/2, bgQuad[i]->v[0].y = y - c_size/2;
		bgQuad[i]->v[1].x = x + c_size/2, bgQuad[i]->v[1].y = y - c_size/2;
		bgQuad[i]->v[2].x = x + c_size/2, bgQuad[i]->v[2].y = y + c_size/2;
		bgQuad[i]->v[3].x = x - c_size/2, bgQuad[i]->v[3].y = y + c_size/2;
		for(int j = 0; j < 4; ++j)
		{
			bgQuad[i]->v[j].z = 0;
			bgQuad[i]->v[j].col = 0xffffffff;
		}
		//纹理x，y坐标
		bgQuad[i]->v[0].tx = 0, bgQuad[i]->v[0].ty = 0;
		bgQuad[i]->v[1].tx = 1, bgQuad[i]->v[1].ty = 0;
		bgQuad[i]->v[2].tx = 1, bgQuad[i]->v[2].ty = 1;
		bgQuad[i]->v[3].tx = 0, bgQuad[i]->v[3].ty = 1;
		bgQuad[i]->blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;
	}
	//左
	for(int i = 8; i < 16; ++i)
	{
		char tmp[100];
		strcpy(tmp, tmps);
		strcat(tmp, "2/");
		strcat(tmp, path);
		char num[10] = "2 (0).png";
		num[3] = '1' + i - 8;
		strcat(tmp, num);
		texs[i] = hge->Texture_Load(tmp);
		bgQuad[i] = new hgeQuad();
		bgQuad[i]->tex = texs[i];
		//屏幕想x，y设置
		bgQuad[i]->v[0].x = x - c_size/2, bgQuad[i]->v[0].y = y - c_size/2;
		bgQuad[i]->v[1].x = x + c_size/2, bgQuad[i]->v[1].y = y - c_size/2;
		bgQuad[i]->v[2].x = x + c_size/2, bgQuad[i]->v[2].y = y + c_size/2;
		bgQuad[i]->v[3].x = x - c_size/2, bgQuad[i]->v[3].y = y + c_size/2;
		for(int j = 0; j < 4; ++j)
		{
			bgQuad[i]->v[j].z = 0;
			bgQuad[i]->v[j].col = 0xffffffff;
		}
		//纹理x，y坐标
		bgQuad[i]->v[0].tx = 0, bgQuad[i]->v[0].ty = 0;
		bgQuad[i]->v[1].tx = 1, bgQuad[i]->v[1].ty = 0;
		bgQuad[i]->v[2].tx = 1, bgQuad[i]->v[2].ty = 1;
		bgQuad[i]->v[3].tx = 0, bgQuad[i]->v[3].ty = 1;
		bgQuad[i]->blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;
	}
	//上
	for(int i = 16; i < 24; ++i)
	{
		char tmp[100];
		strcpy(tmp, tmps);
		strcat(tmp, "1/");
		strcat(tmp, path);
		char num[10] = "1 (0).png";
		num[3] = '1' + i - 16;
		strcat(tmp, num);
		texs[i] = hge->Texture_Load(tmp);
		bgQuad[i] = new hgeQuad();
		bgQuad[i]->tex = texs[i];
		//屏幕想x，y设置
		bgQuad[i]->v[0].x = x - c_size/2, bgQuad[i]->v[0].y = y - c_size/2;
		bgQuad[i]->v[1].x = x + c_size/2, bgQuad[i]->v[1].y = y - c_size/2;
		bgQuad[i]->v[2].x = x + c_size/2, bgQuad[i]->v[2].y = y + c_size/2;
		bgQuad[i]->v[3].x = x - c_size/2, bgQuad[i]->v[3].y = y + c_size/2;
		for(int j = 0; j < 4; ++j)
		{
			bgQuad[i]->v[j].z = 0;
			bgQuad[i]->v[j].col = 0xffffffff;
		}
		//纹理x，y坐标
		bgQuad[i]->v[0].tx = 0, bgQuad[i]->v[0].ty = 0;
		bgQuad[i]->v[1].tx = 1, bgQuad[i]->v[1].ty = 0;
		bgQuad[i]->v[2].tx = 1, bgQuad[i]->v[2].ty = 1;
		bgQuad[i]->v[3].tx = 0, bgQuad[i]->v[3].ty = 1;
		bgQuad[i]->blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;
	}
	//右
	for(int i = 24; i < 32; ++i)
	{
		char tmp[100];
		strcpy(tmp, tmps);
		strcat(tmp, "4/");
		strcat(tmp, path);
		char num[10] = "4 (0).png";
		num[3] = '1' + i - 24;
		strcat(tmp, num);
		texs[i] = hge->Texture_Load(tmp);
		bgQuad[i] = new hgeQuad();
		bgQuad[i]->tex = texs[i];
		//屏幕想x，y设置
		bgQuad[i]->v[0].x = x - c_size/2, bgQuad[i]->v[0].y = y - c_size/2;
		bgQuad[i]->v[1].x = x + c_size/2, bgQuad[i]->v[1].y = y - c_size/2;
		bgQuad[i]->v[2].x = x + c_size/2, bgQuad[i]->v[2].y = y + c_size/2;
		bgQuad[i]->v[3].x = x - c_size/2, bgQuad[i]->v[3].y = y + c_size/2;
		for(int j = 0; j < 4; ++j)
		{
			bgQuad[i]->v[j].z = 0;
			bgQuad[i]->v[j].col = 0xffffffff;
		}
		//纹理x，y坐标
		bgQuad[i]->v[0].tx = 0, bgQuad[i]->v[0].ty = 0;
		bgQuad[i]->v[1].tx = 1, bgQuad[i]->v[1].ty = 0;
		bgQuad[i]->v[2].tx = 1, bgQuad[i]->v[2].ty = 1;
		bgQuad[i]->v[3].tx = 0, bgQuad[i]->v[3].ty = 1;
		bgQuad[i]->blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;
	}
	char state_path[5][50] = {
		"../character/x.png",
		"../character/h.png",
		"../character/r.png",
		"../character/y.png",
		"../character/g.png"
	};
	for(int i = 0; i < 5; ++i)
	{
		tex[i] = hge->Texture_Load(state_path[i]);
		Quad[i] = new hgeQuad();
		Quad[i]->tex = tex[i];
		bgQuad[i]->tex = texs[i];
		//屏幕想x，y设置
		Quad[i]->v[0].x = x - c_size/2, Quad[i]->v[0].y = y - c_size/2;
		Quad[i]->v[1].x = x + c_size/2, Quad[i]->v[1].y = y - c_size/2;
		Quad[i]->v[2].x = x + c_size/2, Quad[i]->v[2].y = y + c_size/2;
		Quad[i]->v[3].x = x - c_size/2, Quad[i]->v[3].y = y + c_size/2;
		for(int j = 0; j < 4; ++j)
		{
			Quad[i]->v[j].z = 0;
			Quad[i]->v[j].col = 0xffffffff;
		}
		//纹理x，y坐标
		Quad[i]->v[0].tx = 0, Quad[i]->v[0].ty = 0;
		Quad[i]->v[1].tx = 1, Quad[i]->v[1].ty = 0;
		Quad[i]->v[2].tx = 1, Quad[i]->v[2].ty = 1;
		Quad[i]->v[3].tx = 0, Quad[i]->v[3].ty = 1;
		Quad[i]->blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;
	}
	return true;
}
bool Character::Character_Frame()
{
	if(no_show) //消失
		return false;
	if(state_t <= 0 && (state == 1 || state == 3))
		state = -1;
	if(move <= 0)
		return false;
	//--move;
	if(move == 0)
	{
		x = dx;
		y = dy;
		return false;
	}
	switch(dir)
	{
		case 1:
			if(move % 7 == 0)
				++id;
			if(id >= 8)
				id = 0;
			num = 16 + id;
			x += sx;
			y += sy;
			break;
		case 2:
			if(move % 7 == 0)
				++id;
			if(id >= 8)
				id = 0;
			num = id;
			x += sx;
			y += sy;
			break;
		case 3:
			if(move % 7 == 0)
				++id;
			if(id >= 8)
				id = 0;
			num = 8 + id;
			x += sx;
			y += sy;
			break;
		case 4:
			if(move % 7 == 0)
				++id;
			if(id >= 8)
				id = 0;
			num = 24 + id;
			x += sx;
			y += sy;
			break;
	}
	bgQuad[num]->v[0].x = x - c_size/2, bgQuad[num]->v[0].y = y - c_size/2;
	bgQuad[num]->v[1].x = x + c_size/2, bgQuad[num]->v[1].y = y - c_size/2;
	bgQuad[num]->v[2].x = x + c_size/2, bgQuad[num]->v[2].y = y + c_size/2;
	bgQuad[num]->v[3].x = x - c_size/2, bgQuad[num]->v[3].y = y + c_size/2;
	return false;
}
bool Character::Character_Render()
{
	if(no_show) //消失
		return false;
	if(state >= 0 && state <= 3)
	{
		--round_time;
		float rt = (float)round_time/20.0;
		/*
		Quad[state]->v[0].x = x - c_size/2, Quad[state]->v[0].y = y - c_size/2;
		Quad[state]->v[1].x = x + c_size/2, Quad[state]->v[1].y = y - c_size/2;
		Quad[state]->v[2].x = x + c_size/2, Quad[state]->v[2].y = y + c_size/2;
		Quad[state]->v[3].x = x - c_size/2, Quad[state]->v[3].y = y + c_size/2;
		*/
		//旋转
		int x0 = -c_size/1.7;
		int y0 = -c_size/1.7;
		int x1 = c_size/1.7;
		int y1 = -c_size/1.7;
		int x2 = c_size/1.7;
		int y2 = c_size/1.7;
		int x3 = -c_size/1.7;
		int y3 = c_size/1.7;
		int _x0 = x0*cos(rt) - y0*sin(rt);
		int _y0 = x0*sin(rt) + y0*cos(rt);
		int _x1 = x1*cos(rt) - y1*sin(rt);
		int _y1 = x1*sin(rt) + y1*cos(rt);
		int _x2 = x2*cos(rt) - y2*sin(rt);
		int _y2 = x2*sin(rt) + y2*cos(rt);
		int _x3 = x3*cos(rt) - y3*sin(rt);
		int _y3 = x3*sin(rt) + y3*cos(rt);
		Quad[state]->v[0].x = x + _x0, Quad[state]->v[0].y = y + _y0;
		Quad[state]->v[1].x = x + _x1, Quad[state]->v[1].y = y + _y1;
		Quad[state]->v[2].x = x + _x2, Quad[state]->v[2].y = y + _y2;
		Quad[state]->v[3].x = x + _x3, Quad[state]->v[3].y = y + _y3;
		if(death_time <= 0)
			hge->Gfx_RenderQuad(Quad[state]);
	}
	
	bgQuad[num]->v[0].x = x - c_size/2, bgQuad[num]->v[0].y = y - c_size/2;
	bgQuad[num]->v[1].x = x + c_size/2, bgQuad[num]->v[1].y = y - c_size/2;
	bgQuad[num]->v[2].x = x + c_size/2, bgQuad[num]->v[2].y = y + c_size/2;
	bgQuad[num]->v[3].x = x - c_size/2, bgQuad[num]->v[3].y = y + c_size/2;
	if(death_time > 0)
	{
		float _c_size = c_size*sin(death_time/10.0)/3.3 + c_size/1.2;
		Quad[4]->v[0].x = x - _c_size, Quad[4]->v[0].y = y - _c_size - y*(200-death_time)/200.0;
		Quad[4]->v[1].x = x + _c_size, Quad[4]->v[1].y = y - _c_size - y*(200-death_time)/200.0;
		Quad[4]->v[2].x = x + _c_size, Quad[4]->v[2].y = y + _c_size - y*(200-death_time)/200.0;
		Quad[4]->v[3].x = x - _c_size, Quad[4]->v[3].y = y + _c_size - y*(200-death_time)/200.0;
		bgQuad[num]->v[0].y = y - c_size/2 - y*(200-death_time)/200.0;
		bgQuad[num]->v[1].y = y - c_size/2 - y*(200-death_time)/200.0;
		bgQuad[num]->v[2].y = y + c_size/2 - y*(200-death_time)/200.0;
		bgQuad[num]->v[3].y = y + c_size/2 - y*(200-death_time)/200.0;
		for(int i = 0; i < 4; ++i)
		{
			Quad[4]->v[i].col = ARGB(0xff/400.0*(death_time + 100), 0xff, 0xff, 0xff);
			bgQuad[num]->v[i].col = ARGB(0xff/400.0*(death_time + 100), 0xff, 0xff, 0xff);
		}
		hge->Gfx_RenderQuad(Quad[4]);
	}
	hge->Gfx_RenderQuad(bgQuad[num]);
	return false;
}
void Character::Character_Release()
{
	for(int i = 0; i < 5; ++i)
	{
		if(Quad[i])
		{
			delete Quad[i];
			Quad[i] = 0;
		}
		if(tex[i])
		{
			hge->Texture_Free(tex[i]);
			tex[i] = 0;
		}
	}
	for(int i = 0; i < 32; ++i)
	{
		if(bgQuad[i])
		{
			delete bgQuad[i];
			bgQuad[i] = 0;
		}
		if(texs[i])
		{
			hge->Texture_Free(texs[i]);
			texs[i] = 0;
		}
	}
	return;
}
void Character::setC_Size(int _cs)
{
	c_size = _cs;
}
void Character::setDan(int _v)
{
	for(int i = 0; i < 4; ++i)
		bgQuad[num]->v[i].col = ARGB(_v,0xff,0xff,0xff);
}
void Character::setShi()
{
	for(int i = 0; i < 4; ++i)
		bgQuad[num]->v[i].col = 0xffffffff;
}
bool Character::ok()
{
	return (death_time <= 0);
}
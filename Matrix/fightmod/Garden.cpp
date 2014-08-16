#include "./Garden.h"
#include <fstream>
#include <string.h>
#include <math.h>
#include "../Info.h"
Garden::Garden()
{
}
Garden::~Garden()
{
	//Garden_Release();
}
bool Garden::Garden_Initiate()
{
	tex = hge->Texture_Load("../fightmod/moon.png");

	first = true;
	t = 0;

	//spr替代,陆地
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

	//0-7为障碍，7+为物品
	char tmp[] = "../fightmod/tree ";
	for(int i = 0; i < 14; ++i)
	{
		if(i < 8)
		{
			char tmp1[40];
			strcpy(tmp1, tmp);
			char tmp2[] = "(1).png";
			tmp2[1] = char(i + int('1'));
			strcat(tmp1, tmp2);
			texs[i] = hge->Texture_Load(tmp1);
		}
		else if(i >= 8)
		{
			switch(i)
			{
				case 8:
					//魔力灵药
					texs[i] = hge->Texture_Load("../fightmod/blue.png");
					break;
				case 9:
					//福星高照
					texs[i] = hge->Texture_Load("../fightmod/bomb.png");
					break;
				case 10:
					//生命之书
					texs[i] = hge->Texture_Load("../fightmod/book.png");
					break;
				case 11:
					//魔之法阵
					texs[i] = hge->Texture_Load("../fightmod/magic.png");
					break;
					//神圣之光
				case 12:
					texs[i] = hge->Texture_Load("../fightmod/light.png");
					break;
					//胜利之炎
				case 13:
					texs[i] = hge->Texture_Load("../fightmod/win.png");
					break;
			}
		}

		//spr替代
		bgQuads[i] = new hgeQuad();
		bgQuads[i]->tex = texs[i];
		//屏幕想x，y设置
		bgQuads[i]->v[0].x = 0, bgQuads[i]->v[0].y = 0;
		bgQuads[i]->v[1].x = 0, bgQuads[i]->v[1].y = 0;
		bgQuads[i]->v[2].x = 0, bgQuads[i]->v[2].y = 0;
		bgQuads[i]->v[3].x = 0, bgQuads[i]->v[3].y = 0;
		for(int j = 0; j < 4; ++j)
		{
			bgQuads[i]->v[j].z = 0;
			bgQuads[i]->v[j].col = 0xffffffff;
		}
		//纹理x，y坐标
		bgQuads[i]->v[0].tx = 0, bgQuads[i]->v[0].ty = 0;
		bgQuads[i]->v[1].tx = 1, bgQuads[i]->v[1].ty = 0;
		bgQuads[i]->v[2].tx = 1, bgQuads[i]->v[2].ty = 1;
		bgQuads[i]->v[3].tx = 0, bgQuads[i]->v[3].ty = 1;
		bgQuads[i]->blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;
	}

	isSuper = false;
	mx = new float*[y];
	my = new float*[y];
	act = new int*[y];
	dst = new bool*[y];
	for(int j = 0; j < y; ++j)
	{
		mx[j] = new float[x];
		my[j] = new float[x];
		act[j] = new int[x];
		dst[j] = new bool[x];
		for(int i = 0; i < x; ++i)
		{
			mx[j][i] = i * tot_size + 60.0f;
			my[j][i] = j * tot_size + 20.0f;
			act[j][i] = 0;
			dst[j][i] = 0;
		}
	}
	mz = new float*[y];
	for(int j = 0; j < y; ++j)
	{
		mz[j] = new float[x];
		for(int i = 0; i < x; ++i)
			mz[j][i] = (0.8f - (y - j) / 100.0f) * 0.8;
	}

	return true;
}
bool Garden::Super_Garden_Initiate()
{
	//超级模式，注意x,y的作用域覆盖
	isSuper = true;
	const int x = 28;
	const int y = 20;

	first = true;
	t = 0;

	tex = hge->Texture_Load("../fightmod/moon.png");

	//spr替代,陆地
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

	//0-7为障碍，7+为物品
	char tmp[] = "../fightmod/tree ";
	for(int i = 0; i < 14; ++i)
	{
		if(i < 8)
		{
			char tmp1[40];
			strcpy(tmp1, tmp);
			char tmp2[] = "(1).png";
			tmp2[1] = char(i + int('1'));
			strcat(tmp1, tmp2);
			texs[i] = hge->Texture_Load(tmp1);
		}
		else if(i >= 8)
		{
			switch(i)
			{
				case 8:
					texs[i] = hge->Texture_Load("../fightmod/blue.png");
					break;
				case 9:
					texs[i] = hge->Texture_Load("../fightmod/bomb.png");
					break;
				case 10:
					texs[i] = hge->Texture_Load("../fightmod/book.png");
					break;
				case 11:
					texs[i] = hge->Texture_Load("../fightmod/magic.png");
					break;
				case 12:
					texs[i] = hge->Texture_Load("../fightmod/light.png");
					break;
				case 13:
					texs[i] = hge->Texture_Load("../fightmod/win.png");
					break;
			}
		}

		//spr替代
		bgQuads[i] = new hgeQuad();
		bgQuads[i]->tex = texs[i];
		//屏幕想x，y设置
		bgQuads[i]->v[0].x = 0, bgQuads[i]->v[0].y = 0;
		bgQuads[i]->v[1].x = 0, bgQuads[i]->v[1].y = 0;
		bgQuads[i]->v[2].x = 0, bgQuads[i]->v[2].y = 0;
		bgQuads[i]->v[3].x = 0, bgQuads[i]->v[3].y = 0;
		for(int j = 0; j < 4; ++j)
		{
			bgQuads[i]->v[j].z = 0;
			bgQuads[i]->v[j].col = 0xffffffff;
		}
		//纹理x，y坐标
		bgQuads[i]->v[0].tx = 0, bgQuads[i]->v[0].ty = 0;
		bgQuads[i]->v[1].tx = 1, bgQuads[i]->v[1].ty = 0;
		bgQuads[i]->v[2].tx = 1, bgQuads[i]->v[2].ty = 1;
		bgQuads[i]->v[3].tx = 0, bgQuads[i]->v[3].ty = 1;
		bgQuads[i]->blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;
	}

	mx = new float*[y];
	my = new float*[y];
	act = new int*[y];
	for(int j = 0; j < y; ++j)
	{
		mx[j] = new float[x];
		my[j] = new float[x];
		act[j] = new int[x];
		for(int i = 0; i < x; ++i)
		{
			mx[j][i] = i * tot_size + 60.0f;
			my[j][i] = j * tot_size + 20.0f;
			act[j][i] = 0;
		}
	}
	mz = new float*[y];
	for(int j = 0; j < y; ++j)
	{
		mz[j] = new float[x];
		for(int i = 0; i < x; ++i)
			mz[j][i] = (0.8f - (y - j) / 100.0f) * 0.8;
	}

	return true;
}
bool Garden::Garden_Frame()
{
	++t;
	//处理光环
	for(int k = 0; k < 30 && !first; ++k)
	{
		if(t > 2 && goods[k].first == 0 && goods[k].vis == true)
		{
			int id = 11;
			++goods[k].r_t;
			for(int p = 0; p < 4; ++p)
			{
				int val = goods[k].r_t * 2;
				if(val >= 510)
				{
					val = 0;
					goods[k].r_t = 0;
				}
				if(val >= 255)
					val = 510 - val;
				bgQuads[id]->v[p].col = ARGB(val, 0xff, 0xff, 0xff);
			}
		}	
	}
	if(!first)
		return false;
	//物品初始化
	int t = 0;
	for(int i = 0; i < 30; ++i)
	{
		t += 5;
		int rand_x = rand() % boderX;
		int rand_y = rand() % boderY;
		while(act[rand_y][rand_x] != LAND)
		{
			rand_x = rand() % boderX;
			rand_y = rand() % boderY;
		}
		int id = hge->Random_Int(0, 2);
		goods[i].set_Goods(i, t, rand_x, rand_y, id, false, false, false, 0);
		goods[i].skill = false;
	}
	first = false;
	/*
	if(hge->Input_GetKey() == HGEK_F11)
	{
		float dc = 0.1f;

		std::ifstream files;
		files.open("E:/Matrix/in.txt");
		files>>dc;
		files.close();

		for(int j = 0; j < y; ++j)
		{	
			mz[j] = new float[x];
			for(int i = 0; i < x; ++i)
				mz[j][i] = (0.8f - (y - j) / 100.0f) * dc;
		}
	}
	//*/
	return false;
}
bool Garden::Garden_Frame_GOODS()
{
	//处理动画
	for(int k = 0; k < 30; ++k)
	{
		int i = goods[k].x;
		int j = goods[k].y;
		if(goods[k].first == 1 && goods[k].free == false)
		{
			//方案一，移动过去
			++goods[k].r_t;
			/*
			float rx = - mx[j][i] + 320;
			float ry = - my[j][i] + 200;
			for(int p = 0; p < goods[k]->r_t; ++p)
			{
				rx += (mx[j][i] - 320)/100.0;
				ry += (my[j][i] - 200)/100.0;
			}
			int id = goods[k]->id + 8;
			bgQuads[id]->v[0].x = mx[j][i] - 15.0f + rx, bgQuads[id]->v[0].y = my[j][i] + ry;
			bgQuads[id]->v[1].x = mx[j][i] + 15.0f + rx, bgQuads[id]->v[1].y = my[j][i] + ry;
			bgQuads[id]->v[2].x = mx[j][i] + 15.0f + rx, bgQuads[id]->v[2].y = my[j][i] + 30.0f + ry;
			bgQuads[id]->v[3].x = mx[j][i] - 15.0f + rx, bgQuads[id]->v[3].y = my[j][i] + 30.0f + ry;
			*/
			//方案二，光束
			int id2 = 12;
			bgQuads[id2]->v[0].x = mx[j][i] - goods[k].r_t/3.3 + 3, bgQuads[id2]->v[0].y = 0;
			bgQuads[id2]->v[1].x = mx[j][i] + goods[k].r_t/3.3 - 3, bgQuads[id2]->v[1].y = 0;
			bgQuads[id2]->v[2].x = mx[j][i] + goods[k].r_t/3.3, bgQuads[id2]->v[2].y = my[j][i] + 30.0;
			bgQuads[id2]->v[3].x = mx[j][i] - goods[k].r_t/3.3, bgQuads[id2]->v[3].y = my[j][i] + 30.0;
			//bgQuads[id2]->v[2].x = bgQuads[id]->v[2].x + 50, bgQuads[id2]->v[2].y = bgQuads[id]->v[2].y;
			//bgQuads[id2]->v[3].x = bgQuads[id]->v[3].x, bgQuads[id2]->v[3].y = bgQuads[id]->v[3].y;
		}
	}
	return false;
}
bool Garden::Garden_Render_LAND(int _x, int _y, float _t)
{
	//刷月球
	int val = t / 8;
	if(val > 10)
		t = val = 0;
	if(val > 5)
		val = 10 - val;
	float vs = 1.5 / ((100.0 - _t) * 0.03 + 1.0);
	_t *= vs;
	if(mx && my && mz && act)
	{
		bgQuad->v[0].x = _x - 150 + _t, bgQuad->v[0].y = _y - 150 + _t - val;
		bgQuad->v[1].x = _x + 150 - _t, bgQuad->v[1].y = _y - 150 + _t - val;
		bgQuad->v[2].x = _x + 150 - _t, bgQuad->v[2].y = _y + 150 - _t - val;
		bgQuad->v[3].x = _x - 150 + _t, bgQuad->v[3].y = _y + 150 - _t - val;
		for(int i = 0; i < 4; ++i)
			bgQuad->v[i].col = ARGB(0xbf,0xff,0xff,0xff);
		hge->Gfx_RenderQuad(bgQuad);
	}
	return false;
}
bool Garden::Garden_Render_GOODS(int j, int i)
{
	//显示光环
	for(int k = 0; k < 30; ++k)
	{
		if(goods[k].first == 0 && goods[k].vis == true && goods[k].eat == false && goods[k].x == i && goods[k].y == j)
		{
			int id = 11;
			bgQuads[id]->v[0].x = mx[j][i] - 25.0f, bgQuads[id]->v[0].y = my[j][i] - 10.0f;
			bgQuads[id]->v[1].x = mx[j][i] + 25.0f, bgQuads[id]->v[1].y = my[j][i] - 10.0f;
			bgQuads[id]->v[2].x = mx[j][i] + 25.0f, bgQuads[id]->v[2].y = my[j][i] + 40.0f;
			bgQuads[id]->v[3].x = mx[j][i] - 25.0f, bgQuads[id]->v[3].y = my[j][i] + 40.0f;
			hge->Gfx_RenderQuad(bgQuads[id]);
		}
	}
	//显示物品
	for(int k = 0; k < 30; ++k)
	{
		int id = goods[k].id + 8;
		float xx = mx[j][i];
		float yy = my[j][i];
		if(goods[k].skill)
		{
			xx = goods[k].xx;
			yy = goods[k].yy;
		}
		bgQuads[id]->v[0].x = xx - 15.0f, bgQuads[id]->v[0].y = yy;
		bgQuads[id]->v[1].x = xx + 15.0f, bgQuads[id]->v[1].y = yy;
		bgQuads[id]->v[2].x = xx + 15.0f, bgQuads[id]->v[2].y = yy + 30.0f;
		bgQuads[id]->v[3].x = xx - 15.0f, bgQuads[id]->v[3].y = yy + 30.0f;
		for(int p = 0; p < 4; ++p)
			bgQuads[id]->v[p].col = ARGB(0x5f, 0xff, 0xff, 0xff);
		if(goods[k].free == true && goods[k].eat == false && goods[k].x == i && goods[k].y == j)
		{
			for(int p = 0; p < 4; ++p)
				bgQuads[id]->v[p].col = ARGB(0xff, 0xff, 0xff, 0xff);
			hge->Gfx_RenderQuad(bgQuads[id]);
		}
		if(goods[k].first == 0 && goods[k].vis == true && goods[k].eat == false && goods[k].x == i && goods[k].y == j)
		{
			hge->Gfx_RenderQuad(bgQuads[id]);
		}
		if(goods[k].first == 1 && goods[k].eat == false && goods[k].x == i && goods[k].y == j)
		{
			//方案一，移动
			id = goods[k].id + 8;
			hge->Gfx_RenderQuad(bgQuads[id]);
			//方案二，光束
			id = 12;
			hge->Gfx_RenderQuad(bgQuads[id]);
		}
	}
	return false;
}
bool Garden::Garden_Render_TREE(int j, int i, int cnt, bool ds)
{
	//刷障碍
	int n = (int(cnt * sin(i * 18.0f)) + i + j + cnt) % 7;
	if(ds)
		n = 7;
	bgQuads[n]->v[0].x = mx[j][i] - 30.0f, bgQuads[n]->v[0].y = my[j][i] - 15.0f;
	bgQuads[n]->v[1].x = mx[j][i] + 30.0f, bgQuads[n]->v[1].y = my[j][i] - 15.0f;
	bgQuads[n]->v[2].x = mx[j][i] + 30.0f, bgQuads[n]->v[2].y = my[j][i] + 45.0f;
	bgQuads[n]->v[3].x = mx[j][i] - 30.0f, bgQuads[n]->v[3].y = my[j][i] + 45.0f;
	hge->Gfx_RenderQuad(bgQuads[n]);
	return false;
}
bool Garden::Garden_Render_WIN(int _x, int _y)
{
	int id = 13;
	bgQuads[id]->v[0].x = _x - 110.0f, bgQuads[id]->v[0].y = _y - 110.0f;
	bgQuads[id]->v[1].x = _x + 110.0f, bgQuads[id]->v[1].y = _y - 110.0f;
	bgQuads[id]->v[2].x = _x + 110.0f, bgQuads[id]->v[2].y = _y + 110.0f;
	bgQuads[id]->v[3].x = _x - 110.0f, bgQuads[id]->v[3].y = _y + 110.0f;
	hge->Gfx_RenderQuad(bgQuads[id]);
	return false;
}
void Garden::Garden_Release()
{
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
	for(int i = 0; i < 14; ++i)
	{
		if(bgQuads[i])
			delete bgQuads[i];
		bgQuads[i] = 0;
	}
	for(int i = 0 ; i < 14; ++i)
		if(texs[i])
		{
			hge->Texture_Free(texs[i]);
			texs[i] = 0;
		}
	if(isSuper)
	{
		Super_Garden_Release();
		return;
	}
	if(mx)
	{
		for(int i = 0; i < y; ++i)
			delete []mx[i];
		delete []mx;
		mx = 0;
	}
	if(my)
	{
		for(int i = 0; i < y; ++i)
			delete []my[i];
		delete []my;
		my = 0;
	}
	if(mz)
	{
		for(int i = 0; i < y; ++i)
			delete []mz[i];
		delete []mz;
		mz = 0;
	}
	if(act)
	{
		for(int i = 0; i < y; ++i)
			delete []act[i];
		delete []act;
		act = 0;
	}
	if(dst)
	{
		for(int i = 0; i < y; ++i)
			delete []dst[i];
		delete []dst;
		dst = 0;
	}
}
void Garden::Super_Garden_Release()
{
	//超级模式，注意x,y的作用域覆盖
	const int x = 28;
	const int y = 20;
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
	for(int i = 0; i < 14; ++i)
	{
		if(bgQuads[i])
			delete bgQuads[i];
		bgQuads[i] = 0;
	}
	for(int i = 0 ; i < 14; ++i)
		if(texs[i])
		{
			hge->Texture_Free(texs[i]);
			texs[i] = 0;
		}
	if(mx)
	{
		for(int i = 0; i < y; ++i)
			delete []mx[i];
		delete []mx;
		mx = 0;
	}
	if(my)
	{
		for(int i = 0; i < y; ++i)
			delete []my[i];
		delete []my;
		my = 0;
	}
	if(mz)
	{
		for(int i = 0; i < y; ++i)
			delete []mz[i];
		delete []mz;
		mz = 0;
	}
	if(act)
	{
		for(int i = 0; i < y; ++i)
			delete []act[i];
		delete []act;
		act = 0;
	}
}
bool Garden::check(int _x, int _y)
{
	if(isSuper)
	{
		//超级模式，注意x,y的作用域覆盖
		const int x = 28;
		const int y = 20;
		return ((_x >= 0) && (_y >= 0) && (_x < x) && (_y < y) && (act[_y][_x] == LAND));
	}
	return ((_x >= 0) && (_y >= 0) && (_x < x) && (_y < y) && (act[_y][_x] == LAND));
}
bool Garden::check_skill(int _x, int _y)
{
	if(isSuper)
	{
		//超级模式，注意x,y的作用域覆盖
		const int x = 28;
		const int y = 20;
		return ((_x >= 0) && (_y >= 0) && (_x < x) && (_y < y) && (act[_y][_x] == STAR || act[_y][_x] == LAND));
	}
	return ((_x >= 0) && (_y >= 0) && (_x < x) && (_y < y) && (act[_y][_x] == STAR || act[_y][_x] == LAND));
}
void Garden::setAct(int _x, int _y,int _val)
{
	act[_y][_x] = _val;
}
void Garden::Super_reset()
{
	const int x = 28;
	const int y = 20;
	for(int j = 0; j < y; ++j)
		for(int i = 0; i < x; ++i)
			act[j][i] = 0;
}
void Garden::reset()
{
	if(isSuper)
	{
		Super_reset();
		return;
	}
	for(int j = 0; j < y; ++j)
		for(int i = 0; i < x; ++i)
			act[j][i] = 0;
}
float Garden::getX(int _x, int _y)
{
	return mx[_y][_x];
}
float Garden::getY(int _x, int _y)
{
	return my[_y][_x];
}
float Garden::getZ(int _x, int _y)
{
	return mz[_y][_x];
}
int Garden::getInfo(int _x, int _y)
{
	return act[_y][_x];
}
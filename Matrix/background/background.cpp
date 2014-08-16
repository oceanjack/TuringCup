#include "background.h"
Background::Background(char *p, int d, bool nd)
{
	timed = -1;
	dir = d;
	path = p;
	showfnt = true; //默认显示字体
	nodis = nd;
	errordis = false;
}
Background::~Background()
{
	//Background_Release();
}
bool Background::Background_Initiate()
{
	bgtex = hge->Texture_Load(path);
	if(!bgtex)
	{
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return false;
	}
	//为了适应不同图像大小而采用截图的方法获取最佳匹配图
	chtex = hge->Texture_Load("shotfs001.bmp");
	if(!chtex)
	{
		errordis = true;
		chtex = bgtex;
	}
	else
		errordis = false;
	bgQuad = new hgeQuad();
	bgQuad->tex = bgtex;
	//屏幕想x，y设置
	bgQuad->v[0].x = 0, bgQuad->v[0].y = 0;
	bgQuad->v[1].x = 800, bgQuad->v[1].y = 0;
	bgQuad->v[2].x = 800, bgQuad->v[2].y = 600;
	bgQuad->v[3].x = 0, bgQuad->v[3].y = 600;
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

	//字体
	bgFnt = new hgeFont("../font/font2.fnt");
	if (!bgFnt)
		return false;

	//扭曲
	dis = new hgeDistortionMesh(16, 16);
	dis->SetTexture(chtex);
	dis->SetTextureRect(0, 0, 0, 0);
	dis->SetBlendMode(BLEND_COLORADD | BLEND_ALPHABLEND | BLEND_ZWRITE);
	dis->Clear(0xff000000);
	hge->Random_Seed(int(hge->Timer_GetDelta()));
	return true;
}
bool Background::Background_Frame()
{	
	//变换
	if(bgQuad->v[2].x > 0 && dir == 0) 
	{
		bgQuad->v[2].x -= 1.0f;
		bgQuad->v[3].x += 1.0f;
	}
	else if(bgQuad->v[1].x > 0 && dir == 0)
	{
		bgQuad->v[1].x -= 1.0f;
		bgQuad->v[0].x += 1.0f;
	}
	else if(dir == 0 || dir == 1)
	{
		dir = 1;
		if(bgQuad->v[3].x > 0) 
		{
			bgQuad->v[2].x += 1.0f;
			bgQuad->v[3].x -= 1.0f;
		}
		else if(bgQuad->v[0].x > 0)
		{
			bgQuad->v[1].x += 1.0f;
			bgQuad->v[0].x -= 1.0f;
		}
		else
			dir = 0;
	}
	//扭曲
	if(dir == 0 || dir == 1 || nodis || errordis)
		return false;
	bool click = (hge->Input_GetKey() == HGEK_RBUTTON);
	if(timed <= 0 || click)
	{
		float x, y;
		if(click)
			hge->Input_GetMousePos(&x, &y);
		else
		{
			x = hge->Random_Float(0.0f, 800.0f);
			y = hge->Random_Float(0.0f, 600.0f);
		}
		cx = x;
		cy = y;
		dis->SetTextureRect(cx - 128.0f, cy - 128.0f, 256.0f, 256.0f);
		timed = 300;
	}
	//设置起伏
	int m[16][16] = 
	{
		{0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},
		{0,0,1,1,0,0,-1,-1,-1,0,0,1,1,0,0,0},
		{0,0,1,0,0,-1,0,0,0,-1,0,0,1,0,0,0},
		{0,1,0,0,-1,0,1,1,1,0,-1,0,0,1,0,0},
		{0,1,0,-1,0,1,0,0,0,1,0,-1,0,1,0,0},
		{1,0,-1,0,1,0,0,0,0,0,1,0,-1,0,1,0},
		{1,0,-1,0,1,0,0,-1,0,0,1,0,-1,0,1,0},
		{1,0,-1,0,1,0,0,-1,0,0,1,0,-1,0,1,0},
		{0,1,-1,0,1,0,0,0,0,0,1,0,-1,1,0,0},
		{0,1,0,-1,0,1,0,0,0,1,0,-1,0,1,0,0},
		{0,0,1,0,-1,0,1,1,1,0,-1,0,1,0,0,0},
		{0,0,1,0,0,-1,0,0,0,-1,0,0,1,0,0,0},
		{0,0,0,1,0,0,-1,-1,-1,0,0,1,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0}
	};
	//设置高度
	int h[16][16] = 
	{
		{0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0},
		{0,0,0,0,1,1,2,2,2,1,1,0,0,0,0,0},
		{0,0,1,1,2,2,3,3,3,2,2,1,1,0,0,0},
		{0,0,1,2,2,3,3,3,3,3,2,2,1,0,0,0},
		{0,1,2,2,3,3,4,4,4,3,3,2,2,1,0,0},
		{0,1,2,3,3,4,4,4,4,4,3,3,2,1,0,0},
		{1,2,3,3,4,4,4,5,4,4,4,3,3,2,1,0},
		{1,2,3,3,4,4,5,5,5,4,4,3,3,2,1,0},
		{1,2,3,3,4,4,5,5,5,4,4,3,3,2,1,0},
		{0,1,3,3,4,4,4,5,4,4,4,3,3,1,0,0},
		{0,1,2,3,3,4,4,4,4,4,3,3,2,1,0,0},
		{0,0,1,2,3,3,4,4,4,3,3,2,1,0,0,0},
		{0,0,1,2,2,3,3,3,3,3,2,2,1,0,0,0},
		{0,0,0,1,2,2,3,3,3,2,2,1,0,0,0,0},
		{0,0,0,0,1,1,2,2,2,1,1,0,0,0,0,0},
		{0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0}
	};
	if(timed > 0)
	{
		--timed;
		for(int i = 0; i < 16; ++i)
			for(int j = 0; j < 16; ++j)
			{
				//高度调整
				if(h[i][j] != 0)
					h[i][j] -= (4 - timed/70);
				if(h[i][j] < 0)
					h[i][j] /= 3;
				if(timed > 270)
					h[i][j] /= 2;
				dis->SetDisplacement(j, i, sinf(timed/15.0f + m[i][j])*h[i][j]/1.5f, sinf(timed/15.0f + m[i][j])*h[i][j]/1.5f, HGEDISP_NODE);
			}
	}
	return false;
}
bool Background::Background_Render()
{
	//背景
	hge->Gfx_RenderQuad(bgQuad);
	//扭曲
	if(dir != 0 && dir != 1 && timed > 0)
		dis->Render(cx - 128, cy - 128);
	if(showfnt)
		bgFnt->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d (constant)\n", hge->Timer_GetDelta(), hge->Timer_GetFPS());
	return false;
}
void Background::Background_Release()
{
	if(bgQuad)
		delete bgQuad;
	if(bgFnt)
		delete bgFnt;
	if(dis)
		delete dis;
	if(bgtex != chtex)
		hge->Texture_Free(chtex);
	if(bgtex)
		hge->Texture_Free(bgtex);
	return;
}
void Background::Set_Fnt(bool s)
{
	showfnt = s;
	return;
}
void Background::Set_Size(float h, float w)
{
	//屏幕想x，y设置
	bgQuad->v[0].x = 0, bgQuad->v[0].y = 0;
	bgQuad->v[1].x = w, bgQuad->v[1].y = 0;
	bgQuad->v[2].x = w, bgQuad->v[2].y = h;
	bgQuad->v[3].x = 0, bgQuad->v[3].y = h;
}

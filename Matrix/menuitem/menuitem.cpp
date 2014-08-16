#include "menuitem.h"
hgeGUIMenuItem::hgeGUIMenuItem(int _id, float _x, float _y, float _dx, float _dy, float _dx2, float _dy2, float _delay, char *p, char *p2, bool ps)
{
	id = _id;
	x = _x;
	y = _y;
	dx = _dx;
	dy = _dy;
	dx2 = _dx2;
	dy2 = _dy2;
	delay = _delay;
	path = p;
	path2 = p2;
	if(ps)
		cx = float(800 + id * 40);
	else
		cx = _x;
	cy = _y;
	mx = 1.0f;
	my = 1.0f;
	model = 1;

	isRun = false;
	isFocus = false;

	bStatic = false;
	bVisible = true;
	bEnabled = true;
	hge->Random_Seed(int(hge->Timer_GetDelta()));
}
bool hgeGUIMenuItem::hgeGUIMenuItem_Initiate()
{
	//按钮资源
	snd = new Effect("../sound/menu.wav");
	snd->Effect_Initiate();
	tex = hge->Texture_Load(path);
	tex3 = hge->Texture_Load("../menuitem/2.png");
	if((!tex) || (!tex3))
	{
		MessageBox(NULL, hge->System_GetErrorMessage(), "menu_Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return false;
	}

	//代替spr
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

	spr3 = new hgeSprite(tex3, 96, 0, 32, 32);
	spr3->SetHotSpot(-16, 0);
	//粒子系统
	par = new hgeParticleSystem("../menuitem/2.psi", spr3);
	return true;
}
//相当于逻辑层
void hgeGUIMenuItem::Update(float dt)
{
	if(model == 1) //进入
	{
		if(cx > x)
			cx -= 3.0f;
		else
		{
			cx = x;
			isFinish = true;
			model = 3;
		}
	}
	else if(model == 2) //退出
	{
		if(cx < 800)
			cx += 3.0f;
		else
		{
			cx = 800;
			isFinish = true;
			model = 3;
		}
	}
	else if(model == 3 && isRun) //乱跑
	{
		if(cx > 720 || cx < 0)
		{
			if(mx > 0)
				mx = -(hge->Random_Float(1.0f, 2.0f));
			else
				mx = (hge->Random_Float(1.0f, 2.0f));
			cx += mx;
		}
		if(cy > 550 || cy < 0)
		{
			if(my > 0)
				my = -(hge->Random_Float(1.0f, 2.0f));
			else
				my = (hge->Random_Float(1.0f, 2.0f));
			cy += my;
		}
		cx += mx;
		cy += my;
	}
	//碰撞检测
	rect.Set(cx, cy, cx+80.3f, cy+40.0f);
	//粒子效果
	par->Fire();
	par->info.nEmission = 30;
	par->info.fLifetime = 100;
	par->MoveTo(cx, cy);
	par->Update(hge->Timer_GetDelta());
	return;
}
void hgeGUIMenuItem::Render()
{
	if(isFocus || isRun)
		par->Render();
	if(isFocus)
	{
		bgQuad->v[0].x = cx - 50.0f, bgQuad->v[0].y = cy - 65.0f;
		bgQuad->v[1].x = cx + 150.0f, bgQuad->v[1].y = cy - 65.0f;
		bgQuad->v[2].x = cx + 150.0f, bgQuad->v[2].y = cy + 135.0f;
		bgQuad->v[3].x = cx - 50.0f, bgQuad->v[3].y = cy + 135.0f;
		hge->Gfx_RenderQuad(bgQuad);
	}
	else
	{
		bgQuad->v[0].x = cx - 50.0f, bgQuad->v[0].y = cy - 70.0f;
		bgQuad->v[1].x = cx + 150.0f, bgQuad->v[1].y = cy - 70.0f;
		bgQuad->v[2].x = cx + 150.0f, bgQuad->v[2].y = cy + 130.0f;
		bgQuad->v[3].x = cx - 50.0f, bgQuad->v[3].y = cy + 130.0f;
		hge->Gfx_RenderQuad(bgQuad);
	}
	return;
}
void hgeGUIMenuItem::hgeGUIMenuItem_Release()
{
	if(snd)
	{
		snd->Effect_Release();
		delete snd;
		snd = 0;
	}
	if(par)
	{
		delete par;
		par = 0;
	}
	if(bgQuad)
	{
		delete bgQuad;
		bgQuad = 0;
	}
	if(spr3)
	{
		delete spr3;
		spr3 = 0;
	}
	if(tex)
	{
		hge->Texture_Free(tex);
		tex = 0;
	}
	if(tex3)
	{
		hge->Texture_Free(tex3);
		tex3 = 0;
	}
	return;
}
//进入动画
void hgeGUIMenuItem::Enter()
{
	model = 1;
	isFinish = false;
	return;
}
//退出动画
void hgeGUIMenuItem::Leave()
{
	model = 2;
	isFinish = false;
	return;
}
//
bool hgeGUIMenuItem::IsDone()
{
	return isFinish;
}
//焦点
void hgeGUIMenuItem::Focus(bool bFocused)
{
	if(bFocused)
	{
		snd->Effect_Frame();
		isFocus = true;
	}
	else
		isFocus = false;
	return;
}
//鼠标接触
void hgeGUIMenuItem::MouseOver(bool bOver)
{
	if(bOver)
		gui->SetFocus(id);
	else
		gui->SetFocus(0);
	return;
}
//鼠标点击
bool hgeGUIMenuItem::MouseLButton(bool bDown)
{
	if(bDown)
		return true;
	return false;
}
bool hgeGUIMenuItem::MouseRButton(bool bDown)
{
	if(bDown)
	{
		isRun = (bool)(1 - int(isRun));
		return true;
	}
	return false;
}
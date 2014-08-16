#include "./skill.h"
#include "../Info.h"
#include <string.h>
#include <fstream>
Skill::Skill(int _x, int _y, int _dir, char *_path, int _l)
{
	psi_path = _path;
	x = _x;
	y = _y;
	int px = 0, py = 0;
	if(_dir == UP)
		px = 0, py = -1;
	else if(_dir == DOWN)
		px = 0, py = 1;
	else if(_dir == LEFT)
		px = -1, py = 0;
	else if(_dir == RIGHT)
		px = 1, py = 0;
	cx[0] = x;
	cy[0] = y;
	len = _l;
	if(len >= 30)
		len = 30;
	for(int i = 1; i < len; ++i)
	{
		cx[i] = cx[i-1] + px;
		cy[i] = cy[i-1] + py;
	}
}
Skill::~Skill()
{
	//Skill_Release();
}
bool Skill::Skill_Initiate()
{
	id = 1;
	s = t = 0;
	dt = 0;
	tt = 0;
	move = false;
	char tmp[50];
	strcpy(tmp, psi_path);
	tmp[strlen(tmp) - 3] = 'p';
	tmp[strlen(tmp) - 2] = 'n';
	tmp[strlen(tmp) - 1] = 'g';
	tex = hge->Texture_Load(tmp);
	if(!tex)
	{
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return false;
	}

	//纹理
	spr = new hgeSprite(tex, 0, 0, 24, 24);
	spr->SetHotSpot(0, 0);

	//粒子系统
	par = new hgeParticleSystem(psi_path, spr);

	return true;
}
bool Skill::Skill_Frame()
{
	++tt;
	s = t;
	if(id != 1)
		return true;
	//++dt;
	if(dt > speed)
	{
		++t;
		move = false;
		dt = 0;
		tt = 0;
	}
	if(t >= len)
	{
		id = 0;
		return true;
	}
	x = cx[t];
	y = cy[t];
	if(x < 0 || y < 0 || x >= boderX || y >= boderY)
	{
		id = 0;
		return true;
	}
	par->Fire();
	par->info.nEmission = 80;
	int dcx = 0;
	int dcy = 0;
	if(t + 1 < len && (cx[t + 1] - x) == 0)
		dcy = 1;
	if(t + 1 < len && (cy[t + 1] - y) == 0)
		dcx = 1;
	par->FireAt(x * tot_size + 40.0f + tt*2 * dcx, y * tot_size + 10.0f + tt*2 * dcy);
	par->MoveTo(x * tot_size + 40.0f + tt*2 * dcx, y * tot_size + 10.0f + tt*2 * dcy);
	par->Update(hge->Timer_GetDelta());
	return false;
}
bool Skill::Skill_Render()
{
	if(id == 1)
	{
		par->Render();
	}
	return false;
}
void Skill::Skill_Release()
{
	if(par)
		delete par;
	if(spr)
		delete spr;
	if(tex)
		hge->Texture_Free(tex);
	return;
}
void Skill::setID()
{
	id = 0;
}
void Skill::setPath(int dir[])
{
	int _x = cx[0];
	int _y = cy[0];
	if(len >= 30)
		len = 30;
	for(int i = 0; i < len; ++i)
	{
		switch(dir[i])
		{
			case UP:
				cy[i] = _y - 1;
				cx[i] = _x;
				break;
			case DOWN:
				cy[i] = _y + 1;
				cx[i] = _x;
				break;
			case LEFT:
				cy[i] = _y;
				cx[i] = _x - 1;
				break;
			case RIGHT:
				cy[i] = _y;
				cx[i] = _x + 1;
				break;
			default:
				cy[i] = _y;
				cx[i] = _x;
		}
		_x = cx[i];
		_y = cy[i];
	}
}
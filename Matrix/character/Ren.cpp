#include "./Ren.h"
Ren::Ren(char *p, char *p2, char *p3, char *p4)
{
	path = p;
	path2 = p2;
	path3 = p3;
	path4 = p4;
	for(int i = 0; i < 4; ++i)
		pos[i] = i;
}
Ren::~Ren()
{
}
bool Ren::Ren_Initiate()
{
	x = 250;
	y = 400;
	s = 85;
	t = 0;
	dir = 0;
	ready = true;

	tex[0] = hge->Texture_Load(path);
	tex[1] = hge->Texture_Load(path2);
	tex[2] = hge->Texture_Load(path3);
	tex[3] = hge->Texture_Load(path4);

	for(int i = 0; i < 4; ++i)
	{
		Quad[i] = new hgeQuad();
		Quad[i]->tex = tex[i];
		for(int j = 0; j < 4; ++j)
		{
			Quad[i]->v[j].x = 0;
			Quad[i]->v[j].y = 0;
			Quad[i]->v[j].z = 0;
			Quad[i]->v[j].col = 0xffffffff;
		}
		Quad[i]->v[0].tx = 0, Quad[i]->v[0].ty = 0;
		Quad[i]->v[1].tx = 1, Quad[i]->v[1].ty = 0;
		Quad[i]->v[2].tx = 1, Quad[i]->v[2].ty = 1;
		Quad[i]->v[3].tx = 0, Quad[i]->v[3].ty = 1;
		Quad[i]->blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;
	}
	return true;
}
bool Ren::Ren_Frame()
{
	--t;
	if(t <= 0)
	{
		ready = true;
		t = 0;
		dir = 0;
	}
	if(ready)
	{
		int ps;
		switch(hge->Input_GetKey())
		{
			case HGEK_LEFT:
				t = 100;
				ready = false;
				dir = -1;
				ps = pos[0];
				for(int i = 0; i < 3; ++i)
					pos[i] = pos[i+1];
				pos[3] = ps;
			break;
		}
		for(int i = 0; i < 4 && ready; ++i)
		{
			int k = pos[i];
			switch(i)
			{
				case 0:
					for(int j = 0; j < 4; ++j)
					{
						Quad[k]->v[j].x = 0;
						Quad[k]->v[j].y = 0;
						Quad[k]->v[j].z = 0;
						Quad[k]->v[j].col = 0x00ffffff;
					}
				break;
				case 1:
					Quad[k]->v[0].x = x - s*1.5, Quad[k]->v[0].y = y - s*0.5;
					Quad[k]->v[1].x = x - s*0.5, Quad[k]->v[1].y = y - s*1.0;
					Quad[k]->v[2].x = x - s*0.5, Quad[k]->v[2].y = y + s*1.0;
					Quad[k]->v[3].x = x - s*1.5, Quad[k]->v[3].y = y + s*0.5;
					for(int j = 0; j < 4; ++j)
					{
						Quad[k]->v[j].col = ARGB(0x2f, 0xff, 0xff, 0xff);
					}
				break;
				case 2:
					Quad[k]->v[0].x = x - s, Quad[k]->v[0].y = y - s;
					Quad[k]->v[1].x = x + s, Quad[k]->v[1].y = y - s;
					Quad[k]->v[2].x = x + s, Quad[k]->v[2].y = y + s;
					Quad[k]->v[3].x = x - s, Quad[k]->v[3].y = y + s;
					for(int j = 0; j < 4; ++j)
					{
						Quad[k]->v[j].col = ARGB(0x9f, 0xff, 0xff, 0xff);
					}
				break;
				case 3:
					Quad[k]->v[0].x = x + s*0.5, Quad[k]->v[0].y = y - s*1.0;
					Quad[k]->v[1].x = x + s*1.5, Quad[k]->v[1].y = y - s*0.5;
					Quad[k]->v[2].x = x + s*1.5, Quad[k]->v[2].y = y + s*0.5;
					Quad[k]->v[3].x = x + s*0.5, Quad[k]->v[3].y = y + s*1.0;
					for(int j = 0; j < 4; ++j)
					{
						Quad[k]->v[j].col = ARGB(0x2f, 0xff, 0xff, 0xff);
					}
				break;
			}
		}
	}
	else
	{
		if(dir == -1)
		{
			for(int i = 0; i < 4; ++i)
			{
				int k = pos[i];
				switch(i)
				{
					case 0:
						Quad[k]->v[0].x = x - s*1.5 + s*1.5/100.0*(100-t), Quad[k]->v[0].y = y - s*0.5 + s*0.5/100.0*(100-t);
						Quad[k]->v[1].x = x - s*0.5 + s*0.5/100.0*(100-t), Quad[k]->v[1].y = y - s*1.0 + s*0.5/100.0*(100-t);
						Quad[k]->v[2].x = x - s*0.5 + s*0.5/100.0*(100-t), Quad[k]->v[2].y = y + s*1.0 - s*1.0/100.0*(100-t);
						Quad[k]->v[3].x = x - s*1.5 + s*1.5/100.0*(100-t), Quad[k]->v[3].y = y + s*0.5 - s*1.0/100.0*(100-t);
						for(int j = 0; j < 4; ++j)
						{
							Quad[k]->v[j].col = ARGB(0x2f - 0x2f/100.0*(100-t), 0xff, 0xff, 0xff);
						}
					break;
					case 1:
						Quad[k]->v[0].x = x - s - s*0.5/100.0*(100-t), Quad[k]->v[0].y = y - s + s*0.5/100.0*(100-t);
						Quad[k]->v[1].x = x + s - s*1.5/100.0*(100-t), Quad[k]->v[1].y = y - s*1.0;
						Quad[k]->v[2].x = x + s - s*1.5/100.0*(100-t), Quad[k]->v[2].y = y + s*1.0;
						Quad[k]->v[3].x = x - s - s*0.5/100.0*(100-t), Quad[k]->v[3].y = y + s - s*0.5/100.0*(100-t);
						for(int j = 0; j < 4; ++j)
						{
							Quad[k]->v[j].col = ARGB(0x9f - 0x7f/100.0*(100-t), 0xff, 0xff, 0xff);
						}
					break;
					case 2:
						Quad[k]->v[0].x = x + s*0.5 - s*1.5/100.0*(100-t), Quad[k]->v[0].y = y - s;
						Quad[k]->v[1].x = x + s*1.5 - s*0.5/100.0*(100-t), Quad[k]->v[1].y = y - s*0.5 - s*0.5/100.0*(100-t);
						Quad[k]->v[2].x = x + s*1.5 - s*0.5/100.0*(100-t), Quad[k]->v[2].y = y + s*0.5 + s*0.5/100.0*(100-t);
						Quad[k]->v[3].x = x + s*0.5 - s*1.5/100.0*(100-t), Quad[k]->v[3].y = y + s;
						for(int j = 0; j < 4; ++j)
						{
							Quad[k]->v[j].col = ARGB(0x2f + 0x7f/100.0*(100-t), 0xff, 0xff, 0xff);
						}
					break;
					case 3:
						Quad[k]->v[0].x = x + s*0.5/100.0*(100-t), Quad[k]->v[0].y = y - s*1.0/100.0*(100-t);
						Quad[k]->v[1].x = x + s*1.5/100.0*(100-t), Quad[k]->v[1].y = y - s*0.5/100.0*(100-t);
						Quad[k]->v[2].x = x + s*1.5/100.0*(100-t), Quad[k]->v[2].y = y + s*0.5/100.0*(100-t);
						Quad[k]->v[3].x = x + s*0.5/100.0*(100-t), Quad[k]->v[3].y = y + s*1.0/100.0*(100-t);
						for(int j = 0; j < 4; ++j)
						{
							Quad[k]->v[j].col = ARGB(0x2f/100.0*(100-t), 0xff, 0xff, 0xff);
						}
					break;
				}
			}
		}
	}
	return false;
}
bool Ren::Ren_Render()
{
	hge->Gfx_RenderQuad(Quad[pos[0]]);
	hge->Gfx_RenderQuad(Quad[pos[1]]);
	hge->Gfx_RenderQuad(Quad[pos[3]]);
	hge->Gfx_RenderQuad(Quad[pos[2]]);
	return false;
}
void Ren::Ren_Release()
{
	for(int i = 0; i < 4; ++i)
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
}
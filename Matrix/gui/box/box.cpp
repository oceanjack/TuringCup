#include "box.h"
#include <string.h>
box::box(char *p)
{
	path = p;
}
box::~box()
{
	box_Release();
}
bool box::box_Initiate()
{
	list_path1 = list_path2 = list_path3 = 0;
	font = new hgeFont("../font/font3.fnt");
	tex = hge->Texture_Load("../gui/box/box.png");
	if(!tex || !font)
	{
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		return false;
	}
	font->SetScale(0.8);

	//spr替代
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

	//GUI
	gui = new hgeGUI();
	//按钮
	Item3 = new hgeGUIMenuItem(1, 420, 470, 0, 0, 0, 0, 0, "../menuitem/1_4.png", "../menuitem/1_4.png", false);
	Item4 = new hgeGUIMenuItem(2, 520, 470, 0, 0, 0, 0, 0, "../menuitem/1_5.png", "../menuitem/1_5.png", false);
	Item3->hgeGUIMenuItem_Initiate();
	Item4->hgeGUIMenuItem_Initiate();
	gui->AddCtrl(Item3);
	gui->AddCtrl(Item4);
	//列表
	list1 = new AIc("../Map/*.map", font, 3, 200, 130, 100, 320);
	list2 = new AIc("../AI/*.dll", font, 4, 350, 130, 100, 320);
	list3 = new AIc("../AI/*.dll", font, 5, 500, 130, 100, 320);
	list1->AIc_Initiate();
	list2->AIc_Initiate();
	list3->AIc_Initiate();
	gui->AddCtrl(list1);
	gui->AddCtrl(list2);
	gui->AddCtrl(list3);
	gui->SetFocus(0);
	gui->Enter();
	return true;
}
bool box::box_Frame(bool &ready)
{
	//按钮检测
	ready = false;
	gui->Update(hge->Timer_GetDelta());
	int id = gui->GetFocus();
	switch(hge->Input_GetKey())
	{
		case HGEK_LBUTTON:
			switch(id)
			{
				case 1:
					list_path1 = list1->getPath(list1->GetSelectedItem());
					list_path2 = list2->getPath(list2->GetSelectedItem());
					list_path3 = list3->getPath(list3->GetSelectedItem());
					ready = true;
					break;
				case 2:
					list_path1 = list1->getPath(list1->GetSelectedItem());
					list_path2 = list2->getPath(list2->GetSelectedItem());
					list_path3 = list3->getPath(list3->GetSelectedItem());
					ready = true;
					return true;
					break;
				default:
					break;
			}
			break;
		case HGEK_4:
			list_path1 = list1->getPath(list1->GetSelectedItem());
			list_path2 = list2->getPath(list2->GetSelectedItem());
			list_path3 = list3->getPath(list3->GetSelectedItem());
			ready = true;
			break;
		case HGEK_5: 
			list_path1 = list1->getPath(list1->GetSelectedItem());
			list_path2 = list2->getPath(list2->GetSelectedItem());
			list_path3 = list3->getPath(list3->GetSelectedItem());
			ready = true;
			return true;
			break;
	}
	return false;
}
bool box::box_Render()
{
	bgQuad->v[0].x = 165, bgQuad->v[0].y = 65;
	bgQuad->v[1].x = 635, bgQuad->v[1].y = 65;
	bgQuad->v[2].x = 635, bgQuad->v[2].y = 535;
	bgQuad->v[3].x = 165, bgQuad->v[3].y = 535;
	hge->Gfx_RenderQuad(bgQuad);
	gui->Render();
	font->SetColor(0xffff0000);
	font->printf(200, 90, HGETEXT_LEFT, "%s", "Map:");
	font->printf(350, 90, HGETEXT_LEFT, "%s", "Left:");
	font->printf(500, 90, HGETEXT_LEFT, "%s", "Right:");
	return false;
}
void box::box_Release()
{
	if(bgQuad)
	{
		delete bgQuad;
		bgQuad = 0;
	}
	if(Item3)
	{
		Item3->hgeGUIMenuItem_Release();
		Item3 = 0;
	}
	if(Item4)
	{
		Item4->hgeGUIMenuItem_Release();
		Item4 = 0;
	}
	if(list1)
	{
		list1->AIc_Release();
		list1 = 0;
	}
	if(list2)
	{
		list2->AIc_Release();
		list2 = 0;
	}
	if(list3)
	{
		list3->AIc_Release();
		list3 = 0;
	}
	if(gui)
	{
		delete gui;
		gui = 0;
	}
	if(font)
	{
		delete font;
		font = 0;
	}
	if(tex)
	{
		hge->Texture_Free(tex);
		tex = 0;
	}
	return;
}
bool box::getPath(char *m, char *a1, char *a2)
{
	if(list_path1 && list_path2 && list_path1)
	{
		strcpy(m, list_path1);
		strcpy(a1, list_path2);
		strcpy(a2, list_path3);
		return true;
	}
	else
		return false;
}
#include "./header.h"
#include "./sound/sound.h"
#include "./background/background.h"
#include "./cursor/cursor.h"
#include "./character/character.h"
#include "./gui/box/box.h"
#include "./background/autoblack.h"
#include "./fightmod/Garden.h"
#include "./timer/timer.h"
#include "./star/star.h"
#include "./background/Stars.h"
#include "./skill/skill.h"
#include "./fontCN/GfxFont.h"
#include "./fontCN/GfxEdit.h"
#include "./character/Ren.h"
#include "./orz/orz.h"
#include "./orz/orz2.h"
#include "./User.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <string>
#include <direct.h>

using namespace::std;

HGE *hge; //hge指针

bool firstrun = true;
bool stop = false; //暂停
bool window = true; //窗口
bool bgsound = false; //音乐加载标志，ture表示已加载
bool changesign = false; //转场标志
bool musicState = true; //音乐状态
bool effectState = true; //音效状态

//场景标识
int model_index = 0; //当前场景
const int max_model_index = 2; //最大场景数
const int MENU = 1;
const int PREPARE = 0;
const int TEST = 2;

//大技能标示
int big_skill_id = 0;
int big_skill_time = 0;
//大技能cd时间
int big_skill_cd_one = 0;
int big_skill_cd_two = 0;
int big_skill_cd_three = 0;
int big_skill_cd_four = 0;

//背景音乐用
int fntTime = 0; //一些字体的显示时间

//主函数声明
bool Initiate(); //主初始化
bool FrameFunc(); //主逻辑层
bool RenderFunc(); //主渲染层
void Release(); //卸载资源

//公共
AutoBlack atb(150, 1);
Garden garden; //映射表
int game_speed = 100;
GfxFont *fnt; //中文字体
GfxFont *fnt2;
GfxFont *fnt3;
ORZ *orz[17] = {0};
ORZ2 *orz2[30] = {0};
int run_time;
//录像记录信息
bool replay;
struct Num_info
{
	int goods_info_num;
	int dead_info_num;
	int order_info_num;
} num_info;
struct Goods_info //物品变换信息
{
	int t;
	Goods g[30]; //地图物品
	int ng1; //人物1物品数量
	int g1[30]; //人物1物品
	int ng2; //人物2物品数量
	int g2[30]; //人物2物品
	int ng3; //人物3物品数量
	int g3[30]; //人物3物品
	int ng4; //人物4物品数量
	int g4[30]; //人物4物品
} goods_info[500];
struct Dead_info //死亡信息
{
	int t;
	int who;
	int x;
	int y;
} dead_info[500];
struct Order_info //有效指令信息
{
	int t;
	int who;
	Order o;
} order_info[10010];
struct Map_info
{
	PlayerName pn[2];
	int m[boderY][boderX];
} map_info;
void FPS_FrameFunc()
{
	int r_FPS = hge->Timer_GetFPS();
	char title[30] = "Magic Field - ";
	char r_fps[5];
	int head = 0;
	while(r_FPS)
	{
		r_fps[head] = (r_FPS%10) + '0';
		++head;
		r_FPS /= 10;
	}
	int i = head - 1, j = 0;
	for( ; i >= 0; --i, ++j)
		title[14 + j] = r_fps[i];
	title[14 + j] = '\0';
	hge->System_SetState(HGE_TITLE, title);
}

//AI接口
typedef Order (*get_order)(User *);
typedef PlayerName (*NameAI)();
get_order proc[2];
NameAI proc2[2];
Order AI_order[2];
PlayerName AI_name[2];
User *user[2];
char map_path[50];
char ai1_path[50];
char ai2_path[50];
const int AI_WAIT_TIME = 77;

//AI函数附加
DWORD WINAPI AILS1(LPVOID user)
{
	if(proc[0])
	{
		AI_order[0] = proc[0]((User*)user);
	}
	return 1;
}
DWORD WINAPI AILS2(LPVOID user)
{
	if(proc[1])
	{
		AI_order[1] = proc[1]((User*)user);
	}
	return 1;
}
void AI_work(int id,bool ok)
{
	HANDLE handle = 0;
	int o_id = id;
	if(id == 0 || id == 2)
		id = 0;
	else
		id = 1;
	if(!ok)
	{
		Order tmp;
		tmp.act = STOP;
		AI_order[id] = tmp;
	}
	else
	{
		if(replay == false)
		{
			if(proc[id])
			{
				if(id == 0)
					handle = CreateThread(NULL,0,&AILS1,user[id],0,NULL);
				else
					handle = CreateThread(NULL,0,&AILS2,user[id],0,NULL);
			}/*
			if(IsDebuggerPresent())
			{
				WaitForSingleObject(handle,INFINITE);
			}
			else */
			if(WaitForSingleObject(handle,AI_WAIT_TIME) == WAIT_TIMEOUT)
			{
				SuspendThread(handle);
				TerminateThread(handle, 0);
				Order tmp;
				tmp.act = STOP;
				AI_order[id] = tmp;
				//if(id == 0)
				//	MessageBox(hge->System_GetState(HGE_HWND),"A超时了",ERROR,MB_OK);
				//else
				//	MessageBox(hge->System_GetState(HGE_HWND),"B超时了",ERROR,MB_OK);
			}
			else
			{
				SuspendThread(handle);
				TerminateThread(handle, 0);
			}
		}
		else
		{
			Order tmp;
			tmp.act = STOP;
			AI_order[id] = tmp;
		}
	}
	//指令信息记录
	if(replay == false)
	{
		order_info[run_time].t = run_time;
		order_info[run_time].o = AI_order[id];
		order_info[run_time].who = o_id;
	}
	else
	{
		AI_order[id] = order_info[run_time].o;
	}
}
void AI_getname()
{
	if(replay == false)
	{
		AI_name[0] = proc2[0]();
		AI_name[1] = proc2[1]();
		for(int i = 0; AI_name[0].TeamName[i] != '\0'; ++i)
			if(AI_name[0].TeamName[i] == ' ')
				AI_name[0].TeamName[i] = '-';
		for(int i = 0; AI_name[1].TeamName[i] != '\0'; ++i)
			if(AI_name[1].TeamName[i] == ' ')
				AI_name[1].TeamName[i] = '-';

		for(int i = 0; AI_name[0].my1Name[i] != '\0'; ++i)
			if(AI_name[0].my1Name[i] == ' ')
				AI_name[0].my1Name[i] = '-';
		for(int i = 0; AI_name[0].my2Name[i] != '\0'; ++i)
			if(AI_name[0].my2Name[i] == ' ')
				AI_name[0].my2Name[i] = '-';

		for(int i = 0; AI_name[1].my1Name[i] != '\0'; ++i)
			if(AI_name[1].my1Name[i] == ' ')
				AI_name[1].my1Name[i] = '-';
		for(int i = 0; AI_name[1].my2Name[i] != '\0'; ++i)
			if(AI_name[1].my2Name[i] == ' ')
				AI_name[1].my2Name[i] = '-';
	}
	else
	{
		AI_name[0] = map_info.pn[0];
		AI_name[1] = map_info.pn[1];
	}
}
bool AI_init()
{
	user[0] = new User();
	user[1] = new User();
	HINSTANCE hdll[2];
	char tmp[100];
	if(replay)
	{
		strcpy(ai1_path, "KeyBoard.dll");
		strcpy(ai2_path, "KeyBoard.dll");
	}
	strcpy_s(tmp, "../AI/");
	strcat_s(tmp, ai1_path);
	strcpy_s(ai1_path, tmp);
	strcpy_s(tmp, "../AI/");
	strcat_s(tmp, ai2_path);
	strcpy_s(ai2_path, tmp);
	hdll[0] = LoadLibrary(ai1_path);
	hdll[1] = LoadLibrary(ai2_path);
	proc[0] = (get_order)GetProcAddress(hdll[0], "get_order");
	proc[1] = (get_order)GetProcAddress(hdll[1], "get_order");
	proc2[0] = (NameAI)GetProcAddress(hdll[0], "NameAI");
	proc2[1] = (NameAI)GetProcAddress(hdll[1], "NameAI");
	return true;
}
void Map_init()
{
	if(replay)
	{
		for(int j = 0; j < 17; ++j)
			for(int i = 0; i < 22; ++i)
				garden.setAct(i, j, map_info.m[j][i]);
		return;
	}
	char tmp[100];
	strcpy_s(tmp, "../Map/");
	strcat_s(tmp, map_path);
	strcpy_s(map_path, tmp);
	int b_x, b_y;
	freopen(map_path, "r", stdin);
	scanf("%d%d", &b_x, &b_y);
	for(int j = 0; j < b_y; ++j)
		for(int i = 0; i < b_x; ++i)
		{
			int tmp;
			scanf("%d", &tmp);
			garden.setAct(i, j, tmp);
			if(replay == false)
				map_info.m[j][i] = tmp;
		}
}
string getNum(int num)
{
	string a = "";
	while(num)
	{
		a += ((num%10) + '0');
		num /= 10;
	}
	for(int i = 0; i < a.size()/2; ++i)
	{
		char ch = a[i];
		a[i] = a[a.size()-i-1];
		a[a.size()-i-1] = ch;
	}
	return a;
}
void Save_replay(bool reSave = false)
{
	string tt = "";
	if(reSave)
	{
		char tmp_path[100] = "..\\replay\\";
		time_t now;	
		struct tm *fmt;
		time(&now);
		fmt = localtime(&now);
		tt += getNum(fmt->tm_year+1900);
		tt += '-';
		tt += getNum(fmt->tm_mon+1);
		tt += '-';
		tt += getNum(fmt->tm_mday);
		tt += '-';
		tt += getNum(fmt->tm_hour);
		tt += '-';
		tt += getNum(fmt->tm_min);
		tt += '-';
		tt += getNum(fmt->tm_sec);
		tt += '-';
		tt += AI_name[0].TeamName;
		tt += '-';
		tt += AI_name[1].TeamName;
		strcat(tmp_path, tt.c_str());
		_mkdir(tmp_path);
		//system(tmp_path);
	}
	
	char result_path[100] = "../replay/";
	char map_path[100] = "../replay/";
	char goods_path[100] = "../replay/";
	char order_path[100] = "../replay/";
	char dead_path[100] = "../replay/";
	char error_path[100] = "../replay/";
	strcat(result_path, tt.c_str());
	strcat(map_path, tt.c_str());
	strcat(goods_path, tt.c_str());
	strcat(order_path, tt.c_str());
	strcat(dead_path, tt.c_str());
	strcat(error_path, tt.c_str());
	strcat(result_path, "/result.bmp");
	strcat(map_path, "/map.dat");
	strcat(goods_path, "/goods.dat");
	strcat(order_path, "/order.dat");
	strcat(dead_path, "/dead.dat");
	strcat(error_path, "/error.dat");
	//截图
	hge->System_Snapshot(result_path);
	//地图及玩家信息
	freopen(map_path, "w", stdout);
	for(int j = 0; j < 17; ++j)
	{
		for(int i = 0; i < 22; ++i)
			printf("%d ", map_info.m[j][i]);
		puts("");
	}
	printf("%s\n", AI_name[0].TeamName);
	printf("%s\n", AI_name[0].my1Name);
	printf("%s\n", AI_name[0].my2Name);
	printf("%s\n", AI_name[1].TeamName);
	printf("%s\n", AI_name[1].my1Name);
	printf("%s\n", AI_name[1].my2Name);
	//物品信息
	freopen(goods_path, "w", stdout);
	printf("%d\n", num_info.goods_info_num);
	for(int i = 0; i < num_info.goods_info_num; ++i)
	{
		for(int j = 0; j < 30; ++j)
		{
			printf("%d ",(int)goods_info[i].g[j].eat);
			printf("%d ",goods_info[i].g[j].first);
			printf("%d ",(int)goods_info[i].g[j].free);
			printf("%d ",goods_info[i].g[j].ID);
			printf("%d ",goods_info[i].g[j].id);
			printf("%d ",goods_info[i].g[j].r_t);
			printf("%d ",(int)goods_info[i].g[j].skill);
			printf("%d ",goods_info[i].g[j].t);
			printf("%d ",(int)goods_info[i].g[j].vis);
			printf("%d ",goods_info[i].g[j].x);
			printf("%d ",goods_info[i].g[j].xx);
			printf("%d ",goods_info[i].g[j].y);
			printf("%d ",goods_info[i].g[j].yy);
			puts("");
		}
		for(int j = 0; j < 30; ++j)
			printf("%d ", goods_info[i].g1[j]);
		puts("");
		for(int j = 0; j < 30; ++j)
			printf("%d ", goods_info[i].g2[j]);
		puts("");
		for(int j = 0; j < 30; ++j)
			printf("%d ", goods_info[i].g3[j]);
		puts("");
		for(int j = 0; j < 30; ++j)
			printf("%d ", goods_info[i].g4[j]);
		puts("");
		printf("%d ", goods_info[i].ng1);
		printf("%d ", goods_info[i].ng2);
		printf("%d ", goods_info[i].ng3);
		printf("%d ", goods_info[i].ng4);
		printf("%d ", goods_info[i].t);
		puts("");
	}
	//死亡信息
	freopen(dead_path, "w", stdout);
	printf("%d\n",num_info.dead_info_num);
	for(int i = 0; i < num_info.dead_info_num; ++i)
	{
		printf("%d ", dead_info[i].t);
		printf("%d ", dead_info[i].who);
		printf("%d ", dead_info[i].x);
		printf("%d ", dead_info[i].y);
		puts("");
	}
	//指令信息
	freopen(order_path, "w", stdout);
	for(int i = 0; i <= 10000; ++i)
	{
		printf("%d ",order_info[i].who);
		printf("%d ",order_info[i].t);
		printf("%d ",order_info[i].o.act);
		puts("");
		if(order_info[i].o.act >= 5 && order_info[i].o.act <= 8)
		{
			for(int j = 0; j < 30; ++j)
				printf("%d ",order_info[i].o.dir[j]);
			puts("");
		}
	}
	freopen(error_path,"w",stdout);
}
void Replay_init()
{
	char map_path[] = "../replay/map.dat";
	char goods_path[] = "../replay/goods.dat";
	char order_path[] = "../replay/order.dat";
	char dead_path[] = "../replay/dead.dat";
	char error_path[] = "../replay/error.dat";
	//地图及玩家信息
	freopen(map_path, "r", stdin);
	for(int j = 0; j < 17; ++j)
		for(int i = 0; i < 22; ++i)
			scanf("%d", &map_info.m[j][i]);
	scanf("%s", map_info.pn[0].TeamName);
	scanf("%s", map_info.pn[0].my1Name);
	scanf("%s", map_info.pn[0].my2Name);
	scanf("%s", map_info.pn[1].TeamName);
	scanf("%s", map_info.pn[1].my1Name);
	scanf("%s", map_info.pn[1].my2Name);
	//物品信息
	freopen(goods_path, "r", stdin);
	scanf("%d", &num_info.goods_info_num);
	for(int i = 0; i < num_info.goods_info_num; ++i)
	{
		for(int j = 0; j < 30; ++j)
		{
			int tmp;
			scanf("%d ",&tmp);
			goods_info[i].g[j].eat = (bool)tmp;
			scanf("%d ",&goods_info[i].g[j].first);
			scanf("%d ",&tmp);
			goods_info[i].g[j].free = (bool)tmp;
			scanf("%d ",&goods_info[i].g[j].ID);
			scanf("%d ",&goods_info[i].g[j].id);
			scanf("%d ",&goods_info[i].g[j].r_t);
			scanf("%d ",&tmp);
			goods_info[i].g[j].skill = (bool)tmp;
			scanf("%d ",&goods_info[i].g[j].t);
			scanf("%d ",&tmp);
			goods_info[i].g[j].vis = (bool)tmp;
			scanf("%d ",&goods_info[i].g[j].x);
			scanf("%d ",&goods_info[i].g[j].xx);
			scanf("%d ",&goods_info[i].g[j].y);
			scanf("%d ",&goods_info[i].g[j].yy);
		}
		for(int j = 0; j < 30; ++j)
			scanf("%d ", &goods_info[i].g1[j]);
		for(int j = 0; j < 30; ++j)
			scanf("%d ", &goods_info[i].g2[j]);
		for(int j = 0; j < 30; ++j)
			scanf("%d ", &goods_info[i].g3[j]);
		for(int j = 0; j < 30; ++j)
			scanf("%d ", &goods_info[i].g4[j]);
		scanf("%d ", &goods_info[i].ng1);
		scanf("%d ", &goods_info[i].ng2);
		scanf("%d ", &goods_info[i].ng3);
		scanf("%d ", &goods_info[i].ng4);
		scanf("%d ", &goods_info[i].t);
	}
	num_info.goods_info_num = 0;
	//死亡信息
	freopen(dead_path, "r", stdin);
	scanf("%d", &num_info.dead_info_num);
	for(int i = 0; i < num_info.dead_info_num; ++i)
	{
		scanf("%d ", &dead_info[i].t);
		scanf("%d ", &dead_info[i].who);
		scanf("%d ", &dead_info[i].x);
		scanf("%d ", &dead_info[i].y);
	}
	num_info.dead_info_num = 0;
	//指令信息
	freopen(order_path, "r", stdin);
	for(int i = 0; i <= 10000; ++i)
	{
		scanf("%d ", &order_info[i].who);
		scanf("%d ", &order_info[i].t);
		scanf("%d ", &order_info[i].o.act);
		if(order_info[i].o.act >= 5 && order_info[i].o.act <= 8)
		{
			for(int j = 0; j < 30; ++j)
				scanf("%d ", &order_info[i].o.dir[j]);
		}
	}
	freopen(error_path,"r",stdin);
}

//场景~准备场景 PREPARE
int pre_round = 0;
int act_id_one;
int act_id_two;
Sound snd2("../sound/pre.ogg");
Background bag2("../background/pre.png", 2, true); //0-1表示场景扭曲
Background t_logo("../background/turing_logo.png", 2, true); //0-1表示场景扭曲
bool prepare_Initiate()
{
	srand(time(0));
	atb.AutoBlack_Initiate();
	snd2.Sound_Initiate();
	bag2.Background_Initiate();
	bag2.Set_Fnt(false);
	t_logo.Background_Initiate();
	t_logo.Set_Fnt(false);
	fnt3 = new GfxFont("楷体", 32, true, false, true);
	fnt3->SetColor(0xFFFFFFFF);
	return true;
}
bool prepare_FrameFunc()
{
	if(fntTime > 0)
		--fntTime;
	//背景明暗变幻
	bool yes = atb.AutoBlack_Frame();
	if(yes && pre_round == 0)
	{
		atb.reSet(150, 2);
		pre_round = 1;
	}
	else if(yes && pre_round == 1)
	{
		atb.reSet(150, 1);
		pre_round = 2;
	}
	else if(yes && pre_round == 2)
	{
		atb.reSet(150, 2);
		pre_round = 3;
	}
	else if(yes && pre_round == 3)
		return true;
	if(bgsound == false)
	{
		snd2.Sound_Frame();
		bgsound = true;
	}
	bag2.Background_Frame();
	t_logo.Background_Frame();
	return false;
}
bool prepare_RenderFunc()
{
	if(pre_round == 0 || pre_round == 1)
		t_logo.Background_Render();
	else if(pre_round == 2 || pre_round == 3)
	{
		bag2.Background_Render();
		fnt3->Print(0xffffff00, 0xfffff4500 ,330, 400, "%s\n", "朱艺楠 献礼");
	}
	if(fntTime > 0)
		snd2.Sound_Render();
	atb.AutoBlack_Render();
	return false;
}
void prepare_Release()
{
	if(fnt3)
		delete fnt3;
	t_logo.Background_Release();
	snd2.Sound_Release();
	bag2.Background_Release();
	atb.AutoBlack_Release();
	return;
}

//场景~菜单资源 MENU
Cursor cur("../cursor/mouse.png","../psi/trail.psi");
char music_path[] = "../sound/bg (1).ogg";
Sound snd(music_path);
Background bag("../background/1.jpg", 2, false);
Ren ren = Ren("../character/fire/fire.png","../character/wind/wind.png","../character/rock/rock.png","../character/water/water.png");
int boxindex;
bool show_ren;
box *box1;
hgeGUI *gui;
int id = 0;
hgeGUIMenuItem *Item1;
hgeGUIMenuItem *Item2;
hgeGUIMenuItem *Item3;
bool menu_Initiate()
{
	replay = false;
	show_ren = false;
	boxindex = 0;
	atb.AutoBlack_Initiate();
	atb.reSet(200, 1);
	int num = hge->Random_Int(1, 4);
	music_path[strlen(music_path) - 6] = num + '0';
	snd.Sound_setPath(music_path);
	snd.Sound_Initiate();
	bag.Background_Initiate();
	bag.Set_Fnt(false);
	cur.Cursor_Initiate();
	ren.Ren_Initiate();
	//菜单初始化
	Item1 = new hgeGUIMenuItem(1, 600, 340, 0, 0, 0, 0, 0, "../menuitem/1_1.png", "../menuitem/1_1.png");
	Item2 = new hgeGUIMenuItem(2, 600, 400, 0, 0, 0, 0, 0, "../menuitem/1_2.png", "../menuitem/1_2.png");
	Item3 = new hgeGUIMenuItem(3, 600, 460, 0, 0, 0, 0, 0, "../menuitem/1_3.png", "../menuitem/1_3.png");
	Item1->hgeGUIMenuItem_Initiate();
	Item2->hgeGUIMenuItem_Initiate();
	Item3->hgeGUIMenuItem_Initiate();
	//菜单装载
	gui = new hgeGUI();
	gui->AddCtrl(Item1);
	gui->AddCtrl(Item2);
	gui->AddCtrl(Item3);
	gui->SetFocus(0);
	gui->Enter();
	return true;
}
bool menu_FrameFunc()
{
	if(fntTime > 0)
		--fntTime;
	atb.AutoBlack_Frame();
	switch(hge->Input_GetKey())
	{
		case HGEK_EQUALS:
			if(show_ren)
				show_ren = false;
			else
			{
				act_id_one = 1;
				act_id_two = 2;
				show_ren = true;
			}
		break;
	}
	if(boxindex == 0 && show_ren)
		ren.Ren_Frame();
	switch(hge->Input_GetKey())
	{
		case HGEK_LBUTTON:
			switch(id)
			{
				case 1:
					//进入准备画面
					replay = true;
					hge->System_Snapshot("backrock.bmp");
					Release();
					//资源标记
					//hge->Release();
					//hge = hgeCreate(HGE_VERSION);
					//hge->System_Initiate();
					//
					++model_index;
					bgsound = false;
					changesign = true;
					Initiate();
					return false;
					break;
					/*
					hge->System_Snapshot("backrock.bmp");
					Release();
					//资源标记
					hge->Release();
					hge = hgeCreate(HGE_VERSION);
					hge->System_Initiate();
					//
					++model_index;
					bgsound = false;
					changesign = true;
					Initiate();
					return false;
					break;
					//*/
				case 2:
					//菜单框
					if(boxindex == 0)
					{
						box1 = new box("");
						box1->box_Initiate();
						boxindex = 1;
						//背景变暗
						atb.setStop(0xcf);
						gui->EnableCtrl(1, false);
						gui->EnableCtrl(2, false);
						gui->EnableCtrl(3, false);
					}
					break;
				case 3:
					//结束
					return true;
					break;
			}
			break;
		case HGEK_1:
			//进入准备画面
			replay = true;
			hge->System_Snapshot("backrock.bmp");
			Release();
			//资源标记
			//hge->Release();
			//hge = hgeCreate(HGE_VERSION);
			//hge->System_Initiate();
			//
			++model_index;
			bgsound = false;
			changesign = true;
			Initiate();
			return false;
			break;
			/*
			if(boxindex == 0)
			{
				hge->System_Snapshot("backrock.bmp");
				Release();
				//资源标记
				hge->Release();
				hge = hgeCreate(HGE_VERSION);
				hge->System_Initiate();
				//
				++model_index;
				bgsound = false;
				changesign = true;
				Initiate();
				return false;
			}
			break;
			//*/
		case HGEK_2:
			//菜单框
			if(boxindex == 0)
			{
				box1 = new box("");
				box1->box_Initiate();
				boxindex = 1;
				//背景变暗
				atb.setStop(0xcf);
				gui->EnableCtrl(1, false);
				gui->EnableCtrl(2, false);
				gui->EnableCtrl(3, false);
			}
			break;
		case HGEK_3:
			//结束
			if(boxindex == 0)
				return true;
			break;
		case HGEK_CTRL:
			//用于解除菜单框
			if(boxindex != 0)
			{
				gui->EnableCtrl(1, true);
				gui->EnableCtrl(2, true);
				gui->EnableCtrl(3, true);
			}
			switch(boxindex)
			{
				case 1:
					box1->box_Release();
					delete box1;
					boxindex = 0;
					break;
			}
			break;
	}
	if(bgsound == false)
	{
		snd.Sound_Frame();
		bgsound = true;
	}
	bag.Background_Frame();
	cur.Cursor_Frame();
	//按钮检测
	gui->Update(hge->Timer_GetDelta());
	id = gui->GetFocus();
	//选项框
	switch(boxindex)
	{
		case 1:
			bool ready = false;
			bool ans = box1->box_Frame(ready);
			//取得路径
			if(ready)
			{
				if(false == box1->getPath(map_path, ai1_path, ai2_path))
					ans = true;
			}
			//取消
			if(ans)
			{
				gui->EnableCtrl(1, true);
				gui->EnableCtrl(2, true);
				gui->EnableCtrl(3, true);
				box1->box_Release();
				delete box1;
				boxindex = 0;
				break;
			}
			//ai确认载入
			if(ready && (ans == false))
			{
				//错误检测
				hge->System_Snapshot("backrock.bmp");
				Release();
				//资源标记
				//hge->Release();
				//hge = hgeCreate(HGE_VERSION);
				//hge->System_Initiate();
				//
				++model_index;
				bgsound = false;
				changesign = true;
				Initiate();
				return false;
			}
			break;
	}
	return false;
}
bool menu_RenderFunc()
{
	bag.Background_Render();
	gui->Render();
	if(boxindex == 0 && show_ren)
		ren.Ren_Render();
	switch(boxindex)
	{
		case 1:
			atb.AutoBlack_Render();
			box1->box_Render();
			break;
	}
	if(fntTime > 0)
		snd.Sound_Render();
	cur.Cursor_Render();
	if(atb.getStop() == false)
		atb.AutoBlack_Render();
	return false;
}
void menu_Release()
{
	atb.AutoBlack_Release();
	snd.Sound_Release();
	bag.Background_Release();
	ren.Ren_Release();
	cur.Cursor_Release();
	//卸载GUI
	gui->Leave();
	Item1->hgeGUIMenuItem_Release();
	Item2->hgeGUIMenuItem_Release();
	Item3->hgeGUIMenuItem_Release();
	delete gui;
	return;
}

//场景~人物场景 TEST
float atc_num[2];
float atc_tot[2];
int act_id;
int show_cnt = 0;
hgeFont *showFnt;
hgeFont *winFnt;
Timer timer; //倒计时器
bool clear_time;
Effect *big_skill_one_snd;
Effect *big_skill_two_snd;
Effect *big_skill_three_snd;
Effect *big_skill_four_snd;
Effect *be_killed;
Effect *be_killed_ha;
Star star[10] =
{
	Star(170, 150),
	Star(250, 140),
	Star(380, 230),
	Star(310, 180),
	Star(450, 130),
	Star(320, 360),
	Star(220, 430),
	Star(340, 150),
	Star(430, 320),
	Star(490, 360)
};
Stars bgstar; //星空
Background boder("../background/border.png", 2, true);
Background grass("../background/grass.png", 2, true);
Background rock_up("../background/rock_up.png", 2, true);
Background rock_down("../background/rock_down.png", 2, true);
Background Light("../fightmod/light.png",0, true);
Sound snd3("../sound/bootup.ogg");
char actpath[4][10] =
{
	"fire",
	"wind",
	"rock",
	"water"
};
Character act[4] =
{
	Character(actpath[0],"../psi/a.psi",7, 7),
	Character(actpath[1],"../psi/a.psi",14, 7),
	Character(actpath[2],"../psi/a.psi",7, 11),
	Character(actpath[3],"../psi/a.psi",14, 11)
};
char skillpath[4][30] =
{
	"../psi/fire.psi",
	"../psi/wind.psi",
	"../psi/rock.psi",
	"../psi/water.psi"
};
//人物指令
int actKey[4][4] =
{
	{HGEK_W, HGEK_S, HGEK_A, HGEK_D},
	{HGEK_T, HGEK_G, HGEK_F, HGEK_H},
	{HGEK_I, HGEK_K, HGEK_J, HGEK_L},
	{HGEK_UP, HGEK_DOWN, HGEK_LEFT, HGEK_RIGHT}
};
bool god_mode[2];
int skillKey[4] = {HGEK_Q, HGEK_R, HGEK_U, HGEK_SPACE};
//开启超级garden
bool isSuper_Garden = false;
//技能
Skill *skill[4] = {0, 0, 0, 0};
//AI信息刷新
void setInfo() 
{
	user[0]->t = user[1]->t = run_time;
	user[0]->FPS = user[1]->FPS = hge->Timer_GetFPS();
	user[0]->id = user[1]->id = act_id;
	//0号和2号
	if(act_id == 0 || act_id == 2)
	{
		for(int j = 0; j < boderY; ++j)
			for(int i = 0; i < boderX; ++i)
				user[0]->mapInfo[j][i] = garden.getInfo(i, j);
		//谁
		user[0]->who = (act_id == 0) ? (0) : (1);
		//坐标
		user[0]->x[0] = act[0].cx;
		user[0]->y[0] = act[0].cy;
		user[0]->x[1] = act[2].cx;
		user[0]->y[1] = act[2].cy;
		//冷却时间
		user[0]->dead_cd[0] = act[0].dead_cd;
		user[0]->dead_cd[1] = act[2].dead_cd;
		user[0]->s_skill_cd[0] = act[0].small_skill_cd;
		user[0]->s_skill_cd[1] = act[2].small_skill_cd;
		user[0]->b_skill_cd[0] = big_skill_cd_one;
		user[0]->b_skill_cd[1] = big_skill_cd_three;
		//受幻术影响
		if(act[0].state == 1 && act[2].state == 1)
			return;
		//物品处理
		int map_goods_num = 0;
		for(int i = 0; i < 30; ++i)
			if(garden.goods[i].eat == false && start_time/100 - garden.goods[i].t + 10 >= run_time/100)
			{
				user[0]->map_goods_x[map_goods_num] = garden.goods[i].x;
				user[0]->map_goods_y[map_goods_num] = garden.goods[i].y;
				user[0]->map_goods_time[map_goods_num] = start_time/100 - garden.goods[i].t;
				user[0]->map_goods_type[map_goods_num] = garden.goods[i].id;
				++map_goods_num;
			}
		user[0]->map_goods = map_goods_num;
		//属性
		user[0]->magic_len[0] = act[0].len;
		user[0]->magic_len[1] = act[2].len;
		user[0]->dir[0] = act[0].dir;
		user[0]->dir[1] = act[2].dir;
		user[0]->power[0] = act[0].life;
		user[0]->power[1] = act[2].life;
		user[0]->sorce[0] = act[0].sorce;
		user[0]->sorce[1] = act[2].sorce;
		user[0]->goods_num[0] = act[0].num_g;
		user[0]->goods_num[1] = act[2].num_g;
		for(int i = 0; i < act[0].num_g; ++i)
			user[0]->goods_tpye[0][i] = garden.goods[act[0].goods[i]].id;
		for(int i = 0; i < act[2].num_g; ++i)
			user[0]->goods_tpye[1][i] = garden.goods[act[2].goods[i]].id;
		int skill_len = 0;
		if(skill[0])
		{
			skill_len = skill[0]->s + 1;
			if(god_mode[0])
				skill_len = skill[0]->len;
		}
		user[0]->magic_num[0] = skill_len;
		for(int i = 0; i < skill_len; ++i)
		{
			user[0]->magic_x[0][i] = skill[0]->cx[i];
			user[0]->magic_y[0][i] = skill[0]->cy[i];
		}
		skill_len = 0;
		if(skill[2])
		{
			skill_len = skill[2]->s + 1;
			if(god_mode[0])
				skill_len = skill[2]->len;
		}
		user[0]->magic_num[1] = skill_len;
		for(int i = 0; i < skill_len; ++i)
		{
			user[0]->magic_x[1][i] = skill[2]->cx[i];
			user[0]->magic_y[1][i] = skill[2]->cy[i];
		}
		//坐标
		user[0]->ex[0] = act[1].cx;
		user[0]->ey[0] = act[1].cy;
		user[0]->ex[1] = act[3].cx;
		user[0]->ey[1] = act[3].cy;
		if(show_ren)
		{
			user[0]->ex[0] = act[3].cx;
			user[0]->ey[0] = act[3].cy;
		}
		//
		if(!act[1].ok())
		{
			user[0]->ex[0] = 0;
			user[0]->ey[0] = 0;
		}
		if(!act[3].ok())
		{
			user[0]->ex[1] = 0;
			user[0]->ey[1] = 0;
		}
		//
		//冷却时间
		user[0]->edead_cd[0] = act[1].dead_cd;
		user[0]->edead_cd[1] = act[3].dead_cd;
		user[0]->es_skill_cd[0] = act[1].small_skill_cd;
		user[0]->es_skill_cd[1] = act[3].small_skill_cd;
		user[0]->eb_skill_cd[0] = big_skill_cd_two;
		user[0]->eb_skill_cd[1] = big_skill_cd_four;
		//属性
		user[0]->edir[0] = act[1].dir;
		user[0]->edir[1] = act[3].dir;
		user[0]->epower[0] = act[1].life;
		user[0]->epower[1] = act[3].life;
		user[0]->esorce[0] = act[1].sorce;
		user[0]->esorce[1] = act[3].sorce;
		user[0]->egoods_num[0] = act[1].num_g;
		user[0]->egoods_num[1] = act[3].num_g;
		for(int i = 0; i < act[1].num_g; ++i)
			user[0]->egoods_tpye[0][i] = garden.goods[act[1].goods[i]].id;
		for(int i = 0; i < act[3].num_g; ++i)
			user[0]->egoods_tpye[1][i] = garden.goods[act[3].goods[i]].id;
		skill_len = 0;
		if(skill[1])
		{
			skill_len = skill[1]->s + 1;
			if(god_mode[0])
				skill_len = skill[1]->len;
		}
		user[0]->emagic_num[0] = skill_len;
		for(int i = 0; i < skill_len; ++i)
		{
			user[0]->emagic_x[0][i] = skill[1]->cx[i];
			user[0]->emagic_y[0][i] = skill[1]->cy[i];
		}
		skill_len = 0;
		if(skill[3])
		{
			skill_len = skill[3]->s + 1;
			if(god_mode[0])
				skill_len = skill[3]->len;
		}
		user[0]->emagic_num[1] = skill_len;
		for(int i = 0; i < skill_len; ++i)
		{
			user[0]->emagic_x[1][i] = skill[3]->cx[i];
			user[0]->emagic_y[1][i] = skill[3]->cy[i];
		}
	}
	//1号和3号
	if(act_id == 1 || act_id == 3)
	{
		for(int j = 0; j < boderY; ++j)
			for(int i = 0; i < boderX; ++i)
				user[1]->mapInfo[j][i] = garden.getInfo(i, j);
		//谁
		user[1]->who = (act_id == 1) ? (0) : (1);
		//坐标
		user[1]->x[0] = act[1].cx;
		user[1]->y[0] = act[1].cy;
		user[1]->x[1] = act[3].cx;
		user[1]->y[1] = act[3].cy;
		//冷却时间
		user[1]->dead_cd[0] = act[1].dead_cd;
		user[1]->dead_cd[1] = act[3].dead_cd;
		user[1]->s_skill_cd[0] = act[1].small_skill_cd;
		user[1]->s_skill_cd[1] = act[3].small_skill_cd;
		user[1]->b_skill_cd[0] = big_skill_cd_two;
		user[1]->b_skill_cd[1] = big_skill_cd_four;
		//晕眩大技能劫持
		if(act[1].state == 3 && act[3].state == 3)
		{
			user[1]->x[0] = 0;
			user[1]->y[0] =	0;
			user[1]->x[1] = 0;
			user[1]->y[1] = 0;
		}
		//物品处理
		int map_goods_num = 0;
		for(int i = 0; i < 30; ++i)
			if(garden.goods[i].eat == false && start_time/100 - garden.goods[i].t + 10 >= run_time/100)
			{
				user[1]->map_goods_x[map_goods_num] = garden.goods[i].x;
				user[1]->map_goods_y[map_goods_num] = garden.goods[i].y;
				user[1]->map_goods_time[map_goods_num] = start_time/100 - garden.goods[i].t;
				user[1]->map_goods_type[map_goods_num] = garden.goods[i].id;
				++map_goods_num;
			}
		user[1]->map_goods = map_goods_num;
		//属性
		user[1]->magic_len[0] = act[1].len;
		user[1]->magic_len[1] = act[3].len;
		user[1]->dir[0] = act[1].dir;
		user[1]->dir[1] = act[3].dir;
		user[1]->power[0] = act[1].life;
		user[1]->power[1] = act[3].life;
		user[1]->sorce[0] = act[1].sorce;
		user[1]->sorce[1] = act[3].sorce;
		user[1]->goods_num[0] = act[1].num_g;
		user[1]->goods_num[1] = act[3].num_g;
		for(int i = 0; i < act[1].num_g; ++i)
			user[1]->goods_tpye[0][i] = garden.goods[act[1].goods[i]].id;
		for(int i = 0; i < act[3].num_g; ++i)
			user[1]->goods_tpye[1][i] = garden.goods[act[3].goods[i]].id;
		int skill_len = 0;
		if(skill[1])
		{
			skill_len = skill[1]->s + 1;
			if(god_mode[1])
				skill_len = skill[1]->len;
		}
		user[1]->magic_num[0] = skill_len;
		for(int i = 0; i < skill_len; ++i)
		{
			user[1]->magic_x[0][i] = skill[1]->cx[i];
			user[1]->magic_y[0][i] = skill[1]->cy[i];
		}
		skill_len = 0;
		if(skill[3])
		{
			skill_len = skill[3]->s + 1;
			if(god_mode[1])
				skill_len = skill[3]->len;
		}
		user[1]->magic_num[1] = skill_len;
		for(int i = 0; i < skill_len; ++i)
		{
			user[1]->magic_x[1][i] = skill[3]->cx[i];
			user[1]->magic_y[1][i] = skill[3]->cy[i];
		}
		//坐标
		user[1]->ex[0] = act[0].cx;
		user[1]->ey[0] = act[0].cy;
		user[1]->ex[1] = act[2].cx;
		user[1]->ey[1] = act[2].cy;
		if(show_ren)
		{
			user[1]->ex[1] = act[0].cx;
			user[1]->ey[1] = act[0].cy;
		}
		if(!act[0].ok())
		{
			user[1]->ex[0] = 0;
			user[1]->ey[0] = 0;
		}
		if(!act[2].ok())
		{
			user[1]->ex[1] = 0;
			user[1]->ey[1] = 0;
		}
		//晕眩大技能劫持
		if(act[1].state == 3 && act[3].state == 3)
		{
			user[1]->ex[0] = 0;
			user[1]->ey[0] = 0;
			user[1]->ex[1] = 0;
			user[1]->ey[1] = 0;
		}
		//冷却时间
		user[1]->edead_cd[0] = act[0].dead_cd;
		user[1]->edead_cd[1] = act[2].dead_cd;
		user[1]->es_skill_cd[0] = act[0].small_skill_cd;
		user[1]->es_skill_cd[1] = act[2].small_skill_cd;
		user[1]->eb_skill_cd[0] = big_skill_cd_one;
		user[1]->eb_skill_cd[1] = big_skill_cd_three;
		//属性
		user[1]->edir[0] = act[0].dir;
		user[1]->edir[1] = act[2].dir;
		user[1]->epower[0] = act[0].life;
		user[1]->epower[1] = act[2].life;
		user[1]->esorce[0] = act[0].sorce;
		user[1]->esorce[1] = act[2].sorce;
		user[1]->egoods_num[0] = act[0].num_g;
		user[1]->egoods_num[1] = act[2].num_g;
		for(int i = 0; i < act[0].num_g; ++i)
			user[1]->egoods_tpye[0][i] = garden.goods[act[0].goods[i]].id;
		for(int i = 0; i < act[2].num_g; ++i)
			user[1]->egoods_tpye[1][i] = garden.goods[act[2].goods[i]].id;
		skill_len = 0;
		if(skill[0])
		{
			skill_len = skill[0]->s + 1;
			if(god_mode[1])
				skill_len = skill[0]->len;
		}
		user[1]->emagic_num[0] = skill_len;
		for(int i = 0; i < skill_len; ++i)
		{
			user[1]->emagic_x[0][i] = skill[0]->cx[i];
			user[1]->emagic_y[0][i] = skill[0]->cy[i];
		}
		skill_len = 0;
		if(skill[2])
		{
			skill_len = skill[2]->s + 1;
			if(god_mode[1])
				skill_len = skill[2]->len;
		}
		user[1]->emagic_num[1] = skill_len;
		for(int i = 0; i < skill_len; ++i)
		{
			user[1]->emagic_x[1][i] = skill[2]->cx[i];
			user[1]->emagic_y[1][i] = skill[2]->cy[i];
		}
	}
}
//超级模式
void test_key()
{
	/*
	if(hge->Input_GetKey() == HGEK_0 && isSuper_Garden == false)
	{
		isSuper_Garden = true;
		garden.Garden_Release();
		garden.Super_Garden_Initiate();
	}
	*/
}
bool dis(int x0, int y0,int len)
{
	for(int i = 0; i < 4; ++i)
	{
		if(abs(act[i].cx - x0) + abs(act[i].cy - y0) < len)
			return true;
	}
	return false;
}
//人物移动
void character_move()
{
	//人物移动外部驱动力
	bool ai[4][4];
	for(int j = 0; j < 4; ++j)
	{
		if(act_id != j)
		{
			for(int i = 0; i < 4; ++i)
				ai[j][i] = false;
			continue;
		}
		for(int i = 0; i < 4; ++i)
		{
			int k = 0;
			if(j == 1 || j == 3)
				k = 1;
			if(AI_order[k].act == i + 1)
				ai[j][i] = true;
			else
				ai[j][i] = false;
		}
	}
	//*人物移动*//
	int move = 20;
	for(int i = 0; i < 4; ++i)
	{
		if(act[i].move <= 0 && act[i].ok())
		{
			//上
			if(hge->Input_GetKeyState(actKey[i][0]) || ai[i][0])
			{
				act[i].dir = UP;
				int tmp = act[i].cy - act[i].speed;
				if(garden.check(act[i].cx, tmp))
					act[i].cy -= act[i].speed;
				act[i].move = move;

			}
			//下
			if(hge->Input_GetKeyState(actKey[i][1]) || ai[i][1])
			{
				act[i].dir = DOWN;
				int tmp = act[i].cy + act[i].speed;
				if(garden.check(act[i].cx, tmp))
					act[i].cy += act[i].speed;
				act[i].move = move;
			}
			//左
			if(hge->Input_GetKeyState(actKey[i][2]) || ai[i][2])
			{
				act[i].dir = LEFT;
				int tmp = act[i].cx - act[i].speed;
				if(garden.check(tmp, act[i].cy))
					act[i].cx -= act[i].speed;
				act[i].move = move;
			}
			//右
			if(hge->Input_GetKeyState(actKey[i][3]) || ai[i][3])
			{
				act[i].dir = RIGHT;
				int tmp = act[i].cx + act[i].speed;
				if(garden.check(tmp, act[i].cy))
					act[i].cx += act[i].speed;
				act[i].move = move;
			}
			act[i].dx = garden.getX(act[i].cx, act[i].cy);
			act[i].dy = garden.getY(act[i].cx, act[i].cy);
			act[i].sx = (act[i].dx - act[i].x) / move;
			act[i].sy = (act[i].dy - act[i].y) / move;
			act[i].size = garden.getZ(act[i].cx, act[i].cy);
		}
		act[i].Character_Frame();
	}
}
//计分规则
bool roles(int x, int y)
{
	return x != y;
}
//边界判断
bool isIn(int x, int y)
{
	return (x >= 0 && x < boderX && y >= 0 && y < boderY);
}
//安全判断
bool IsSafe(int x, int y)
{
	for(int i = 0; i < 4; ++i)
		if(skill[i])
			for(int j = 0; j < skill[i]->len; ++j)
				if(skill[i]->cx[j] == x && skill[i]->cy[j] == y)
					return false;
	return true;
}
//复活
void Come_Back(int a_id)
{
	int rand_x = rand() % boderX;
	int rand_y = rand() % boderY;
	int len = 0;
	int num = 0;
	for(int i = 0; i < 4; ++i)
		len = max(len, act[i].len + 1);
	while(garden.getInfo(rand_x, rand_y) != LAND || !IsSafe(rand_x, rand_y) || dis(rand_x, rand_y, len))
	{
		++num;
		rand_x = rand() % boderX;
		rand_y = rand() % boderY;
		if(num % 100 == 0)
			--len;
	}
	act[a_id].setPos(rand_x, rand_y);
	act[a_id].x = garden.getX(rand_x, rand_y);
	act[a_id].y = garden.getY(rand_x, rand_y);
	//信息记录
	if(replay == false)
	{
		//坐标记录
		dead_info[num_info.dead_info_num].t = run_time;
		dead_info[num_info.dead_info_num].who = a_id;
		dead_info[num_info.dead_info_num].x = act[a_id].cx;
		dead_info[num_info.dead_info_num].y = act[a_id].cy;
		++num_info.dead_info_num;
	}
	else
	{
		//坐标记录
		act[a_id].setPos(dead_info[num_info.dead_info_num].x, dead_info[num_info.dead_info_num].y);
		act[a_id].x = garden.getX(act[a_id].cx, act[a_id].cy);
		act[a_id].y = garden.getY(act[a_id].cx, act[a_id].cy);
		++num_info.dead_info_num;
	}
	//动画效果
	int j = 15 - act_id;
	if(orz[j] == 0)
	{
		char tmp[] = "../orz/so/so";
		float xx = act[a_id].x;
		float yy = act[a_id].y;
		orz[j] = new ORZ(7 ,xx, yy, 50, 50, 8, tmp, tmp);
		orz[j]->ORZ_Initiate();
	}
}
//技能控制
void skill_act()
{
	//触发初始化//注意这里单打模式的特殊处理
	for(int i = 0; i < 4; ++i)
	{
		if(show_ren)
		{
			if(i == act_id_one || i == act_id_two)
				continue;
			if(i == 0 || i == 3)
			{
				if(skill[i] == 0 && act[i].life >= 10 && act[i].ok())
				{
					for(int j = 0; j < 4; ++j)
					{
						if(skill[i] == 0 && hge->Input_GetKeyState(actKey[(i&1)+1][j]))
						{
							skill[i] = new Skill(act[i].cx, act[i].cy, STOP, skillpath[i], act[i].len);
							skill[i]->Skill_Initiate();
							skill[i]->who = i;

							atc_tot[i&1]+=1;

							act[i].life -= 10; //技能消耗
							for(int k = 0; k < 30; ++k)
								AI_order[i&1].dir[k] = STOP;
							skill[i]->setPath(AI_order[i&1].dir);
						}
					}
				}
				else if(skill[i])
				{
					for(int j = 0; j < 4; ++j)
					{
						if(hge->Input_GetKeyState(actKey[(i&1)+1][j]))
						{
							switch(j+1)
							{
								case UP:
									if(isIn(skill[i]->cx[skill[i]->s], skill[i]->cy[skill[i]->s] - 1)
										&& garden.getInfo(skill[i]->cx[skill[i]->s], skill[i]->cy[skill[i]->s] - 1) != TREE)
									{
										skill[i]->cx[skill[i]->s+1] = skill[i]->cx[skill[i]->s];
										skill[i]->cy[skill[i]->s+1] = skill[i]->cy[skill[i]->s] - 1;
									}
									else
									{
										skill[i]->cx[skill[i]->s+1] = skill[i]->cx[skill[i]->s];
										skill[i]->cy[skill[i]->s+1] = skill[i]->cy[skill[i]->s];
									}
									break;
								case DOWN:
									if(isIn(skill[i]->cx[skill[i]->s], skill[i]->cy[skill[i]->s] + 1)
										&& garden.getInfo(skill[i]->cx[skill[i]->s], skill[i]->cy[skill[i]->s] + 1) != TREE)
									{
										skill[i]->cx[skill[i]->s+1] = skill[i]->cx[skill[i]->s];
										skill[i]->cy[skill[i]->s+1] = skill[i]->cy[skill[i]->s] + 1;
									}
									else
									{
										skill[i]->cx[skill[i]->s+1] = skill[i]->cx[skill[i]->s];
										skill[i]->cy[skill[i]->s+1] = skill[i]->cy[skill[i]->s];
									}
									break;
								case LEFT:
									if(isIn(skill[i]->cx[skill[i]->s] - 1, skill[i]->cy[skill[i]->s])
										&& garden.getInfo(skill[i]->cx[skill[i]->s] - 1, skill[i]->cy[skill[i]->s]) != TREE)
									{
										skill[i]->cx[skill[i]->s+1] = skill[i]->cx[skill[i]->s] - 1;
										skill[i]->cy[skill[i]->s+1] = skill[i]->cy[skill[i]->s];
									}
									else
									{
										skill[i]->cx[skill[i]->s+1] = skill[i]->cx[skill[i]->s];
										skill[i]->cy[skill[i]->s+1] = skill[i]->cy[skill[i]->s];
									}
									break;
								case RIGHT:
									if(isIn(skill[i]->cx[skill[i]->s] + 1, skill[i]->cy[skill[i]->s])
										&& garden.getInfo(skill[i]->cx[skill[i]->s] + 1, skill[i]->cy[skill[i]->s]) != TREE)
									{
										skill[i]->cx[skill[i]->s+1] = skill[i]->cx[skill[i]->s] + 1;
										skill[i]->cy[skill[i]->s+1] = skill[i]->cy[skill[i]->s];
									}
									else
									{
										skill[i]->cx[skill[i]->s+1] = skill[i]->cx[skill[i]->s];
										skill[i]->cy[skill[i]->s+1] = skill[i]->cy[skill[i]->s];
									}
									break;
								default:
									skill[i]->cx[skill[i]->s+1] = skill[i]->cx[skill[i]->s];
									skill[i]->cy[skill[i]->s+1] = skill[i]->cy[skill[i]->s];
									break;
							}
							for(int k = skill[i]->s+2; k < 30; ++k)
							{
								skill[i]->cx[k] = skill[i]->cx[skill[i]->s+1];
								skill[i]->cy[k] = skill[i]->cy[skill[i]->s+1];
							}
						}
					}
				}
			}
		}
		if(hge->Input_GetKeyState(skillKey[i]) && skill[i] == 0 && act[i].life >= 10 && act[i].ok())
		{
			skill[i] = new Skill(act[i].cx, act[i].cy, act[i].dir, skillpath[i], act[i].len);
			skill[i]->Skill_Initiate();
			skill[i]->who = i;

			atc_tot[i&1]+=1;

			act[i].life -= 10; //技能消耗
		}
		int j = 0;
		if(act_id == 1 || act_id == 3)
			j = 1;
		if(i == act_id && AI_order[j].act == ATC && skill[i] == 0 && act[i].life >= 10 && act[i].ok())
		{
			skill[i] = new Skill(act[i].cx, act[i].cy, act[i].dir, skillpath[i], act[i].len);
			skill[i]->Skill_Initiate();
			skill[i]->who = i;
			act[i].life -= 10; //技能消耗 

			atc_tot[i&1]+=1;

			skill[i]->setPath(AI_order[j].dir);
		}
	}
	//消失后析构
	for(int i = 0; i < 4; ++i)
	{
		if(skill[i])
		{
			if(skill[i]->Skill_Frame())
			{
				skill[i]->Skill_Release();
				delete skill[i];
				skill[i] = 0;
			}
		}
	}
	//与garden交互
	bool use_be_killed = false;
	bool atc_need_to_add[2] = {false, false};
	for(int i = 0; i < 4; ++i)
	{
		//撞击物体
		if(skill[i])
		{
			if(garden.check_skill(skill[i]->x, skill[i]->y) == false)
			{
				skill[i]->id = 0;
				skill[i]->Skill_Release();
				delete skill[i];
				skill[i] = 0;
			}
		}
		//攻击
		if(skill[i])
		{
			bool ok = false;
			for(int j = 0; j < 4; ++j)
				for(int k = 0; k <= skill[i]->s; ++k)
				{
					if(show_ren)
					{
						if(j == act_id_one || j == act_id_two)
							continue;
					}
					if(skill[i]->move == false && act[j].ok() && act[j].cx == skill[i]->cx[k] && act[j].cy == skill[i]->cy[k] && roles(i, j))
					{
						atc_need_to_add[i&1] = true;

						int add = skill[i]->s*2 + 5;
						if(i == 0 || i == 2)
						{
							if(j == 0 || j == 2)
								add = -10;
						}
						if(i == 1 || i == 3)
						{
							if(j == 1 || j == 3)
								add = -10;
						}
						act[skill[i]->who].sorce += add; //加分
						//掉物
						int size = act[j].num_g;
						int num = (act[j].num_g + 1) / 2;
						bool vis[30] = {0};
						while(num)
						{
							int g_id = rand() % size;
							if(vis[g_id] == false)
							{
								vis[g_id] = true;
								--num;
								switch(garden.goods[act[j].goods[g_id]].id)
								{
									case 0:
										act[j].len -= 2;
										break;
									case 1:
									case 2:
										break;
								}
							}
						}
						//分配物品
						bool goods_time[200] = {0};
						for(int p = 0; p < 30; ++p)
							goods_time[garden.goods[p].t] = 1;
						int goods_var = 0;
						for(int p = 0; p < size; ++p)
						{
							if(vis[p])
							{
								int rand_x = rand() % boderX;
								int rand_y = rand() % boderY;
								while(garden.getInfo(rand_x, rand_y) != LAND)
								{
									rand_x = rand() % boderX;
									rand_y = rand() % boderY;
								}
								garden.goods[act[j].goods[p]].x = rand_x;
								garden.goods[act[j].goods[p]].y = rand_y;
								garden.goods[act[j].goods[p]].eat = false;
								garden.goods[act[j].goods[p]].first = 0;
								garden.goods[act[j].goods[p]].free = false;
								garden.goods[act[j].goods[p]].vis = false;
								do
								{
									++goods_var;
								}while(goods_time[start_time/100 - run_time/100+goods_var]);
								int next_time = start_time/100 - run_time/100 + goods_var;
								garden.goods[act[j].goods[p]].t = next_time;
							}
						}
						//整理背包
						int n_id[30];
						int q = 0;
						for(int p = 0; p < size; ++p)
						{
							if(vis[p] == false)
							{
								n_id[q] = act[j].goods[p];
								++q;
							}
						}
						act[j].num_g = q;
						for(int p = 0; p < q; ++p)
							act[j].goods[p] = n_id[p];
						//打飞
						act[j].death_time = 200;
						ok = true;
						int start_i = 0;
						int end_i = 0;
						switch(j)
						{
							case 0:
								start_i = 0;
								end_i = 7;
								break;
							case 1:
								start_i = 7;
								end_i = 14;
								break;
							case 2:
								start_i = 14;
								end_i = 21;
								break;
							case 3:
								start_i = 21;
								end_i = 28;
								break;
						}
						//动画效果
						for(int ii = start_i; ii < end_i; ++ii)
						{
							if(orz2[ii] == 0)
							{
								orz2[ii] = new ORZ2(act[j].x, act[j].y, act[j].x + hge->Random_Int(-200, 200), act[j].y + hge->Random_Int(-200, 200));
								orz2[ii]->ORZ2_Initiate();
							}
						}
						//音效
						use_be_killed = true;
						//信息记录
						if(replay == false)
						{
							//物品记录
							goods_info[num_info.goods_info_num].t = run_time;
							for(int p = 0; p < 30; ++p)
							{
								goods_info[num_info.goods_info_num].g[p] = garden.goods[p];
								goods_info[num_info.goods_info_num].g1[p] = act[0].goods[p];
								goods_info[num_info.goods_info_num].g2[p] = act[1].goods[p];
								goods_info[num_info.goods_info_num].g3[p] = act[2].goods[p];
								goods_info[num_info.goods_info_num].g4[p] = act[3].goods[p];
							}
							goods_info[num_info.goods_info_num].ng1 = act[0].num_g;
							goods_info[num_info.goods_info_num].ng2 = act[1].num_g;
							goods_info[num_info.goods_info_num].ng3 = act[2].num_g;
							goods_info[num_info.goods_info_num].ng4 = act[3].num_g;
							++num_info.goods_info_num;
						}
						else
						{
							//物品记录
							for(int p = 0; p < 30; ++p)
							{
								garden.goods[p] = goods_info[num_info.goods_info_num].g[p];
								act[0].goods[p] = goods_info[num_info.goods_info_num].g1[p];
								act[1].goods[p] = goods_info[num_info.goods_info_num].g2[p];
								act[2].goods[p] = goods_info[num_info.goods_info_num].g3[p];
								act[3].goods[p] = goods_info[num_info.goods_info_num].g4[p];
							}
							act[0].num_g = goods_info[num_info.goods_info_num].ng1;
							act[1].num_g = goods_info[num_info.goods_info_num].ng2;
							act[2].num_g = goods_info[num_info.goods_info_num].ng3;
							act[3].num_g = goods_info[num_info.goods_info_num].ng4;
							for(int p = 0; p < 4; ++p)
							{
								int m_len = 8;
								for(int q = 0; q < act[p].num_g; ++q)
									if(garden.goods[act[p].goods[q]].id == BLUE)
										m_len += 2;
								act[p].len = m_len;
							}
							++num_info.goods_info_num;
							//坐标记录
						}
					}
				}
			if(ok)
				skill[i]->move = true;
		}
	}
	for(int i = 0; i < 2; ++i)
		if(atc_need_to_add[i])
			atc_num[i]+=1;
	if(use_be_killed)
	{
		char tmp[50] = "../sound/k";
		char hehe[10] = "0.mp3";
		hehe[0] = rand()%7 + 1 + '0';
		strcat(tmp, hehe);
		be_killed_ha->Effect_Release();
		be_killed_ha->Effect_ReSet(tmp);
		be_killed_ha->Effect_Initiate();
		be_killed_ha->Effect_Frame();
	}
}
//物品控制
void goods_change()
{
	//显示光环
	for(int i = 0; i < 30; ++i)
		if(start_time/100 - garden.goods[i].t + 10 >= (run_time+100)/100 && garden.goods[i].vis == false)
		{	
			garden.goods[i].vis = true;
			garden.goods[i].r_t = 0;
		}
	//动画
	for(int i = 0; i < 30; ++i)
		if(start_time/100 - garden.goods[i].t + 1 >= (run_time+100)/100 && garden.goods[i].first == 0)
		{
			garden.goods[i].r_t = 0;
			garden.goods[i].first = 1;
		}
	//动画效果
	garden.Garden_Frame_GOODS();
	//显示
	for(int i = 0; i < 30; ++i)
		if(start_time/100 - garden.goods[i].t >= (run_time+100)/100 && garden.goods[i].free == false)
		{	
			garden.goods[i].first = 2;
			garden.goods[i].free = true;
			garden.goods[i].vis = false;
		}
	//被吃
	for(int i = 0; i < 30; ++i)
		for(int j = 0; j < 4; ++j)
			if(garden.goods[i].free == true && garden.goods[i].eat == false && garden.goods[i].x == act[j].cx && garden.goods[i].y == act[j].cy && act[j].ok())
			{
				garden.goods[i].eat = true;
				act[j].goods[act[j].num_g] = garden.goods[i].ID;
				++act[j].num_g;
				switch(garden.goods[i].id)
				{
					case 0:
						act[j].len += 2;
						break;
					case 1:
						act[j].sorce += 30;
						break;
					case 2:
						act[j].life += 30;
						break;
				}
			}
}
//社交函数
void get_talk(int id)
{
	if(act[id].say_time <= 0 && AI_order[id&1].act >= 11 && AI_order[id&1].act <= 43)
	{
		act[id].say_time = 300;
		act[id].say = AI_order[id&1].act;
	}
}
//说话
void have_fun()
{
	fnt->SetColor(0xffffff00);
	for(int i = 0; i < 4; ++i)
		if(act[i].say_time > 100)
		{
			switch(act[i].say)
			{
				case 11:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "O(∩_∩)O哈哈");
					break;
				case 12:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "o(╯□╰)o囧");
					break;
				case 13:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "o(>﹏<)o不要啊");
					break;
				case 14:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "~(≧▽≦)~啦啦啦");
					break;
				case 15:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "杀气腾腾");
					break;
				case 16:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "你们死定了");
					break;
				case 17:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "吃我一招");
					break;
				case 18:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "纯属巧合");
					break;
				case 19:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "这...");
					break;
				case 20:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "我还是回家种菜去吧");
					break;
				case 21:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "这是个意外");
					break;
				case 22:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "为什么");
					break;
				case 23:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "啊！！！！");
					break;
				case 24:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "这不科学");
					break;
				case 25:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "元芳，你在哪？");
					break;
				case 26:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "坑爹啊");
					break;
				case 27:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "⊙﹏⊙b汗");
					break;
				case 28:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "吓");
					break;
				case 29:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "呜呜，哭");
					break;
				case 30:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "这...");
					break;
				case 31:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "元芳，我来了");
					break;
				case 32:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "动感光波，哔哔");
					break;
				case 33:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "有何颜面见江东父老啊");
					break;
				case 34:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "嘿嘿");
					break;
				case 35:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "赢了~");
					break;
				case 36:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "悲剧了");
					break;
				case 37:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "黑线...");
					break;
				case 38:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "我欲乘风归去");
					break;
				case 39:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "咆哮吧");
					break;
				case 40:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "沸腾吧");
					break;
				case 41:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "等死吧");
					break;
				case 42:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "呵呵");
					break;
				case 43:
					fnt->Print(act[i].x - 30, act[i].y - 50, "%s", "Bingo");
					break;
			}
		}
}
//信息显示
void info_show()
{
	/*
	winFnt->SetColor(0xffffff00);
	showFnt->SetColor(0xffffff00);
	showFnt->printf(10, 10, HGETEXT_LEFT, "Time:%d\n", run_time);
	if(run_time <= 0)
	{
		if(act[0].sorce + act[2].sorce > act[1].sorce + act[3].sorce)
			winFnt->SetColor(0xffff0000);
		if(act[0].sorce + act[2].sorce < act[1].sorce + act[3].sorce)
			showFnt->SetColor(0xffff0000);
	}
	winFnt->printf(665, 40, HGETEXT_LEFT, "%s\nPower:%d-%d\nScore:%d\nGoods:%d-%d\n", AI_name[0].TeamName, act[0].life, act[2].life, act[0].sorce + act[2].sorce, act[0].num_g, act[2].num_g);
	showFnt->printf(665, 465, HGETEXT_LEFT, "%s\nPower:%d-%d\nScore:%d\nGoods:%d-%d\n", AI_name[1].TeamName, act[1].life, act[3].life, act[1].sorce + act[3].sorce, act[1].num_g, act[3].num_g);
	*/
	fnt->Print(0xffffff00, 0xfffff4500, 10, 10, "时间:%02d.%02d\n", run_time/100, run_time%100);

	//说话控制
	have_fun();

	if(act[0].death_time <= 0)
		fnt->Print(0xffffff00, 0xfffff4500, act[0].x - 30, act[0].y + 30, "A:%s", AI_name[0].my1Name);
	if(!show_ren)
	{
		if(act[2].death_time <= 0)
			fnt->Print(0xffffff00, 0xfffff4500, act[2].x - 30, act[2].y + 30, "A:%s", AI_name[0].my2Name);
		if(act[1].death_time <= 0)
			fnt->Print(0xffffff00, 0xfffff4500, act[1].x - 30, act[1].y + 30, "B:%s", AI_name[1].my1Name);
	}
	if(act[3].death_time <= 0)
		fnt->Print(0xffffff00, 0xfffff4500, act[3].x - 30, act[3].y + 30, "B:%s", AI_name[1].my2Name);
	
	float atc_per[2] = {0};
	for(int i = 0; i < 2; ++i)
	{
		if(atc_tot[i] < 0.9)
			atc_per[i] = 0;
		else
			atc_per[i] = atc_num[i]/atc_tot[i];
	}
	fnt->Print(0xffffff00, 0xfffff4500, 660, 465, "B:%s\n能量:%d-%d\n分数:%d-%.f%%\n物品:%d-%d\n", AI_name[1].TeamName, act[1].life, act[3].life, act[1].sorce + act[3].sorce, atc_per[1]*100.0, act[1].num_g, act[3].num_g);
	fnt->Print(0xffffff00, 0xfffff4500, 660, 40, "A:%s\n能量:%d-%d\n分数:%d-%.f%%\n物品:%d-%d\n", AI_name[0].TeamName, act[0].life, act[2].life, act[0].sorce + act[2].sorce, atc_per[0]*100.0, act[0].num_g, act[2].num_g);
	
	if(run_time <= 0)
	{
		if(act[0].sorce + act[2].sorce > act[1].sorce + act[3].sorce)
		{
			fnt2->Print(0x7fffff00, 0x7ffff4500 ,311 - strlen(AI_name[0].TeamName)*4, 251, "%s\n", AI_name[0].TeamName);
			fnt2->Print(0xffffff00, 0xfffff4500 ,310 - strlen(AI_name[0].TeamName)*4, 250, "%s\n", AI_name[0].TeamName);
		}
		else if(act[0].sorce + act[2].sorce < act[1].sorce + act[3].sorce)
		{
			fnt2->Print(0x7fffff00, 0x7ffff4500 ,311 - strlen(AI_name[1].TeamName)*4, 251, "%s\n", AI_name[1].TeamName);
			fnt2->Print(0xffffff00, 0xfffff4500 ,310 - strlen(AI_name[1].TeamName)*4, 250, "%s\n", AI_name[1].TeamName);
		}
		else
		{
			fnt2->Print(0x7fffff00, 0x7ffff4500 ,311, 251, "%s\n", "平局");
			fnt2->Print(0xffffff00, 0xfffff4500 ,310, 250, "%s\n", "平局");
		}
	}
	/*
	for(int j = 0; j < 17; ++j)
		for(int i = 0; i < 22; ++i)
			fnt->Print(garden.getX(i,j)-10,garden.getY(i,j)+10,"%d",garden.getInfo(i,j));*/
}
//计时
bool changeTime()
{
	if(run_time <= 0)
	{
		run_time = 0;
		return true;
	}
	//大技能冷却时间控制
	if(big_skill_cd_one > 0)
		--big_skill_cd_one;
	if(big_skill_cd_two > 0)
		--big_skill_cd_two;
	if(big_skill_cd_three > 0)
		--big_skill_cd_three;
	if(big_skill_cd_four > 0)
		--big_skill_cd_four;
	for(int i = 0; i < 4; ++i)
	{
		//人物移动控制
		if(act[i].move > 0)
			--act[i].move;
		//技能yidong
		if(skill[i])
			++skill[i]->dt;
		//人物异常状体冷却时间控制
		if(act[i].state_t > 0)
			--act[i].state_t;
		//小技能冷却控制
		if(act[i].small_skill_cd > 0)
			--act[i].small_skill_cd;
		//自杀冷却控制
		if(act[i].dead_cd > 0)
			--act[i].dead_cd;
		//说话时间控制
		if(act[i].say_time > 0)
		{
			--act[i].say_time;
			if(act[i].say_time <= 0)
				act[i].say = 0;
		}
		//死亡时间控制
		if(act[i].death_time > 0)
		{
			--act[i].death_time;
			if(act[i].death_time <= 0)
				Come_Back(i);
		}
	}
	//总时间控制
	--run_time;
	return false;
}
//自杀
bool kill_myself()
{
	int run_id = 0;
	if(act_id == 1 || act_id == 3)
		run_id = 1;
	if((hge->Input_GetKey() == HGEK_9 && act[act_id].dead_cd <= 0 && act[act_id].ok()) || (AI_order[run_id].act == DEAD && act[act_id].dead_cd <= 0 && act[act_id].ok()))
	{
		be_killed->Effect_Frame();
		act[act_id].dead_cd = 1000;
		act[act_id].sorce -= 40;
		act[act_id].death_time = 200;
	}
	return false;
}
//小技能控制
bool small_skill_act()
{
	if(!act[act_id].ok())
		return true;
	int k = 0;
	if(act_id == 1 || act_id == 3)
		k = 1;
	//造墙
	if(hge->Input_GetKey() == HGEK_B || AI_order[k].act == ONE)
	{
		if(act[act_id].small_skill_cd <= 0 && act[act_id].life >= 10)
		{
			int _x = act[act_id].cx;
			int _y = act[act_id].cy;
			switch(AI_order[k].dir[0])
			{
				case UP:
					--_y;
					break;
				case DOWN:
					++_y;
					break;
				case LEFT:
					--_x;
					break;
				case RIGHT:
					++_x;
					break;
				default:
					--_y;
					break;
			}
			if(isIn(_x, _y) && garden.getInfo(_x, _y) == LAND)
			{
				for(int i = 0; i < 4; ++i)
					if(_x == act[i].cx && _y == act[i].cy)
						return true;
				if(orz[act_id] == 0)
				{
					char tmp[] = "../orz/so/so";
					float xx = garden.getX(_x, _y);
					float yy = garden.getY(_x, _y);
					orz[act_id] = new ORZ(7 ,xx, yy, 50, 50, 8, tmp, tmp);
					orz[act_id]->ORZ_Initiate();
				}
				garden.dst[_y][_x] = true;
				garden.setAct(_x, _y, TREE);
				act[act_id].life -= 10;
				act[act_id].small_skill_cd = 1200;
				if(replay == false)
				{
					order_info[run_time].o.dir[2] = -11;
					order_info[run_time].o.dir[3] = _x;
					order_info[run_time].o.dir[4] = _y;
				}
			}
		}
	}
	//拆墙
	if(hge->Input_GetKey() == HGEK_N || AI_order[k].act == TWO)
	{
		if(act[act_id].small_skill_cd <= 0 && act[act_id].life >= 10)
		{
			int _x = act[act_id].cx;
			int _y = act[act_id].cy;
			switch(AI_order[k].dir[0])
			{
				case UP:
					--_y;
					break;
				case DOWN:
					++_y;
					break;
				case LEFT:
					--_x;
					break;
				case RIGHT:
					++_x;
					break;
				default:
					--_y;
					break;
			}
			if(isIn(_x, _y) && garden.getInfo(_x, _y) == TREE)
			{
				if(orz[act_id] == 0)
				{
					char tmp[] = "../orz/dip/dip";
					float xx = garden.getX(_x, _y);
					float yy = garden.getY(_x, _y);
					orz[act_id] = new ORZ(7, xx, yy/2, yy/2, 50, 8, tmp, tmp);
					orz[act_id]->ORZ_Initiate();
				}
				garden.setAct(_x, _y, LAND);
				act[act_id].life -= 10;
				act[act_id].small_skill_cd = 1400;
				if(replay == false)
				{
					order_info[run_time].o.dir[2] = -11;
					order_info[run_time].o.dir[3] = _x;
					order_info[run_time].o.dir[4] = _y;
				}
			}
		}
	}
	//瞬移
	if(hge->Input_GetKey() == HGEK_M || AI_order[k].act == THREE)
	{
		if(act[act_id].small_skill_cd <= 0 && act[act_id].life >= 10)
		{
			int _x = act[act_id].cx;
			int _y = act[act_id].cy;
			switch(AI_order[k].dir[0])
			{
				case UP:
					_y -= AI_order[k].dir[1];
					break;
				case DOWN:
					_y += AI_order[k].dir[1];
					break;
				case LEFT:
					_x -= AI_order[k].dir[1];
					break;
				case RIGHT:
					_x += AI_order[k].dir[1];
					break;
				default:
					_y -= hge->Random_Int(3, 10);
					break;
			}
			if(replay)
			{
				if(order_info[run_time].o.dir[2] == -11)
				{
					_x = order_info[run_time].o.dir[3];
					_y = order_info[run_time].o.dir[4];
					if(orz[act_id] == 0)
					{
						char tmp[] = "../orz/st/st";
						float xx = garden.getX(act[act_id].cx, act[act_id].cy);
						float yy = garden.getY(act[act_id].cx, act[act_id].cy);
						orz[act_id] = new ORZ(7, xx, yy, 50, 50, 8, tmp, tmp);
						orz[act_id]->ORZ_Initiate();
					}
					if(orz[act_id + 8] == 0)
					{
						char tmp[] = "../orz/so/so";
						float xx = garden.getX(_x, _y);
						float yy = garden.getY(_x, _y);
						orz[act_id + 8] = new ORZ(7, xx, yy, 50, 50, 8, tmp, tmp);
						orz[act_id + 8]->ORZ_Initiate();
					}
					act[act_id].setPos(_x, _y);
					act[act_id].x = garden.getX(_x, _y);
					act[act_id].y = garden.getY(_x, _y);
					act[act_id].life -= 10;
					act[act_id].small_skill_cd = 1700;
				}
			}
			if(isIn(_x, _y) && garden.getInfo(_x, _y) == LAND && replay == false)
			{
				if(orz[act_id] == 0)
				{
					char tmp[] = "../orz/st/st";
					float xx = garden.getX(act[act_id].cx, act[act_id].cy);
					float yy = garden.getY(act[act_id].cx, act[act_id].cy);
					orz[act_id] = new ORZ(7, xx, yy, 50, 50, 8, tmp, tmp);
					orz[act_id]->ORZ_Initiate();
				}
				if(orz[act_id + 8] == 0)
				{
					char tmp[] = "../orz/so/so";
					float xx = garden.getX(_x, _y);
					float yy = garden.getY(_x, _y);
					orz[act_id + 8] = new ORZ(7, xx, yy, 50, 50, 8, tmp, tmp);
					orz[act_id + 8]->ORZ_Initiate();
				}
				act[act_id].setPos(_x, _y);
				act[act_id].x = garden.getX(_x, _y);
				act[act_id].y = garden.getY(_x, _y);
				act[act_id].life -= 10;
				act[act_id].small_skill_cd = 1700;
				if(replay == false)
				{
					order_info[run_time].o.dir[2] = -11;
					order_info[run_time].o.dir[3] = _x;
					order_info[run_time].o.dir[4] = _y;
				}
			}
		}
	}
	return false;
}
//大技能,吸物
void big_skill_act_one()
{
	int act_id = 0;
	//初始化
	if(big_skill_time <= 0)
	{
		if(musicState)
			hge->System_SetState(HGE_STREAMVOLUME, 30);
		big_skill_one_snd = new Effect("../sound/one.ogg");
		big_skill_one_snd->Effect_Initiate();
		big_skill_one_snd->Effect_Frame();
		act[act_id].state = 0;
		for(int i = 0; i < 30; ++i)
		{
			if(garden.goods[i].free == true && garden.goods[i].eat == false)
			{
				garden.goods[i].xx = garden.getX(garden.goods[i].x, garden.goods[i].y);
				garden.goods[i].yy = garden.getY(garden.goods[i].x, garden.goods[i].y);
				garden.goods[i].skill = true;
			}
		}
	}
	//运行
	++big_skill_time;
	for(int i = 0; i < 30; ++i)
	{
		if(garden.goods[i].skill)
		{
			float dx = act[act_id].x - garden.getX(garden.goods[i].x, garden.goods[i].y);
			float dy = act[act_id].y - garden.getY(garden.goods[i].x, garden.goods[i].y);
			garden.goods[i].xx = garden.getX(garden.goods[i].x, garden.goods[i].y) + dx * big_skill_time / 200.0;
			garden.goods[i].yy = garden.getY(garden.goods[i].x, garden.goods[i].y) + dy * big_skill_time / 200.0;
		}
	}
	//结束
	if(big_skill_time >= 200)
	{
		if(musicState)
			hge->System_SetState(HGE_STREAMVOLUME, 100);
		if(big_skill_one_snd)
		{
			big_skill_one_snd->Effect_Release();
			delete big_skill_one_snd;
			big_skill_one_snd = 0;
		}
		act[act_id].state = -1;
		for(int i = 0; i < 30; ++i)
			if(garden.goods[i].skill)
			{
				garden.goods[i].x = act[act_id].cx;
				garden.goods[i].y = act[act_id].cy;
				garden.goods[i].skill = false;
			}
		big_skill_id = 0;
	}
}
//大技能，扭曲
HTEXTURE hdmtex;
hgeDistortionMesh *hdm;
void big_skill_act_two()
{
	int act_id = 1;
	//初始化
	if(big_skill_time <= 0)
	{
		if(musicState)
			hge->System_SetState(HGE_STREAMVOLUME, 30);
		big_skill_two_snd = new Effect("../sound/two.ogg");
		big_skill_two_snd->Effect_Initiate();
		big_skill_two_snd->Effect_Frame();
		hge->System_Snapshot("skillrock.bmp");
		hdmtex = hge->Texture_Load("skillrock.bmp");
		if(!hdmtex)
			MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		//扭曲
		hdm = new hgeDistortionMesh(32, 32);
		hdm->SetTexture(hdmtex);
		hdm->SetTextureRect(30, 30, 620, 460);
		hdm->SetBlendMode(BLEND_COLORADD | BLEND_ALPHABLEND | BLEND_ZWRITE);
		hdm->Clear(0xff000000);
		hge->Random_Seed(int(hge->Timer_GetDelta()));
	}
	++big_skill_time;
	float h = big_skill_time;
	if(h >= 200)
	{
		h = 400 - big_skill_time;
	}
	h /= 15.0;
	for(int i = 0; i < 32; ++i)
		for(int j = 0; j < 32; ++j)
		{
			float x0 = sinf(big_skill_time/5.0+i+j)*h;
			float y0 = cosf(big_skill_time/5.0+i+j)*h;
			x0 *= sqrt((5 + sqrt(16.0*16.0 + 16.0*16.0))/sqrt(1 + (i-16.0)*(i-16.0) + (j-16.0)*(j-16.0))) - 1.0;
			y0 *= sqrt((5 + sqrt(16.0*16.0 + 16.0*16.0))/sqrt(1 + (i-16.0)*(i-16.0) + (j-16.0)*(j-16.0))) - 1.0;
			x0 *= 0.7;
			y0 *= 0.7;
			hdm->SetDisplacement(i, j, x0, y0, HGEDISP_NODE);
			/*
			float nc = 31;
			float nr = 31;
			float ch = 350/30.0;
			float cw = 350/30.0;
			float r = sqrtf(powf(j-nc/2,2)+powf(i-nr/2,2));
			float a = r*cosf(h/2.0)*0.1;
			float dx = sinf(a)*(i*ch-175)+cosf(a)*(j*cw-175);
			float dy = cosf(a)*(i*ch-175)-sinf(a)*(j*cw-175);
			hdm->SetDisplacement(j,i,dx,dy,HGEDISP_CENTER);
			*/
		}
	//卸载
	if(big_skill_time >= 400)
	{
		atb.reSet(500, 2);
		if(musicState)
			hge->System_SetState(HGE_STREAMVOLUME, 100);
		if(big_skill_two_snd)
		{
			big_skill_two_snd->Effect_Release();
			delete big_skill_two_snd;
			big_skill_two_snd = 0;
		}
		if(hdm)
		{
			delete hdm;
			hdm = 0;
		}
		if(hdmtex)
		{
			hge->Texture_Free(hdmtex);
			hdmtex = 0;
		}
		big_skill_id = 0;
	}
}
void big_skill_act_three()
{
	int act_id = 2;
	//初始化
	if(big_skill_time <= 0)
	{
		if(musicState)
			hge->System_SetState(HGE_STREAMVOLUME, 30);
		big_skill_three_snd = new Effect("../sound/three.ogg");
		big_skill_three_snd->Effect_Initiate();
		big_skill_three_snd->Effect_Frame();
		hge->System_Snapshot("skillrock.bmp");
		hdmtex = hge->Texture_Load("skillrock.bmp");
		if(!hdmtex)
			MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		//扭曲
		hdm = new hgeDistortionMesh(80, 60);
		hdm->SetTexture(hdmtex);
		hdm->SetTextureRect(30, 30, 620, 460);
		hdm->SetBlendMode(BLEND_COLORADD | BLEND_ALPHABLEND | BLEND_ZWRITE);
		hdm->Clear(0xff000000);
		hge->Random_Seed(int(hge->Timer_GetDelta()));
	}
	++big_skill_time;
	float h = big_skill_time;
	if(h >= 200)
	{
		h = 400 - big_skill_time;
	}
	h /= 5.0;
	for(int j = 0; j < 60; ++j)
		for(int i = 0; i < 80; ++i)
		{
			float x = i - 40;
			float y = j - 30;
			float tht = sqrt(40.0*40.0 + 30.0*30.0) - sqrt(x*x + y*y);
			float t = big_skill_time/100.0;
			float x0 = x*cos(tht*t) - y*sin(tht*t);
			float y0 = x*sin(tht*t) + y*cos(tht*t);
			x0 *= h/100.0;
			y0 *= h/100.0;
			x0 *= sqrt(40.0*40.0 + 30.0*30.0)/sqrt(1 + (i-40.0)*(i-40.0) + (j-30.0)*(j-30.0)) - 1.0;
			y0 *= sqrt(40.0*40.0 + 30.0*30.0)/sqrt(1 + (i-40.0)*(i-40.0) + (j-30.0)*(j-30.0)) - 1.0;
			hdm->SetDisplacement(i, j, x0, y0, 0);
		}
	//卸载
	if(big_skill_time >= 400)
	{
		if(musicState)
			hge->System_SetState(HGE_STREAMVOLUME, 100);
		if(big_skill_three_snd)
		{
			big_skill_three_snd->Effect_Release();
			delete big_skill_three_snd;
			big_skill_three_snd = 0;
		}
		if(hdm)
		{
			delete hdm;
			hdm = 0;
		}
		if(hdmtex)
		{
			hge->Texture_Free(hdmtex);
			hdmtex = 0;
		}
		big_skill_id = 0;
	}
}
//大技能，吸人
void big_skill_act_four()
{
	int act_id = 3;
	//初始化
	if(big_skill_time <= 0)
	{
		if(musicState)
			hge->System_SetState(HGE_STREAMVOLUME, 30);
		big_skill_four_snd = new Effect("../sound/four.ogg");
		big_skill_four_snd->Effect_Initiate();
		big_skill_four_snd->Effect_Frame();
		act[act_id].state = 2;
		for(int i = 0; i < 4; ++i)
		{
			if(i == 1)
				continue;
			act[i].sx = 0;
			act[i].sy = 0;
		}
	}
	//运行
	++big_skill_time;
	for(int i = 0; i < 4; ++i)
	{
		if(i == 1)
			continue;
		float dx = act[act_id].x - garden.getX(act[i].cx, act[i].cy);
		float dy = act[act_id].y - garden.getY(act[i].cx, act[i].cy);
		act[i].x = garden.getX(act[i].cx, act[i].cy) + dx * big_skill_time / 200.0;
		act[i].y = garden.getY(act[i].cx, act[i].cy) + dy * big_skill_time / 200.0;
	}
	if(big_skill_time >= 200)
	{
		if(musicState)
			hge->System_SetState(HGE_STREAMVOLUME, 100);
		if(big_skill_four_snd)
		{
			big_skill_four_snd->Effect_Release();
			delete big_skill_four_snd;
			big_skill_four_snd = 0;
		}
		act[act_id].state = -1;
		for(int i = 0; i < 4; ++i)
		{
			if(i == 1)
				continue;
			act[i].cx = act[act_id].cx;
			act[i].cy = act[act_id].cy;
			act[i].dx = act[act_id].x;
			act[i].dy = act[act_id].y;
		}
		big_skill_id = 0;
	}
}
//大技能控制
bool big_skill_run()
{
	switch(big_skill_id)
	{
		case 1:
			big_skill_act_one();
			return true;
			break;
		case 2:
			big_skill_act_two();
			return true;
			break;
		case 3:
			big_skill_act_three();
			return true;
			break;
		case 4:
			big_skill_act_four();
			return true;
			break;
	}
	return false;
}
bool big_skill_act()
{
	if(!act[act_id].ok())
		return false;
	if(hge->Input_GetKey() == HGEK_2 || (act_id == 0 && AI_order[0].act == 9))
	{
		if((!act[0].ok()) || big_skill_cd_one > 0 || act[0].life < 30 || act[0].state != -1)
			return false;
		if(orz[4] == 0)
		{
			char tmp[] = "../orz/one/one";
			orz[4] = new ORZ(17 ,345, 230, 50, 50, 8, tmp, tmp);
			orz[4]->ORZ_Initiate();
		}
		act[0].life -= 30;
		big_skill_cd_one = 5000;
		big_skill_time = 0;
		big_skill_id = 1;
		big_skill_run();
		return true;
	}
	if(hge->Input_GetKey() == HGEK_5 || (act_id == 1 && AI_order[1].act == 9))
	{
		if((!act[1].ok()) || big_skill_cd_two > 0 || act[1].life < 30 || act[1].state != -1)
			return false;
		if(orz[5] == 0)
		{
			char tmp[] = "../orz/two/two";
			orz[5] = new ORZ(17 ,345, 230, 50, 50, 8, tmp, tmp);
			orz[5]->ORZ_Initiate();
		}
		act[1].life -= 30;
		act[0].state = 1;
		act[2].state = 1;
		act[0].state_t = 800;
		act[2].state_t = 800;
		big_skill_cd_two = 5000;
		big_skill_time = 0;
		big_skill_id = 2;
		big_skill_run();
		return true;
	}
	if(hge->Input_GetKey() == HGEK_8 || (act_id == 2 && AI_order[0].act == 9))
	{
		if((!act[2].ok()) || big_skill_cd_three > 0 || act[2].life < 30 || act[2].state != -1)
			return false;
		if(orz[6] == 0)
		{
			char tmp[] = "../orz/three/three";
			orz[6] = new ORZ(17 ,345, 230, 50, 50, 8, tmp, tmp);
			orz[6]->ORZ_Initiate();
		}
		act[2].life -= 30;
		act[1].state = 3;
		act[3].state = 3;
		act[1].state_t = 800;
		act[3].state_t = 800;
		big_skill_cd_three = 5000;
		big_skill_time = 0;
		big_skill_id = 3;
		big_skill_run();
		return true;
	}
	if(hge->Input_GetKey() == HGEK_C || (act_id == 3 && AI_order[1].act == 9))
	{
		if((!act[3].ok()) || big_skill_cd_four > 0 || act[3].life < 30 || act[3].state != -1)
			return false;
		if(orz[7] == 0)
		{
			char tmp[] = "../orz/four/four";
			orz[7] = new ORZ(17 ,345, 230, 50, 50, 8, tmp, tmp);
			orz[7]->ORZ_Initiate();
		}
		act[3].life -= 30;
		act[3].state = 3;
		big_skill_cd_four = 5000;
		big_skill_time = 0;
		big_skill_id = 4;
		big_skill_run();
		return true;
	}
	return false;
}
//大技能显示控制
bool big_skill_act_RenderFunc()
{
	switch(big_skill_id)
	{
		case 2:
			hdm->Render(30, 30);
			return true;
			break;
		case 3:
			hdm->Render(30, 30);
			return true;
			break;
	}
	return false;
}
//初次物品信息记录
void first_goods()
{
	//信息记录
	if(replay == false)
	{
		//物品记录
		goods_info[num_info.goods_info_num].t = run_time;
		for(int p = 0; p < 30; ++p)
		{
			goods_info[num_info.goods_info_num].g[p] = garden.goods[p];
			goods_info[num_info.goods_info_num].g1[p] = act[0].goods[p];
			goods_info[num_info.goods_info_num].g2[p] = act[1].goods[p];
			goods_info[num_info.goods_info_num].g3[p] = act[2].goods[p];
			goods_info[num_info.goods_info_num].g4[p] = act[3].goods[p];
		}
		goods_info[num_info.goods_info_num].ng1 = act[0].num_g;
		goods_info[num_info.goods_info_num].ng2 = act[1].num_g;
		goods_info[num_info.goods_info_num].ng3 = act[2].num_g;
		goods_info[num_info.goods_info_num].ng4 = act[3].num_g;
		++num_info.goods_info_num;
	}
	else
	{
		//物品记录
		for(int p = 0; p < 30; ++p)
		{
			garden.goods[p] = goods_info[num_info.goods_info_num].g[p];
			act[0].goods[p] = goods_info[num_info.goods_info_num].g1[p];
			act[1].goods[p] = goods_info[num_info.goods_info_num].g2[p];
			act[2].goods[p] = goods_info[num_info.goods_info_num].g3[p];
			act[3].goods[p] = goods_info[num_info.goods_info_num].g4[p];
		}
		act[0].num_g = goods_info[num_info.goods_info_num].ng1;
		act[1].num_g = goods_info[num_info.goods_info_num].ng2;
		act[2].num_g = goods_info[num_info.goods_info_num].ng3;
		act[3].num_g = goods_info[num_info.goods_info_num].ng4;
		++num_info.goods_info_num;
	}
}

//test层
bool test_Initiate()
{
	srand(time(0));
	if(replay)
		Replay_init();
	num_info.dead_info_num = 0;
	num_info.goods_info_num = 0;
	num_info.order_info_num = 0;
	//技能初始化
	big_skill_cd_one = 0;
	big_skill_cd_two = 0;
	big_skill_cd_three = 0;
	big_skill_cd_four = 0;
	//音效初始化
	big_skill_one_snd = 0;
	big_skill_two_snd = 0;
	big_skill_three_snd = 0;
	big_skill_four_snd = 0;
	be_killed = 0;
	be_killed = new Effect("../sound/killed1.mp3");
	be_killed->Effect_Initiate();
	be_killed_ha = 0;
	be_killed_ha = new Effect("../sound/k1.mp3");
	be_killed_ha->Effect_Initiate();

	act_id = 0;
	AI_init();
	run_time = start_time;
	big_skill_id = 0;

	//获取队名
	AI_getname();
	//清除一些初始信息
	memset(user[0]->m,0,sizeof(user[0]->m));
	memset(user[1]->m,0,sizeof(user[1]->m));
	memset(user[0]->m2,0,sizeof(user[0]->m2));
	memset(user[1]->m2,0,sizeof(user[1]->m2));
	atc_num[0] = atc_num[1] = 0;
	atc_tot[0] = atc_tot[1] = 0;
	//中文字体
	fnt = new GfxFont("楷体", 18, true, false, true);
	fnt->SetColor(0xFFFFFFFF);
	fnt2 = new GfxFont("楷体", 32, true, false, true);
	fnt2->SetColor(0xFFFFFFFF);
	//上帝模式
	god_mode[0] = false;
	god_mode[1] = false;
	clear_time = false;

	show_cnt += 50;
	atb.AutoBlack_Initiate();
	isSuper_Garden = false;
	boder.Background_Initiate();
	boder.Set_Fnt(false);
	grass.Background_Initiate();
	grass.Set_Fnt(false);
	rock_up.Background_Initiate();
	rock_up.Set_Fnt(false);
	rock_down.Background_Initiate();
	rock_down.Set_Fnt(false);
	bgstar.Stars_Initiate();
	for(int i = 0; i < 10; ++i)
		star[i].Star_Initiate();
	timer.Timer_Initiate();
	garden.Garden_Initiate();
	snd3.Sound_Initiate();
	//字体
	showFnt = new hgeFont("../font/font2.fnt");
	if (!showFnt)
		return false;
	winFnt = new hgeFont("../font/font2.fnt");
	if (!winFnt)
		return false;
	//位置初始化
	act[0].setPos(7, 7);
	act[1].setPos(14, 7);
	act[2].setPos(7, 11);
	act[3].setPos(14, 11);
	for(int i = 0; i < 4; ++i)
	{
		act[i].dir = DOWN;
		act[i].x = garden.getX(act[i].cx, act[i].cy);
		act[i].y = garden.getY(act[i].cx, act[i].cy);
		act[i].size = garden.getZ(act[i].cx, act[i].cy);
		act[i].Character_Initiate();
	}
	if(show_ren) //开启单打
	{
		act[act_id_one].no_show = true;
		act[act_id_two].no_show = true;
		act[act_id_one].death_time = 20000;
		act[act_id_two].death_time = 20000;
	}
	//读取地图
	Map_init();
	return true;
}
bool test_FrameFunc()
{
	//彩蛋
	if(hge->Input_GetKey() == HGEK_0)
		clear_time = bool(1 - (int)clear_time);
	//二号彩蛋
	if(hge->Input_GetKey() == HGEK_X)
		snd3.Sound_setPath("../sound/TheMass.ogg");
	//游戏结束
	if(run_time <= 0)
		return false;
	if(fntTime > 0)
		--fntTime;
	if(bgsound == false)
	{
		snd3.Sound_Frame();
		bgsound = true;
	}
	//星空背景
	bgstar.Stars_Frame();
	//让平台挂掉
	//test_key();
	//garden
	if(garden.first)
	{
		garden.Garden_Frame();
		first_goods();
	}
	else
	{
		garden.Garden_Frame();
	}
	//计时器
	if(!timer.Timer_Frame())
		return false;
	//边框
	if(isSuper_Garden == false)
		boder.Background_Frame();
	//动画
	for(int i = 4; i < 8; ++i)
		if(orz[i])
		{
			if(orz[i]->ORZ_Frame())
			{
				orz[i]->ORZ_Release();
				delete orz[i];
				orz[i] = 0;
			}
		}
	//大技能劫持
	if(big_skill_run())
		return false;
	//动画控制
	for(int i = 0; i < 16; ++i)
		if(orz[i])
		{
			if(orz[i]->ORZ_Frame())
			{
				orz[i]->ORZ_Release();
				delete orz[i];
				orz[i] = 0;
			}
		}
	for(int i = 0; i < 30; ++i)
		if(orz2[i])
		{
			if(orz2[i]->ORZ2_Frame())
			{
				orz2[i]->ORZ2_Release();
				delete orz2[i];
				orz2[i] = 0;
			}
		}
	//计时
	changeTime();
	//开始刷AI
	setInfo();
	AI_work(act_id, act[act_id].ok());
	//社交
	get_talk(act_id);
	//幻术效果
	if(act[0].state == 1 && act[2].state == 1)
	{
		if(atb.AutoBlack_Frame())
			atb.reSet(300, 1);
	}
	else
		atb.setStop(true);
	//能量增长
	if(run_time % 100 == 0)
		for(int i = 0; i < 4; ++i)
			++act[i].life;
	//上帝模式检测
	if(run_time <= 9500 && god_mode[0] == false && AI_order[0].act == 2012 && strcmp(AI_name[0].my1Name, "1992") == 0 && strcmp(AI_name[0].my2Name, "zyn") == 0)
		god_mode[0] = true;
	if(run_time <= 9500 && god_mode[1] == false && AI_order[1].act == 2012 && strcmp(AI_name[1].my1Name, "1992") == 0 && strcmp(AI_name[1].my2Name, "zyn") == 0)
		god_mode[1] = true;
	if(god_mode[act_id&1])
	{
		act[act_id].small_skill_cd = 0;
		act[act_id].len = (10000 - run_time) / 334;
		if(run_time % 3 == 0)
			++act[act_id].life;
	}
	//物品处理k
	goods_change();
	//大技能
	big_skill_act();
	//小技能
	small_skill_act();
	//人物移动
	character_move();
	//技能
	skill_act();
	//自杀
	kill_myself();
	//星星
	for(int i = 0; i < 10; ++i)
	{
		star[i].Star_Frame();
		star[i].setPos(hge->Random_Int(250, 420), hge->Random_Int(150, 300));
	}
	//人物轮换
	if(++act_id >= 4)
		act_id = 0;
	return false;
}
bool test_RenderFunc()
{
	//星空背景
	bgstar.Stars_Render();
	
	//陆地
	if(clear_time == false)
		grass.Background_Render();
	//岩石
	if(clear_time == false)
		rock_up.Background_Render();
	//物品,障碍与人物
	for(int j = 0; j < boderY; ++j)
		for(int i = 0; i < boderX; ++i)
		{
			for(int k = 0; k < 4; ++k)
				if(act[k].cx == i && act[k].cy == j)
				{
					act[k].setShi();
					//置幻动画效果
					if(act[0].state == 1 && act[2].state == 1)
					{
						if(k == 1 || k == 3)
							act[k].setDan(0x5f);
					}
					//晕眩动画效果
					if(act[1].state == 3 && act[3].state == 3)
					{
						for(int p = 0; p < 4; ++p)
							act[p].setDan((run_time*(p+1)/2) % 255);
					}
					act[k].Character_Render();
				}
				if(garden.getInfo(i, j) == TREE)
					garden.Garden_Render_TREE(j, i, show_cnt, garden.dst[j][i]);
			garden.Garden_Render_GOODS(j, i);
		}
	//岩石
	if(clear_time == false)
		rock_down.Background_Render();
	//幻术效果
	if(act[2].state == 1 && act[2].state == 1)
		atb.AutoBlack_Render();
	//技能
	for(int i = 0; i < 4; ++i)
	{
		if(skill[i])
			skill[i]->Skill_Render();
	}
	
	//月球
	garden.Garden_Render_LAND(340, 230, (start_time - run_time)/100.0);
	//动画控制
	for(int i = 0; i < 16; ++i)
		if(orz[i])
			orz[i]->ORZ_Render();
	for(int i = 0; i < 30; ++i)
		if(orz2[i])
			orz2[i]->ORZ2_Render();
	//星星
	for(int i = 0; i < 10; ++i)
		star[i].Star_Render();
	if(fntTime > 0)
		snd3.Sound_Render();
	//倒计时
	timer.Timer_Render();
	//胜利信息
	if(act[0].sorce + act[2].sorce > act[1].sorce + act[3].sorce)
		garden.Garden_Render_WIN(720, 80);
	if(act[0].sorce + act[2].sorce < act[1].sorce + act[3].sorce)
		garden.Garden_Render_WIN(715, 500);
	//边框
	if(isSuper_Garden == false)
		boder.Background_Render();
	//信息显示
	info_show();
	//大技能影响
	big_skill_act_RenderFunc();
	//动画控制
	for(int i = 4; i < 8; ++i)
		if(orz[i])
			orz[i]->ORZ_Render();
	return false;
}
void test_Release()
{
	//技能的附加资源析构
	if(big_skill_id != 0)
	{
		big_skill_time = 500;
		big_skill_act();
	}
	//动画控制
	for(int i = 0; i < 16; ++i)
		if(orz[i])
		{
			orz[i]->ORZ_Release();
			delete orz[i];
			orz[i] = 0;
		}
	for(int i = 0; i < 30; ++i)
		if(orz2[i])
		{
			orz2[i]->ORZ2_Release();
			delete orz2[i];
			orz2[i] = 0;
		}
	//音效
	if(big_skill_one_snd)
	{
		big_skill_one_snd->Effect_Release();
		delete big_skill_one_snd;
		big_skill_one_snd = 0;
	}
	if(big_skill_two_snd)
	{
		big_skill_two_snd->Effect_Release();
		delete big_skill_two_snd;
		big_skill_two_snd = 0;
	}
	if(big_skill_three_snd)
	{
		big_skill_three_snd->Effect_Release();
		delete big_skill_three_snd;
		big_skill_three_snd = 0;
	}
	if(big_skill_four_snd)
	{
		big_skill_four_snd->Effect_Release();
		delete big_skill_four_snd;
		big_skill_four_snd = 0;
	}
	if(be_killed)
	{
		be_killed->Effect_Release();
		delete be_killed;
		be_killed = 0;
	}
	if(be_killed_ha)
	{
		be_killed_ha->Effect_Release();
		delete be_killed_ha;
		be_killed_ha = 0;
	}
	atb.AutoBlack_Release();
	bgstar.Stars_Release();
	for(int i = 0; i < 10; ++i)
		star[i].Star_Release();
	rock_up.Background_Release();
	rock_down.Background_Release();
	grass.Background_Release();
	boder.Background_Release();
	timer.Timer_Release();
	garden.Garden_Release();
	snd3.Sound_Release();
	for(int i = 0; i < 4; ++i)
		act[i].Character_Release();
	for(int i = 0; i < 4; ++i)
	{
		if(skill[i])
		{
			skill[i]->Skill_Release();
			skill[i] = 0;
		}
	}
	if(showFnt)
	{
		delete showFnt;
		showFnt = 0;
	}
	if(winFnt)
	{
		delete winFnt;
		winFnt = 0;
	}
	if(fnt)
	{
		delete fnt;
		fnt = 0;
	}
	if(fnt2)
	{
		delete fnt2;
		fnt2 = 0;
	}
	if(user[0])
	{
		delete user[0];
		user[0] = 0;
	}
	if(user[1])
	{
		delete user[1];
		user[1] = 0;
	}
	return;
}

//转场用场景 BACKROCK
HTEXTURE tex;
hgeQuad bgQuad;
int val;
//声明： FrameFunc()中有Release()
bool backrock_Initiate()
{
	if(changesign)
	{
		val = 255;
		tex = hge->Texture_Load("backrock.bmp");
		if(!tex)
		{
			MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
			return false;
		}
		bgQuad.tex = tex;
		//屏幕想x，y设置
		bgQuad.v[0].x = 0, bgQuad.v[0].y = 0;
		bgQuad.v[1].x = 800, bgQuad.v[1].y = 0;
		bgQuad.v[2].x = 800, bgQuad.v[2].y = 600;
		bgQuad.v[3].x = 0, bgQuad.v[3].y = 600;
		for(int i = 0; i < 4; ++i)
		{
			bgQuad.v[i].z = 0;
			bgQuad.v[i].col = 0xffffffff;
		}
		//纹理x，y坐标
		bgQuad.v[0].tx = 0, bgQuad.v[0].ty = 0;
		bgQuad.v[1].tx = 1, bgQuad.v[1].ty = 0;
		bgQuad.v[2].tx = 1, bgQuad.v[2].ty = 1;
		bgQuad.v[3].tx = 0, bgQuad.v[3].ty = 1;
		bgQuad.blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_NOZWRITE;
	}
	return true;
}
bool backrock_FrameFunc()
{
	if(changesign)
	{
		//暂停介入
		if(stop)
			return true;
		if(val > 0)
		{
			for(int i = 0; i < 4 ; ++i)
				bgQuad.v[i].col = ARGB(val, 0xff, 0xff, 0xff);
			val -= 2;
		}
		else
		{
			changesign = false;
			hge->Texture_Free(tex);
		}
		return false; //劫持逻辑层
	}
	return true;
}
bool backrock_RenderFunc()
{
	if(changesign)
	{
		hge->Gfx_RenderQuad(&bgQuad);
	}
	return false;
}
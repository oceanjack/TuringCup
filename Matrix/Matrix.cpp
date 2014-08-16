#include "Matrix.h"
#include "resource.h"
//主窗口
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	hge = hgeCreate(HGE_VERSION);

	hge->System_SetState(HGE_TITLE, "Magic Field - 100"); //标题
	hge->System_SetState(HGE_LOGFILE, "logs.log"); //log
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc); //逻辑
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc); //渲染
	hge->System_SetState(HGE_USESOUND, true); //音频
	hge->System_SetState(HGE_WINDOWED, window); //窗口
	hge->System_SetState(HGE_SHOWSPLASH, false); //logo
	hge->System_SetState(HGE_FPS, 100); //频率
	hge->System_SetState(HGE_SCREENWIDTH, 800); //宽
	hge->System_SetState(HGE_SCREENHEIGHT, 600); //高
	hge->System_SetState(HGE_SCREENBPP, 32); //颜色
	hge->System_SetState(HGE_ICON,MAKEINTRESOURCE(IDI_ICON1)); //标志

	if(!hge->System_Initiate() || !Initiate())
	{
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		Release();
		hge->System_Shutdown();
		hge->Release();
	}
	hge->System_Start();

	Release();
	hge->System_Shutdown();
	hge->Release();
	return 0;
}
//初始化加载资源
bool Initiate()
{
	//转场专用
	backrock_Initiate();
	switch(model_index)
	{
		case MENU:
			menu_Initiate();
			break;
		case PREPARE:
			prepare_Initiate();
			break;
		case TEST:
			test_Initiate();
			break;
	}
	return true;
}
//逻辑层
bool FrameFunc()
{
	//FPS显示
	FPS_FrameFunc();
	//转场专用
	if(!backrock_FrameFunc())
		return false; //劫持逻辑层
	float dt = hge->Timer_GetDelta();
	switch(hge->Input_GetKey())
	{
		//音乐大小
		case HGEK_F9:
			if(musicState == false)
			{
				hge->System_SetState(HGE_STREAMVOLUME, 100);
				musicState = true;
			}
			else
			{
				hge->System_SetState(HGE_STREAMVOLUME, 0);
				musicState = false;
			}
			break;
		case HGEK_F5:
			fntTime = 100;
			switch(model_index)
			{
				case MENU:
					snd.Sound_decVal();
					break;
				case PREPARE:
					snd2.Sound_decVal();
					break;
				case TEST:
					snd3.Sound_decVal();
					break;
			}
			break;
		case HGEK_F6:
			fntTime = 100;
			switch(model_index)
			{
				case MENU:
					snd.Sound_addVal();
					break;
				case PREPARE:
					snd2.Sound_addVal();
					break;
				case TEST:
					snd3.Sound_addVal();
					break;
			}
			break;
		//音效大小
		case HGEK_F10:
			if(effectState == false)
			{
				hge->System_SetState(HGE_FXVOLUME, 100);
				effectState = true;
			}
			else
			{
				hge->System_SetState(HGE_FXVOLUME, 0);
				effectState = false;
			}
			break;
		case HGEK_F7:
			Effect::dec();
			break;
		case HGEK_F8:
			Effect::add();
			break;
		//设置窗口
		case HGEK_F1:
			if(window == false)
				window = true;
			else
				window = false;
			hge->System_SetState(HGE_WINDOWED, window);
			break;
		//调整游戏速度
		case HGEK_F3:
			game_speed -= 10;
			if(game_speed <= 20)
				game_speed = 20;
			hge->System_SetState(HGE_FPS, game_speed);
			break;
		case HGEK_F4:
			game_speed += 10;
			if(game_speed >= 150)
				game_speed = 150;
			hge->System_SetState(HGE_FPS, game_speed);
			break;
		//暂停
		case HGEK_F2:
			if(stop == false)
				stop = true;
			else
				stop = false;
			return false;
			break;
		//后退
		case HGEK_ESCAPE:
			//取消暂停
			stop = false;
			switch(model_index)
			{
				case PREPARE:
					return true;
					break;
				case MENU:
					return true;
					break;
				case TEST:
					if(replay == false)
					{
						Save_replay();
						if(run_time <= 0)
							Save_replay(true);
					}
					else
					{
						hge->System_Snapshot("../replay/result.bmp");
					}
					hge->System_Snapshot("backrock.bmp");
					Release();
					//资源标记
					//hge->Release();
					//hge = hgeCreate(HGE_VERSION);
					//hge->System_Initiate();
					//
					--model_index;
					bgsound = false;
					changesign = true;
					Initiate();
					FrameFunc();
					break;
			}
			break;
		//跳转画面
		case HGEK_ENTER:
			stop = false;
			if(model_index >= max_model_index || model_index == MENU)
				break;
			Release();
			//资源标记
			//hge->Release();
			//hge = hgeCreate(HGE_VERSION);
			//hge->System_Initiate();
			//
			++model_index;
			bgsound = false;
			changesign = false;
			Initiate();
			break;
	}
	//暂停
	if(stop)
		return false;
	bool sign = false;
	switch(model_index)
	{
		case MENU:
			sign = menu_FrameFunc();
			break;
		case PREPARE:
			sign = prepare_FrameFunc();
			if(sign)
			{
				stop = false;
				if(model_index == max_model_index || model_index == MENU)
					break;
				Release();
				//资源标记
				//hge->Release();
				//hge = hgeCreate(HGE_VERSION);
				//hge->System_Initiate();
				//
				++model_index;
				bgsound = false;
				changesign = false;
				Initiate();
				sign = false;
				FrameFunc();
			}
			break;
		case TEST:
			sign = test_FrameFunc();
			break;
	}
	return sign;
}
//渲染层
bool RenderFunc()
{
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);
	//初次截图(换背景时用，不要删)
	/*
	if(firstrun && model_index == MENU)
	{
		switch(model_index)
		{
			case MENU:
				bag.Background_Render();
				break;
			case PREPARE:
				bag2.Background_Render();
				break;
		}
		hge->Gfx_EndScene();
		hge->System_Snapshot("shotfs001.bmp");
		firstrun = false;
		return false;
	}*/
	switch(model_index)
	{
		case MENU:
			menu_RenderFunc();
			break;
		case PREPARE:
			prepare_RenderFunc();
			break;
		case TEST:
			test_RenderFunc();
			break;
	}
	//转场专用
	backrock_RenderFunc();
	hge->Gfx_EndScene();
	return false;
}
//卸载资源
void Release()
{
	switch(model_index)
	{
		case MENU:
			menu_Release();
			break;
		case PREPARE:
			prepare_Release();
			break;
		case TEST:
			test_Release();
			break;
	}
	return;
}
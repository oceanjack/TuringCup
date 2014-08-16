#pragma once
#include "Info.h"
#include <string.h>
struct User
{
	int m[200]; //记忆空间，不会被刷新的存储位置，请把需要的信息记录在其中。
	//**复赛增加接口
	double m2[200]; //记忆空间，不会被刷新的存储位置，请把需要的信息记录在其中。
	//**
	int mapInfo[boderY][boderX]; //地图信息
	int who; //当前人物
	int t; //时间
	int map_goods; // 地图物品数量
	int map_goods_x[50]; //坐标
	int map_goods_y[50];
	int map_goods_time[50]; //出现时间
	int map_goods_type[50]; //物品类型
	//**复赛增加接口
	int FPS; //平台即时帧数
	int id; //当前运行人物
	//**

	int y[2]; //我方坐标
	int x[2];
	int dir[2]; //我方人物方向
	int power[2]; //我方人物power
	int sorce[2]; //我方分数
	int goods_num[2]; //物品数量
	int goods_tpye[2][50]; //物品类型
	int magic_len[2]; //人物魔法长度
	int magic_num[2]; //已运行魔法长度
	int magic_x[2][50]; //魔法坐标数组
	int magic_y[2][50];
	//**复赛增加接口
	int dead_cd[2]; //自杀冷却时间
	int s_skill_cd[2]; //小技能冷却时间
	int b_skill_cd[2]; //大技能冷却时间
	//**

	int ey[2]; //敌方坐标
	int ex[2];
	int edir[2]; //敌方人物方向
	int epower[2]; //敌方人物power
	int esorce[2]; //敌方分数
	int egoods_num[2]; //敌方物品数量
	int egoods_tpye[2][50]; //敌方物品类型
	int emagic_num[2]; //敌方已运行魔法长度
	int emagic_x[2][50]; //敌方魔法坐标数组
	int emagic_y[2][50];
	//**复赛增加接口
	int edead_cd[2]; //敌方自杀冷却时间
	int es_skill_cd[2]; //敌方小技能冷却时间
	int eb_skill_cd[2]; //敌方大技能冷却时间
	//**
};
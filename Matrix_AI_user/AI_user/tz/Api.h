#pragma once

#include <iostream>
using namespace std;

//����
const int STOP = 0;
const int UP = 1;
const int DOWN = 2;
const int LEFT = 3;
const int RIGHT = 4;
const int ATC = 5; //����
//**��������ָ��
const int ONE = 6; //С����1
const int TWO = 7; //С����2
const int THREE = 8; //С����3
const int KILL = 9; //����
const int DEAD = 44; //��ɱ����40��
//**

//ָ��
struct Order
{
	int act;
	int dir[30];
};

//�߽�
const int boderX = 22;
const int boderY = 17;

struct PlayerName
{
	char TeamName[50];		//��������
	char my1Name[50];		//һ��AI������
	char my2Name[50];		//����AI������
};

//��ͼ����
const int STAR = 0; //�����ߣ�ħ���ɴ�
const int LAND = 1; //���ߣ�ħ���ɴ�
const int TREE = 2; //�����ߣ�ħ�����ɴ�

//��Ʒ����
const int BLUE = 0; //�ӳ�ħ������
const int BOMB = 1; //��30��
const int BOOK = 2; //��30����

struct User
{
	int m[200]; //����ռ䣬���ᱻˢ�µĴ洢λ�ã������Ҫ����Ϣ��¼�����С�
	//**�������ӽӿ�
	double m2[200]; //����ռ䣬���ᱻˢ�µĴ洢λ�ã������Ҫ����Ϣ��¼�����С�
	//**
	int mapInfo[boderY][boderX]; //��ͼ��Ϣ
	int who; //��ǰ����
	int t; //ʱ��
	int map_goods; // ��ͼ��Ʒ����
	int map_goods_x[50]; //����
	int map_goods_y[50];
	int map_goods_time[50]; //����ʱ��
	int map_goods_type[50]; //��Ʒ����
	//**�������ӽӿ�
	int FPS; //ƽ̨��ʱ֡��
	int id; //��ǰ��������
	//**

	int y[2]; //�ҷ�����
	int x[2];
	int dir[2]; //�ҷ����﷽��
	int power[2]; //�ҷ�����power
	int sorce[2]; //�ҷ�����
	int goods_num[2]; //��Ʒ����
	int goods_tpye[2][50]; //��Ʒ����
	int magic_len[2]; //����ħ������
	int magic_num[2]; //������ħ������
	int magic_x[2][50]; //ħ����������
	int magic_y[2][50];
	//**�������ӽӿ�
	int dead_cd[2]; //��ɱ��ȴʱ��
	int s_skill_cd[2]; //С������ȴʱ��
	int b_skill_cd[2]; //������ȴʱ��
	//**

	int ey[2]; //�з�����
	int ex[2];
	int edir[2]; //�з����﷽��
	int epower[2]; //�з�����power
	int esorce[2]; //�з�����
	int egoods_num[2]; //�з���Ʒ����
	int egoods_tpye[2][50]; //�з���Ʒ����
	int emagic_num[2]; //�з�������ħ������
	int emagic_x[2][50]; //�з�ħ����������
	int emagic_y[2][50];
	//**�������ӽӿ�
	int edead_cd[2]; //�з���ɱ��ȴʱ��
	int es_skill_cd[2]; //�з�С������ȴʱ��
	int eb_skill_cd[2]; //�з�������ȴʱ��
	//**
};

#ifdef AI_USER_EXPORTS
#define AI_USER_API __declspec(dllexport)
#else
#define AI_USER_API __declspec(dllimport)
#endif

extern "C" AI_USER_API PlayerName NameAI();
extern "C" AI_USER_API Order get_order(User *);
#pragma once
#include "User.h"
#include <stdlib.h>
//���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
//��ı�׼�������� DLL �е������ļ��������������϶���� AI_USER_EXPORTS
//���ű���ġ���ʹ�ô� DLL ��
//�κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
//AI_USER_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
//������Ϊ�Ǳ������ġ�
#ifdef AI_USER_EXPORTS
#define AI_USER_API __declspec(dllexport)
#else
#define AI_USER_API __declspec(dllimport)
#endif
//�����Ǵ� AI_user.dll ������
class AI_USER_API CAI_user 
{
	public:
		CAI_user(void);
};
extern AI_USER_API int nAI_user;
AI_USER_API int fnAI_user(void);
extern "C" AI_USER_API PlayerName NameAI();
extern "C" AI_USER_API Order get_order(User *);
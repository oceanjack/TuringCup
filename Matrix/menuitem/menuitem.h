#include "../header.h"
#include "../sound/effect.h"
class hgeGUIMenuItem : public hgeGUIObject
{
	private:
		float x; //����
		float y;
		float dx, dx2; //��ȡλ��
		float dy, dy2;
		float cx, cy; //λ��λ��
		float delay; //�ӳ�
		float mx, my; //�ƶ�
		bool isFocus; //����
		bool isFinish; //����
		bool isRun; //����
		hgeQuad *bgQuad; //���ο�
		HEFFECT tex; //��ť�ز�
		HEFFECT tex3; //�����ز�
		char *path; //·��
		char *path2;
		hgeSprite *spr3;
		hgeParticleSystem *par; //����ϵͳ
		int model;
		Effect *snd;
	public:
		hgeGUIMenuItem(int _id, float _x, float _y, float _dx, float _dy, float _dx2, float _dy2, float _delay, char *p, char *p2, bool ps = true);
		bool hgeGUIMenuItem_Initiate();
		virtual void Render();
		void hgeGUIMenuItem_Release();
		virtual void Update(float dt);
		virtual void Enter();
		virtual void Leave();
		virtual bool IsDone();
		virtual void Focus(bool bFocused);
		virtual void MouseOver(bool bOver);
		virtual bool MouseLButton(bool bDown);
		virtual bool MouseRButton(bool bDown);
};
#include "../header.h"
#include "../sound/effect.h"
class hgeGUIMenuItem : public hgeGUIObject
{
	private:
		float x; //坐标
		float y;
		float dx, dx2; //截取位置
		float dy, dy2;
		float cx, cy; //位移位置
		float delay; //延迟
		float mx, my; //移动
		bool isFocus; //焦点
		bool isFinish; //结束
		bool isRun; //乱跑
		hgeQuad *bgQuad; //矩形框
		HEFFECT tex; //按钮素材
		HEFFECT tex3; //粒子素材
		char *path; //路径
		char *path2;
		hgeSprite *spr3;
		hgeParticleSystem *par; //粒子系统
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
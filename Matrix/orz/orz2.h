#include "../header.h"
extern HGE *hge;
class ORZ2
{
	private:
		int t; //运行时间控制
		float l;
		float x; //设置坐标
		float y;
		float dx;
		float dy;
		int c; //随机因素
		HEFFECT tex;
		hgeSprite *spr; //精灵
		hgeParticleSystem *par; //粒子系统
	public:
		ORZ2(float _x, float _y, float _dx, float _dy);
		~ORZ2();
		bool ORZ2_Initiate();
		bool ORZ2_Frame();
		bool ORZ2_Render();
		void ORZ2_Release();
		void setXY(int _dx, int _dy);
};
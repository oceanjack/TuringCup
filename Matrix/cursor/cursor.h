#include "../header.h"
extern HGE *hge;
class Cursor
{
	private:
		float x; ///横坐标
		float y; ///纵坐标
		float cx; //原坐标
		float cy; //原坐标
		bool isRender;
		HEFFECT tex; //
		char *path; //鼠标图片
		char *path2; //鼠标精灵
		hgeSprite *spr; //精灵
		hgeQuad *bgQuad; //矩形框
		hgeParticleSystem *par; //粒子系统
	public:
		Cursor(char *p, char *p2); //构造
		~Cursor();
		bool Cursor_Initiate();
		bool Cursor_Frame();
		bool Cursor_Render();
		void Cursor_Release();
};

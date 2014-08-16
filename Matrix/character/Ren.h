#include "../header.h"
extern HGE *hge;
class Ren
{
	private:
		float x; //实际坐标 
		float y;
		float s; //尺寸
		char *path; //人物路径
		char *path2;
		char *path3;
		char *path4;
		int pos[4];
		int t; //时间
		int dir; //方向
		bool ready; //动画准备
		HEFFECT tex[4]; //人物
		hgeQuad *Quad[4]; //人物图像
	public:
		Ren(char *p, char *p2, char *p3, char *p4);
		~Ren();
		bool Ren_Initiate();
		bool Ren_Frame();
		bool Ren_Render();
		void Ren_Release();
};

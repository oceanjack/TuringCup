#include "../header.h"
extern HGE *hge;
class Background
{
	private:
		int timed; //运行时间
		float cx; //记录点
		float cy; //记录点
		int dir; //扭曲画面控制
		char *path; //图片路径
		bool showfnt; //显示字体
		bool nodis; //是否制造波纹效果
		bool errordis; //资源未加载去除波纹
		float hight; //高
		float weight; //宽
		HTEXTURE bgtex; //背景
		HTEXTURE chtex; //扭曲背景
		hgeDistortionMesh *dis; //波纹
		hgeQuad *bgQuad; //矩形框
		hgeFont *bgFnt; //字体
	public:
		Background(char *p, int d, bool nd); //构造
		~Background();
		bool Background_Initiate();
		bool Background_Frame();
		bool Background_Render();
		void Background_Release();
		void Set_Fnt(bool s); //设置显示字体
		void Set_Size(float h, float w);
};

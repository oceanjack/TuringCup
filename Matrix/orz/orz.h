#include "../header.h"
extern HGE *hge;
class ORZ
{
	private:
		int id; //当前图片
		int t; //运行时间控制
		int len; //动画图片长度
		char *img_path;
		char *music_path;
		int h; //设置高度
		int w; //设置宽度
		int x; //设置坐标
		int y;
		int l_t; //时间限定
		HEFFECT texs[10];
		hgeQuad *Quad[10];
	public:
		ORZ(int _lt, int _x, int _y, int _h, int _w, int _l, char *_ip, char *_mp);
		~ORZ();
		bool ORZ_Initiate();
		bool ORZ_Frame();
		bool ORZ_Render();
		void ORZ_Release();
};
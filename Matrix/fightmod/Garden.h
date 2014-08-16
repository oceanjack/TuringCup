#include "../header.h"
#include "../fightmod/Goods.h"
extern HGE *hge;
class Garden
{
	private:
		float **mx; //x坐标的映射
		float **my; //y坐标的映射
		float **mz; //缩放
		int **act; //舞台信息
		HEFFECT tex; //月球
		HEFFECT texs[14]; //0-7为障碍，8-10为物品,11-13为各种光
		hgeQuad *bgQuad; //矩形框
		hgeQuad *bgQuads[14]; //0-7为障碍，8-10为物品，11-13为各种光
		bool isSuper;
		int t; //生命线
	public:
		bool **dst; //造树特效
		bool first; //初始化
		const static int x = 22;
		const static int y = 17;
		Goods goods[30];
		Garden();
		~Garden();
		bool Garden_Initiate();
		bool Super_Garden_Initiate();
		bool Garden_Frame();
		bool Garden_Frame_GOODS();
		bool Garden_Render_LAND(int _x, int _y, float _t);
		bool Garden_Render_GOODS(int j, int i);
		bool Garden_Render_TREE(int j, int i, int cnt,bool ds);
		bool Garden_Render_WIN(int _x, int _y);
		void Garden_Release();
		void Super_Garden_Release();
		bool check(int _x, int _y);
		bool check_skill(int _x, int _y);
		void setAct(int _x, int _y, int _val); //设置
		void reset(); //清空
		void Super_reset(); //清空
		float getX(int _x, int _y);
		float getY(int _x, int _y);
		float getZ(int _x, int _y);
		int getInfo(int _x, int _y);
};
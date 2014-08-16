#include "../header.h"
extern HGE *hge;
class AutoBlack
{
	private:
		int time; //变换时间
		hgeQuad bgQuad; //变换四边形
		int dir; //变换形式
		float cval; //透明度
		float dval; //变化度
		static const int M_1 = 1;
		static const int M_2 = 2;
		bool stop;
	public:
		AutoBlack(int t, int d);
		~AutoBlack();
		bool AutoBlack_Initiate();
		bool AutoBlack_Frame();
		bool AutoBlack_Render();
		void AutoBlack_Release();
		void reSet(int t, int d);
		void setStop(int _val);
		bool getStop();
};
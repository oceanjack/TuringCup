#include "../header.h"
extern HGE *hge;
class Star
{
	private:
		bool act; //是否活动
		float x;
		float y;
		int timed;
		HEFFECT tex;
		hgeSprite *spr;
		hgeParticleSystem *par;
	public:
		Star(float _x, float _y);
		~Star();
		bool Star_Initiate();
		bool Star_Frame();
		bool Star_Render();
		void Star_Release();
		void setAct(bool f);
		void setPos(float _x, float _y);
};
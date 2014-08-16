#include "../header.h"
extern HGE *hge;
class Stars
{
	private:
		float ty;
		float tx;
		float t;
		float dt;
		float tt;
		float dtt;
		int up;
		int down;
		hgeQuad bgd;
		hgeQuad bgr;
		HTEXTURE bgtex;
	public:
		Stars();
		~Stars();
		bool Stars_Initiate();
		bool Stars_Frame();
		bool Stars_Render();
		void Stars_Release();
};
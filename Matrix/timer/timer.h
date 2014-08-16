#include "../header.h"
extern HGE *hge;
class Timer
{
	private:
		int id;
		int t;
		HEFFECT texs[4];
		hgeQuad *Quad[4];
	public:
		Timer();
		~Timer();
		bool Timer_Initiate();
		bool Timer_Frame();
		bool Timer_Render();
		void Timer_Release();
};
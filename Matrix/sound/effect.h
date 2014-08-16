#include "../header.h"
extern HGE *hge;
class Effect
{
	private:
		HEFFECT snd;
		int pan;
		float pitch;
		bool loop;
		char *path;
	public:
		Effect(char *p);
		~Effect();
		bool Effect_Initiate();
		bool Effect_Frame();
		bool Effect_Render();
		void Effect_Release();
		void Effect_ReSet(char *p);
		static void add();
		static void dec();
};

#include "../header.h"
extern HGE *hge;
class Sound
{
	private:
		HEFFECT snd;
		bool loop;
		char *path;
		hgeFont *Fnt;
	public:
		Sound(char *p);
		~Sound();
		bool Sound_Initiate();
		bool Sound_Frame();
		bool Sound_Render();
		void Sound_Release();
		void Sound_addVal();
		void Sound_decVal();
		void Sound_setPath(char *p);
		void Sound_stop();
		void Sound_play();
};

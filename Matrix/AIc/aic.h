#include "../header.h"
#include "./ReadAI.h"
extern HGE *hge;
class AIc : public hgeGUIListbox
{
	private:
		char *list_type;
		char **res;
		int list_size;
		int id;
	public:
		AIc(char *t, hgeFont *_font, int _id, float _x, float _y, float _w, float _h) : hgeGUIListbox(_id, _x, _y, _w, _h, _font, 0xFFffff00, 0xFFffffff, 0x558B1A1A)
		{
			id = _id;
			list_type = t;
		}
		bool AIc_Initiate();
		bool AIc_Frame();
		bool AIc_Render();
		void AIc_Release();
		char *getPath(int _i);
};
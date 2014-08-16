#include "../../header.h"
#include "../../menuitem/menuitem.h"
#include "../../AIc/aic.h"
extern HGE *hge;
class box
{
	private:
		HTEXTURE tex;
		hgeFont *font;
		char *path;
		hgeQuad *bgQuad; //矩形框
		hgeGUI *gui;
		hgeGUIMenuItem *Item3, *Item4; //按钮
		AIc *list1, *list2, *list3; //列表
		char *list_path1, *list_path2, *list_path3;
	public:
		box(char *p);
		~box();
		bool box_Initiate();
		bool box_Frame(bool &ready);
		bool box_Render();
		void box_Release();
		bool getPath(char *m, char *a1, char *a2);
};

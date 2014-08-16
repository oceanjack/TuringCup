#include "../header.h"
extern HGE *hge;
class Skill
{
	private:
		int t; //运行时间
		
		HEFFECT tex;
		hgeSprite *spr;
		char *psi_path;
		hgeParticleSystem *par;
		int tt; //用于移动效果控制
		static const int speed = 4;
	public:
		int dt; //间隔调整
		bool move; //是否发生新的移动
		int id; //判断是否存在，1存在
		int who; //谁释放的技能
		int s; //秒数记录为分
		int x; //抽象坐标
		int y;
		int len;
		int cx[60]; //位置数组
		int cy[60];
		Skill(int _x, int _y, int _dir, char *_path, int _l);
		~Skill();
		bool Skill_Initiate();
		bool Skill_Frame();
		bool Skill_Render();
		void Skill_Release();
		void setID(); //设置id
		void setPath(int dir[]); //设置技能路径
};
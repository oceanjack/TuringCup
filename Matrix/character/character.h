#include "../header.h"
extern HGE *hge;
class Character
{
	private:
		int num; //图片截取位置
		int id; //动作次序
		char *path; //人物路径
		char *path2;
		HEFFECT texs[4 * 8]; //人物图像
		HEFFECT tex[5]; //人物状态图像
		hgeQuad *bgQuad[4 * 8]; //人物素材
		hgeQuad *Quad[5]; //人物图像
	public:
		//0吸星,1制幻,2吸月,3晕眩,4复活
		int state; //人物状态
		int state_t; //状态持续
		int small_skill_cd; //小技能cd
		int dead_cd; //自杀冷却
		int c_size;
		int len; //魔法波长
		int num_g;
		int goods[30];
		int move; //移动分解步数
		int dir; //方向1上2下3左4右
		int say;
		int say_time;
		float x; //实际坐标 
		float y;
		float dx; //目的目标
		float dy;
		float sx; //跨度
		float sy;
		int cx; //抽象坐标
		int cy;
		int speed; //速度
		float size;
		int sorce; //分
		int life; //命
		int death_time; //复活时间
		int round_time; //旋转
		bool no_show; //消失
		Character(char *p, char *p2, int _x, int _y);
		~Character();
		bool Character_Initiate();
		bool Character_Frame();
		bool Character_Render();
		void Character_Release();
		void setShi(); //设置不透明
		void setDan(int _v); //设置透明
		void setPos(int _x, int _y);
		void setC_Size(int _cs);
		bool ok();
};

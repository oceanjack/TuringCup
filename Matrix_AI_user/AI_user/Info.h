//方向
const int STOP = 0;
const int UP = 1;
const int DOWN = 2;
const int LEFT = 3;
const int RIGHT = 4;
const int ATC = 5; //攻击
//**复赛增加指令
const int ONE = 6; //小技能1
const int TWO = 7; //小技能2
const int THREE = 8; //小技能3
const int KILL = 9; //大招
const int DEAD = 44; //自杀，扣40分
//**

//指令
struct Order
{
	int act;
	int dir[30];
};

//边界
const int boderX = 22;
const int boderY = 17;

struct PlayerName
{
	char TeamName[50];		//队伍名称
	char my1Name[50];		//一号AI的名称
	char my2Name[50];		//二号AI的名称
};

//地图类型
const int STAR = 0; //不可走，魔法可穿
const int LAND = 1; //可走，魔法可穿
const int TREE = 2; //不可走，魔法不可穿

//物品类型
const int BLUE = 0; //加长魔法长度
const int BOMB = 1; //加30分
const int BOOK = 2; //加30能量

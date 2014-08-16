//物质
struct Goods
{
	int ID; //总编号
	int t; //出现时间
	int x; //出现坐标
	int y;
	int xx; //实际坐标
	int yy;
	int id; //物品编号
	int r_t; //生命时间
	bool free; //是否自由，1为自由
	bool eat; //是否被拿，1为被拿
	bool vis; //显示光环
	bool skill; //技能影响
	int first; //初次动画
	Goods() {};
	void set_Goods(int _ID, int _t, int _x, int _y, int _id, bool _f, bool _e, bool _v, int _fi)
	{
		ID = _ID;
		t = _t;
		x = _x;
		y = _y;
		id = _id;
		free = _f;
		eat = _e;
		vis =_v;
		first = _fi;
	}
};
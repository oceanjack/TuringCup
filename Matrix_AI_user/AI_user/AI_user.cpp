#include "stdafx.h"
#include "AI_user.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <bitset>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

extern "C" AI_USER_API PlayerName NameAI() {
	PlayerName temp;
	strcpy_s(temp.TeamName, "初赛小Boss");		//队名
	strcpy_s(temp.my1Name,  "南理");			//1号AI的名字
	strcpy_s(temp.my2Name,  "科协");			//2号AI的名字
	return temp;
}

#define firststart 0
#define emagic_len(x) (100+x)
#define ekill(x) (102+x)
#define killme(x, y) (104+x*2+y)
#define steptime 18
#define goods(x) (1+x)

const int inf = 0x13131313;	// 极大值
const int willkill = 0x01010101;
const int fx[5][2] = {{0,0},{-1,0},{1,0},{0,-1},{0,1}};
int mymap[boderY][boderX];
int emap[2][boderY][boderX];

// TODO: 在这里添加你的函数

// 坐标hash
int xy_hash(int y, int x) {
	return y * boderX + x;
}

// 最近吃到物品的时间
int get_time_(int x, int y, int t, User * u) {
	return max(mymap[y][x], (u->t - t + steptime - 1) / steptime);
}

// 预测攻击路径
void find_way(int who, User * u) {
	int maptmp[boderY][boderX];
	int eemax[50], eemay[50];
	int que[boderY * boderX][4], qn = 0;
	for ( int i=0; i<2; i++) {
		fill(maptmp[0], maptmp[boderY], 0);
		qn = 0;
		que[qn][0] = u->ey[who];
		que[qn][1] = u->ex[who];
		que[qn][2] = -1;
		que[qn++][3] = 0;
		bool get_break = false;
		for ( int j=0; j<qn; j++) {
			for ( int k=1; k<=4; k++) {
				int tmpy = que[j][0] + fx[k][0];
				int tmpx = que[j][1] + fx[k][1];
				if (tmpy >= 0 && tmpy < boderY && 
					tmpx >= 0 && tmpx < boderX && 
					maptmp[tmpy][tmpx] == 0 && u->mapInfo[tmpy][tmpx] != 2) {
					maptmp[tmpy][tmpx] = maptmp[que[j][0]][que[j][1]] + 1;
					que[qn][0] = tmpy;
					que[qn][1] = tmpx;
					que[qn][2] = j;
					que[qn++][3] = maptmp[tmpy][tmpx];
					if (xy_hash(tmpy, tmpx) == u->m[killme(who, i)]) {
						get_break = true;
						break;
					}
				}
			}
			if (get_break) {
				break;
			}
		}
		if (get_break) {
			int jj = qn-1;
			while (jj) {
				int tmpy = que[jj][0];
				int tmpx = que[jj][1];
				eemax[maptmp[tmpy][tmpx]-1] = tmpx;
				eemay[maptmp[tmpy][tmpx]-1] = tmpy;
				jj = que[jj][2];
			}
			for ( int j=que[qn-1][3]; j<u->m[emagic_len(who)]; j++) {
				bool flag = false;
				int yy = eemay[j-1] - eemay[j-2] + eemay[j-1];
				int xx = eemax[j-1] - eemax[j-2] + eemax[j-1];
				if (yy >= 0 && yy < boderY && 
					xx >= 0 && xx < boderX && 
					u->mapInfo[yy][xx] != 2) {
					eemay[j] = yy; eemax[j] = xx;
					continue;
				}
				for ( int k=1; k<=4; k++) {
					int tmpy = eemay[j-1] + fx[k][0];
					int tmpx = eemax[j-1] + fx[k][1];
					if (tmpy >= 0 && tmpy < boderY && 
						tmpx >= 0 && tmpx < boderX && 
						u->mapInfo[tmpy][tmpx] != 2 &&
						!(tmpy != eemay[j-2] && tmpx != eemax[j-2])) {
						eemay[j] = tmpy;
						eemax[j] = tmpx;
						break;
					}
				}
			}
		}
		bool yes = true;
		for ( int j=0; j<u->emagic_num[who]; j++) {
			if (u->emagic_x[who][j] == eemax[j] && u->emagic_y[who][j] == eemay[j]) {
			} else {
				yes = false;
				break;
			}
		}
		if (yes) {
			for ( int j=u->emagic_num[who]; j<u->m[emagic_len(who)]; j++) {
				u->emagic_y[who][j] = eemay[j];
				u->emagic_x[who][j] = eemax[j];
			}
			u->emagic_num[who] = u->m[emagic_len(who)];
			return;
		}
	}
}

// 第一遍预处理
void first_judge(User * u, int ymap[boderY][boderX], int x, int y, bool is_me = false) {
	if (is_me) {
		u->m[emagic_len(0)] = u->m[emagic_len(1)] = 8;
		for ( int i=0; i<2; i++) {
			for ( int j=0; j<u->egoods_num[i]; j++) {
				if (u->egoods_tpye[i][j] == BLUE) {
					u->m[emagic_len(i)] += 2;
				}
			}
			u->m[emagic_len(i)] = min(u->m[emagic_len(i)], 30);
		}
		for ( int i=0; i<2; i++) {
			if (u->emagic_num[i]) {
				if (~u->m[ekill(i)]) {
				} else {
					u->m[ekill(i)] = u->t;
					u->m[killme(i, 0)] = xy_hash(u->y[0], u->x[0]);
					u->m[killme(i, 1)] = xy_hash(u->y[1], u->x[1]);
				}
			} else {
				if (~u->m[ekill(i)]) {
					u->m[ekill(i)] = -1;
				} else {
				}
			}
		}
		if (~u->m[ekill(0)]) {
			find_way(0, u);
		}
		if (~u->m[ekill(1)]) {
			find_way(1, u);
		}
	}
	fill(ymap[0], ymap[boderY], inf);
	if (is_me) {
		for ( int i=0; i<u->magic_num[!u->who]; i++) {
			ymap[u->magic_y[!u->who][i]][u->magic_x[!u->who][i]] = willkill;
		//	u->mapInfo[u->magic_y[!u->who][i]][u->magic_x[!u->who][i]] = 0;
		}
		for ( int i=0; i<2; i++) {
			for ( int j=0; j<u->emagic_num[i]; j++) {
				ymap[u->emagic_y[i][j]][u->emagic_x[i][j]] = willkill;
			//	u->mapInfo[u->emagic_y[i][j]][u->emagic_x[i][j]] = 0;
			}
		}
	}
	ymap[y][x] = 0;
	int que[boderY * boderX][2], qn = 0;
	que[qn][0] = y; que[qn++][1] = x;
	for ( int i=0; i<qn; i++) {
		for ( int j=1; j<=4; j++) {
			int ty = que[i][0] + fx[j][0];
			int tx = que[i][1] + fx[j][1];
			if (u->mapInfo[ty][tx] == 1 && ymap[ty][tx] == inf) {
				ymap[ty][tx] = ymap[que[i][0]][que[i][1]] + 1;
				que[qn][0] = ty; que[qn++][1] = tx;
			}
		}
	}
}

// 击杀敌人
bool judge_kill(Order & tmp, User * u) {
	if (u->power[u->who] < 10 || u->magic_num[u->who]) {
		return false;
	}
	tmp.act = ATC;
	int maptmp[boderY][boderX];
	int que[boderY * boderX][4], qn;
	for ( int i=0; i<2; i++) {
		fill(maptmp[0], maptmp[boderY], 0);
		qn = 0;
		que[qn][0] = u->y[u->who];
		que[qn][1] = u->x[u->who];
		que[qn][2] = -1;
		que[qn++][3] = 0;
		for ( int j=0; j<qn; j++) {
			for ( int k=1; k<=4; k++) {
				int tmpy = que[j][0] + fx[k][0];
				int tmpx = que[j][1] + fx[k][1];
				if (tmpy >= 0 && tmpy < boderY && 
					tmpx >= 0 && tmpx < boderX &&
					maptmp[tmpy][tmpx] == 0 && u->mapInfo[tmpy][tmpx] != 2) {
					maptmp[tmpy][tmpx] = maptmp[que[j][0]][que[j][1]] + 1;
					que[qn][0] = tmpy;
					que[qn][1] = tmpx;
					que[qn][2] = j;
					que[qn++][3] = k;
					if (tmpy == u->ey[i] && tmpx == u->ex[i]) {
						break;
					}
				}
			}
			if (maptmp[u->ey[i]][u->ex[i]]) {
				break;
			}
		}
		srand(time(0));
		int tu = rand() & 1;
		if (maptmp[u->ey[i]][u->ex[i]] <= u->magic_len[u->who]-tu && 
			maptmp[u->ey[i]][u->ex[i]] <= 15) {
			int j = qn-1;
			while (j) {
				int tmpy = que[j][0];
				int tmpx = que[j][1];
				tmp.dir[maptmp[tmpy][tmpx]-1] = que[j][3];
				j = que[j][2];
			}
			int fic = que[qn-1][3], tx = u->ex[i], ty = u->ey[i];
			for ( int j=maptmp[u->ey[i]][u->ex[i]]; j<u->magic_len[u->who]; j++) {
				int tmy = ty + fx[fic][0];
				int tmx = tx + fx[fic][1];
				if (tmy >= 0 && tmy < boderY && 
					tmx >= 0 && tmx < boderX && 
					u->mapInfo[tmy][tmx] != 2) {
					ty = tmy; tx = tmx;
					tmp.dir[j] = fic;
					continue;
				}
				for ( int k=1; k<=4; k++) {
					tmy = ty + fx[k][0];
					tmx = tx + fx[k][1];
					if (tmy >= 0 && tmy < boderY && 
						tmx >= 0 && tmx < boderX && 
						u->mapInfo[tmy][tmx] != 2) {
						ty = tmy; tx = tmx;
						tmp.dir[j] = fic = k;
						break;
					}
				}
			}
			return true;
		}
	}
	return false;
}

/**************************官方函数****************************/
// 寻路，传入坐标(x,y)
Order goto_des(int x, int y, User * u) {
	if (x == u->x[u->who] && y == u->y[u->who]) {
		Order o; o.act = STOP;
		return o;
	}
	int maptmp[boderY][boderX];
	int que[boderY * boderX][2], qn=0;
	Order ord[5];
	ord[0].act = STOP; ord[1].act = UP;
	ord[2].act = DOWN; ord[3].act = LEFT;
	ord[4].act = RIGHT;
	fill(maptmp[0], maptmp[boderY], inf);
	maptmp[y][x] = 0;
	que[qn][0] = y;
	que[qn++][1] = x;
	for ( int i=0; i<qn; i++) {
		int qu[4][2], nn = 0;
		for ( int j=1; j<=4; j++) {
			int tmpy = que[i][0] + fx[j][0];
			int tmpx = que[i][1] + fx[j][1];
			if (tmpy >= 0 && tmpy < boderY && 
				tmpx >= 0 && tmpx < boderX && 
				u->mapInfo[tmpy][tmpx] == 1 && maptmp[tmpy][tmpx] == inf) {
				maptmp[tmpy][tmpx] = maptmp[que[i][0]][que[i][1]] + 1;
				qu[nn][0] = tmpy;
				qu[nn++][1] = tmpx;
			}
		}
		for ( int j=0; j<nn; j++) {
			if (mymap[qu[j][0]][qu[j][1]] != willkill) {
				que[qn][0] = qu[j][0];
				que[qn++][1] = qu[j][1];
				if (maptmp[u->y[u->who]][u->x[u->who]] != inf) {
					break;
				}
			}
		}
		if (maptmp[u->y[u->who]][u->x[u->who]] != inf) {
			break;
		}
		for ( int j=0; j<nn; j++) {
			if (mymap[qu[j][0]][qu[j][1]] == willkill) {
				que[qn][0] = qu[j][0];
				que[qn++][1] = qu[j][1];
				if (maptmp[u->y[u->who]][u->x[u->who]] != inf) {
					break;
				}
			}
		}
		if (maptmp[u->y[u->who]][u->x[u->who]] != inf) {
			break;
		}
	}
	for ( int i=1; i<=4; i++) {
		int tmpy = u->y[u->who] + fx[i][0];
		int tmpx = u->x[u->who] + fx[i][1];
		if (tmpx >= 0 && tmpx < boderX && 
			tmpy >= 0 && tmpy < boderY && 
			mymap[tmpy][tmpx] != willkill &&
			maptmp[tmpy][tmpx] + 1 == maptmp[u->y[u->who]][u->x[u->who]]) {
			return ord[i];
		}
	}
	for ( int i=1; i<=4; i++) {
		int tmpy = u->y[u->who] + fx[i][0];
		int tmpx = u->x[u->who] + fx[i][1];
		if (tmpx >= 0 && tmpx < boderX && 
			tmpy >= 0 && tmpy < boderY && 
			maptmp[tmpy][tmpx] + 1 == maptmp[u->y[u->who]][u->x[u->who]]) {
			return ord[i];
		}
	}
	return ord[0];
}

Order goto_des2(int x, int y, User * u) {
	if (x == u->x[u->who] && y == u->y[u->who]) {
		Order o; o.act = STOP;
		return o;
	}
	int maptmp[boderY][boderX];
	int que[boderY * boderX][2], qn=0;
	Order ord[5];
	ord[0].act = STOP; ord[1].act = UP;
	ord[2].act = DOWN; ord[3].act = LEFT;
	ord[4].act = RIGHT;
	fill(maptmp[0], maptmp[boderY], inf);
	maptmp[y][x] = 0;
	que[qn][0] = y;
	que[qn++][1] = x;
	for ( int i=0; i<qn; i++) {
		int qu[4][2], nn = 0;
		for ( int j=1; j<=4; j++) {
			int tmpy = que[i][0] + fx[j][0];
			int tmpx = que[i][1] + fx[j][1];
			if (tmpy >= 0 && tmpy < boderY && 
				tmpx >= 0 && tmpx < boderX && 
				mymap[tmpy][tmpx] != willkill && 
				u->mapInfo[tmpy][tmpx] == 1 && maptmp[tmpy][tmpx] == inf) {
				maptmp[tmpy][tmpx] = maptmp[que[i][0]][que[i][1]] + 1;
				qu[nn][0] = tmpy;
				qu[nn++][1] = tmpx;
			}
		}
		for ( int j=0; j<nn; j++) {
			if (mymap[qu[j][0]][qu[j][1]] != willkill) {
				que[qn][0] = qu[j][0];
				que[qn++][1] = qu[j][1];
				if (maptmp[u->y[u->who]][u->x[u->who]] != inf) {
					break;
				}
			}
		}
		if (maptmp[u->y[u->who]][u->x[u->who]] != inf) {
			break;
		}
		for ( int j=0; j<nn; j++) {
			if (mymap[qu[j][0]][qu[j][1]] == willkill) {
				que[qn][0] = qu[j][0];
				que[qn++][1] = qu[j][1];
				if (maptmp[u->y[u->who]][u->x[u->who]] != inf) {
					break;
				}
			}
		}
		if (maptmp[u->y[u->who]][u->x[u->who]] != inf) {
			break;
		}
	}
	for ( int i=1; i<=4; i++) {
		int tmpy = u->y[u->who] + fx[i][0];
		int tmpx = u->x[u->who] + fx[i][1];
		if (tmpx >= 0 && tmpx < boderX && 
			tmpy >= 0 && tmpy < boderY && 
			mymap[tmpy][tmpx] != willkill &&
			maptmp[tmpy][tmpx] + 1 == maptmp[u->y[u->who]][u->x[u->who]]) {
			return ord[i];
		}
	}
	for ( int i=1; i<=4; i++) {
		int tmpy = u->y[u->who] + fx[i][0];
		int tmpx = u->x[u->who] + fx[i][1];
		if (tmpx >= 0 && tmpx < boderX && 
			tmpy >= 0 && tmpy < boderY && 
			maptmp[tmpy][tmpx] + 1 == maptmp[u->y[u->who]][u->x[u->who]]) {
			return ord[i];
		}
	}
	return ord[0];
}

// 直线攻击
Order line_kill(int fictions, User * u) {
	Order ord; ord.act = ATC;
	for ( int i=0; i<u->magic_len[u->who]; i++) {
		ord.dir[i] = fictions % 5;
	}
	return ord;
}
/**********************官方代码********************************/

extern "C" AI_USER_API Order get_order(User *u) {
	// TODO: 在这里添加你的代码
	Order tmp; tmp.act = STOP;
	if (~u->m[firststart]) {
		memset(u->m, -1, sizeof(u->m));
	}
	first_judge(u, emap[0], u->ex[0], u->ey[0], false);
	first_judge(u, emap[1], u->ex[1], u->ey[1], false);
	first_judge(u, mymap, u->x[u->who], u->y[u->who], true);
	int step = inf, x = -1, y = -1;
	if (judge_kill(tmp, u)) {
		return tmp;
	}
	for ( int i=0; i<u->map_goods; i++) {
		int tmp = get_time_(u->map_goods_x[i], u->map_goods_y[i], u->map_goods_time[i]*100, u);
		if (mymap[u->map_goods_y[i]][u->map_goods_x[i]] != willkill && 
			step > tmp) {
			if (u->who && u->m[goods(0)] == xy_hash(u->map_goods_y[i], u->map_goods_x[i])) {
				continue;
			}
			step = tmp;
			x = u->map_goods_x[i];
			y = u->map_goods_y[i];
		}
	}
	if (!u->who) {
		u->m[goods(0)] = xy_hash(y, x);
	}
	if (step != inf) {
		tmp = goto_des2(x, y, u);
		if (tmp.act == STOP) {
			tmp = goto_des(x, y, u);
		}
	} else {
		tmp = goto_des2(u->ex[u->who], u->ey[u->who], u);
		if (tmp.act == STOP) {
			tmp = goto_des(x, y, u);
		}
	}
	/*
	if(u->t < 1000 && u->m[199] == -1) {
		srand(time(NULL));
		tmp.act = 2012;
		u->m[199] = 1;
		return tmp;
	}*/
	return tmp;
}

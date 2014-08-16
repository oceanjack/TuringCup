#include "stdafx.h"
#include "Api.h"

int mapForWalk[boderY*boderX][boderY*boderX];
int mapForAttack[boderY*boderX][boderY*boderX];
int safePositionCount;
int safePosition[boderY*boderX];

struct Path
{
	int length;
	int nextDirection[boderX*boderY];
};

int ConvertCoordinateToIndex(int x,int y)
{
	return y*boderX+x;
}

void SetUndirectedEdge(int map[boderX*boderY][boderX*boderY],int x,int y)
{
	map[x][y]=1;
	map[y][x]=1;
}

void BuildMapForWalk(User *user)
{
	int i,j,x,y;
	if(mapForWalk[0][0]!=0)
	{
		return;
	}
	for(i=0;i<boderX*boderY;i++)
	{
		for(j=0;j<boderX*boderY;j++)
		{
			mapForWalk[i][j]=100;
		}
	}
	for(i=0;i<boderX*boderY;i++)
	{
		int current;
		x=i%boderX;
		y=i/boderX;
		current=user->mapInfo[y][x];
		if(current==LAND)
		{
			int degree=0;
			if(x>0)
			{
				int left;
				left=user->mapInfo[y][x-1];
				if(left==LAND)
				{
					int v1,v2;
					v1=ConvertCoordinateToIndex(x-1,y);
					v2=ConvertCoordinateToIndex(x,y);
					SetUndirectedEdge(mapForWalk,v1,v2);
					degree++;
				}
			}
			if(y>0)
			{
				int up;
				up=user->mapInfo[y-1][x];
				if(up==LAND)
				{
					int v1,v2;
					v1=ConvertCoordinateToIndex(x,y-1);
					v2=ConvertCoordinateToIndex(x,y);
					SetUndirectedEdge(mapForWalk,v1,v2);
					degree++;
				}
			}
			if(x<boderX-1)
			{
				int right;
				right=user->mapInfo[y][x+1];
				if(right==LAND)
				{
					int v1,v2;
					v1=ConvertCoordinateToIndex(x+1,y);
					v2=ConvertCoordinateToIndex(x,y);
					SetUndirectedEdge(mapForWalk,v1,v2);
					degree++;
				}
			}
			if(y<boderY-1)
			{
				int down;
				down=user->mapInfo[y+1][x];
				if(down==LAND)
				{
					int v1,v2;
					v1=ConvertCoordinateToIndex(x,y+1);
					v2=ConvertCoordinateToIndex(x,y);
					SetUndirectedEdge(mapForWalk,v1,v2);
					degree++;
				}
			}
			if(degree>2)
			{
				safePosition[safePositionCount]=i;
				safePositionCount++;
			}
		}
	}
}

void BuildMapForAttack(User *user)
{
	int i,j,x,y;
	if(mapForAttack[0][0]!=0)
	{
		return;
	}
	for(i=0;i<boderX*boderY;i++)
	{
		for(j=0;j<boderX*boderY;j++)
		{
			mapForAttack[i][j]=100;
		}
	}
	for(i=0;i<boderX*boderY;i++)
	{
		int current;
		x=i%boderX;
		y=i/boderX;
		current=user->mapInfo[y][x];
		if(current==LAND || current==STAR)
		{
			if(x>0)
			{
				int left;
				left=user->mapInfo[y][x-1];
				if(left==LAND || left==STAR)
				{
					int v1,v2;
					v1=ConvertCoordinateToIndex(x-1,y);
					v2=ConvertCoordinateToIndex(x,y);
					SetUndirectedEdge(mapForAttack,v1,v2);
				}
			}
			if(y>0)
			{
				int up;
				up=user->mapInfo[y-1][x];
				if(up==LAND || up==STAR)
				{
					int v1,v2;
					v1=ConvertCoordinateToIndex(x,y-1);
					v2=ConvertCoordinateToIndex(x,y);
					SetUndirectedEdge(mapForAttack,v1,v2);
				}
			}
			if(x<boderX-1)
			{
				int right;
				right=user->mapInfo[y][x+1];
				if(right==LAND || right==STAR)
				{
					int v1,v2;
					v1=ConvertCoordinateToIndex(x+1,y);
					v2=ConvertCoordinateToIndex(x,y);
					SetUndirectedEdge(mapForAttack,v1,v2);
				}
			}
			if(y<boderY-1)
			{
				int down;
				down=user->mapInfo[y+1][x];
				if(down==LAND || down==STAR)
				{
					int v1,v2;
					v1=ConvertCoordinateToIndex(x,y+1);
					v2=ConvertCoordinateToIndex(x,y);
					SetUndirectedEdge(mapForAttack,v1,v2);
				}
			}
		}
	}
}

void InitializeMapForWalk(User *user)
{
	BuildMapForWalk(user);
}

void InitializeMapForAttack(User *user)
{
	BuildMapForAttack(user);
}

Path ShortestPath(int map[boderX*boderY][boderX*boderY],int source,int destination)
{
	int i,j,k;
	bool visited[boderX*boderY];
	int dist[boderX*boderY];
	int path[boderX*boderY];
	for(i=0;i<boderX*boderY;i++)
	{
		if(map[source][i]>0&&i!=source)
		{
			dist[i]=map[source][i];
			path[i]=source;
		}
		else
		{
			dist[i]=INT_MAX;
			path[i]=-1;
		}
		visited[i]=false;
		path[source]=source;
		dist[source]=0;
	}
	visited[source]=true;
	for(i=1;i<boderX*boderY;i++)
	{
		int min=INT_MAX;
		int u;
		for(j=0;j<boderX*boderY;j++)
		{
			if(visited[j]==false && dist[j]<min)
			{
				min=dist[j];
				u=j;        
			}
		} 
		visited[u]=true;
		for(k=0;k<boderX*boderY;k++)
		{
			if(visited[k]==false && map[u][k]>0 && min+map[u][k]<dist[k])
			{
				dist[k]=min+map[u][k];
				path[k]=u; 
			}
		}        
	}
	Path p;
	p.length=dist[destination];
	stack<int> s;
	int u=destination;
	while(u!=source)
	{
		s.push(u);
		u=path[u];
	}
	int pathIndex=0;
	int current=source;
	while(!s.empty())
	{
		int next=s.top();
		int currentX=current%boderX;
		int currentY=current/boderX;
		int nextX=next%boderX;
		int nextY=next/boderX;
		if(currentX-nextX==1 && currentY==nextY)
		{
			p.nextDirection[pathIndex]=LEFT;
		}
		else if(currentX-nextX==-1 && currentY==nextY)
		{
			p.nextDirection[pathIndex]=RIGHT;
		}
		else if(currentY-nextY==1 && currentX==nextX)
		{
			p.nextDirection[pathIndex]=UP;
		}
		else if(currentY-nextY==-1 && currentX==nextX)
		{
			p.nextDirection[pathIndex]=DOWN;
		}
		else
		{
			p.nextDirection[pathIndex]=STOP;
		}
		s.pop();
		pathIndex++;
		current=next;
	}
	return p;
}

Path PathBetween(int x1,int y1,int x2,int y2)
{
	int v1=ConvertCoordinateToIndex(x1,y1);
	int v2=ConvertCoordinateToIndex(x2,y2);
	return ShortestPath(mapForWalk,v1,v2);
}

Path MagicPathBetween(int x1,int y1,int x2,int y2)
{
	int v1=ConvertCoordinateToIndex(x1,y1);
	int v2=ConvertCoordinateToIndex(x2,y2);
	return ShortestPath(mapForAttack,v1,v2);
}

void InitializeMap(User *user)
{
	InitializeMapForWalk(user);
	InitializeMapForAttack(user);
}

void Initialize(User *user)
{
	InitializeMap(user);
}

int BuildAdditionalDirection(User *user,Path path,int targetX,int targetY)
{
	int lastDirection=path.nextDirection[path.length-1];
	bool candidate[4];
	if(targetX>0)
	{
		candidate[0]=user->mapInfo[targetY][targetX-1]==LAND;
		if(lastDirection==RIGHT)
		{
			candidate[0]=false;
		}
	}
	if(targetY>0)
	{				
		candidate[1]=user->mapInfo[targetY-1][targetX]==LAND;
		if(lastDirection==DOWN)
		{
			candidate[1]=false;
		}
	}
	if(targetX<boderX-1)
	{
		candidate[2]=user->mapInfo[targetY][targetX+1]==LAND;
		if(lastDirection==LEFT)
		{
			candidate[2]=false;
		}
	}
	if(targetY<boderY-1)
	{
		candidate[3]=user->mapInfo[targetY+1][targetX]==LAND;
		if(lastDirection==UP)
		{
			candidate[3]=false;
		}
	}
	srand(unsigned(time(0)));
	int select=rand()%4;
	if(candidate[select]==false)
	{
		select=(select+1)%4;
	}
	return candidate[select];
}

Order Attack(User *user)
{
	int who=user->who;
	int oneX,oneY,twoX,twoY;
	int selfX,selfY;
	selfX=user->x[who];
	selfY=user->y[who];
	oneX=user->ex[0];
	oneY=user->ey[0];
	twoX=user->ex[1];
	twoY=user->ey[1];
	Path one=MagicPathBetween(selfX,selfY,oneX,oneY);
	Path two=MagicPathBetween(selfX,selfY,twoX,twoY);
	if(one.length<user->magic_len[user->who]-1)
	{
		Order order;
		memset(&order,0,sizeof(order));
		order.act=ATC;
		int i;
		for(i=0;i<one.length;i++)
		{
			order.dir[i]=one.nextDirection[i];
		}
		order.dir[i]=BuildAdditionalDirection(user,one,oneX,oneY);
		return order;
	}
	if(two.length<user->magic_len[user->who]-1)
	{
		Order order;
		memset(&order,0,sizeof(order));
		order.act=ATC;
		int i;
		for(i=0;i<two.length;i++)
		{
			order.dir[i]=two.nextDirection[i];
		}
		order.dir[i]=BuildAdditionalDirection(user,two,twoX,twoY);
		return order;
	}
	Order order;
	memset(&order,0,sizeof(order));
	order.act=STOP;
	return order;
}

Order FindItem(User *user)
{
	if(user->map_goods>0)
	{
		int targetX,targetY;
		int selfX,selfY;
		targetX=user->map_goods_x[user->who%user->map_goods];
		targetY=user->map_goods_y[user->who%user->map_goods];
		selfX=user->x[user->who];
		selfY=user->y[user->who];
		Path path=PathBetween(selfX,selfY,targetX,targetY);
		Order order;
		memset(&order,0,sizeof(order));
		order.act=path.nextDirection[0];
		return order;
	}
	Order order;
	memset(&order,0,sizeof(order));
	order.act=STOP;
	return order;
}

Order Dodge(User *user)
{
	int selfX=user->x[user->who];
	int selfY=user->y[user->who];
	int targetOneX=user->ex[0];
	int targetOneY=user->ey[0];
	int targetTwoX=user->ex[0];
	int targetTwoY=user->ey[0];
	bool attackOne=user->emagic_num[0]>0;
	bool attackTwo=user->emagic_num[1]>0;
	Path one=MagicPathBetween(targetOneX,targetOneY,selfX,selfY);
	Path two=MagicPathBetween(targetTwoX,targetTwoY,selfX,selfY);
	if(attackOne==true)
	{
		Order order;
		memset(&order,0,sizeof(order));
		order.act=BuildAdditionalDirection(user,one,selfX,selfY);
		return order;
	}
	if(attackTwo==true)
	{
		Order order;
		memset(&order,0,sizeof(order));
		order.act=BuildAdditionalDirection(user,two,selfX,selfY);
		return order;
	}
	Order order;
	memset(&order,0,sizeof(order));
	order.act=STOP;
	return order;
}

bool IsSafe(User *user,int x,int y)
{
	int current,degree=0;
	current=user->mapInfo[y][x];
	if(x>0)
	{
		int left;
		left=user->mapInfo[y][x-1];
		if(left==LAND)
		{
			degree++;
		}
	}
	if(y>0)
	{
		int up;
		up=user->mapInfo[y-1][x];
		if(up==LAND)
		{
			degree++;
		}
	}
	if(x<boderX-1)
	{
		int right;
		right=user->mapInfo[y][x+1];
		if(right==LAND)
		{
			degree++;
		}
	}
	if(y<boderY-1)
	{
		int down;
		down=user->mapInfo[y+1][x];
		if(down==LAND)
		{
			degree++;
		}
	}
	if(degree>2)
	{
		return true;
	}
	return false;
}

Order FindSafePlace(User *user)
{
	int selfX=user->x[user->who];
	int selfY=user->y[user->who];
	int select;
	if(!IsSafe(user,selfX,selfY))
	{
		srand(unsigned(time(0)));
		select=rand()%safePositionCount;
		int x=safePosition[select]%boderX;
		int y=safePosition[select]/boderX;
		Path path=PathBetween(selfX,selfY,x,y);
		Order order;
		memset(&order,0,sizeof(order));
		order.act=path.nextDirection[0];
		return order;
	}
	Order order;
	memset(&order,0,sizeof(order));
	order.act=STOP;
	return order;
}

int GetMagicLength(User *user,int id)
{
	int originalLength=8;
	int blueCount=0;
	int goodsCount=user->egoods_num[id];
	int i;
	for(i=0;i<goodsCount;i++)
	{
		if(user->egoods_tpye[id][i]==BLUE)
		{
			blueCount++;
		}
	}
	return originalLength+blueCount*2;
}

Order OutOfRange(User *user)
{
	int selfX=user->x[user->who];
	int selfY=user->y[user->who];
	int targetOneX=user->ex[0];
	int targetOneY=user->ey[0];
	int targetTwoX=user->ex[0];
	int targetTwoY=user->ey[0];
	int magicLengthOne=GetMagicLength(user,0);
	int magicLengthTwo=GetMagicLength(user,1);
	Path one=MagicPathBetween(targetOneX,targetOneY,selfX,selfY);
	Path two=MagicPathBetween(targetTwoX,targetTwoY,selfX,selfY);
	if(magicLengthOne>one.length)
	{
		Order order;
		memset(&order,0,sizeof(order));
		order.act=BuildAdditionalDirection(user,one,selfX,selfY);
		return order;
	}
	if(magicLengthTwo>two.length)
	{
		Order order;
		memset(&order,0,sizeof(order));
		order.act=BuildAdditionalDirection(user,two,selfX,selfY);
		return order;
	}
	Order order;
	memset(&order,0,sizeof(order));
	order.act=STOP;
	return order;
}

Order Driver(User *user)
{
	Order order;
	order=Attack(user);
	if(order.act==STOP)
	{
		order=Dodge(user);
		if(order.act==STOP)
		{
			order=FindItem(user);
			if(order.act==STOP)
			{
				order=OutOfRange(user);
				if(order.act==STOP)
				{
					order=FindSafePlace(user);
				}
			}
		}
	}
	return order;
}

extern "C" AI_USER_API PlayerName NameAI() {
	PlayerName temp;
	strcpy_s(temp.TeamName, "·ÐÌÚ°ÉÇò");
	strcpy_s(temp.my1Name,  "·ÐÌÚµÄÇò");
	strcpy_s(temp.my2Name,  "·ÐÌÚµÄ²éÀí");
	return temp;
}

extern "C" AI_USER_API Order get_order(User *user) {
	Initialize(user);
	return Driver(user);
}
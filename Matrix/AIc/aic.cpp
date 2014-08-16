#include "./aic.h"
#include <string.h>
bool AIc::AIc_Initiate()
{
	//初始化
	CFindFile cff;
	list_size = 0;
	res = new char*[50];
	//读取文件
	for(cff.First(list_type); cff.IsOK(); cff.Next())
	{
		res[list_size] = new char[50];
		strcpy(res[list_size], cff.GetCurFile().cFileName);
		if(++list_size >= 50)
			break;
	}
	//添加文件
	for(int i = 0; i < list_size; ++i)
		this->AddItem(res[i]);
	return true;
}
bool AIc::AIc_Frame()
{
	return false;
}
bool AIc::AIc_Render()
{
	return false;
}
void AIc::AIc_Release()
{
	if(res)
	{
		for(int i = 0; i < list_size; ++i)
			delete []res[i];
		delete []res;
		res = 0;
	}
}
char *AIc::getPath(int _i)
{
	if(list_size == 0)
		return NULL;
	else
		return res[_i];
}
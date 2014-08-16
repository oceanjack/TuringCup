#pragma once 
#include "windows.h"
// 遍历一个目录的文件。 
/*
lpFileName 是目录名称。一般使用通配符。 
lpFindFileData 是找到的文件或目录属性。 
hFindFile 是下一个文件或目录的句柄。 
*/
class CFindFile 
{
	public: 
		CFindFile(void);
		~CFindFile(void);
		//找到第一个文件。 
		void First(LPCTSTR lpFileName) ;
		//查找一下文件。 
		void Next(void) ;
		//是否可以查找一下文件。 
		bool IsOK(void) const ;
		//返回当前文件的属性。 
		const WIN32_FIND_DATA& GetCurFile(void) ;
	protected: 
		HANDLE m_hFind; //保存当查找的位置句柄。 
		bool m_bFound; //当前查找是否成功。 
		WIN32_FIND_DATA m_FindFileData; //保存当前文件的属性。 
}; 

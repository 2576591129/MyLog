#include <fstream>
#include <Windows.h>
#include <process.h>
#include <iostream>
#include <string>
#include "shlwapi.h"  
#pragma comment(lib,"shlwapi.lib") 
using namespace std;

#define TIME_LEN     50
#define MESSAGE_LEN  250
#define LOG_LEV_LEN 10
class LOG
{
private:
	SYSTEMTIME s_nowtime;
	string string_time;
	char Level[LOG_LEV_LEN];
	char Message[MESSAGE_LEN];
	CRITICAL_SECTION g_cs;
	void * pdelete;

public:
	LOG(LPCTSTR  strpath = L"Mylog\\")
	{
		//----------------------------------------------初始化数据----------------------------------------------
		pdelete = 0;
		ZeroMemory(Message,MESSAGE_LEN);
		ZeroMemory(Level,LOG_LEV_LEN);
		InitializeCriticalSection(&g_cs);

		//----------------------------------------------创建文件夹----------------------------------------------
		if (!PathIsDirectory( strpath) )
			::CreateDirectory(strpath, 0);  
		//---------------------------------------------宽字符与char* 的转换------------------------------		
		char path[TIME_LEN] ={0} ;
		strcpy(path,WcharToChar(strpath));

		//---------------------------------------------设置文件名-----------------------------------------------	
		GetLocalTime(&s_nowtime);	
		string_time += path;//文件夹									
		string_time += IntToChar(s_nowtime.wYear);   //年
		string_time+="_";
		string_time += IntToChar(s_nowtime.wMonth); //月
		string_time+="_";
		string_time += IntToChar(s_nowtime.wDay);
		string_time +=".log";  //后缀名
	}

	~LOG()
	{
		Destory(nullptr);
	}

	void WriteLog(int Level,char * fun ,char * Message,...)
	{
		GetLevel(Level);
		GetLocalTime(&s_nowtime);	

		char  buff[1024] ={0};
		va_list args;
		va_start(args, Message);
		int nSize = vsnprintf( buff, sizeof(buff) -1, Message, args);
		char *buff2 = new char[nSize+1];
		Destory(buff2);
		buff2 = buff;

		strcpy_s(this->Message,MESSAGE_LEN,buff2);

		EnterCriticalSection(&g_cs);
		fstream fs(string_time,fstream::out |fstream::app | fstream::in);
		fs<<"["<<s_nowtime.wYear<<"/"<<s_nowtime.wMonth<<"/"<<s_nowtime.wDay<<" "<<s_nowtime.wHour<<":"<<s_nowtime.wMinute<<":"<<s_nowtime.wSecond<<"."<<s_nowtime.wMilliseconds<<"] "
			<<"["<<fun<<"] "<<"["<<this->Level<<"] "<<"["<<this->Message<<"]"<<endl;
		if (fs.is_open())
			fs.close();
		LeaveCriticalSection(&g_cs);
	}
	//为了兼容更多的语言, 支持 多字节与宽字节的相互转换,并且输入参数, 就能直接用返回值
	char *  WcharToChar(LPCTSTR widestr)
	{
		int num = WideCharToMultiByte(CP_OEMCP,NULL,widestr,-1,NULL,0,NULL,FALSE);
		char *pchar = new char[num];
		Destory(pchar);
		WideCharToMultiByte (CP_OEMCP,NULL,widestr,-1,pchar,num,NULL,FALSE);
		return pchar;
	}
	LPCWSTR CharToWchar(char* ch)
	{
		int num = MultiByteToWideChar(0,0,ch,-1,NULL,0);
		wchar_t *wide = new wchar_t[num];
		Destory(wide);
		MultiByteToWideChar(0,0,ch,-1,wide,num);
		return wide;
	}
	char * IntToChar(WORD num)
	{
		char *temp_buf = new char [TIME_LEN];
		Destory(temp_buf);
		_itoa(num,temp_buf,10);
		return temp_buf;
	}
private:

	void GetLevel(int Level)
	{
		switch (Level)
		{
		case 1 : strcpy_s(this->Level,LOG_LEV_LEN,"INFO");break;
		case 0: strcpy_s(this->Level,LOG_LEV_LEN,"WORNNING");break;
		case -1: strcpy_s(this->Level,LOG_LEV_LEN,"ERROR");break;
		}
	}
	void Destory(void * temp)
	{
		if (!pdelete)
			delete(pdelete);
		pdelete = temp;
	}
	char * FormatOutput(LPCSTR formatstring, ...) 
	{
		char  buff[1024] ={0};
		va_list args;
		va_start(args, formatstring);
		int nSize = vsnprintf( buff, sizeof(buff) -1, formatstring, args);
		char *buff2 = new char[nSize+1];
		Destory(buff2);
		buff2 = buff;
		return buff;
	}

};

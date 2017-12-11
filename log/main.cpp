#include <iostream>
#include "log.h"
#include <string>
#include<list>
using namespace std;

char *xm_vsprintf_ex(char *fmt, ... )  
{  
	va_list ap;  
	char *ptr = NULL;  

	ptr = (char *)malloc(0);  

	va_start(ap, fmt);  
	memcpy(ptr,fmt,1);
	vsprintf(ptr,fmt, ap);  
	va_end(ap);  



	return ptr;  
} 

void fun(int a, ...)
{
	int *temp = &a;
	temp++;
	temp ++;
	for (int i = 0; i < a-1; ++i)
	{
		cout << *temp << endl;
		temp++;
	}
}

int demo( char *fmt, ... )    
{    
	va_list argp;    
	int argno = 0;     
	char *para;    
	va_start(argp, fmt);    
	while (1)    
	{    
		para = va_arg(argp, char *);    
		if (strcmp( para, "") == 0)    
			break;    
		printf("Parameter #%d is: %s\n", argno, para);    
		argno++;    
	}    
	va_end( argp );    
	return 0;    
}  

void fun(char * str,...)
{
	va_list arg;
	va_start(arg,str);
	char** para = &str;
	while (*para !=NULL)
	{

		printf("%s\n",*para);
		para++;
	}



}

int foo(char *fmt, ...)
{
	va_list ap;
	int d;
	char c, *s;
	va_start(ap, fmt);
	while (*fmt)
		switch(*(fmt++))
	{
		case 's':           /* string */
			{
				s = va_arg(ap, char *);
				printf("string %s\n", s);
			}
			break;
		case 'd':           /* int */
			{
				d = va_arg(ap, int);
				printf("int %d\n", d);
			}
			break;
		case 'c':           /* char */
			{
				/* need a cast here since va_arg only
				takes fully promoted types */
				c = (char) va_arg(ap, int);
				printf("char %c\n", c);
			}
			break;
	}
	va_end(ap);
	return 0;
}

char * FormatOutput(LPCSTR formatstring, ...) 
{
	int num =10;
	char  buff[1024] ={0};
	//memset(buff, 0, sizeof(buff));
	va_list args;
	va_start(args, formatstring);
	int nSize = vsnprintf( buff, sizeof(buff) -1, formatstring, args); // C4996
	// Note: vsnprintf is deprecated; consider vsnprintf_s instead
	char *buff2 = new char[nSize+1];
	buff2 = buff;
	return buff;
}

int main()
{
	/*cout<<FormatOutput("%s%s%d", "dfg","hello",100)<<endl;*/

	LOG log;
	int a = 100;
	char *str = "hello";
	log.WriteLog(-1,__FUNCTION__,"jucheng %s %d ",str,a);
	

	system("pause");
	return 0;

}



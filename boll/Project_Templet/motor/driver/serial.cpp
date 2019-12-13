#include "serial.h"
#include <iostream>
using namespace std;

const Serial::SerialOptions Serial::defaultoptions= //默认选择
{
	"COM6",
	//"COM10",
	115200,
	8,			//字节长度
	NOPARITY,	//奇偶校验
	ONESTOPBIT,	//停止位1个。这里0对应停止位1  0,1,2 == 1,1.5,2
};
 Serial::Serial(std::string comname,DWORD	BaudRate)
 {
	 serialoptions = defaultoptions;
	 serialoptions.serialname = comname;
	 serialoptions.BaudRate = BaudRate;
	 serialstatus = openPort();
}

bool Serial::openPort()
{
	std::string com_port ="\\\\.\\"+serialoptions.serialname;/*打开窗口24   \\.\  */
	//std::string com_port =".\\"+serialoptions.serialname;//*打开窗口24   .\ */
   	_hcom = CreateFile
	(
		com_port.c_str(),			//指向文件名的指针
		GENERIC_READ|GENERIC_WRITE, //访问模式（读 / 写）
		0,							 //共享模式  如果是0表示不共享；是FILE_SHARE_READ和/或FILE_SHARE_WRITE表示允许对文件进行共享；
		NULL,						//指向安全属性的指针   
		OPEN_EXISTING,				 //如何让创建 指定当文件存在或不存在时的操作。常见的操作有5种 1.OPEN_EXISTING-文件已经存在.....
		FILE_ATTRIBUTE_NORMAL,		//文件属性  FILE_ATTRIBUTE_NORMAL：默认属性
		NULL						//用于复制文件句柄   用于存储的文件句柄；如果不为0，则指定一个文件句柄，新的文件将从这个文件中复制扩展属性；
	);
	if(_hcom==INVALID_HANDLE_VALUE)
	{
		std::cout<<"CreateFile false"<<endl;
		return false;
	}
	SetupComm(_hcom,8,8); 
	COMMTIMEOUTS TimeOuts;
	TimeOuts.ReadTotalTimeoutConstant=200;
	TimeOuts.WriteTotalTimeoutMultiplier=500;
	TimeOuts.WriteTotalTimeoutConstant=2000;
	if(SetCommTimeouts(_hcom,&TimeOuts)==false)
	{
		std::cout<<"SetCommTimeouts false"<<endl;
		return false;
	}
	
	if(GetCommState(_hcom,&_serialoptions)==false)
	{
		std::cout<<"GetCommState false"<<endl;
		return false;
	}
	
	_serialoptions.BaudRate=serialoptions.BaudRate;
	_serialoptions.ByteSize=serialoptions.ByteSize; 
	_serialoptions.Parity=serialoptions.Parity; 
	_serialoptions.StopBits=serialoptions.StopBits; 	
	if(SetCommState(_hcom,&_serialoptions)==false)//该函数初始化一个指定的通信设备的通信参数。
	{
		std::cout<<"SetCommState false"<<endl;
		return false;
	}	
	if(PurgeComm(_hcom,PURGE_TXCLEAR|PURGE_RXCLEAR)==false)//清空缓冲区    
														//PURGE_TXABORT 终止所有正在进行的字符输出操作,完成一个正处于等待状态的重叠i/o操作,他将产生一个事件,指明完成了写操作
														//PURGE_RXABORT 终止所有正在进行的字符输入操作,完成一个正在进行中的重叠i/o操作,并带有已设置得适当事件
	{
		std::cout<<"PurgeComm false"<<endl;
		return false;
	}	
   	return true;
}

int Serial::readFromPort(void*buff,DWORD size,unsigned int timeout)
{
   COMMTIMEOUTS readCommTimeOuts;
   memset(&readCommTimeOuts, 0,sizeof(readCommTimeOuts));
   readCommTimeOuts.ReadTotalTimeoutConstant=timeout;
   SetCommTimeouts(_hcom,&readCommTimeOuts);
   DWORD dwReadBytes = 0;
   if (ReadFile(_hcom,buff,size, &dwReadBytes,NULL))
   {
		return dwReadBytes;
   }
   else
   {
		//std::cout<<"ReadFile false"<<endl;
       	return -1;
   }
}

bool Serial::writeToPort(const void*buff,DWORD size,unsigned int timeout)
{
   COMMTIMEOUTS writeCommTimeOuts;
   memset(&writeCommTimeOuts, 0,sizeof(writeCommTimeOuts));
   writeCommTimeOuts.WriteTotalTimeoutConstant=timeout;
   SetCommTimeouts(_hcom,&writeCommTimeOuts);
   DWORD dwWriteBytes = 0;
   if (WriteFile(_hcom,buff,size, &dwWriteBytes,NULL))
   {
       	return true;
   }
   else
   {
		//std::cout<<"ReadFile false"<<endl;
       	return false;
   }
}

bool Serial::closePort()
{
  	if (_hcom!= INVALID_HANDLE_VALUE)
	{
		return CloseHandle(_hcom);
    }
  	return false;
}


bool Serial::is_open() 
{
	return serialstatus;
}

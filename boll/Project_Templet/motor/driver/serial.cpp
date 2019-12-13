#include "serial.h"
#include <iostream>
using namespace std;

const Serial::SerialOptions Serial::defaultoptions= //Ĭ��ѡ��
{
	"COM6",
	//"COM10",
	115200,
	8,			//�ֽڳ���
	NOPARITY,	//��żУ��
	ONESTOPBIT,	//ֹͣλ1��������0��Ӧֹͣλ1  0,1,2 == 1,1.5,2
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
	std::string com_port ="\\\\.\\"+serialoptions.serialname;/*�򿪴���24   \\.\  */
	//std::string com_port =".\\"+serialoptions.serialname;//*�򿪴���24   .\ */
   	_hcom = CreateFile
	(
		com_port.c_str(),			//ָ���ļ�����ָ��
		GENERIC_READ|GENERIC_WRITE, //����ģʽ���� / д��
		0,							 //����ģʽ  �����0��ʾ��������FILE_SHARE_READ��/��FILE_SHARE_WRITE��ʾ������ļ����й���
		NULL,						//ָ��ȫ���Ե�ָ��   
		OPEN_EXISTING,				 //����ô��� ָ�����ļ����ڻ򲻴���ʱ�Ĳ����������Ĳ�����5�� 1.OPEN_EXISTING-�ļ��Ѿ�����.....
		FILE_ATTRIBUTE_NORMAL,		//�ļ�����  FILE_ATTRIBUTE_NORMAL��Ĭ������
		NULL						//���ڸ����ļ����   ���ڴ洢���ļ�����������Ϊ0����ָ��һ���ļ�������µ��ļ���������ļ��и�����չ���ԣ�
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
	if(SetCommState(_hcom,&_serialoptions)==false)//�ú�����ʼ��һ��ָ����ͨ���豸��ͨ�Ų�����
	{
		std::cout<<"SetCommState false"<<endl;
		return false;
	}	
	if(PurgeComm(_hcom,PURGE_TXCLEAR|PURGE_RXCLEAR)==false)//��ջ�����    
														//PURGE_TXABORT ��ֹ�������ڽ��е��ַ��������,���һ�������ڵȴ�״̬���ص�i/o����,��������һ���¼�,ָ�������д����
														//PURGE_RXABORT ��ֹ�������ڽ��е��ַ��������,���һ�����ڽ����е��ص�i/o����,�����������õ��ʵ��¼�
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

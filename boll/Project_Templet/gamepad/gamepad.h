#pragma once
#include "dinput.h"
#include "dinputd.h"

#include "joystickapi.h"

//#pragma ��һ��������ָ��,�Ǹ����߱�������Ҫ����һЩ����,Ȼ���ں����comment����ָ����ʲô����  ����#pragma���÷����Բο�msdn
#pragma comment(lib,"dxguid.lib")  //DirectX����  ����Ҫ�Ŀ�
#pragma comment(lib,"dinput8.lib") 
#pragma comment(lib, "winmm.lib") //����winmm.lib��,�������ſ���֧�ֶ�windows ��ý��ı��.  
// �磺��ʾ����wpcap.lib����⡣���ڹ���������д������wpcap.lib��Ч��һ�����������ַ���д�� ���������ʹ����Ĵ����ʱ���
//���������ù���settings�ˡ��������������ӵ�ʱ��Ҫ��ws2_32.lib��������Ͳ�����linker��lib������ָ�����lib�ˡ�


class Gamepad
{
public:
	enum padnum{key1,key2,key3,key4, key5,key6,key7,key8,key9,key10, noval, Up, Dwon, Right, Left};
	void Getnum(enum padnum &keyval);
	 bool bJoyStickCanUse();
	 void  updatakeyval(void);
	 bool sta;
private:
	HANDLE HOne;
	 enum padnum _lastkeynum;
	 UINT uiXmid, uiYmid, uiLeft, uiRight, uiUp, uiDown;
};

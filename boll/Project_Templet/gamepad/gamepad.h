#pragma once
#include "dinput.h"
#include "dinputd.h"

#include "joystickapi.h"

//#pragma 是一条编译器指令,是给告诉编译器你要链接一些东西,然后在后面的comment里面指明是什么东西  关于#pragma的用法可以参考msdn
#pragma comment(lib,"dxguid.lib")  //DirectX开发  必须要的库
#pragma comment(lib,"dinput8.lib") 
#pragma comment(lib, "winmm.lib") //导入winmm.lib库,有了它才可以支持对windows 多媒体的编程.  
// 如：表示链接wpcap.lib这个库。和在工程设置里写上链入wpcap.lib的效果一样，不过这种方法写的 程序别人在使用你的代码的时候就
//不用再设置工程settings了。告诉连接器连接的时候要找ws2_32.lib，这样你就不用在linker的lib设置里指定这个lib了。


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

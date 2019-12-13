#include "gamepad.h"
#include <windows.h>


void  Gamepad::Getnum(enum padnum & keyval)
{
	keyval = _lastkeynum;
}

bool Gamepad::bJoyStickCanUse()
{
	JOYCAPS jc;
	if (0 >= joyGetNumDevs())//�ж�ϵͳ֧�ֵ��ֱ���Ŀ
		return false;
	if (JOYERR_NOERROR != joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)))//�ж�ID1�ֱ��Ƿ����ʹ��
		return false;
	uiXmid = (jc.wXmin + jc.wXmax) / 2;//x����м�λ��
	uiYmid = (jc.wYmin + jc.wYmax) / 2;//y����м�λ��
	uiLeft = (uiXmid + jc.wXmin) / 2;//��ת�ķ�ֵ
	uiRight = (uiXmid + jc.wXmax) / 2;//��ת�ķ�ֵ
	uiUp = (uiYmid + jc.wYmax) / 2;//�����ķ�ֵ
	uiDown = (uiYmid + jc.wYmin) / 2;//�»��ķ�ֵ
	return true;
}

void Gamepad::updatakeyval(void) //����MFC�ֱ�����ֵ�����ң��£��ϣ�����
{
	JOYINFOEX jiex;
	if (!bJoyStickCanUse())
		return ;
	jiex.dwFlags = JOY_RETURNALL;
	if (JOYERR_NOERROR != joyGetPosEx(JOYSTICKID1, &jiex))
		return;
	if (jiex.dwXpos <= uiLeft)
	{
		Sleep(50);
		_lastkeynum = Left;
		return;
	}
	if (jiex.dwXpos >= uiRight)
	{
		Sleep(50);
		_lastkeynum = Right;
		return;
	}
	if (jiex.dwYpos <= uiDown)
	{
		Sleep(50);
		_lastkeynum = Dwon;
		return;
	}
	if (jiex.dwYpos >= uiUp)
	{
		Sleep(50);
		_lastkeynum = Up;
		return;
	}
	if (jiex.dwButtonNumber > 0)
	{
		for (int i = 0; i < 32; ++i)
		{
			if (jiex.dwButtons&(1 << i))
			{
				Sleep(50);
				_lastkeynum = (enum padnum) i; //0--9
				return;
			}
		}
	}
	_lastkeynum = noval;
	Sleep(150);
}




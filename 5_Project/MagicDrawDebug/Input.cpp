//
// Input.cpp
//
// 2009.01.10. Kihong Kim / mad_dog@hanmail.net
//
#include "../RRYEngine/framework.h"
#include "Input.h"


//////////////////////////////////////////////////////////////////////////////
//
// 키 멥핑 시스템 : 사용자 키 설정... P1, P2 용...
//
UINT g_Keys[2][ KEY_MAX_ ];			//unsigned int == UINT





//////////////////////////////////////////////////////////////////////////////
//
void  KeyInit()
{

	// 사용자 키 설정... P1, P2 용...
	// 사용자가 입력한 키가 변경되어도 로직상의 변화는 없다.
	//Player #1.
	g_Keys[0][ KMOVE_LEFT_ ]	= VK_LEFT;			// 기본 플레이어 키값 재설정.
	g_Keys[0][ KMOVE_RIGHT_ ]	= VK_RIGHT;
	g_Keys[0][ KMOVE_UP_ ]		= VK_UP;
	g_Keys[0][ KMOVE_DOWN_ ]	= VK_DOWN;
	

	//Player #2.
	g_Keys[1][ KMOVE_LEFT_ ]	= 0x41;	//A
	g_Keys[1][ KMOVE_RIGHT_ ]	= 0x44;	//D
	g_Keys[1][ KMOVE_UP_ ]		= 0x57;	//W
	g_Keys[1][ KMOVE_DOWN_ ]	= 0x53;	//S


}



/******************* end of file "Input.cpp" ********************************/

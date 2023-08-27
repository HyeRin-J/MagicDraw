//
// Input.h
//
// 2009.01.10. Kihong Kim / mad_dog@hanmail.net
//
 

#pragma once

//////////////////////////////////////////////////////////////////////////////
//
// Ű ���� �ý��� : ����Ű ����.
//
enum { 
	KMOVE_LEFT_,
	KMOVE_RIGHT_,
	KMOVE_UP_,
	KMOVE_DOWN_,

	KMOVE_GUNFIRE_,
	KMOVE_BOMBRELEASE_, 


	KEY_MAX_
};


void KeyInit();
void KeyRemap();

#ifndef UINT
typedef unsigned int  UINT;
#endif

extern UINT g_Keys[2][ KEY_MAX_ ];	//unsigned int == UINT


//�񵿱� Ű���� �Է� ó��.
#define IsKeyDown(k) GetAsyncKeyState(k) & 0x8000


/******************* end of file "Input.h" ********************************/
//
// Input.h
//
// 2009.01.10. Kihong Kim / mad_dog@hanmail.net
//
 

#pragma once

//////////////////////////////////////////////////////////////////////////////
//
// 키 멥핑 시스템 : 조작키 정의.
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


//비동기 키보드 입력 처리.
#define IsKeyDown(k) GetAsyncKeyState(k) & 0x8000


/******************* end of file "Input.h" ********************************/
/*
**	Redeemer Engine 2010 (C) Copyright by Dominik "squ@ll" Jasiñski.
**
**	S_GameMain.cpp
**		Entry point for "sh@ke" game
*/

//------------------------------------------------------------------------------------------------------------------------
#include "S_GameApplication.h"
//#include "vld.h"

#include <windows.h>

/*
**	WinMain
*/
int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
	try
	{
		REDEEMER::SHAKE::S_GameApplication	gameApplication;

		gameApplication.Initialize ();

		gameApplication.Finalize ();		
	}
	catch (...)
	{
		MessageBox (NULL, L"General exception failure!", L"Error", MB_ICONERROR | MB_OK);
	}

	return 0;
}
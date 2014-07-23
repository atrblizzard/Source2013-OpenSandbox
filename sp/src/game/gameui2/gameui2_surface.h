    // ======= Copyright Valve Corporation, Chris and Nicolas. All rights reserved. ======= //
   //			
  // Purpose:	Open Source GameUI for Source Engine.
 //				
// ======================================================================================== //
#if !defined( GAMEUI2_SURFACE_H )
#define GAMEUI2_SURFACE_H

#if defined( _WIN32 )
#pragma once
#endif // _WIN32

#include "vgui_controls/Frame.h"

  //---------------------------------------------------------------------------
 // Purpose: Shared button states
//-----------------------------------------------------------------------------
enum GameUI2_ButtonState
{
	Out,
	Over,
	Pressed,
	Released
};

  //---------------------------------------------------------------------------
 // Purpose: Our new hacky surface class!
//-----------------------------------------------------------------------------
class gameui2
{
private:
	gameui2(){}

public:
	// GameUI2 checks:
	virtual bool		IsIn_MainMenu();
	virtual bool		IsIn_PauseMenu();
	virtual bool		IsIn_SharedMenu();
	virtual bool		IsIn_Loading();
	virtual bool		IsIn_Multiplayer();

	// Color
	virtual void		DrawSetShapeColor( int r, int g, int b, int a );
	virtual void		DrawSetTextColor( int r, int g, int b, int a );

	// Shapes
	virtual void		DrawFilledRect( int x, int y, int width, int height );
	virtual void		DrawOutlinedRect( int x, int y, int width, int height );
	virtual void		DrawOutlinedCorners( int x, int y, int width, int height, int length );
	virtual void		DrawOutlinedRectFade( int x, int y, int width, int height );
	virtual void		DrawDiagonalRect( int x, int y, int width, int height );

	// Text
	virtual void		DrawSmallText( wchar_t *m_pText, int x, int y );

	// Elements
	virtual void		DrawLabel( wchar_t *m_pLabel_Text, int x, int y, int width, int height );
	virtual void		DrawTitle( wchar_t *m_pTitle_Text, int x, int y, int width, int height );

	// Shaders
	// Coming soon!

	// Animation
	virtual void		FadeIn_Int( int &fade_from, int fade_to, float fade_duration, float &fade_realtime );
	virtual void		FadeOut_Int( int &fade_from, int fade_to, float fade_duration, float &fade_realtime );

	static gameui2* s_pInstance;
	static gameui2* surface()
	{
		if( s_pInstance == 0 )
		{
			s_pInstance = new gameui2();
			return s_pInstance;
		}

		return s_pInstance;
	}
};
#endif // GAMEUI2_SURFACE_H
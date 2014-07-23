    // ======= Copyright Valve Corporation, Chris and Nicolas. All rights reserved. ======= //
   //			
  // Purpose:	Open Source GameUI for Source Engine.
 //				
// ======================================================================================== //
#if !defined( GAMEUI2_MAINMENU_H )
#define GAMEUI2_MAINMENU_H

#if defined( _WIN32 )
#pragma once
#endif // _WIN32

#include "vgui_controls/Frame.h"
#include "gameui2_button_mainmenu.h"
#include "chunk/menu_parser.h"

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
class CGameUI2_MainMenu : public vgui::Frame
{
	DECLARE_CLASS_SIMPLE( CGameUI2_MainMenu, vgui::Frame );

public:
						CGameUI2_MainMenu( vgui::Panel *parent );
						~CGameUI2_MainMenu();
	virtual void 		OnThink();
	virtual bool		IsVisible();
	virtual bool 		DrawLogo( int width, int height );
	virtual bool 		DrawMainMenu( int width, int height );
	virtual void 		Paint( void );
	virtual void 		PaintBackground();
	virtual void 		PaintBorder(){}
	virtual void 		PaintBuildOverlay(){}
	virtual void		OnCommand( char const *cmd );
	void				OnSetFocus();
	void				OnKillFocus();
	const char			*GetAlign(){ return align; }

private:
	CUtlVector< CGameUI2_Button_MainMenu* > m_Button_MM; // Main Menu
	CUtlVector< CGameUI2_Button_MainMenu* > m_Button_PM; // Pause Menu
	CUtlVector< CGameUI2_Button_MainMenu* > m_Button_SM; // Shared Menu
	
	CMenuParser			*menuParser;

	const char			*align;

	int					screen_width;
	int					screen_height;
	bool				focused;
};

  //---------------------------------------------------------------------------
 // Purpose: This class removes some engine stuff which we do not need!
//-----------------------------------------------------------------------------
class CGameUI2_Post_Frame : public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CGameUI2_Post_Frame, vgui::Panel );

public:
						CGameUI2_Post_Frame( CGameUI2_MainMenu *pPanel, vgui::Panel *parent );
};
#endif // GAMEUI2_MAINMENU_H
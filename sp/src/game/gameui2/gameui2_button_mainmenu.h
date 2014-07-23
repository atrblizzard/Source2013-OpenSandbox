    // ======= Copyright Valve Corporation, Chris and Nicolas. All rights reserved. ======= //
   //			
  // Purpose:	Open Source GameUI for Source Engine.
 //				
// ======================================================================================== //
#if !defined( GAMEUI2_BUTTON_MAINMENU_H )
#define GAMEUI2_BUTTON_MAINMENU_H

#if defined( _WIN32 )
#pragma once
#endif // _WIN32

#include "vgui_controls/Button.h"
#include "gameui2_surface.h"

#include <vector>
#include <sstream>
#include <iostream>
#include <string>

using namespace vgui;

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
class CGameUI2_Button_MainMenu : public vgui::Button
{
	DECLARE_CLASS_SIMPLE( CGameUI2_Button_MainMenu, vgui::Button );

public:
	CGameUI2_Button_MainMenu( vgui::Panel *parent, const char *panelName, const char *text, Panel *pActionSignalTarget = NULL, const char *pCmd = NULL, std::string textalign = "", int width = 0, int height = 0, int button_priority = 0 );
	CGameUI2_Button_MainMenu( vgui::Panel *parent, const char *panelName, wchar_t *text, Panel *pActionSignalTarget = NULL, const char *pCmd = NULL, std::string textalign = "", int width = 0, int height = 0, int button_priority = 0 );
	~CGameUI2_Button_MainMenu();

	virtual void		Init_Shared( vgui::Panel *parent, const char *pCmd = NULL, std::string textalign = "", int width = 0, int height = 0, int button_priority = 0 );
	virtual void		ApplySchemeSettings( IScheme *pScheme );
	virtual void 		OnThink();	
	virtual void 		DrawButton();
	virtual void 		DrawText();
	virtual void 		Paint( void );
	virtual void 		PaintBackground();

	void 				OnCursorEntered();
	void 				OnCursorExited();
	void 				OnMouseReleased( vgui::MouseCode code );
	void 				OnMousePressed( vgui::MouseCode code );

	static int			GetWidth(){ return button_width; }
	static int			GetHeight(){ return button_height; }
	int					GetPriority() const{ return priority; }

	GameUI2_ButtonState	button_state;

	Panel* 				m_pParent;
	char 				btn_command[64];

private:
	static int 			button_width;
	static int 			button_height;
	int					priority;
};
#endif // GAMEUI2_BUTTON_MAINMENU_H
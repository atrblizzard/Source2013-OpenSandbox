    // ======= Copyright Valve Corporation, Chris and Nicolas. All rights reserved. ======= //
   //			
  // Purpose:	Open Source GameUI for Source Engine.
 //				
// ======================================================================================== //
#include "cbase.h"
#include "gameui2_button_mainmenu.h"
#include "vgui_controls/Button.h"
#include "vgui_controls/CheckButton.h"
#include <vgui/IInput.h>
#include "vgui/ILocalize.h"
#include "vgui/ISurface.h"
#include "vgui/IVGui.h"
#include "KeyValues.h"
#include <UtlSymbol.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

DECLARE_BUILD_FACTORY_DEFAULT_TEXT( CGameUI2_Button_MainMenu, CGameUI2_Button_MainMenu );

extern CUtlSymbolTable g_ButtonSoundNames;

int CGameUI2_Button_MainMenu::button_width = 0;
int CGameUI2_Button_MainMenu::button_height = 0;

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
CGameUI2_Button_MainMenu::CGameUI2_Button_MainMenu( vgui::Panel *parent, const char *panelName, const char *text, Panel *pActionSignalTarget, const char *pCmd, std::string textalign, int width, int height, int button_priority ) : BaseClass( parent, panelName, text, pActionSignalTarget, pCmd )
{
	Init_Shared( parent, pCmd, textalign, width, height, button_priority );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
CGameUI2_Button_MainMenu::CGameUI2_Button_MainMenu( vgui::Panel *parent, const char *panelName, wchar_t *text, Panel *pActionSignalTarget, const char *pCmd, std::string textalign, int width, int height, int button_priority ) : BaseClass( parent, panelName, text, pActionSignalTarget, pCmd )
{
	Init_Shared( parent, pCmd, textalign, width, height, button_priority );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
CGameUI2_Button_MainMenu::~CGameUI2_Button_MainMenu()
{

}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_Button_MainMenu::Init_Shared( vgui::Panel *parent, const char *pCmd, std::string textalign, int width, int height, int button_priority )
{
	// Setting original vgui stuff
	SetProportional( false );
	SetPaintBorderEnabled( false );
	SetPaintBackgroundEnabled( false );
	SetEnabled( true );

	std::string left = "left";
	std::string center = "center";
	std::string right = "right";

	if( textalign == left )
		SetContentAlignment( a_west );
	else if( textalign == center )
		SetContentAlignment( a_center );
	else if( textalign == right )
		SetContentAlignment( a_east );
	else
		SetContentAlignment( a_west );

	// Set button size
	button_width = width;
	button_height = height;
	SetBounds( 0, 0, button_width, button_height );

	// Let's override original command code
	m_pParent = parent;
	if( pCmd != NULL )
		Q_strcpy( btn_command, pCmd );

	button_state = Out;

	priority = button_priority;
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_Button_MainMenu::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	// Setting font
	vgui::HScheme Scheme = vgui::scheme()->LoadSchemeFromFile( "interface/gameui2_scheme.res", "GameUI2Scheme" );
	vgui::HFont hFont = vgui::scheme()->GetIScheme( Scheme )->GetFont( "MainMenu" );
	SetFont(hFont);

	// Set text position
	SetTextInset( 11, 1 );

	// Set button sounds
	SetArmedSound("interface/ui/button_over.wav");
	SetDepressedSound("interface/ui/button_click.wav");
	SetReleasedSound("interface/ui/button_release.wav");
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_Button_MainMenu::OnThink()
{
	BaseClass::OnThink();
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_Button_MainMenu::DrawButton()
{
	switch( button_state )
	{
		case Out:
				gameui2::surface()->DrawSetShapeColor( 0, 0, 0, 0 );
				gameui2::surface()->DrawFilledRect( 0, 0, button_width, button_height );
				break;

		case Over:
				gameui2::surface()->DrawSetShapeColor( 190, 190, 190, 255 );
				gameui2::surface()->DrawFilledRect( 0, 0, button_width, button_height );
				break;

		case Pressed:
				gameui2::surface()->DrawSetShapeColor( 190, 190, 190, 255 );
				gameui2::surface()->DrawFilledRect( 0, 0, button_width, button_height );
				break;

		case Released:
				gameui2::surface()->DrawSetShapeColor( 190, 190, 190, 255 );
				gameui2::surface()->DrawFilledRect( 0, 0, button_width, button_height );
				break;

		default:
				gameui2::surface()->DrawSetShapeColor( 0, 0, 0, 0 );
				gameui2::surface()->DrawFilledRect( 0, 0, button_width, button_height );
				break;
	}
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_Button_MainMenu::DrawText()
{
	// Set colour
	Color colA = Color( 30, 30, 30, 255 );
	Color colB = Color( 179, 179, 179, 255 );

	SetDefaultColor( colB, colB );
	SetArmedColor( colA, colA );
	SetSelectedColor( colA, colA );
	SetDepressedColor( colA, colA );
	SetBlinkColor( colA );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_Button_MainMenu::Paint()
{
	BaseClass::Paint();

	// Drawing our button
	DrawButton();

	// Drawing our text
	DrawText();
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_Button_MainMenu::PaintBackground()
{
	// Overriding default style
	SetBgColor( Color( 0, 0, 0, 0 ) );
	SetPaintBackgroundType( 0 );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_Button_MainMenu::OnCursorExited()
{
	BaseClass::OnCursorExited();

	button_state = Out;
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_Button_MainMenu::OnCursorEntered()
{
	BaseClass::OnCursorEntered();

	button_state = Over;
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_Button_MainMenu::OnMousePressed( vgui::MouseCode code )
{
	if( code == MOUSE_LEFT )
	{
		if( m_sDepressedSoundName != UTL_INVAL_SYMBOL )
		{
			surface()->PlaySound( g_ButtonSoundNames.String( m_sDepressedSoundName ) );
		}
		
		button_state = Pressed;
	}
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_Button_MainMenu::OnMouseReleased( vgui::MouseCode code )
{
	m_pParent->OnCommand( btn_command );
	if( code == MOUSE_LEFT )
	{
		if( m_sReleasedSoundName != UTL_INVAL_SYMBOL )
		{
			surface()->PlaySound( g_ButtonSoundNames.String( m_sReleasedSoundName ) );
		}
		
		button_state = Released;
	}
}
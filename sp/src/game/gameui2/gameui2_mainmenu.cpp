    // ======= Copyright Valve Corporation, Chris and Nicolas. All rights reserved. ======= //
   //			
  // Purpose:	Open Source GameUI for Source Engine.
 //				
// ======================================================================================== //
#include "cbase.h"
#include "gameui2_mainmenu.h"
#include "gameui2_surface.h"
#include "vgui/ISurface.h"
#include "vgui/ILocalize.h"
#include "vgui/IVGui.h"
#include "KeyValues.h"
#include <filesystem.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar gameui2_mainmenu_draw_logo( "gameui2_mainmenu_draw_logo", "1", FCVAR_CHEAT | FCVAR_HIDDEN );
ConVar gameui2_mainmenu_draw_mainmenu( "gameui2_mainmenu_draw_mainmenu", "1", FCVAR_CHEAT | FCVAR_HIDDEN );

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
CGameUI2_MainMenu::CGameUI2_MainMenu( vgui::Panel *parent ) : BaseClass( NULL, "GameUI2_MainMenu" )
{
	SetProportional( false );
	SetDeleteSelfOnClose( true );
	SetSizeable( false );
	SetMoveable( false );
	SetCloseButtonVisible( false );
	SetMenuButtonVisible( false );
	Activate();

	// Small hack.
	// SetMinimumSize( 0, 0 );
	
	std::string filename = "/interface/scripts/mainmenu.gui2";
	std::stringstream filepath;
	filepath << engine->GetGameDirectory() << filename;
	
	menuParser = new CMenuParser( this, &m_Button_MM, &m_Button_PM, &m_Button_SM );
	menuParser->LoadMenuConfig( filepath.str().c_str() );
	align = menuParser->GetAlign();

	focused = true;
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
CGameUI2_MainMenu::~CGameUI2_MainMenu()
{

}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_MainMenu::OnThink()
{
	BaseClass::OnThink();

	// Get our screen resolution
	engine->GetScreenSize( screen_width, screen_height );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
bool CGameUI2_MainMenu::IsVisible( void )
{
	if( gameui2::surface()->IsIn_Loading() )
		return false;

	return focused;
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
bool CGameUI2_MainMenu::DrawLogo( int width, int height )
{
	if( !gameui2_mainmenu_draw_logo.GetBool() )
		return false;

	// TODO: Logo!
	
	return true;
}

  //---------------------------------------------------------------------------
 // Purpose: Sorting buttons position inside the tail.
//-----------------------------------------------------------------------------
typedef CGameUI2_Button_MainMenu SortButtonPriorityType;

int __cdecl ButtonsPositionBottom( SortButtonPriorityType *const *s1, SortButtonPriorityType *const *s2 )
{
	return ( ( *s1 )->GetPriority() > ( *s2 )->GetPriority() );
}

int __cdecl ButtonsPositionTop( SortButtonPriorityType *const *s1, SortButtonPriorityType *const *s2 )
{
	return ( ( *s1 )->GetPriority() < ( *s2 )->GetPriority() );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
bool CGameUI2_MainMenu::DrawMainMenu( int width, int height )
{
	if( !gameui2_mainmenu_draw_mainmenu.GetBool() )
		return false;

	std::string left = "left";
	std::string left_top = "left top";
	std::string left_center = "left center";
	std::string left_bottom = "left bottom";

	std::string center = "center";
	std::string center_top = "center top";
	std::string center_center = "center center";
	std::string center_bottom = "center bottom";
	std::string center_left = "center left";
	std::string center_right = "center right";
	
	std::string right = "right";
	std::string right_top = "right top";
	std::string right_center = "right center";
	std::string right_bottom = "right bottom";

	std::string top = "top";
	std::string top_left = "top left";
	std::string top_center = "top center";
	std::string top_right = "top right";

	std::string bottom = "bottom";
	std::string bottom_left = "bottom left";
	std::string bottom_center = "bottom center";
	std::string bottom_right = "bottom right";

	CUtlVector< CGameUI2_Button_MainMenu* > m_Buttons;

	for( int i = 0; i < m_Button_PM.Count(); i++ )
		m_Button_PM[i]->SetVisible( gameui2::surface()->IsIn_PauseMenu() );

	for( int i = 0; i < m_Button_MM.Count(); i++ )
		m_Button_MM[i]->SetVisible( gameui2::surface()->IsIn_MainMenu() );

	for( int i = 0; i < m_Button_SM.Count(); i++ )
		m_Button_SM[i]->SetVisible( gameui2::surface()->IsIn_SharedMenu() );
	
	if( gameui2::surface()->IsIn_PauseMenu() )
		m_Buttons.AddVectorToTail( m_Button_PM );
	
	if( gameui2::surface()->IsIn_MainMenu() )
		m_Buttons.AddVectorToTail( m_Button_MM );

	if( gameui2::surface()->IsIn_SharedMenu() )
		m_Buttons.AddVectorToTail( m_Button_SM );

	if( !gameui2::surface()->IsIn_SharedMenu() )
	{
		gameui2::surface()->DrawSetShapeColor( 255, 255, 255, 150 );
		gameui2::surface()->DrawFilledRect( 32, 32, 256, 256 );
	}

	if( ( align == left_bottom ) || ( align == center_bottom ) || ( align == right_bottom ) || ( align == bottom ) || ( align == bottom_left ) || ( align == bottom_center ) || ( align == bottom_right ) )
		m_Buttons.Sort( ButtonsPositionBottom );
	else if( ( align == left_top ) || ( align == center_top ) || ( align == right_top ) || ( align == top ) || ( align == top_left ) || ( align == top_center ) || ( align == top_right ) || ( align == center_center ) || ( align == center_left ) || ( align == center_right ) || ( align == center ) || ( align == left_center ) || ( align == right_center ) )
		m_Buttons.Sort( ButtonsPositionTop );
	else
		m_Buttons.Sort( ButtonsPositionBottom );

	int menu_section_x, menu_section_y;
	int buttons_count = m_Buttons.Count();
	int buttons_width = CGameUI2_Button_MainMenu::GetWidth();
	int buttons_height = CGameUI2_Button_MainMenu::GetHeight();

	for( int i = 0; i < buttons_count; i++ )
	{
		int h_left		= 32;
		int h_right		= width - ( 32 + buttons_width );
		int h_center	= width / 2 - buttons_width / 2;
		int v_top		= 32 + ( buttons_height * i );
		int v_bottom	= height - ( 32 + ( buttons_height * ( i + 1 ) ) );
		int v_center	= ( ( height / 2 ) - ( ( buttons_height * buttons_count ) / 2 ) ) + ( buttons_height * i );
		
		if( align == left )
		{
			menu_section_x = h_left;
			menu_section_y = v_bottom;
		}
		else if( align == left_top )
		{
			menu_section_x = h_left;
			menu_section_y = v_top;
		}
		else if( align == left_center )
		{
			menu_section_x = h_left;
			menu_section_y = v_center;
		}
		else if( align == left_bottom )
		{
			menu_section_x = h_left;
			menu_section_y = v_bottom;
		}
		else if( align == center )
		{
			menu_section_x = h_center;
			menu_section_y = v_center;
		}
		else if( align == center_top )
		{
			menu_section_x = h_center;
			menu_section_y = v_top;
		}
		else if( align == center_center )
		{
			menu_section_x = h_center;
			menu_section_y = v_center;
		}
		else if( align == center_bottom )
		{
			menu_section_x = h_center;
			menu_section_y = v_bottom;
		}
		else if( align == center_left )
		{
			menu_section_x = h_left;
			menu_section_y = v_center;
		}
		else if( align == center_right )
		{
			menu_section_x = h_right;
			menu_section_y = v_center;
		}
		else if( align == right )
		{
			menu_section_x = h_right;
			menu_section_y = v_bottom;
		}
		else if( align == right_top )
		{
			menu_section_x = h_right;
			menu_section_y = v_top;
		}
		else if( align == right_center )
		{
			menu_section_x = h_right;
			menu_section_y = v_center;
		}
		else if( align == right_bottom )
		{
			menu_section_x = h_right;
			menu_section_y = v_bottom;
		}
		else if( align == top )
		{
			menu_section_x = h_left;
			menu_section_y = v_top;
		}
		else if( align == top_left )
		{
			menu_section_x = h_left;
			menu_section_y = v_top;
		}
		else if( align == top_center )
		{
			menu_section_x = h_center;
			menu_section_y = v_top;
		}
		else if( align == top_right )
		{
			menu_section_x = h_right;
			menu_section_y = v_top;
		}
		else if( align == bottom )
		{
			menu_section_x = h_left;
			menu_section_y = v_bottom;
		}
		else if( align == bottom_left )
		{
			menu_section_x = h_left;
			menu_section_y = v_bottom;
		}
		else if( align == bottom_center )
		{
			menu_section_x = h_center;
			menu_section_y = v_bottom;
		}
		else if( align == bottom_right )
		{
			menu_section_x = h_right;
			menu_section_y = v_bottom;
		}
		else
		{
			menu_section_x = h_left;
			menu_section_y = v_bottom;
		}
		
		m_Buttons[i]->SetPos( menu_section_x, menu_section_y );
	}
	
	return true;
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_MainMenu::Paint()
{
	BaseClass::Paint();

	// Set main menu's panel position( fullscreen )
	SetBounds( 0, 0, screen_width, screen_height );

	// Drawing mod's logo
	// DrawLogo( screen_width, screen_height );

	// Drawing mod's main menu
	DrawMainMenu( screen_width, screen_height );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_MainMenu::PaintBackground()
{
	// Overriding default style
	SetBgColor( Color( 0, 0, 0, 0 ) );
	SetFgColor( Color( 0, 0, 0, 0 ) );
	SetPaintBackgroundType( 0 );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_MainMenu::OnCommand( char const *cmd )
{
	if( !Q_stricmp( cmd, "action_resumegame" ) )
		engine->ClientCmd( "gamemenucommand resumegame" );
	else if( !Q_stricmp( cmd, "action_savegame" ) )
		engine->ClientCmd( "gamemenucommand opensavegamedialog" );
	else if( !Q_stricmp( cmd, "action_loadgame" ) )
		engine->ClientCmd( "gamemenucommand openloadgamedialog" );
	else if( !Q_stricmp( cmd, "action_singleplayer" ) )
		engine->ClientCmd( "gamemenucommand opennewgamedialog" );
	else if( !Q_stricmp( cmd, "action_options" ) )
		engine->ClientCmd( "gamemenucommand openoptionsdialog" );
	else if( !Q_stricmp( cmd, "action_quitgame" ) )
		engine->ClientCmd( "gamemenucommand quit" );
	else if( !Q_stricmp( cmd, "action_disconnect" ) )
		engine->ClientCmd( "gamemenucommand disconnect" );
	else if( !Q_stricmp( cmd, "action_serverbrowser" ) )
		engine->ClientCmd( "gamemenucommand openserverbrowser" );
	else if( !Q_stricmp( cmd, "action_createmultiplayer" ) )
		engine->ClientCmd( "gamemenucommand opencreatemultiplayergamedialog" );
	else
		BaseClass::OnCommand( cmd );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_MainMenu::OnSetFocus()
{
	BaseClass::OnSetFocus();

	focused = true;

	vgui::surface()->PlaySound( "interface/ui/menu_open.wav" );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_MainMenu::OnKillFocus()
{
	BaseClass::OnKillFocus();

	focused = false;

	vgui::surface()->PlaySound( "interface/ui/menu_close.wav" );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
CGameUI2_Post_Frame::CGameUI2_Post_Frame( CGameUI2_MainMenu *pPanel, vgui::Panel *parent ) : BaseClass( parent )
{
	// Small hack.
	// SetMinimumSize( 0, 0 );
	
	pPanel->SetParent( this );
	pPanel->MakeReadyForUse();
	pPanel->SetZPos( 0 );
}
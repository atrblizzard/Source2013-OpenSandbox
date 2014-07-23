    // ======= Copyright Valve Corporation, Chris and Nicolas. All rights reserved. ======= //
   //			
  // Purpose:	Open Source GameUI for Source Engine.
 //				
// ======================================================================================== //
#include "cbase.h"

#include "gameui2_frame.h"
#include "gameui2_mainmenu.h"
#include "gameui2_loading.h"

#include "ienginevgui.h"

#include "engine/ienginesound.h"
#include "SoundEmitterSystem/isoundemittersystembase.h"

#include "vgui/IVGui.h"
#include "vgui/ISurface.h"
#include "vgui/ILocalize.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ISoundEmitterSystemBase *soundemitterbase;
static CDllDemandLoader g_GameUI( "GameUI" );
static CGameUI2_Frame *g_pGameUI2_Frame;
CGameUI2_Frame *GetGUI2Frame()
{
	return g_pGameUI2_Frame;
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
CGameUI2_Frame::CGameUI2_Frame( vgui::VPANEL parent ) : BaseClass( NULL )
{
	SetParent( parent );
	SetKeyBoardInputEnabled( true );
	SetMouseInputEnabled( true );
	SetProportional( false );
	SetVisible( false );

	// Small hack.
	SetMinimumSize( 0, 0 );

	/// NOTE: This is background music
	/// You can set it inside the "scripts/game_sounds_interface.txt"
	///
	/// NOTE: Make sure to include "game_sounds_interface.txt" inside the "scripts/game_sounds_manifest.txt"
	m_backgroundMusic = "Interface.Music"; // "Interface.MainUI"
	m_nBackgroundMusicGUID = 0;

	/// NOTE: This block of code is Source2013 only!!!
	/// Source2006 or Source2007 do not have "SetMainMenuOverride()"
	///
	/// NOTE: In case if you are using Source2006 or Source2007 please comment this block of code
	/// and replace all text inside the "GameMenu.res" in your mod's "resource" folder to "GameMenu"{}
	CreateInterfaceFn gameUIFactory = g_GameUI.GetFactory();
	if( gameUIFactory )
	{
		m_pGameUI = ( IGameUI* ) gameUIFactory( GAMEUI_INTERFACE_VERSION, NULL );
		if( m_pGameUI )
		{
			m_pGameUI->SetMainMenuOverride( GetVPanel() );
		}
	}

	g_pVGuiLocalize->AddFile( "interface/gameui2_%language%.txt");

	// Create our main menu.
	if( !m_pPostFrame )
	{
		m_pPostFrame = new CGameUI2_Post_Frame( new CGameUI2_MainMenu( NULL ), NULL );
		m_pPostFrame->SetParent( ( vgui::VPANEL )parent );
	}
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
CGameUI2_Frame::~CGameUI2_Frame()
{

}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_Frame::Create()
{	
	vgui::VPANEL GameUI2 = enginevgui->GetPanel( PANEL_GAMEUIDLL );	
	g_pGameUI2_Frame = new CGameUI2_Frame( GameUI2 );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_Frame::OnThink()
{	
	BaseClass::OnThink();

	// Get our screen resolution
    engine->GetScreenSize( screen_width, screen_height );

	// We need to start music in main menu and disable it if we start any map.
	if( !IsBackgroundMusicPlaying() && gameui2::surface()->IsIn_MainMenu() )
		StartBackgroundMusic( 1.0f );
	else if( IsBackgroundMusicPlaying() && !gameui2::surface()->IsIn_MainMenu() )
		ReleaseBackgroundMusic();
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_Frame::Paint()
{
	BaseClass::Paint();

	// Set gameui2 frame's panel position( fullscreen )
	SetBounds( 0, 0, screen_width, screen_height );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_Frame::PaintBackground()
{
	// Overriding default style
	SetBgColor( Color( 0, 0, 0, 0 ) );
	SetPaintBackgroundType( 0 );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
bool CGameUI2_Frame::IsBackgroundMusicPlaying()
{
	if( m_backgroundMusic.IsEmpty() )
		return false;

	if( m_nBackgroundMusicGUID == 0 )
		return false;
	
	return enginesound->IsSoundStillPlaying( m_nBackgroundMusicGUID );
}

// per Morasky
#define BACKGROUND_MUSIC_DUCK	0.15f

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
bool CGameUI2_Frame::StartBackgroundMusic( float fVol )
{
	if( IsBackgroundMusicPlaying() )
		return true;
	
	if( m_backgroundMusic.IsEmpty() )
		return false;
	
	CSoundParameters params;
	if( !soundemitterbase->GetParametersForSound( m_backgroundMusic.Get(), params, GENDER_NONE ) )
		return false;

	enginesound->EmitAmbientSound( params.soundname, params.volume * fVol, params.pitch );
	m_nBackgroundMusicGUID = enginesound->GetGuidForLastSoundEmitted();
		
	return ( m_nBackgroundMusicGUID != 0 );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_Frame::UpdateBackgroundMusicVolume( float fVol )
{
	if( !IsBackgroundMusicPlaying() )
		return;

	// mixes too loud against soft ui sounds
	enginesound->SetVolumeByGuid( m_nBackgroundMusicGUID, BACKGROUND_MUSIC_DUCK * fVol );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void CGameUI2_Frame::ReleaseBackgroundMusic()
{
	if( m_backgroundMusic.IsEmpty() )
		return;

	if( m_nBackgroundMusicGUID == 0 )
		return;

	// need to stop the sound now, do not queue the stop
	// we must release the 2-5 MB held by this resource
	enginesound->StopSoundByGuid( m_nBackgroundMusicGUID );

	m_nBackgroundMusicGUID = 0;
}
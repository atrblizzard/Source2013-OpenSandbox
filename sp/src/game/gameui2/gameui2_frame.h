    // ======= Copyright Valve Corporation, Chris and Nicolas. All rights reserved. ======= //
   //			
  // Purpose:	Open Source GameUI for Source Engine.
 //				
// ======================================================================================== //
#if !defined( GAMEUI2_FRAME_H )
#define GAMEUI2_FRAME_H

#if defined( _WIN32 )
#pragma once
#endif // _WIN32

#include "vgui_controls/Panel.h"
#include "GameUI/IGameUI.h"

class CGameUI2_Post_Frame;

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
class CGameUI2_Frame : public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CGameUI2_Frame, vgui::Panel );

public:
						CGameUI2_Frame( vgui::VPANEL parent );
						~CGameUI2_Frame();
	static void			Create();
	virtual void 		OnThink();
	virtual void		Paint( void );
	virtual void		PaintBackground();
	virtual void 		PaintBorder(){}
	virtual void 		PaintBuildOverlay(){}

	// NOTE: From Alien Swarm.
	bool				IsBackgroundMusicPlaying();
	bool				StartBackgroundMusic( float fVol );
	void				UpdateBackgroundMusicVolume( float fVol );
	void				ReleaseBackgroundMusic();
	CUtlString			m_backgroundMusic;
	int					m_nBackgroundMusicGUID;

private:
	IGameUI				*m_pGameUI;

	CGameUI2_Post_Frame	*m_pPostFrame;

	int					screen_width;
	int					screen_height;
};

extern CGameUI2_Frame *GetGUI2Frame();
#endif // GAMEUI2_FRAME_H
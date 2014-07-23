    // ======= Copyright Valve Corporation, Chris and Nicolas. All rights reserved. ======= //
   //			
  // Purpose:	Open Source GameUI for Source Engine.
 //				
// ======================================================================================== //
#include "cbase.h"
#include "gameui2_surface.h"
#include "vgui/ISurface.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

gameui2* gameui2::s_pInstance = 0;

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
bool gameui2::IsIn_MainMenu( void )
{
	if( IsIn_PauseMenu() )
	{
		return false;
	}
	return true;
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
bool gameui2::IsIn_PauseMenu( void )
{
	const char *levelName = engine->GetLevelName();
	if( levelName && levelName[0] && !engine->IsLevelMainMenuBackground() )
	{
		return true;
	}
	return false;
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
bool gameui2::IsIn_SharedMenu( void )
{
	return ( IsIn_MainMenu() || IsIn_PauseMenu() );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
bool gameui2::IsIn_Loading( void )
{
	if( !engine->IsDrawingLoadingImage() )
	{
		return false;
	}
	return true;
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
bool gameui2::IsIn_Multiplayer( void )
{
	return ( IsIn_PauseMenu() && engine->GetMaxClients() > 1 );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void gameui2::DrawSetShapeColor( int r, int g, int b, int a )
{
	vgui::surface()->DrawSetColor( Color( r, g, b, a ) );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void gameui2::DrawSetTextColor( int r, int g, int b, int a )
{
	vgui::surface()->DrawSetTextColor( r, g, b, a );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void gameui2::DrawFilledRect( int x, int y, int width, int height )
{
	vgui::surface()->DrawFilledRect( x, y, width + x, height + y );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void gameui2::DrawOutlinedRect( int x, int y, int width, int height )
{
	vgui::surface()->DrawOutlinedRect( x, y, width + x, height + y );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void gameui2::DrawOutlinedCorners( int x, int y, int width, int height, int length )
{
	int x1r = width - length;
	int y1r = height - length;
	int hl_len = length + x;
	int hr_len = length + x1r;
	int vl_len = length + y;
	int x1m = width - 1;

	DrawFilledRect( x, y, hl_len, y + 1 ); // top - left
	DrawFilledRect( x1r, y, hr_len, y + 1 ); // top - right

	DrawFilledRect( x, y + 1, x + 1, vl_len ); // left - top
	DrawFilledRect( x, y1r, x + 1, length + y1r ); // left - bottom

	DrawFilledRect( x1m, y + 1, x1m + 1, vl_len ); // right - top
	DrawFilledRect( x1m, y1r, x1m + 1, length + y1r ); // right - bottom

	DrawFilledRect( x, height - 1, hl_len, height ); // bottom - left
	DrawFilledRect( x1r, height - 1, hr_len, height ); // bottom - right
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void gameui2::DrawOutlinedRectFade( int x, int y, int width, int height )
{
	// draw an outline of a rectangle using 4 filledRect
	vgui::surface()->DrawFilledRectFade( x, y, width, y + 1, 255, 0, true ); // top
	vgui::surface()->DrawFilledRectFade( x, height - 1, width, height, 255, 0, true ); // bottom
	DrawFilledRect( x, y + 1, x + 1, height - 1 ); // left
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void gameui2::DrawDiagonalRect( int x, int y, int width, int height )
{
	vgui::surface()->DrawFilledRectFade( x, y, width, height, 57, 0, true );
	vgui::surface()->DrawFilledRectFade( x, y, width, height, 127, 0, false );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void gameui2::DrawSmallText( wchar_t *m_pText, int x, int y )
{
	// Get our scheme
	vgui::HScheme Scheme = vgui::scheme()->LoadSchemeFromFile( "interface/gameui2_scheme.res", "GameUI2Scheme" );

	// Set text font
	vgui::HFont hFont = vgui::scheme()->GetIScheme( Scheme )->GetFont( "HUD_Small" );
	vgui::surface()->DrawSetTextFont( hFont );

	// Set text position
	vgui::surface()->DrawSetTextPos( x, y );

	// Draw our text
	vgui::surface()->DrawPrintText( m_pText, wcslen( m_pText ) );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void gameui2::DrawLabel( wchar_t *m_pLabel_Text, int x, int y, int width, int height )
{
	// Here comes our background!
	gameui2::surface()->DrawSetShapeColor( 30, 30, 30, 255 );
	gameui2::surface()->DrawFilledRect( x, y, width, height );

	gameui2::surface()->DrawSetShapeColor( 35, 35, 35, 255 );
	gameui2::surface()->DrawOutlinedRect( x, y, width, height );

	/// Now we draw text!
	// Get our scheme
	vgui::HScheme Scheme = vgui::scheme()->LoadSchemeFromFile( "interface/gameui2_scheme.res", "GameUI2Scheme" );

	// Set text font
	vgui::HFont hFont = vgui::scheme()->GetIScheme( Scheme )->GetFont( "Label" );
	vgui::surface()->DrawSetTextFont( hFont );

	// Set text color
	vgui::surface()->DrawSetTextColor( 255, 255, 255, 255 );

	// Set text size
	int textTall = vgui::surface()->GetFontTall( hFont );
	int textLength = 0;
	int Length = wcslen( m_pLabel_Text );
	for( int i = 0; i < Length; i++ )
	{
		textLength += vgui::surface()->GetCharacterWidth( hFont, m_pLabel_Text[i] );
	}

	// Set text position
	vgui::surface()->DrawSetTextPos( ( ( width / 2 ) - ( textLength / 2 ) ) + x, ( ( height / 2 ) - ( textTall / 2 ) ) + ( y - 2 ) );

	// Draw our text
	vgui::surface()->DrawPrintText( m_pLabel_Text, wcslen( m_pLabel_Text ) );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void gameui2::DrawTitle( wchar_t *m_pTitle_Text, int x, int y, int width, int height )
{
	// Here comes our background!
	gameui2::surface()->DrawSetShapeColor( 0, 0, 0, 0 );
	gameui2::surface()->DrawFilledRect( x, y, width, height );

	/// Now we draw text!
	// Get our scheme
	vgui::HScheme Scheme = vgui::scheme()->LoadSchemeFromFile( "interface/gameui2_scheme.res", "GameUI2Scheme" );

	// Set text font
	vgui::HFont hFont = vgui::scheme()->GetIScheme( Scheme )->GetFont( "Title" );
	vgui::surface()->DrawSetTextFont( hFont );

	// Set text color
	vgui::surface()->DrawSetTextColor( 255, 255, 255, 150 );

	// Set text position
	vgui::surface()->DrawSetTextPos( x - 2, y - 7 );

	// Draw our text
	vgui::surface()->DrawPrintText( m_pTitle_Text, wcslen( m_pTitle_Text ) );
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void gameui2::FadeIn_Int( int &fade_from, int fade_to, float fade_duration, float &fade_realtime )
{
	// TODO
}

  //---------------------------------------------------------------------------
 // Purpose: 
//-----------------------------------------------------------------------------
void gameui2::FadeOut_Int( int &fade_from, int fade_to, float fade_duration, float &fade_realtime )
{
	// TODO
}
    // ======= Copyright Valve Corporation, Chris and Nicolas. All rights reserved. ======= //
   //			
  // Purpose:	Open Source GameUI for Source Engine.
 //				
// ======================================================================================== //
#pragma once

#include "chunk_handler.h"
#include <vector>
#include <sstream>
#include <iostream>
#include <string>
#include "gameui2_button_mainmenu.h"

class CMenuParser : public CChunkHandler
{
public:
	//std::vector< configStruct* > m_Configs;
	CChunk *currentChunk;
	// maybe add a current config var
	int currentConfig;

	vgui::Panel *ourpanelptr;

	std::string m_sAlign;

	CUtlVector< CGameUI2_Button_MainMenu* > *m_Button_MM; // Main Menu
	CUtlVector< CGameUI2_Button_MainMenu* > *m_Button_PM; // Pause Menu
	CUtlVector< CGameUI2_Button_MainMenu* > *m_Button_SM; // Shared Menu

	CMenuParser(vgui::Panel *thepanel,
				CUtlVector< CGameUI2_Button_MainMenu* > *Button_MM,
				CUtlVector< CGameUI2_Button_MainMenu* > *Button_PM,
				CUtlVector< CGameUI2_Button_MainMenu* > *Button_SM)
	{
		ourpanelptr = thepanel;
		m_Button_MM = Button_MM;
		m_Button_PM = Button_PM;
		m_Button_SM = Button_SM;
		currentConfig = 0;
		m_sAlign = "";
	}

	void updateRootChunk()
	{

	}

	bool SaveGameConfig( const char* script )
	{
		// this doesnt save anything new right now
		// to do that you need to add code for updateRootChunk()
		updateRootChunk();
		WriteToFile(script);
		return true;
	}

	const char* GetAlign()
	{
		return m_sAlign.c_str();
	}

	bool LoadMenuConfig( const char* script )
	{
		m_pRoot = ReadChunkFile( script );
		if( !m_pRoot )
			return 0;
		
		CChunk *MenusChunk = m_pRoot->FindChunk( "Root" );

		std::string align = "";
		CKeyValue *align_key = MenusChunk->FindKey( "align" );
		if( align_key )
		{
			align = align_key->m_pValue;
		}
		m_sAlign = align;

		std::string textalign = "";
		CKeyValue *textalign_key = MenusChunk->FindKey( "label-align" );
		if( textalign_key )
		{
			textalign = textalign_key->m_pValue;
		}

		int button_width = 0;
		CKeyValue *button_width_key = MenusChunk->FindKey( "width" );
		if( button_width_key )
		{
			button_width = atoi( button_width_key->m_pValue );
		}

		int button_height = 0;
		CKeyValue *button_height_key = MenusChunk->FindKey( "height" );
		if( button_height_key )
		{
			button_height = atoi( button_height_key->m_pValue );
		}

		CChunk *MainMenuChunk = MenusChunk->FindChunk( "MainMenu" );
		for( unsigned short i = MainMenuChunk->m_Chunks.Head(); i != MainMenuChunk->m_Chunks.InvalidIndex(); i = MainMenuChunk->m_Chunks.Next( i ) )
		{
			CChunk *menuObject = MainMenuChunk->m_Chunks[i];
			
			std::string objectname = menuObject->m_pChunkName;
			if( objectname == "Button" )
			{
				std::string text = "";
				CKeyValue *text_key = menuObject->FindKey( "label" );
				if( text_key )
				{
					text = text_key->m_pValue;
				}

				std::string cmd = "";
				CKeyValue *cmd_key = menuObject->FindKey( "command" );
				if( cmd_key )
				{
					cmd = cmd_key->m_pValue;
				}

				int order = 0;
				CKeyValue *order_key = menuObject->FindKey( "order" );
				if( order_key )
				{
					order = atoi( order_key->m_pValue );
				}
				
				m_Button_MM->AddToTail( new CGameUI2_Button_MainMenu( ourpanelptr, "", text.c_str(), ourpanelptr, cmd.c_str(), textalign.c_str(), button_width, button_height, order ) );
			}
		}

		CChunk *PauseMenuChunk = MenusChunk->FindChunk( "PauseMenu" );
		for( unsigned short i = PauseMenuChunk->m_Chunks.Head(); i != PauseMenuChunk->m_Chunks.InvalidIndex(); i = PauseMenuChunk->m_Chunks.Next( i ) )
		{
			CChunk *menuObject = PauseMenuChunk->m_Chunks[i];
			
			std::string objectname = menuObject->m_pChunkName;
			if( objectname == "Button" )
			{
				std::string text = "";
				CKeyValue *text_key = menuObject->FindKey( "label" );
				if( text_key )
				{
					text = text_key->m_pValue;
				}

				std::string cmd = "";
				CKeyValue *cmd_key = menuObject->FindKey( "command" );
				if( cmd_key )
				{
					cmd = cmd_key->m_pValue;
				}

				int order = 0;
				CKeyValue *order_key = menuObject->FindKey( "order" );
				if( order_key )
				{
					order = atoi(order_key->m_pValue);
				}

				m_Button_PM->AddToTail( new CGameUI2_Button_MainMenu( ourpanelptr, "", text.c_str(), ourpanelptr, cmd.c_str(), textalign.c_str(), button_width, button_height, order ) );
			}
		}

		CChunk *SharedMenuChunk = MenusChunk->FindChunk( "SharedMenu" );
		for( unsigned short i = SharedMenuChunk->m_Chunks.Head(); i != SharedMenuChunk->m_Chunks.InvalidIndex(); i = SharedMenuChunk->m_Chunks.Next( i ) )
		{
			CChunk *menuObject = SharedMenuChunk->m_Chunks[i];
			
			std::string objectname = menuObject->m_pChunkName;
			if( objectname == "Button" )
			{
				std::string text = "";
				CKeyValue *text_key = menuObject->FindKey( "label" );
				if( text_key )
				{
					text = text_key->m_pValue;
				}

				std::string cmd = "";
				CKeyValue *cmd_key = menuObject->FindKey( "command" );
				if( cmd_key )
				{
					cmd = cmd_key->m_pValue;
				}

				int order = 0;
				CKeyValue *order_key = menuObject->FindKey( "order" );
				if( order_key )
				{
					order = atoi( order_key->m_pValue );
				}

				m_Button_SM->AddToTail( new CGameUI2_Button_MainMenu( ourpanelptr, "", text.c_str(), ourpanelptr, cmd.c_str(), textalign.c_str(), button_width, button_height, order ) );
			}
		}

		return 1;
	}
};
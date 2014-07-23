//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
// load_chunkfile.cpp : Defines the entry point for the console application.
//

#include "cbase.h"
#include "load_chunkfile.h"

char *CopyString(const char *in)
{
	if (!in)
		return NULL;

	int len = Q_strlen(in);
	char *out = new char[len + 1];
	Q_memcpy(out, in, len);
	out[len] = 0;
	return out;
}

// --------------------------------------------------------------------------------- //
// CChunk implementation.
// --------------------------------------------------------------------------------- //

bool CChunk::CompareKey( char const *pKeyName, char const *pValueStr )
{
	CKeyValue *pKey = FindKey( pKeyName );

	if( pKey && stricmp( pKey->m_pValue, pValueStr ) == 0 )
		return true;
	else
		return false;
}	


CKeyValue* CChunk::FindKey( char const *pKeyName )
{
	for( unsigned short i=m_Keys.Head(); i != m_Keys.InvalidIndex(); i=m_Keys.Next(i) )
	{
		CKeyValue *pKey = m_Keys[i];

		if( stricmp( pKey->m_pKey, pKeyName ) == 0 )
			return pKey;
	}

	return NULL;
}


CChunk* CChunk::FindChunk( char const *pChunkName )
{
	for( unsigned short i=m_Chunks.Head(); i != m_Chunks.InvalidIndex(); i=m_Chunks.Next(i) )
	{
		CChunk *pChunk = m_Chunks[i];

		if( stricmp( pChunk->m_pChunkName, pChunkName ) == 0 )
			return pChunk;
	}

	return NULL;
}

CKeyValue* CChunk::FindKey( char const *pKeyName, const char *pValue )
{
	for( unsigned short i=m_Keys.Head(); i != m_Keys.InvalidIndex(); i=m_Keys.Next(i) )
	{
		CKeyValue *pKey = m_Keys[i];

		if( stricmp( pKey->m_pKey, pKeyName ) == 0 && stricmp( pKey->m_pValue, pValue ) == 0 )
			return pKey;
	}

	return NULL;
}


void CChunk::RenameKey( const char *szOldName, const char *szNewName )
{
	if ((!szOldName) || (!szNewName))
		return;

	CKeyValue *pKey = FindKey( szOldName );
	if ( pKey )
	{
		delete pKey->m_pKey;
		pKey->m_pKey = CopyString( szNewName );
	}
}
#ifndef CHUNK_HANDLER_H
#define CHUNK_HANDLER_H

#include "load_chunkfile.h"
#include "stdio.h"
#include "tier1/strtools.h"
#include <new.h>
#include "chunkfile2.h"
#include "utlrbtree.h"
#include <sstream>

static unsigned long g_CurLoadOrder = 0;
static CChunk		*g_pCurChunk = 0;
static CChunkFile	*g_pChunkFile = 0;
static CChunk* ParseChunk( char const *pChunkName, bool bOnlyOne );

static char *CopyString(const char *in)
{
	if (!in)
		return NULL;

	int len = Q_strlen(in);
	char *out = new char[len + 1];
	Q_memcpy(out, in, len);
	out[len] = 0;
	return out;
}


static ChunkFileResult_t MyDefaultHandler( CChunkFile *pFile, void *pData, char const *pChunkName )
{
	CChunk *pChunk = ParseChunk( pChunkName, true );
	g_pCurChunk->m_Chunks.AddToTail( pChunk );
	return ChunkFile_Ok;
}

static ChunkFileResult_t MyKeyHandler( const char *szKey, const char *szValue, void *pData )
{
	// Add the key to the current chunk.
	CKeyValue *pKey = new CKeyValue;
	pKey->m_LoadOrder = g_CurLoadOrder++;

	pKey->m_pKey   = CopyString( szKey );
	pKey->m_pValue = CopyString( szValue );

	g_pCurChunk->m_Keys.AddToTail( pKey );
	return ChunkFile_Ok;
}

static CChunk* ParseChunk( char const *pChunkName, bool bOnlyOne )
{
	// Add the new chunk.
	CChunk *pChunk = new CChunk;
	pChunk->m_pChunkName = CopyString( pChunkName );
	pChunk->m_LoadOrder = g_CurLoadOrder++;

	// Parse it out..
	CChunk *pOldChunk = g_pCurChunk;
	g_pCurChunk = pChunk;

	//if( ++g_DotCounter % 16 == 0 )
	//	printf( "." );

	while( 1 )
	{
		if( g_pChunkFile->ReadChunk( MyKeyHandler ) != ChunkFile_Ok )
			break;

		if( bOnlyOne )
			break;
	}

	g_pCurChunk = pOldChunk;
	return pChunk;
}

static void WriteChunks_R( CChunkFile *pFile, CChunk *pChunk, bool bRoot )
{
	if( !bRoot )
	{
		pFile->BeginChunk( pChunk->m_pChunkName );
	}

	// Sort them..
	CUtlRBTree<CChunkHolder,int> sortedStuff( 0, 0, &CChunkHolder::SortChunkFn );

	// Write keys.
	for( unsigned short i=pChunk->m_Keys.Head(); i != pChunk->m_Keys.InvalidIndex(); i = pChunk->m_Keys.Next( i ) )
	{
		CChunkHolder holder;
		holder.m_pKey = pChunk->m_Keys[i];
		holder.m_LoadOrder = holder.m_pKey->m_LoadOrder;
		holder.m_pChunk = NULL;
		sortedStuff.Insert( holder );
	}

	// Write subchunks.
	for( int i=pChunk->m_Chunks.Head(); i != pChunk->m_Chunks.InvalidIndex(); i = pChunk->m_Chunks.Next( i ) )
	{
		CChunkHolder holder;
		holder.m_pChunk = pChunk->m_Chunks[i];
		holder.m_LoadOrder = holder.m_pChunk->m_LoadOrder;
		holder.m_pKey = NULL;
		sortedStuff.Insert( holder );
	}

	// Write stuff in sorted order.
	int i = sortedStuff.FirstInorder();
	if ( i != sortedStuff.InvalidIndex() )
	{
		while ( 1 )
		{
			CChunkHolder &h = sortedStuff[i];
			if ( h.m_pKey )
			{
				pFile->WriteKeyValue( h.m_pKey->m_pKey, h.m_pKey->m_pValue );
			}
			else
			{
				WriteChunks_R( pFile, h.m_pChunk, false );
			}
			if ( i == sortedStuff.LastInorder() )
				break;
			i = sortedStuff.NextInorder( i );
		}
	}

	if( !bRoot )
	{
		pFile->EndChunk();
	}
}


static bool WriteChunkFile( char const *pOutFilename, CChunk *pRoot, HANDLE hCon )
{
	CChunkFile chunkFile;

	if( chunkFile.Open( pOutFilename, ChunkFile_Write ) != ChunkFile_Ok )
	{

		ConWarning("Error opening chunk file %s for writing.\n", pOutFilename);

		return false;
	}

	ConWarning("attempting to open chunk file %s for writing.\n", pOutFilename);
	ConWarning("Writing...\n");
	WriteChunks_R( &chunkFile, pRoot, true );
	ConWarning("\n\n");

	return true;
}

class CChunkHandler
{
public:
	CChunk *m_pRoot;
	HANDLE hCon;
	CChunkHandler()
	{
		m_pRoot = NULL;
	}
	bool WriteToFile( char const *pOutFilename)
	{
		if (m_pRoot == NULL)
		{
			ConWarning("error: root chunk is NULL!\n");
		}
			
		return WriteChunkFile( pOutFilename, m_pRoot, hCon );
	}
	void printKey(CKeyValue *the_key)
	{
		//ConColorMsg
		ConWarning("%s: ", the_key->m_pKey);
		
		//ConColorMsg
		ConWarning("%s\n", the_key->m_pValue);
	}

	CChunk* ReadChunkFile( char const *pInFilename )
	{
		CChunkFile chunkFile;

		if( chunkFile.Open( pInFilename, ChunkFile_Read ) != ChunkFile_Ok )
		{
			ConWarning("Error opening chunk file %s for reading.\n", pInFilename);

			return NULL;
		}
		std::stringstream strstrm1;
		strstrm1 << "Reading chunk file: " << pInFilename << "\n";
		
		ConWarning(strstrm1.str().c_str());
		
		chunkFile.SetDefaultChunkHandler( MyDefaultHandler, 0 );
		g_pChunkFile = &chunkFile;
	
		CChunk *pRet = ParseChunk( "***ROOT***", false );
		//printf( "\n" );

		return pRet;
	}
};

#endif
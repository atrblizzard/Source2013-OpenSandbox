#ifndef LOAD_CHUNKFILE_H
#define LOAD_CHUNKFILE_H

#include "utllinkedlist.h"
//#include "util.h"

static char *CopyString(const char *in);


class CChunkKeyBase
{
public:
	unsigned long m_LoadOrder;	// Which order it appeared in the file.
};

class CKeyValue : public CChunkKeyBase
{

public:
	CKeyValue(){}
	CKeyValue(unsigned long loadorder, char *the_key, char *the_value)
	{
		m_LoadOrder = loadorder;
		m_pKey = the_key;
		m_pValue = the_value;
	}
	void SetKey( const char *pStr )
	{
		delete [] m_pKey;
		m_pKey = CopyString( pStr );
	}

	void SetValue( const char *pStr )
	{
		delete [] m_pValue;
		m_pValue = CopyString( pStr );
	}
	

public:
	char	*m_pKey;
	char	*m_pValue;
};

class CChunk : public CChunkKeyBase
{
public:
	// Returns true if the chunk has the specified key and if its value
	// is equal to pValueStr (case-insensitive).
	bool			CompareKey( char const *pKeyName, char const *pValueStr );

	// Look for a key by name.
	CKeyValue*		FindKey( char const *pKeyName );
	CKeyValue*		FindKey( char const *pKeyName, const char *pValue );

	CChunk*			FindChunk( char const *pKeyName );

	// Find a key by name, and replace any occurences with the new name.
	void			RenameKey( const char *szOldName, const char *szNewName );

public:

	char										*m_pChunkName;
	CUtlLinkedList<CKeyValue*, unsigned short>	m_Keys;
	CUtlLinkedList<CChunk*, unsigned short>		m_Chunks;
};

class CChunkHolder
{
public:
	static bool SortChunkFn( const CChunkHolder &a, const CChunkHolder &b )
	{
		return a.m_LoadOrder < b.m_LoadOrder;
	}

	unsigned long m_LoadOrder;
	CKeyValue *m_pKey;
	CChunk *m_pChunk;
};


#endif
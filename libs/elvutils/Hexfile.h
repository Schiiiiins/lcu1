// Hexfile.h: Schnittstelle für die Klasse Hexfile.
//
//////////////////////////////////////////////////////////////////////

#ifndef _HEXFILE_H_
#define _HEXFILE_H_

#include "dllexport.h"
#include "typedefs.h"	// Hinzugefügt von der Klassenansicht
#include <string>

class ELVUTILS_DLLEXPORT Hexfile  
{
public:
	const std::string& GetFilename();
	int Modify(int address, const ucVec& data);
	int GetStart();
	ucVec& GetBuffer();
	int Read(const std::string& filename);
	Hexfile();
	virtual ~Hexfile();

protected:
	unsigned char Hexbyte(const char* s);
	int start;
	ucVec buffer;
	std::string filename;
};

#endif

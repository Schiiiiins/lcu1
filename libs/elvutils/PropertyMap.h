// PropertyMap.h: Schnittstelle für die Klasse PropertyMap.
//
//////////////////////////////////////////////////////////////////////

#ifndef _PROPERTYMAP_H_
#define _PROPERTYMAP_H_

#include "dllexport.h"
#include <map>
#include <vector>
#include <string>

class ELVUTILS_DLLEXPORT PropertyMap  
{
public:
    typedef std::vector<std::string> StringList;
	typedef std::map<std::string, std::string> Section;
	void Clear();
	const std::string GetStringValue(const std::string& key, const std::string& default_value="");
	int GetIntValue(const std::string& key, int default_value=0);
	double GetFloatValue(const std::string& key, double default_value=0.0);
	std::string GetBinaryValue(const std::string& key, const std::string& default_value="");
    void SetValue(const std::string& key, const std::string& value);
	void SetIntValue(const std::string& key, const int& value);
    void SetBinaryValue(const std::string& key, const std::string& value);
	inline void SetStringValue(const std::string& key, const std::string& value)
	{ 
		SetValue(key, value);
	};
    StringList ListSections();
    Section GetSection(const std::string& section);
    bool SetCurrentSection(const std::string& section_name);
    const std::string& GetCurrentSection();
	int ReadFromFile(std::string filename);
    int WriteToFile(std::string filename="");
    bool IsDirty();
    void SetWriteSpacesFlag(bool b);
	bool HasSection(const std::string& section_name);
	typedef std::map<std::string, Section> t_sectionMap;
	PropertyMap();
	virtual ~PropertyMap();
protected:
    t_sectionMap section_map;
    std::string filename;
    std::string current_section;
    bool dirty;
    bool write_spaces;
};

#endif

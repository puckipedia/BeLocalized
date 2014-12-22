#ifndef POOTLE_API_POOTLE_LANGUAGES_ENDPOINT_H
#define POOTLE_API_POOTLE_LANGUAGES_ENDPOINT_H

#include "PootleEndpoint.h"

#include <ObjectList.h>

#include <map>

class PootleTranslationProject;
class PootleLanguagesEndpoint;

class PootleLanguage {
public:
	BString						LanguageCode();
	BString						Description();
	BString						FullName();
	int							NumberOfPlurals();
	BString						PluralEquation();
	BString						SpecialCharacters();
	int 						CountTranslationProjects();
	PootleTranslationProject	GetTranslationProject(int index);

	BString ResourceUri() { return mUri; }

								PootleLanguage() {}
protected:
	friend class PootleLanguagesEndpoint;
	PootleLanguage(PootleLanguagesEndpoint *, BMessage &);
	PootleLanguage(PootleLanguagesEndpoint *, int);
	PootleLanguage(PootleLanguagesEndpoint *, BString);
private:
	void						_EnsureData();
	PootleLanguagesEndpoint *mEndpoint;
	BString mUri;
	BMessage mData;
};

class PootleLanguagesEndpoint : public PootleEndpoint {
public:
	PootleLanguagesEndpoint(Pootle *pootle)
		: PootleEndpoint(pootle, "languages/") {}

	typedef PootleLanguage Data;

	BObjectList<PootleLanguage> Get(int limit = -1, int offset = 0);
	PootleLanguage GetById(int id);
	PootleLanguage GetByUrl(BString);
	
	PootleLanguage _get_from_cache(int);
	PootleLanguage _get_from_cache(BString);
	
protected:
	friend class PootleLanguage;

	bool _cache_contains(int);
	bool _cache_contains(BString);
	
	void _add_to_cache(int, PootleLanguage);
	void _add_to_cache(BString, PootleLanguage);
private:
	static int _path_to_id(BString);	
	std::map<int, PootleLanguage> mLanguageEndpoints;
};

#endif

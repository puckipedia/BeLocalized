#ifndef POOTLE_API_POOTLE_LANGUAGES_ENDPOINT_H
#define POOTLE_API_POOTLE_LANGUAGES_ENDPOINT_H

#include "PootleEndpoint.h"

#include <ObjectList.h>

#include <map>

class PootleTranslationProject;
class PootleLanguagesEndpoint;

class PootleLanguage {
public:
	typedef PootleEndpoint<PootleLanguage> _Endpoint;

	BString									LanguageCode();
	BString									Description();
	BString									FullName();
	int										NumberOfPlurals();
	BString									PluralEquation();
	BString									SpecialCharacters();
	BObjectList<PootleTranslationProject>	TranslationProjects();
	int 									CountTranslationProjects();
	PootleTranslationProject				GetTranslationProject(int index);

	BString ResourceUri() { return mUri; }

	PootleLanguage() {}
	PootleLanguage(_Endpoint *, BMessage &);
	PootleLanguage(_Endpoint *, int);
	PootleLanguage(_Endpoint *, BString);
private:
	void		_EnsureData();
	_Endpoint	*mEndpoint;
	BString		mUri;
	BMessage	mData;
};

class PootleLanguagesEndpoint : public PootleEndpoint<PootleLanguage> {
public:
	PootleLanguagesEndpoint(Pootle *pootle)
		: PootleEndpoint<PootleLanguage>(pootle, "languages/") {}

	BObjectList<PootleLanguage> Get(int limit = -1, int offset = 0);
protected:
	friend class PootleLanguage;
};

#endif

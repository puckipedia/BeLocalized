#ifndef POOTLE_API_POOTLE_SUGGESTIONS_ENDPOINT_H
#define POOTLE_API_POOTLE_SUGGESTIONS_ENDPOINT_H

#include "PootleEndpoint.h"

#include <ObjectList.h>

#include <map>

class PootleSuggestionsEndpoint;
class PootleTranslationProject;
class PootleUnit;


class PootleSuggestion {
public:
	PootleUnit		Unit();
	BString			Target();
	BString			TranslatorComment();
	BString			ResourceUri() { return mUri; }

	void			SetTarget(const char *);
	void			SetTranslatorComment(const char *);
	void			SetUnit(PootleUnit &unit);

	void			Put();
	void			Create(PootleSuggestionsEndpoint *);

	PootleSuggestion() {}

protected:
	friend class PootleSuggestionsEndpoint;
	PootleSuggestion(PootleSuggestionsEndpoint *, BMessage &);
	PootleSuggestion(PootleSuggestionsEndpoint *, int);
	PootleSuggestion(PootleSuggestionsEndpoint *, BString);

private:
	void						_EnsureData();
	PootleSuggestionsEndpoint		*mEndpoint;
	BString						mUri;
	BMessage					mData;
};

class PootleSuggestionsEndpoint : public PootleEndpoint {
public:
	PootleSuggestionsEndpoint(Pootle *pootle)
		: PootleEndpoint(pootle, "suggestions/") {}

	typedef PootleSuggestion Data;

	PootleSuggestion					GetByUrl(BString);
	PootleSuggestion					GetById(int id);
	BObjectList<PootleSuggestion>		GetByList(BObjectList<BString>);

protected:
	friend class PootleSuggestion;
		
	PootleSuggestion _get_from_cache(int);
	PootleSuggestion _get_from_cache(BString);

	bool _cache_contains(int);
	bool _cache_contains(BString);
	
	void _add_to_cache(int, PootleSuggestion);
	void _add_to_cache(BString, PootleSuggestion);

private:
	static int _path_to_id(BString);
	std::map<int, PootleSuggestion> mLanguageEndpoints;
};

#endif

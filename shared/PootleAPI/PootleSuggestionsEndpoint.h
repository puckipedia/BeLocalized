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
	typedef PootleEndpoint<PootleSuggestion> _Endpoint;

	PootleUnit		Unit();
	BString			Target();
	BString			TranslatorComment();
	BString			ResourceUri() { return mUri; }

	void			SetTarget(const char *);
	void			SetTranslatorComment(const char *);
	void			SetUnit(PootleUnit &unit);

	bool			Put();
	bool			Create(PootleSuggestionsEndpoint *);

	PootleSuggestion() {}
	PootleSuggestion(_Endpoint *, BMessage &);
	PootleSuggestion(_Endpoint *, int);
	PootleSuggestion(_Endpoint *, BString);

private:
	void		_EnsureData();
	_Endpoint	*mEndpoint;
	BString		mUri;
	BMessage	mData;
};

class PootleSuggestionsEndpoint : public PootleEndpoint<PootleSuggestion> {
public:
	PootleSuggestionsEndpoint(Pootle *pootle)
		: PootleEndpoint<PootleSuggestion>(pootle, "suggestions/") {}
protected:
	friend class PootleSuggestion;
};

#endif

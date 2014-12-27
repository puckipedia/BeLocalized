#ifndef POOTLE_API_POOTLE_UNITS_ENDPOINT_H
#define POOTLE_API_POOTLE_UNITS_ENDPOINT_H

#include "PootleEndpoint.h"

#include <ObjectList.h>

#include <map>

class PootleUnitsEndpoint;
class PootleTranslationProject;
class PootleStore;
class PootleSuggestion;
class PootleUser;

struct Translation {
	BString	text;
	int		length;
	int		wordcount;
};

class PootleUnit {
public:
	typedef PootleEndpoint<PootleUnit> _Endpoint;
	PootleUser						CommentedBy();
	BString							CommentedOn();
	BString							Context();
	BString							DeveloperComment();
	BString							Locations();
	BString							MTime();
	Translation						Source();
	int								State();
	PootleStore						Store();
	BString							SubmittedOn();
	PootleUser						SubmittedBy();
	BObjectList<PootleSuggestion>	Suggestions();
	int								CountSuggestions();
	PootleSuggestion				GetSuggestion(int);
	Translation						Target();
	BString							TranslatorComment();

	BString ResourceUri() { return mUri; }

	PootleUnit() {}
	PootleUnit(_Endpoint *, BMessage &);
	PootleUnit(_Endpoint *, int);
	PootleUnit(_Endpoint *, BString);

private:
	void			_EnsureData();
	_Endpoint		*mEndpoint;
	BString			mUri;
	BMessage		mData;
};

class PootleUnitsEndpoint : public PootleEndpoint<PootleUnit> {
public:
	PootleUnitsEndpoint(Pootle *pootle)
		: PootleEndpoint<PootleUnit>(pootle, "units/") {}
protected:
	friend class PootleUnit;
};

#endif

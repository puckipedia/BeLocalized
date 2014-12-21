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
	PootleUser			CommentedBy();
	BString				CommentedOn();
	BString				Context();
	BString				DeveloperComment();
	BString				Locations();
	BString				MTime();
	Translation			Source();
	int					State();
	PootleStore			Store();
	BString				SubmittedOn();
	PootleUser			SubmittedBy();
	int					CountSuggestions();
	PootleSuggestion	GetSuggestion(int);
	Translation			Target();
	BString				TranslatorComment();

	PootleUnit() {}

protected:
	friend class PootleUnitsEndpoint;
	PootleUnit(PootleUnitsEndpoint *, BMessage &);
	PootleUnit(PootleUnitsEndpoint *, int);
	PootleUnit(PootleUnitsEndpoint *, BString);

private:
	void						_EnsureData();
	PootleUnitsEndpoint		*mEndpoint;
	BString						mUri;
	BMessage					mData;
};

class PootleUnitsEndpoint : public PootleEndpoint {
public:
	PootleUnitsEndpoint(Pootle *pootle)
		: PootleEndpoint(pootle, "units/") {}

	PootleUnit					GetByUrl(BString);
	PootleUnit					GetById(int id);
protected:
	friend class PootleUnit;
		
	PootleUnit _get_from_cache(int);
	PootleUnit _get_from_cache(BString);

	bool _cache_contains(int);
	bool _cache_contains(BString);
	
	void _add_to_cache(int, PootleUnit);
	void _add_to_cache(BString, PootleUnit);

private:
	static int _path_to_id(BString);
	std::map<int, PootleUnit> mLanguageEndpoints;
};

#endif

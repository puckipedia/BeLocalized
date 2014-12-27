#ifndef POOTLE_API_POOTLE_TRANSLATION_PROJECTS_ENDPOINT_H
#define POOTLE_API_POOTLE_TRANSLATION_PROJECTS_ENDPOINT_H

#include "PootleEndpoint.h"

#include <ObjectList.h>

#include <map>

class PootleTranslationProjectsEndpoint;
class PootleLanguage;
class PootleStore;
class PootleProject;

class PootleTranslationProject {
public:
	typedef PootleEndpoint<PootleTranslationProject> _Endpoint;

	BString						Description();
	PootleLanguage				Language();
	BString						PootlePath();
	PootleProject				Project();
	BString						RealPath();
	BObjectList<PootleStore>	Stores();
	int							CountStores();
	PootleStore					GetStore(int);

	BString ResourceUri() { return mUri; }

	PootleTranslationProject() {}
	PootleTranslationProject(_Endpoint *, BMessage &);
	PootleTranslationProject(_Endpoint *, int);
	PootleTranslationProject(_Endpoint *, BString);

private:
	void		_EnsureData();
	_Endpoint	*mEndpoint;
	BString		mUri;
	BMessage	mData;
};

class PootleTranslationProjectsEndpoint
	: public PootleEndpoint<PootleTranslationProject> {
public:
	PootleTranslationProjectsEndpoint(Pootle *pootle)
		: PootleEndpoint<PootleTranslationProject>(pootle,
			"translation-projects/") {}
protected:
	friend class PootleTranslationProject;
};

#endif

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
	BString						Description();
	PootleLanguage				Language();
	BString						PootlePath();
	PootleProject				Project();
	BString						RealPath();
	int							CountStores();
	PootleStore					GetStore(int);

	PootleTranslationProject() {}

protected:
	friend class PootleTranslationProjectsEndpoint;
	PootleTranslationProject(PootleTranslationProjectsEndpoint *, BMessage &);
	PootleTranslationProject(PootleTranslationProjectsEndpoint *, int);
	PootleTranslationProject(PootleTranslationProjectsEndpoint *, BString);

private:
	void									_EnsureData();
	PootleTranslationProjectsEndpoint		*mEndpoint;
	BString									mUri;
	BMessage								mData;
};

class PootleTranslationProjectsEndpoint : public PootleEndpoint {
public:
	PootleTranslationProjectsEndpoint(Pootle *pootle)
		: PootleEndpoint(pootle, "translation-projects/") {}

	BObjectList<PootleTranslationProject>	Get(int limit = -1, int offset = 0);
	PootleTranslationProject				GetByUrl(BString);
	PootleTranslationProject				GetById(int id);
protected:
	friend class PootleTranslationProject;
		
	PootleTranslationProject _get_from_cache(int);
	PootleTranslationProject _get_from_cache(BString);

	bool _cache_contains(int);
	bool _cache_contains(BString);
	
	void _add_to_cache(int, PootleTranslationProject);
	void _add_to_cache(BString, PootleTranslationProject);

private:
	static int _path_to_id(BString);
	std::map<int, PootleTranslationProject> mLanguageEndpoints;
};

#endif

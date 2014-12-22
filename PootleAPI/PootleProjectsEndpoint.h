#ifndef POOTLE_API_POOTLE_PROJECTS_ENDPOINT_H
#define POOTLE_API_POOTLE_PROJECTS_ENDPOINT_H

#include "PootleEndpoint.h"

#include <ObjectList.h>

#include <map>

class PootleLanguage;
class PootleProjectsEndpoint;
class PootleTranslationProject;

class PootleProject {
public:
	BString						Backlink();
	BString						CheckStyle();
	BString						Code();
	BString						Description();
	BString						FullName();
	BString						IgnoredFiles();
	BString						LocalFileType();
	BString						ResourceURI();
	PootleLanguage				SourceLanguage();
	int							CountTranslationProjects();
	PootleTranslationProject	GetTranslationProject(int index);
	BObjectList<BString>		GetTranslationProjectUrls();
	BString						TreeStyle();

	BString ResourceUri() { return mUri; }

	PootleProject() {}

protected:
	friend class PootleProjectsEndpoint;
	PootleProject(PootleProjectsEndpoint *, BMessage &);
	PootleProject(PootleProjectsEndpoint *, int);
	PootleProject(PootleProjectsEndpoint *, BString);

private:
	void						_EnsureData();
	PootleProjectsEndpoint		*mEndpoint;
	BString						mUri;
	BMessage					mData;
};

class PootleProjectsEndpoint : public PootleEndpoint {
public:
	PootleProjectsEndpoint(Pootle *pootle)
		: PootleEndpoint(pootle, "projects/") {}

	typedef PootleProject Data;

	BObjectList<PootleProject>	Get(int limit = -1, int offset = 0);
	PootleProject				GetByUrl(BString);
	PootleProject				GetById(int id);
protected:
	friend class PootleProject;
		
	PootleProject _get_from_cache(int);
	PootleProject _get_from_cache(BString);

	bool _cache_contains(int);
	bool _cache_contains(BString);
	
	void _add_to_cache(int, PootleProject);
	void _add_to_cache(BString, PootleProject);

private:
	static int _path_to_id(BString);
	std::map<int, PootleProject> mLanguageEndpoints;
};

#endif

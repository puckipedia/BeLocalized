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
	typedef	PootleEndpoint<PootleProject>	_Endpoint;

	BString									Backlink();
	BString									CheckStyle();
	BString									Code();
	BString									Description();
	BString									FullName();
	BString									IgnoredFiles();
	BString									LocalFileType();
	BString									ResourceURI();
	PootleLanguage							SourceLanguage();
	int										CountTranslationProjects();
	PootleTranslationProject				GetTranslationProject(int index);
	BObjectList<PootleTranslationProject>	TranslationProjects();
	BString									TreeStyle();

	BString ResourceUri() { return mUri; }

	PootleProject() {}
	PootleProject(_Endpoint *, BMessage &);
	PootleProject(_Endpoint *, int);
	PootleProject(_Endpoint *, BString);

private:
	void		_EnsureData();
	_Endpoint	*mEndpoint;
	BString		mUri;
	BMessage	mData;
};

class PootleProjectsEndpoint : public PootleEndpoint<PootleProject> {
public:
	PootleProjectsEndpoint(Pootle *pootle)
		: PootleEndpoint<PootleProject>(pootle, "projects/") {}

	BObjectList<PootleProject> Get(int limit = -1, int offset = 0);
protected:
	friend class PootleProject;
};

#endif

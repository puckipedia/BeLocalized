#include "PootleProjectsEndpoint.h"

#include "Pootle.h"
#include "PootleLanguagesEndpoint.h"
#include "PootleTranslationProjectsEndpoint.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

BString
PootleProject::Backlink()
{
	_EnsureData();
	return BString(mData.GetString("backlink", ""));
}


BString
PootleProject::CheckStyle()
{
	_EnsureData();
	return BString(mData.GetString("checkstyle", ""));
}


BString
PootleProject::Code()
{
	_EnsureData();
	return BString(mData.GetString("code", ""));
}


BString
PootleProject::Description()
{
	_EnsureData();
	return BString(mData.GetString("description", ""));
}


BString
PootleProject::FullName()
{
	_EnsureData();
	return BString(mData.GetString("fullname", ""));
}


BString
PootleProject::IgnoredFiles()
{
	_EnsureData();
	return BString(mData.GetString("ignoredfiles", ""));
}


BString
PootleProject::LocalFileType()
{
	_EnsureData();
	return BString(mData.GetString("localfiletype", ""));
}


BString
PootleProject::ResourceURI()
{
	_EnsureData();
	return BString(mData.GetString("resource_uri", ""));
}


int
PootleProject::CountTranslationProjects()
{
	_EnsureData();
	BMessage projects;
	mData.FindMessage("translation_projects", &projects);
	return projects.CountNames(B_ANY_TYPE);
}


PootleTranslationProject
PootleProject::GetTranslationProject(int index)
{
	char buffer[33];
	sprintf(buffer, "%d", index);

	_EnsureData();
	BMessage projects;
	mData.FindMessage("translation_projects", &projects);
	BString str = projects.GetString(buffer, "");

	return mEndpoint->mPootle->TranslationProjects()->GetByUrl(str);
}


BObjectList<BString>
PootleProject::GetTranslationProjectUrls()
{
	BMessage projects;
	mData.FindMessage("translation_projects", &projects);

	BObjectList<BString> returnval(20, true);
	
	int32 count = CountTranslationProjects();
	BString indexer;
	
	for (int32 i = 0; i < count; i++) {
		indexer.SetToFormat("%d", i);
		returnval.AddItem(new BString(projects.GetString(indexer, "")));
	}
	
	return returnval;
}


BString
PootleProject::TreeStyle()
{
	_EnsureData();
	return BString(mData.GetString("treestyle", ""));
}


PootleLanguage
PootleProject::SourceLanguage()
{
	_EnsureData();
	return mEndpoint->mPootle->Languages()->GetByUrl(
		mData.GetString("source_language", ""));
}


void
PootleProject::_EnsureData()
{
	if(!mData.IsEmpty())
		return;

	mData = mEndpoint->_SendRequest("GET", mUri);
	mEndpoint->_add_to_cache(mUri, *this);
}

PootleProject::PootleProject(PootleProjectsEndpoint *endpoint, BMessage &data)
	:
	mEndpoint(endpoint),
	mData(data),
	mUri(data.GetString("resource_uri", ""))
{
	mEndpoint->_add_to_cache(mUri, *this);
}


PootleProject::PootleProject(PootleProjectsEndpoint *endpoint, int id)
	:
	mEndpoint(endpoint)
{
	mUri.SetToFormat("%d/", id);
	if (mEndpoint->_cache_contains(id))
		*this = mEndpoint->_get_from_cache(id);
}


PootleProject::PootleProject(PootleProjectsEndpoint *endpoint, BString uri)
	:
	mEndpoint(endpoint),
	mUri(uri)
{
	if (mEndpoint->_cache_contains(uri))
		*this = mEndpoint->_get_from_cache(uri);
}


BObjectList<PootleProject>
PootleProjectsEndpoint::Get(int limit, int offset)
{
	BObjectList<PootleProject> objectlist(20, true);
	BString url;
	int32 count = -1;
	if (limit == -1)
		limit = 100;

	url.SetToFormat("?limit=%d&offset=%d", limit, offset);
	BMessage data = _GetAll(url, limit < 0 ? 0 : limit);
	data.GetInfo("data", NULL, &count);

	if (count < 0)
		return objectlist;

	for (int32 i = 0; i < count; i++) {
		BMessage msg;
		data.FindMessage("data", i, &msg);
		PootleProject *lang = new PootleProject(this, msg);
		objectlist.AddItem(lang);
	}
	
	return objectlist;
}

PootleProject
PootleProjectsEndpoint::GetById(int id)
{
	return PootleProject(this, id);
}


PootleProject
PootleProjectsEndpoint::GetByUrl(BString url)
{
	return PootleProject(this, url);
}


void
PootleProjectsEndpoint::_add_to_cache(int id, PootleProject lang)
{
	mLanguageEndpoints[id] = lang;
}


int
PootleProjectsEndpoint::_path_to_id(BString path)
{
	int32 index = path.FindLast("/", path.Length() - 2);
	if (index < 0)
		index = 0;
	else
		index += 1;

	int id = atoi(path.String() + index);
	return id;
}


void
PootleProjectsEndpoint::_add_to_cache(BString path, PootleProject lang)
{
	_add_to_cache(_path_to_id(path), lang);
}


bool
PootleProjectsEndpoint::_cache_contains(int id)
{
	return mLanguageEndpoints.count(id) != 0;
}


bool
PootleProjectsEndpoint::_cache_contains(BString path)
{
	return _cache_contains(_path_to_id(path));
}


PootleProject
PootleProjectsEndpoint::_get_from_cache(int id)
{
	return mLanguageEndpoints[id];
}


PootleProject
PootleProjectsEndpoint::_get_from_cache(BString path)
{
	return _get_from_cache(_path_to_id(path));
}


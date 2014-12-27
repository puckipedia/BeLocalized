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

	return mEndpoint->API()->TranslationProjects()->GetByUrl(str);
}


BObjectList<PootleTranslationProject>
PootleProject::TranslationProjects()
{
	BMessage tprojects;
	mData.FindMessage("translation_projects", &tprojects);

	BObjectList<BString> urls(20, true);
	
	int32 count = tprojects.CountNames(B_ANY_TYPE);

	char buffer[33];
	for (int32 i = 0; i < count; i++) {
		sprintf(buffer, "%d", i);
		urls.AddItem(new BString(tprojects.GetString(buffer, "")));
	}
	
	return mEndpoint->API()->TranslationProjects()->GetByList(urls);

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
	return mEndpoint->API()->Languages()->GetByUrl(
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

PootleProject::PootleProject(_Endpoint *endpoint, BMessage &data)
	:
	mEndpoint(endpoint),
	mData(data),
	mUri(data.GetString("resource_uri", ""))
{
	mEndpoint->_add_to_cache(mUri, *this);
}


PootleProject::PootleProject(_Endpoint *endpoint, int id)
	:
	mEndpoint(endpoint)
{
	mUri.SetToFormat("%d/", id);
	if (mEndpoint->_cache_contains(id))
		*this = mEndpoint->_get_from_cache(id);
}


PootleProject::PootleProject(_Endpoint *endpoint, BString uri)
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

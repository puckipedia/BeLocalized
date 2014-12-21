#include "PootleProjectsEndpoint.h"

#include "Pootle.h"
#include "PootleLanguagesEndpoint.h"

#include <assert.h>
#include <stdio.h>


BString
PootleTranslationProject::Description()
{
	_EnsureData();
	return BString(mData.GetString("description", ""));
}


PootleLanguage
PootleTranslationProject::Language()
{
	_EnsureData();
	return mEndpoint->mPootle->Languages()->GetByUrl(
		mData.GetString("language", ""));
}


BString
PootleTranslationProject::PootlePath()
{
	_EnsureData();
	return BString(mData.GetString("pootlepath", ""));
}


PootleProject
PootleTranslationProject::Project()
{
	_EnsureData();
	return mEndpoint->mPootle->Projects()->GetByUrl(
		mData.GetString("project", ""));
}


BString
PootleTranslationProject::RealPath()
{
	_EnsureData();
	return BString(mData.GetString("realpath", ""));
}



int
PootleTranslationProject::CountStores()
{
	_EnsureData();
	BMessage stores;
	mData.FindMessage("stores", &stores);
	return stores.CountNames(B_ANY_TYPE);
}


PootleStore
PootleTranslationProject::GetStore(int index)
{
	char buffer[33];
	sprintf(buffer, "%d", index);

	_EnsureData();
	BMessage stores;
	mData.FindMessage("stores", &stores);
	BString str = stores.GetString(buffer, "");

	return mEndpoint->mPootle->Stores()->GetByUrl(str);
}

void
PootleTranslationProject::_EnsureData()
{
	if(!mData.IsEmpty())
		return;

	mData = mEndpoint->_SendRequest("GET", mUri);
	mEndpoint->_add_to_cache(mUri, *this);
}

PootleTranslationProject::PootleTranslationProject(
	PootleTranslationProjectsEndpoint *endpoint, BMessage &data)
	:
	mEndpoint(endpoint),
	mData(data),
	mUri(data.GetString("resource_uri", ""))
{
	mEndpoint->_add_to_cache(mUri, *this);
}


PootleTranslationProject::PootleTranslationProject(
	PootleTranslationProjectsEndpoint *endpoint, int id)
	:
	mEndpoint(endpoint)
{
	mUri.SetToFormat("%d/", id);
	if (mEndpoint->_cache_contains(id))
		*this = mEndpoint->_get_from_cache(id);
}


PootleTranslationProject::PootleTranslationProject(
	PootleTranslationProjectsEndpoint *endpoint, BString uri)
	:
	mEndpoint(endpoint),
	mUri(uri)
{
	if (mEndpoint->_cache_contains(uri))
		*this = mEndpoint->_get_from_cache(uri);
}


BObjectList<PootleTranslationProject>
PootleTranslationProjectsEndpoint::Get(int limit, int offset)
{
	BObjectList<PootleTranslationProject> objectlist(20, true);
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
		PootleTranslationProject *lang
			= new PootleTranslationProject(this, msg);
		objectlist.AddItem(lang);
	}
	
	return objectlist;
}

PootleTranslationProject
PootleTranslationProjectsEndpoint::GetById(int id)
{
	return PootleTranslationProject(this, id);
}


PootleTranslationProject
PootleTranslationProjectsEndpoint::GetByUrl(BString url)
{
	return PootleTranslationProject(this, url);
}


void
PootleTranslationProjectsEndpoint::_add_to_cache(int id, PootleTranslationProject lang)
{
	mLanguageEndpoints[id] = lang;
}


int
PootleTranslationProjectsEndpoint::_path_to_id(BString path)
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
PootleTranslationProjectsEndpoint::_add_to_cache(BString path,
	PootleTranslationProject lang)
{
	_add_to_cache(_path_to_id(path), lang);
}


bool
PootleTranslationProjectsEndpoint::_cache_contains(int id)
{
	return mLanguageEndpoints.count(id) != 0;
}


bool
PootleTranslationProjectsEndpoint::_cache_contains(BString path)
{
	return _cache_contains(_path_to_id(path));
}


PootleTranslationProject
PootleTranslationProjectsEndpoint::_get_from_cache(int id)
{
	return mLanguageEndpoints[id];
}


PootleTranslationProject
PootleTranslationProjectsEndpoint::_get_from_cache(BString path)
{
	return _get_from_cache(_path_to_id(path));
}


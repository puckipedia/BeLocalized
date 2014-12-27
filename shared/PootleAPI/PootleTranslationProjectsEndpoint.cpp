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
	return BString(mData.GetString("pootle_path", ""));
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
	return BString(mData.GetString("real_path", ""));
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


BObjectList<PootleTranslationProject>
PootleTranslationProjectsEndpoint::GetByList(BObjectList<BString> list)
{
	BObjectList<PootleTranslationProject> returnList(20, true);
	char buffer[34];
	
	BString setUrl = mBaseEndpoint.Path() + "set/";
	for (int32 i = 0; i < list.CountItems(); i++) {
		BString *str = list.ItemAt(i);
		sprintf(buffer, "%d;", _path_to_id(*str));
		setUrl.Append(buffer);
	}

	setUrl.RemoveLast(";");
	setUrl.Append("/");
	
	BMessage ret = _SendRequest("GET", setUrl);
	BMessage objects;
	ret.FindMessage("object", &objects);
	int32 count = objects.CountNames(B_ANY_TYPE);
	for (int32 i = 0; i < count; i++) {
		sprintf(buffer, "%d", i);
		BMessage msg;
		objects.FindMessage(buffer, &msg);
		returnList.AddItem(new PootleTranslationProject(this, msg));
	}
	
	return returnList;
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


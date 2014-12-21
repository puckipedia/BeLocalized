#include "PootleStoresEndpoint.h"

#include "Pootle.h"
#include "PootleTranslationProjectsEndpoint.h"

#include <assert.h>
#include <stdio.h>


BString
PootleStore::File()
{
	_EnsureData();
	return BString(mData.GetString("file", ""));
}


PootleTranslationProject
PootleStore::TranslationProject()
{
	_EnsureData();
	return mEndpoint->mPootle->TranslationProjects()->GetByUrl(
		mData.GetString("translation_project", ""));
}


BString
PootleStore::Name()
{
	_EnsureData();
	return BString(mData.GetString("name", ""));
}


BString
PootleStore::PootlePath()
{
	_EnsureData();
	return BString(mData.GetString("pootlepath", ""));
}


int
PootleStore::State()
{
	_EnsureData();
	return (int)mData.GetDouble("state", -1.0);
}


BString
PootleStore::SyncTime()
{
	_EnsureData();
	return BString(mData.GetString("synctime", ""));
}


int
PootleStore::CountUnits()
{
	_EnsureData();
	BMessage units;
	mData.FindMessage("units", &units);
	return units.CountNames(B_ANY_TYPE);
}


PootleUnit
PootleStore::GetUnit(int index)
{
	char buffer[33];
	sprintf(buffer, "%d", index);

	_EnsureData();
	BMessage units;
	mData.FindMessage("units", &units);
	BString str = units.GetString(buffer, "");

	return mEndpoint->mPootle->Units()->GetByUrl(str);
}


void
PootleStore::_EnsureData()
{
	if(!mData.IsEmpty())
		return;

	mData = mEndpoint->_SendRequest("GET", mUri);
	mEndpoint->_add_to_cache(mUri, *this);
}

PootleStore::PootleStore(
	PootleStoresEndpoint *endpoint, BMessage &data)
	:
	mEndpoint(endpoint),
	mData(data),
	mUri(data.GetString("resource_uri", ""))
{
	mEndpoint->_add_to_cache(mUri, *this);
}


PootleStore::PootleStore(
	PootleStoresEndpoint *endpoint, int id)
	:
	mEndpoint(endpoint)
{
	mUri.SetToFormat("%d/", id);
	if (mEndpoint->_cache_contains(id))
		*this = mEndpoint->_get_from_cache(id);
}


PootleStore::PootleStore(
	PootleStoresEndpoint *endpoint, BString uri)
	:
	mEndpoint(endpoint),
	mUri(uri)
{
	if (mEndpoint->_cache_contains(uri))
		*this = mEndpoint->_get_from_cache(uri);
}


PootleStore
PootleStoresEndpoint::GetById(int id)
{
	return PootleStore(this, id);
}


PootleStore
PootleStoresEndpoint::GetByUrl(BString url)
{
	return PootleStore(this, url);
}


void
PootleStoresEndpoint::_add_to_cache(int id, PootleStore lang)
{
	mLanguageEndpoints[id] = lang;
}


int
PootleStoresEndpoint::_path_to_id(BString path)
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
PootleStoresEndpoint::_add_to_cache(BString path,
	PootleStore lang)
{
	_add_to_cache(_path_to_id(path), lang);
}


bool
PootleStoresEndpoint::_cache_contains(int id)
{
	return mLanguageEndpoints.count(id) != 0;
}


bool
PootleStoresEndpoint::_cache_contains(BString path)
{
	return _cache_contains(_path_to_id(path));
}


PootleStore
PootleStoresEndpoint::_get_from_cache(int id)
{
	return mLanguageEndpoints[id];
}


PootleStore
PootleStoresEndpoint::_get_from_cache(BString path)
{
	return _get_from_cache(_path_to_id(path));
}


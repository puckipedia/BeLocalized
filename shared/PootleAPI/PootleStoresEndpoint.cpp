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
	return mEndpoint->API()->TranslationProjects()->GetByUrl(
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
	return BString(mData.GetString("pootle_path", ""));
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
	return BString(mData.GetString("sync_time", ""));
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

	return mEndpoint->API()->Units()->GetByUrl(str);
}


BObjectList<BString>
PootleStore::UnitUrls()
{
	BMessage units;
	mData.FindMessage("units", &units);

	BObjectList<BString> urls(20, true);
	
	int32 count = units.CountNames(B_ANY_TYPE);

	char buffer[33];
	for (int32 i = 0; i < count; i++) {
		sprintf(buffer, "%d", i);
		urls.AddItem(new BString(units.GetString(buffer, "")));
	}
	return urls;
}

BObjectList<PootleUnit>
PootleStore::Units()
{
	return mEndpoint->API()->Units()->GetByList(UnitUrls());
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
	_Endpoint *endpoint, BMessage &data)
	:
	mEndpoint(endpoint),
	mData(data),
	mUri(data.GetString("resource_uri", ""))
{
	mEndpoint->_add_to_cache(mUri, *this);
}


PootleStore::PootleStore(
	_Endpoint *endpoint, int id)
	:
	mEndpoint(endpoint)
{
	mUri.SetToFormat("%d/", id);
	if (mEndpoint->_cache_contains(id))
		*this = mEndpoint->_get_from_cache(id);
}


PootleStore::PootleStore(
	_Endpoint *endpoint, BString uri)
	:
	mEndpoint(endpoint),
	mUri(uri)
{
	if (mEndpoint->_cache_contains(uri))
		*this = mEndpoint->_get_from_cache(uri);
}

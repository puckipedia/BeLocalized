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
	return mEndpoint->API()->Languages()->GetByUrl(
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
	return mEndpoint->API()->Projects()->GetByUrl(
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

	return mEndpoint->API()->Stores()->GetByUrl(str);
}


BObjectList<PootleStore>
PootleTranslationProject::Stores()
{
	BMessage stores;
	mData.FindMessage("stores", &stores);

	BObjectList<BString> urls(20, true);
	
	int32 count = stores.CountNames(B_ANY_TYPE);

	char buffer[33];
	for (int32 i = 0; i < count; i++) {
		sprintf(buffer, "%d", i);
		urls.AddItem(new BString(stores.GetString(buffer, "")));
	}
	
	return mEndpoint->API()->Stores()->GetByList(urls);

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
	_Endpoint *endpoint, BMessage &data)
	:
	mEndpoint(endpoint),
	mData(data),
	mUri(data.GetString("resource_uri", ""))
{
	mEndpoint->_add_to_cache(mUri, *this);
}


PootleTranslationProject::PootleTranslationProject(
	_Endpoint *endpoint, int id)
	:
	mEndpoint(endpoint)
{
	mUri.SetToFormat("%d/", id);
	if (mEndpoint->_cache_contains(id))
		*this = mEndpoint->_get_from_cache(id);
}


PootleTranslationProject::PootleTranslationProject(
	_Endpoint *endpoint, BString uri)
	:
	mEndpoint(endpoint),
	mUri(uri)
{
	if (mEndpoint->_cache_contains(uri))
		*this = mEndpoint->_get_from_cache(uri);
}

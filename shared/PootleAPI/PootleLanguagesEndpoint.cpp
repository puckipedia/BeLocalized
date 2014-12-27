#include "PootleLanguagesEndpoint.h"

#include "Pootle.h"
#include "PootleTranslationProjectsEndpoint.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

BString
PootleLanguage::LanguageCode()
{
	_EnsureData();
	return BString(mData.GetString("code", ""));
}


BString
PootleLanguage::Description()
{
	_EnsureData();
	return BString(mData.GetString("description", ""));
}


BString
PootleLanguage::FullName()
{
	_EnsureData();
	return BString(mData.GetString("fullname", ""));
}


int
PootleLanguage::NumberOfPlurals()
{
	_EnsureData();
	return (int)mData.GetDouble("nplurals", 0.0);
}


BString
PootleLanguage::PluralEquation()
{
	_EnsureData();
	return BString(mData.GetString("pluralequation", ""));
}


BString
PootleLanguage::SpecialCharacters()
{
	_EnsureData();
	return BString(mData.GetString("specialchars", ""));
}


int
PootleLanguage::CountTranslationProjects()
{
	_EnsureData();
	BMessage projects;
	mData.FindMessage("translation_projects", &projects);
	return projects.CountNames(B_ANY_TYPE);
}


PootleTranslationProject
PootleLanguage::GetTranslationProject(int index)
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
PootleLanguage::TranslationProjects()
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


void
PootleLanguage::_EnsureData()
{
	if(!mData.IsEmpty())
		return;

	mData = mEndpoint->_SendRequest("GET", mUri);
	mEndpoint->_add_to_cache(mUri, *this);
}


PootleLanguage::PootleLanguage(_Endpoint *endpoint, BMessage &data)
	:
	mEndpoint(endpoint),
	mData(data),
	mUri(data.GetString("resource_uri", ""))
{
	mEndpoint->_add_to_cache(mUri, *this);
}


PootleLanguage::PootleLanguage(_Endpoint *endpoint, int id)
	:
	mEndpoint(endpoint)
{
	mUri.SetToFormat("%d/", id);
	if (mEndpoint->_cache_contains(id))
		*this = mEndpoint->_get_from_cache(id);
}


PootleLanguage::PootleLanguage(_Endpoint *endpoint, BString uri)
	:
	mEndpoint(endpoint),
	mUri(uri)
{
	if (mEndpoint->_cache_contains(uri))
		*this = mEndpoint->_get_from_cache(uri);
}


BObjectList<PootleLanguage>
PootleLanguagesEndpoint::Get(int limit, int offset)
{
	BObjectList<PootleLanguage> objectlist(20, true);
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
		PootleLanguage *lang = new PootleLanguage(this, msg);
		objectlist.AddItem(lang);
	}
	
	return objectlist;
}

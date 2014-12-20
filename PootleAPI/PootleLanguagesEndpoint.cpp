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

	return mEndpoint->mPootle->TranslationProjects()->GetByUrl(str);
}


void
PootleLanguage::_EnsureData()
{
	if(!mData.IsEmpty())
		return;

	mData = mEndpoint->_SendRequest("GET", mUri, "");
	mEndpoint->_add_to_cache(mUri, *this);
}

PootleLanguage::PootleLanguage(PootleLanguagesEndpoint *endpoint, BMessage &data)
	:
	mEndpoint(endpoint),
	mData(data),
	mUri(data.GetString("resource_uri", ""))
{
	mEndpoint->_add_to_cache(mUri, *this);
}


PootleLanguage::PootleLanguage(PootleLanguagesEndpoint *endpoint, int id)
	:
	mEndpoint(endpoint)
{
	mUri.SetToFormat("%d/", id);
	if (mEndpoint->_cache_contains(id))
		*this = mEndpoint->_get_from_cache(id);
}


PootleLanguage::PootleLanguage(PootleLanguagesEndpoint *endpoint, BString uri)
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


PootleLanguage
PootleLanguagesEndpoint::GetById(int id)
{
	return PootleLanguage(this, id);
}


PootleLanguage
PootleLanguagesEndpoint::GetByUrl(BString url)
{
	return PootleLanguage(this, url);
}


void
PootleLanguagesEndpoint::_add_to_cache(int id, PootleLanguage lang)
{
	mLanguageEndpoints[id] = lang;
}


int
PootleLanguagesEndpoint::_path_to_id(BString path)
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
PootleLanguagesEndpoint::_add_to_cache(BString path, PootleLanguage lang)
{
	_add_to_cache(_path_to_id(path), lang);
}


bool
PootleLanguagesEndpoint::_cache_contains(int id)
{
	return mLanguageEndpoints.count(id) != 0;
}


bool
PootleLanguagesEndpoint::_cache_contains(BString path)
{
	return _cache_contains(_path_to_id(path));
}


PootleLanguage
PootleLanguagesEndpoint::_get_from_cache(int id)
{
	return mLanguageEndpoints[id];
}


PootleLanguage
PootleLanguagesEndpoint::_get_from_cache(BString path)
{
	return _get_from_cache(_path_to_id(path));
}


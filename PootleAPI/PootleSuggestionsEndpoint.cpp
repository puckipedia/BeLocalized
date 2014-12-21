#include "PootleSuggestionsEndpoint.h"

#include "Pootle.h"
#include "PootleTranslationProjectsEndpoint.h"
#include "PootleUnitsEndpoint.h"

#include <assert.h>
#include <stdio.h>


PootleUnit
PootleSuggestion::Unit()
{
	_EnsureData();
	return mEndpoint->mPootle->Units()->GetByUrl(
		mData.GetString("unit", ""));
}


BString
PootleSuggestion::Target()
{
	_EnsureData();
	return BString(mData.GetString("target_f", ""));
}


BString
PootleSuggestion::TranslatorComment()
{
	_EnsureData();
	return BString(mData.GetString("translator_comment_f", ""));
}


void
PootleSuggestion::SetTarget(const char *target)
{
	_EnsureData();
	mData.ReplaceString("target_f", target);
}


void
PootleSuggestion::SetTranslatorComment(const char *comment)
{
	_EnsureData();
	mData.ReplaceString("translator_comment_f", comment);
}


void
PootleSuggestion::SetUnit(PootleUnit &unit)
{
	_EnsureData();
	mData.ReplaceString("unit", unit.ResourceUri());
}

void
PootleSuggestion::Put()
{
	if (mData.IsEmpty())
		return;
	
	mEndpoint->_SendRequest("PUT", mUri, mData);
	mEndpoint->_add_to_cache(mUri, *this);
}

void
PootleSuggestion::Create()
{
	BMessage msg = mEndpoint->_SendRequest("POST", "", mData);
	mUri = msg.GetString("_location", "");
}

void
PootleSuggestion::_EnsureData()
{
	if (mUri.Length() < 1)
		return;

	if(!mData.IsEmpty())
		return;

	mData = mEndpoint->_SendRequest("GET", mUri);
	mEndpoint->_add_to_cache(mUri, *this);
}

PootleSuggestion::PootleSuggestion(
	PootleSuggestionsEndpoint *endpoint, BMessage &data)
	:
	mEndpoint(endpoint),
	mData(data),
	mUri(data.GetString("resource_uri", ""))
{
	mEndpoint->_add_to_cache(mUri, *this);
}


PootleSuggestion::PootleSuggestion(
	PootleSuggestionsEndpoint *endpoint, int id)
	:
	mEndpoint(endpoint)
{
	mUri.SetToFormat("%d/", id);
	if (mEndpoint->_cache_contains(id))
		*this = mEndpoint->_get_from_cache(id);
}


PootleSuggestion::PootleSuggestion(
	PootleSuggestionsEndpoint *endpoint, BString uri)
	:
	mEndpoint(endpoint),
	mUri(uri)
{
	if (mEndpoint->_cache_contains(uri))
		*this = mEndpoint->_get_from_cache(uri);
}


PootleSuggestion
PootleSuggestionsEndpoint::GetById(int id)
{
	return PootleSuggestion(this, id);
}


PootleSuggestion
PootleSuggestionsEndpoint::GetByUrl(BString url)
{
	return PootleSuggestion(this, url);
}


void
PootleSuggestionsEndpoint::_add_to_cache(int id, PootleSuggestion lang)
{
	mLanguageEndpoints[id] = lang;
}


int
PootleSuggestionsEndpoint::_path_to_id(BString path)
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
PootleSuggestionsEndpoint::_add_to_cache(BString path,
	PootleSuggestion lang)
{
	_add_to_cache(_path_to_id(path), lang);
}


bool
PootleSuggestionsEndpoint::_cache_contains(int id)
{
	return mLanguageEndpoints.count(id) != 0;
}


bool
PootleSuggestionsEndpoint::_cache_contains(BString path)
{
	return _cache_contains(_path_to_id(path));
}


PootleSuggestion
PootleSuggestionsEndpoint::_get_from_cache(int id)
{
	return mLanguageEndpoints[id];
}


PootleSuggestion
PootleSuggestionsEndpoint::_get_from_cache(BString path)
{
	return _get_from_cache(_path_to_id(path));
}


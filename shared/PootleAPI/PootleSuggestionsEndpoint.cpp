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
	return mEndpoint->API()->Units()->GetByUrl(
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
	mData.RemoveName("target_f");
	mData.AddString("target_f", target);
}


void
PootleSuggestion::SetTranslatorComment(const char *comment)
{
	_EnsureData();
	mData.RemoveName("translator_comment_f");
	mData.AddString("translator_comment_f", comment);
}


void
PootleSuggestion::SetUnit(PootleUnit &unit)
{
	_EnsureData();
	mData.RemoveName("unit");
	mData.AddString("unit", unit.ResourceUri());
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
PootleSuggestion::Create(PootleSuggestionsEndpoint *endpoint)
{
	mEndpoint = endpoint;
	mData.PrintToStream();
	BMessage msg = mEndpoint->_SendRequest("POST", "", mData);
	*this = PootleSuggestion(mEndpoint, msg.GetString("_location", ""));
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
	_Endpoint *endpoint, BMessage &data)
	:
	mEndpoint(endpoint),
	mData(data),
	mUri(data.GetString("resource_uri", ""))
{
	mEndpoint->_add_to_cache(mUri, *this);
}


PootleSuggestion::PootleSuggestion(
	_Endpoint *endpoint, int id)
	:
	mEndpoint(endpoint)
{
	mUri.SetToFormat("%d/", id);
	if (mEndpoint->_cache_contains(id))
		*this = mEndpoint->_get_from_cache(id);
}


PootleSuggestion::PootleSuggestion(
	_Endpoint *endpoint, BString uri)
	:
	mEndpoint(endpoint),
	mUri(uri)
{
	if (mEndpoint->_cache_contains(uri))
		*this = mEndpoint->_get_from_cache(uri);
}

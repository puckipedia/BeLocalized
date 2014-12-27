#include "PootleUnitsEndpoint.h"

#include "Pootle.h"
#include "PootleStoresEndpoint.h"

#include <assert.h>
#include <stdio.h>


int
PootleUnit::CountSuggestions()
{
	_EnsureData();
	BMessage suggestions;
	mData.FindMessage("suggestions", &suggestions);
	return suggestions.CountNames(B_ANY_TYPE);
}


PootleSuggestion
PootleUnit::GetSuggestion(int index)
{
	char buffer[33];
	sprintf(buffer, "%d", index);

	_EnsureData();
	BMessage suggestions;
	mData.FindMessage("suggestions", &suggestions);
	BString str = suggestions.GetString(buffer, "");

	return mEndpoint->mPootle->Suggestions()->GetByUrl(str);
}


PootleUser
PootleUnit::CommentedBy()
{
	_EnsureData();
	return mEndpoint->mPootle->Users()->GetByUrl(
		mData.GetString("commented_by", ""));
}


BString
PootleUnit::CommentedOn()
{
	_EnsureData();
	return BString(mData.GetString("commented_on", ""));
}


BString
PootleUnit::Context()
{
	_EnsureData();
	return BString(mData.GetString("context", ""));
}


BString
PootleUnit::DeveloperComment()
{
	_EnsureData();
	return BString(mData.GetString("developer_comment", ""));
}


BString
PootleUnit::Locations()
{
	_EnsureData();
	return BString(mData.GetString("locations", ""));
}


BString
PootleUnit::MTime()
{
	_EnsureData();
	return BString(mData.GetString("mtime", ""));
}


BString
PootleUnit::SubmittedOn()
{
	_EnsureData();
	return BString(mData.GetString("submitted_on", ""));
}


PootleUser
PootleUnit::SubmittedBy()
{
	_EnsureData();
	return mEndpoint->mPootle->Users()->GetByUrl(
		mData.GetString("submitted_by", ""));
}


BString
PootleUnit::TranslatorComment()
{
	_EnsureData();
	return BString(mData.GetString("translator_comment", ""));
}


Translation
PootleUnit::Source()
{
	_EnsureData();
	Translation trans;

	trans.text = mData.GetString("source_f", "");
	trans.length = (int)mData.GetDouble("source_length", 0.0);
	trans.wordcount = (int)mData.GetDouble("source_wordcount", 0.0);
	
	return trans;
}


Translation
PootleUnit::Target()
{
	_EnsureData();
	Translation trans;

	trans.text = mData.GetString("target_f", "");
	trans.length = (int)mData.GetDouble("target_length", 0.0);
	trans.wordcount = (int)mData.GetDouble("target_wordcount", 0.0);
	
	return trans;
}


int
PootleUnit::State()
{
	_EnsureData();
	
	return (int)mData.GetDouble("state", 0.0);
}


PootleStore
PootleUnit::Store()
{
	_EnsureData();
	return mEndpoint->mPootle->Stores()->GetByUrl(
		mData.GetString("store", ""));
}


void
PootleUnit::_EnsureData()
{
	if(!mData.IsEmpty())
		return;

	mData = mEndpoint->_SendRequest("GET", mUri);
	mEndpoint->_add_to_cache(mUri, *this);
}

PootleUnit::PootleUnit(
	PootleUnitsEndpoint *endpoint, BMessage &data)
	:
	mEndpoint(endpoint),
	mData(data),
	mUri(data.GetString("resource_uri", ""))
{
	mEndpoint->_add_to_cache(mUri, *this);
}


PootleUnit::PootleUnit(
	PootleUnitsEndpoint *endpoint, int id)
	:
	mEndpoint(endpoint)
{
	mUri.SetToFormat("%d/", id);
	if (mEndpoint->_cache_contains(id))
		*this = mEndpoint->_get_from_cache(id);
}


PootleUnit::PootleUnit(
	PootleUnitsEndpoint *endpoint, BString uri)
	:
	mEndpoint(endpoint),
	mUri(uri)
{
	if (mEndpoint->_cache_contains(uri))
		*this = mEndpoint->_get_from_cache(uri);
}


PootleUnit
PootleUnitsEndpoint::GetById(int id)
{
	return PootleUnit(this, id);
}


PootleUnit
PootleUnitsEndpoint::GetByUrl(BString url)
{
	return PootleUnit(this, url);
}


BObjectList<PootleUnit>
PootleUnitsEndpoint::GetByList(BObjectList<BString> list)
{
	BObjectList<PootleUnit> returnList(20, true);
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
		returnList.AddItem(new PootleUnit(this, msg));
	}
	
	return returnList;
}


void
PootleUnitsEndpoint::_add_to_cache(int id, PootleUnit lang)
{
	mLanguageEndpoints[id] = lang;
}


int
PootleUnitsEndpoint::_path_to_id(BString path)
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
PootleUnitsEndpoint::_add_to_cache(BString path,
	PootleUnit lang)
{
	_add_to_cache(_path_to_id(path), lang);
}


bool
PootleUnitsEndpoint::_cache_contains(int id)
{
	return mLanguageEndpoints.count(id) != 0;
}


bool
PootleUnitsEndpoint::_cache_contains(BString path)
{
	return _cache_contains(_path_to_id(path));
}


PootleUnit
PootleUnitsEndpoint::_get_from_cache(int id)
{
	return mLanguageEndpoints[id];
}


PootleUnit
PootleUnitsEndpoint::_get_from_cache(BString path)
{
	return _get_from_cache(_path_to_id(path));
}


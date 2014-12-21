#include "PootleUsersEndpoint.h"

#include "Pootle.h"
#include "PootleTranslationProjectsEndpoint.h"

#include <assert.h>
#include <stdio.h>


BString
PootleUser::DateJoined()
{
	_EnsureData();
	return BString(mData.GetString("datejoined", ""));
}


BString
PootleUser::Email()
{
	_EnsureData();
	return BString(mData.GetString("email", ""));
}


BString
PootleUser::FirstName()
{
	_EnsureData();
	return BString(mData.GetString("firstname", ""));
}


BString
PootleUser::LastName()
{
	_EnsureData();
	return BString(mData.GetString("lastname", ""));
}


BString
PootleUser::UserName()
{
	_EnsureData();
	return BString(mData.GetString("username", ""));
}


void
PootleUser::_EnsureData()
{
	if(!mData.IsEmpty())
		return;

	mData = mEndpoint->_SendRequest("GET", mUri, "");
	mEndpoint->_add_to_cache(mUri, *this);
}

PootleUser::PootleUser(
	PootleUsersEndpoint *endpoint, BMessage &data)
	:
	mEndpoint(endpoint),
	mData(data),
	mUri(data.GetString("resource_uri", ""))
{
	mEndpoint->_add_to_cache(mUri, *this);
}


PootleUser::PootleUser(
	PootleUsersEndpoint *endpoint, int id)
	:
	mEndpoint(endpoint)
{
	mUri.SetToFormat("%d/", id);
	if (mEndpoint->_cache_contains(id))
		*this = mEndpoint->_get_from_cache(id);
}


PootleUser::PootleUser(
	PootleUsersEndpoint *endpoint, BString uri)
	:
	mEndpoint(endpoint),
	mUri(uri)
{
	if (mEndpoint->_cache_contains(uri))
		*this = mEndpoint->_get_from_cache(uri);
}


PootleUser
PootleUsersEndpoint::GetById(int id)
{
	return PootleUser(this, id);
}


PootleUser
PootleUsersEndpoint::GetByUrl(BString url)
{
	return PootleUser(this, url);
}


void
PootleUsersEndpoint::_add_to_cache(int id, PootleUser lang)
{
	mLanguageEndpoints[id] = lang;
}


int
PootleUsersEndpoint::_path_to_id(BString path)
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
PootleUsersEndpoint::_add_to_cache(BString path,
	PootleUser lang)
{
	_add_to_cache(_path_to_id(path), lang);
}


bool
PootleUsersEndpoint::_cache_contains(int id)
{
	return mLanguageEndpoints.count(id) != 0;
}


bool
PootleUsersEndpoint::_cache_contains(BString path)
{
	return _cache_contains(_path_to_id(path));
}


PootleUser
PootleUsersEndpoint::_get_from_cache(int id)
{
	return mLanguageEndpoints[id];
}


PootleUser
PootleUsersEndpoint::_get_from_cache(BString path)
{
	return _get_from_cache(_path_to_id(path));
}


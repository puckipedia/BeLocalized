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

	mData = mEndpoint->_SendRequest("GET", mUri);
	mEndpoint->_add_to_cache(mUri, *this);
}

PootleUser::PootleUser(
	_Endpoint *endpoint, BMessage &data)
	:
	mEndpoint(endpoint),
	mData(data),
	mUri(data.GetString("resource_uri", ""))
{
	mEndpoint->_add_to_cache(mUri, *this);
}


PootleUser::PootleUser(
	_Endpoint *endpoint, int id)
	:
	mEndpoint(endpoint)
{
	mUri.SetToFormat("%d/", id);
	if (mEndpoint->_cache_contains(id))
		*this = mEndpoint->_get_from_cache(id);
}


PootleUser::PootleUser(
	_Endpoint *endpoint, BString uri)
	:
	mEndpoint(endpoint),
	mUri(uri)
{
	if (mEndpoint->_cache_contains(uri))
		*this = mEndpoint->_get_from_cache(uri);
}

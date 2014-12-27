#ifndef POOTLE_API_POOTLE_USERS_ENDPOINT_H
#define POOTLE_API_POOTLE_USERS_ENDPOINT_H

#include "PootleEndpoint.h"

#include <ObjectList.h>

#include <map>

class PootleUsersEndpoint;
class PootleTranslationProject;
class PootleUnit;

class PootleUser {
public:
	typedef PootleEndpoint<PootleUser> _Endpoint;

	BString	DateJoined();
	BString	Email();
	BString	FirstName();
	BString	LastName();
	BString	UserName();

	BString ResourceUri() { return mUri; }

	PootleUser() {}
	PootleUser(_Endpoint *, int);
	PootleUser(_Endpoint *, BString);
	PootleUser(_Endpoint *, BMessage &);

private:
	void			_EnsureData();
	_Endpoint		*mEndpoint;
	BString			mUri;
	BMessage		mData;
};

class PootleUsersEndpoint : public PootleEndpoint<PootleUser> {
public:
	PootleUsersEndpoint(Pootle *pootle)
		: PootleEndpoint<PootleUser>(pootle, "users/") {}

protected:
	friend class PootleUser;
};

#endif

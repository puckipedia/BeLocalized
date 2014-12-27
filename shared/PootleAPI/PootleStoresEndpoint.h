#ifndef POOTLE_API_POOTLE_STORES_ENDPOINT_H
#define POOTLE_API_POOTLE_STORES_ENDPOINT_H

#include "PootleEndpoint.h"

#include <ObjectList.h>

#include <map>

class PootleStoresEndpoint;
class PootleTranslationProject;
class PootleUnit;

class PootleStore {
public:
	typedef PootleEndpoint<PootleStore> _Endpoint;

	BString						File();
	BString						Name();
	BString						PootlePath();
	int							State();
	BString						SyncTime();
	PootleTranslationProject	TranslationProject();
	BObjectList<PootleUnit>		Units();
	BObjectList<BString>		UnitUrls();
	int							CountUnits();
	PootleUnit					GetUnit(int);

	BString ResourceUri() { return mUri; }

	PootleStore() {}
	PootleStore(_Endpoint *, BMessage &);
	PootleStore(_Endpoint *, int);
	PootleStore(_Endpoint *, BString);

private:
	void			_EnsureData();
	_Endpoint		*mEndpoint;
	BString			mUri;
	BMessage		mData;
};

class PootleStoresEndpoint : public PootleEndpoint<PootleStore> {
public:
	PootleStoresEndpoint(Pootle *pootle)
		: PootleEndpoint<PootleStore>(pootle, "stores/") {}
protected:
	friend class PootleStore;
};

#endif

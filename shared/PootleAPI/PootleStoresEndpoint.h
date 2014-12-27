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
	BString						File();
	BString						Name();
	BString						PootlePath();
	int							State();
	BString						SyncTime();
	PootleTranslationProject	TranslationProject();
	int							CountUnits();
	PootleUnit					GetUnit(int);

	BString ResourceUri() { return mUri; }

	PootleStore() {}

protected:
	friend class PootleStoresEndpoint;
	PootleStore(PootleStoresEndpoint *, BMessage &);
	PootleStore(PootleStoresEndpoint *, int);
	PootleStore(PootleStoresEndpoint *, BString);

private:
	void						_EnsureData();
	PootleStoresEndpoint		*mEndpoint;
	BString						mUri;
	BMessage					mData;
};

class PootleStoresEndpoint : public PootleEndpoint {
public:
	PootleStoresEndpoint(Pootle *pootle)
		: PootleEndpoint(pootle, "stores/") {}

	typedef PootleStore Data;

	PootleStore					GetByUrl(BString);
	PootleStore					GetById(int id);
	BObjectList<PootleStore>	GetByList(BObjectList<BString>);
protected:
	friend class PootleStore;
		
	PootleStore _get_from_cache(int);
	PootleStore _get_from_cache(BString);

	bool _cache_contains(int);
	bool _cache_contains(BString);
	
	void _add_to_cache(int, PootleStore);
	void _add_to_cache(BString, PootleStore);

private:
	static int _path_to_id(BString);
	std::map<int, PootleStore> mLanguageEndpoints;
};

#endif

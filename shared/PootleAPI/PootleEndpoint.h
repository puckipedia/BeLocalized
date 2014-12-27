#ifndef POOTLE_API_POOTLE_ENDPOINT_H
#define POOTLE_API_POOTLE_ENDPOINT_H

class Pootle;

#include <ObjectList.h>
#include <Url.h>

#include <map>

struct PootleEndpointMeta {
	int limit;
	BString next;
	int offset;
	BString previous;
	int total_count;
};

class _PootleBase {
public:
	_PootleBase(Pootle *, const char *);
	
	Pootle	*API() { return mPootle; }

	static PootleEndpointMeta _GetMeta(BMessage &msg);

	BMessage _GetAll(const char *name, int maximum);
	BMessage _SendRequest(const char *method, const char *name, BMessage &data);
	BMessage _SendRequest(const char *method, const char *name);

	static int _path_to_id(BString);
protected:
	friend class Pootle;

	Pootle *mPootle;
	BUrl mBaseEndpoint;
};

template<class T>
class PootleEndpoint : public _PootleBase {
public:
	PootleEndpoint(Pootle *p, const char *e)
		: _PootleBase(p, e) {}

	typedef T Data;
	
	T				GetByUrl(BString str)
						{ return T(this, str); }
	T				GetById(int i)
						{ return T(this, i); }

	BObjectList<T>	GetByList(BObjectList<BString> list)
				{
					BObjectList<T> returnList(20, true);
					char buffer[34];
	
					BString setUrl = mBaseEndpoint.Path();
					setUrl.Append("set/");

					std::map<int, T> alreadyGotten;

					for (int32 i = 0; i < list.CountItems(); i++) {
						BString *str = list.ItemAt(i);
						int id = _path_to_id(*str);
						if (_cache_contains(id)) {
							alreadyGotten[i] = _get_from_cache(id);
						} else {
							sprintf(buffer, "%d;", _path_to_id(*str));
							setUrl.Append(buffer);
						}
					}

					setUrl.RemoveLast(";");
					setUrl.Append("/");
	
					BMessage ret = _SendRequest("GET", setUrl);
					BMessage objects;
					ret.FindMessage("objects", &objects);
					int32 count = objects.CountNames(B_ANY_TYPE);
					for (int32 i = 0; i < count; i++) {
						sprintf(buffer, "%d", i);
						BMessage msg;
						objects.FindMessage(buffer, &msg);
						returnList.AddItem(new T(this, msg));
					}
					
					for (std::map<int, T>::iterator i = alreadyGotten.begin();
						i != alreadyGotten.end(); ++i) {
						returnList.AddItem(new T(i->second), i->first);
					}
					
					return returnList;	
				}

	T		_get_from_cache(int i)
				{ return mCache[i]; }
	T		_get_from_cache(BString path)
				{ return mCache[_path_to_id(path)]; }

	bool	_cache_contains(int i)
				{ return mCache.count(i) != 0; }
	bool	_cache_contains(BString str)
				{ return _cache_contains(_path_to_id(str)); }
	
	void	_add_to_cache(int i, T item)
				{ mCache[i] = item; }
	void	_add_to_cache(BString str, T item)
				{ mCache[_path_to_id(str)] = item; }

private:
	std::map<int, T> mCache;

};

#endif

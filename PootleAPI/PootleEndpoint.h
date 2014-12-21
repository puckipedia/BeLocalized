#ifndef POOTLE_API_POOTLE_ENDPOINT_H
#define POOTLE_API_POOTLE_ENDPOINT_H

class Pootle;

#include <Url.h>

struct PootleEndpointMeta {
	int limit;
	BString next;
	int offset;
	BString previous;
	int total_count;
};

class PootleEndpoint {
public:
	PootleEndpoint(Pootle *, const char *);
protected:
	BMessage _GetAll(const char *name, int maximum);
	static PootleEndpointMeta _GetMeta(BMessage &msg);
	BMessage _SendRequest(const char *method, const char *name, BMessage &data);
	BMessage _SendRequest(const char *method, const char *name);
	Pootle *mPootle;
	BUrl mBaseEndpoint;
};

#endif

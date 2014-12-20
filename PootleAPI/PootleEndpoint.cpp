#include "PootleEndpoint.h"

#include "Pootle.h"

#include <private/shared/Json.h>
#include <UrlProtocolRoster.h>
#include <UrlProtocolListener.h>
#include <HttpRequest.h>

#include <stdlib.h>
#include <string.h>

class SynchronousDataCollector : public BUrlProtocolListener {
public:
	SynchronousDataCollector()
		:
		data(NULL),
		dataSize(0),
		semaphore(create_sem(0, "url request semaphore")) { }

	void
	DataReceived(BUrlRequest *caller, const char *recv_data,
		off_t position, ssize_t size)
	{
		if (data == NULL) {
			data = (char *)malloc(position + size);
			dataSize = position + size;
		} else if (dataSize < position + size) {
			data = (char *)realloc(data, position + size);
			dataSize = position + size;
		}
		
		memcpy(data + position, recv_data, size);
	}
	
	void
	RequestCompleted(BUrlRequest *caller, bool success)
	{
		release_sem(semaphore);
	}

	void
	DebugMessage(BUrlRequest *caller, BUrlProtocolDebugMessage msg,
		const char *text)
	{
		#if DEBUG
			printf("DEBUG: %s\n", text);
		#endif
	}

	char *data;
	size_t dataSize;
	sem_id semaphore;
};


PootleEndpoint::PootleEndpoint(Pootle *pootle, const char *endpoint)
	:
	mPootle(pootle),
	mBaseEndpoint(pootle->mBaseUrl, endpoint)
{
	printf("Inited pootle endpoint %s, %s\n", endpoint, mBaseEndpoint.UrlString().String());
	
}


PootleEndpointMeta
PootleEndpoint::_GetMeta(BMessage &msg)
{
	PootleEndpointMeta meta;
	BMessage message;
	msg.FindMessage("meta", &message);
	
	meta.limit = (int)message.GetDouble("limit", 0.0);
	meta.offset = (int)message.GetDouble("offset", 0.0);
	meta.total_count = (int)message.GetDouble("total_count", 0.0);
	
	meta.next = message.GetString("next", "");
	meta.previous = message.GetString("previous", "");
	
	return meta;
}


BMessage
PootleEndpoint::_SendRequest(const char *method, const char *name,
	const char *data)
{
	printf("Sending %s request to '%s' -> '%s', '%s'\n", method, mBaseEndpoint.UrlString().String(), name, data);
	SynchronousDataCollector collector;
	BUrlRequest *r = BUrlProtocolRoster::MakeRequest(
		BUrl(mBaseEndpoint, name), &collector, &mPootle->mContext);

	BHttpRequest *hr = dynamic_cast<BHttpRequest *>(r);
	if (hr) {
		hr->SetMethod(method);
		size_t length = strlen(data);
		BMemoryIO io(data, length);
		hr->AdoptInputData(&io, length);
		hr->SetUserName(mPootle->mUsername);
		hr->SetPassword(mPootle->mPassword);
	}

	r->Run();
	acquire_sem(collector.semaphore);

	BMessage msg;
	BJson::Parse(msg, collector.data);
	msg.PrintToStream();
	return msg;
}

BMessage
PootleEndpoint::_GetAll(const char *name, int maximum)
{
	PootleEndpointMeta meta;
	BString url(name);
	char indexbuffer[33];
	BMessage data;
	
	do {
		BMessage msg = _SendRequest("GET", url, "");
		meta = _GetMeta(msg);
		url = meta.next;
		
		BMessage objects;
		msg.FindMessage("objects", &objects);
		
		int32 datacount = objects.CountNames(B_MESSAGE_TYPE);
		
		for (int32 i = 0; i < datacount; i++) {
			sprintf(indexbuffer, "%d", i);
			BMessage object;
			objects.FindMessage(indexbuffer, &object);
			data.AddMessage("data", &object);
			
			if (maximum > 0 && i + meta.offset > maximum)
				break;
		}
	} while (meta.next.Length() != 0);
	
	return data;
}

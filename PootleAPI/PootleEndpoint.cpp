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


void
serialize_string(BString &str, const char *s)
{
	str += "\"";
	while(*s) {
		if (*s != '"' && *s != '\\' && *s != '\n' && *s != '\t')
			str += *s;
		else {
			str += "\\";
			switch (*s) {
			case '\n':
				str += "n";
				break;
			case '\t':
				str += "t";
				break;
			default:
				str += *s;
			}
		}

		s++;
	}
	str += "\"";
}


BString
message_to_json(BMessage &msg)
{
	BString str;
	bool array = msg.what == BJson::JSON_TYPE_ARRAY;
	if (array)
		str += "[";
	else
		str += "{";

	int32 count = msg.CountNames(B_ANY_TYPE); 
	for (int32 i = 0; i < count; i++) {
		char *name_found;
		type_code type_found;
		msg.GetInfo(B_ANY_TYPE, i, &name_found, &type_found);

		if (!array) {
			serialize_string(str, name_found);
			str += ": ";
		}

		switch (type_found) {
		case B_STRING_TYPE:
			serialize_string(str, msg.GetString(name_found, ""));
			break;
		case B_DOUBLE_TYPE: {
			str += BString().SetToFormat("%g", msg.GetDouble(name_found, 0.0));
			break;
		}
		case B_MESSAGE_TYPE: {
			BMessage submsg;
			msg.FindMessage(name_found, &submsg);
			str += message_to_json(submsg);
			break;
		}
		case B_POINTER_TYPE:
		default:
			str += "null";
			break;
		}
		
		if (i + 1 != count)
			str += ", ";
	}
	
	if (array)
		str += "]";
	else
		str += "}";
		
	return str;
	
}

BMessage
PootleEndpoint::_SendRequest(const char *method, const char *name)
{
	BMessage data;
	return _SendRequest(method, name, data);
}

BMessage
PootleEndpoint::_SendRequest(const char *method, const char *name,
	BMessage &data)
{
	SynchronousDataCollector collector;
	BUrlRequest *r = BUrlProtocolRoster::MakeRequest(
		BUrl(mBaseEndpoint, name), &collector, &mPootle->mContext);

	BHttpRequest *hr = dynamic_cast<BHttpRequest *>(r);
	if (hr) {
		hr->SetMethod(method);
		if (!data.IsEmpty()) {
			BString data_str = message_to_json(data);
			size_t length = data_str.Length() + 1;
			BMemoryIO io(data_str.String(), length);
			hr->AdoptInputData(&io, length);
		}
		hr->SetUserName(mPootle->mUsername);
		hr->SetPassword(mPootle->mPassword);
	}

	r->Run();
	acquire_sem(collector.semaphore);
	
	BMessage msg;
	if(collector.dataSize == 0)
		msg.AddString("_location", ((BHttpResult &)hr->Result()).Headers().HeaderValue("Location"));
	else
		BJson::Parse(msg, collector.data);
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
		BMessage msg = _SendRequest("GET", url);
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

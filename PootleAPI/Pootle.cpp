#include "Pootle.h"

#include <String.h>
#include <UrlRequest.h>
#include <UrlProtocolRoster.h>
#include <private/shared/Json.h>
#include <stdio.h>

Pootle::Pootle(BUrl url, const BMessenger &resultMessenger)
	:
	mBaseUrl(url, "api/v1/"),
	mResultMessenger(resultMessenger),
	mResponseData(NULL)
{
	BUrlRequest *request = BUrlProtocolRoster::MakeRequest(mBaseUrl, this);
	request->Run();
}


void
Pootle::DataReceived(BUrlRequest* caller, const char* data, off_t position,
	ssize_t size)
{
	if (!mResponseData) {
		mResponseData = (char *)malloc(position + size);
		mResponseSize = position + size;
	} else if (mResponseSize < position + size) {
		mResponseData = (char *)realloc(mResponseData, position + size);
		mResponseSize = position + size;
	}

	printf("Data: %p\n", mResponseData);

	memcpy(mResponseData + position, data, size);
	mResponseData[position + size] = 0;
	printf("Got %d bytes at position %d\n", size, position);
}


void
Pootle::RequestCompleted(BUrlRequest *caller, bool success)
{
	status_t s = BPrivate::BJson::Parse(mData, BString(mResponseData));
	printf("GOT %s\n%s\nTOG\n", strerror(s), mResponseData);
	mData.PrintToStream();
	mResultMessenger.SendMessage('WOOO');
}

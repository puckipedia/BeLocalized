#include "Pootle.h"

#include <HttpAuthentication.h>
#include <String.h>
#include <UrlRequest.h>
#include <UrlProtocolRoster.h>
#include <private/shared/Json.h>
#include <stdio.h>

Pootle::Pootle(BUrl url, const BMessenger &resultMessenger,
	const BString username, const BString password)
	:
	mBaseUrl(url, "api/v1/"),
	mResultMessenger(resultMessenger),
	mResponseData(NULL),
	mUsername(username),
	mPassword(password),
	mLanguages(this),
	mProjects(this),
	mTranslationProjects(this)
{
	mContext.AddAuthentication(BUrl(url, "api/v1"),
		BHttpAuthentication(username, password));
	BUrlRequest *request = BUrlProtocolRoster::MakeRequest(mBaseUrl, this,
		&mContext);
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

	memcpy(mResponseData + position, data, size);
	mResponseData[position + size] = 0;
}


void
Pootle::RequestCompleted(BUrlRequest *caller, bool success)
{
	BMessage msg;
	BPrivate::BJson::Parse(msg, BString(mResponseData));
	mData = msg;

	if (mData.GetInfo("languages", NULL) == B_NAME_NOT_FOUND
		|| mData.GetInfo("projects", NULL) == B_NAME_NOT_FOUND
		|| mData.GetInfo("stores", NULL) == B_NAME_NOT_FOUND
		|| mData.GetInfo("suggestions", NULL) == B_NAME_NOT_FOUND
		|| mData.GetInfo("translation-projects", NULL) == B_NAME_NOT_FOUND
		|| mData.GetInfo("units", NULL) == B_NAME_NOT_FOUND
		|| mData.GetInfo("users", NULL) == B_NAME_NOT_FOUND)
		mResultMessenger.SendMessage(kMsgPootleInitFailed);
	else
		mResultMessenger.SendMessage(kMsgPootleInited);

	delete caller;
}

void
Pootle::DebugMessage(BUrlRequest *caller, BUrlProtocolDebugMessage msg, const char *text)
{
#if DEBUG
	printf("DEBUG: %s\n", text);
#endif
}

#ifndef POOTLE_API_POOTLE_H
#define POOTLE_API_POOTLE_H

#include "PootleEndpoint.h"
#include "PootleLanguagesEndpoint.h"
#include "PootleProjectsEndpoint.h"
#include "PootleTranslationProjectsEndpoint.h"

#include <Message.h>
#include <Messenger.h>
#include <String.h>
#include <Url.h>
#include <UrlContext.h>
#include <UrlProtocolListener.h>

class PootleEndpoint;
class PootleLanguagesEndpoint;

const int32 kMsgPootleInited = 'PoIn';
const int32 kMsgPootleInitFailed = 'PoFa';

class Pootle : public BUrlProtocolListener {
public:
					Pootle(BUrl url, const BMessenger &resultMessenger,
						const BString username, const BString password);

	PootleLanguagesEndpoint	*Languages()
		{ return &mLanguages; }
	PootleProjectsEndpoint	*Projects()
		{ return &mProjects; }
	PootleEndpoint	Stores()
		{ return PootleEndpoint(this, "stores/"); }
	PootleEndpoint	Suggestions()
		{ return PootleEndpoint(this, "suggestions/"); }
	PootleTranslationProjectsEndpoint	*TranslationProjects() 
		{ return &mTranslationProjects; }
	PootleEndpoint	Units()
		{ return PootleEndpoint(this, "units/"); }
	PootleEndpoint	Users() 
		{ return PootleEndpoint(this, "users/"); }
	
	void			DataReceived(BUrlRequest* caller, const char* data,
						off_t position, ssize_t size);
	void			RequestCompleted(BUrlRequest* caller, bool success);
	void			DebugMessage(BUrlRequest *, BUrlProtocolDebugMessage, const char *);
protected:
	BUrl mBaseUrl;
	BUrlContext mContext;
	friend class PootleEndpoint;
	
	PootleLanguagesEndpoint mLanguages;
	PootleProjectsEndpoint mProjects;
	PootleTranslationProjectsEndpoint mTranslationProjects;
	
	BString mUsername;
	BString mPassword;
private:
	char *mResponseData;
	int mResponseSize;
	BMessage mData;
	BMessenger mResultMessenger;
};

#endif

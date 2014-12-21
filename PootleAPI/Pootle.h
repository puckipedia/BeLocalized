#ifndef POOTLE_API_POOTLE_H
#define POOTLE_API_POOTLE_H

#include "PootleEndpoint.h"
#include "PootleLanguagesEndpoint.h"
#include "PootleProjectsEndpoint.h"
#include "PootleStoresEndpoint.h"
#include "PootleSuggestionsEndpoint.h"
#include "PootleTranslationProjectsEndpoint.h"
#include "PootleUnitsEndpoint.h"
#include "PootleUsersEndpoint.h"

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
	PootleStoresEndpoint	*Stores()
		{ return &mStores; }
	PootleSuggestionsEndpoint	*Suggestions()
		{ return &mSuggestions; }
	PootleTranslationProjectsEndpoint	*TranslationProjects() 
		{ return &mTranslationProjects; }
	PootleUnitsEndpoint	*Units()
		{ return &mUnits; }
	PootleUsersEndpoint	*Users() 
		{ return &mUsers; }
	
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
	PootleStoresEndpoint mStores;
	PootleSuggestionsEndpoint mSuggestions;
	PootleUnitsEndpoint mUnits;
	PootleUsersEndpoint mUsers;
	
	BString mUsername;
	BString mPassword;
private:
	char *mResponseData;
	int mResponseSize;
	BMessage mData;
	BMessenger mResultMessenger;
};

#endif

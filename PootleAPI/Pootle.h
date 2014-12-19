#ifndef POOTLE_API_POOTLE_H
#define POOTLE_API_POOTLE_H

#include <Message.h>
#include <Messenger.h>
#include <Url.h>
#include <UrlProtocolListener.h>

class PootleEndpoint;

class Pootle : public BUrlProtocolListener {
public:
					Pootle(BUrl url, const BMessenger &resultMessenger);

	/* PootleEndpoint	&Languages();
	PootleEndpoint	&Projects();
	PootleEndpoint	&Stores();
	PootleEndpoint	&Suggestions();
	PootleEndpoint	&TranslationProjects();
	PootleEndpoint	&Units();
	PootleEndpoint	&Users(); */
	
	void			DataReceived(BUrlRequest* caller, const char* data,
						off_t position, ssize_t size);
	void			RequestCompleted(BUrlRequest* caller, bool success);
protected:
	BUrl mBaseUrl;
private:
	char *mResponseData;
	int mResponseSize;
	BMessage mData;
	BMessenger mResultMessenger;
};

#endif

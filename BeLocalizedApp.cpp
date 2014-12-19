#include "BeLocalizedApp.h"

#include "PootleAPI/Pootle.h"

#include <stdio.h>

void
BeLocalizedApp::ArgvReceived(int32 argc, char **argv)
{
	printf("Got %d args\n", argc);
}


void
BeLocalizedApp::MessageReceived(BMessage *msg)
{
	msg->PrintToStream();
	BApplication::MessageReceived(msg);
}


void
BeLocalizedApp::ReadyToRun()
{
	Pootle p(BUrl("http://192.168.178.44/"), BMessenger(this));
}

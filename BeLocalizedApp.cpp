#include "BeLocalizedApp.h"

#include <stdio.h>


void
BeLocalizedApp::ArgvReceived(int32 argc, char **argv) {
	printf("Got %d args\n", argc);
}


void
BeLocalizedApp::ReadyToRun()
{
	Quit();
}

/*

app.cpp

FileCropper - Truncates files. That's simple.

Copyright (C) 2002 Maurice Michalski, http://fetal.de, http://maurice-michalski.de

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
#include "app.h"
#include <Alert.h>
#include <Entry.h>
#include <Path.h>
#include <File.h>

app::app():BApplication("application/x-FileCropper") {
	mainW=new mainWindow();
}

app::~app() {
}

void app::ReadyToRun() {
	mainW->Show();
}

bool app::QuitRequested() {
	while (CountWindows()>0) {WindowAt(0)->Lock();WindowAt(0)->Quit();}
	return true;
}

void app::MessageReceived(BMessage *msg) {
	switch(msg->what) {

		case OK_MSG: {
			off_t first=mainW->FirstBytes(), last=mainW->LastBytes();
			off_t size=0;
			BFile *file=new BFile(mainW->GetFilename(), B_READ_WRITE);
			if (file->InitCheck()==B_OK) {
				file->GetSize(&size);
				size-=last;
				file->SetSize(size);

				file->Unset();
			}
			break;
		}

		case B_SIMPLE_DATA:
			RefsReceived(msg);
			break;

		default:
			BApplication::MessageReceived(msg);
			break;
	}
}

void app::RefsReceived(BMessage *msg) {
	uint32 		type;
	int32 		count;
	BEntry		*entry=new BEntry();
	entry_ref	ref;
	BPath		path;

	msg->GetInfo("refs", &type, &count);

	// not a entry_ref?
	if (type != B_REF_TYPE) {
		delete entry;
		return;
	}

	if (msg->FindRef("refs", 0, &ref) == B_OK)
		if (entry->SetTo(&ref,true)==B_OK) {
			// entry is ok. use it here.
			entry->GetPath(&path);
			mainW->Lock();
			mainW->SetFilename(path.Path());
			mainW->Unlock();
		}
	delete entry;
}

void app::AboutRequested() {
	BAlert *alert=new BAlert("about","FileCropper 0.1\n\nCopyright by Maurice Michalski",":-)");
	alert->Go();
}


int main() {
	new app();
	be_app->Run();
	delete be_app;
	return 0;
}

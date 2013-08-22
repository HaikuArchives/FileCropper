/*
 
mainWindow.cpp

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
#include "mainWindow.h"
#include <Application.h>
#include <StringView.h>
#include <stdlib.h>


mainWindow::mainWindow() : BWindow(BRect(150,100, 400, 226), "FileCropper 0.1", B_TITLED_WINDOW, B_NOT_ZOOMABLE | B_NOT_RESIZABLE) {
	mainView=new BView(Bounds(), "mainView", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	AddChild(mainView);
	mainView->SetViewColor(216, 216, 216);
	
	mainView->AddChild(new BStringView(BRect(8,1,Bounds().Width()-7, 22), "stringview1", "Cut off the"));
	
	firstTC=new BTextControl(BRect(7,27,101,47), "firstTC", "first ", "0",NULL, B_FOLLOW_LEFT | B_FOLLOW_TOP);
	mainView->AddChild(firstTC);
	firstTC->SetDivider(28);
	lastTC=new BTextControl(BRect((Bounds().Width()-7)-133,27,Bounds().Width()-10,47), "lastTC", "and the last ", "0",NULL, B_FOLLOW_LEFT | B_FOLLOW_TOP);
	mainView->AddChild(lastTC);
	lastTC->SetDivider(63);
	firstTC->SetEnabled(false);
	mainView->AddChild(new BStringView(BRect(8,47,Bounds().Width()-7, 67), "stringview1", "bytes of this file (drop it):"));

	filenameTC=new BTextControl(BRect(5,68,Bounds().Width()-10,88), "filenameTC", "", "",NULL, B_FOLLOW_LEFT | B_FOLLOW_TOP);
	mainView->AddChild(filenameTC);
	filenameTC->SetDivider(0);
	
	doitB=new BButton(BRect(Bounds().Width()-80,93,Bounds().Width()-9, 119), "doitB", "Do It!", new BMessage(OK_MSG));
	mainView->AddChild(doitB);
	doitB->SetTarget(be_app_messenger);
	cancelB=new BButton(BRect(Bounds().Width()-160,93,Bounds().Width()-89, 119), "doitB", "Cancel", new BMessage(B_QUIT_REQUESTED));
	mainView->AddChild(cancelB);
	aboutB=new BButton(BRect(Bounds().Width()-241,93,Bounds().Width()-169, 119), "aboutB", "About", new BMessage(B_ABOUT_REQUESTED));
	mainView->AddChild(aboutB);
	aboutB->SetTarget(be_app_messenger);
}

mainWindow::~mainWindow() {
}

bool mainWindow::QuitRequested() {
	be_app->PostMessage(B_QUIT_REQUESTED);
	return false;
}

void mainWindow::SetFilename(const char *name) {
	filenameTC->SetText(name);
}

const char *mainWindow::GetFilename() {
	return filenameTC->Text();
}


void mainWindow::MessageReceived(BMessage *msg) {
	if (msg->what==B_SIMPLE_DATA)
		be_app->PostMessage(msg);
	else
		BWindow::MessageReceived(msg);
}

off_t mainWindow::FirstBytes() {
	return (off_t)atol(firstTC->Text());
}

off_t mainWindow::LastBytes() {
	return (off_t)atol(lastTC->Text());
}

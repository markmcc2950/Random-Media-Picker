// Source: https://www.youtube.com/watch?v=FOIbK4bJKS8

#include "cMain.h"=
#include "shobjidl_core.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <windows.h>
#include <shlobj.h>

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_BUTTON(10001, OnRandomButtonClicked)
EVT_BUTTON(10002, OnContinuousButtonClicked)
EVT_BUTTON(10003, onBrowseButtonClicked)
wxEND_EVENT_TABLE()

RandomEpisode re;
extern DirectoryHandler dh;

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Random Episode Generator", wxDefaultPosition, wxDefaultSize) {
	// Manually set size to our screen's current dimensions
	SetSize(windowWidth, windowHeight);
	//Maximize(true);

	// Create our buttons
	m_btn1 = new wxButton(this, 10001, "Generate", wxPoint(windowWidth * 0.1, windowHeight * 0.1), wxSize(windowWidth * 0.1, windowHeight * 0.05));
	m_btn2 = new wxButton(this, 10002, "Generate\nContinuous", wxPoint(windowWidth * 0.1, windowHeight * 0.2), wxSize(windowWidth * 0.1, windowHeight * 0.05));
	m_btn3 = new wxButton(this, 10003, "Browse", wxPoint(windowWidth - (windowWidth * 0.2), windowHeight * 0.1), wxSize(windowWidth * 0.1, windowHeight * 0.05));
	
	// Create our string boxes
		/*
			- list1: Most recent picked
			- list2: 10 most recent picked
			- list3: Browse directory path
		*/
	m_list1 = new wxListBox(this, wxID_ANY, wxPoint(windowWidth * 0.1, windowHeight * 0.3), wxSize(windowWidth * 0.5, windowHeight * 0.035));
	m_list2 = new wxListBox(this, wxID_ANY, wxPoint(windowWidth * 0.1, windowHeight * 0.4), wxSize(windowWidth * 0.5, windowHeight * 0.325));

	// Create the directory string box, but hide it
	m_list3 = new wxListBox(this, wxID_ANY, wxPoint(windowWidth * 0.5, windowHeight * 0.2), wxSize(windowWidth * 0.5, windowHeight * 0.2));

	setElementSizes();

	// Set tool tips for the buttons
	m_btn1->SetToolTip("Generate Random Media");
	m_btn2->SetToolTip("Generate Continuous Random Media");
	m_btn3->SetToolTip("Select Your Media Path");

	// Initialize all settings now that the UI is initialized
	initialSettings();

	// Bind the event handler that detects when window is resized
	this->Bind(wxEVT_SIZE, &cMain::onResize, this);
}

cMain::~cMain() {
	DestroyButton(m_btn1);
	DestroyButton(m_btn2);
	DestroyButton(m_btn3);
	DestroyListBox(m_list1);
	DestroyListBox(m_list2);
	DestroyListBox(m_list3);
}

void cMain::setElementSizes() {
	// Set positions based on current window size
	m_btn1->SetPosition(wxPoint(windowWidth * 0.1, windowHeight * 0.1));
	m_btn2->SetPosition(wxPoint(windowWidth * 0.1, windowHeight * 0.2));
	m_btn3->SetPosition(wxPoint(windowWidth - (windowWidth * 0.2), windowHeight * 0.1));
	m_list1->SetPosition(wxPoint(windowWidth * 0.1, windowHeight * 0.3));
	m_list2->SetPosition(wxPoint(windowWidth * 0.1, windowHeight * 0.4));

	// Set dimensions based on current window size
	m_btn1->SetSize(wxSize(windowWidth * 0.1, windowHeight * 0.05));
	m_btn2->SetSize(wxSize(windowWidth * 0.1, windowHeight * 0.05));
	m_btn3->SetSize(wxSize(windowWidth * 0.1, windowHeight * 0.05));
	m_list1->SetSize(wxSize(windowWidth * 0.5, windowHeight * 0.035));
	m_list2->SetSize(wxSize(windowWidth * 0.5, windowHeight * 0.325));

	// Handle m_list3 with its own function
	setMediaDirectory();

	// Create the fonts and set them for the lists and buttons
	wxFont btnFont(windowHeight * 0.015, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT);
	wxFont listFont(windowHeight * 0.02, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

	m_btn1->SetFont(btnFont);
	m_btn2->SetFont(btnFont);
	m_btn3->SetFont(btnFont);
	m_list1->SetFont(listFont);
	m_list2->SetFont(listFont);
	m_list3->SetFont(listFont);
}

void cMain::initialSettings() {
	// Populate the recently watched list
	re.updateRecentWatched(episodeStack);

	// Set how many values to show, max of 10 (for now), or if files viewed < 10, only display those
	int toDisplay = std::min((int)episodeStack.size(), filesToDisplay);
	episodeList.resize(10);																		// Allocate the size needed for our episodeList vector

	for (int i = 0; i < toDisplay; i++) {
		episodeList[i] = episodeStack.top();
		episodeStack.pop();
		m_list2->AppendString(episodeList[i]);
	}

	// Populate the directory where our media is located (if used before)
	bool loadFile = dh.LoadPathFromFile(selectedDirectory);	

	if (loadFile) {
		setMediaDirectory();
	}
	else {
		m_list3->Hide();
		m_btn1->Disable();
		m_btn2->Disable();			// This button is for "watch continuous"
	}
}

void cMain::setMediaDirectory() {
	// Clear our previous browse path directory
	if (m_list3) {
		m_list3->Clear();
	}

	// Get the character length of our directory path
	int pathLength = (selectedDirectory.length()) * 5 + 40;
	int pathListLength = windowWidth * 0.0013 * pathLength;

	// Re-populate the browse list with our new directory path and length, adjusting the UI components accordingly
	m_list3->SetSize(pathListLength, windowHeight * 0.035);
	m_list3->SetPosition(wxPoint(windowWidth - (windowWidth * 0.1) - pathListLength, windowHeight * 0.2));
	m_list3->AppendString(dh.getDirectory());
	m_list3->Show();
	m_btn1->Enable();
	m_btn2->Enable();
}

void cMain::onBrowseButtonClicked(wxCommandEvent& evt) {
	// Initialize the BROWSEINFO structure
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(bi));

	// Return only file system paths and in new browse dialog style
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

	// Display the "Browse for Folder" dialog box
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl != NULL)
	{
		TCHAR path[MAX_PATH];
		if (SHGetPathFromIDList(pidl, path))
		{
			std::string newPath = re.tcharToString(path);
			dh.setDirectory(newPath);

			m_btn1->Enable();
		}
		else
		{
			std::cout << "Error: Failed to retrieve folder path." << std::endl;
		}
		CoTaskMemFree(pidl);
	}
	else
	{
		std::cout << "No folder selected." << std::endl;
	}

	std::string directoryPath = dh.getDirectory();
	int pathLength = directoryPath.length();
	pathLength = pathLength * 5 + 40;

	// Only clear the list if it currently exists/has been initialized already
	if (m_list3 != nullptr) {
		m_list3->Clear();
	}

	// If we have a valid path, set the media path and update the UI
	if (pathLength > 0) {
		setMediaDirectory();
	}
}

void cMain::selectRandomEpisode() {
	// Disable button and show we are searching
	m_btn1->Disable();
	m_list1->Clear();
	m_list1->AppendString("SEARCHING...");

	// Pull from the stored selected media directory
	selectedDirectory = dh.getDirectory();

	// Find the file and folders
	if (dh.findDirectoryPath(selectedDirectory, vlcPath, episodeList, filesToDisplay)) {
		m_list1->Clear();
		m_list1->AppendString(selectedDirectory);

		// Don't show the first episode, but update the list after each episode
		if (selectedDirectory != "" && filesToDisplay > 0) {
			m_list2->Clear();

			// Clear our list of viewed episodes, reappend our updated list
			for (int i = 0; i < filesToDisplay; i++) {
				m_list2->AppendString(episodeList[i]);
			}

			// Reverse the list to show latest watched first
			for (int i = filesToDisplay - 1; i > filesToDisplay / 2; i--) {
				std::string temp = episodeList[i];
				episodeList[i] = episodeList[i - 1];
				episodeList[i - 1] = temp;
			}

			// Finally, append our latest valid selected media as most recently watched
			episodeList[0] = selectedDirectory;
		}
		// Reset our loop counter and re-enable our button to search again, exit from the loop
		loopCounter = 0;
		m_btn1->Enable();
	}
	else {
		std::string errorDisplay = "ERROR! Unable to find an episode: " + selectedDirectory;

		m_list1->Clear();
		m_list1->AppendString(errorDisplay);

		m_btn1->Enable();																	// Re-Enable the button to be able to search again
		loopCounter = 0;
	}

	std::string episodeName = dh.getFileByIndex(vlcPath, randomValue);
	vlcPath += "\\" + episodeName;
	vlcPath = dh.formatFinalDirectory(vlcPath, "//", "\\", false);

	re.openFile(vlcPath);

	// Views episodes continuously until user closes the application
	//if (re.openFile(vlcPath)) {
	//	OnRandomButtonClicked(evt);
	//}
	re.updateRecentWatched(episodeStack);
}

void cMain::OnRandomButtonClicked(wxCommandEvent& evt) {
	selectRandomEpisode();

	evt.Skip();
}

void cMain::OnContinuousButtonClicked(wxCommandEvent& evt) {
	int ctr = 0;																				// To keep track of our current loop
	int maxCounter = 3;																			// Max number of episodes back to back before checking in on the user
	bool isWatching = true;

	while (isWatching) {
		if (ctr < maxCounter) {
			selectRandomEpisode();
			ctr++;
		}
		else {
			std::cout << "DEBUG: Are you still watching?" << std::endl;
			isWatching = false;
		}
	}

	evt.Skip();
}

void cMain::DestroyListBox(wxListBox* listBox) {
	listBox->Destroy();
}

void cMain::DestroyButton(wxButton* button) {
	button->Destroy();
}

void cMain::onResize(wxSizeEvent& event) {
	wxSize newSize = event.GetSize();
	windowWidth = newSize.GetWidth();
	windowHeight = newSize.GetHeight();

	setElementSizes();

	event.Skip();
}
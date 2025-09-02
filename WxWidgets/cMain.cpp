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
	re.retrieveAllViewed(episodeStack, episodeList, episodesViewedHash);

	appendEpisodesList();
	

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

void cMain::appendEpisodesList() {
	// Set how many values to show, max of 10 (for now), or if files viewed < 10, only display those
	int toDisplay = std::min((int)episodeStack.size(), filesToDisplay);
	episodeList.resize(10);																		// Allocate the size needed for our episodeList vector

	for (int i = 0; i < toDisplay; i++) {
		episodeList[i] = episodeStack.top();
		episodeStack.pop();
		m_list2->AppendString(episodeList[i]);
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

// Used in tandem with onBrowseButtonClicked to help with navigating us to the currently stored path (if it exists)
int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {
	if (uMsg == BFFM_INITIALIZED) {
		const wchar_t* initialPath = reinterpret_cast<const wchar_t*>(lpData);
		if (initialPath && initialPath[0] != L'\0') {
			SendMessageW(hwnd, BFFM_SETSELECTIONW, TRUE, (LPARAM)initialPath);
		}
	}
	return 0;
}

void cMain::onBrowseButtonClicked(wxCommandEvent& evt) {
	std::string storedPath = dh.getDirectory();

	// Convert std::string to std::wstring for Unicode
	std::wstring wCurrentPath(storedPath.begin(), storedPath.end());

	BROWSEINFOW bi;
	ZeroMemory(&bi, sizeof(bi));
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_USENEWUI;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = (LPARAM)(wCurrentPath.empty() ? nullptr : wCurrentPath.c_str());
	bi.hwndOwner = NULL;
	bi.lpszTitle = L"Select Media Directory";

	LPITEMIDLIST pidl = SHBrowseForFolderW(&bi); // Unicode version
	if (pidl != nullptr) {
		wchar_t path[MAX_PATH];
		if (SHGetPathFromIDListW(pidl, path)) {
			// Convert back to std::string
			std::wstring wpath(path);
			std::string newPath(wpath.begin(), wpath.end());

			dh.setDirectory(newPath);
			m_btn1->Enable();
		}
		else {
			std::cout << "Error: Failed to retrieve folder path." << std::endl;
		}
		CoTaskMemFree(pidl);
	}
	else {
		std::cout << "No folder selected." << std::endl;
	}

	std::string directoryPath = dh.getDirectory();
	int pathLength = directoryPath.length();
	pathLength = pathLength * 5 + 40;

	if (m_list3 != nullptr) {
		m_list3->Clear();
	}

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
	if (dh.findDirectoryPath(selectedDirectory, vlcPath, episodeList, filesToDisplay, randomValue, episodesViewedHash)) {
		m_list1->Clear();
		m_list1->AppendString(selectedDirectory);

		// Don't show the first episode, but update the list after each episode
		if (selectedDirectory != "" && filesToDisplay > 0) {
			m_list2->Clear();

			// Clear our list of viewed episodes, reappend our updated list
			for (int i = 0; i < filesToDisplay; i++) {
				m_list2->AppendString(episodeList[i]);
			}

			// Append the new episode and reverse the list to show latest watched first
			episodeList.insert(episodeList.begin(), selectedDirectory);

			episodeList.resize(10);
		}
		// Reset our loop counter and re-enable our button to search again, exit from the loop
		loopCounter = 0;
		m_btn1->Enable();

		std::string episodeName = dh.getFileByIndex(vlcPath, randomValue);						// Given an index, returns the name of the file in the folder
		vlcPath += "\\" + episodeName;
		vlcPath = dh.normalizePath(vlcPath, "//", "\\");

		// Store information in our hash and local file to reference that it's been watched
		re.storeRecentWatched(selectedDirectory, episodesViewedHash);
		re.updateRecentWatched(episodeStack, episodeList);

		re.openFile(vlcPath);
	}
	else {
		std::string errorDisplay = "ERROR! Unable to find unviewed episode: " + selectedDirectory;

		m_list1->Clear();
		m_list1->AppendString(errorDisplay);

		m_btn1->Enable();																	// Re-Enable the button to be able to search again
		loopCounter = 0;
	}
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
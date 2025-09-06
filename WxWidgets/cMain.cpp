// Source: https://www.youtube.com/watch?v=FOIbK4bJKS8

#include "cMain.h"
#include "shobjidl_core.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <windows.h>
#include <shlobj.h>

// Handle events for buttons/radios clicked
wxBEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_BUTTON(10001, OnRandomButtonClicked)
EVT_BUTTON(10002, onContinuousButtonClicked)
EVT_BUTTON(10003, onBrowseButtonClicked)
EVT_BUTTON(10004, onContinuousButtonClicked)
EVT_BUTTON(10005, onNoPromptButtonClicked)
EVT_RADIOBUTTON(20001, onRadioViewNumClicked)
EVT_RADIOBUTTON(20002, onRadioViewNumClicked)
EVT_RADIOBUTTON(20003, onRadioViewNumClicked)
EVT_RADIOBUTTON(20004, onRadioLightThemeClicked)
EVT_RADIOBUTTON(20005, onRadioLightThemeClicked)
wxEND_EVENT_TABLE()

extern RandomEpisode re;
extern DirectoryHandler dh;
InterfaceHandler ih;

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Random Episode Generator", wxDefaultPosition, wxDefaultSize) {
	// Create UI and initialize all settings (such as pulling info from our local list of stored viewed episodes)
	createUI();
	initialSettings();

	// Bind the event handler that detects when window is resized
	this->Bind(wxEVT_SIZE, &cMain::onResize, this);
}

cMain::~cMain() {
	// Destroy all buttons
	for (auto* btn : wxBtnVec) {
		DestroyButton(btn);
	}
	// Destroy all lists
	for (auto* list : wxListVec) {
		DestroyListBox(list);
	}
}

void cMain::setNumToShow(int n) {
	filesToDisplay = n;
	
	// Clear the list if it's been populated already
	if (m_list2) {
		m_list2->Clear();
	}

	for (int i = episodeList.size() - 1; i >= std::min((int)episodeList.size() - filesToDisplay, filesToDisplay); i--) {
		if (i >= 0) {
			m_list2->AppendString(episodeList[i]);
		}

		// In case we go into negative index values
		else {
			break;
		}
	}
}

void cMain::createUI() {
	// Set initial size to our screen's current dimensions
	int width, height;
	wxDisplaySize(&width, &height);

	windowWidth = std::max(width * 0.4, (double)minDimensions), windowHeight = std::max(height * 0.6, (double)minDimensions);
	SetSize(windowWidth, windowHeight);
	SetMinSize(wxSize(minDimensions, minDimensions));

	// Create our buttons
	m_btn1 = new wxButton(this, 10001, "Watch\nRandom", wxPoint(100, 100), wxSize(100, 100));
	m_btn2 = new wxButton(this, 10002, "Watch\nContinuous", wxPoint(100, 100), wxSize(100, 100));
	m_btn3 = new wxButton(this, 10003, "Select\nMedia Path", wxPoint(100, 100), wxSize(100, 100));
	m_btn4 = new wxButton(this, 10004, "Yes", wxPoint(100, 100), wxSize(100, 100));											// For our "Are You Still There?" prompt
	m_btn5 = new wxButton(this, 10005, "No", wxPoint(100, 100), wxSize(100, 100));

	std::string emoji_string = u8"\U0001F4A1";

	// Create our radio buttons
	m_radio1 = new wxRadioButton(this, 20001, wxString::Format("%d", 10), wxPoint(100, 100), wxSize(10, 10), wxRB_GROUP);	// Show 10 recent episodes
	m_radio2 = new wxRadioButton(this, 20002, wxString::Format("%d", 25), wxPoint(100, 100), wxSize(10, 10));				// Show 25 recent episodes
	m_radio3 = new wxRadioButton(this, 20003, wxString::Format("%d", 50), wxPoint(100, 100), wxSize(10, 10));				// Show 50 recent episodes
	m_radio4 = new wxRadioButton(this, 20004, wxString::Format("%s", wxString::FromUTF8("\U0001F4A1")), wxPoint(100, 100), wxSize(10, 10), wxRB_GROUP);	// Enable Light Mode
	m_radio5 = new wxRadioButton(this, 20005, wxString::Format("%s", wxString::FromUTF8("\U0001F319")), wxPoint(100, 100), wxSize(10, 10));				// Enable Dark Mode

	// Create our static text
	m_label1 = new wxStaticText(this, wxID_ANY, "ARE YOU STILL THERE?", wxPoint(50, 50), wxDefaultSize, wxALIGN_CENTER);

	// Create our lists
	m_list1 = new wxListBox(this, wxID_ANY, wxPoint(100, 100), wxSize(100, 100));											// Most recent episode picked
	m_list2 = new wxListBox(this, wxID_ANY, wxPoint(100, 100), wxSize(100, 100));											// Most recent N episodes picked (radio buttons)
	m_list3 = new wxListBox(this, wxID_ANY, wxPoint(100, 100), wxSize(100, 100));											// Media directory path

	// Set tool tips for the buttons
	m_btn1->SetToolTip("Play a Random Episode");
	m_btn2->SetToolTip("Continuous Play Random Episodes");
	m_btn3->SetToolTip("Select Your Media Path");
	// Radio buttons
	m_radio1->SetToolTip(wxString::Format("%d Most Recent Episodes Viewed", 10));
	m_radio2->SetToolTip(wxString::Format("%d Most Recent Episodes Viewed", 25));
	m_radio3->SetToolTip(wxString::Format("%d Most Recent Episodes Viewed", 50));
	m_radio4->SetToolTip("Toggle Light Mode");
	m_radio5->SetToolTip("Toggle Dark Mode");

	// Hide prompt-specific UI
	m_btn4->Hide();
	m_btn5->Hide();
	m_label1->Hide();

	// Add UI elements to their associative vectors
	wxBtnVec = { m_btn1, m_btn2, m_btn3, m_btn4, m_btn5 };
	wxRadVec = { m_radio1, m_radio2, m_radio3, m_radio4, m_radio5 };
	wxListVec = { m_list1, m_list2, m_list3 };
	wxTextVec = { m_label1 };

	std::pair<int&, int&> windowDimensions = { windowWidth, windowHeight };
	ih.setElementStyles(this, wxBtnVec, wxListVec, wxRadVec, wxTextVec, selectedDirectory, windowDimensions);
}



void cMain::initialSettings() {
	// Populate the recently watched list
	re.retrieveAllViewed(episodeStack, episodeList, episodesViewedHash, filesToDisplay);

	appendEpisodesList();

	m_radio1->SetValue(true);
	m_radio1->SetFocus();
	m_radio5->SetValue(true);
	m_radio5->SetFocus();

	// Populate the directory where our media is located (if used before)
	bool loadFile = dh.LoadPathFromFile(selectedDirectory);	

	if (loadFile) {
		m_list3->SetToolTip(selectedDirectory);
		ih.setElementStyles(this, wxBtnVec, wxListVec, wxRadVec, wxTextVec, selectedDirectory, windowDimensions);
	}
	else {
		m_list3->Hide();
		m_btn1->Disable();
		m_btn2->Disable();			// This button is for "watch continuous"
	}
}

void cMain::appendEpisodesList() {
	// Set how many values to show, max of filesToDisplay (for now), or if files viewed < filesToDisplay, only display those
	int toDisplay = std::min((int)episodeStack.size(), filesToDisplay);

	for (int i = 0; i < toDisplay; i++) {
		m_list2->AppendString(episodeStack.top());
		episodeStack.pop();		
	}
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
			selectedDirectory = newPath;											// Store the path locally, and store it into our text file for later retrieval
			dh.setDirectory(newPath);
			m_btn1->Enable();
		}

		CoTaskMemFree(pidl);
	}

	std::string directoryPath = dh.getDirectory();
	m_list3->SetToolTip(selectedDirectory);											// Set tooltip to show the full path when we hover over m_list3

	int pathLength = (selectedDirectory.length() * 5) + 40;

	// If we already have a populated directory path, clear it
	if (m_list3 != nullptr) {
		m_list3->Clear();
	}

	if (pathLength > 0) {
		ih.setElementStyles(this, wxBtnVec, wxListVec, wxRadVec, wxTextVec, selectedDirectory, windowDimensions);
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
			for (int i = 0; i < std::min(filesToDisplay, (int)episodeList.size()); i++) {
				m_list2->AppendString(episodeList[i]);
			}

			// Append the new episode and reverse the list to show latest watched first
			episodeList.insert(episodeList.begin(), selectedDirectory);
		}
		// Reset our loop counter and re-enable our button to search again, exit from the loop
		loopCounter = 0;
		m_btn1->Enable();

		// Take our file index and find the file name. This lets us open the file in VLC with the full path + episode name
		std::string episodeName = dh.getFileByIndex(vlcPath, randomValue);
		vlcPath += "\\" + episodeName;
		vlcPath = dh.normalizePath(vlcPath, "//", "\\");

		// Store information in our hash and local file to reference that it's been watched, then open the file in VLC
		re.storeRecentWatched(selectedDirectory, episodesViewedHash);
		re.updateRecentWatched(episodeStack, episodeList, filesToDisplay);
		re.openFile(vlcPath);
	}
	// If an episode wasn't found (counter limit hit), display an error in m_list1, reset loop counter, and re-enable the button
	else {
		std::string errorDisplay = "ERROR! Unable to find unviewed episode: " + selectedDirectory;

		m_list1->Clear();
		m_list1->AppendString(errorDisplay);

		m_btn1->Enable();
		loopCounter = 0;
	}
}

// Pick one random episode
void cMain::OnRandomButtonClicked(wxCommandEvent& evt) {
	selectRandomEpisode();

	evt.Skip();
}

// Pick 3 random episodes at a time before being prompted to continue
void cMain::onContinuousButtonClicked(wxCommandEvent& evt) {
	ctsCtr = 0;																					// To keep track of our current loop
	int maxCounter = 3;																			// Max number of episodes back to back before checking in on the user
	watchingCts = true;
	m_list3->Hide();																			// Hide our directory path until back to normal

	while (watchingCts) {		
		if (ctsCtr < maxCounter) {
			selectRandomEpisode();
			ctsCtr++;
		}
		else {
			// Hide main UI, show prompt UI
			ih.showMainUI(false, wxBtnVec, wxListVec, wxRadVec);
			ih.showPromptUI(true, wxBtnVec, wxTextVec);			

			break;
		}
	}

	evt.Skip();
}

void cMain::onNoPromptButtonClicked(wxCommandEvent& evt) {
	watchingCts = false;

	// If this is reached, the user is not still watching, so we hide the prompt UI and return original UI
	ih.showMainUI(true, wxBtnVec, wxListVec, wxRadVec);
	ih.showPromptUI(false, wxBtnVec, wxTextVec);
}

void cMain::onRadioViewNumClicked(wxCommandEvent& evt) {
	switch (evt.GetId()) {
	case 20001:
		setNumToShow(10);
		break;
	case 20002:
		setNumToShow(25);
		break;
	case 20003:
		setNumToShow(50);
		break;
	default:
		break;
	}
}

void cMain::onRadioLightThemeClicked(wxCommandEvent& evt) {
	switch (evt.GetId()) {
	case 20004:
		ih.setLightMode();
		lightMode = true;
		break;
	case 20005:
		ih.setDarkMode();
		lightMode = false;
		break;
	default:
		break;
	}

	// Refresh and update the window
	ih.setElementStyles(this, wxBtnVec, wxListVec, wxRadVec, wxTextVec, selectedDirectory, windowDimensions);
	this->Refresh(true);
	this->Update();
}

void cMain::onResize(wxSizeEvent& event) {
	wxSize newSize = event.GetSize();
	windowWidth = std::max(newSize.GetWidth(), 640);
	windowHeight = std::max(newSize.GetHeight(), 320);

	ih.setElementStyles(this, wxBtnVec, wxListVec, wxRadVec, wxTextVec, selectedDirectory, windowDimensions);;

	event.Skip();
}

void cMain::DestroyListBox(wxListBox* listBox) {
	listBox->Destroy();
}

void cMain::DestroyButton(wxButton* button) {
	button->Destroy();
}
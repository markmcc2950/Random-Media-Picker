// Source: https://www.youtube.com/watch?v=FOIbK4bJKS8

#include "cMain.h"
#include "shobjidl_core.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <windows.h>
#include <shlobj.h>

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_BUTTON(10001, OnRandomButtonClicked)
EVT_BUTTON(10002, onContinuousButtonClicked)
EVT_BUTTON(10003, onBrowseButtonClicked)
EVT_BUTTON(10004, onContinuousButtonClicked)
EVT_BUTTON(10005, onNoPromptButtonClicked)
EVT_RADIOBUTTON(20001, onRadioButtonClicked)
EVT_RADIOBUTTON(20002, onRadioButtonClicked)
EVT_RADIOBUTTON(20003, onRadioButtonClicked)
wxEND_EVENT_TABLE()

RandomEpisode re;
DirectoryHandler dh;

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Random Episode Generator", wxDefaultPosition, wxDefaultSize) {
	// Create UI and initialize all settings (such as pulling info from our local list of stored viewed episodes)
	createUI();
	initialSettings();

	// Bind the event handler that detects when window is resized
	this->Bind(wxEVT_SIZE, &cMain::onResize, this);
}

cMain::~cMain() {
	DestroyButton(m_btn1);
	DestroyButton(m_btn2);
	DestroyButton(m_btn3);
	DestroyButton(m_btn4);
	DestroyButton(m_btn5);
	DestroyListBox(m_list1);
	DestroyListBox(m_list2);
	DestroyListBox(m_list3);
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
	// Manually set size to our screen's current dimensions
	SetSize(windowWidth, windowHeight);
	SetMinSize(wxSize(minDimensions, minDimensions));

	SetBackgroundColour(bgColor);

	// Create our buttons
	m_btn1 = new wxButton(this, 10001, "Watch Random", wxPoint(windowWidth * 0.1, windowHeight * 0.1), wxSize(windowWidth * 0.1, windowHeight * 0.05));
	m_btn2 = new wxButton(this, 10002, "Watch\nContinuous", wxPoint(windowWidth * 0.1, windowHeight * 0.2), wxSize(windowWidth * 0.1, windowHeight * 0.05));
	m_btn3 = new wxButton(this, 10003, "Select\nMedia Path", wxPoint(windowWidth - (windowWidth * 0.2), windowHeight * 0.1), wxSize(windowWidth * 0.1, windowHeight * 0.05));

	// Buttons exclusive to prompts (like are you still watching)
	m_btn4 = new wxButton(this, 10004, "Yes", wxPoint(windowWidth * 0.1, windowHeight * 0.1), wxSize(windowWidth * 0.1, windowHeight * 0.05));
	m_btn5 = new wxButton(this, 10005, "No", wxPoint(windowWidth * 0.1, windowHeight * 0.1), wxSize(windowWidth * 0.1, windowHeight * 0.05));
	m_label1 = new wxStaticText(this, wxID_ANY, "ARE YOU STILL THERE?", wxPoint(50, 50), wxDefaultSize, wxALIGN_CENTER);
	m_label1->SetOwnForegroundColour(fgColor);

	// Create our string boxes
		/*
			- list1: Most recent picked (empty until an episode is picked)
			- list2: Up to 10 most recently watched episodes
			- list3: Selected media root folder
		*/
	m_list1 = new wxListBox(this, wxID_ANY, wxPoint(windowWidth * 0.1, windowHeight * 0.3), wxSize(windowWidth * 0.5, windowHeight * 0.035));			// Most recent episode picked
	m_list2 = new wxListBox(this, wxID_ANY, wxPoint(windowWidth * 0.1, windowHeight * 0.4), wxSize(windowWidth * 0.5, windowHeight * 0.325));			// Most recent N episodes picked (radio buttons)
	m_list3 = new wxListBox(this, wxID_ANY, wxPoint(windowWidth * 0.5, windowHeight * 0.2), wxSize(windowWidth * 0.5, windowHeight * 0.2));				// Media directory path

	// Radio buttons
	m_radio1 = new wxRadioButton(this, 20001, wxString::Format("%d", radio1Val), wxPoint(windowWidth * 0.2, windowHeight * 0.8), wxSize(50, 50));		// Show 10 recent episodes
	m_radio2 = new wxRadioButton(this, 20002, wxString::Format("%d", radio2Val), wxPoint(windowWidth * 0.5, windowHeight * 0.8), wxSize(50, 50));		// Show 25 recent episodes
	m_radio3 = new wxRadioButton(this, 20003, wxString::Format("%d", radio3Val), wxPoint(windowWidth * 0.8, windowHeight * 0.8), wxSize(50, 50));		// Show 50 recent episodes

	setElementStyles();

	// Set tool tips for the buttons
	m_btn1->SetToolTip("Play a Random Episode");
	m_btn2->SetToolTip("Continuous Play Random Episodes");
	m_btn3->SetToolTip("Select Your Media Path");
	// Radio buttons
	m_radio1->SetToolTip(wxString::Format("%d Most Recent Episodes Viewed", radio1Val));
	m_radio2->SetToolTip(wxString::Format("%d Most Recent Episodes Viewed", radio2Val));
	m_radio3->SetToolTip(wxString::Format("%d Most Recent Episodes Viewed", radio3Val));

	// Hide prompt-specific UI
	m_btn4->Hide();
	m_btn5->Hide();
	m_label1->Hide();
}

void cMain::setElementStyles() {
	buttonWidth = std::max(windowWidth * 0.15, (double)minDimensions / 24);
	buttonHeight = std::max((windowHeight * 0.075) + (windowWidth * 0.01), (double)minDimensions / 24);
	int largerDimension = std::max(windowWidth, windowHeight);

	// Set positions based on current window size
	m_btn1->SetPosition(wxPoint(windowWidth * 0.1, windowHeight * 0.075));
	m_btn2->SetPosition(wxPoint(windowWidth * 0.3, windowHeight * 0.075));
	m_btn3->SetPosition(wxPoint(windowWidth - (windowWidth * 0.1) - buttonWidth, windowHeight * 0.075));
	m_list1->SetPosition(wxPoint(windowWidth * 0.1, windowHeight * 0.25));
	m_list2->SetPosition(wxPoint(windowWidth * 0.1, windowHeight * 0.3));
	m_radio1->SetPosition(wxPoint(windowWidth * 0.1, windowHeight * 0.8));
	m_radio2->SetPosition(wxPoint(windowWidth * 0.2, windowHeight * 0.8));
	m_radio3->SetPosition(wxPoint(windowWidth * 0.3, windowHeight * 0.8));

	// Set special prompt dimensions and locations
	m_btn4->SetPosition(wxPoint(windowWidth * 0.3, windowHeight * 0.5));
	m_btn5->SetPosition(wxPoint(windowWidth - (windowWidth * 0.5), windowHeight * 0.5));
	m_btn4->SetSize(wxSize(std::max(windowWidth * 0.2, 64.0), std::max(windowHeight * 0.1, (double)minDimensions / 8)));
	m_btn5->SetSize(wxSize(std::max(windowWidth * 0.2, 64.0), std::max(windowHeight * 0.1, (double)minDimensions / 8)));

	// "Are you still there" text and sizing
	wxFont textFont(std::max(windowWidth * 1.0 * 0.025, 6.0), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	m_label1->SetPosition(wxPoint(windowWidth * 0.3, windowHeight * 0.3));	
	m_label1->SetFont(textFont);

	// Set dimensions based on current window size
	m_btn1->SetSize(wxSize(buttonWidth, buttonHeight));
	m_btn2->SetSize(wxSize(buttonWidth, buttonHeight));
	m_btn3->SetSize(wxSize(buttonWidth, buttonHeight));
	m_list1->SetSize(wxSize(windowWidth * 0.8, windowHeight * 0.045));
	m_list2->SetSize(wxSize(windowWidth * 0.8, windowHeight * 0.4));
	
	m_radio1->SetSize(wxSize(std::min(largerDimension / 10, 50), std::min(largerDimension / 10, 50)));
	m_radio2->SetSize(wxSize(std::min(largerDimension / 10, 50), std::min(largerDimension / 10, 50)));
	m_radio3->SetSize(wxSize(std::min(largerDimension / 10, 50), std::min(largerDimension / 10, 50)));

	// Create the fonts and set them for the lists and buttons
	int listFontSize = std::max(std::max(windowHeight * 1.25, windowWidth * 0.5) * 0.015, (double)minDimensions / 60);
	wxFont btnFont(std::max(std::max(windowHeight, windowWidth) * 0.012, (double)minDimensions / 80), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT);
	wxFont listFont(listFontSize * 0.75, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	wxFont radioFont(std::max(listFontSize * 0.5, 8.0), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

	// Handle m_list3 with its own function
	setMediaDirectory(listFontSize);

	// Button fonts
	m_btn1->SetFont(btnFont);
	m_btn2->SetFont(btnFont);
	m_btn3->SetFont(btnFont);
	m_btn4->SetFont(btnFont);
	m_btn5->SetFont(btnFont);

	// List fonts
	m_list1->SetFont(listFont);
	m_list2->SetFont(listFont);

	// Radio buttons fonts & text color
	m_radio1->SetFont(radioFont);
	m_radio2->SetFont(radioFont);
	m_radio3->SetFont(radioFont);
	m_radio1->SetForegroundColour(fgColor);
	m_radio2->SetForegroundColour(fgColor);
	m_radio3->SetForegroundColour(fgColor);
}

void cMain::initialSettings() {
	// Populate the recently watched list
	re.retrieveAllViewed(episodeStack, episodeList, episodesViewedHash, filesToDisplay);

	appendEpisodesList();
	

	// Populate the directory where our media is located (if used before)
	bool loadFile = dh.LoadPathFromFile(selectedDirectory);	

	if (loadFile) {
		m_list3->SetToolTip(selectedDirectory);
		setElementStyles();
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

void cMain::setMediaDirectory(int fontSize) {
	// Clear our previous browse path directory
	if (m_list3) {
		m_list3->Clear();
	}

	wxFont listFont(fontSize * 0.75, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

	m_list3->AppendString(dh.getDirectory());
	m_list3->SetFont(listFont);

	// Get the character length of our directory path
	wxSize textSize = m_list3->GetTextExtent(selectedDirectory);
	int pathLength = (selectedDirectory.length()) * fontSize * 0.4 + 40;
	int pathListLength = windowWidth * 0.0013 * pathLength;

	// Re-populate the browse list with our new directory path and length, adjusting the UI components accordingly
	m_list3->SetSize(textSize.GetWidth() + 20, textSize.GetHeight() + 10);
	m_list3->SetPosition(wxPoint(windowWidth - (windowWidth * 0.1) - textSize.GetWidth() - 20, windowHeight * 0.075 + (buttonHeight * 1.15)));

	
	
	// Only show this path if we aren't in a "Watch Continuous" loop
	if (!watchingCts) {
		m_list3->Show();
	}
	
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

		CoTaskMemFree(pidl);
	}

	std::string directoryPath = dh.getDirectory();
	m_list3->SetToolTip(directoryPath);														// So when we highlight this with our mouse, it shows the full path

	int pathLength = directoryPath.length();
	pathLength = pathLength * 5 + 40;

	if (m_list3 != nullptr) {
		m_list3->Clear();
	}

	if (pathLength > 0) {
		setElementStyles();
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

	while (watchingCts) {
		if (ctsCtr < maxCounter) {
			selectRandomEpisode();
			ctsCtr++;
		}
		else {
			// Hide main UI, show prompt UI
			showMainUI(false);
			showPromptUI(true);			

			break;
		}
	}

	evt.Skip();
}

void cMain::onNoPromptButtonClicked(wxCommandEvent& evt) {
	watchingCts = false;

	// If this is reached, the user is not still watching, so we hide the prompt UI and return original UI
	showMainUI(true);
	showPromptUI(false);
}

void cMain::onRadioButtonClicked(wxCommandEvent& evt) {
	switch (evt.GetId()) {
	case 20001:
		setNumToShow(radio1Val);
		break;
	case 20002:
		setNumToShow(radio2Val);
		break;
	case 20003:
		setNumToShow(radio3Val);
		break;
	default:
		break;
	}
}

void cMain::onResize(wxSizeEvent& event) {
	wxSize newSize = event.GetSize();
	windowWidth = std::max(newSize.GetWidth(), 640);
	windowHeight = std::max(newSize.GetHeight(), 320);

	if (windowWidth < 640) {
		newSize.SetWidth(windowWidth);
	}
	if (windowHeight < 320) {
		newSize.SetHeight(windowHeight);
	}

	setElementStyles();

	event.Skip();
}

// Shows or hides the startup UI
void cMain::showMainUI(bool show) {
	if (show) {
		m_btn1->Show();
		m_btn2->Show();
		m_btn3->Show();
		m_list1->Show();
		m_list2->Show();
		m_list3->Show();

		m_radio1->Show();
		m_radio2->Show();
		m_radio3->Show();
	}
	else {
		m_btn1->Hide();
		m_btn2->Hide();
		m_btn3->Hide();
		m_list1->Hide();
		m_list2->Hide();
		m_list3->Hide();

		m_radio1->Hide();
		m_radio2->Hide();
		m_radio3->Hide();
	}	
}

// Shows or hides the "Are you still there?" prompt UI
void cMain::showPromptUI(bool show) {
	if (show) {
		m_btn4->Show();
		m_btn5->Show();
		m_label1->Show();
	}
	else {
		m_btn4->Hide();
		m_btn5->Hide();
		m_label1->Hide();
	}	
}

void cMain::DestroyListBox(wxListBox* listBox) {
	listBox->Destroy();
}

void cMain::DestroyButton(wxButton* button) {
	button->Destroy();
}
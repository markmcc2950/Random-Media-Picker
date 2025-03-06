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
EVT_BUTTON(10002, OnRandomButtonClicked)
EVT_BUTTON(10003, onBrowseButtonClicked)
wxEND_EVENT_TABLE()

RandomEpisode re;
extern DirectoryHandler dh;

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Random Episode Generator", wxDefaultPosition, wxDefaultSize)
{
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

cMain::~cMain() 
{
	DestroyButton(m_btn1);
	DestroyButton(m_btn2);
	DestroyButton(m_btn3);
	DestroyListBox(m_list1);
	DestroyListBox(m_list2);
	DestroyListBox(m_list3);
}

void cMain::setElementSizes() {
	// Set positions
	m_btn1->SetPosition(wxPoint(windowWidth * 0.1, windowHeight * 0.1));
	m_btn2->SetPosition(wxPoint(windowWidth * 0.1, windowHeight * 0.2));
	m_btn3->SetPosition(wxPoint(windowWidth - (windowWidth * 0.2), windowHeight * 0.1));
	m_list1->SetPosition(wxPoint(windowWidth * 0.1, windowHeight * 0.3));
	m_list2->SetPosition(wxPoint(windowWidth * 0.1, windowHeight * 0.4));

	// Set dimensions
	m_btn1->SetSize(wxSize(windowWidth * 0.1, windowHeight * 0.05));
	m_btn2->SetSize(wxSize(windowWidth * 0.1, windowHeight * 0.05));
	m_btn3->SetSize(wxSize(windowWidth * 0.1, windowHeight * 0.05));
	m_list1->SetSize(wxSize(windowWidth * 0.5, windowHeight * 0.035));
	m_list2->SetSize(wxSize(windowWidth * 0.5, windowHeight * 0.325));

	// Handle m_list3 with its own function
	setPathLength();

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

	// Set how many values to show, max of 10
	int toDisplay = std::min((int)episodeStack.size(), 10);

	for (int i = 0; i < toDisplay; i++) {
		episodeList[i] = episodeStack.top();
		episodeStack.pop();
		m_list2->AppendString(episodeList[i]);
	}

	// Populate the browse button (if used before)
	bool loadFile = dh.LoadPathFromFile(selectedDirectory);	

	if (loadFile) {
		setPathLength();
	}
	else {
		m_list3->Hide();
		m_btn1->Disable();
		m_btn2->Disable();			// Will enable when extra logic added
	}
}

void cMain::setPathLength() {
	if (m_list3) {
		m_list3->Clear();
	}
	int pathLength = (selectedDirectory.length()) * 5 + 40;
	int pathListLength = windowWidth * 0.0013 * pathLength;
	m_list3->SetSize(pathListLength, windowHeight * 0.035);
	m_list3->SetPosition(wxPoint(windowWidth - (windowWidth * 0.1) - pathListLength, windowHeight * 0.2));
	m_list3->AppendString(dh.getDirectory());
	m_list3->Show();
	m_btn1->Enable();
	m_btn2->Disable();
}

void cMain::onBrowseButtonClicked(wxCommandEvent& evt)
{
	// Initialize the BROWSEINFO structure
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(bi));

	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

	// Display the "Browse for Folder" dialog box
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl != NULL)
	{
		TCHAR path[MAX_PATH];
		if (SHGetPathFromIDList(pidl, path))
		{
			std::cout << "Selected folder: " << path << std::endl;
			std::string newPath = re.tcharToString(path);
			std::cout << newPath << std::endl;
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

	if (m_list3 != nullptr) {
		m_list3->Clear();
	}
	if (pathLength > 0) {
		setPathLength();
	}
	
}

void cMain::findDirectoryPath() {
	bool isValidFolder = true;

	srand(time(0));

	while (isValidFolder) {
		directoryCount = dh.getDirectoryFolderCount(selectedDirectory);
		if (directoryCount > 0) {																	// Append name to directory
			rand() % 2 ? randomValue = rand() % directoryCount : randomValue = re.getRandomNumber(directoryCount);

			std::string nextFolder = re.getRandomFolder(selectedDirectory, randomValue);			// Get name of randomly chosen folder
			selectedDirectory += "//" + nextFolder;
		}
		else {																						// If we return no more folders
			fileCount = dh.getNumFilesInFolder(selectedDirectory);
			if (fileCount > 0) {
				randomValue = rand() % fileCount;
				vlcPath = selectedDirectory;
				selectedDirectory += "//Episode " + std::to_string(randomValue + 1);

				// Format our string prior to checking with the written file
				selectedDirectory = dh.formatFinalDirectory(selectedDirectory, "//", "\\", false);
				selectedDirectory = dh.formatFinalDirectory(selectedDirectory, dh.getDirectory() + "\\", "", false);
				selectedDirectory = dh.formatFinalDirectory(selectedDirectory, "\\", "", true);

				// Returns false if we HAVEN'T watched it yet
				if (!re.hasEpisodeBeenViewed(selectedDirectory)) {
					isValidFolder = false;

					m_list1->Clear();
					m_list1->AppendString(selectedDirectory);

					// Don't show the first episode, but update the list after each episode
					if (selectedDirectory != "") {
						m_list2->Clear();						

						for (int i = 0; i < 10; i++) {
							m_list2->AppendString(episodeList[i]);
						}

						for (int i = 9; i > 0; i--) {
							episodeList[i] = episodeList[i - 1];
						}

						episodeList[0] = selectedDirectory;
					}
					maxLoop = 0;
					m_btn1->Enable();																// Re-Enable the button to be able to search again

					break;
				}
				// If we have watched it, reset and continue searching
				else {
					isValidFolder = true;
					selectedDirectory = dh.getDirectory();
					loopCounter = -1;
				}
			}
			// We selected an empty folder, reset search
			else {
				isValidFolder = true;
				selectedDirectory = dh.getDirectory();
				loopCounter = 0;
			}
		}

		++loopCounter;
		if (loopCounter >= 10 && isValidFolder) {													// If we're too many folders in, reset
			isValidFolder = true;
			selectedDirectory = dh.getDirectory();
			loopCounter = 0;
		}

		++maxLoop;
		if (maxLoop >= 10000) {
			std::string errorDisplay = "ERROR! Unable to find an episode: " + selectedDirectory;

			m_list1->Clear();
			m_list1->AppendString(errorDisplay);

			m_btn1->Enable();																	// Re-Enable the button to be able to search again
			maxLoop = 0;
			break;
		}
	}
}

void cMain::OnRandomButtonClicked(wxCommandEvent& evt)
{
	// Disable button and show we are searching
	m_btn1->Disable();
	m_list1->Clear();
	m_list1->AppendString("SEARCHING...");

	// Get the number of folders within the selected directory
	selectedDirectory = dh.getDirectory();
	//bool isValidFolder = true;

	// Grab a random series, season, and episode
	findDirectoryPath();

	// Re-enable the find button after finding an episode
	m_btn1->Enable();

	std::string episodeName = dh.getFileByIndex(vlcPath, randomValue);

	vlcPath += "\\" + episodeName;
	vlcPath = dh.formatFinalDirectory(vlcPath, "//", "\\", false);

	re.openFile(vlcPath);
	
	// Views episodes continuously until user closes the application
	//if (re.openFile(vlcPath)) {
	//	OnRandomButtonClicked(evt);
	//}
	re.updateRecentWatched(episodeStack);

	evt.Skip();
}

void cMain::OnContinuousButtonClicked(wxCommandEvent& evt) {
	/*
		- Display message to select a specific directory to resume
		- When a directory is selected, look for `FOLDER_NAME.txt`
		- If exists, open folder and display last 10 viewed episodes
		- When button is played, search for last viewed episode
			- Whatever index value that file is in the folder, add +1 to start next episode
		- Look for index (lastFile + 1) and begin that file
			- If last viewed video was last file in folder, look up next series
	*/

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
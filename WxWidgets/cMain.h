#pragma once
#include "wx/wx.h"
#include <stack>
#include <string>
#include <unordered_map>
#include "DirectoryHandler.h"
#include "RandomEpisode.h"

class cMain : public wxFrame {
public:
	cMain();
	~cMain();

public:	
	void setNumToShow(int n);
	wxDECLARE_EVENT_TABLE();

private:
	void setElementStyles();
	void initialSettings();
	void DestroyListBox(wxListBox* listBox);
	void DestroyButton(wxButton* button);
	void createUI();
	void onResize(wxSizeEvent& event);
	void setMediaDirectory(int fontSize);
	void selectRandomEpisode();
	void appendEpisodesList();
	void showMainUI(bool show);
	void showPromptUI(bool show);	

	// Interaction-specific functions
	void OnRandomButtonClicked(wxCommandEvent& evt);
	void onBrowseButtonClicked(wxCommandEvent& evt);
	void onContinuousButtonClicked(wxCommandEvent& evt);
	void onNoPromptButtonClicked(wxCommandEvent& evt);
	void onRadioButtonClicked(wxCommandEvent& evt);

	// Main UI
	// Buttons
	wxButton* m_btn1 = nullptr;
	wxButton* m_btn2 = nullptr;
	wxButton* m_btn3 = nullptr;
	// Lists
	wxListBox* m_list1 = nullptr;
	wxListBox* m_list2 = nullptr;
	wxListBox* m_list3 = nullptr;

	// Are you still there/Button prompts (Yes/No)
	wxStaticText* m_label1 = nullptr;
	wxButton* m_btn4 = nullptr;
	wxButton* m_btn5 = nullptr;

	// Radio buttons
	wxRadioButton* m_radio1 = nullptr;
	wxRadioButton* m_radio2 = nullptr;
	wxRadioButton* m_radio3 = nullptr;

	// Foreground and Background colors
	wxColour fgColor = wxColour(255, 255, 255);
	wxColour bgColor = wxColour(40, 40, 40);

	std::string recentEpisode = "";
	std::vector<std::string> episodeList;
	std::string vlcPath = "";
	std::string selectedDirectory = "";

	std::vector<std::string> recentWatched;
	std::stack<std::string> episodeStack;
	std::unordered_map<std::string, bool> episodesViewedHash;

	bool episodePicked = false;
	bool watchingCts = false;											// If user is watching continuous

	int directoryCount = 0;
	int fileCount = 0;
	int randomValue = -1;
	int loopCounter = 0;
	int filesToDisplay = 15;
	int ctsCtr = 0;														// Counter for continuous watching

	int windowWidth = 900;
	int windowHeight = 900;

	int minDimensions = 600;											// Minimum window width or height dimensions

	int radio1Val = 10;
	int radio2Val = 25;
	int radio3Val = 50;

	// Button dimensions
	int buttonHeight = 0;
	int buttonWidth = 0;
};


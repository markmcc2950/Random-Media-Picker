#pragma once
#include "wx/wx.h"
#include "wx/statline.h"
#include <stack>
#include <string>
#include <unordered_map>
#include "DirectoryHandler.h"
#include "InterfaceHandler.h"
#include "RandomEpisode.h"

class cMain : public wxFrame {
public:
	cMain();
	~cMain();

public:	
	wxDECLARE_EVENT_TABLE();

private:
	void initialSettings();
	void onResize(wxSizeEvent& event);

	void createUI();
	void setNumToShow(int n);	
	void selectRandomEpisode();

	// Interaction-specific functions
	void OnRandomButtonClicked(wxCommandEvent& evt);
	void onBrowseButtonClicked(wxCommandEvent& evt);
	void onContinuousButtonClicked(wxCommandEvent& evt);
	void onOpenSettingsButtonClicked(wxCommandEvent& evt);
	void onCloseSettingsButtonClicked(wxCommandEvent& evt);
	void onNoPromptButtonClicked(wxCommandEvent& evt);
	void onRadioViewNumClicked(wxCommandEvent& evt);
	void onRadioLightThemeClicked(wxCommandEvent& evt);

	// Main UI
	// Vectors to hold UI elements
	std::vector<wxButton*> wxBtnVec;
	std::vector<wxRadioButton*> wxRadVec;
	std::vector<wxListBox*> wxListVec;
	std::vector<wxStaticText*> wxTextVec;
	std::vector<wxStaticLine*> wxLineVec;

	// wx Member UI Elements
	wxButton* m_btn1 = nullptr;						// Click buttons
	wxButton* m_btn2 = nullptr;
	wxButton* m_btn3 = nullptr;
	wxButton* m_btn4 = nullptr;
	wxButton* m_btn5 = nullptr;
	wxButton* m_btn6 = nullptr;						// -- Settings button
	wxRadioButton* m_radio1 = nullptr;				// Radio buttons
	wxRadioButton* m_radio2 = nullptr;
	wxRadioButton* m_radio3 = nullptr;
	wxRadioButton* m_radio4 = nullptr;
	wxRadioButton* m_radio5 = nullptr;
	wxRadioButton* m_radio6 = nullptr;
	wxListBox* m_list1 = nullptr;					// Lists
	wxListBox* m_list2 = nullptr;
	wxListBox* m_list3 = nullptr;
	wxStaticText* m_label1 = nullptr;				// Static text
	wxStaticText* s_label1 = nullptr;
	wxStaticText* s_label2 = nullptr;
	wxStaticText* s_label3 = nullptr;
	wxStaticText* s_label4 = nullptr;
	wxStaticLine* s_line1 = nullptr;				// Static lines

	// Settings Menu UI Elements
	wxButton* s_btn1 = nullptr;						// -- Close settings
	wxButton* s_btn2 = nullptr;						// -- Media Path settings
	wxButton* s_btn3 = nullptr;						// -- Application settings
	

	std::string recentEpisode = "";
	std::vector<std::string> episodeList;
	std::string vlcPath = "";
	std::string selectedDirectory = "";

	std::vector<std::string> recentWatched;
	std::unordered_map<std::string, bool> episodesViewedHash;

	int windowWidth = 900;							// Window dimensions
	int windowHeight = 900;
	std::pair<int&, int&> windowDimensions = { windowWidth, windowHeight };

	bool episodePicked = false;
	bool watchingCts = false;											// If user is watching continuous
	bool lightMode = false;												// Light Mode/Dark Mode

	int directoryCount = 0;
	int fileCount = 0;
	int randomValue = -1;
	int loopCounter = 0;
	int filesToDisplay = 10;
	int ctsCtr = 0;														// Counter for continuous watching
	int minDimensions = 600;



	

	
};


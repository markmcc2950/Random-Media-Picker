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
	void OnRandomButtonClicked(wxCommandEvent& evt);
	void setElementSizes();
	void initialSettings();
	void onBrowseButtonClicked(wxCommandEvent& evt);
	void onContinuousButtonClicked(wxCommandEvent& evt);
	void onNoPromptButtonClicked(wxCommandEvent& evt);
	void DestroyListBox(wxListBox* listBox);
	void DestroyButton(wxButton* button);
	
	wxDECLARE_EVENT_TABLE();

private:
	void onResize(wxSizeEvent& event);
	void setMediaDirectory(int fontSize);
	void selectRandomEpisode();
	void appendEpisodesList();

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

	int windowWidth = 1280;
	int windowHeight = 720;
};


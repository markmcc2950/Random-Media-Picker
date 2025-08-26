#pragma once
#include "wx/wx.h"
#include <stack>
#include <string>
#include <unordered_map>
#include "RandomEpisode.h"
#include "DirectoryHandler.h"

class cMain : public wxFrame {
public:
	cMain();
	~cMain();

public:
	void findDirectoryPath();
	void OnRandomButtonClicked(wxCommandEvent& evt);
	void setElementSizes();
	void initialSettings();
	void onBrowseButtonClicked(wxCommandEvent& evt);
	void OnContinuousButtonClicked(wxCommandEvent& evt);
	void DestroyListBox(wxListBox* listBox);
	void DestroyButton(wxButton* button);
	
	wxDECLARE_EVENT_TABLE();

private:
	void onResize(wxSizeEvent& event);
	void setMediaDirectory();

	wxButton* m_btn1 = nullptr;
	wxButton* m_btn2 = nullptr;
	wxButton* m_btn3 = nullptr;

	wxTextCtrl* m_txt1 = nullptr;

	wxListBox* m_list1 = nullptr;
	wxListBox* m_list2 = nullptr;
	wxListBox* m_list3 = nullptr;

	std::string recentEpisode = "";
	std::string episodeList[10];
	std::string vlcPath = "";
	std::string selectedDirectory = "";

	std::vector<std::string> recentWatched;
	std::stack<std::string> episodeStack;

	bool episodePicked = false;

	int directoryCount = 0;
	int fileCount = 0;
	int randomValue = 0;
	int loopCounter = 0;
	int maxLoop = 0;
	int filesToDisplay = 10;

	int windowWidth = 1280;
	int windowHeight = 720;
};


#ifndef InterfaceHandler_h
#define InterfaceHandler_h

#include "wx/wx.h"
#include "wx/statline.h"
#include "DirectoryHandler.h"


class InterfaceHandler {
public:
	void setElementStyles(wxFrame* frame,
		std::vector<wxButton*>& wxBtnVec,
		std::vector<wxListBox*>& wxListVec,
		std::vector<wxRadioButton*>& wxRadVec,
		std::vector<wxStaticText*>& wxTextVec,
		std::vector<wxStaticLine*>& wxLineVec,
		const std::string& selectedDirectory,
		std::pair<int&, int&>& windowDimensions);
	void showSettingsUI(bool show, 
		std::vector<wxButton*>& wxBtnVec, 
		std::vector<wxStaticText*>& wxTextVec, 
		std::vector<wxListBox*>& wxListVec, 
		std::vector<wxRadioButton*>& wxRadVec, 
		std::vector<wxStaticLine*>& wxLineVec);
	void setMediaDirectory(int fontSize, std::vector<wxListBox*>& wxListVec, const std::string& selectedDirectory);
	void showMainUI(bool show, std::vector<wxButton*>& wxBtnVec, std::vector<wxListBox*>& wxListVec, std::vector<wxRadioButton*>& wxRadVec);
	void showPromptUI(bool show, std::vector<wxButton*>& wxBtnVec, std::vector<wxStaticText*>& wxTextVec);
	void setDarkMode();
	void setLightMode();
private:
	// Foreground and Background colors
	wxColour colorLight = wxColour(255, 255, 255);
	wxColour colorDark = wxColour(40, 40, 40);
	wxColour colorFG = wxColour(255, 255, 255);
	wxColour colorBG = wxColour(40, 40, 40);
	wxColour colorGrey = wxColour(128, 128, 128);

	int buttonHeight = 0;							// Button dimensions
	int buttonWidth = 0;
	int windowWidth = 900;							// Window dimensions
	int windowHeight = 900;
	int minDimensions = 600;						// Minimum window width or height dimensions
	int radio1Val = 10;								// Radio button values (amount to show in m_list2)
	int radio2Val = 25;
	int radio3Val = 50;
};

#endif
#include "InterfaceHandler.h"
#include "wx/listbox.h"

extern DirectoryHandler dh;

void InterfaceHandler::setElementStyles(wxFrame* frame, 
	std::vector<wxButton*>& wxBtnVec, 
	std::vector<wxListBox*>& wxListVec, 
	std::vector<wxRadioButton*>& wxRadVec, 
	std::vector<wxStaticText*>& wxTextVec,
	const std::string& selectedDirectory,
	std::pair<int&, int&>& windowDimensions)
{
	windowWidth = windowDimensions.first;
	windowHeight = windowDimensions.second;

	// Fonts
	int listFontSize = std::max(std::max(windowHeight * 1.25, windowWidth * 0.5) * 0.015, (double)minDimensions / 60);
	wxFont textFont(std::max(windowWidth * 1.0 * 0.025, 6.0), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	wxFont btnFont(std::max(std::max(windowHeight, windowWidth) * 0.012, (double)minDimensions / 80), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT);
	wxFont listFont(listFontSize * 0.9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	wxFont radioFont(std::max(listFontSize * 0.5, 8.0), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Segoe UI Emoji");

	// Set frame elements
	frame->SetBackgroundColour(colorBG);
	frame->SetSize(windowWidth, windowHeight);
	frame->SetMinSize(wxSize(minDimensions, minDimensions));

	buttonWidth = std::max(windowWidth * 0.15, (double)minDimensions / 24);
	buttonHeight = std::max((windowHeight * 0.075) + (windowWidth * 0.01), (double)minDimensions / 24);
	int largerDimension = std::max(windowWidth, windowHeight);

	// Set positions & dimensions based on current window size
	wxBtnVec[0]->SetPosition(wxPoint(windowWidth * 0.1, windowHeight * 0.075));
	wxBtnVec[1]->SetPosition(wxPoint(windowWidth * 0.3, windowHeight * 0.075));
	wxBtnVec[2]->SetPosition(wxPoint(windowWidth - (windowWidth * 0.1) - buttonWidth, windowHeight * 0.075));
	
	wxListVec[0]->SetPosition(wxPoint(windowWidth * 0.1, windowHeight * 0.25));
	wxListVec[1]->SetPosition(wxPoint(windowWidth * 0.1, windowHeight * 0.3));
	
	wxRadVec[0]->SetPosition(wxPoint(windowWidth * 0.1, windowHeight * 0.8));
	wxRadVec[1]->SetPosition(wxPoint(windowWidth * 0.2, windowHeight * 0.8));
	wxRadVec[2]->SetPosition(wxPoint(windowWidth * 0.3, windowHeight * 0.8));
	wxRadVec[3]->SetPosition(wxPoint(windowWidth * 0.4, windowHeight * 0.8));
	wxRadVec[4]->SetPosition(wxPoint(windowWidth * 0.75, windowHeight * 0.8));
	wxRadVec[5]->SetPosition(wxPoint(windowWidth * 0.8, windowHeight * 0.8));

	// Set special prompt dimensions and locations
	wxBtnVec[3]->SetPosition(wxPoint(windowWidth * 0.3, windowHeight * 0.5));
	wxBtnVec[4]->SetPosition(wxPoint(windowWidth - (windowWidth * 0.5), windowHeight * 0.5));

	// "Are you still there" text and sizing
	wxTextVec[0]->SetPosition(wxPoint(windowWidth * 0.25, windowHeight * 0.3));
	wxTextVec[0]->SetFont(textFont);
	wxListVec[0]->SetSize(wxSize(windowWidth * 0.8, windowHeight * 0.045));
	wxListVec[1]->SetSize(wxSize(windowWidth * 0.8, windowHeight * 0.4));

	// Handle m_list3 with its own function
	setMediaDirectory(listFontSize, wxListVec, selectedDirectory);
	//wxBtnVec[0]->Enable();
	//wxBtnVec[1]->Enable();

	// Button fonts and colors
	for (auto* btn : wxBtnVec) {
		// Set all to the standard button size
		btn->SetSize(wxSize(buttonWidth, buttonHeight));

		// Set all fonts and colors
		btn->SetFont(btnFont);
		btn->SetBackgroundColour(colorBG);
		btn->SetForegroundColour(colorFG);
	}

	// List fonts and colors
	for (auto* list : wxListVec) {
		list->SetFont(listFont);
		list->SetBackgroundColour(colorBG);
		list->SetForegroundColour(colorFG);
	}

	// Radio buttons fonts & text color
	for (auto* rad : wxRadVec) {
		rad->SetSize(wxSize(std::min(largerDimension / 10, 32), std::min(largerDimension / 10, 32)));
		rad->SetFont(radioFont);
		rad->SetForegroundColour(colorFG);
	}

	// Static text color
	for (auto* text : wxTextVec) {
		text->SetForegroundColour(colorFG);
	}
}

void InterfaceHandler::setMediaDirectory(int fontSize, std::vector<wxListBox*>& wxListVec, const std::string& selectedDirectory) {
	// Clear our previous browse path directory
	if (wxListVec[2]) {
		wxListVec[2]->Clear();
	}

	wxFont listFont(fontSize * 0.75, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

	wxListVec[2]->AppendString(dh.getDirectory());
	wxListVec[2]->SetFont(listFont);

	// Get the character length of our directory path
	wxSize textSize = wxListVec[2]->GetTextExtent(selectedDirectory);
	int pathLength = (selectedDirectory.length()) * fontSize * 0.4 + 40;
	int pathListLength = windowWidth * 0.0013 * pathLength;

	// Re-populate the browse list with our new directory path and length, adjusting the UI components accordingly
	wxListVec[2]->SetSize(textSize.GetWidth() + 20, textSize.GetHeight() + 10);
	wxListVec[2]->SetPosition(wxPoint(windowWidth - (windowWidth * 0.1) - textSize.GetWidth() - 20, windowHeight * 0.075 + (buttonHeight * 1.15)));
}

// Shows or hides the startup UI
void InterfaceHandler::showMainUI(bool show, std::vector<wxButton*>& wxBtnVec, std::vector<wxListBox*>& wxListVec, std::vector<wxRadioButton*>& wxRadVec) {
	if (show) {
		wxBtnVec[0]->Show();
		wxBtnVec[1]->Show();
		wxBtnVec[2]->Show();
		wxListVec[0]->Show();
		wxListVec[1]->Show();
		wxListVec[2]->Show();

		wxRadVec[0]->Show();
		wxRadVec[1]->Show();
		wxRadVec[2]->Show();
		wxRadVec[3]->Show();
		wxRadVec[4]->Show();
		wxRadVec[5]->Show();
	}
	else {
		wxBtnVec[0]->Hide();
		wxBtnVec[1]->Hide();
		wxBtnVec[2]->Hide();
		wxListVec[0]->Hide();
		wxListVec[1]->Hide();
		wxListVec[2]->Hide();

		wxRadVec[0]->Hide();
		wxRadVec[1]->Hide();
		wxRadVec[2]->Hide();
		wxRadVec[3]->Hide();
		wxRadVec[4]->Hide();
		wxRadVec[5]->Hide();
	}
}

// Shows or hides the "Are you still there?" prompt UI
void InterfaceHandler::showPromptUI(bool show, std::vector<wxButton*>& wxBtnVec, std::vector<wxStaticText*>& wxTextVec) {
	if (show) {
		wxBtnVec[3]->Show();
		wxBtnVec[4]->Show();
		wxTextVec[0]->Show();
	}
	else {
		wxBtnVec[3]->Hide();
		wxBtnVec[4]->Hide();
		wxTextVec[0]->Hide();
	}
}

void InterfaceHandler::setDarkMode() {
	colorFG = colorLight;
	colorBG = colorDark;
}

void InterfaceHandler::setLightMode() {
	colorFG = colorDark;
	colorBG = colorLight;
}
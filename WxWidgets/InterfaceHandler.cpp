#include "InterfaceHandler.h"
#include "wx/listbox.h"

extern DirectoryHandler dh;

void InterfaceHandler::setElementStyles(wxFrame* frame, 
	std::vector<wxButton*>& wxBtnVec, 
	std::vector<wxListBox*>& wxListVec, 
	std::vector<wxRadioButton*>& wxRadVec, 
	std::vector<wxStaticText*>& wxTextVec,
	std::vector<wxStaticLine*>& wxLineVec,
	const std::string& selectedDirectory,
	std::pair<int&, int&>& windowDimensions)
{
	windowWidth = windowDimensions.first;
	windowHeight = windowDimensions.second;

	// Fonts
	int listFontSize = std::max(std::max(windowHeight * 1.25, windowWidth * 0.5) * 0.015, (double)minDimensions / 60);
	wxFont textFont(std::max(windowWidth * 1.0 * 0.025, 6.0), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	wxFont btnFont(std::max(std::max(windowHeight, windowWidth) * 0.012, (double)minDimensions / 80), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT);
	wxFont settingsBtnFont(std::max(windowHeight * 0.024, (double)minDimensions / 40), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT);
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
	wxBtnVec[5]->SetPosition(wxPoint(windowWidth - (windowWidth * 0.1) - (buttonHeight / 2), windowHeight * 0.075));		// Settings button
	
	wxListVec[0]->SetPosition(wxPoint(windowWidth * 0.1, windowHeight * 0.25));
	wxListVec[1]->SetPosition(wxPoint(windowWidth * 0.1, windowHeight * 0.3));
	
	wxRadVec[0]->SetPosition(wxPoint(windowWidth * 0.1, windowHeight * 0.8));
	wxRadVec[1]->SetPosition(wxPoint(windowWidth * 0.2, windowHeight * 0.8));
	wxRadVec[2]->SetPosition(wxPoint(windowWidth * 0.3, windowHeight * 0.8));
	wxRadVec[3]->SetPosition(wxPoint(windowWidth * 0.4, windowHeight * 0.8));
	wxRadVec[4]->SetPosition(wxPoint(windowWidth * 0.8, windowHeight * 0.8));
	wxRadVec[5]->SetPosition(wxPoint(windowWidth * 0.85, windowHeight * 0.8));

	// Set special prompt dimensions and locations
	wxBtnVec[3]->SetPosition(wxPoint(windowWidth * 0.3, windowHeight * 0.5));
	wxBtnVec[4]->SetPosition(wxPoint(windowWidth - (windowWidth * 0.5), windowHeight * 0.5));

	// "Are you still there" text and sizing ---------------------------------------------------------
	wxTextVec[0]->SetPosition(wxPoint(windowWidth * 0.25, windowHeight * 0.3));
	wxListVec[0]->SetSize(wxSize(windowWidth * 0.8, windowHeight * 0.045));
	wxListVec[1]->SetSize(wxSize(windowWidth * 0.8, windowHeight * 0.4));

	// Settings menu UI elements ---------------------------------------------------------------------
	wxBtnVec[2]->SetPosition(wxPoint(windowWidth * 0.3, windowHeight * 0.1));
	wxBtnVec[6]->SetPosition(wxPoint(windowWidth * 0.05, windowHeight - (buttonHeight) - (windowHeight * 0.1)));
	wxBtnVec[7]->SetPosition(wxPoint(windowWidth * 0.05, (windowHeight * 0.1)));
	wxBtnVec[7]->SetWindowStyleFlag(wxBORDER_NONE);										// Disables the button border
	wxBtnVec[8]->SetPosition(wxPoint(windowWidth * 0.05, (windowHeight * 0.25)));
	wxBtnVec[8]->SetWindowStyleFlag(wxBORDER_NONE);
	wxLineVec[0]->SetPosition(wxPoint(windowWidth * 0.25, windowHeight * 0.025));
	wxLineVec[0]->SetSize(wxSize((windowWidth * 0.01), (windowHeight * 0.875)));
	wxTextVec[1]->SetPosition(wxPoint(windowWidth * 0.3, windowHeight * 0.025));
	wxTextVec[2]->SetPosition(wxPoint(windowWidth * 0.3, windowHeight * 0.225));
	wxTextVec[3]->SetPosition(wxPoint(windowWidth * 0.3, windowHeight * 0.425));
	wxTextVec[4]->SetPosition(wxPoint(windowWidth * 0.3, windowHeight * 0.625));

	// Handle m_list3 with its own function
	setMediaDirectory(listFontSize, wxListVec, selectedDirectory);

	// Button fonts and colors
	for (auto* btn : wxBtnVec) {
		if (btn == wxBtnVec[5]) {
			// Specially set the size for our settings button
			btn->SetSize(wxSize(buttonHeight / 2, buttonHeight / 2));
			btn->SetFont(settingsBtnFont);

			btn->SetBackgroundColour(colorBG);
			btn->SetForegroundColour(colorFG);
		}
		else if (btn == wxBtnVec[8]) {
			// Set to the standard button size
			btn->SetSize(wxSize(buttonWidth, buttonHeight));

			// Set fonts and colors
			btn->SetFont(btnFont);
			btn->SetForegroundColour(colorGrey);
			btn->SetBackgroundColour(colorBG);
		}
		else {
			// Set all to the standard button size
			btn->SetSize(wxSize(buttonWidth, buttonHeight));

			// Set all fonts and colors
			btn->SetFont(btnFont);

			btn->SetBackgroundColour(colorBG);
			btn->SetForegroundColour(colorFG);
		}
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
		text->SetFont(textFont);
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
	wxListVec[2]->SetSize((textSize.GetWidth() * 1.1) + (5 * fontSize), textSize.GetHeight() + 10);
	wxListVec[2]->SetPosition(wxPoint((windowWidth * 0.3) + (buttonWidth * 1.2), windowHeight * 0.11));
}

// Shows or hides the startup UI
void InterfaceHandler::showMainUI(bool show, std::vector<wxButton*>& wxBtnVec, std::vector<wxListBox*>& wxListVec, std::vector<wxRadioButton*>& wxRadVec) {
	if (show) {
		wxBtnVec[0]->Show();
		wxBtnVec[1]->Show();
		wxBtnVec[5]->Show();
		wxListVec[0]->Show();
		wxListVec[1]->Show();

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
		wxBtnVec[5]->Hide();
		wxListVec[0]->Hide();
		wxListVec[1]->Hide();

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

void InterfaceHandler::showSettingsUI(
	bool show, 
	std::vector<wxButton*>& wxBtnVec, 
	std::vector<wxStaticText*>& wxTextVec, 
	std::vector<wxListBox*>& wxListVec, 
	std::vector<wxRadioButton*>& wxRadVec, 
	std::vector<wxStaticLine*>& wxLineVec
) {
	if (show) {
		// Hide main UI
		wxBtnVec[0]->Hide();
		wxBtnVec[1]->Hide();		
		wxBtnVec[3]->Hide();
		wxBtnVec[4]->Hide();
		wxBtnVec[5]->Hide();

		wxListVec[0]->Hide();
		wxListVec[1]->Hide();		

		wxTextVec[0]->Hide();

		wxRadVec[0]->Hide();
		wxRadVec[1]->Hide();
		wxRadVec[2]->Hide();
		wxRadVec[3]->Hide();
		wxRadVec[4]->Hide();
		wxRadVec[5]->Hide();

		// Show settings UI
		wxBtnVec[2]->Show();
		wxBtnVec[6]->Show();
		wxBtnVec[7]->Show();
		wxBtnVec[8]->Show();
		wxListVec[2]->Show();
		wxLineVec[0]->Show();
		wxTextVec[1]->Show();
		wxTextVec[2]->Show();
		wxTextVec[3]->Show();
		wxTextVec[4]->Show();
	}
	else {
		// Hide settings UI
		wxBtnVec[2]->Hide();
		wxBtnVec[6]->Hide();
		wxBtnVec[7]->Hide();
		wxBtnVec[8]->Hide();
		wxListVec[2]->Hide();
		wxLineVec[0]->Hide();
		wxTextVec[1]->Hide();
		wxTextVec[2]->Hide();
		wxTextVec[3]->Hide();
		wxTextVec[4]->Hide();

		// Show main UI
		wxBtnVec[0]->Show();
		wxBtnVec[1]->Show();
		wxBtnVec[5]->Show();

		wxListVec[0]->Show();
		wxListVec[1]->Show();

		wxRadVec[0]->Show();
		wxRadVec[1]->Show();
		wxRadVec[2]->Show();
		wxRadVec[3]->Show();
		wxRadVec[4]->Show();
		wxRadVec[5]->Show();
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
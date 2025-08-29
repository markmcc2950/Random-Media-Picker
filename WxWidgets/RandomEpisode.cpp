#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sstream>
#include <fstream>
#include <iostream>

#define NOMINMAX
#include <windows.h>
#include <ShlObj.h>
#include <tchar.h>
#include <string>

#include "RandomEpisode.h"

/*

	TODO:	Need to save the values of the episode, season, and series to a global variable
			Use this information to be passed into the values to be read to grab the actual episode information for VLC player
			-	Find the episode name based on the episode int number given

*/

std::string RandomEpisode::tcharToString(TCHAR toConvert[]) {
	int tstr_len = _tcslen(toConvert);																	// Get the length of TCHAR string

	char* buffer = new char[tstr_len + 1];																// Create buffer to hold the converted string

	// Convert TCHAR to char using the WideCharToMultiByte function
	WideCharToMultiByte(CP_UTF8, 0, toConvert, -1, buffer, tstr_len, nullptr, nullptr);
	buffer[tstr_len] = '\0';																			// Add null terminator to the converted string

	std::string str(buffer);																			// Create std::string from converted char buffer

	delete[] buffer;																					// Free memory used by buffer

	return str;
}

std::string RandomEpisode::showRecentEpisodes(int i) {
	return recentEpisodesList[i];
}

bool RandomEpisode::openFile(std::string episodePath) {
	// Get the application (VLC) to use
	const char* appname = "C:\\Program Files\\VideoLAN\\VLC\\vlc.exe";

	// Save the file name to a const char*
	const char* fileName = &episodePath[0];

	STARTUPINFOA startupInfo;
	PROCESS_INFORMATION processInfo;
	memset(&startupInfo, 0, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	memset(&processInfo, 0, sizeof(processInfo));

	char buf[MAX_PATH + 300];
	wsprintfA(buf, "%s \"%s\" --play-and-exit --fullscreen", appname, fileName);
	
	if (!CreateProcessA(NULL, buf, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo)) {
		std::cerr << "Failed to start VLC: " << GetLastError() << std::endl;
		return false;
	}

	// Wait for the VLC process to exit
	WaitForSingleObject(processInfo.hProcess, INFINITE);

	// Close process and thread handles
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);

	// Inform function call that we have returned from VLC player
	return true;
}

void RandomEpisode::updateRecentWatched(std::stack<std::string>& episodeStack) {
	std::ifstream brFile("EpisodesViewed.txt");
	if (!brFile) {
		// File doesn't exist, create it
		std::ofstream createFile("EpisodesViewed.txt");
		if (createFile) {
			std::cout << "Created EpisodesViewed.txt" << std::endl;
			createFile.close();
		}
		else {
			std::cerr << "Failed to create EpisodesViewed.txt" << std::endl;
		}
		// Reopen the file
		brFile.open("EpisodesViewed.txt");
	}
	

	int episodesViewed = totalEpisodesViewed();
	std::string line;
	std::vector<std::string> episodeVector;

	while (std::getline(brFile, line)) {
		episodeVector.push_back(line);
		episodesViewedHash[line] = true;
	}
	brFile.close();
	
	int maxEpisodesToStore = std::min(10, (int)episodeVector.size());

	for (int i = episodeVector.size() - maxEpisodesToStore; i < episodeVector.size(); i++) {
		episodeStack.push(episodeVector[i]);
	}
	
	int listSize = recentEpisodesList.size();
	if (listSize > 1) {
		for (int i = 0; i < listSize / 2; i++) {
			std::swap(recentEpisodesList[i], recentEpisodesList[listSize - 1 - i]);
		}
	}
}

int RandomEpisode::totalEpisodesViewed() {
	/*
		This section checks how many episodes have been viewed. This assists in parsing the files later to know when to end
	*/
	std::ifstream fileViewed("EpisodesViewed.txt");
	std::string lineViewed;

	int viewedCounter = 0;
	if (fileViewed.is_open()) {
		while (std::getline(fileViewed, lineViewed)) {
			viewedCounter++;
		}
		fileViewed.close();
	}
	return viewedCounter;
}

void RandomEpisode::writeToFile(std::string toWrite) {													// Called when needed to write the found episode
	std::ofstream brFile2;
	brFile2.open("EpisodesViewed.txt", std::ios_base::app);
	if (brFile2.is_open()) {
			brFile2 << toWrite << "\n";
	}
	brFile2.close();
}

bool RandomEpisode::findViewed(std::string search, int ctr) {
	int counter = 0;	

	std::ifstream episodeFile("EpisodesViewed.txt");								// Open EpisodesViewed file to check if episode's been viewed

	if (!episodeFile.is_open()) {
		std::cerr << "Unable to open EpisodesViewed.txt" << std::endl;
		return false;
	}

	// Check if file is empty
	if (episodeFile.peek() == std::ifstream::traits_type::eof()) {		
		foundEpisode = search;
		episodeFile.close();
		writeToFile(search);
		searching = false;													// Stop searching
		return false;														// Stops the loop of searching through files
	}

	std::string line = "";

	while (std::getline(episodeFile, line) && searching) {
		++counter;																// Counts how many lines have been viewed
		if (line.find(search, 0) != std::string::npos) {						// If there's a match, return true
			episodeFile.close();
			return true;
			break;
		}
		else if (counter >= (ctr)) {
			foundEpisode = search;
			episodeFile.close();
			writeToFile(search);
			searching = false;													// Stop searching
			return false;														// Stops the loop of searching through files
			break;
		}
	}

	episodeFile.close();
	return false;
}

bool RandomEpisode::hasEpisodeBeenViewed(static std::string& episode) {
	int episodeCounter = totalEpisodesViewed();
	searching = true;
	return findViewed(episode, episodeCounter);
}
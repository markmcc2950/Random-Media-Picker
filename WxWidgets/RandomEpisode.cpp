#include "RandomEpisode.h"

#include <time.h>
#include <fstream>
#include <windows.h>

#include "CipherHandler.h"

CipherHandler ch;
RandomEpisode re;

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
		return false;
	}

	// Wait for the VLC process to exit (Set to INFINITE to wait for the episode to end and VLC to close)
	WaitForSingleObject(processInfo.hProcess, INFINITE);

	// Close process and thread handles
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);

	// Inform function call that we have returned from VLC player
	return true;
}

// Grabs all previously viewed episodes
void RandomEpisode::retrieveAllViewed(std::stack<std::string>& episodeStack, std::vector<std::string>& episodeList, std::unordered_map<std::string, bool>& episodesViewedHash, int& filesToDisplay) {
	std::ifstream file("EpisodesViewed.txt");

	// If the file doesn't exist, create it and exit this function early as we don't have anything to pull
	if (!file) {
		std::ofstream createFile("EpisodesViewed.txt");
		if (createFile) {
			createFile.close();
		}

		return;
	}

	std::string line;

	// Iterate through each line in our file, push to our vector, and set our hash map to "viewed"
	while (std::getline(file, line)) {
		std::string decodedString = ch.affineDecode(line);
		episodeList.push_back(decodedString);
		episodesViewedHash[decodedString] = true;
	}

	file.close();

	updateRecentWatched(episodeStack, episodeList, filesToDisplay);
}

void RandomEpisode::updateRecentWatched(std::stack<std::string>& episodeStack, std::vector<std::string>& episodeList, int& filesToDisplay) {
	// Iterate only through the number of episodes viewed to prevent out of range index values
	int i = episodeList.size() < filesToDisplay ? 0 : episodeList.size() - filesToDisplay;
	for (i; i < episodeList.size(); i++) {
		episodeStack.push(episodeList[i]);
	}
}

// Add newly watched episodes to file and hash map
void RandomEpisode::storeRecentWatched(std::string toWrite, std::unordered_map<std::string, bool>& episodesViewedHash) {
	std::ofstream file;
	file.open("EpisodesViewed.txt", std::ios_base::app);
	
	if (file.is_open()) {		
		std::string encodedWrite = ch.affineEncode(toWrite);
		file << encodedWrite << "\n";
	}
	file.close();

	episodesViewedHash[toWrite] = true;
}
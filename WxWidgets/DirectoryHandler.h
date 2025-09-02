#ifndef DirectoryHandler_h
#define DirectoryHandler_h
#include <string>
#include "RandomEpisode.h"

class DirectoryHandler {
public:
	std::string getDirectory();
	void setDirectory(const std::string& newFolderPath);
	bool LoadPathFromFile(std::string& filePath);
	void writeToFile(std::string write);
	int getDirectoryFolderCount(const std::string& directoryPath);
	int getNumFilesInFolder(const std::string& directoryPath);
	std::string getFolderByIndex(const std::string& directoryPath, int index);
	std::string getFileByIndex(const std::string& directoryPath, const int& index);
	std::string normalizePath(const std::string& input, const std::string& target, const std::string& replacement);
	std::string formatFinalDirectory(const std::string& fullPath);
	bool findDirectoryPath(std::string& selectedDirectory, std::string& vlcPath, std::vector<std::string>& episodeList, int& filesToDisplay, int& randomValue, std::unordered_map<std::string, bool>& episodesViewed);
	std::string getRandomFolder(std::string selectedFolder, int folderIndex);
private:
	std::string folderPath = "";
	void saveBrowsePathToFile(const std::string& path);
	bool findRandomFile(int& randomValue, int& fileCount, std::string& selectedDirectory, std::string& vlcPath, bool& isValidFolder, std::unordered_map<std::string, bool>& episodesViewed);
	void findRandomFolder(int& randomValue, int& directoryCount, std::string& selectedDirectory);
};

extern DirectoryHandler dh;

#endif
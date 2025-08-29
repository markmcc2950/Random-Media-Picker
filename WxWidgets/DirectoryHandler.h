#ifndef DirectoryHandler_h
#define DirectoryHandler_h
#include <string>
#include "RandomEpisode.h"

class DirectoryHandler {
public:
	std::string getDirectory();
	void setDirectory(const std::string& newFolderPath);
	bool LoadPathFromFile(std::string& filePath);
	int getDirectoryFolderCount(const std::string& directoryPath);
	int getNumFilesInFolder(const std::string& directoryPath);
	std::string getFolderByIndex(const std::string& directoryPath, int index);
	std::string getFileByIndex(const std::string& directoryPath, int index);
	std::string formatFinalDirectory(const std::string& input, const std::string& target, const std::string& replacement, const bool isFinalFormat);
	bool findDirectoryPath(std::string& selectedDirectory, std::string& vlcPath, std::vector<std::string>& episodeList, int& filesToDisplay);
	std::string getRandomFolder(std::string selectedFolder, int folderIndex);
private:
	std::string folderPath = "";
	void saveBrowsePathToFile(const std::string& path);
};

extern DirectoryHandler dh;

#endif
#include "DirectoryHandler.h"

#include <windows.h>
#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>

DirectoryHandler dh;
extern RandomEpisode re;

std::string DirectoryHandler::getDirectory() {
	return folderPath;
}

void DirectoryHandler::setDirectory(const std::string& newfolderpath) {
	folderPath = newfolderpath;
    saveBrowsePathToFile(folderPath);
}

void DirectoryHandler::saveBrowsePathToFile(const std::string& path) {
    std::ofstream file("config.txt");
    if (file.is_open()) {
        file << path;
        file.close();
    }
    else {
        std::cerr << "Failed to save path." << std::endl;
    }
}

bool DirectoryHandler::LoadPathFromFile(std::string& filePath) {
    std::ifstream file("config.txt");
    std::string path;

    if (file.is_open()) {
        std::getline(file, path);  // Read the saved path
        file.close();
        filePath = path;
        folderPath = path;
        return true;
    }
    else {
        std::cerr << "No saved path found." << std::endl;
        return false;
    }

    return false;
}

int DirectoryHandler::getDirectoryFolderCount(const std::string& directoryPath) {
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    // Adjust the search path to include a wildcard to find all items in the directory
    std::string searchPath = directoryPath + "\\*";
    hFind = FindFirstFileA(searchPath.c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "FindFirstFile failed for path: " << directoryPath << " with error: " << GetLastError() << std::endl;
        return -1;
    }

    int folderCount = 0;

    do {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            std::string folderName = findFileData.cFileName;

            // Skip the "." and ".." directories
            if (folderName != "." && folderName != "..") {
                ++folderCount;
            }
        }
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);
    return folderCount;
}

int DirectoryHandler::getNumFilesInFolder(const std::string& directoryPath) {
    int fileCount = 0;

    try {
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
            if (std::filesystem::is_regular_file(entry.status())) {
                auto extension = entry.path().extension();
                if (
                    extension == ".mkv" || 
                    extension == ".wav" || 
                    extension == ".mp4" ||
                    extension == ".wmv" ||
                    extension == ".avi") {
                    ++fileCount;
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return fileCount;
}

std::string DirectoryHandler::getFolderByIndex(const std::string& directoryPath, int index) {
    int currentIndex = 0;

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.is_directory()) {
            if (currentIndex == index) {
                return entry.path().filename().string();
            }
            ++currentIndex;
        }
    }

    // Return an empty string if index is out of bounds
    return "";
}

std::string DirectoryHandler::getFileByIndex(const std::string& directoryPath, int index) {
    std::vector<std::string> files;

    try {
        // Iterate over the directory and collect file names
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
            if (entry.is_regular_file()) {
                files.push_back(entry.path().filename().string());
            }
        }

        // Check if the index is valid
        if (index >= files.size()) {
            throw std::out_of_range("Index is out of range.");
        }

        // Return the file name at the specified index
        return files[index];
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return "";                                                                  // Return an empty string if an error occurred
}

std::string DirectoryHandler::formatFinalDirectory(const std::string& input, const std::string& target, const std::string& replacement, const bool isFinalFormat) {
    std::string result = input;
    std::string::size_type pos = 0;
    std::string tempReplacement = replacement;

    int counter = 0;                                                            // Checks for replacing with a colon or a comma (only in final formatting)

    // Replace all instances of target with replacement
    while ((pos = result.find(target, pos)) != std::string::npos) {
        if (isFinalFormat) {
            counter == 0 ? tempReplacement = ": " : tempReplacement = ", ";

            ++counter;                                                          // Increment only in final formatting, tells us to replace with different characters
        }
        result.replace(pos, target.length(), tempReplacement);
        pos += tempReplacement.length();                                        // Move past the replacement
    }

    return result;
}

std::string DirectoryHandler::getRandomFolder(std::string selectedFolder, int folderIndex) {
    return dh.getFolderByIndex(selectedFolder, folderIndex);
}

bool DirectoryHandler::findDirectoryPath(std::string& selectedDirectory, std::string& vlcPath, std::vector<std::string>& episodeList, int& filesToDisplay) {
    bool isValidFolder = true;

    int directoryCount, fileCount, randomValue = -1;
    int loopCounter = 0;

    srand(time(0));

    while (isValidFolder) {
        directoryCount = dh.getDirectoryFolderCount(selectedDirectory);
        if (directoryCount > 0) {
            randomValue = rand() % directoryCount;

            std::string nextFolder = dh.getRandomFolder(selectedDirectory, randomValue);			// Get name of randomly chosen folder
            selectedDirectory += "//" + nextFolder;
        }
        else {																						// If we return no more folders
            fileCount = dh.getNumFilesInFolder(selectedDirectory);
            if (fileCount > 0) {
                randomValue = rand() % fileCount;
                vlcPath = selectedDirectory;
                selectedDirectory += "//Episode " + std::to_string(randomValue + 1);

                // Format our string prior to checking with the written file
                selectedDirectory = dh.formatFinalDirectory(selectedDirectory, "//", "\\", false);
                selectedDirectory = dh.formatFinalDirectory(selectedDirectory, dh.getDirectory() + "\\", "", false);
                selectedDirectory = dh.formatFinalDirectory(selectedDirectory, "\\", "", true);

                // Returns false if we HAVEN'T watched it yet
                if (!re.hasEpisodeBeenViewed(selectedDirectory)) {
                    return true;
                    isValidFolder = false;
                }
                // If we have watched it, reset and continue searching
                else {
                    isValidFolder = true;
                    selectedDirectory = dh.getDirectory();
                    loopCounter = -1;
                }
            }
            // We selected an empty folder, reset search
            else {
                isValidFolder = true;
                selectedDirectory = dh.getDirectory();
                loopCounter = 0;
            }
        }

        ++loopCounter;
        // Corner case: If we're too many folders deep, reset the search loop, reduce the loopCounter by half to not fully reset, but try not to get stuck in infinite loops
        if (loopCounter >= 10 && isValidFolder) {
            isValidFolder = true;
            selectedDirectory = dh.getDirectory();
            loopCounter = loopCounter / 2;
        }

        ++loopCounter;

        // Terminate the search if we've gone for too long
        if (loopCounter >= 10000) {
            return false;
        }
    }
    return false;
}
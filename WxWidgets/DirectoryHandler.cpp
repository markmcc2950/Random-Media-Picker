#include "DirectoryHandler.h"

#include <windows.h>
#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

extern RandomEpisode re;
DirectoryHandler dh;

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

bool DirectoryHandler::isValidPath(std::string& path) {
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
        return true;
    }
    return false;
}

bool DirectoryHandler::LoadPathFromFile(std::string& filePath) {
    std::ifstream file("config.txt");
    if (!file) {
        std::ofstream createFile("config.txt");
        if (createFile) {
            createFile.close();
        }

        return false;
    }

    std::string path;

    if (file.is_open()) {
        std::getline(file, path);  // Read the saved path
        file.close();
        if (path.length() > 0 && isValidPath(path)) {
            filePath = path;
            folderPath = path;
            return true;
        }
        return false;
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
                // Verify extensions match our compatible video formats
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

std::string DirectoryHandler::getFileByIndex(const std::string& directoryPath, const int& index) {
    std::vector<std::string> files;

    try {
        // Iterate over the directory and collect file names
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
            auto extension = entry.path().extension();
            if (entry.is_regular_file() && 
                (extension == ".mkv" ||
                extension == ".wav" ||
                extension == ".mp4" ||
                extension == ".wmv" ||
                extension == ".avi")) {
                files.push_back(entry.path().filename().string());              
            }
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

std::string DirectoryHandler::normalizePath(const std::string& input, const std::string& target, const std::string& replacement) {
    std::string result = input;
    size_t pos = 0;
    while ((pos = result.find(target, pos)) != std::string::npos) {
        result.replace(pos, target.length(), replacement);
        pos += replacement.length();
    }
    return result;
}

std::string DirectoryHandler::formatFinalDirectory(const std::string& fullPath) {
    namespace fs = std::filesystem;

    fs::path p(fullPath);
    std::vector<std::string> parts;

    // Get file name (episode file)
    std::string filename = p.filename().string();
    if (!filename.empty()) {
        // Strip extension
        size_t dotPos = filename.find_last_of('.');
        if (dotPos != std::string::npos) {
            filename = filename.substr(0, dotPos);
        }

        // Extract leading number if present
        std::istringstream iss(filename);
        int epNumber;
        if (iss >> epNumber) {
            filename = "Episode " + std::to_string(epNumber);
        }
    }

    // Push back season and show folder
    if (p.has_parent_path()) {
        parts.insert(parts.begin(), p.parent_path().filename().string()); // Season 01
        if (p.parent_path().has_parent_path()) {
            parts.insert(parts.begin(), p.parent_path().parent_path().filename().string()); // Friends
        }
    }

    // Add filename (episode)
    if (!filename.empty()) {
        parts.push_back(filename);
    }

    // Now format: first colon, rest commas
    if (parts.empty()) return "";

    std::string result = parts[0];
    for (int i = 1; i < parts.size(); i++) {
        result += (i == 1 ? ": " : ", ") + parts[i];
    }

    return result;
}

std::string DirectoryHandler::getRandomFolder(std::string selectedFolder, int folderIndex) {
    return getFolderByIndex(selectedFolder, folderIndex);
}

void DirectoryHandler::findRandomFolder(int& randomValue, int& directoryCount, std::string& selectedDirectory) {
    randomValue = rand() % directoryCount;

    std::string nextFolder = getRandomFolder(selectedDirectory, randomValue);			// Get name of randomly chosen folder
    selectedDirectory += "//" + nextFolder;
}

bool DirectoryHandler::findRandomFile(int& randomValue, int& fileCount, std::string& selectedDirectory, std::string& vlcPath, bool& isValidFolder, std::unordered_map<std::string, bool>& episodesViewedHash) {
    randomValue = rand() % fileCount;
    vlcPath = selectedDirectory;

    // Add episode marker
    selectedDirectory += "\\Episode " + std::to_string(randomValue + 1);

    // Convert to the preferred format (Series: Season ##, Episode ##)
    selectedDirectory = formatFinalDirectory(selectedDirectory);

    // Use the formatted string for history tracking; Returns FALSE is episode hasn't been viewed yet
    if (!episodesViewedHash[selectedDirectory]) {
        isValidFolder = false;
        return true;
    }
    else {
        return false;
    }
}

bool DirectoryHandler::findDirectoryPath(std::string& selectedDirectory, std::string& vlcPath, std::vector<std::string>& episodeList, int& filesToDisplay, int& randomValue, std::unordered_map<std::string, bool>& episodesViewedHash) {
    bool isValidFolder = true;
    std::string originalDirectory = selectedDirectory;

    int directoryCount, fileCount = -1;
    int loopCounter = 0;

    srand(time(0));

    while (isValidFolder) {
        directoryCount = getDirectoryFolderCount(selectedDirectory);
        fileCount = getNumFilesInFolder(selectedDirectory);

        // If we have both files and folders in this directory, choose at random to choose a file or another folder to parse through
        if (directoryCount > 0 && fileCount > 0) {
            int tempRand = rand() % 2;

            // If 0, search through folders. If 1, search through files
            if (tempRand == 0) {
                findRandomFolder(randomValue, directoryCount, selectedDirectory);
            }
            else {
                if (findRandomFile(randomValue, fileCount, selectedDirectory, vlcPath, isValidFolder, episodesViewedHash)) {
                    return true;
                }
                // If we have watched it, reset and continue searching
                else {
                    selectedDirectory = getDirectory();
                    loopCounter = -1;
                }
            }
        }
        // If we have only folders in this directory
        else if (directoryCount > 0 && fileCount == 0) {
            findRandomFolder(randomValue, directoryCount, selectedDirectory);
        }
        // If we have only files within this directory
        else {
            if (directoryCount == 0 && fileCount > 0) {
                if (findRandomFile(randomValue, fileCount, selectedDirectory, vlcPath, isValidFolder, episodesViewedHash)) {
                    return true;
                }
                // If we have watched it, reset and continue searching
                else {
                    //selectedDirectory = getDirectory();
                    selectedDirectory = originalDirectory;
                }
            }
            // We selected an empty folder, reset search
            else {
                //selectedDirectory = getDirectory();
                selectedDirectory = originalDirectory;
            }
        }

        ++loopCounter;
        // Corner case: If we're too many folders deep, reset the search loop, try not to get stuck in infinite loops
        if (loopCounter >= 10 && isValidFolder) {
            //selectedDirectory = getDirectory();
            selectedDirectory = originalDirectory;
        }

        // Terminate the search if we've gone for too long
        if (loopCounter >= 100000) {
            return false;
        }
    }
    return false;
}
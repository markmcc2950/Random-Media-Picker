#pragma once
#ifndef RandomEpisode_h
#define RandomEpisode_h
#include <stack>
#include <string>
#include <tchar.h>
#include <unordered_map>
#include <vector>

class RandomEpisode {
public:
	// Public functions
	std::string tcharToString(TCHAR toConvert[]);
	std::string showRecentEpisodes(int i);

	bool openFile(std::string episodePath);
	void retrieveAllViewed(std::vector<std::string>& recentWatched, std::unordered_map<std::string, bool>& episodesViewedHash, int& filesToDisplay);
	void storeRecentWatched(std::string write, std::unordered_map<std::string, bool>& episodesViewedHash);
private:
	// Private variables
	bool searching = true;

	int counter = 0;
	int totalEpisodes = 0;

	std::vector<std::string> recentEpisodesList;

	std::string foundEpisode = "";
	std::string seriesName = "";
	std::string seasonName = "";
	std::string episodeName = "";													// Name of the episode
	std::string episodeNum = "";													// Number of the episode (Example: 05 TITLE)
};

#endif

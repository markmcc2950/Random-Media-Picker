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
	std::string tcharToString(TCHAR toConvert[]);
	std::string showRecentEpisodes(int i);

	bool openFile(std::string episodePath);
	void updateRecentWatched(std::stack<std::string>& episodeStack);
	int totalEpisodesViewed();
	void writeToFile(std::string write);
	bool findViewed(std::string search, int ctr);

	bool hasEpisodeBeenViewed(static std::string& episode);
private:
	bool searching = true;
	int counter = 0;
	int totalEpisodes = 0;
	//std::string recentEpisodesList[10];
	std::vector<std::string> recentEpisodesList;
	std::string foundEpisode = "";
	std::string seriesName = "";
	std::string seasonName = "";
	std::string episodeName = "";													// Name of the episode
	std::string episodeNum = "";													// Number of the episode (Example: 05 TITLE)

	std::unordered_map<std::string, bool> episodesViewedHash;
};

#endif

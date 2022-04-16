#include "SearchEngine.h"

#include <algorithm>

Search::SearchEngine::SearchEngine()
{   
    synonymDict_ = new Search::Synonyms();
    pageIndex_ = new Search::PageIndex();

    std::cout << "Num indexed words: " << pageIndex_->numElements() << "\n";
}

Search::SearchEngine::~SearchEngine()
{
    delete synonymDict_;
    delete pageIndex_;
}

std::vector<std::pair<std::string, unsigned int>> Search::SearchEngine::search(std::string query) const
{
    std::cout << "Search::SearchEngine::search(): query=\"" << query << "\"\n";

    // Parse words in query
    std::vector<std::string> words = getWords(query);

    // Generate keywords, adding synonyms of words included in query
    Search::SearchKeywords keywords = formKeywords(words);

    // Search page index for results
    std::vector<std::pair<std::string, unsigned int>> results = searchPageIndex(keywords);

    return results;
}

std::vector<std::pair<std::string, unsigned int>> Search::SearchEngine::searchPageIndex(const Search::SearchKeywords& keywords) const {
    // Map tracking how many keyword matches each site has
    std::map<std::string, unsigned int> resultsRanking;

    // Loop all keywords, for now only searching original word (first in synonym list)
    for(unsigned int keywordIdx = 0; keywordIdx < keywords.size(); keywordIdx++)
    {
        if(keywords[keywordIdx].size() > 0)
        {   
            // Query page index for sites that contain word
            std::vector<Search::Site> sites = pageIndex_->find(keywords[keywordIdx][0]);

            // Loop every site and add/update occurances counter
            for(unsigned int siteIdx = 0; siteIdx < sites.size(); siteIdx++)
            {
                Search::Site site = sites[siteIdx];

                // Add occurances count to map
                if(resultsRanking.find(site.sitelink) != resultsRanking.end())
                {
                    resultsRanking[site.sitelink] += site.occurances;
                }
                // Insert site into map
                else
                {
                    std::pair<std::string, unsigned int> pair = std::make_pair(site.sitelink, site.occurances);
                    resultsRanking.insert(pair);
                }
            }
        }
    }

    // Convert map to vector for sorting
    std::vector<std::pair<std::string, unsigned int>> resultsList = {};

    for(auto& it : resultsRanking)
    {
        resultsList.push_back(it);
    }

    // Sort map by occurance count, use lambda, because modern stuff
    std::sort(resultsList.begin(), resultsList.end(),
        [](const std::pair<std::string, unsigned int>& a, 
           const std::pair<std::string, unsigned int>& b) -> bool
    { 
        return a.second > b.second;
    });

    auto end = std::min(resultsList.begin() + 10, resultsList.end());
    std::vector<std::pair<std::string, unsigned int>> top(resultsList.begin(), end);

    return top;
}


Search::SearchKeywords Search::SearchEngine::formKeywords(const std::vector<std::string> words) const
{
    Search::SearchKeywords keywords = {};

    for(auto it = words.begin(); it != words.end(); ++it)
    {
        Search::SynonymList list = {};

        bool found = synonymDict_->find(*it, list);

        if(found)
        {
            list.insert(list.begin(), *it);

            keywords.push_back(list);
        }
    }

    return keywords;
}

// Assumptions: The string has no leading/trailing whitespace, 
//              and all words are seperated by one space
std::vector<std::string> Search::SearchEngine::getWords(std::string& query, std::string delimiter) const
{
    std::vector<std::string> words = {};
    std::size_t start;
    std::size_t end;
    std::size_t len;
    std::string s;

    // Find first character
    start = query.find_first_not_of(delimiter);

    while(start != std::string::npos)
    {
        // Find next delimiter
        end = query.find_first_of(delimiter, start);
        
        // Calculate string length
        len = end - start;

        // Find substring
        s = query.substr(start, len);

        // Check duplicate before inserting??
        auto it = std::find(words.begin(), words.end(), s);
        if(it == words.end() && s.size() > 0)
        {
            words.push_back(s);
        }

        start = query.find_first_not_of(delimiter, end);
    }

    return words;
}
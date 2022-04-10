#include "SearchEngine.h"
#include "Synonyms.h"

#include <algorithm>

Search::SearchEngine::SearchEngine()
{   
    std::cout << "Search::Engine::Engine()\n";
    config_ = new Search::Config();
}

Search::SearchEngine::~SearchEngine()
{
    std::cout << "Search::Engine::~Engine()\n";
    delete config_;
}

std::vector<Search::SearchResult> Search::SearchEngine::search(std::string query) const
{
    std::cout << "Search::SearchEngine::search(): query=\"" << query << "\"\n";

    // Parse words in query
    std::vector<std::string> words = getWords(query);

    // Generate keywords, adding synonyms of words included in query
    Search::Keywords keywords = formKeywords(words);

    // 

    std::vector<Search::SearchResult> results = {};
    return results;
}

Search::Keywords Search::SearchEngine::formKeywords(const std::vector<std::string> words) const
{
    Search::Keywords keywords = {};

    Utils::Synonyms* synonymDictionary = config_->getSynonymDict();

    for(auto it = words.begin(); it != words.end(); ++it)
    {
        Utils::SynonymList list = {};

        bool found = synonymDictionary->find(*it, list);

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
#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include "Synonyms.h"
#include "Filter.h"
#include "PageIndex.h"

namespace Search 
{
    typedef std::vector<std::vector<std::string>> SearchKeywords;

    struct SearchResult
    {
        std::string headline;
        std::string sitelink;

        bool operator==(const SearchResult& result) const {
            return sitelink == result.sitelink;
        }

        bool operator<(const SearchResult& result) const {
            return sitelink.compare(result.sitelink);
        }
    };

    class SearchEngine
    {
        public:
            // Constructor
            SearchEngine();
            // Destructor
            ~SearchEngine();

            // Process search queries and return results
            std::vector<std::pair<Search::SearchResult, unsigned int>> search(std::string query) const;

            std::vector<std::pair<Search::SearchResult, unsigned int>> searchPageIndex(const Search::SearchKeywords& keywords) const;

            // Process query and return list of all words contained
            std::vector<std::string> getWords(std::string& query,
                                              std::string delimiter = " ") const;

            Search::SearchKeywords formKeywords(const std::vector<std::string> words) const;

        private:
            Search::Synonyms* synonymDict_;
            Search::Filter* filter_;
            Search::PageIndex* pageIndex_;
    };
};

#endif
#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include "Config.h"

namespace Search 
{
    typedef std::vector<std::vector<std::string>> Keywords;

    struct SearchResult
    {
        std::string headline;
        std::string date;
        std::string preview;
    };

    class SearchEngine
    {
        public:
            // Constructor
            SearchEngine();
            // Destructor
            ~SearchEngine();

            // Process search queries and return results
            std::vector<SearchResult> search(std::string query) const;

            // Process query and return list of all words contained
            std::vector<std::string> getWords(std::string& query,
                                              std::string delimiter = " ") const;

            Keywords formKeywords(const std::vector<std::string> words) const;

        private:
            Config* config_;
    };
};

#endif
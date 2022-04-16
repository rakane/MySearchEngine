#ifndef PAGEINDEX_H
#define PAGEINDEX_H

#include "CSVReader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>



namespace Search 
{
    struct Site {
        std::string sitelink;
        std::string headline;
        unsigned int occurances;
    };

    typedef std::map<std::string, std::vector<Search::Site>> Index;

    class PageIndex
    {
        public:
            // Constructor
            PageIndex();
            // Destructor
            ~PageIndex();

            std::vector<Search::Site> find(const std::string& word) const;
            unsigned int numElements() const;

        private:

            void processKeyword(const std::string& row);
            Search::Site processSite(const std::string& site);

            Index* index_;
            Utils::CSVReader* csvReader_;
    };
};

#endif
#ifndef SEARCH_H
#define SEARCH_H

#include "SearchEngine.h"

namespace Search
{
    class Search
    {
        public:
            Search();
            ~Search();
            int start();

        private:
            void formatInput(std::string& s, 
                             std::string whitespace = " \t");

            void trim(std::string& s, 
                           std::string whitespace = " \t");

            void reduce(std::string& s, 
                           std::string whitespace = " \t",
                           std::string replace = " ");

            SearchEngine* searchEngine_;
    };
};

#endif
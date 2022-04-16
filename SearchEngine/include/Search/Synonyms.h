#ifndef SYNONYMS_H
#define SYNONYMS_H

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "CSVReader.h"

namespace Search 
{
    typedef std::vector<std::string> SynonymList;
    typedef std::map<std::string, SynonymList> SynonymDict;

    class Synonyms 
    {
        public:
            // Default constructor
            Synonyms();
            
            // Destructor
            ~Synonyms();

            // Method: find
            //
            // Inputs: word - input word to find synonyms of
            //
            // Outputs: list - list of all synonyms=
            bool find(std::string word, SynonymList& list) const;
            
        private:
            void processSynonym(const std::string&) const;
            std::size_t numSynonyms() const;

            SynonymDict* dictionary_;
            Utils::CSVReader* csvReader_;
    };
};

#endif
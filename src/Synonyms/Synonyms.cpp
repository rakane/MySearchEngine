#include "Synonyms.h"
#include "CSVReader.h"
#include <time.h>
#include <unistd.h>
#include <algorithm>

Utils::Synonyms::Synonyms() 
{
    std::cout << "Utils::Synonyms::Synonyms()\n";
    csvReader_ = new Utils::CSVReader();

    // Load synonyms
    dictionary_ = new SynonymDict();
   
    int isOpen = csvReader_->open("../data/synonyms.csv");

    if(isOpen)
    {
        std::string data = "";

        // Skip first row
        int success(csvReader_->getRow(data));

        if(success)
        {
            while(csvReader_->getRow(data))
            {
                processSynonym(data);
                //std::cout << data << "\n";
            }
        }
    }
}

Utils::Synonyms::~Synonyms()
{
    std::cout << "Utils::Synonyms::~Synonyms()\n";
    delete dictionary_;
    delete csvReader_;
}

bool Utils::Synonyms::find(std::string word, Utils::SynonymList& synonyms) const
{
    bool found(false);

    auto it = dictionary_->find(word);
    if(it != dictionary_->end())
    {               
        synonyms = it->second;
        found = true;
    }

    return found;
}

void Utils::Synonyms::processSynonym(const std::string& row) const
{
    // Synonym row format: word, type, synonym1;synonym2;...synonym10
    if(!row.empty())
    {
        // Store new word and synoynm list
        std::string word = "";
        std::vector<std::string> synonyms = { };

        std::string temp = row;

        // Find index of first break
        unsigned int wordEndingIdx = temp.find_first_of(',', 0);
        if(wordEndingIdx > 0 && wordEndingIdx < row.size())
        {
            word = temp.substr(0, wordEndingIdx);
        }

        std::size_t len;

        // Get index of first synonym, which starts after the last comma
        std::size_t startIdx = temp.find_last_of(',') + 1;
        
        // Find the next delimiter from start of synonym
        std::size_t delimiterIdx = std::min(
            temp.find_first_of(';', startIdx),
            temp.find_first_of('|', startIdx));

        bool exitLoop = false;
        while(startIdx < temp.size() && startIdx != std::string::npos && !exitLoop)
        {
            // Last synonym, set length and exitLoop flag to true
            if(delimiterIdx == std::string::npos)
            {
                exitLoop = true;
            }

            // Get new synonym
            len = std::min(delimiterIdx, temp.size()) - startIdx;
            std::string newSynonym = temp.substr(startIdx, len);

            // Check if duplicate before inserting
            auto it = std::find(synonyms.begin(), synonyms.end(), newSynonym);
            if(it == synonyms.end() && newSynonym.size() > 1)
            {
                synonyms.push_back(newSynonym);
            }

            // Set next synonym start at the delimiterIdx + 1
            startIdx = delimiterIdx + 1;

            // Find the next delimiter from synonym start
            delimiterIdx = std::min(
                temp.find_first_of(';', startIdx + 1),
                temp.find_first_of('|', startIdx + 1));
        }

        // Insert into dictionary, check for duplicate
        std::pair<std::string, std::vector<std::string>> pair = std::make_pair(word, synonyms);
        if(dictionary_->find(word) == dictionary_->end())
        {
            dictionary_->insert(pair);
        }
    }
}

std::size_t Utils::Synonyms::numSynonyms() const
{
    return dictionary_->size();
}
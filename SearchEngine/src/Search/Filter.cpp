#include "Filter.h"

#include <time.h>
#include <unistd.h>
#include <algorithm>

Search::Filter::Filter() 
{
    csvReader_ = new Utils::CSVReader();

    // Load Filter
    words_ = new std::vector<std::string>();
   
    int isOpen = csvReader_->open("../data/filter.csv");

    if(isOpen)
    {
        std::string data = "";

        // Skip first row
        int success(csvReader_->getRow(data));

        if(success)
        {
            while(csvReader_->getRow(data))
            {
                words_->push_back(data);
            }
        }
    }
}

bool Search::Filter::includes(std::string word) const 
{
    auto it = std::find(words_->begin(), words_->end(), word);

    return it != words_->end();
}

Search::Filter::~Filter()
{
    delete words_;
    delete csvReader_;
}
#include "PageIndex.h"

#include <unistd.h>

Search::PageIndex::PageIndex()
{   
    csvReader_ = new Utils::CSVReader();
    index_ = new Search::Index();

    // Load index from csv file
    int isOpen = csvReader_->open("../data/index.csv");

    if(isOpen)
    {
        std::string data = "";

        // Skip first row
        int success(csvReader_->getRow(data));

        if(success)
        {
            while(csvReader_->getRow(data))
            {
                processKeyword(data);
            }
        }
    }
}

Search::PageIndex::~PageIndex()
{
    delete csvReader_;
}

std::vector<Search::Site> Search::PageIndex::find(const std::string& word) const
{
    std::vector<Search::Site> sites = {};

    auto it = index_->find(word);
    if(it != index_->end())
    {
        sites = it->second;
    }

    return sites;
}

unsigned int Search::PageIndex::numElements() const
{
    return index_->size();
}

void Search::PageIndex::processKeyword(const std::string& row)
{
    try {
        // Keyword row format: word,"sitelink1"-"headline1"-"ocurrances1";"sitelink2"-"headline2"-"ocurrances2"
        if(row.size() > 3 && row.find_first_of(',', 0) != std::string::npos)
        {
            // Store new word and list of sites
            std::string word = "";
            std::vector<Search::Site> sites = { };

            // Get word between start and first comma
            std::size_t wordEnd = row.find_first_of(',', 0);
            if(wordEnd > 0 && wordEnd != std::string::npos)
            {
                word = row.substr(0, wordEnd);
            }

            bool exitLoop = false;
            std::string temp;

            // Get first site, ending at first semicolon or end of line
            std::size_t start = wordEnd + 1;
            std::size_t end = row.find_first_of(';');

            while(start < row.size() && start != std::string::npos && !exitLoop) {
                // Last site, set exitLoop flag to true
                if(end == std::string::npos)
                {
                    exitLoop = true;
                }

                std::size_t len = std::min(row.size(), end) - start;

                try {
                    temp = row.substr(start, len);
                } catch(...) {
                    std::cout << row << "\n";
                    std::cout << start << " " << end << " " << len << " " << row.size() << "\n";
                    char c = 'c';
                    const char* ptr = &c;
                    throw std::out_of_range(ptr);
                }
                
                sites.push_back(processSite(temp));

                start = end + 1;
                end = row.find_first_of(';', start);
            }

            // Insert into index, check for duplicate
            std::pair<std::string, std::vector<Search::Site>> pair = std::make_pair(word, sites);
            if(index_->find(word) == index_->end())
            {
                index_->insert(pair);
            }
        }
    } catch(...) {
        // Error occured, don't add to list
    }
}

Search::Site Search::PageIndex::processSite(const std::string& serializedSite)
{
    std::string delimiter = "`-`";

    Search::Site site;

    try {
        size_t linkStart = serializedSite.find_first_not_of("`");
        size_t linkEnd = serializedSite.find(delimiter, linkStart);
        std::string sitelink = serializedSite.substr(linkStart, linkEnd - linkStart);
        site.sitelink = sitelink;

        size_t headlineStart = serializedSite.find_first_not_of(delimiter, linkEnd);
        size_t headlineEnd = serializedSite.find(delimiter, headlineStart);
        std::string headline = serializedSite.substr(headlineStart, headlineEnd - headlineStart);
        site.headline = headline;

        size_t occurancesStart = serializedSite.find_first_not_of(delimiter, headlineEnd);
        size_t occurancesEnd = serializedSite.find_last_of("`", occurancesStart, 1);

        std::string occurancesStr = serializedSite.substr(occurancesStart, occurancesEnd - occurancesStart);
        site.occurances = std::stoi(occurancesStr);

        return site;
    } catch(...) {
        //std::cout << serializedSite << "\n";
        char c = 'c';
        throw std::out_of_range(&c);
    }
}

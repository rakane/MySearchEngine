#include "Search.h"
#include "SearchEngine.h"

Search::Search::Search()
{
    searchEngine_ = new SearchEngine();
}

Search::Search::~Search()
{
    delete searchEngine_;
}



int Search::Search::start()
{
    std::string input = "";
    std::vector<std::pair<std::string, unsigned int>> results = {};
    
    while(input != "q")
    {
        input.clear();
        std::cout << "Enter search: ";
        getline(std::cin, input);

        if(input.size() > 0)
        {
            // Format input
            formatInput(input);

            // Search
            results = searchEngine_->search(input);

            if(!results.empty())
            {
                std::cout << "Results:\n";
                for(auto it = results.begin(); it != results.end(); ++it)
                {
                    std::cout << it->first << ": " << it->second << "\n";
                }
                std::cout << "\n";
            }
            else {
                std::cout << "No results found, please try again!\n\n";
            }
        }
    }

    return 0;
}

void Search::Search::formatInput(std::string& s,
                                 std::string whitespace)
{
    trim(s, whitespace);
    reduce(s, whitespace);
}

void Search::Search::trim(std::string& s,
                          std::string whitespace)
{
    // Remove leading and trailing spaces
    std::size_t start = s.find_first_not_of(whitespace);
    
    if(start != std::string::npos)
    {            
        std::size_t end = s.find_last_not_of(whitespace);
        std::size_t len = end - start + 1;

        s = s.substr(start, len);
    }
    // No content
    else
    {
        s = "";
    }
}

void Search::Search::reduce(std::string& s,
                            std::string whitespace,
                            std::string replace)
{
    std::size_t start = s.find_first_of(whitespace);

    while(start != std::string::npos)
    {
        std::size_t end = s.find_first_not_of(whitespace, start);
        std::size_t len = end - start;

        // Replace extra spaces
        s.replace(start, len, replace);

        // Search for next space
        start = s.find_first_of(whitespace, start + replace.size());
    }
}

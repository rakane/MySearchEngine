#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <map>

#include "Config.h"
#include "Synonyms.h"

Search::Config::Config()
{
    std::cout << "Search::Config::Config()\n";
    synonymDict_ = new Utils::Synonyms();
}

Search::Config::~Config()
{
    std::cout << "Search::Config::~Config()\n";
    delete synonymDict_;
}

Utils::Synonyms* Search::Config::getSynonymDict()
{
    return synonymDict_;
}
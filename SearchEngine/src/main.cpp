#include <stdio.h>
#include <iostream>

#include "Search.h"

int main(int argc, char** argv) 
{
    Search::Search* search = new Search::Search();

    // Start search
    int status = search->start();

    delete search;

    return status;
}
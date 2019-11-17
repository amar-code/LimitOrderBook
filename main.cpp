#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include "error_monitor.h"
#include "feed_handler.h"

int main(int argc, char **argv)
{
    if( argc <2 ){
	std::cout<<" Please Use it like this: ./OrderBook <Input_File>"<<std::endl;
	return -1;
    }
    CS::FeedHandler feed;
    std::string line;
    const std::string filename(argv[1]);
    std::ifstream infile(filename.c_str(), std::ios::in);

    decltype(std::chrono::high_resolution_clock::now()) start;
    decltype(start) end;
    uint64_t ticks = 0;
    int counter = 0;

    while (std::getline(infile, line)) {
        start = std::chrono::high_resolution_clock::now();
        feed.ProcessMessage(line);
        end = std::chrono::high_resolution_clock::now();

        ++counter;
        ticks += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    }



    feed.PrintCurrentOrderBook(std::cout);
    std::cout << "Total time Taken : " << ticks << " per msg:" << ticks/counter << " nanoseconds." << std::endl;
    return 0;
}

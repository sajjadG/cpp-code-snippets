#include <boost/regex.hpp>
#include <vector>
#include <string>
#include <iostream>

#include <archive.h>
#include <archive_entry.h>

#include "archive-manager.hh"

int main(int argc, char** argv)
{
	if(argc<2){
	std::cout<<"few arguments"<<std::endl;	
		return 1;
	}
	//extractFile(argv[1], argv[2]);
	list_archive(argv [1]);
	extract(argv[1]);
	std::cout<<"Done."<<std::endl;
}

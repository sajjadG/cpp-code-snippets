#include <boost/asio.hpp>
#include <iostream>

#include "url2ip.hh"

int main(int argc, char** argv)
{
	if(argc<2){
		std::cout<<"ABORT:few argument.\nusage: ./url2IP www.google.com\n";
		return 1;
	}
	
	Url2IP u2i(argv[1]);
	std::cout << "IP addresses: \n";
	for(int i=0; i<u2i.values.size(); i++){
		std::cout << u2i.values.at(i)<< "\t";
	}
	std::cout << '\n';
    
    return 0;
}

#include <boost/regex.hpp>
#include <vector>
#include <string>
#include <iostream>

#include "urlparser.hh"

int main(int argc, char** argv)
{
	UrlParser u(argv[1]);
	std::cout<<"protocol="<<u.protocol<<std::endl
		<<"host="<<u.host<<std::endl
		<<"Port="<<u.port<<std::endl
		<<"Path="<<u.path<<std::endl
		<<"File="<<u.file<<std::endl
		<<"Parameters="<<u.parameters<<std::endl;
    
    return 0;
}

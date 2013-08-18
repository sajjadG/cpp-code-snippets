///This class is a header only class for getting the corresponding IP of a url.
#include <boost/regex.hpp>
#include <vector>
#include <string>

class Url2IP{
	public:
		Url2IP(std::string url){
			boost::asio::io_service io_service;
			boost::asio::ip::tcp::resolver resolver(io_service);
			boost::asio::ip::tcp::resolver::query query(url, "");
			for(boost::asio::ip::tcp::resolver::iterator i = resolver.resolve(query);
		                        i != boost::asio::ip::tcp::resolver::iterator();
		                        ++i)
			{
				boost::asio::ip::tcp::endpoint end = *i;
				values.push_back(end.address().to_string());
			}
		}//constructor
		
		~Url2IP(){};
		
		//variables
		std::vector<std::string> values;
};

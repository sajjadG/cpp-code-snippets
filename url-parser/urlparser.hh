///This is a header only, small class that gets a  string as url and parse it to "Protocol", "Host", "Port", "Path", "File", "Parameters".
#include <boost/regex.hpp>
#include <vector>
#include <string>

///This class gets a url string and parse it to "Protocol", "Host", "Port", "Path", "File", "Parameters".
class UrlParser{
	public:
		UrlParser(std::string url){
			std::vector<std::string> values;
			boost::regex expression(
			//       proto                 host               port
					"^(\?:([^:/\?#]+)://)\?(\\w+[^/\?#:]*)(\?::(\\d+))\?"
			//       path                  file       parameters
					"(/\?(\?:[^\?#/]*/)*)\?([^\?#]*)\?(\\\?(.*))\?"
						           );
			
			if (boost::regex_split(std::back_inserter(values), url, expression)){
			/*
				const char* names[] = {"Protocol", "Host", "Port", "Path", "File", "Parameters", NULL};
				for (int i = 0; names[i]; i++)
					printf("%s: %s\n", names[i], values[i].c_str());
			}*/
				protocol = values[0].c_str();
				host = values[1].c_str();
				port = values[2].c_str();
				path = values[3];//.c_str();
				file = values[4].c_str();
				parameters = values[5];
			}else{//Fatal error
			}
		}//constructor
		
		~UrlParser(){};
		
		//variables
		std::string protocol;
		std::string host;
		std::string port;
		std::string path;
		std::string file;
		std::string parameters;
};

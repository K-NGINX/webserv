#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#define BUFFER_SIZE 1024

#define RESET "\033[0m"
#define GRAY "\033[1;90m"
#define RED "\033[1;91m"
#define GREEN "\033[1;92m"
#define YELLOW "\033[1;93m"
#define BLUE "\033[1;94m"
#define MAGENTA "\033[1;95m"
#define CYAN "\033[1;96m"

class Utils {
   public:
	static void trimComment(std::string& str);
	static void trimWhiteSpace(std::string& str);
	static int stoi(std::string str);
	static std::string getMIMEType(const std::string& file_type);
	static bool checkMIMEType(const std::string& mime_type);
	static int findSubVector(std::vector<char>& v, std::vector<char>& obj);
	
	static std::string whitespace;
	static std::map<std::string, std::string> m_mime;

   private:
	Utils();
	~Utils();
};

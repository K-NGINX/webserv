#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../Utils/Utils.hpp"

class CommonDirectives {
   public:
	CommonDirectives();
	CommonDirectives& operator=(const CommonDirectives& other);

	/* getter */
	bool isAutoindex() const;
	int getClientMaxBodySize() const;
	const std::vector<std::string>& getErrorCodeVec() const;
	const std::string& getErrorPage() const;
	const std::vector<std::string>& getIndexVec() const;
	const std::string& getRoot() const;
	/* setter */
	void setAutoindex(std::string& value);
	void setClientMaxBodySize(std::string& value);
	void setErrorPage(std::string& value);
	void setIndex(std::string& value);
	void setRoot(std::string& value);

	void refine(std::map<std::string, std::string>& m_directives);
	void print(const std::string& indent) const;
	bool isErrorCode(const std::string& error_code) const;

   private:
	bool is_autoindex_;
	int client_max_body_size_;
	std::vector<std::string> v_error_code_;
	std::string error_page_;
	std::vector<std::string> v_index_;
	std::string root_;
};

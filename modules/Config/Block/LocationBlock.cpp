#include "LocationBlock.hpp"

LocationBlock::LocationBlock(const std::string& match_directive) : match_directive_(match_directive) {}

const std::string& LocationBlock::getMatchDirective() const { return match_directive_; }
const std::vector<std::string>& LocationBlock::getAllowMethodVec() const { return v_allow_method_; }
const std::string& LocationBlock::getCgiPath() const { return cgi_path_; }
const std::string& LocationBlock::getUploadPath() const { return upload_path_; }

/**
 * @note location 블록은 하위 블록을 가질 수 없으므로, 예외를 던짐
 *
 */
void LocationBlock::addSubBlock(std::string& line) {
	(void)line;
	throw std::runtime_error("location blocks can't have sub-blocks");
}

void LocationBlock::setAllowMethodVec(std::string& value) {
	size_t pos_start = value.find_first_not_of(Utils::whitespace), pos_end;
	std::string method;

	while (pos_start != std::string::npos) {
		pos_end = value.find_first_of(Utils::whitespace, pos_start);

		method = value.substr(pos_start, pos_end - pos_start);
		if (method == "GET")
			v_allow_method_.push_back(method);
		else if (method == "POST")
			v_allow_method_.push_back(method);
		else if (method == "DELETE")
			v_allow_method_.push_back(method);
		else
			throw std::runtime_error("invalid HTTP method");

		pos_start = value.find_first_not_of(Utils::whitespace, pos_end);
	}
}

/**
 * @brief location 블록에서만 사용되는 지시어를 정제하는 함수
 *
 */
void LocationBlock::refineLocationDirectives() {
	std::map<std::string, std::string>::iterator directive_it;

	if ((directive_it = m_directives_.find("allow_method")) != m_directives_.end())
		setAllowMethodVec(directive_it->second);
	if ((directive_it = m_directives_.find("cgi_path")) != m_directives_.end())
		cgi_path_ = directive_it->second;
	if ((directive_it = m_directives_.find("upload_path")) != m_directives_.end())
		upload_path_ = directive_it->second;
}

void LocationBlock::refineDirectives() {
	refineLocationDirectives();
	common_directives_.refine(m_directives_); // 상위 블록인 서버 블록과 다른 공통 지시어가 있다면 덮어씀
}

bool LocationBlock::isAllowMethod(const std::string& method) const {
	return std::find(v_allow_method_.begin(), v_allow_method_.end(), method) != v_allow_method_.end();
}

bool LocationBlock::checkBodySize(int body_size) const {
	if (common_directives_.getClientMaxBodySize() > 0) // 클라이언트 본문 크기를 제한하는 경우
		return body_size <= common_directives_.getClientMaxBodySize();
	return true;
}

void LocationBlock::print() {
	std::string indent = "        ";
	std::cout << indent << "[LOCATION] \"" << match_directive_ << "\"" << std::endl;
	if (!v_allow_method_.empty()) {
		std::cout << indent << "- allow_method: ";
		for (size_t i = 0; i < v_allow_method_.size(); i++)
			std::cout << v_allow_method_[i] + " ";
		std::cout << std::endl;
	}
	if (cgi_path_ != "")
		std::cout << indent << "- cgi_path: " << cgi_path_ << std::endl;
	if (upload_path_ != "")
		std::cout << indent << "- upload_path: " << upload_path_ << std::endl;
	common_directives_.print(indent);
}

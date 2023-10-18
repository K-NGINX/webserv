#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "CommonDirectives.hpp"

/**
 * @brief 모든 블록의 부모 클래스
 * 
 * @note 공통 지시어와 하위 블록을 가질 수 있음
 * 
 */
class ABlock {
public:
    /* getter */
    const CommonDirectives& getCommonDirectives() const;

    void addDirective(const std::string& line);
    virtual void addSubBlock(const std::string& line) = 0;
    virtual void refineDirectives() = 0;

protected:
    std::map<std::string, std::string> m_directives_;
    CommonDirectives common_directives_;
};
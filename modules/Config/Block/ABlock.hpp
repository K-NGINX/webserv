#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "../CommonDirectives/CommonDirectives.hpp"

/**
 * @brief 모든 블록의 부모 클래스
 * 
 * @note 하위 블록을 가질 수 있음
 * 
 */
class ABlock {
public:
    virtual ~ABlock();
    
    virtual ABlock* getLastSubBlock();

    void addDirective(const std::string& line);
    virtual void addSubBlock(std::string& line) = 0;
    virtual void refineDirectives() = 0;
    virtual void print() = 0;

    CommonDirectives common_directives_;

protected:
    std::map<std::string, std::string> m_directives_; // 지시어 정제 후에는 사용되지 않음
};
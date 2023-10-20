#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "../../Utils/Utils.hpp"
#include "../CommonDirectives.hpp"

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
    void setCommonDirectives(CommonDirectives common_direcvites);

    void addDirective(const std::string& line);
    virtual void addSubBlock(std::string& line) = 0;
    virtual void refineDirectives() = 0;

protected:
    CommonDirectives common_directives_;
    std::map<std::string, std::string> m_directives_;
};
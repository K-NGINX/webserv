#include "UData.hpp"

UData::UData(FdType fd_type) : fd_type_(fd_type) {}

const FdType& UData::getFdType() const { return fd_type_; }

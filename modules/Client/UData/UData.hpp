#pragma once

#include <unistd.h>

enum FdType {
    CLIENT_SOCKET,
    CGI_FD,
    FILE_FD
};

class UData {
public:
    UData(FdType fd_type);

    const FdType& getFdType() const;

private:
    FdType fd_type_;
};
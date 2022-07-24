#pragma once

#include <string>

class IStringable {
    virtual std::string toString() const = 0;
};
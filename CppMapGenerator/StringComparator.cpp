#include "StringComparator.h"

bool StringComparator::operator()(const std::string& s1, const std::string& s2)
{
    return s1.compare(s2);
}

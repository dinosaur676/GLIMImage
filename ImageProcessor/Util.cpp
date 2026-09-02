#include "Util.h"

std::vector<std::string> util::stringSplit(std::string origin, std::string sep)
{
    std::vector<std::string> filterSplit;
    int currentPos = 0;
    int pos;

    while ((pos = origin.find(sep, currentPos)) != std::string::npos) {
        int len = pos - currentPos;
        filterSplit.push_back(origin.substr(currentPos, len));
        currentPos = pos + 1;
    }

    filterSplit.push_back(origin.substr(currentPos, origin.length() - currentPos));

    return filterSplit;
}

unsigned int util::clamp(int value, int min, int max)
{
    return value < min ? min : value > max ? max : value;
}

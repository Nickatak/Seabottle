/* Because I think I'm tired of C++ not having nice things; the implementations themselves will try to use as little of the stdlib as possible (other than the structures).  This may result in a performance loss, but, I think will be beneficial while cross compiling (which is what I'm currently doing).

THIS FILE IS NOT BEING USED IN SEABOTTLE, I'm just transferring stuff here for now -- I'll actually create a utility implementation of this stuff later.
*/
#ifndef CUSTOM_UTILS_H
#define CUSTOM_UTILS_H

#include <string>
#include <vector>


namespace custom_utils {
    // Might need one for const char*?  I'll templatize this later--MAYBE.
    std::vector<std::string> split(std::string input, std::string delimiter);
    std::vector<std::string> split(std::string input, std::string* delimiter);
    std::vector<std::string> split(std::string input, char delimiter);
    std::vector<std::string> split(std::string input, char* delimiter, size_t delimiter_length);
    std::vector<std::string> split(std::string* input, std::string delimiter);
    std::vector<std::string> split(std::string* input, std::string* delimiter);
    std::vector<std::string> split(std::string* input, char delimiter);
    std::vector<std::string> split(std::string* input, char* delimiter, size_t delimiter_length);

}


#endif

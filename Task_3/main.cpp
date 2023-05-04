#include <iostream>
#include <cctype>
#include <functional>

std::string trim(const std::string & str, int (*condition)(int));
std::string trim(const std::string & str, const std::function<bool (const unsigned char)> & condition);

int main() {

    std::string test = "   przykladowy string        ";
    std::cout << test<< std::endl;
    std::cout << trim(test, isspace)<< std::endl;
    std::cout << trim("   test    ", isspace)<< std::endl;

    return 0;
}

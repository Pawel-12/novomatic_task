#include <iostream>
#include <functional>

std::string trim(const std::string & str, const std::function<bool (const unsigned char)> & condition)
{
     int begin  = 0;
     int end    = str.size()-1;

    if(begin > end)
        return "";

    if(begin == end)
        return condition(str[begin]) ? "" : str;

    bool check_begin  = condition(str[begin]);
    bool check_end    = condition(str[end]);

    while((check_begin || check_end) && (begin < end) )
    {
        if(check_begin)
        {
            begin++;
            check_begin = condition(str[begin]);
        }
        if(check_end)
        {
            end--;
            check_end  = condition(str[end]);
        }
    }
    return str.substr(begin,end - begin + 1);
}

std::string trim(const std::string & str, int (*condition)(int))
{
    auto lambda = [&condition](const unsigned char x) -> bool {return condition(x);};

    return trim(str, lambda);
}
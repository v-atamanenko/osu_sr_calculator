#ifndef OSU_SR_CALCULATOR_UTILS_H
#define OSU_SR_CALCULATOR_UTILS_H

#include "vector"
#include "string"

class Utils {
public:
    static std::string str_join(const std::vector<std::string> &lst, const std::string &delim)
    {
        std::string ret;
        for(const auto &s : lst) {
            if(!ret.empty())
                ret += delim;
            ret += s;
        }
        return ret;
    }

    static std::vector<std::string> str_split(const std::string& str, const std::string &delim, int expected_len=16)
    {
        std::vector<std::string> vec;
        vec.reserve(expected_len);

        auto start = 0U;
        auto end = str.find(delim);
        while (end != std::string::npos)
        {
            vec.emplace_back(str.substr(start, end - start));
            start = end + delim.length();
            end = str.find(delim, start);
        }

        vec.emplace_back(str.substr(start));
        return vec;
    }

    static std::vector<std::string> str_split_and_strip(const std::string& str, const std::string &delim, int expected_len=16)
    {
        std::vector<std::string> vec;
        vec.reserve(expected_len);

        auto start = 0U;
        auto end = str.find(delim);
        while (end != std::string::npos)
        {
            std::string s = str.substr(start, end - start);
            vec.emplace_back(trim(s));
            start = end + delim.length();
            end = str.find(delim, start);
        }

        std::string s = str.substr(start);
        vec.emplace_back(trim(s));
        return vec;
    }

    // trim from left
    static inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
    {
        std::size_t found = s.find_first_not_of(t);
        if (found != std::string::npos) {
            s.erase(0, found);
        }
        return s;
    }

    // trim from right
    static inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
    {
        std::size_t found = s.find_last_not_of(t);
        if (found != std::string::npos) {
            s.erase(found + 1);
        }
        return s;
    }

    // trim from left & right
    static inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v")
    {
        return ltrim(rtrim(s, t), t);
    }

    template<typename T>
    static std::vector<T> vec_slice(const std::vector<T> &v, int m, int n)
    {
        if (n == 0) {
            return std::vector<T>();
        }
        if (n > (v.size() - 1)) {
            n = v.size() - 1;
        }
        // Begin and End iterator
        auto first = v.begin() + m;
        auto last = v.begin() + n + 1;
        if (n+1 >= v.size()) {
            last = v.end();
        }

        // Return the results
        return std::vector<T>(first, last);
    }

    template<typename T>
    static void vec_splice(std::vector<T> &v, int m, int n)
    {
        v.erase(
                v.begin() + m,
                v.begin() + m + n
        );
    }

    template<typename T>
    static int vec_getValIndex(const std::vector<T>& v, T K)
    {
        auto it = find(v.begin(), v.end(), K);
        assert(it != v.end());
        return ((int)(it - v.begin()));
    }
};


#endif //OSU_SR_CALCULATOR_UTILS_H

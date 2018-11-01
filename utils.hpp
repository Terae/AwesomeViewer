//
// Created by terae on 01/11/18.
//

#ifndef AWESOME_VIEWER_UTILS_HPP
#define AWESOME_VIEWER_UTILS_HPP

#include <string>
#include <vector>
#include <algorithm>

namespace AwesomeViewer {

    auto to_string(std::string const& x) -> std::string {
        return x;
    }

    template<class T>
    auto to_string(T const& x) -> decltype(std::to_string(x)) {
        return std::to_string(x);
    }


    std::vector<std::string> split(std::string const& str, const std::string& delimiter = "\n") {
        std::vector<std::string> tokens;

        auto start = 0U;
        auto end = str.find(delimiter);
        while (end != std::string::npos) {
            tokens.push_back(str.substr(start, end - start));
            start = end + delimiter.size();
            end = str.find(delimiter, start);
        }

        if(start != str.size())
            tokens.push_back(str.substr(start, str.size()-start));

        return tokens;
    }


    template<class T, class F>
    auto map(T const& data, const F& f) {
        std::vector<decltype(f(data[0]))> result(data.size());
        std::transform(
                data.begin(), data.end(),
                result.begin(),
                f);
        return result;
    }

    std::string repeat(unsigned n, const std::string& s) {
        std::string result;
        for(unsigned i=0; i<n;++i) {
            result += s;
        }
        return result;
    }

    std::string clear_before_cursor() {
        return "\e[0K";
    }

    std::string clear_after_cursor() {
        return "\e[1K";
    }

    std::string clear_line() {
        return "\e[2K\r";
    }


    std::string move_up(unsigned n = 1) {
        return "\e[" + std::to_string(n) + "A\r";
    }


    std::string clear_lines(unsigned n = 1) {
        return "\e[0m" + clear_before_cursor() + ((n) ? repeat(n, clear_line() + move_up()) : std::string(""));
    }

}

#endif //AWESOME_VIEWER_UTILS_HPP

//
// Created by terae on 01/11/18.
//

#ifndef AWESOME_VIEWER_STYLESTRING_H
#define AWESOME_VIEWER_STYLESTRING_H

#include "Style.hpp"

#include <algorithm>
#include <string>
#include <utility>
#include <deque>
#include <stdexcept>

namespace AwesomeViewer {

    class StyleString {
        std::deque<std::pair<Style, std::string>> _data;

      public:
        StyleString(std::initializer_list<std::pair<Style, std::string>> list) : _data(list) {}

        explicit StyleString(std::deque<std::pair<Style, std::string>> str) : _data(std::move(str)) {}
        StyleString(Style style, std::string str) {
            _data.emplace_back(style, std::move(str));
        }
        explicit StyleString(const std::string &str) : StyleString(Style::Default(), str) {}
        StyleString() = default;

        std::string to_string() const {
            std::string result;
            for (const auto &p : _data) {
                result += p.first.to_string() + p.second;
            }
            return result;
        }

        std::size_t size() const {
            std::size_t size = 0;
            std::for_each(_data.cbegin(), _data.cend(), [&size](const auto & p) {
                size += p.second.size();
            });
            return size;
        }

        StyleString substr(std::size_t begin_pos, std::size_t end_pos = std::string::npos) const {
            StyleString copy = *this;

            while (begin_pos > 0) {
                auto size = copy._data.front().second.size();
                if (size >= begin_pos) {
                    copy._data[0].second = copy._data.front().second.substr(begin_pos);
                    begin_pos = 0;
                } else {
                    copy._data.pop_front();
                    begin_pos -= size;
                }
            }

            if (end_pos != std::string::npos) {
                std::size_t count = 0;
                for (std::size_t i = 0; i < copy._data.size(); ++i) {
                    // need to truncate
                    if (count + copy._data[i].second.size() > end_pos) {
                        copy._data[i].second = copy._data[i].second.substr(0, end_pos - count);
                        copy._data.resize(i + 1);
                        break;
                    }
                }
            }
            return std::move(copy);
        }

        StyleString &operator+=(const std::string &str) {
            this->insert(str);
            return *this;
        }

        StyleString &operator+=(std::pair<Style, std::string> p) {
            this->insert(std::move(p));
            return *this;
        }

        void insert(std::pair<Style, std::string> p) {
            _data.push_back(std::move(p));
        }

        void insert(Style style, std::string str) {
            _data.emplace_back(style, std::move(str));
        }

        void insert(std::string str) {
            _data.emplace_back(Style::Default(), std::move(str));
        }

        inline bool empty() const {
            return _data.empty();
        }

        inline void clear() {
            _data.clear();
        }

        std::size_t find_first_of(char c) const {
            std::size_t position = 0;
            bool found = false;
            for (const auto &p : _data) {
                auto result = p.second.find_first_of(c);
                if (result == std::string::npos) {
                    position += p.second.size();
                } else {
                    position += result;
                    found = true;
                    break;
                }
            }

            if (!found) {
                return std::string::npos;
            }

            return position;
        }
    };

    std::ostream &operator<<(std::ostream &os, const StyleString &str) {
        return os << str.to_string();
    }
}

#endif //AWESOME_VIEWER_STYLESTRING_H

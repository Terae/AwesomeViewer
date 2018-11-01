//
// Created by terae on 30/10/18.
//

#ifndef UNTITLED_CELL_H
#define UNTITLED_CELL_H

#include <algorithm>
#include <functional>
#include <iomanip>
#include <map>
#include <string>
#include <sstream>
#include <vector>

#include "Style.hpp"

namespace AwesomeViewer {

    class AbstractCell {
      protected:
        unsigned int _width, _height;
        std::vector<std::string> _data;

        AbstractCell(unsigned int width, unsigned int height) :
            _width(width), _height(height) {}

      public:
        virtual void update() = 0;

        unsigned int get_height() const {
            return _height;
        }
        unsigned int get_width() const {
            return _width;
        }

        std::string get_nth_line(std::size_t line) const {
            if (_data.empty()) {
                throw std::runtime_error("You need to update the cell.");
            }
            if (_height < line) {
                throw std::range_error("Line out of range.");
            }
            return _data[line];
        }
    };

    class StringCell : public AbstractCell {
        std::function<std::string()> _data_generator;

      public:
        StringCell(unsigned int width, unsigned int height, std::function<std::string()> generator) : AbstractCell(width,
                    height), _data_generator(std::move(generator)) {}
        StringCell(unsigned int width, unsigned int height, const std::string &str) : StringCell(width, height, [str]() {
            return str;
        }) {}

        void update() override {
            _data.clear();
            std::string str = _data_generator();
            for (unsigned int i = 0; i < _height; ++i) {
                if (str.empty()) {
                    _data.emplace_back(_width, ' ');
                } else {
                    std::size_t eol = str.find_first_of('\n');
                    std::string tmp = str.substr(0, std::min(eol, static_cast<std::size_t>(_width)));
                    if (tmp.size() < _width) {
                        tmp += std::string(_width - tmp.size(), ' ');
                    }

                    if (eol == std::string::npos) {
                        str = "";
                    } else {
                        str = str.substr(eol + 1);
                    }

                    _data.push_back(tmp);
                }
            }
        }
    };

    class ProgressCell : public AbstractCell {
        std::function<double()> _percent_generator;
        bool _print_percent;

      public:
        ProgressCell(unsigned int width, unsigned int height, std::function<double()> percent_generator,
                     bool print_percent = true)
            : AbstractCell(width, height), _percent_generator(std::move(percent_generator)) {
            _print_percent = print_percent && _width > 7;
        }

        void update() override {
            _data.clear();
            double progress = _percent_generator();
            progress = std::min(100.0, std::max(0.0, progress));
            unsigned int progress_width = (_print_percent ? _width - 4 : _width);
            auto amount = static_cast<unsigned int>(progress * progress_width / 100);

            Style style = Style::Default();
            style.bg = Color::Green;
            std::stringstream result;
            result << style.to_string() << std::string(amount, ' ');
            style.bg = Color::Transparent;
            result << style.to_string() << std::string(progress_width - amount, ' ');

            if (_print_percent) {
                style.fg = FontColor::Black;
                style.font = Font::Bold;
                result << style.to_string() << std::right << std::setw(3) << static_cast<int>(progress) << '%';
            }
            _data.push_back(result.str());
        }
    };

    template <typename T>
    class MapCell : public AbstractCell {
        std::function<std::map<std::string, T>()> _data_generator;

      public:
        MapCell(unsigned int width, unsigned int height,
                std::function<std::map<std::string, T>()> generator) : AbstractCell(width, height),
            _data_generator(std::move(generator)) {}
        MapCell(unsigned int width, unsigned int height, std::map<std::string, T> map) : MapCell(width, height, [map]() {
            return map;
        }) {}

        void update() override {
            _data.clear();

            auto generated_map = _data_generator();
            unsigned int max_size = 0;

            auto it = generated_map.begin();
            for (unsigned int i = 0; i < _height; i++, it++) {
                if (it == generated_map.cend()) {
                    break;
                }

                if (it->first.size() > max_size) {
                    max_size = it->first.size();
                }
            }

            it = generated_map.begin();
            for (unsigned int i = 0; i < _height; ++i) {
                std::string result;
                Style style = Style::Default();

                if (it == generated_map.cend()) {
                    result = std::string(max_size + 1, ' ') + "-" + std::string(_width - max_size - 2, ' ');

                    style.fg = FontColor::Black;
                    style.font = Font::Bold;
                    result.insert(0, style.to_string());
                } else {
                    std::stringstream ss;
                    ss << std::right << std::setw(max_size) << it->first << " : " << std::left << std::setw(
                           _width - 3 - max_size) << it->second;
                    result = ss.str().substr(0, _width);

                    if (result.size() >= max_size + 2) {
                        result.insert(max_size + 2, style.to_string());
                    }

                    style.fg = FontColor::Black;
                    style.font = Font::Bold;
                    result.insert(0, style.to_string());

                    ++it;
                }
                _data.push_back(result);
            }
        }
    };

}


#endif //UNTITLED_CELL_H

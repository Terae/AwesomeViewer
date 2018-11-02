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
#include "StyleString.hpp"

namespace AwesomeViewer {

    class AbstractCell {
      protected:
        unsigned int _width, _height;
        std::vector<StyleString> _data;

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
            return _data[line].to_string();
        }
    };

    class StringCell : public AbstractCell {
        std::function<StyleString()> _data_generator;

      public:
        StringCell(unsigned int width, unsigned int height, std::function<std::string()> generator) : StringCell(
                width,
                height,
                [generator]() {
            return StyleString(Style::Default(), generator());
        }
        ) {}

        StringCell(unsigned int width, unsigned int height, std::function<StyleString()> generator) : AbstractCell(
                width,
                height), _data_generator(std::move(generator)) {}

        StringCell(unsigned int width, unsigned int height, const std::string &str) :
            StringCell(width, height, {
            Style::Default(), str
        }) {}

        StringCell(unsigned int width, unsigned int height, const StyleString &str) :
            StringCell(width, height,
                       [str]() {
            return str;
        }) {}

        void update() override {
            _data.clear();
            StyleString str = _data_generator();
            for (unsigned int i = 0; i < _height; ++i) {
                if (str.empty()) {
                    _data.emplace_back(Style::Default(), std::string(_width, ' '));
                } else {
                    std::size_t eol = str.find_first_of('\n');
                    StyleString tmp = str.substr(0, std::min(eol, static_cast<std::size_t>(_width)));
                    if (tmp.size() < _width) {
                        tmp += std::string(_width - tmp.size(), ' ');
                    }

                    if (eol == std::string::npos) {
                        str.clear();
                    } else {
                        str = str.substr(eol + 1);
                    }

                    _data.push_back(tmp);
                }
            }
        }
    };

    class ProgressCell : public AbstractCell {
        double _min;
        double _max;
        std::function<double()> _percent_generator;
        bool _print_percent;

      public:
        ProgressCell(unsigned int width, unsigned int height, double min, double max,
                     std::function<double()> percent_generator,
                     bool print_percent = true)
            : AbstractCell(width, height), _min(min), _max(max), _percent_generator(std::move(percent_generator)) {
            _print_percent = print_percent && _width > 7;
            if (_max < _min) {
                double tmp = _min;
                _min = _max;
                _max = tmp;
            }
        }

        ProgressCell(unsigned int width, unsigned int height, std::function<double()> percent_generator,
                     bool print_percent = true) : ProgressCell(width, height, 0.0, 100.0, std::move(percent_generator),
                                 print_percent) {}

        void update() override {
            _data.clear();
            double progress = _percent_generator();
            progress = std::min(_max, std::max(_min, progress));
            if (_min < 0) {
                progress -= _min;
            }
            progress *= 100.0 / (_max - _min);
            unsigned int progress_width = (_print_percent ? _width - 4 : _width);
            auto amount = (_min == _max ? progress_width : static_cast<unsigned int>(progress * progress_width / 100));

            StyleString result;
            result.insert(Style(Color::Green), std::string(amount, ' '));

            result.insert(Style::Default(), std::string(progress_width - amount, ' '));

            if (_print_percent) {
                std::stringstream ss;
                ss << std::right << std::setw(3) << static_cast<int>(progress) << '%';

                result.insert(Style(FontColor::Black, Font::Bold), ss.str());
            }
            _data.push_back(result);
        }
    };

    template<typename T>
    class MapCell : public AbstractCell {
        std::function<std::map<std::string, T>()> _data_generator;

        template<class Q = T>
        typename std::enable_if<std::is_same<Q, StyleString>::value, StyleString>::type T_to_string(Q x, unsigned int size) {
            StyleString str = x.substr(0, size);
            str += std::string(std::max(0, (int)(size - x.size())), ' ');
            return str;
        }

        template<class Q = T>
        typename std::enable_if < !std::is_same<Q, StyleString>::value, StyleString >::type T_to_string(Q x,
                unsigned int size) {
            std::stringstream ss;
            ss << std::left << std::setw(size) << x;

            return StyleString(ss.str());
        }

      public:
        MapCell(unsigned int width, unsigned int height,
                std::function<std::map<std::string, T>()> generator) : AbstractCell(width, height),
            _data_generator(std::move(generator)) {}

        MapCell(unsigned int width, unsigned int height, std::map<std::string, T> map) :
            MapCell(width, height,
                    [map]() {
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
                StyleString result;

                if (it == generated_map.cend()) {
                    std::string str = std::string(max_size + 1, ' ') + "-" + std::string(_width - max_size - 2, ' ');
                    result.insert(Style(FontColor::Black, Font::Bold), str);
                } else {
                    std::stringstream ss_left;
                    ss_left << std::right << std::setw(max_size) << it->first << " : ";
                    result.insert(Style(FontColor::Black, Font::Bold), ss_left.str());

                    result += T_to_string(it->second, _width - 3 - max_size);

                    ++it;
                }
                _data.push_back(result.substr(0, _width));
            }
        }
    };
}

#endif //UNTITLED_CELL_H

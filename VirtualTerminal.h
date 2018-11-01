//
// Created by terae on 31/10/18.
//

#ifndef AWESOME_VIEWER_VIRTUALTERMINAL_H
#define AWESOME_VIEWER_VIRTUALTERMINAL_H

#include "Cell.h"
#include "Pixel.hpp"

#include <algorithm>
#include <chrono>
#include <limits>
#include <memory>
#include <thread>

namespace AwesomeViewer {

    class VirtualTerminal {
        unsigned int _width;
        unsigned int _height;

        std::vector<std::vector<std::unique_ptr<AbstractPixel>>> _pixels;

        struct Coord {
            unsigned int x, y;

            friend bool operator==(const Coord& c1, const Coord& c2) {
                return c1.x == c2.x && c1.y == c2.y;
            }
        };
        const Coord out_of_space = {std::numeric_limits<unsigned int>::max(), std::numeric_limits<unsigned int>::max()};


        Coord get_free_space(const AbstractCell& cell) const {
            // TODO
            bool found = false;
            Coord result{std::numeric_limits<unsigned int>::max(), std::numeric_limits<unsigned int>::max()};
            for(unsigned int y = 0; y < _height - cell.get_height() && !found; ++y) {
                for(unsigned int x = 0; x < _width - cell.get_width() && !found; ++x) {
                    bool interesting_cell = _pixels[y][x] == nullptr;
                    if(!interesting_cell) {
                        interesting_cell = _pixels[y][x]->can_be_overwritten();
                    }

                    if(interesting_cell) {
                        bool is_ok = true;
                        for(unsigned int Y = y; Y < y + cell.get_height() && is_ok; ++Y) {
                            for(unsigned int X = x; X < x + cell.get_width() && is_ok; ++X) {
                                if(_pixels[Y][X] != nullptr) {
                                    auto p = _pixels[y][x]->get_type();
                                    if(!_pixels[Y][X]->can_be_overwritten()) {
                                        is_ok = false;
                                    }
                                }
                            }
                        }
                        if(is_ok) {
                            found = true;
                            result = {x, y};
                        }
                    }
                }
            }
            return result;
        }

        void insert_border(const Coord& coords, PixelType border) {
            if(_pixels[coords.y][coords.x] == nullptr) {
                _pixels[coords.y][coords.x] = std::make_unique<BorderPixel>(border);
            } else {
                try {
                    auto &pixel = dynamic_cast<BorderPixel &>(*_pixels[coords.y][coords.x]);
                    pixel.update_border_type(border);
                } catch(std::bad_cast&) {
                    throw std::runtime_error("The pixel isn't a border.");
                }
            }
        }

    public:
        VirtualTerminal(unsigned int max_width, unsigned int max_height) : _width(max_width), _height(max_height) {
            for(unsigned int y = 0; y < _height; ++y) {
                std::vector<std::unique_ptr<AbstractPixel>> v(_width);
                for(unsigned int x = 0; x < _width; ++x) {
                    v.emplace_back(nullptr);
                }
                _pixels.emplace_back(std::move(v));
            }
        }

        void add_cell(const std::string& name, AbstractCell& cell) {
            Coord space = get_free_space(cell);
            if(space == out_of_space) {
                throw std::runtime_error("No space left.");
            }

            // Top border
            unsigned int x = space.x;
            unsigned int y = space.y;
            insert_border({x++, y}, TopLeftCorner);
            insert_border({x++, y}, HorizontalBorder);
            insert_border({x++, y}, EmptyBorder);

            std::string s = name.substr(0, (std::size_t)std::max(static_cast<int>(cell.get_width()) - 2, 0));
            _pixels[y][x++] = std::make_unique<CellNamePixel>(s);
            for(unsigned int i = 0; i < s.size() - 1; ++i) {
                _pixels[y][x++] = std::make_unique<EmptyPixel>();
            }

            insert_border({x++, y}, EmptyBorder);

            for(unsigned int i = 0; i < cell.get_width() - 1 - s.size(); ++i) {
                insert_border({x++, y}, HorizontalBorder);
            }
            insert_border({x, y++}, TopRightCorner);


            // Cell
            for(unsigned int i = 0; i < cell.get_height(); ++i) {
                x = space.x;
                insert_border({x++, y}, VerticalBorder);
                insert_border({x++, y}, EmptyBorder);

                _pixels[y][x++] = std::make_unique<CellValuePixel>([&cell, i]() {
                    if (i == 0) {
                        cell.update();
                    }
                    return cell.get_nth_line(i);
                });
                for(unsigned int j = 0; j < cell.get_width() - 1; ++j) {
                    _pixels[y][x++] = std::make_unique<EmptyPixel>();
                }

                insert_border({x++, y}, EmptyBorder);
                insert_border({x, y++}, VerticalBorder);
            }

            // Bottom border
            x = space.x;
            insert_border({x++, y}, BottomLeftCorner);
            for(unsigned i = 0; i < cell.get_width() + 2; ++i) {
                insert_border({x++, y}, HorizontalBorder);
            }
            insert_border({x, y}, BottomRightCorner);
        }

        void print(std::ostream& os, std::chrono::duration<int64_t, std::milli> tempo = std::chrono::milliseconds(200)) const {
            for (const auto& line : _pixels) {
                for(const auto& pixel : line) {
                    if (pixel == nullptr) {
                        os << ' ' << std::flush;
                    } else {
                        os << pixel->to_string() << std::flush;
                    }
                }
                os << std::endl;
            }
            os << std::endl;
            std::this_thread::sleep_for(tempo);
        }
    };

}


#endif //AWESOME_VIEWER_VIRTUALTERMINAL_H

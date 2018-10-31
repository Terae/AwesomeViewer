//
// Created by terae on 31/10/18.
//

#ifndef AWESOME_VIEWER_VIRTUALTERMINAL_H
#define AWESOME_VIEWER_VIRTUALTERMINAL_H

#include "Cell.h"
#include "Pixel.hpp"

#include <algorithm>
#include <memory>
#include <chrono>
#include <thread>

namespace AwesomeViewer {

    class VirtualTerminal {
        unsigned int _width;
        unsigned int _height;

        std::vector<std::vector<std::unique_ptr<AbstractPixel>>> _pixels;

        struct Coord {
            unsigned int x, y;
        };
        Coord get_free_space(AbstractCell& cell) const {
            return {0, 0};
        }

    public:
        VirtualTerminal(unsigned int max_width, unsigned int max_height) : _width(max_width), _height(max_height) {
            for(unsigned int y = 0; y < _height; ++y) {
                _pixels.emplace_back();
                _pixels[y].reserve(_width); // FIXME : util?
            }
        }

        void add_cell(const std::string& name, AbstractCell& cell) {
            Coord space = get_free_space(cell);

            // Top border
            _pixels[space.y].emplace_back(std::make_unique<BorderPixel>(TopLeftCorner));
            _pixels[space.y].emplace_back(std::make_unique<BorderPixel>(HorizontalBorder));
            _pixels[space.y].emplace_back(std::make_unique<BorderPixel>(EmptyBorder));
            std::string s = name.substr(0, (std::size_t)std::max(static_cast<int>(cell.get_width()) - 6, 0));
            _pixels[space.y].emplace_back(std::make_unique<CellNamePixel>(s));
            for(unsigned int i = 0; i < s.size() - 1; ++i) {
                _pixels[space.y].emplace_back(std::make_unique<EmptyPixel>());
            }
            _pixels[space.y].emplace_back(std::make_unique<BorderPixel>(EmptyBorder));
            for(unsigned int i = 0; i < cell.get_width() - 1 - s.size(); ++i) {
                _pixels[space.y].emplace_back(std::make_unique<BorderPixel>(HorizontalBorder));
            }
            _pixels[space.y].emplace_back(std::make_unique<BorderPixel>(TopRightCorner));

            // Cell
            cell.update();
            for(unsigned int y = 0; y < cell.get_height(); ++y) {
                _pixels[space.y + y + 1].emplace_back(std::make_unique<BorderPixel>(VerticalBorder));
                _pixels[space.y + y + 1].emplace_back(std::make_unique<BorderPixel>(EmptyBorder));
                _pixels[space.y + y + 1].emplace_back(std::make_unique<CellValuePixel>([&cell, y]() {
                    return cell.get_nth_line(y);
                }));
                for(unsigned int x = 0; x < cell.get_width() - 1; ++x) {
                    _pixels[y + 1].emplace_back(std::make_unique<EmptyPixel>());
                }
                _pixels[space.y + y + 1].emplace_back(std::make_unique<BorderPixel>(EmptyBorder));
                _pixels[space.y + y + 1].emplace_back(std::make_unique<BorderPixel>(VerticalBorder));
            }

            // Bottom border
            _pixels[cell.get_height() + 1].emplace_back(std::make_unique<BorderPixel>(BottomLeftCorner));
            for(unsigned x = 0; x < cell.get_width() + 2; ++x) {
                _pixels[space.y + cell.get_height() + 1].emplace_back(std::make_unique<BorderPixel>(HorizontalBorder));
            }
            _pixels[space.y + cell.get_height() + 1].emplace_back(std::make_unique<BorderPixel>(BottomRightCorner));
        }

        void print(std::ostream& os, std::chrono::duration<int64_t, std::milli> tempo = std::chrono::milliseconds(200)) const {
            for (const auto& line : _pixels) {
                for(const auto& pixel : line) {
                    os << pixel->to_string();
                }
                os << std::endl;
            }
            os << std::endl;
            std::this_thread::sleep_for(tempo);
        }
    };

}


#endif //AWESOME_VIEWER_VIRTUALTERMINAL_H

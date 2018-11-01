//
// Created by terae on 31/10/18.
//

#ifndef AWESOME_VIEWER_PIXEL_H
#define AWESOME_VIEWER_PIXEL_H

#include "Style.hpp"
#include <functional>

namespace AwesomeViewer {
    enum PixelType {
        // bits order: 'bottom | top |left | right'
        EmptyBorder           = 0b00000, //
        HorizontalLeftBorder  = 0b00010, // ─
        HorizontalRightBorder = 0b00001, // ─
        HorizontalBorder      = 0b00011, // ─
        VerticalTopBorder     = 0b00100, // │
        VerticalBottomBorder  = 0b01000, // │
        VerticalBorder        = 0b01100, // │
        TopLeftCorner         = 0b00110, // ┌
        TopRightCorner        = 0b00101, // ┐
        BottomLeftCorner      = 0b01010, // └
        BottomRightCorner     = 0b01001, // ┘
        HorizontalTopT        = 0b00111, // ┬
        HorizontalBottomT     = 0b01011, // ┴
        VerticalRightT        = 0b01101, // ┤
        VerticalLeftT         = 0b01110, // ├
        Cross                 = 0b01111, // ┼
        CellName              = 0b010000, // begin of the cell name
        CellValue             = 0b100000, // begin of a cell line
    };

    PixelType operator+(const PixelType &p1, const PixelType &p2) {
        if (p1 == CellValue || p1 == CellName || p1 == EmptyBorder)
            return p1;
        if (p2 == CellValue || p2 == CellName || p2 == EmptyBorder)
            return p2;
        return static_cast<PixelType>(p1 | p2);
    }

    std::string get_symbol_of(PixelType type) {
        switch(type) {
            case EmptyBorder:
                return " ";
            case HorizontalLeftBorder:
                return "─";
            case HorizontalRightBorder:
                return "─";
            case HorizontalBorder:
                return "─";
            case VerticalTopBorder:
                return "│";
            case VerticalBottomBorder:
                return "│";
            case VerticalBorder:
                return "│";
            case TopLeftCorner:
                return "┌";
            case TopRightCorner:
                return "┐";
            case BottomLeftCorner:
                return "└";
            case BottomRightCorner:
                return "┘";
            case HorizontalTopT:
                return "┬";
            case HorizontalBottomT:
                return "┴";
            case VerticalRightT:
                return "┤";
            case VerticalLeftT:
                return "├";
            case Cross:
                return "┼";
            case CellName:
            case CellValue:
                throw std::invalid_argument("None associated symbol.");
        }
    }

    class AbstractPixel {
    protected:
        PixelType _type = EmptyBorder;
        Style _style = Style::Default();

    public:
        virtual std::string to_string() const = 0;

        PixelType get_type() const { return _type; }

        bool can_be_overwritten() const {
            return _type != EmptyBorder && _type != CellValue && _type != CellName;
        }
    };

    class EmptyPixel : public AbstractPixel {
    public:
        EmptyPixel() = default;

        std::string to_string() const override {
            return "";
        }
    };

    class CellNamePixel : public AbstractPixel {
    private:
        std::string _name;

    public:
        explicit CellNamePixel(std::string name) : _name(std::move(name)) {
            _style.fg = FontColor::Cyan;
            _type = CellName;
        }

        std::string to_string() const override {
            return _style.to_string() + _name;
        }
    };

    class CellValuePixel : public AbstractPixel {
    private:
        std::function<std::string()> _value;

    public:
        explicit CellValuePixel(std::function<std::string()> value) : _value(std::move(value)) {
            _type = CellValue;
            _style.font = Font::Default;
            _style.fg = FontColor::White;
        }

        std::string to_string() const override {
            return _style.to_string() + _value();
        }
    };

    class BorderPixel : public AbstractPixel {
    public:
        explicit BorderPixel(PixelType type) {
            _style.fg = FontColor::Black;
            _style.font = Font::Bold;
            _type = type;
        }

        void update_border_type(const PixelType& other) {
            _type = (_type + other);
        }

        std::string to_string() const override {
            return _style.to_string() + get_symbol_of(_type);
        }
    };
}

#endif //AWESOME_VIEWER_PIXEL_H

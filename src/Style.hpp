//
// Created by terae on 31/10/18.
//

#ifndef AWESOME_VIEWER_STYLE_H
#define AWESOME_VIEWER_STYLE_H

#include <string>

namespace AwesomeViewer {
    using std::string;

    enum class Font {
        None = 0,
        Default = 1,
        Bold = 1 << 1, // 1
        Faint = 1 << 2, // 2
        Roman = 1 << 3,// not Italic
        Italic = 1 << 4, // 3
        NotUnderline = 1 << 5,
        Underline = 1 << 6, //4
        Visible = 1 << 7,
        Hidden = 1 << 8, //8
        Uncrossed = 1 << 9,
        Crossed = 1 << 10, // 9
        Inherit = 1 << 11
    };

    enum class FontColor {
        None = 0,
        Black = 1,
        Red = 2,
        Green = 3,
        Yellow = 4,
        Blue = 5,
        Magenta = 6,
        Cyan = 7,
        White = 8,
        Default = 9,
        Transparent = 10,
        Inherit = 11
    };


    enum class Color {
        None = 0,
        Black = 1,
        Red = 2,
        Green = 3,
        Yellow = 4,
        Blue = 5,
        Magenta = 6,
        Cyan = 7,
        White = 8,
        Default = 9,
        Transparent = 10,
        Inherit = 11
    };

    template<class X>
    constexpr auto compose_mod(X x) {
        return x;
    }

    template<class X, class...Xs>
    constexpr auto compose_mod(X x, Xs...xs) {
        auto const r = compose_mod(xs...);
        auto const delim = r.empty() || x.empty() ? "" : ";";
        return x + delim + r;
    }

    template<class X, class...Xs>
    constexpr auto compute_mod(X x, Xs...xs) {
        auto const r = compose_mod(x, xs...);
        return r.empty() ? "" : "\e[" + r + "m";
    }

    FontColor is_style(FontColor x) {
        return x;
    }

    Color is_style(Color x) {
        return x;
    }

    Font is_style(Font x) {
        return x;
    }

    FontColor to_font_color(FontColor c) {
        return c;
    }

    Color to_color(Color c) {
        return c;
    }

    Font to_font(Font f) {
        return f;
    }

    FontColor to_font_color(...) {
        return FontColor::None;
    }

    Color to_color(...) {
        return Color::None;
    }

    Font to_font(...) {
        return Font::None;
    }

    template<class...Xs>
    constexpr FontColor get_font_color(Xs...xs) {
        return (FontColor) std::max({0, (int) to_font_color(xs)...});
    }

    template<class...Xs>
    constexpr Color get_color(Xs...xs) {
        return (Color) std::max({0, (int) to_color(xs)...});
    }

    constexpr Font get_font_style(Font f = Font::None) {
        return f;
    }


    template<class X, class...Xs>
    constexpr Font get_font_style(X x, Xs...xs) {
        return Font(((int) get_font_style(to_font(x))) | ((int) get_font_style(to_font(xs)...)));
    }


    bool has(Font x, Font y) {
        return (bool) (((int) x) & ((int) y));
    }

    struct Style {
        Color bg;
        FontColor fg;
        Font font;

        template<class...Styles>
        constexpr Style(
            Styles...styles)
            : bg{get_color(is_style(styles)...)}, fg{get_font_color(is_style(styles)...)},
              font{get_font_style(is_style(styles)...)} {}

        constexpr static Style None() {
            return {};
        }

        constexpr static Style Default() {
            return {Font::Default};
        }

        string default_mod() const {
            return has(font, Font::Default) ? "0" : "";
        }


        string bold_mod() const {
            return has(font, Font::Bold) ? "1" : "";
        }

        string underline_mod() const {
            return has(font, Font::Underline) ? "4" : "";
        }

        string faint_mod() const {
            return has(font, Font::Faint) ? "2" : "";
        }

        string italic_mod() const {
            return has(font, Font::Italic) ? "3" : "";
        }

        string hidden_mod() const {
            return has(font, Font::Hidden) ? "8" : "";
        }

        string crossed_mod() const {
            return has(font, Font::Crossed) ? "9" : "";
        }


        string bg_mod() const {
            return ((int) bg < 11) ? std::to_string(40 + (int) bg - 1) : "";
        }

        string fg_mod() const {
            return ((int) fg < 11) ? std::to_string(30 + (int) fg - 1) : "";
        }

        std::string to_string() const {
            return compute_mod(
                       default_mod(),
                       bold_mod(),
                       underline_mod(),
                       faint_mod(),
                       italic_mod(),
                       hidden_mod(),
                       crossed_mod(),
                       bg_mod(),
                       fg_mod()
                   );
        }
    };


    Style diff(Style const &a, Style const &b = Style::None()) {
        bool keepBG = (a.bg == b.bg);
        bool keepFG = (a.fg == b.fg);
        bool keepFont = (a.font == b.font);

        int l = (int) a.font;
        int r = (int) b.font;
        bool reset = static_cast<bool>(l & ~r);

        return Style{
            (keepBG and !reset) ? Color::Inherit : b.bg,
            (keepFG and !reset) ? FontColor::Inherit : b.fg,
            (keepFont and !reset) ? Font::Inherit : (Font) ((r & ((l & r) ^ r)) | reset)
        };
    }
}

#endif //AWESOME_VIEWER_STYLE_H

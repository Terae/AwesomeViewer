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

    constexpr FontColor is_style(FontColor x) {
        return x;
    }

    constexpr Color is_style(Color x) {
        return x;
    }

    constexpr Font is_style(Font x) {
        return x;
    }

    constexpr FontColor to_font_color(FontColor c) {
        return c;
    }

    constexpr Color to_color(Color c) {
        return c;
    }

    constexpr Font to_font(Font f) {
        return f;
    }

    constexpr FontColor to_font_color(...) {
        return FontColor::None;
    }

    constexpr Color to_color(...) {
        return Color::None;
    }

    constexpr Font to_font(...) {
        return Font::None;
    }

    template<class...Xs>
    constexpr FontColor get_font_color(Xs...xs) {
        return static_cast<FontColor>(std::max({0, static_cast<int>(to_font_color(xs))...}));
    }

    template<class...Xs>
    constexpr Color get_color(Xs...xs) {
        return static_cast<Color>(std::max({0, static_cast<int>(to_color(xs))...}));
    }

    constexpr Font get_font_style(Font f = Font::None) {
        return f;
    }


    template<class X, class...Xs>
    constexpr Font get_font_style(X x, Xs...xs) {
        return Font((static_cast<int>(get_font_style(to_font(x)))) | (static_cast<int>(get_font_style(to_font(xs)...))));
    }


    constexpr bool has(Font x, Font y) {
        return static_cast<bool>((static_cast<int>(x) & static_cast<int>(y)));
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

        inline string default_mod() const {
            return has(font, Font::Default) ? "0" : "";
        }

        inline string bold_mod() const {
            return has(font, Font::Bold) ? "1" : "";
        }

        inline string underline_mod() const {
            return has(font, Font::Underline) ? "4" : "";
        }

        inline string faint_mod() const {
            return has(font, Font::Faint) ? "2" : "";
        }

        inline string italic_mod() const {
            return has(font, Font::Italic) ? "3" : "";
        }

        inline string hidden_mod() const {
            return has(font, Font::Hidden) ? "8" : "";
        }

        inline string crossed_mod() const {
            return has(font, Font::Crossed) ? "9" : "";
        }


        inline string bg_mod() const {
            return (static_cast<int>(bg) < 11) ? std::to_string(40 + static_cast<int>(bg) - 1) : "";
        }

        inline string fg_mod() const {
            return (static_cast<int>(fg) < 11) ? std::to_string(30 + static_cast<int>(fg) - 1) : "";
        }

        inline std::string to_string() const {
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


    constexpr Style diff(Style const &a, Style const &b = Style::None()) {
        bool keepBG = (a.bg == b.bg);
        bool keepFG = (a.fg == b.fg);
        bool keepFont = (a.font == b.font);

        int l = static_cast<int>(a.font);
        int r = static_cast<int>(b.font);
        bool reset = static_cast<bool>(l & ~r);

        return Style{
            (keepBG and !reset) ? Color::Inherit : b.bg,
            (keepFG and !reset) ? FontColor::Inherit : b.fg,
            (keepFont and !reset) ? Font::Inherit : static_cast<Font>((r & ((l & r) ^ r)) | reset)
        };
    }
}

#endif //AWESOME_VIEWER_STYLE_H

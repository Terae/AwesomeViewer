//
// Created by terae on 30/10/18.
//

#include "Cell.hpp"
#include "VirtualTerminal.hpp"
#include <iostream>

using namespace AwesomeViewer;

int main() {
    VirtualTerminal vt(56, 13);

    StringCell c1(22, 4, "Hello communicator!\nI'm an helper text\nAnd I am a very long string");
    vt.add_cell(c1, "Communicator");

    MapCell<int> c2(27, 7, []() {
        std::map<std::string, int> result;
        result["moving"] = 5;
        result["avoidance"] = 2;
        result["very long name"] = 8;
        return result;
    });
    vt.add_cell(c2, "ModuleManager");

    std::vector<std::pair<Style, std::string>> v;
    StringCell c3(12, 2, {{Style(Font::Italic), "Sarah\n"}, {Style::Default(), "  Connor"}});
    vt.add_cell(c3, "Terminator");

    int timer = 0;
    StringCell c4(7, 2, [&timer]() {
        std::string str_timer = std::to_string(timer / 2);
        return std::string(3 - str_timer.size(), ' ') + str_timer + " s";
    });
    vt.add_cell(c4, "Timer");

    StringCell c5(50, 1, {{Style::Default(), "I need to print a "}, {Style(Font::Italic, Font::Bold, FontColor::Blue), "very long"}, {Style::Default(), " string in this box"}});
    vt.add_cell(c5, "Long container");

    ProgressCell c6(23, 1, [&timer]() -> double {
        return timer;
    });
    vt.add_cell(c6, "Progress bar");

    MapCell<StyleString> c7(19, 1, {{"test", StyleString(Style(Font::Italic), "It works!")}});
    vt.add_cell(c7);

    for (; timer <= 100; ++timer) {
        vt.print();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

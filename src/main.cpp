//
// Created by terae on 30/10/18.
//

#include "Cell.hpp"
#include "VirtualTerminal.hpp"
#include <iostream>

using namespace AwesomeViewer;

int main() {
    VirtualTerminal vt(54, 13);

    StringCell c1(20, 4, "Hello communicator!\nI'm an helper text\nAnd I am a very long string");
    vt.add_cell("Communicator", c1);

    MapCell<int> c2(30, 7, []() {
        std::map<std::string, int> result;
        result["moving"] = 5;
        result["avoidance"] = 2;
        result["very long name"] = 8;
        return result;
    });
    vt.add_cell("ModuleManager", c2);

    StringCell c3(12, 2, "Sarah\n  Connor");
    vt.add_cell("Terminator", c3);

    int timer = 0;
    StringCell c4(5, 2, [&timer]() {
        std::string str_timer = std::to_string(timer / 2);
        return std::string(3 - str_timer.size(), ' ') + str_timer + " s";
    });
    vt.add_cell("Timer", c4);

    StringCell c5(50, 1, "I need to print a very long string in this box.");
    vt.add_cell("Long container", c5);

    ProgressCell c6(23, 1, [&timer]() -> double {
        return timer;
    });
    vt.add_cell("Progress bar", c6);

    for (; timer <= 100; ++timer) {
        vt.print(std::cout);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
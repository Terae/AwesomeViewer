//
// Created by terae on 30/10/18.
//

#include "Cell.h"
#include "VirtualTerminal.h"
#include <iostream>

using namespace AwesomeViewer;

int main() {
    StringCell c1(20, 4, [](){return "Hello communicator!\nI'm an helper text\nAnd I am a very long string";});
    VirtualTerminal vt1(30, 8);
    vt1.add_cell("Communicator", c1);
    vt1.print(std::cout);

    MapCell<int> c2(30, 6, [](){
        std::map<std::string, int> result;
        result["moving"] = 5;
        result["avoidance"] = 2;
        result["very long name"] = 8;
        return result;
    });
    VirtualTerminal vt2(30, 16);
    vt2.add_cell("ModuleManager", c2);
    vt2.print(std::cout);

    /*for (const std::string& str : c1.update()) {
        std::cout << str << std::endl;
    }

    MapCell<int> c2(30, 6, [](){
        std::map<std::string, int> result;
        result["moving"] = 5;
        result["avoidance"] = 2;
        result["very long name"] = 8;
        return result;
    });
    for (const std::string& str : c2.update()) {
        std::cout << str << std::endl;
    }}*/

#if 0
int main() {
    // Layouts de soit une map soit du texte (soit une barre de chargement)
    // Leur taille correspond à l'intérieur du rectangle
    std::map<std::string, int> modules = {
            {"moving", 5},
            {"io", 8},
            {"avoidance", 12}
    };
    Layout layout_modules(30, 16, modules);

    Laout layout_communication(20, 3, "connected\ntype: RS232");

    // conteneur de tous les layouts ; s'occupe de l'affichage des rectangles
    AwesomeViewer aw;
    aw.insert("ModuleManager", layout_modules, 0, 0);
    aw.insert("Communicator", layout_communication, 32, 3);

    // Affichage de tout ; s'il n'y a aucune modification rien ne se passe, sinon modifie juste les pixels appropriés
    VirtualTerminal vt(aw);
    for(;;) {
        vt = vt.flip();
    }

    /*
     * - ModuleManager -------------|
     * |    moving : 5              |
     * |        io : 8              |
     * | avoidance : 12             |- Communicator ----|
     * |           -                | connected         |
     * |           -                | type: RS232       |
     * |           -                |-------------------|
     * |           -                |
     * |           -                |
     * |           -                |
     * |           -                |
     * |           -                |
     * |           -                |
     * |           -                |
     * |           -                |
     * |           -                |
     * |           -                |
     * |           -                |
     * |           -                |
     * |----------------------------|
     */
#endif
}

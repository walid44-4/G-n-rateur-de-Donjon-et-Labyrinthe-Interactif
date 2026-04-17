#include <iostream>

#include "Case.hpp"

int main()
{
    std::cout << "Projet C++ - Generateur de donjon" << std::endl;

    Case* cases[] = {
        new Mur(),
        new Passage(),
        new Tresor(),
        new Monstre(),
        new Piege()
    };

    std::cout << "Symboles des cases : ";
    for (Case* c : cases) {
        std::cout << c->afficher();
    }
    std::cout << std::endl;

    for (Case* c : cases) {
        delete c;
    }

    return 0;
}

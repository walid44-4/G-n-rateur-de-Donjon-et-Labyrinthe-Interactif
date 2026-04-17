#include <iostream>

#include "CaseFactory.hpp"

int main()
{
    std::cout << "Projet C++ - Generateur de donjon" << std::endl;

    Case* cases[] = {
        CaseFactory::creerCase(TypeCase::MUR),
        CaseFactory::creerCase(TypeCase::PASSAGE),
        CaseFactory::creerCase(TypeCase::TRESOR),
        CaseFactory::creerCase(TypeCase::MONSTRE),
        CaseFactory::creerCase(TypeCase::PIEGE)
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

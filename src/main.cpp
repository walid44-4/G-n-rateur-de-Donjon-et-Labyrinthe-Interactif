#include <iostream>

#include "Donjon.hpp"

int main()
{
    std::cout << "Projet C++ - Generateur de donjon" << std::endl;

    Donjon donjon;
    donjon.initialiserGrille(3, 2, TypeCase::MUR);
    donjon.remplacerCase(1, 1, TypeCase::PASSAGE);

    std::cout << "Taille du donjon : "
              << donjon.getLargeur() << "x" << donjon.getHauteur()
              << std::endl;

    std::cout << "Case (0,0) est un mur : "
              << (donjon.estMur(0, 0) ? "oui" : "non")
              << std::endl;

    Case* caseTest = donjon.getCase(1, 1);
    if (caseTest != nullptr) {
        std::cout << "Case (1,1) apres remplacement : '"
                  << caseTest->afficher() << "'"
                  << std::endl;
    }

    return 0;
}

#include <iostream>

#include "Donjon.hpp"

bool toutesLesCasesSontDesMurs(const Donjon& donjon)
{
    for (int y = 0; y < donjon.getHauteur(); ++y) {
        for (int x = 0; x < donjon.getLargeur(); ++x) {
            if (!donjon.estMur(x, y)) {
                return false;
            }
        }
    }

    return true;
}

int main()
{
    std::cout << "Projet C++ - Generateur de donjon" << std::endl;

    Donjon donjon;
    donjon.generer(20, 20);

    std::cout << "Taille du donjon : "
              << donjon.getLargeur() << "x" << donjon.getHauteur()
              << std::endl;

    std::cout << "Entree provisoire : ("
              << donjon.getEntree().first << ", "
              << donjon.getEntree().second << ")"
              << std::endl;

    std::cout << "Sortie provisoire : ("
              << donjon.getSortie().first << ", "
              << donjon.getSortie().second << ")"
              << std::endl;

    std::cout << "Toutes les cases sont des murs : "
              << (toutesLesCasesSontDesMurs(donjon) ? "oui" : "non")
              << std::endl;

    donjon.generer(21, 21);
    std::cout << "Nouvelle taille du donjon : "
              << donjon.getLargeur() << "x" << donjon.getHauteur()
              << std::endl;

    return 0;
}

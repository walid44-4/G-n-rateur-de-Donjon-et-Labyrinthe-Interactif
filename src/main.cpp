#include <iostream>

#include "Aventurier.hpp"
#include "Donjon.hpp"

int main()
{
    std::cout << "Projet C++ - Generateur de donjon" << std::endl;

    Donjon donjon;
    donjon.generer(21, 21);

    std::cout << donjon;

    Aventurier joueur(donjon.getEntree().first, donjon.getEntree().second);
    joueur.afficherStatut();

    joueur.deplacer(donjon.getEntree().first + 1, donjon.getEntree().second);
    joueur.perdreVie(15);

    std::cout << "Apres un test de deplacement et de perte de vie :" << std::endl;
    joueur.afficherStatut();
    std::cout << "Aventurier vivant : " << (joueur.estVivant() ? "oui" : "non") << std::endl;

    return 0;
}

#include <iostream>

#include "Aventurier.hpp"
#include "Donjon.hpp"

bool tenterDeplacement(const Donjon& donjon, Aventurier& joueur, char commande)
{
    int nx = joueur.getX();
    int ny = joueur.getY();

    switch (commande) {
    case 'z':
        --ny;
        break;
    case 'q':
        --nx;
        break;
    case 's':
        ++ny;
        break;
    case 'd':
        ++nx;
        break;
    default:
        std::cout << "Commande inconnue." << std::endl;
        return false;
    }

    if (!donjon.estDansBornes(nx, ny)) {
        std::cout << "Deplacement impossible : hors du donjon." << std::endl;
        return false;
    }

    if (donjon.estMur(nx, ny)) {
        std::cout << "Deplacement impossible : mur." << std::endl;
        return false;
    }

    joueur.deplacer(nx, ny);
    return true;
}

int main()
{
    std::cout << "Projet C++ - Generateur de donjon" << std::endl;

    Donjon donjon;
    donjon.generer(21, 21);

    Aventurier joueur(donjon.getEntree().first, donjon.getEntree().second);
    donjon.afficherAvecAventurier(joueur);
    joueur.afficherStatut();

    std::cout << "Commande (z/q/s/d) : ";
    char commande;
    std::cin >> commande;

    tenterDeplacement(donjon, joueur, commande);

    donjon.afficherAvecAventurier(joueur);
    joueur.afficherStatut();

    return 0;
}

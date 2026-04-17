#include <iostream>
#include <random>

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

void resoudreCase(Donjon& donjon, Aventurier& joueur, int ancienX, int ancienY)
{
    Case* caseCourante = donjon.getCase(joueur.getX(), joueur.getY());

    if (dynamic_cast<Tresor*>(caseCourante) != nullptr) {
        static std::random_device rd;
        static std::mt19937 generateur(rd());
        std::uniform_int_distribution<int> distribution(0, 1);

        joueur.ajouterTresor();

        if (distribution(generateur) == 0) {
            std::cout << "Tresor trouve : une pomme (+15 PV)." << std::endl;
            joueur.soigner(15);
        } else {
            std::cout << "Tresor trouve : une epee." << std::endl;
            joueur.ajouterEpee();
        }

        donjon.remplacerCase(joueur.getX(), joueur.getY(), TypeCase::PASSAGE);
    } else if (dynamic_cast<Monstre*>(caseCourante) != nullptr) {
        std::cout << "Un monstre apparait. Combat (c) ou fuite (f) ? ";
        char choix;
        std::cin >> choix;

        if (choix == 'f') {
            std::cout << "Vous fuyez et revenez a votre ancienne position." << std::endl;
            joueur.deplacer(ancienX, ancienY);
            return;
        }

        if (joueur.utiliserEpee()) {
            std::cout << "Vous utilisez une epee et battez le monstre sans degat." << std::endl;
        } else {
            std::cout << "Vous combattez sans epee et perdez 20 PV." << std::endl;
            joueur.perdreVie(20);
        }

        donjon.remplacerCase(joueur.getX(), joueur.getY(), TypeCase::PASSAGE);
    } else if (dynamic_cast<Piege*>(caseCourante) != nullptr) {
        std::cout << "Vous declenchez un piege (-15 PV)." << std::endl;
        joueur.perdreVie(15);
        donjon.remplacerCase(joueur.getX(), joueur.getY(), TypeCase::PASSAGE);
    }
}

int main()
{
    std::cout << "Projet C++ - Generateur de donjon" << std::endl;

    Donjon donjon;
    donjon.generer(21, 21);

    Aventurier joueur(donjon.getEntree().first, donjon.getEntree().second);
    bool quitter = false;

    while (joueur.estVivant()
           && !quitter
           && std::make_pair(joueur.getX(), joueur.getY()) != donjon.getSortie()) {
        donjon.afficherAvecAventurier(joueur);
        joueur.afficherStatut();

        std::vector<std::pair<int, int>> chemin = donjon.trouverChemin(
            {joueur.getX(), joueur.getY()},
            donjon.getSortie()
        );

        if (!chemin.empty()) {
            std::cout << "Distance a la sortie : " << chemin.size() - 1 << " cases" << std::endl;
        } else {
            std::cout << "Aucun chemin trouve vers la sortie." << std::endl;
        }

        std::cout << "Commande (z/q/s/d, p pour le chemin, x pour quitter) : ";
        char commande;
        if (!(std::cin >> commande)) {
            std::cout << "Lecture de commande interrompue." << std::endl;
            break;
        }

        if (commande == 'x') {
            quitter = true;
            continue;
        }

        if (commande == 'p') {
            if (!chemin.empty()) {
                donjon.afficherAvecChemin(chemin, joueur);
                std::cout << "Distance a la sortie : " << chemin.size() - 1 << " cases" << std::endl;
            } else {
                std::cout << "Aucun chemin trouve vers la sortie." << std::endl;
            }

            continue;
        }

        int ancienX = joueur.getX();
        int ancienY = joueur.getY();
        bool deplacementEffectue = tenterDeplacement(donjon, joueur, commande);

        if (deplacementEffectue) {
            resoudreCase(donjon, joueur, ancienX, ancienY);
        }
    }

    if (!joueur.estVivant()) {
        std::cout << "L'aventurier est mort." << std::endl;
    } else if (std::make_pair(joueur.getX(), joueur.getY()) == donjon.getSortie()) {
        donjon.afficherAvecAventurier(joueur);
        joueur.afficherStatut();
        std::cout << "Victoire, vous avez atteint la sortie !" << std::endl;
    } else if (quitter) {
        std::cout << "Partie quittee." << std::endl;
    }

    return 0;
}

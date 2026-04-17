#include "Donjon.hpp"

#include <algorithm>
#include <iostream>
#include <queue>
#include <random>

Donjon::Donjon()
    : largeur(0), hauteur(0), entree({0, 0}), sortie({0, 0})
{
}

Donjon::~Donjon()
{
    viderGrille();
}

void Donjon::generer(int nouvelleLargeur, int nouvelleHauteur)
{
    if (nouvelleLargeur < 5) {
        std::cout << "Largeur trop petite, ajustee a 5." << std::endl;
        nouvelleLargeur = 5;
    }

    if (nouvelleHauteur < 5) {
        std::cout << "Hauteur trop petite, ajustee a 5." << std::endl;
        nouvelleHauteur = 5;
    }

    if (nouvelleLargeur % 2 == 0) {
        std::cout << "Largeur paire ajustee de " << nouvelleLargeur
                  << " a " << nouvelleLargeur + 1 << "." << std::endl;
        ++nouvelleLargeur;
    }

    if (nouvelleHauteur % 2 == 0) {
        std::cout << "Hauteur paire ajustee de " << nouvelleHauteur
                  << " a " << nouvelleHauteur + 1 << "." << std::endl;
        ++nouvelleHauteur;
    }

    initialiserGrille(nouvelleLargeur, nouvelleHauteur, TypeCase::MUR);
    entree = {1, 1};
    sortie = {largeur - 2, hauteur - 2};

    std::vector<std::vector<bool>> visite(hauteur, std::vector<bool>(largeur, false));
    remplacerCase(entree.first, entree.second, TypeCase::PASSAGE);
    genererLabyrinthe(entree.first, entree.second, visite);
    remplacerCase(sortie.first, sortie.second, TypeCase::PASSAGE);
    placerElements();
}

void Donjon::afficher() const
{
    std::cout << *this;
}

void Donjon::afficherAvecAventurier(const Aventurier& joueur) const
{
    std::cout << '+';
    for (int x = 0; x < largeur; ++x) {
        std::cout << '-';
    }
    std::cout << '+' << std::endl;

    for (int y = 0; y < hauteur; ++y) {
        std::cout << '|';
        for (int x = 0; x < largeur; ++x) {
            if (joueur.getX() == x && joueur.getY() == y) {
                std::cout << '@';
            } else if (entree == std::make_pair(x, y)) {
                std::cout << 'E';
            } else if (sortie == std::make_pair(x, y)) {
                std::cout << 'S';
            } else {
                Case* c = getCase(x, y);
                std::cout << (c != nullptr ? c->afficher() : ' ');
            }
        }
        std::cout << '|' << std::endl;
    }

    std::cout << '+';
    for (int x = 0; x < largeur; ++x) {
        std::cout << '-';
    }
    std::cout << '+' << std::endl;
}

std::vector<std::pair<int, int>> Donjon::trouverChemin(std::pair<int, int> depart,
                                                       std::pair<int, int> arrivee) const
{
    std::vector<std::pair<int, int>> chemin;

    if (!estDansBornes(depart.first, depart.second)
        || !estDansBornes(arrivee.first, arrivee.second)
        || estMur(depart.first, depart.second)
        || estMur(arrivee.first, arrivee.second)) {
        return chemin;
    }

    if (depart == arrivee) {
        chemin.push_back(depart);
        return chemin;
    }

    std::queue<std::pair<int, int>> file;
    std::vector<std::vector<bool>> visite(hauteur, std::vector<bool>(largeur, false));
    std::vector<std::vector<std::pair<int, int>>> parent(
        hauteur,
        std::vector<std::pair<int, int>>(largeur, {-1, -1})
    );

    file.push(depart);
    visite[depart.second][depart.first] = true;

    std::vector<std::pair<int, int>> directions = {
        {0, -1},
        {0, 1},
        {1, 0},
        {-1, 0}
    };

    while (!file.empty()) {
        std::pair<int, int> courant = file.front();
        file.pop();

        if (courant == arrivee) {
            std::pair<int, int> position = arrivee;

            while (position != depart) {
                chemin.insert(chemin.begin(), position);
                position = parent[position.second][position.first];
            }

            chemin.insert(chemin.begin(), depart);
            return chemin;
        }

        for (const std::pair<int, int>& direction : directions) {
            int nx = courant.first + direction.first;
            int ny = courant.second + direction.second;

            if (estDansBornes(nx, ny) && !visite[ny][nx] && !estMur(nx, ny)) {
                visite[ny][nx] = true;
                parent[ny][nx] = courant;
                file.push({nx, ny});
            }
        }
    }

    return chemin;
}

void Donjon::genererLabyrinthe(int x, int y, std::vector<std::vector<bool>>& visite)
{
    visite[y][x] = true;

    std::vector<std::pair<int, int>> directions = {
        {0, -2},
        {0, 2},
        {2, 0},
        {-2, 0}
    };

    static std::random_device rd;
    static std::mt19937 generateur(rd());
    std::shuffle(directions.begin(), directions.end(), generateur);

    for (const std::pair<int, int>& direction : directions) {
        int nx = x + direction.first;
        int ny = y + direction.second;

        if (estCelluleValidePourGeneration(nx, ny) && !visite[ny][nx]) {
            int murX = x + direction.first / 2;
            int murY = y + direction.second / 2;

            remplacerCase(murX, murY, TypeCase::PASSAGE);
            remplacerCase(nx, ny, TypeCase::PASSAGE);
            genererLabyrinthe(nx, ny, visite);
        }
    }
}

bool Donjon::estCelluleValidePourGeneration(int x, int y) const
{
    return x > 0 && x < largeur - 1 && y > 0 && y < hauteur - 1;
}

void Donjon::placerElements()
{
    static std::random_device rd;
    static std::mt19937 generateur(rd());
    std::uniform_int_distribution<int> distribution(1, 100);

    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {
            if (std::make_pair(x, y) == entree || std::make_pair(x, y) == sortie) {
                continue;
            }

            if (!estPassage(x, y)) {
                continue;
            }

            int tirage = distribution(generateur);

            if (tirage <= 5) {
                remplacerCase(x, y, TypeCase::TRESOR);
            } else if (tirage <= 10) {
                remplacerCase(x, y, TypeCase::MONSTRE);
            } else if (tirage <= 13) {
                remplacerCase(x, y, TypeCase::PIEGE);
            }
        }
    }
}

void Donjon::viderGrille()
{
    for (std::vector<Case*>& ligne : grille) {
        for (Case* c : ligne) {
            delete c;
        }
    }

    grille.clear();
    largeur = 0;
    hauteur = 0;
    entree = {0, 0};
    sortie = {0, 0};
}

void Donjon::initialiserGrille(int nouvelleLargeur, int nouvelleHauteur, TypeCase type)
{
    viderGrille();

    if (nouvelleLargeur <= 0 || nouvelleHauteur <= 0) {
        return;
    }

    largeur = nouvelleLargeur;
    hauteur = nouvelleHauteur;
    grille.resize(hauteur);

    for (int y = 0; y < hauteur; ++y) {
        grille[y].resize(largeur, nullptr);
        for (int x = 0; x < largeur; ++x) {
            grille[y][x] = CaseFactory::creerCase(type);
        }
    }
}

void Donjon::remplacerCase(int x, int y, TypeCase type)
{
    if (!estDansBornes(x, y)) {
        return;
    }

    delete grille[y][x];
    grille[y][x] = CaseFactory::creerCase(type);
}

bool Donjon::estDansBornes(int x, int y) const
{
    return x >= 0 && x < largeur && y >= 0 && y < hauteur;
}

bool Donjon::estMur(int x, int y) const
{
    if (!estDansBornes(x, y)) {
        return false;
    }

    return dynamic_cast<Mur*>(grille[y][x]) != nullptr;
}

bool Donjon::estPassage(int x, int y) const
{
    if (!estDansBornes(x, y)) {
        return false;
    }

    return dynamic_cast<Passage*>(grille[y][x]) != nullptr;
}

Case* Donjon::getCase(int x, int y) const
{
    if (!estDansBornes(x, y)) {
        return nullptr;
    }

    return grille[y][x];
}

int Donjon::getLargeur() const
{
    return largeur;
}

int Donjon::getHauteur() const
{
    return hauteur;
}

std::pair<int, int> Donjon::getEntree() const
{
    return entree;
}

std::pair<int, int> Donjon::getSortie() const
{
    return sortie;
}

void Donjon::setEntree(int x, int y)
{
    if (estDansBornes(x, y)) {
        entree = {x, y};
    }
}

void Donjon::setSortie(int x, int y)
{
    if (estDansBornes(x, y)) {
        sortie = {x, y};
    }
}

std::ostream& operator<<(std::ostream& os, const Donjon& donjon)
{
    os << '+';
    for (int x = 0; x < donjon.getLargeur(); ++x) {
        os << '-';
    }
    os << '+' << std::endl;

    for (int y = 0; y < donjon.getHauteur(); ++y) {
        os << '|';
        for (int x = 0; x < donjon.getLargeur(); ++x) {
            if (donjon.getEntree() == std::make_pair(x, y)) {
                os << 'E';
            } else if (donjon.getSortie() == std::make_pair(x, y)) {
                os << 'S';
            } else {
                Case* c = donjon.getCase(x, y);
                os << (c != nullptr ? c->afficher() : ' ');
            }
        }
        os << '|' << std::endl;
    }

    os << '+';
    for (int x = 0; x < donjon.getLargeur(); ++x) {
        os << '-';
    }
    os << '+' << std::endl;

    return os;
}

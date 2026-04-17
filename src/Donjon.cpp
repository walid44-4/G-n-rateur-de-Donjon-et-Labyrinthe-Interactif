#include "Donjon.hpp"

Donjon::Donjon()
    : largeur(0), hauteur(0), entree({0, 0}), sortie({0, 0})
{
}

Donjon::~Donjon()
{
    viderGrille();
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

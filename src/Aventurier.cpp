#include "Aventurier.hpp"

#include <iostream>

Aventurier::Aventurier(int departX, int departY)
    : x(departX),
      y(departY),
      pointsDeVie(100),
      pointsDeVieMax(100),
      tresorsRamasses(0)
{
    inventaire["pomme"] = 0;
    inventaire["epee"] = 0;
}

void Aventurier::deplacer(int nx, int ny)
{
    x = nx;
    y = ny;
}

void Aventurier::perdreVie(int degats)
{
    pointsDeVie -= degats;
    if (pointsDeVie < 0) {
        pointsDeVie = 0;
    }
}

bool Aventurier::estVivant() const
{
    return pointsDeVie > 0;
}

void Aventurier::afficherStatut() const
{
    std::cout << "Position : (" << x << ", " << y << ")" << std::endl;
    std::cout << "Sante : " << pointsDeVie << "/" << pointsDeVieMax << std::endl;
    std::cout << "Tresors ramasses : " << tresorsRamasses << std::endl;
    std::cout << "Inventaire :" << std::endl;

    for (const auto& element : inventaire) {
        std::cout << "  " << element.first << " : " << element.second << std::endl;
    }
}

int Aventurier::getX() const
{
    return x;
}

int Aventurier::getY() const
{
    return y;
}

int Aventurier::getPointsDeVie() const
{
    return pointsDeVie;
}

int Aventurier::getPointsDeVieMax() const
{
    return pointsDeVieMax;
}

int Aventurier::getTresorsRamasses() const
{
    return tresorsRamasses;
}

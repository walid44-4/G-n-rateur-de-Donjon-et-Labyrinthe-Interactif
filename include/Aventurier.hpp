#ifndef AVENTURIER_HPP
#define AVENTURIER_HPP

#include <map>
#include <string>

class Aventurier {
private:
    int x;
    int y;
    int pointsDeVie;
    int pointsDeVieMax;
    int tresorsRamasses;
    std::map<std::string, int> inventaire;

public:
    Aventurier(int departX, int departY);

    void deplacer(int nx, int ny);
    void perdreVie(int degats);
    void soigner(int points);
    void ajouterEpee();
    bool aUneEpee() const;
    bool utiliserEpee();
    void ajouterTresor();
    bool estVivant() const;
    void afficherStatut() const;

    int getX() const;
    int getY() const;
    int getPointsDeVie() const;
    int getPointsDeVieMax() const;
    int getTresorsRamasses() const;
};

#endif

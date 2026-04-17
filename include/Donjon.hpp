#ifndef DONJON_HPP
#define DONJON_HPP

#include <iosfwd>
#include <utility>
#include <vector>

#include "Aventurier.hpp"
#include "Case.hpp"
#include "CaseFactory.hpp"

class Donjon {
private:
    std::vector<std::vector<Case*>> grille;
    int largeur;
    int hauteur;
    std::pair<int, int> entree;
    std::pair<int, int> sortie;

    void genererLabyrinthe(int x, int y, std::vector<std::vector<bool>>& visite);
    bool estCelluleValidePourGeneration(int x, int y) const;
    void placerElements();

public:
    Donjon();
    ~Donjon();

    void generer(int nouvelleLargeur, int nouvelleHauteur);
    void afficher() const;
    void afficherAvecAventurier(const Aventurier& joueur) const;
    void viderGrille();
    void initialiserGrille(int nouvelleLargeur, int nouvelleHauteur, TypeCase type);
    void remplacerCase(int x, int y, TypeCase type);

    bool estDansBornes(int x, int y) const;
    bool estMur(int x, int y) const;
    bool estPassage(int x, int y) const;
    Case* getCase(int x, int y) const;

    int getLargeur() const;
    int getHauteur() const;
    std::pair<int, int> getEntree() const;
    std::pair<int, int> getSortie() const;

    void setEntree(int x, int y);
    void setSortie(int x, int y);
};

std::ostream& operator<<(std::ostream& os, const Donjon& donjon);

#endif

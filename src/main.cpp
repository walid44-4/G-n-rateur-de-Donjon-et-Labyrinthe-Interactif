#include <iostream>

#include "Donjon.hpp"

int main()
{
    std::cout << "Projet C++ - Generateur de donjon" << std::endl;

    Donjon donjon;
    donjon.generer(7, 5);

    std::cout << donjon;
    donjon.afficher();

    return 0;
}

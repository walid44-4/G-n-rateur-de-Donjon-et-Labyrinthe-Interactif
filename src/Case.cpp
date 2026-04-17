#include "Case.hpp"

char Mur::afficher()
{
    return '#';
}

char Passage::afficher()
{
    return ' ';
}

char Tresor::afficher()
{
    return '+';
}

char Monstre::afficher()
{
    return 'M';
}

char Piege::afficher()
{
    return 'T';
}

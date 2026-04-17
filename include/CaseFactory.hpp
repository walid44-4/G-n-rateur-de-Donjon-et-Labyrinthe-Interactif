#ifndef CASEFACTORY_HPP
#define CASEFACTORY_HPP

#include "Case.hpp"

enum class TypeCase {
    MUR,
    PASSAGE,
    TRESOR,
    MONSTRE,
    PIEGE
};

class CaseFactory {
public:
    static Case* creerCase(TypeCase type);
};

#endif

#ifndef CASE_HPP
#define CASE_HPP

class Case {
public:
    virtual char afficher() = 0;
    virtual ~Case() = default;
};

class Mur : public Case {
public:
    char afficher() override;
};

class Passage : public Case {
public:
    char afficher() override;
};

class Tresor : public Case {
public:
    char afficher() override;
};

class Monstre : public Case {
public:
    char afficher() override;
};

class Piege : public Case {
public:
    char afficher() override;
};

#endif

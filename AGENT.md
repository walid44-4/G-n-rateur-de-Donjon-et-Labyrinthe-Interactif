# AGENT.md - Projet C++ Donjon / Labyrinthe

## Objectif du projet

Ce projet doit rester fidele au sujet PDF `2526_C___projet.pdf`.

Il s'agit d'un projet C++ etudiant : generateur de donjon/labyrinthe interactif en console, avec generation aleatoire, cases speciales, aventurier, affichage ASCII, chemin optimal par BFS et utilisation obligatoire du pattern Factory.

Ne pas moderniser inutilement l'architecture si cela eloigne le rendu de l'enonce. La priorite est un code clair, credible pour un rendu etudiant, conforme aux classes, signatures et contraintes du sujet.

## Regles de travail pour Codex

- Ne pas commencer par coder sans avoir explique l'etape en cours.
- Avancer progressivement, par petites modifications coherentes.
- Respecter les choix valides par l'utilisateur.
- Ne pas remplacer l'architecture attendue par une architecture trop moderne.
- Garder un style C++ simple, lisible et pedagogique.
- Avant une modification importante, expliquer ce qui va etre modifie et pourquoi.
- Ne pas ajouter de fonctionnalites bonus tant que les fonctionnalites obligatoires ne sont pas terminees.
- A la fin de chaque etape validee du plan, faire un commit Git propre avec un message clair et descriptif, puis faire un push vers le depot distant.
- Ne pas modifier le PDF du sujet.
- Ne pas supprimer les fichiers existants sans demande explicite.

## Choix d'architecture valides

### Structure du projet

Utiliser une structure simple :

```text
include/
src/
Makefile
```

Le dossier `data/` n'est pas obligatoire pour l'instant. Il pourra etre ajoute uniquement si des fichiers d'exemple de donjons generes sont fournis.

### Build

Utiliser un `Makefile` simple.

Le projet doit rester compilable facilement en ligne de commande.

### Gestion memoire

Utiliser des pointeurs bruts `Case*`, comme dans le sujet.

`Donjon` contient :

```cpp
vector<vector<Case*>> grille;
```

Comme des pointeurs bruts sont utilises, il faut gerer correctement la memoire :

- liberer toutes les cases dans le destructeur de `Donjon`;
- eviter les fuites lors du remplacement d'une case;
- ne pas utiliser `unique_ptr` pour la grille principale, afin de rester proche de l'enonce.

## Classes obligatoires

### Case

Classe abstraite de base.

Elle doit contenir au minimum :

```cpp
virtual char afficher() = 0;
virtual ~Case() = default;
```

### Sous-classes de Case

Implementer au minimum :

- `Mur`
- `Passage`
- `Tresor`
- `Monstre`
- `Piege`

Symboles ASCII :

- mur : `#`
- passage : espace `' '`
- tresor : `+`
- monstre : `M`
- piege : `T`
- aventurier : `@`
- entree : `E`
- sortie : `S`
- chemin temporaire BFS : `.`

L'entree et la sortie ne sont pas des classes supplementaires. Ce sont de simples coordonnees situees sur des passages.

### CaseFactory

Le pattern Factory est obligatoire.

Definir un `enum TypeCase`, par exemple :

```cpp
enum class TypeCase {
    MUR,
    PASSAGE,
    TRESOR,
    MONSTRE,
    PIEGE
};
```

La creation des cases doit passer par :

```cpp
static Case* creerCase(TypeCase type);
```

`Donjon` ne doit pas instancier directement `new Mur`, `new Tresor`, etc. La creation doit etre centralisee dans `CaseFactory`.

### Donjon

Classe principale pour la grille.

Elle doit rester proche de la structure du sujet :

```cpp
class Donjon {
    vector<vector<Case*>> grille;
public:
    void generer(int largeur, int hauteur);
    void afficher();
    vector<pair<int,int>> trouverChemin(/* ... */);
};
```

Adaptations acceptees :

- ajouter un constructeur et un destructeur;
- ajouter des getters simples pour largeur, hauteur, entree, sortie;
- ajouter une methode pour verifier les bornes;
- ajouter une methode pour tester si une case est un mur;
- ajouter une methode pour remplacer une case proprement;
- ajouter des parametres a `trouverChemin` si necessaire.

Ne pas transformer `Donjon` en architecture complexe.

### Aventurier

La classe `Aventurier` doit contenir :

- position `(x, y)`;
- points de vie;
- inventaire;
- tresors ramasses;
- nombre d'epees;

L'inventaire peut utiliser `map` pour respecter naturellement l'exigence du sujet sur les structures STL.

Exemple acceptable :

```cpp
map<string, int> inventaire;
```

Fonctions attendues :

- `deplacer(int nx, int ny)`;
- `resoudreCase(Case* c)` ou variante recevant aussi le donjon si le remplacement de case est necessaire;
- `estVivant()`;
- `afficherStatut()`;

## Generation du labyrinthe

Utiliser Recursive Backtracking, comme dans le pseudo-code du sujet.

Regles :

- taille par defaut : `21x21`;
- accepter les tailles paires;
- si une taille paire est fournie, l'ajuster automatiquement a la taille impaire superieure;
- afficher un message expliquant l'ajustement;
- initialiser toute la grille avec des murs via `CaseFactory`;
- demarrer la generation a `(1, 1)`;
- creuser par sauts de 2 cases;
- casser le mur intermediaire;
- melanger les directions aleatoirement;
- garantir un chemin entre l'entree et la sortie.

Entree et sortie :

- entree : symbole `E`;
- sortie : symbole `S`;
- simples coordonnees sur des passages;
- ne pas creer de classes `Entree` ou `Sortie`.

Placement des elements :

- uniquement sur des passages;
- tresors, monstres et pieges places aleatoirement;
- utiliser les probabilites du pseudo-code si possible :
  - environ 5% tresors;
  - environ 5% monstres;
  - environ 3% pieges.

## Exploration

Lecture clavier portable avec validation par Entree.

Commandes conseillees :

- `z` : haut;
- `q` : gauche;
- `s` : bas;
- `d` : droite;
- `p` : afficher temporairement le chemin optimal et la distance;
- eventuellement `x` : quitter.

Regles :

- si la case cible est un mur, le deplacement est refuse;
- sinon, l'aventurier se deplace;
- l'effet de la case est ensuite resolu;
- la partie continue tant que l'aventurier est vivant et n'a pas atteint la sortie.

## Effets des cases

### Tresor

Un tresor contient aleatoirement :

- soit une pomme;
- soit une epee.

Effets :

- pomme : rend 15 points de vie;
- epee : ajoute une epee a l'inventaire;
- apres ramassage, la case devient un passage.

### Monstre

Quand l'aventurier rencontre un monstre, il choisit :

- combat;
- fuite.

Combat :

- si l'aventurier possede une epee :
  - il tue le monstre;
  - il ne recoit aucun degat;
  - il perd une epee;
- sinon :
  - il recoit 20 points de degats fixes;
  - le monstre est vaincu;

Apres combat, la case devient un passage.

Fuite :

- l'aventurier retourne a son ancienne case;
- le monstre reste en place.

### Piege

Effet :

- retire 15 points de vie;
- disparait ensuite;
- la case devient un passage.

## BFS

Utiliser BFS pour calculer le chemin le plus court.

Regles :

- utiliser `queue`;
- utiliser un tableau 2D de visites;
- utiliser un tableau 2D de parents;
- explorer les 4 directions;
- traverser toutes les cases non mur;
- les tresors, monstres et pieges sont donc traversables pour BFS;
- reconstruire le chemin depuis la sortie vers le depart;
- retourner un `vector<pair<int,int>>`.

Affichage :

- quand l'utilisateur tape `p`, afficher le chemin avec `.` sur une copie temporaire de la grille;
- afficher aussi la distance;
- ne pas rendre le chemin permanent dans la grille.

## Affichage console

L'affichage doit rester simple et lisible en ASCII.

Afficher :

- la grille;
- la position;
- la sante;
- l'inventaire;
- le nombre de tresors ramasses;
- la distance a la sortie si le chemin est demande ou calcule.

La surcharge d'operateur est obligatoire au moins pour `Donjon`.

Preferer :

```cpp
ostream& operator<<(ostream& os, const Donjon& donjon);
```

Une surcharge pour `Aventurier` est aussi acceptable si elle reste simple.

## STL obligatoire

Utiliser naturellement :

- `vector` pour la grille et les chemins;
- `queue` pour BFS;
- `map` pour l'inventaire ou les commandes.

Ne pas ajouter une `map` artificielle si une utilisation simple et claire existe deja.

## Fonctionnalites bonus

Pour l'instant, viser uniquement les fonctionnalites obligatoires.

Ne pas implementer maintenant :

- sauvegarde / chargement complet;
- plusieurs aventuriers;
- autre algorithme de generation;
- systeme de combat avance avec des;
- statistiques avancees.

Les fichiers `.txt` d'exemples de donjons generes pourront etre fournis en livrable, sans forcement creer une fonctionnalite complete de sauvegarde dans le programme.

## Plan d'implementation recommande

1. Creer la structure `include/`, `src/` et `Makefile`.
2. Implementer `Case`, les sous-classes et `CaseFactory`.
3. Implementer `Donjon` avec gestion memoire, affichage et acces de base.
4. Implementer Recursive Backtracking.
5. Ajouter entree, sortie et placement aleatoire des elements.
6. Implementer `Aventurier` et son inventaire.
7. Implementer la boucle de jeu portable.
8. Implementer les effets des cases.
9. Implementer BFS et l'affichage temporaire du chemin.
10. Tester la compilation et plusieurs parties.
11. Generer quelques exemples de donjons `.txt`.
12. Preparer le rapport PDF a la fin.

## Points a surveiller pendant l'implementation

- Ne pas oublier de liberer les anciennes cases lors d'un remplacement.
- Eviter les erreurs d'indices entre `x/y`, `ligne/colonne`, `largeur/hauteur`.
- Ne pas placer de tresor, monstre ou piege sur l'entree ou la sortie.
- Ne pas ecraser visuellement `@`, `E` ou `S` avec le chemin BFS.
- Garder la boucle de jeu simple et comprehensible.
- Ne pas compliquer l'architecture avec des abstractions non demandees.
- Verifier que le BFS fonctionne meme si le joueur est deja sur la sortie.
- Verifier que les tailles paires sont bien ajustees et annoncees.

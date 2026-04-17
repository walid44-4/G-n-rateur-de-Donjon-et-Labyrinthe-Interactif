# Plan d'implementation

Ce plan suit le sujet `2526_C___projet.pdf` et les decisions de clarification deja validees. L'objectif est de construire le projet progressivement, avec une architecture simple, fidele a l'enonce, credible pour un rendu etudiant, et testable etape par etape.

Choix a respecter pendant toute l'implementation :

- structure simple avec `include/`, `src/` et `Makefile`;
- utilisation de `Case*` et de `vector<vector<Case*>>`;
- signatures proches de celles du sujet;
- pattern Factory obligatoire avec `CaseFactory`;
- generation par Recursive Backtracking;
- BFS pour le chemin optimal;
- lecture clavier portable avec validation par Entree;
- pas de bonus tant que les fonctionnalites obligatoires ne sont pas terminees.

## Etape 1 - Mettre en place la structure du projet

### Objectif

Creer une base de projet claire et minimale, compilable avec un `Makefile`, sans encore implementer la logique complete du jeu.

### A implementer

- Creer les dossiers :
  - `include/`
  - `src/`
- Creer un `Makefile` simple.
- Prevoir une cible principale de compilation.
- Prevoir une cible `clean`.
- Creer un `main.cpp` minimal qui permettra ensuite de lancer le jeu.

### Fichiers concernes

- `Makefile`
- `src/main.cpp`

### Dependances

Aucune dependance interne. C'est la premiere etape.

### Points de vigilance

- Garder le `Makefile` simple et lisible.
- Eviter une configuration trop complexe.
- Utiliser une version C++ raisonnable, par exemple `-std=c++17`, sans introduire d'architecture moderne qui contredit le choix `Case*`.
- Ne pas commencer a implementer la logique du donjon dans cette etape.

### Tests a faire

- Lancer `make`.
- Verifier que l'executable est genere.
- Lancer l'executable.
- Lancer `make clean`.
- Relancer `make` apres nettoyage.

## Etape 2 - Implementer la hierarchie des cases

### Objectif

Mettre en place la base objet du projet avec la classe abstraite `Case` et ses cinq sous-classes obligatoires.

### A implementer

- Classe abstraite `Case`.
- Methode virtuelle pure :
  ```cpp
  virtual char afficher() = 0;
  ```
- Destructeur virtuel :
  ```cpp
  virtual ~Case() = default;
  ```
- Classes concretes :
  - `Mur`
  - `Passage`
  - `Tresor`
  - `Monstre`
  - `Piege`
- Chaque classe concrete doit redefinir `afficher()`.
- Symboles :
  - `Mur` : `#`
  - `Passage` : espace `' '`
  - `Tresor` : `+`
  - `Monstre` : `M`
  - `Piege` : `T`

### Fichiers concernes

- `include/Case.hpp`
- `src/Case.cpp`

Selon le niveau de simplicite souhaite, les sous-classes peuvent etre dans les memes fichiers que `Case`, car elles sont petites.

### Dependances

- Etape 1 terminee pour disposer d'un projet compilable.

### Points de vigilance

- `Case` doit bien etre abstraite.
- Le destructeur doit etre virtuel pour permettre la suppression via `Case*`.
- Ne pas ajouter encore de logique de jeu dans les cases.
- Ne pas creer de classes `Entree` ou `Sortie`, car l'entree et la sortie sont de simples coordonnees.

### Tests a faire

- Instancier temporairement chaque type de case dans `main.cpp`.
- Appeler `afficher()` et verifier les caracteres retournes.
- Supprimer les objets via des pointeurs `Case*` pour verifier que la hierarchie est utilisable polymorphiquement.
- Compiler sans warning important.

## Etape 3 - Implementer TypeCase et CaseFactory

### Objectif

Centraliser la creation des cases avec le pattern Factory, comme demande explicitement dans le sujet.

### A implementer

- Definir l'enum :
  ```cpp
  enum class TypeCase {
      MUR,
      PASSAGE,
      TRESOR,
      MONSTRE,
      PIEGE
  };
  ```
- Creer la classe `CaseFactory`.
- Implementer :
  ```cpp
  static Case* creerCase(TypeCase type);
  ```
- Faire en sorte que la creation de `Mur`, `Passage`, `Tresor`, `Monstre` et `Piege` passe par cette methode.

### Fichiers concernes

- `include/CaseFactory.hpp`
- `src/CaseFactory.cpp`
- `include/Case.hpp`
- `src/Case.cpp`

### Dependances

- Etape 2 terminee.

### Points de vigilance

- `Donjon` ne devra pas appeler directement `new Mur`, `new Passage`, etc.
- La Factory est le seul endroit ou les classes concretes sont instanciees.
- Le retour doit rester un `Case*`, conformement au choix valide.
- Prevoir un comportement simple pour les cas invalides, meme si tous les `TypeCase` sont normalement couverts.

### Tests a faire

- Creer chaque type de case avec `CaseFactory::creerCase`.
- Verifier que `afficher()` retourne le bon symbole.
- Supprimer chaque pointeur apres test.
- Verifier qu'aucun `new Mur` ou equivalent n'est utilise ailleurs que dans la Factory.

## Etape 4 - Creer la classe Donjon et sa gestion memoire

### Objectif

Mettre en place la classe `Donjon`, sa grille de `Case*`, ses dimensions, ses coordonnees d'entree/sortie, et la liberation correcte de la memoire.

### A implementer

- Classe `Donjon`.
- Attribut principal :
  ```cpp
  vector<vector<Case*>> grille;
  ```
- Attributs pour :
  - largeur;
  - hauteur;
  - position de l'entree;
  - position de la sortie.
- Constructeur simple.
- Destructeur qui libere toutes les cases de la grille.
- Methode de nettoyage interne, par exemple `viderGrille()`.
- Methode de remplacement propre d'une case, par exemple :
  - supprimer l'ancienne case;
  - affecter la nouvelle case creee via `CaseFactory`.
- Methodes utilitaires simples :
  - verifier si une position est dans les bornes;
  - verifier si une case est un mur;
  - obtenir une case a une position donnee;
  - obtenir largeur/hauteur/entree/sortie.

### Fichiers concernes

- `include/Donjon.hpp`
- `src/Donjon.cpp`
- `include/Case.hpp`
- `include/CaseFactory.hpp`

### Dependances

- Etape 3 terminee.

### Points de vigilance

- Eviter les fuites memoire.
- Eviter les doubles suppressions.
- Lors d'une nouvelle generation, nettoyer l'ancienne grille avant d'en creer une nouvelle.
- Garder les coordonnees coherentes : definir clairement si `x` correspond a la colonne et `y` a la ligne.
- Ne pas encore implementer tout l'algorithme de generation si cela rend l'etape trop grosse.

### Tests a faire

- Creer un donjon vide ou initialise manuellement.
- Remplir quelques cases via la Factory.
- Remplacer une case et verifier que l'affichage change.
- Detruire le donjon en fin de programme sans crash.
- Compiler avec warnings actives si possible.

## Etape 5 - Initialiser la grille et gerer les tailles

### Objectif

Permettre a `Donjon::generer(int largeur, int hauteur)` de preparer une grille remplie de murs, avec gestion des tailles paires.

### A implementer

- Methode :
  ```cpp
  void generer(int largeur, int hauteur);
  ```
- Si largeur ou hauteur est paire, l'ajuster a la taille impaire superieure.
- Afficher un message indiquant l'ajustement.
- Initialiser toute la grille avec des `Mur` crees par `CaseFactory`.
- Positionner provisoirement :
  - entree;
  - sortie.

### Fichiers concernes

- `include/Donjon.hpp`
- `src/Donjon.cpp`

### Dependances

- Etape 4 terminee.

### Points de vigilance

- Ne jamais ecrire hors des bornes de la grille.
- Ne pas oublier de nettoyer une ancienne grille si `generer()` est appelee plusieurs fois.
- Prevoir une taille minimale raisonnable, par exemple au moins `5x5`, pour que l'algorithme de generation ait du sens.
- Ne pas placer encore les elements speciaux dans cette etape.

### Tests a faire

- Generer un donjon `21x21`.
- Generer un donjon `20x20` et verifier qu'il devient `21x21`.
- Generer un donjon avec deux dimensions paires.
- Verifier que toutes les cases sont des murs.
- Verifier que le programme ne fuit pas ou ne crash pas lors de plusieurs appels a `generer()`.

## Etape 6 - Implementer l'affichage ASCII du Donjon

### Objectif

Afficher proprement la grille en console et mettre en place la surcharge d'operateur demandee.

### A implementer

- Methode :
  ```cpp
  void afficher();
  ```
- Surcharge :
  ```cpp
  ostream& operator<<(ostream& os, const Donjon& donjon);
  ```
- Affichage des cases avec `Case::afficher()`.
- Affichage de l'entree avec `E`.
- Affichage de la sortie avec `S`.
- Optionnel a ce stade : bordures ASCII simples autour de la grille.

### Fichiers concernes

- `include/Donjon.hpp`
- `src/Donjon.cpp`
- `src/main.cpp`

### Dependances

- Etape 5 terminee.

### Points de vigilance

- Ne pas modifier la grille uniquement pour afficher `E` et `S`.
- `E` et `S` doivent etre des symboles d'affichage lies aux coordonnees, pas des classes de cases.
- S'assurer que l'affichage reste lisible pour des tailles comme `21x21`.
- Garder `afficher()` et `operator<<` coherents, idealement l'un utilise l'autre.

### Tests a faire

- Generer une grille remplie de murs et l'afficher.
- Verifier que `E` et `S` apparaissent aux bonnes positions.
- Verifier que les bordures, si elles existent, ont la bonne largeur.
- Verifier que `cout << donjon` fonctionne.

## Etape 7 - Implementer Recursive Backtracking

### Objectif

Generer un vrai labyrinthe coherent a partir d'une grille initialement remplie de murs.

### A implementer

- Methode interne recursive, par exemple :
  ```cpp
  void genererLabyrinthe(int x, int y, vector<vector<bool>>& visite);
  ```
- Directions :
  - nord;
  - sud;
  - est;
  - ouest.
- Melange aleatoire des directions.
- Deplacement par sauts de deux cases.
- Creation de passages via `CaseFactory`.
- Cassage du mur intermediaire entre deux cases.
- Depart de generation a `(1, 1)`.
- Placement de l'entree et de la sortie sur des passages accessibles.

### Fichiers concernes

- `include/Donjon.hpp`
- `src/Donjon.cpp`

### Dependances

- Etape 6 terminee.

### Points de vigilance

- Bien distinguer largeur/hauteur et x/y.
- Ne pas visiter les bordures si elles doivent rester des murs.
- S'assurer que l'entree et la sortie sont connectees au labyrinthe.
- Eviter une recursion sur des positions hors bornes.
- Utiliser la Factory pour remplacer les murs par des passages.
- La generation doit produire un chemin garanti entre entree et sortie.

### Tests a faire

- Generer plusieurs labyrinthes `21x21` et verifier visuellement qu'ils changent.
- Verifier qu'il existe des passages.
- Verifier que les bordures restent coherentes.
- Verifier que l'entree et la sortie sont sur des passages accessibles.
- Faire un test manuel sur une petite grille impaire, par exemple `5x5` ou `7x7`.

## Etape 8 - Placer les elements speciaux

### Objectif

Ajouter aleatoirement tresors, monstres et pieges sur des cases de passage, apres generation du labyrinthe.

### A implementer

- Methode interne, par exemple :
  ```cpp
  void placerElements();
  ```
- Parcourir les passages.
- Tirer un nombre aleatoire entre 0 et 100.
- Appliquer les probabilites du sujet :
  - environ 5% tresors;
  - environ 5% monstres;
  - environ 3% pieges.
- Remplacer les passages par les bonnes cases via `CaseFactory`.

### Fichiers concernes

- `include/Donjon.hpp`
- `src/Donjon.cpp`

### Dependances

- Etape 7 terminee.

### Points de vigilance

- Ne pas placer d'element sur l'entree.
- Ne pas placer d'element sur la sortie.
- Ne placer des elements que sur des passages.
- Ne pas casser la connectivite du labyrinthe : les cases speciales doivent rester franchissables.
- Supprimer proprement l'ancienne case lors d'un remplacement.

### Tests a faire

- Generer plusieurs donjons et verifier la presence aleatoire de `+`, `M`, `T`.
- Verifier que `E` et `S` ne sont pas remplaces.
- Verifier que le nombre d'elements reste raisonnable.
- Verifier que les cases speciales ne sont pas considerees comme des murs.

## Etape 9 - Creer la classe Aventurier

### Objectif

Representer l'etat du joueur : position, sante et inventaire.

### A implementer

- Classe `Aventurier`.
- Attributs :
  - position `x, y`;
  - points de vie;
  - points de vie maximum;
  - inventaire avec `map<string, int>`;
  - compteur de tresors ramasses.
- Methodes :
  ```cpp
  void deplacer(int nx, int ny);
  bool estVivant() const;
  void afficherStatut() const;
  ```
- Eventuellement une surcharge simple de `operator<<` pour afficher le statut.

### Fichiers concernes

- `include/Aventurier.hpp`
- `src/Aventurier.cpp`
- `src/main.cpp`

### Dependances

- Etape 8 terminee pour connaitre les positions d'entree/sortie du donjon.

### Points de vigilance

- Initialiser l'aventurier sur l'entree.
- Ne pas melanger la logique de deplacement avec la logique de resolution des cases trop tot.
- Garder l'inventaire simple.
- Utiliser `map` naturellement, par exemple pour stocker `"pomme"`, `"epee"` ou directement `"epee"` avec un compteur.

### Tests a faire

- Creer un aventurier a une position donnee.
- Afficher son statut.
- Le deplacer vers une nouvelle position valide.
- Verifier que `estVivant()` retourne `true` avec une sante positive.
- Simuler une perte de vie et verifier le changement de statut.

## Etape 10 - Implementer les deplacements dans le donjon

### Objectif

Permettre a l'aventurier de se deplacer dans les quatre directions, avec refus des murs.

### A implementer

- Calcul de position cible selon une commande :
  - `z` : haut;
  - `q` : gauche;
  - `s` : bas;
  - `d` : droite.
- Verification des bornes.
- Verification que la case cible n'est pas un mur.
- Deplacement avec `Aventurier::deplacer`.
- Message simple si le deplacement est impossible.

### Fichiers concernes

- `include/Aventurier.hpp`
- `src/Aventurier.cpp`
- `include/Donjon.hpp`
- `src/Donjon.cpp`
- `src/main.cpp`

### Dependances

- Etape 9 terminee.

### Points de vigilance

- Le deplacement ne doit pas traverser les murs.
- Le joueur ne doit pas sortir de la grille.
- Les cases `Tresor`, `Monstre` et `Piege` sont franchissables.
- Conserver l'ancienne position, car elle sera necessaire pour la fuite contre un monstre.
- Ne pas encore resoudre tous les effets si l'etape devient trop grosse.

### Tests a faire

- Essayer de se deplacer dans un mur : position inchangee.
- Essayer de se deplacer dans un passage : position mise a jour.
- Essayer de sortir des bornes : position inchangee.
- Verifier que l'affichage place `@` au bon endroit.

## Etape 11 - Resoudre les cases speciales

### Objectif

Appliquer les effets des tresors, monstres et pieges lorsque l'aventurier arrive sur une case speciale.

### A implementer

- Fonction ou methode de resolution, par exemple :
  ```cpp
  void resoudreCase(Donjon& donjon, Aventurier& joueur, int ancienX, int ancienY);
  ```
- Tresor :
  - contenu aleatoire : pomme ou epee;
  - pomme : +15 points de vie;
  - epee : +1 epee dans l'inventaire;
  - incrementer les tresors ramasses;
  - remplacer la case par un passage.
- Monstre :
  - demander combat ou fuite;
  - combat avec epee : pas de degat, perdre une epee, monstre supprime;
  - combat sans epee : perdre 20 points de vie, monstre supprime;
  - fuite : retour a l'ancienne position, monstre conserve.
- Piege :
  - perdre 15 points de vie;
  - remplacer la case par un passage.

### Fichiers concernes

- `include/Aventurier.hpp`
- `src/Aventurier.cpp`
- `include/Donjon.hpp`
- `src/Donjon.cpp`
- `src/main.cpp`

Selon l'organisation retenue, la resolution peut etre dans une fonction de `main.cpp` au debut, ou dans une classe de jeu simple plus tard.

### Dependances

- Etape 10 terminee.

### Points de vigilance

- Remplacer les cases via une methode propre de `Donjon`, pas par suppression manuelle dispersee.
- Ne pas oublier que la fuite annule le deplacement.
- Ne pas supprimer le monstre si le joueur fuit.
- Ne pas depasser la sante maximale si une pomme soigne.
- Verifier la mort du joueur apres piege ou combat.
- Le choix combat/fuite doit rester portable avec lecture par Entree.

### Tests a faire

- Forcer temporairement une case tresor devant le joueur et verifier pomme/epee.
- Verifier qu'un tresor devient passage.
- Forcer un monstre :
  - combattre sans epee : -20 PV;
  - combattre avec epee : epee consommee, 0 degat;
  - fuir : retour ancienne position.
- Forcer un piege : -15 PV et remplacement par passage.
- Verifier que la partie detecte la mort si les PV tombent a 0.

## Etape 12 - Implementer BFS

### Objectif

Calculer le chemin le plus court entre la position de l'aventurier et la sortie.

### A implementer

- Methode de `Donjon` proche du sujet :
  ```cpp
  vector<pair<int,int>> trouverChemin(pair<int,int> depart, pair<int,int> arrivee);
  ```
- Utiliser :
  - `queue<pair<int,int>>`;
  - `vector<vector<bool>>` pour les visites;
  - `vector<vector<pair<int,int>>>` pour les parents.
- Explorer les quatre directions.
- Ignorer uniquement :
  - cases hors bornes;
  - cases deja visitees;
  - murs.
- Traverser les tresors, monstres et pieges.
- Reconstruire le chemin avec les parents.

### Fichiers concernes

- `include/Donjon.hpp`
- `src/Donjon.cpp`

### Dependances

- Etape 8 terminee au minimum.
- Etape 9 utile pour appeler BFS depuis la position du joueur.

### Points de vigilance

- Gerer le cas ou depart == arrivee.
- Gerer le cas ou aucun chemin n'est trouve, meme si la generation devrait garantir un chemin.
- Ne pas inverser x/y lors de l'indexation.
- La distance affichee sera `chemin.size() - 1`.
- Ne pas modifier la grille pendant BFS.

### Tests a faire

- Tester BFS sur une petite grille simple.
- Verifier que le chemin commence par la position du joueur.
- Verifier que le chemin finit par la sortie.
- Verifier que la distance est correcte.
- Verifier que BFS traverse bien `+`, `M` et `T`.
- Verifier que BFS ne traverse pas `#`.

## Etape 13 - Afficher temporairement le chemin optimal

### Objectif

Permettre a l'utilisateur de demander le chemin avec `p`, puis afficher temporairement le chemin avec des `.` et la distance.

### A implementer

- Commande `p`.
- Appel a `Donjon::trouverChemin`.
- Methode d'affichage temporaire, par exemple :
  ```cpp
  void afficherAvecChemin(const vector<pair<int,int>>& chemin, const Aventurier& joueur);
  ```
- Afficher `.` sur les cases du chemin.
- Ne pas remplacer :
  - `@`;
  - `E`;
  - `S`.
- Afficher la distance.

### Fichiers concernes

- `include/Donjon.hpp`
- `src/Donjon.cpp`
- `include/Aventurier.hpp`
- `src/main.cpp`

### Dependances

- Etape 12 terminee.

### Points de vigilance

- L'affichage du chemin est temporaire.
- Ne jamais modifier les vrais objets `Case*` pour dessiner le chemin.
- Ne pas masquer l'aventurier.
- Ne pas masquer la sortie.
- Decider simplement si les cases speciales sur le chemin sont remplacees visuellement par `.` dans l'affichage temporaire. Le choix le plus simple est oui, sauf pour `@`, `E`, `S`.

### Tests a faire

- Appuyer sur `p` et verifier que le chemin apparait.
- Se deplacer ensuite et verifier que les `.` disparaissent de l'affichage normal.
- Verifier que la distance change apres deplacement.
- Verifier que la sortie reste visible.

## Etape 14 - Construire la boucle de jeu principale

### Objectif

Assembler les composants pour obtenir une partie jouable en console.

### A implementer

- Initialiser le donjon.
- Initialiser l'aventurier a l'entree.
- Boucle tant que :
  - l'aventurier est vivant;
  - il n'a pas atteint la sortie;
  - l'utilisateur n'a pas choisi de quitter.
- A chaque tour :
  - afficher le donjon avec `@`;
  - afficher le statut;
  - lire une commande avec Entree;
  - appliquer la commande;
  - resoudre la case si deplacement effectue;
  - verifier victoire ou defaite.
- Commandes :
  - `z`, `q`, `s`, `d`;
  - `p`;
  - optionnel : `x` pour quitter.

### Fichiers concernes

- `src/main.cpp`
- `include/Donjon.hpp`
- `src/Donjon.cpp`
- `include/Aventurier.hpp`
- `src/Aventurier.cpp`

### Dependances

- Etapes 1 a 13 terminees.

### Points de vigilance

- Garder la boucle simple.
- Eviter de mettre trop de logique complexe directement dans `main.cpp`.
- Ne pas oublier de conserver l'ancienne position avant un deplacement.
- En cas de commande invalide, afficher un message clair et continuer.
- En cas de mort apres piege ou monstre, terminer proprement.
- En cas d'arrivee sur la sortie, afficher un message de victoire.

### Tests a faire

- Lancer une partie complete.
- Tester toutes les commandes.
- Tester une commande invalide.
- Tester un deplacement dans un mur.
- Tester l'affichage du chemin.
- Tester la victoire en atteignant la sortie.
- Tester la defaite en reduisant les PV a 0.

## Etape 15 - Nettoyer, verifier et stabiliser le code

### Objectif

Rendre le projet propre, coherent et presentable pour un rendu.

### A implementer

- Relire les headers et sources.
- Supprimer les tests temporaires dans `main.cpp`.
- Verifier que les noms sont coherents.
- Ajouter quelques commentaires utiles, sans sur-commenter.
- Verifier les includes.
- Verifier que le `Makefile` compile tous les fichiers.
- Ajouter eventuellement une cible `run` simple si souhaite.

### Fichiers concernes

- Tous les fichiers du projet.

### Dependances

- Etape 14 terminee.

### Points de vigilance

- Ne pas refactorer lourdement a la fin.
- Ne pas introduire de bonus tardif qui pourrait casser les fonctionnalites obligatoires.
- Garder le style simple et etudiant.
- Verifier que `CaseFactory` reste bien le point central de creation des cases.
- Verifier que la gestion memoire est propre.

### Tests a faire

- Compilation complete avec `make clean` puis `make`.
- Lancer plusieurs parties.
- Tester plusieurs tailles, dont :
  - `21x21`;
  - `20x20` avec ajustement;
  - petite taille impaire.
- Verifier que les cases speciales fonctionnent.
- Verifier BFS plusieurs fois.

## Etape 16 - Produire les exemples de donjons et preparer les livrables

### Objectif

Preparer les elements attendus pour le rendu, sans necessairement implementer une fonctionnalite complete de sauvegarde/chargement.

### A implementer

- Generer quelques donjons.
- Copier leur affichage dans des fichiers `.txt`.
- Ajouter eventuellement un dossier `data/` pour stocker ces exemples.
- Preparer les elements du rapport PDF a la fin :
  - architecture globale;
  - classes principales;
  - algorithmes utilises;
  - choix d'implementation;
  - difficultes rencontrees;
  - solutions apportees.
- Ajouter un journal de prompts si utilise.

### Fichiers concernes

- `data/` si cree.
- fichiers `.txt` d'exemples.
- rapport PDF, a produire a la fin hors code.
- journal de prompts, si necessaire.

### Dependances

- Etape 15 terminee.

### Points de vigilance

- La sauvegarde/chargement complet est un bonus, pas une priorite actuelle.
- Les fichiers `.txt` doivent etre lisibles.
- Mentionner les deux noms du binome dans le rendu final.
- Respecter la date de rendu indiquee dans le sujet.

### Tests a faire

- Ouvrir les fichiers `.txt` et verifier leur lisibilite.
- Verifier que le code source complet est present.
- Verifier que le rapport couvre bien les criteres du sujet.
- Verifier que le projet compile depuis une copie propre.

## Ordre de validation recommande

1. Compilation minimale.
2. Cases et Factory.
3. Donjon rempli de murs.
4. Affichage ASCII.
5. Labyrinthe genere.
6. Elements speciaux places.
7. Aventurier affiche et deplace.
8. Effets des cases.
9. BFS.
10. Affichage temporaire du chemin.
11. Boucle de jeu complete.
12. Tests finaux et livrables.

## Risques principaux a surveiller

- Fuites memoire dues a l'utilisation de `Case*`.
- Confusion entre lignes/colonnes et x/y.
- Placement d'elements sur l'entree ou la sortie.
- BFS qui traverse mal les cases speciales.
- Affichage du chemin qui modifie accidentellement la grille.
- Boucle de jeu trop complexe dans `main.cpp`.
- Ajout de bonus avant stabilisation des obligations.


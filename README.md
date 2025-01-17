# Abstract Argumentation Solver

Ce projet implémente trois approches pour résoudre des problèmes dans des systèmes d'argumentation (Argumentation Frameworks - AF) : une approche naïve, une approche basée sur la fonction caractéristique, et une approche utilisant un labelling. Le programme inclut des tests unitaires pour valider ces approches ainsi qu'un fichier de tests automatiques qui génère des graphes aléatoires avec différentes tailles, densités et nombres d'attaques, et sauvegarde les résultats dans un fichier CSV pour analyse.

---

## Organisation du projet

La structure du projet est la suivante :

```
.
├── include/         # Contient les fichiers header (.h)
│   ├── ArgumentationFramework.h
│   ├── Parser.h
│   ├── utility.h
├── results/    # Contient les résultats des tests automatiques 
│   ├── results.csv
│   ├── comparison.png
├── src/             # Contient les fichiers source (.cpp)
│   ├── ArgumentationFramework.cpp
│   ├── Parser.cpp
│   ├── utility.cpp
│   ├── main.cpp
│   ├── auto_tests.cpp
├── af/              # Contient les fichiers de test (.txt) décrivant les cadres argumentatifs
│   ├── af1.txt
│   ├── af2.txt
│   ├── ...
├── tests/           # Tests unitaires et automatiques
│   ├── ArgumentationFrameworkTest.cpp
└── README.md        
```

---

## Fonctionnalités

1. **Approches implémentées :**
   - **Naïve :** Explore exhaustivement tous les sous-ensembles possibles d'arguments pour vérifier les propriétés ou calculer les extensions.
   - **Fonction caractéristique :** Exploite une fonction pour calculer les points fixes correspondant aux extensions complètes et stables.
   - **Labelling :** Calcule des extensions en étiquetant les arguments comme "in", "out", ou "undec".

2. **Tests unitaires :**
   - Situés dans `tests/ArgumentationFrameworkTest.cpp`.
   - Vérifient les propriétés fondamentales des approches sur des graphes d'argumentation bien définis.

3. **Tests automatiques :**
   - Fichier `src/auto_tests.cpp` : Génère des graphes aléatoires avec différentes tailles et densités d'attaques, exécute toutes les commandes et sauvegarde les résultats dans `results/results.csv`.

---

## Compilation et Exécution

### Prérequis

> **Compilateur :** Assurez-vous d'avoir une version de `g++` compatible avec la norme `C++17` (par exemple, `g++ 7.3` ou plus récent).  
> **Make :** Le fichier `Makefile` est inclus pour simplifier la compilation. Assurez-vous que l'outil `make` est installé sur votre système.


### Compilation

Pour compiler le projet, utilisez la commande suivante :

```bash
make
```

### Exécution

L'exécutable `solver` permet d'exécuter différentes commandes pour calculer ou vérifier des propriétés sur les cadres argumentatifs.

```bash
./solver -p COMMAND -f FILE [-a ARG]
```

- **`COMMAND` :**
  - `SE-CO` : Extension complète aléatoire.
  - `DC-CO` : Acceptation crédule dans les extensions complètes.
  - `DS-CO` : Acceptation sceptique dans les extensions complètes.
  - `SE-ST` : Extension stable aléatoire.
  - `DC-ST` : Acceptation crédule dans les extensions stables.
  - `DS-ST` : Acceptation sceptique dans les extensions stables.

- **`FILE` :** Chemin du fichier décrivant le cadre argumentatif.

- **`ARG` :** (Optionnel) Argument à tester pour les commandes `DC-XX` et `DS-XX`.

### Exemple d'exécution

```bash
./solver -p DC-CO -f af/test_af1.txt -a A
```

---

## Format des fichiers d'entrée

Les fichiers doivent suivre le format suivant :

- **Arguments** : `arg(X).` où `X` est le nom de l'argument.
- **Attaques** : `att(X, Y).` où `X` attaque `Y`.

Exemple de fichier `af/af_test.txt` :

```
arg(A).
arg(B).
arg(C).
att(A, B).
att(B, C).
att(C, A).
```

---

## Résultats

Les tests automatiques génèrent des résultats stockés dans un fichier CSV (`results/results.csv`). Les graphes sont générés aléatoirement avec des tailles, densités et nombres d'attaques croissants. Les résultats incluent le temps d'exécution pour chaque commande, ainsi que le nombre d'états explorés pour comparer les performances des trois approches.

---

## Auteurs

- FEKIH HASSEN Yassine, KADIC Anais
- Date : 2024
- Auteure du projet : Mme. BONZON Elise, Université Paris Cité - LIPADE


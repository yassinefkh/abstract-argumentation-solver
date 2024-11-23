# Abstract Argumentation Solver

Ce projet est une implémentation d'un solveur pour les graphes d'argumentation (Abstract Argumentation Frameworks - AF). Il prend en entrée des fichiers décrivant un graphe d'argumentation et permet de calculer les extensions complètes et stables, ainsi que de vérifier l'acceptation crédule et sceptique des arguments.

---

## Organisation du projet

La structure du projet est la suivante :

```
.
├── include/         # Contient les fichiers header (.h)
│   ├── ArgumentationFramework.h
│   ├── Parser.h
│   ├── utility.h
├── src/             # Contient les fichiers source (.cpp)
│   ├── ArgumentationFramework.cpp
│   ├── Parser.cpp
│   ├── utility.cpp
│   ├── main.cpp
├── af/              # Contient les fichiers de test (.txt) décrivant les cadres argumentatifs
│   ├── test_af1.txt
│   ├── test_af2.txt
│   ├── ...
└── README.md        # Ce fichier
```

---

## Fonctionnalités

1. **Extensions complètes** :
   - Calcul de toutes les extensions complètes.
   - Affichage d'une seule extension aléatoire.
2. **Extensions stables** :
   - Calcul de toutes les extensions stables.
   - Affichage d'une seule extension aléatoire.
3. **Acceptation crédule (DC-XX)** :
   - Vérification si un argument appartient à au moins une extension (complète ou stable).
4. **Acceptation sceptique (DS-XX)** :
   - Vérification si un argument appartient à toutes les extensions (complète ou stable).

---

## Compilation

Pour compiler le projet, utilisez g++. Les fichiers source se trouvent dans le dossier `src/`, et les headers dans `include/`.

```bash
g++ -std=c++17 -Iinclude -o solver src/*.cpp
```

---

## Exécution

L'exécutable `solver` prend plusieurs commandes pour exécuter des actions spécifiques.

### Usage général

```bash
./solver -p COMMAND -f FILE [-a ARG]
```

- `COMMAND` : Spécifie l'opération à exécuter :
  - `SE-CO` : Calcul d'une extension complète aléatoire.
  - `DC-CO` : Vérification de l'acceptation crédule d'un argument dans les extensions complètes.
  - `DS-CO` : Vérification de l'acceptation sceptique d'un argument dans les extensions complètes.
  - `SE-ST` : Calcul d'une extension stable aléatoire.
  - `DC-ST` : Vérification de l'acceptation crédule d'un argument dans les extensions stables.
  - `DS-ST` : Vérification de l'acceptation sceptique d'un argument dans les extensions stables.
- `FILE` : Chemin vers le fichier décrivant le cadre argumentatif.
- `ARG` : (Optionnel) Nom de l'argument à tester pour les commandes `DC-XX` et `DS-XX`.

### Exemples d'exécution

- Calculer une extension complète aléatoire :

```bash
./solver -p SE-CO -f af/test_af1.txt
```

- Vérifier l'acceptation crédule d'un argument dans les extensions complètes :

```bash
./solver -p DC-CO -f af/test_af1.txt -a A
```

- Vérifier l'acceptation sceptique d'un argument dans les extensions stables :

```bash
./solver -p DS-ST -f af/test_af2.txt -a B
```

- Calculer une extension stable aléatoire :

```bash
./solver -p SE-ST -f af/test_af2.txt
```

---

## Format des fichiers d'entrée

Les fichiers décrivant les cadres argumentatifs doivent suivre le format suivant :

- Définition des arguments : Chaque argument est défini par une ligne de la forme :

  ```
  arg(X).
  ```

  où `X` est le nom de l'argument (lettres, chiffres ou _).

- Définition des attaques : Chaque attaque est définie par une ligne de la forme :

  ```
  att(X, Y).
  ```

  où `X` attaque `Y`.

### Exemple de fichier `af/test_af1.txt`

```
arg(A).
arg(B).
arg(C).
att(A, B).
att(B, C).
att(C, A).
```

---

## Tests

Des fichiers de test sont disponibles dans le dossier `af/`. Ils incluent des fichiers `.txt` décrivant les graphes d'argumentation.

Pour tester un fichier donné :

```bash
./solver -p SE-CO -f af/test_af1.txt
```

---

## Auteurs

- FEKIH HASSEN Yassine, KADIC Anais
- Date : 2024
- Auteur.e du projet : BONZON Elise

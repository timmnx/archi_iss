# ARCHI TP ISS - Timothée MENEUX

---

### Sommaire :
[1 - Assembleur](#1)

[2 - Syntaxe risc](#2)

    [2.1 - Instructions et opérations](#2.1)

    [2.2 - Input / Output](#2.2)

    [2.3 - End](#2.3)

    [2.4 - Commentaires et exemple](#2.4)

[3 - Compilation et exécution](#3)

[4 - Les tests](#4)

---

## 1 - Assembleur {#1}

J'ai conçu un assembleur en `python3` (`assembleur.py`) qui permet de traduire un code assembleur (`.risc`) en un fichier en hexadécimal (`.hex`) compréhensible par l'interpréteur `main.o`.

Pour compiler un fichier `.risc` il suffit d'exécuter la commande suivante :
```
$ python3 assembleur.py fichier.risc
```
Cette commande va chercher le fichier `fichier.risc` dans le répertoire `tests_code` et produit le fichier `fichier.hex` dans le répertoire `tests`.

---

## 2 - Syntaxe risc {#2}

Une instructoin se note ```op rd rs imm8``` (quand les champs sont nécessaires pour l'instruction), et l'on met au plus une instruction par ligne. On peut écrire en minuscule ou en majuscule, l'assembleur transformera automatiquement tous les caractères en leur version majuscule. Les mots clés doivent respecter une certaine syntaxe :

* ```op``` : le mot clé de l'une des 16 instructions possible,

* ```rd``` : représente l'un des 4 registres, qui peuvent être nommés come suit :
    | Registre             | A      | B      | C      | PC     |
    |----------------------|:------:|:------:|:------:|:------:|
    | **Mot clé littéral** | ``ra`` | ``rb`` | ``rc`` | ``pc`` |
    | **Mot clé binaire**  | ``00`` | ``01`` | ``10`` | ``11`` |

* ```rs``` : même syntaxe que pour ```rd```,

* ```imm8``` : un entier non signé sur 16 bits, écrit en hexadécimal (donc de longeur 4).

### 2.1 - Instructions et opérations {#2.1}
| Mot clé  | op | Opération                                               |
|:--------:|:--:|---------------------------------------------------------|
| `imm`    | 0  | `rd` = sext(`imm8`); `pc`++                             |
| `add`    | 1  | `rd` = `rd` + `rs`; `pc`++                              |
| `addi`   | 2  | `rd` = `rs` + sext(`imm8`); `pc`++                      |
| `sub`    | 3  | `rd` = `rd` - `rs`; `pc`++                              |
| `xor`    | 4  | `rd` = `rd` ^ `rs`; `pc`++                              |
| `or`     | 5  | `rd` = `rd` \| `rs`; `pc`++                             |
| `and`    | 6  | `rd` = `rd` & `rs`; `pc`++                              |
| `sr`     | 7  | `rd` = `rd` >> 1; `pc`++                                |
| `ori`    | 8  | `rd` = `rd` \| `imm8`; `pc`++                           |
| `oris`   | 9  | `rd` = `rd` \| (`imm8` << 8); `pc`++                    |
| `ld`     | A  | `rd` = mem[`rs`]; `pc`++                                |
| `st`     | B  | mem[`rs`] = `rd`; `pc`++                                |
| `lt`     | C  | if (`rd` < `rs`) then `pc` += sext(`imm8`) else `pc`++  |
| `eq`     | D  | if (`rd` == `rs`) then `pc` += sext(`imm8`) else `pc`++ |
| `io`     | E  | input/output ([voir 2.2](#2.2))                             |
| `end`    | F  | stop le programme                                       |

### 2.2 - Input / Output {#2.2}

L'instruction `io rd rs` utilise l'op **E** qui est _undefined_ par défaut et permet d'intégrer une gestion de l'input/output. Voici les différentes façons d'utiliser l'instruction :

| rd              | Type   | Opération           |
|-----------------|--------|---------------------|
|  `00` (ou `ra`) | input  | `rs` = input()      |
|  `01` (ou `rb`) | input  | mem[`rs`] = input() |
|  `10` (ou `rc`) | output | print(`rs`)         |
|  `11` (ou `pc`) | output | print(mem[`rs`])    |

_NB : Pour les opérations de type input, si `rs == 11`, alors l'opération n'aura pas lieu et à la place une ligne vide sera imprimée._

### 2.3 - End {#2.3}
L'instruction `end` utilise l'op **F** qui est _undefined_ par défaut et est le marqueur de fin du programme et arrête donc l'exécution de l'interpréteur. _Il n'est pas nécessaire de le mettre en fin du code `.risc`, l'assembleur l'ajoute automatiquement._

### 2.4 - Commentaires et exemple {#2.4}
Il y a la possibilité de faire des commentaires, tout ce qui suit un `#` dans une même ligne sera ignoré. Par exemple, le code suivant charge les registres A et B à 3 et 6, puis stocke dans A le résultat de A + B et les commentaires sont bien ignorés :
```
# charger les registres
imm ra 3    # ra = 3
imm rb 6    # rb = b

# addition
add ra rb   # ra = ra + rb
```

---

## 3 - Compilation et exécution {#3}

La compilation est assurée par le `Makefile`, c'est-à-dire que la commande `$ make` compile tout le projet.

Ensuite, pour exécuter l'interpréteur, on doit lui fournir un fichier de test `fichier.hex`, stocké (de préférence) dans le répertoire `tests`. Il ne reste plus qu'à exécuter le tout grâce à la commande suivante :
```
$ ./main.o tests/fichier.hex
```

---

## 4 - Les tests {#4}

* `op_1_3`, `op_4_6`, `op_7_9` et `op_A_D` :  ces fichiers fournissent une batterie d'exemples pour toutes les opérations demandées (de `0` à `D`) et affiche les résultats. On peut donc vérifier que notre interpréteur marche correctement.

* `op_E` : ce fichier implémente un programme simple qui utilise les 4 fonctions d'io et les 2 façons d'imprimer un retour à la ligne. La première requête stocke dans `ra` l'emplacement où commencer à écrire puis on écrit 5 valeurs contigus dans la mémoire, de `mem[ra]` à `mem[ra+4]`, puis on les affiche.

* `op_F` : ce fichier montre bien que l'instruction `end`force l'arrêt du programme, bien qu'il puisse y avoir d'autres instruction après.

* `mult` : ce fichier implémente la multiplication entre deux entiers. Pour choisir les entiers, le programme demande à l'utilisateur de les renseigner et les stocke dans la mémoire aux emplacements 0 et 1 (`mem[0]` et `mem[1]`). Le résultat est stocké dans la mémoire à l'emplacement 3 (`mem[3]`) et l'affiche.

* `div` : ce fichier implémente la division euclidienne entre deux entiers. Pour choisir les entiers, le programme demande à l'utilisateur de les renseigner et les stocke dans la mémoire aux emplacements 0 et 1 (`mem[0]` et `mem[1]`). Les résultats sont stocké dans la mémoire aux emplacement 3 et 4, `mem[3]` pour le quotient et `mem[4]` pour le reste, et les affiche.

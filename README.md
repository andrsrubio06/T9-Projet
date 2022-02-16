# Keyboard T9 project

BRAGA E SILVA, Marcelo & RUBIO CARVAJAL, Andrés Eloy

Le 13/02/2022
## Introduction
Le T9 c’est le « text on 9 keys ». Il s’agit d’une technologie inventée par Dale Grover, Martin King et Clifford Kushlor afin de faciliter l’écriture sur nos anciens téléphones dotés de touches. Il s’agit d’une écriture prédictive qui permet de gagner du temps, lorsque en comparaison au clavier Multi-tap si on veut écrire "touche", il faut taper "8666882224433", tandis qu'on T9 c'est simplement "868243" et le clavier est suffisamment intelligent pour déduire que vous voulez écrire "touche".
## Informations

Ce projet fait partie du cours IN204 : Programmation Objet & Génie Logiciel de l'école ENSTA Paris. Le site Web du cours est accessible sur ce [lien](https://perso.ensta-paris.fr/~bmonsuez/Cours/doku.php?id=in204).
Le développement a été divisé entre Backend (c'est-à-dire, le fonctionnement du clavier T9 proprement dit) et Frontend (l'interface graphique qui permet l'utilisateur d'interagir avec l'application).
Le développement Backend a été fait sur Windows avec Visual Studio 2019, cependant, des fichiers Linux sont aussi disponibles.

Pour compiler le projet en Linux il faut les fichiers dans la branche "main": main.cpp et trie.hpp et aussi le fichier wordsandfrequency.txt (ou .csv) qui contient le dictionnaire à implémenter. Alors, une façon de le faire serait:
- g++ main.cpp trie.hpp -o app-keyboard
- ./app-keyboard (pour lancer l'application)

Également pour lancer l'application en utilisant sfml la commande suivante est nécessaire, le fichier contenant l'utilisation de sfml d'autre part n'est pas dans la branche main, il est dans la branche testing.
- g++ -c -Wall -O3 main.cpp trie.hpp && g++ *.o -o app-keyboard -lsfml-graphics -lsfml-window -lsfml-system && ./app-keyboard 


Si vous voulez la version VS 2019 pour Windows, allez dans la branche "backend" et téléchargez le dossier "KeyboardSearchEx" qui contient la solution.

## Comment utiliser le clavier T9

Le clavier a été développé en accord de cette [page web](https://en.wikipedia.org/wiki/T9_(predictive_text)), avec des suggestions de mots en anglais. Il a le layout suivante:

![alt text](https://github.com/andrsrubio06/T9-Projet/blob/main/ex_clavier.PNG)
  
  ## Implémentation et détails du code
  
  ### Structure de données pour le dictionnaire
  Tout d'abord, nous avons défini la structure de données capables de gérer un dictionnaire avec des milliers des mots et au même
  temps qui soit capable d'effectuer une recherche d'un mot de façon rapide. Pour cela, on a choisi la structure [Trie](https://en.wikipedia.org/wiki/Trie).
  Avec un dictionnaire en Trie, on aura une recherche d'un mot d'un complexité au maximum égale à la longueur du mot le plus longue, et un quantité de noeuds 
  d'au maximum la longueur du mot le plus long le nombre de lettres possibles ('a' à 'z').
  
  Pour l'implementation de la database du dictionnaire, on a utilisé le fichier disponible sur ce [lien](https://www.kaggle.com/rtatman/english-word-frequency).
  
  ![alt text](https://camo.githubusercontent.com/8487a43200d8d4c9993154623868caa74143367eaa240b1e2d67dbfb5d7d1d2c/68747470733a2f2f342e62702e626c6f6773706f742e636f6d2f2d474e5763354b554d4759632f5741736b502d4548464b492f4141414141414141457a342f3879696b7863326e69596779714830465746616671355554705f6b554b364f3541434c63422f73313630302f5472696544617461537472756374757265496d706c2e706e67)
  
  Pour l'implémenter, un fichier .txt ou .csv (défini au début du programme) qui contient les mots est leurs fréquences d'utilisation en ordre décroissante est lu est traduite en Trie. 
  Le temps d'implémenter cette structure pour 30000 mots (pour nos tests, 30000 c'est une quantité adéquate des mots) est en moyenne inférieur à 0,3 secondes.

  
  ### Mécanisme de recherche
  Au début, lorsque l'utilisateur touche un chiffre, il est traduit dans les lettres qui correspondent à ce chiffre (à l'aide d'un 'map'). Ces lettres sont ajoutées à une liste des mots courants, initialement vides.
  Pour chaque lettre dans cette liste, on effectue une recherche sur la trie:
  - Si elle retourne -1 => mot n'est ni un mot possible ni un préfixe d'un autre mot
  - Si elle retourne  0 => mot n'est pas un mot possible mais est un préfixe
  - Autrement           => mot est un mot possible, et il sera ajouté à la liste de mots suggérés.
  Après, on met les mots suggérés en ordre décroissant de fréquence et les affiche au utilisateur. Lorsque un nouveau chiffre est tapé, on ajoute les lettres correspondantes à la fin de chaque mot dans la liste courant et répète la procédure.
  
  ### Mécanisme d'effaçage
  La phrase que l'utilisateur écrit est traité dans le code comme un pointer d'un tableau de string. Lorsque l'utilisateur veut effacer une lettre, le programme identifie
  quelle string du tableau a été modifié, fait la conversion de lettres en chiffres (Ex.: home -> 4663), retire le dernier chiffre (Ex.: 466) et ensuite trouve les suggestions 
  pour cette séquence de valeurs.

### Autres fonctionnalités
  - Ponctuation avec button '1' (.,!?)
  - Addition d'un nombre avec la touche 'n'
  - Addition d'un mot au format Multi-tap avec la touche 'a'. Si elle n'est pas dans le dictionnaire, le programme l'ajoute automatiquement.
  - Mettre en majuscule/minuscule avec touche '#'
  - Parcourir parmi les suggestions avec touche '*'
  - Mots en majuscule après un '.' , '!' ou '?'
  - Effacer phrase avec touche 'c'

 ## Points à améliorer
 - La correcte utilisation des flèches '<-' et '->' pour se balader dans la phrase et changer n'importe quel mot.
 - Changement des fréquences d'utilisation des mots en temps réel. Cependant, l'efficacité du code sera affecté car: soit on doit accéder au fichier dictionnaire .txt à chaque mot tapé (cher en temps du code), soit on accès le trie, change la fréquence d'utilisation d'un noeud à chaque mot entrée (peu cher en temps du code) et à la fin du programme retraduire la Trie en dictionnaire .txt (cher, mais sera faite qu'une fois à la fin).
 - Ajouter des noeuds dans la trie avec accent et chiffres spéciaux (comme ç) pour être compatible au français. Pour cela, il faut les ajouter aux fonctions du trie, que pour le moment considère l'alphabet de 26 lettres.
 - Si un mot de taille 'n' n'a pas de suggestion, suggérer les mots de taille plus grande que 'n'. (Ex.: 'perfe' n'est pas un mot, mais on peut déduire que l'utilisateur veut écrire 'perfect') 
 - Réussir à avoir une interface graphique fonctionnelle et qui réplique exactement le portables du passé (comme le Nokia 3310), avec la fonte des mots pixelisé.

## Licence
This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
For more information, read the LICENCE file available in the project root.

## Références
- https://en.wikipedia.org/wiki/T9_(predictive_text)
- https://perso.ensta-paris.fr/~bmonsuez/Cours/doku.php?id=in204:resources
- https://www.kaggle.com/rtatman/english-word-frequency
- https://en.wikipedia.org/wiki/Trie

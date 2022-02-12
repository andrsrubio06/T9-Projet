# Keyboard T9 project

BRAGA E SILVA, Marcelo & RUBIO CARVAJAL, Andrés Eloy

Le 13/02/2022

## Informations

Ce projet fait partie du cours IN204 : Programmation Objet & Génie Logiciel de l'école ENSTA Paris. Le site Web du cours est acesible sur ce [lien](https://perso.ensta-paris.fr/~bmonsuez/Cours/doku.php?id=in204).
Le development a été divisé entre Backend (c'est-à-dire, le fonctionnement du clavier T9 proprement dit) et Frontend (l'interface graphique qui permet l'utilisateur d'intéragir avec l'application).
Le dévélopment Backend a été fait sur Windows avec Visual Studio 2019, cependant, des fichiers Linux sont aussi disponibles.

Pour compiler le projet il faut les fichiers main.cpp et trie.hpp et aussi le fichier wordsandfrequency.txt (ou .csv) qui contient le dictionaire à implementer. Alors, une façon de le faire serait:
- g++ main.cpp trie.hpp -lncurses -o app-keyboard
- ./app-keyboard (pour lancer l'application)

## Comment utiliser le clavier T9

Le clavier a été développé en accord de cette [page web](https://en.wikipedia.org/wiki/T9_(predictive_text)), avec des suggestions de mots en anglais. Il a les fonctionalités suivantes:
![alt text](https://drive.google.com/file/d/1pZfTdZC8S2MXIJQKuwUqFvLRepHsPInS/view?usp=sharing)
  
  ## Implémentation et détails du code
  
  ### Structure de données pour le dictionanire
  Tout d'abbord, nous avons défini la structure de données capables de gerir un dictionaire avec des miliers des mots et au même
  temps qui soit capable d'effectuer une recherche d'un mot de une façon vite. Pour cela, on a choisi la structure [Trie](https://en.wikipedia.org/wiki/Trie).
  Avec un dictionaire en Trie, on aura une recherche d'un mot d'un complexité au maximum égale à la longuer du mot le plus longue, et un quantité de noeuds 
  d'au maximum la longuer du mot le plus longue fois le nombre de lettres possibles ('a' à 'z').
  
  ![alt text](https://camo.githubusercontent.com/8487a43200d8d4c9993154623868caa74143367eaa240b1e2d67dbfb5d7d1d2c/68747470733a2f2f342e62702e626c6f6773706f742e636f6d2f2d474e5763354b554d4759632f5741736b502d4548464b492f4141414141414141457a342f3879696b7863326e69596779714830465746616671355554705f6b554b364f3541434c63422f73313630302f5472696544617461537472756374757265496d706c2e706e67)
  
  Pour l'implémenter, un fichier .txt ou .csv (défini au début du programme) qui contient les mots est leurs fréquences d'utilisation en ordre décroissante est lu est traduite en Trie. 
  Le temps d'implementer cette structure pour 30000 mots (pour nos tests, 30000 c'est une quantité adéquat des mots) est en moyen inférieur à 0,3 secondes.
  
  ### Mechanisme de recherche
  Au début, lorsque l'utilisateur touche un chiffre, il est traduit dans les lettres qui correspondent à ce chiffre (à l'aide d'un 'map'). Ces lettres sont ajoutés a une liste des mots courants, initialement vides.
  Pour chaque lettre dans cette liste, on effectue une recherche sur la trie:
  - Si elle retourne -1 => mot n'est ni un mot possible ni un préfixe d'un autre mot
  - Si elle retourne  0 => mot n'est pas un mot possible mais est un préfixe
  - Autrement           => mot est un mot possible, et il sera ajouté à la liste de mots suggérés.
  Après, on mets les mots suggérés en ordre decroissante de fréquence et les affiche au utilisateur. Lorsque un nouvelle chiffre est tapé, on rajoute les lettres correspondantes à la fin de chaque mot dans la liste courant et répète la procedure.
  
  ### Mechanisme d'effaçage
  La phrase que l'utilisateur écrit est traité dans le code comme un pointer d'un tableau de string. Lorsque l'utilisateur veut effacer une lettre, le programme identifie
  quelle string du tableau a été modifié, fait la conversion de lettres en chiffres (Ex.: home -> 4663), retire le dernier chiffre (Ex.: 466) et ensuite trouve les sugestions 
  pour cette séquence de valeurs.
  
  

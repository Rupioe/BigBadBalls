# BigBadBalls :
Le but du jeu est simple, il ne faut pas se faire toucher par les méchantes grosses boules qui vous foncent dessus.
Pour gagner il faut simplement dépasser son précédent meilleur score.
Quand vous êtes touché il y a une invincibilité de 3 secondes.
Les flèches peuvent être tirées seulement debout.
Vous gagnez une seconde par ennemi abattu et quand vous abattez un ennemi vous pouvez retirer instantanément.
### CONTROLES :
| touche | action         |
|--------|----------------|
|Space   |saut            |
|Q       |gauche          |
|D       | droite         |
|S       |s'accroupir     |
|J       |tirer une flèche|
|K       |fermer le jeu   |
|R       | relancer le jeu|

### Ressources essentielles :
 - le répertoire "dolby_atmos"
 - Le fichier ".highscore" est créé dans le répertoire courant (là d'où est lancé le jeu).
 N'essayez pas de le modifier ça ne marchera pas ;)
 Pour réinitialiser l'highscore vous pouvez effacer le fichier ".highscore" ou le modifier ou recompiler le jeu. 
 - QT5

### Compilation :
Depuis le répertoire source du jeu contenant le code source :
```sh
mkdir build && cd build && cmake .. && make
 ```
Puis pour lancer le jeu
```sh
./bigbadballs
 ```

### OST :
- Heretik System - Resistance
- Gabba Front Berlin - Speedcore Lacrima
- Yosuf - Angry Birds Theme Hardstyle
- Phonk Woods
- Lost woods slow + reverb

 #### NB :
 J'ai souffert d'un rf -Rf ./* de mon home sur linux et j'ai donc perdu une semaine de travail, heureusement que je fais des backups.
 Comme ressource j'ai utilisé le site web QT, le site web donné en ressource pour le saut et des posts forums stack overflow pour le debug.
Il pourrait exister un bug faisant planter le jeu avec comme erreur "illegal instruction" mais il s'est produit une seule fois sur des centaines de lancement, s'il se manifeste, il disparaitra probablement après quelques redémarrages ou en mettant du dentifrice sur votre RAM. 

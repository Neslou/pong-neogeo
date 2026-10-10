# Pong pour Neo Geo, en C

Le code de la série YouTube **« Apprendre à coder sur Neo Geo en C »** (chaîne Neslou).
Un Pong complet pour la Neo Geo (AES / MVS), écrit en C avec [ngdevkit](https://github.com/dciabrin/ngdevkit),
en 13 étapes. Chaque étape est un **tag Git** : tu peux revenir à n'importe quel moment de la série.

L'ordre des notions suit la leçon 0 du cours [CS50's Introduction to Game Development](https://cs50.harvard.edu/games/)
(Harvard, licence CC BY-NC-SA 4.0). Aucun code, image ni son du cours n'est réutilisé : tout est écrit de zéro.

## Les étapes

| Tag | Épisode | Ce qu'on ajoute |
| --- | --- | --- |
| `pong-0` | 03 | La plus petite ROM : PONG au centre de la couche fix |
| `pong-1` | 03 | Un écran titre, deux palettes, la zone sûre |
| `pong-2` | 04 | Raquettes et balle en sprites (tuiles 16 × 16, SCB1 à SCB4) |
| `pong-3` | 05 | Boucle de jeu sur le VBlank, manettes, virgule fixe |
| `pong-4` | 05 | Bords, balle au hasard (xorshift), états DEBUT et JEU |
| `pong-5` | 06 | Une `struct` et un fichier par objet |
| `pong-6` | 06 | Notre interruption VBlank, compteur d'images perdues |
| `pong-7` | 06 | Collisions AABB, rebonds, accélération |
| `pong-8` | 07 | Le score en grands chiffres (sprites), « drapeau sale » |
| `pong-9` | 07 | Le service |
| `pong-10` | 07 | Victoire à 10 points, nouvelle partie |
| `pong-11` | 08 | Trois bruitages ADPCM-A (pilote son Z80) |
| `pong-12` | 08 | Prêt pour le vrai matériel (écritures VRAM espacées) |

**Les bonus « plus Neo Geo »**, un par épisode, sur leurs propres branches :

| Branche (tag) | Effet |
| --- | --- |
| `bonus-1-neogeo` (`pong-1-neogeo`) | Le fond qui respire (une seule couleur changée au VBlank) |
| `bonus-2-neogeo` (`pong-2-neogeo`) | Une palette par joueur, raquettes qui se déplient (réduction SCB2) |
| `bonus-4-neogeo` (`pong-4-neogeo`) | La balle arc-en-ciel (palette cycling) |
| `bonus-7-neogeo` (`pong-7-neogeo`) | La balle en animation automatique matérielle |
| `bonus-10-neogeo` (`pong-10-neogeo`) | La fête de la victoire (fond et score qui bat) |
| `bonus-12-neogeo` (`pong-12-neogeo`) | L'éclair à chaque point |

## Compiler et lancer

Il faut ngdevkit installé (épisode 02 de la série) et le dossier `examples` de
[ngdevkit-examples](https://github.com/dciabrin/ngdevkit-examples) configuré, car `config.mk` vient de là.

```
git clone [lien du dépôt] pong-neogeo
cd pong-neogeo
cp ../examples/config.mk .      # ou : ln -s ../examples/config.mk .
git checkout pong-12            # l'étape voulue
make                            # fabrique la ROM dans build/
make mame                       # ou : make gngeo
```

Commandes : flèches haut/bas pour les raquettes (joueur 1 et joueur 2), START pour commencer,
servir et rejouer. Pendant les étapes pong-6 à pong-11, maintenir le bouton A ralentit volontairement
le jeu pour voir le compteur d'images perdues monter.

## Testé

Toutes les étapes compilent sans avertissement et ont été jouées dans MAME 0.264 (parties complètes
jusqu'à la victoire, sons enregistrés). Le test sur vrai matériel (Analogue Pocket) fait l'objet de
l'épisode 08.

## Licence

Le code est sous licence GPL v3 (voir `COPYING`), comme les exemples ngdevkit-examples dont il part (les fichiers du modèle `00-template` gardent leur propre en-tête de licence).
Neo Geo est une marque de SNK Corporation. Projet indépendant, non affilié à SNK ni à Plaion.

## Les outils

- `tools/sprites_gif.py` régénère l'image des tuiles : `python3 tools/sprites_gif.py base assets/pong-sprites.gif`
  (`base` pour pong-2 à pong-7, `chiffres` à partir de pong-8, `anim` pour le bonus 7). Tu peux aussi
  dessiner ce GIF toi-même (Aseprite : 64 px de large, tuiles 16 × 16, mode indexé, couleur 0 transparente).
- `tools/sons.py` régénère les trois bruitages (ondes carrées, WAV mono 18 500 Hz) : `python3 tools/sons.py assets/sons`.
Les deux scripts demandent Python 3 et Pillow pour le premier (`pip install pillow`).

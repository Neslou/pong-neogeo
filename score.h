/*
 * Pong pour Neo Geo - l'affichage du score en grands chiffres (sprites).
 */
#ifndef FICHIER_SCORE_H
#define FICHIER_SCORE_H

#include <ngdevkit/neogeo.h>

void score_init(void);
void score_afficher(u8 joueur, u8 valeur);

#endif

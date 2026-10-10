/*
 * Pong pour Neo Geo - notre propre attente de VBlank (remplace ng_wait_vblank).
 */
#ifndef FICHIER_VBLANK_H
#define FICHIER_VBLANK_H

#include <ngdevkit/neogeo.h>

extern volatile u32 vblank_compteur;   // nombre d'images depuis l'allumage
void attendre_vblank(void);

#endif

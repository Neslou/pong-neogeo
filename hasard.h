/*
 * Pong pour Neo Geo - un generateur de nombres au hasard.
 */
#ifndef FICHIER_HASARD_H
#define FICHIER_HASARD_H

#include <ngdevkit/neogeo.h>

void hasard_graine(u16 graine);
u16 hasard(void);
s16 hasard_entre(s16 min, s16 max);

#endif

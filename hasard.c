/*
 * Pong pour Neo Geo - un generateur "xorshift" sur 16 bits.
 * Chaque appel melange les bits de la valeur precedente.
 */
#include "hasard.h"

static u16 etat_hasard = 1;

// La graine ne doit jamais valoir 0 (le generateur resterait bloque a 0).
void hasard_graine(u16 graine) {
    etat_hasard = graine ? graine : 1;
}

u16 hasard(void) {
    etat_hasard ^= etat_hasard << 7;
    etat_hasard ^= etat_hasard >> 9;
    etat_hasard ^= etat_hasard << 8;
    return etat_hasard;
}

// Un nombre entre min et max (inclus).
s16 hasard_entre(s16 min, s16 max) {
    return min + (s16)(hasard() % (u16)(max - min + 1));
}

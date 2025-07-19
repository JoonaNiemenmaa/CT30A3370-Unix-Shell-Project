/* Joona Niemenmaa 001823383 16.07.2025 vector.c */
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

int vektorinPituus(char **aVektori) {
    int iKoko = 0;
    char **ptr = aVektori;
    if (ptr != NULL) {
        while (*ptr != NULL) {
            iKoko++;
            ptr++;
        }
    }
    return iKoko;
}

char **lisaaVektoriin(char **aVektori, char *aArvo) {
    int iKoko = vektorinPituus(aVektori);
    /* Vektorin kokoon lisätään kaksi, koska vektorin tulee loppua NULL-pointeriin */
    if ((aVektori = (char **)realloc(aVektori, sizeof(char *) * (iKoko + 2))) == NULL) {
        perror("wish: Muistin varaaminen epäonnistui, lopetetaan");
        exit(1);
    }

    iKoko++;

    aVektori[iKoko - 1] = aArvo;
    aVektori[iKoko] = NULL;

    return aVektori;
}

void tulostaVektori(char **aVektori) {
    char **ptr = aVektori;
    if (ptr != NULL) {
        while (*ptr != NULL) {
            printf("%s\n", *ptr);
            ptr++;
        }
    }
    return;
}

/* eof */

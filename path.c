/* Joona Niemenmaa 001823383 17.07.2025 path.c */
#include "path.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *haePolku(char *aKomento, char **aPolut) {
    char *aPolku = NULL;
    char **ptr = aPolut;
    if (aPolut != NULL) {
        while (*ptr != NULL) {
            if ((aPolku = (char *)malloc(strlen(*ptr) + strlen(aKomento) + 2)) == NULL) {
                perror("wish: Muistin varaamisessa virhe, lopetetaan");
                exit(1);
            }

            aPolku[0] = '\0';

            strcat(aPolku, *ptr);
            strcat(aPolku, "/");
            strcat(aPolku, aKomento);

            if (access(aPolku, X_OK) == 0) {
                return aPolku;
            }

            free(aPolku);
            aPolku = NULL;

            ptr++;
        }
    }
    return NULL;
}

char **muodostaPolut(char **aArgumentit, int iArgumentit) {
    char **aPolut = NULL;
    char *aPolku =  NULL;
    if (iArgumentit > 1) {
        for (int i = 1; i < iArgumentit; i++) {
            if ((aPolku = (char *)malloc(sizeof(strlen(aArgumentit[i]) + 1))) == NULL) {
                perror("path: Muistin varaamisessa virhe, lopetetaan");
                exit(1);
            }
            strcpy(aPolku, aArgumentit[i]);
            aPolut = lisaaVektoriin(aPolut, aPolku);
        }
    } else {
        fprintf(stderr, "path: Virhe, komento tarvitsee ainakin yhden argumentin\n");
    }
    return aPolut;
}

char **vapautaPolut(char **aPolut) {
    if (aPolut != NULL) {
        char **ptr = aPolut;
        while (*ptr != NULL) {
            free(*ptr);
            ptr++;
        }
        free(aPolut);
    }
    return NULL;
}

/* eof */

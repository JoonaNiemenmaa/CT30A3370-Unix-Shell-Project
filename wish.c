/* Joona Niemenmaa 001823383 16.07.2025 wish.c */
#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "vector.h"

#define PROMPT "wish> "

void haePolku(char *aKomento, char *aPolku, char **aPolut);
char **parsiArgumentit(char *aKomento);

int main(int argc, char **argv) {
    char *aRivi = NULL;
    size_t iKoko = 0;
    char **aArgumentit = NULL;
    int iArgumentit = 0;
    int iPaluuarvo = 0;
    int iLapsi = 0;

    char **aPolut = NULL;
    char aPolku[255] = "";
    aPolut = lisaaVektoriin(aPolut, "/bin");

    printf(PROMPT);
    while (getline(&aRivi, &iKoko, stdin) != -1) {
        aRivi[strlen(aRivi) - 1] = '\0';

        aArgumentit = parsiArgumentit(aRivi);
        iArgumentit = vektorinPituus(aArgumentit);

        if (strcmp(aArgumentit[0], "exit") == 0) {
            exit(0);
        } else {
            haePolku(aArgumentit[0], aPolku, aPolut);

            iPaluuarvo = fork();
            if (iPaluuarvo == -1) {
                perror("Virhe luodessa lapsiprosessia");
            } else if (iPaluuarvo == 0) {
                execv(aPolku, aArgumentit);
                perror("Virhe ajaessa ohjelmaa");
                free(aArgumentit);
                exit(1);
            } else if (iPaluuarvo > 0) {
                iLapsi = iPaluuarvo;
                waitpid(iLapsi, NULL, 0);
            }
        }

        free(aArgumentit);
        aArgumentit = NULL;

        printf(PROMPT);
    }

    free(aPolut);
    aPolut = NULL;

    return 0;
}

char **parsiArgumentit(char *aRivi) {
    char *ptr = NULL;
    char **aArgumentit = NULL;
    ptr = strtok(aRivi, " ");
    if (ptr != NULL) {
        aArgumentit = lisaaVektoriin(aArgumentit, ptr);
        while ((ptr = strtok(NULL, " ")) != NULL) {
            lisaaVektoriin(aArgumentit, ptr);
        }
    }
    return aArgumentit;
}

void haePolku(char *aKomento, char *aPolku, char **aPolut) {
    char **ptr = aPolut;
    if (aPolut != NULL) {
        while (*ptr != NULL) {
            aPolku[0] = '\0';
            strcat(aPolku, *ptr);
            strcat(aPolku, "/");
            strcat(aPolku, aKomento);
            if (access(aPolku, F_OK) == 0) {
                return;
            }
            ptr++;
        }
    }
    aPolku[0] = '\0';
    return;
}

/* eof */

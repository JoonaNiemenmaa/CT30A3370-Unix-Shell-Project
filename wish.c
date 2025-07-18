/* Joona Niemenmaa 001823383 16.07.2025 wish.c */
#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "vector.h"
#include "path.h"

#define PROMPT "wish> "

char **parsiArgumentit(char *aKomento, char **aSuuntaus);

int main(int argc, char **argv) {
    char *aRivi = NULL;
    size_t iKoko = 0;

    char **aArgumentit = NULL;
    char *aSuuntaus = NULL;
    int iArgumentit = 0;

    int iPaluuarvo = 0;
    int iLapsi = 0;

    /* Polkulistan alustus on tehty dynaamisella muistinhallinnalla virheellisen muistin vapauttamisen välttämiseksi myöhemmin */
    char **aPolut = NULL;
    char *aAlkuPolku = NULL;

    if ((aAlkuPolku = (char *)malloc(sizeof("/bin") + 1)) == NULL) {
        perror("Muistin varaamisessa virhe, lopetetaan");
        exit(1);
    }

    strcpy(aAlkuPolku, "/bin");
    aPolut = lisaaVektoriin(aPolut, aAlkuPolku);

    printf(PROMPT);
    while (getline(&aRivi, &iKoko, stdin) != -1) {
        aRivi[strlen(aRivi) - 1] = '\0';

        aArgumentit = parsiArgumentit(aRivi, &aSuuntaus);
        iArgumentit = vektorinPituus(aArgumentit);

        if (aArgumentit != NULL) {
            if (strcmp(aArgumentit[0], "exit") == 0) {
                exit(0);
            } else if (strcmp(aArgumentit[0], "path") == 0) {
                aPolut = vapautaPolut(aPolut);
                aPolut = muodostaPolut(aArgumentit, iArgumentit);
                tulostaVektori(aPolut);
            } else if (strcmp(aArgumentit[0], "cd") == 0) {
                if (iArgumentit == 2) {
                    char aUusi[strlen(aArgumentit[1]) + 1];
                    strcpy(aUusi, aArgumentit[1]);
                    if (chdir(aUusi) == -1) {
                        perror("cd");
                    }
                }
            } else {
                char aKomento[strlen(aArgumentit[0])];
                strcpy(aKomento, aArgumentit[0]);

                aArgumentit[0] = haePolku(aKomento, aPolut);

                if (aArgumentit[0] != NULL) {

                    iPaluuarvo = fork();
                    if (iPaluuarvo == -1) {
                        perror("Virhe luodessa lapsiprosessia");
                    } else if (iPaluuarvo == 0) {
                        if (aSuuntaus != NULL) {
                            close(STDOUT_FILENO);
                            open(aSuuntaus, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
                        }
                        execv(aArgumentit[0], aArgumentit);
                        perror("Virhe ajaessa ohjelmaa");
                        exit(1);
                    } else if (iPaluuarvo > 0) {
                        iLapsi = iPaluuarvo;
                        waitpid(iLapsi, NULL, 0);
                    }

                    free(aArgumentit[0]);
                    aArgumentit[0] = NULL;
                }
            }

            free(aArgumentit);
            aArgumentit = NULL;

            if (aSuuntaus != NULL) {
                free(aSuuntaus);
                aSuuntaus = NULL;
            }
        }

        printf(PROMPT);
    }

    free(aPolut);
    aPolut = NULL;

    return 0;
}

char **parsiArgumentit(char *aRivi, char **aSuuntaus) {
    char *ptr = NULL;
    char **aArgumentit = NULL;
    ptr = strtok(aRivi, " ");
    if (ptr != NULL) {
        aArgumentit = lisaaVektoriin(aArgumentit, ptr);
        while ((ptr = strtok(NULL, " ")) != NULL && strcmp(ptr, ">") != 0) {
            aArgumentit = lisaaVektoriin(aArgumentit, ptr);
        }
        if (ptr != NULL) {
            if (strcmp(ptr, ">") == 0) {
                if ((ptr = strtok(NULL, " ")) != NULL) {
                    if ((*aSuuntaus = (char *)malloc(sizeof(char) * (strlen(ptr) + 1))) == NULL) {
                        perror("Muistin varaamisessa virhe, lopetetaan");
                        exit(1);
                    }
                    strcpy(*aSuuntaus, ptr);
                }
            }
        }
    }
    return aArgumentit;
}

/* eof */

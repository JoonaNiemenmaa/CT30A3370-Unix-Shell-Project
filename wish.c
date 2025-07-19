/* Joona Niemenmaa 001823383 16.07.2025 wish.c */
#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "vector.h"
#include "path.h"

#define PROMPT "wish> "

char **parsiArgumentit(char *aKomento, char **aSuuntaus);

int main(int argc, char **argv) {

    char *aSatsi = NULL;
    FILE *Virta = NULL;

    if (argc > 2) {
        fprintf(stderr, "wish: Virhe, luettavia tiedostoja voi määrittää vain yhden\n");
        exit(1);
    } else if (argc == 2) {
        aSatsi = argv[1];
        if ((Virta = fopen(aSatsi, "r")) == NULL) {
            perror("wish: Tiedoston avaaminen epäonnistui, lopetetaan");
            exit(1);
        }
    } else {
        Virta = stdin;
    }

    char *aRivi = NULL;
    size_t iKoko = 0;

    char **aArgumentit = NULL;
    char *aSuuntaus = NULL;
    int iArgumentit = 0;
    char *aKomento = NULL;
    char *aTallenne = NULL;

    int iPaluuarvo = 0;
    int iLapsia = 0;
    int *iLapset = NULL;

    /* Polkulistan alustus on tehty dynaamisella muistinhallinnalla virheellisen muistin vapauttamisen välttämiseksi myöhemmin */
    char **aPolut = NULL;
    char *aAlkuPolku = NULL;

    if ((aAlkuPolku = (char *)malloc(sizeof("/bin") + 1)) == NULL) {
        perror("wish: Muistin varaaminen epäonnistui, lopetetaan");
        exit(1);
    }

    strcpy(aAlkuPolku, "/bin");
    aPolut = lisaaVektoriin(aPolut, aAlkuPolku);

    if (Virta == stdin) {
        printf(PROMPT);
    }
    while (getline(&aRivi, &iKoko, Virta) != -1) {
        aRivi[strlen(aRivi) - 1] = '\0';

        if ((aKomento = strtok_r(aRivi, "&", &aTallenne)) != NULL) {
            do {
                aArgumentit = parsiArgumentit(aKomento, &aSuuntaus);
                iArgumentit = vektorinPituus(aArgumentit);

                if (aArgumentit != NULL) {
                    if (strcmp(aArgumentit[0], "exit") == 0) {
                        if (iArgumentit == 1) {
                            vapautaPolut(aPolut);
                            aPolut = NULL;
                            free(aArgumentit);
                            aArgumentit = NULL;
                            if (aSuuntaus != NULL) {
                                free(aSuuntaus);
                                aSuuntaus = NULL;
                            }
                            free(aRivi);
                            aRivi = NULL;
                            exit(0);
                        } else {
                            fprintf(stderr, "exit: Virhe, komento ei ota argumentteja\n");
                        }

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
                        } else {
                            fprintf(stderr, "cd: Virhe, komento ottaa vain yhden argumentin.\n");
                        }

                    } else {
                        aArgumentit[0] = haePolku(aArgumentit[0], aPolut);

                        if (aArgumentit[0] != NULL) {
                            iPaluuarvo = fork();

                            if (iPaluuarvo == -1) {
                                perror("wish: Lapsiprosessin luominen epäonnistui");
                            } else if (iPaluuarvo == 0) {
                                if (aSuuntaus != NULL) {
                                    close(STDOUT_FILENO);
                                    if (open(aSuuntaus, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU) == -1) {
                                        perror("wish: Virhe ohjelman uudelleensuuntauksessa");
                                        exit(1);
                                    }
                                }
                                execv(aArgumentit[0], aArgumentit);
                                perror("wish: Virhe ajaessa ohjelmaa");
                                exit(1);
                            } else if (iPaluuarvo > 0) {
                                iLapsia++;
                                if ((iLapset = (int *)realloc(iLapset, sizeof(int) * iLapsia)) == NULL) {
                                    perror("wish: Muistin varaaminen epäonnistui, lopetetaan");
                                    exit(1);
                                }
                                iLapset[iLapsia - 1] = iPaluuarvo;
                            }
                            free(aArgumentit[0]);
                            aArgumentit[0] = NULL;
                        } else {
                            fprintf(stderr, "wish: Virhe, komentoa ei löydy.\n");
                        }
                    }

                    free(aArgumentit);
                    aArgumentit = NULL;

                    if (aSuuntaus != NULL) {
                        free(aSuuntaus);
                        aSuuntaus = NULL;
                    }
                }
            } while ((aKomento = strtok_r(NULL, "&", &aTallenne)) != NULL);
        }

        for (int i = 0; i < iLapsia; i++) {
            waitpid(iLapset[i], NULL, 0);
        }

        if (iLapset != NULL) {
            free(iLapset);
            iLapset = NULL;
        }
        iLapsia = 0;

        free(aRivi);
        aRivi = NULL;

        if (Virta == stdin) {
            printf(PROMPT);
        }
    }

    if (aSatsi != NULL) {
        fclose(Virta);
    }

    if (aRivi != NULL) {
        free(aRivi);
        aRivi = NULL;
    }

    vapautaPolut(aPolut);
    aPolut = NULL;

    return 0;
}

char **parsiArgumentit(char *aKomento, char **aSuuntaus) {
    char *ptr = NULL;
    char **aArgumentit = NULL;
    ptr = strtok(aKomento, " \t");
    if (ptr != NULL) {
        aArgumentit = lisaaVektoriin(aArgumentit, ptr);
        while ((ptr = strtok(NULL, " \t")) != NULL && strcmp(ptr, ">") != 0) {
            aArgumentit = lisaaVektoriin(aArgumentit, ptr);
        }
        if (ptr != NULL) {
            if (strcmp(ptr, ">") == 0) {
                if ((ptr = strtok(NULL, " \t")) != NULL) {
                    if ((*aSuuntaus = (char *)malloc(sizeof(char) * (strlen(ptr) + 1))) == NULL) {
                        perror("wish: Muistin varaamisessa virhe, lopetetaan");
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

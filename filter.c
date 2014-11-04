#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {

    char *nume, **mesaj;
    int nr_msg, bad_posts, score, *bad_mess;
} User;

void read_from_file(int *N, int *K, int *P, char ***cuvant, User **u) {

    int i, j;
    char buff[201];
    scanf("%d%d%d", N, K, P);
	getchar();

	char **tempcuvant = (char**)malloc( (*K) *sizeof(char*) );
	// citesc cuvintele ce trebuie cenzurate
	for (i = 0; i < (*K); i++) {

		fgets(buff, 200, stdin);
		tempcuvant[i] = malloc(strlen(buff)*sizeof(char));
		memcpy(tempcuvant[i], buff, strlen(buff) -1);
	}
	*cuvant = tempcuvant;
	//citesc userii si mesajele lor
	User *tempu;
	tempu = (User *)malloc( (*N) * sizeof(User));
	for (i = 0; i < (*N); i++) {

            scanf("%s", buff);
            (tempu+i)-> nume = (char *)malloc( strlen(buff) * sizeof(char));
            memcpy((tempu+i)-> nume, buff, strlen(buff) + 1);
            getchar();
            scanf("%d", &(tempu+i)->nr_msg);
            getchar();
            (tempu+i)->mesaj = (char **)malloc( (tempu+i)->nr_msg * sizeof(char *));
            (tempu+i)->bad_mess = (int *)malloc( (tempu+i)->nr_msg * sizeof(int));
            for (j = 0; j < ((tempu+i)->nr_msg); j++) {

                fgets(buff, 200, stdin);
                (tempu+i)->mesaj[j] = malloc( strlen(buff) * sizeof(char));
                (tempu+i)->bad_mess[j] = 0;
                memcpy((tempu+i)->mesaj[j], buff, strlen(buff) - 1);
            }
    }
    *u = tempu;
}

char *strdel (char *p, int n) { // sterge n caractere din p

    char *aux = strdup(p+n);
    strcpy(p, aux);
    return p;
}

char *strins (char *p, char *s) { // insereaza un sir in alt sir

    char *aux = strdup(p);
    strcpy(p,s);
    strcat(p,aux);
    return p;
}

int main() {

	freopen("test.in", "r", stdin);
	freopen("test.out", "w", stdout);

	int i, j, N, K, P, c, us, m, ok = 0, st, dr, k, ok2 = 1, _maxscore = -100;
	char **cuvant = NULL;
    User *u = NULL;
	read_from_file(&N, &K, &P, &cuvant, &u);
	char inserez[201], cuv2[201];
    for (us = 0; us < N; us++) {

        for (m = 0; m < (u+us)->nr_msg; m++) {

            ok2 = 1;
            for (c = 0; c < K; c++) {

                    for (i = 0; i < strlen(cuvant[c]) - 1; i++) {

                        inserez[i] = '*';
                    }
                    inserez[i] = '\0';

                    //cenzura neinteligenta

                    char *pstr = strstr( (u+us)->mesaj[m], cuvant[c] );
                    while (pstr != NULL) {

                    int pp;
                    pp = (int)(pstr - ( (u+us)->mesaj[m]) );
                    char ch = (u+us)->mesaj[m][pp + strlen(cuvant[c])];
                    if (!isalpha(ch)) {

                        strdel(pstr+1, strlen(cuvant[c]) - 1);
                        strins(pstr+1, inserez);
                        ok2 = 0;
                        (u+us)->bad_posts = (u+us)->bad_posts + 1;
                        pstr = strstr(pstr, cuvant[c]);
                    }
                    else pstr = strstr(pstr+1, cuvant[c]);

                    }

                    // cenzura inteligenta
                    i = j = 0;
                    while (i < strlen((u+us)->mesaj[m])) {

                        if ( ((u+us)->mesaj[m][i] == ' ') && j ) {

                            for (k = 0; k < j; k++) {

                                if (!isalpha(cuv2[k]) && (cuv2[k] != '*')) {

                                    char *pp = cuv2 + k;
                                    strdel(pp,1);
                                    j--, k--;
                                }
                            }
                            if (j == strlen(cuvant[c])) {

                                ok = 1;
                                for (k = 0; k < j; k++)
                                if (cuv2[k] != cuvant[c][k]) ok = 0;
                            }
                            if (ok) {

                                char *pp = (u+us)->mesaj[m] + st + 1;
                                strdel(pp, dr - st);
                                strins(pp, inserez);
                                ok = ok2 = 0;
                                i = i - (dr - st) + strlen(inserez);
                                (u+us)->bad_posts = (u+us)->bad_posts + 1;
                            }
                            j = 0;
                        }
                        else {

                            cuv2[j++] = (u+us)->mesaj[m][i];
                            if (j == 1) st = i;
                            else if ( ((u+us)->mesaj[m][i] >= 'a' && (u+us)->mesaj[m][i] <= 'z') ||  \
									((u+us)->mesaj[m][i] >= 'A' && (u+us)->mesaj[m][i] <= 'Z') )
                            	dr = i;
                        }
                    i++;
                    }

                    for (k = 0; k < j; k++) {

                        if (!isalpha(cuv2[k]) && (cuv2[k] != '*')) {

                            char *pp = cuv2 + k;
                            strdel(pp,1);
                            j--, k--;
                        }
                    }
                    if (j == strlen(cuvant[c])) {

                        ok = 1;
                        for (k = 0; k < j; k++)
                        if (cuv2[k] != cuvant[c][k]) ok = 0;
                    }
                    if (ok) {

                        char *pp = (u+us)->mesaj[m] + st + 1;
                        strdel(pp, dr - st);
                        strins(pp, inserez);
                        ok = ok2 = 0;
                        (u+us)->bad_posts = (u+us)->bad_posts + 1;
                    }
            }
            if (ok2) (u+us)->bad_mess[m] = 1;
        }
    }

    //task 3

    for (us = 0; us < N; us++) {

        if ((u+us)->bad_posts <= P) {

            for (i = 0; i < N; i++) {

                if (i != us) {

                    for (m = 0; m < (u+us)->nr_msg; m++) {

                        char *pos = strstr( (u+us)->mesaj[m], (u+i)->nume );
                        if (pos!= NULL)
                            if ( (u+us)->bad_mess[m] ) (u+i)->score = (u+i)->score + 1;
                            else (u+i)->score = (u+i)->score - 1;
                    }
                }
            }
        }
    }
    for (us = 0; us < N; us++)
        if ((u+us)->bad_posts <= P)
            if ((u+us)->score > _maxscore) _maxscore = (u+us)->score;

    //print dates

    for (us = 0; us < N; us++)
        if ((u+us)->bad_posts <= P) {

            printf("%s\n", (u+us)->nume);
            for (m = 0; m < (u+us)->nr_msg; m++)
                printf("%s\n", (u+us)->mesaj[m]);
        }
    printf("Most popular : ");
    for (us = 0; us < N; us++)
        if ((u+us)->bad_posts <= P)
            if((u+us)->score == _maxscore)
                printf("%s ", (u+us)->nume);
	return 0;
}

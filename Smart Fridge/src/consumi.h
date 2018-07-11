/**
 * @file consumi.h
 *
 * @brief Libreria per la gestione dei consumi.
 *
 * La libreria consumi.h è stata implementata per permettere di registrare
 * gli alimenti o le ricette che vengono consumate dagli utenti permettendo
 * di tenere traccia dei prodotti maggiormente consumati
 *
 * @authors Alessandro Scarcia, Davide Quatela, Michela Salvemini
 */

/// Inclusione delle librerie standard
#ifndef STD_LIB
#define STD_LIB
	#include <stdbool.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
#endif

///Inclusione della libreria matematica
#ifndef MATH_LIB
#define MATH_LIB
	#include <math.h>
#endif

/// Inclusione delle librerie per la manipolazione di stringhe e caratteri
#ifndef STRING_LIB
#define STRING_LIB
	#include <string.h>
	#include <ctype.h>
#endif


#ifndef CONSUMI_LIB
#define CONSUMI_LIB

#define FILE_CONSUMI "../consumi.dat"

#define FLAG_RICETTA 1
#define FLAG_ALIMENTO 0

#define LUNG_NOME_PRODOTTO 31


typedef struct{
	char nome_prodotto[LUNG_NOME_PRODOTTO];
	short flag_prodotto;
	int frequenza;
}consumo;

/**Funzione che si occupa di memorizzare il prodotto consumato(0-->alimento,  1---->ricetta)
 * all'interno di file che memorizza i vari consumi degli utenti
 *
 * @param nome_prodotto		nome della ricetta/alimento da registrare
 * @param flag_prodotto		tipologia di prodotto da registrare (alimento/ricetta)
 * @return 1				in caso di avvenuto successo nella registrazione del consumo
 */
int registra_consumo(char* nome_prodotto, short flag_prodotto);



/**funzione che ricerca il prodotto maggiormente consumato in base al valore del flag che viene passato(0-->alimento, 1--->ricetta) e lo stampa
 *
 * @param flag_prodotto		tipologia di prodotto(ricetta/alimento)	su cui deve essere effettuata la ricerca
 * @return 1				in caso di successo della ricerca
 */
int ricerca_prod_magg_cons(short flag_prodotto);

#endif

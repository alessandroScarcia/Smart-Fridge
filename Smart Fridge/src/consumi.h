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

#ifndef CONSUMI_LIB
#define CONSUMI_LIB

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

/// Nomi di file:
#define FILE_CONSUMI "../consumi.dat"		/// Nome del file contenente le frequenze di consumazione dei prodotti del frigo

/// Flags:
#define FLAG_RICETTA 1						/// Valore corrispondente ad un prodotto di tipo ricetta
#define FLAG_ALIMENTO 0						/// Valore corrispondente ad un prodotto di tipo alimento

/// Lunghezze delle stringhe:
#define LUNG_NOME_PRODOTTO 31				/// Lunghezza massima in caratteri per il nome di un prodotto (compreso il carattere terminatore)

/**
 * @typedef consumo
 *
 * Tipo di dato utilizzato per la lettura e scrittura di un record in FILE_CONSUMI.
 * I membri che lo caratterizzano sono: nome del prodotto consumato, flag che indica
 * il tipo del prodotto, frequenza delle consumazioni del prodotto.
 */
typedef struct{
	char nome_prodotto[LUNG_NOME_PRODOTTO];
	short flag_prodotto;
	int frequenza;
}consumo;

/**
 * Funzione che si occupa di memorizzare l'incremento del valore frequenza di un prodotto
 * in FILE_CONSUMI.
 *
 * @param nome_prodotto		Nome del prodotto per cui incrementare la frequenza.
 * @param flag_prodotto		Tipologia di prodotto da registrare.
 *
 * @return 1				In caso successo nella registrazione del consumo.
 * @return 0				Se i parametri attuali non sono validi
 * @return -1				Se il file non può essere aperto e creato
 */
int registra_consumo(char* nome_prodotto, short flag_prodotto);



/**
 * Funzione che ricerca il prodotto maggiormente consumato, della tipologia ricevuta in ingresso.
 *
 * @param flag_prodotto		Tipologia di prodotto da ricercare.
 *
 * @return 1				In caso di successo della ricerca.
 * @return 0				Se il parametro non è valido.
 * @return -1				Se non è possibile aprire FILE_COSUMI.
 */
int ricerca_prod_magg_cons(short flag_prodotto);

#endif

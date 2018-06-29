/*
 * utenti.h
 *
 *  Created on: 10 mag 2018
 *      Author: Michela
 */

/// Inclusione delle librerie standard
#ifndef STD_LIB
#define STD_LIB
	#include <stdbool.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
#endif

/// Inclusione delle librerie per la manipolazione di stringhe e caratteri
#ifndef STRING_LIB
#define STRING_LIB
	#include <string.h>
	#include <ctype.h>
#endif


#ifndef UTENTI_LIB
#define UTENTI_LIB


#include "alimenti.h"

#include "pulizia_flussi.h"

#define MAX_UTENTI 6

#define MIN_LUNG_NICKNAME 5							/// Lunghezza minima in caratteri per il nickname di un utente (escluso il terminatore '\0')
#define MAX_LUNG_NICKNAME 16 						/// Lunghezza massima in caratteri per il nickname di un utente (compreso il terminatore '\0')
#define NUM_PREFERENZE 3 							/// Numero di preferenze alimentari per ogni utente
#define LUNG_PREFERENZA 21 							/// Lunghezza massima in caratteri per la preferenza alimentare di ogni utente
#define LUNG_PASSWORD 9 							/// Lunghezza massima in caratteri per la password di ogni utente

#define GEN_AUTOMATICA 1
#define GEN_MANUALE 2

#define CAMPO_NICKNAME 1
#define CAMPO_PASSWORD 2
#define CAMPO_PREFERENZE 3

#define NUM_TIPI_CHAR 4
#define CHAR_SIMBOLO 1
#define CHAR_CIFRA 2
#define CHAR_MAIUSCOLA 3
#define CHAR_MINUSCOLA 4
#define MIN_ASCII_SIMBOLO 33
#define MAX_ASCII_SIMBOLO 47
#define MIN_ASCII_CIFRA 48
#define MAX_ASCII_CIFRA 57
#define MIN_ASCII_MAIUSCOLA 66
#define MAX_ASCII_MAIUSCOLA 90
#define MIN_ASCII_MINUSCOLA 97
#define MAX_ASCII_MINUSCOLA 122

#define FILE_DATABASE_UTENTI "database_utenti.dat"

typedef struct{
	char nickname[MAX_LUNG_NICKNAME];
    char password[LUNG_PASSWORD];
    char preferenze[NUM_PREFERENZE][LUNG_PREFERENZA]; //per preferenza si intende una preferenza di tipo alimentare (es: cioccolato, latte, etc..)
}utente;



/**
 * La funzione esiste_nickname() permette di verificare se il nickname passato
 * è già presente nel database.
 *
 * @return 1 se il nickname è già presente nel database.
 * @return 0 se il nickname non è presente nel database.
 * @return -1 se il FILE_DATABASE_UTENTI non può essere aperto.
 */
int esiste_nickname(const char* nickname);


/**
 * La funzione input_nickname() permette di richiedere l'inserimento di un
 * nickname destinato ad un nuovo utente.
 *
 * @return Stringa corrispondente al nickname inserito.
 */
char* input_nuovo_nickname();


char* input_nickname();


/**
 * La funzione genera_nickname() permette di generare un nickname
 * in modo casuale.
 *
 * @return Stringa corrispondente al nickname generato.
 */
char* genera_nickname();


/**
 * La funzione genera_password(int livello_sicurezza) permette la generazione di
 * una password con livello di sicurezza pari a quello inserito.
 *
 * @param livello_sicurezza Livello di sicurezza della password da generare.
 * @return Stringa corrispondente alla password generata.
 */
char* genera_password();


/**
 * La funzione input_password(char* password) permette la richiesta di
 * inserimento di una password.
 *
 * @return Stringa corrispondente alla password inserita.
 */
char* input_password();


/**
 * La procedura generatore_password() permette la selezione della modalità
 * di generazione di una password e il salvataggio della stessa all'interno
 * del corrispondente utente.
 *
 * @param password_utente Puntatore alla stringa che dovrà contenera la password.
 */
void generatore_password(char* password_utente); //Genera una password da memorizzare nel campo password della struct di tipo utente.


/**
 * La funzione input_preferenze() permette di richiedere l'inserimento
 * di una preferenza alimentare di un utente.
 *
 * @return Stringa corrispondente alla preferenza inserita.
 */
int input_preferenza(char* preferenza);


/**
 * La funzione input_utente() permette di richiedere l'inserimento
 * dei dati relativi ad un nuovo utente.
 *
 * @return Valori inseriti del nuovo utente.
 */
utente input_utente(); //salva le struct di tipo utente su un file .cvs


/**
 * La funzione genera_utente() effettua la generazione dei dati
 * di un utente in modo casuale e restituisce l'utente creato.
 *
 * @return Utente generato.
 */
utente genera_utente();


/**
 * La funzione genera_n_utenti() effettua la generazione di n
 * utenti, restituendo il puntatore all'array che li contiene
 *
 * @param n Numero di utenti da salvare
 * @return Puntatore all'array che contiene gli utenti generati
 */
utente* genera_n_utenti(int n);


/**
 * La funzione salva_n_utenti() permette di salvare n utenti sul file
 * FILE_DATABASE_UTENTI.
 *
 * @param utenti Array contenente gli utenti da memorizzare.
 * @param n Numero di utenti da memorizzare.
 * @return 1 se vengono salvati tutti gli utenti.
 * @return 0 se non può essere aperto FILE_DATABASE_UTENTI.
 */
int salva_n_utenti(utente* utenti, int n);


/**
 * La procedura crea_utente() permette la creazione di n nuovi utenti.
 */
int crea_utenti();


void modifica_preferenze(utente* u);


/**
 * La funzione gestore_modifiche() permette di far effettuare l'accesso ad utente per modificare
 * i propri dati.
 */
int gestore_modifiche();

/**
 * La funzione autenticazione() permette di effettuare l'autenticazione di un utente.
 *
 * @param password Password dell'utente da identificare.
 * @return 1 se l'autenticazione avviene.
 * @return 0 se non avviene l'autenticazione.
 */
int autenticazione(utente* u);

/**
 * La procedura output_utente() permette di visualizzare i dati di un utente.
 *
 * @param u Utente da visualizzare.
 */
void output_utente(const utente u);

/**
 * La procedura output_preferenze() permette di visualizzare le preferenze di un utente.
 *
 * @param u Utente per cui visulizzare le preferenze.
 *
 * @return numero di preferenze visualizzate.
 */
int output_preferenze(const utente u);


int visualizza_database_utenti(); //visualizza il contenuto di un file


int menu_database_utenti (int i); //Stampa su schermo un messaggio e permette di scegliere tra diverse opzioni, chiamando funzioni oppurtune alla scelta acquisita da tastiera


int conta_utenti();


int elimina_utente();

#endif

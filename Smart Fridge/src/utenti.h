/**
 * @file utenti.h
 *
 * @brief Libreria per la gestione degli utenti.
 *
 * La libreria utenti.h è stata implementata per permettere la gestione
 * degli utenti che usufruiscono  dei servizi messi a disposizione dall'elettrodomestico.
 * Avendo un profilo sará poi possibile infatti creare il proprio menu settimanale e tenere
 * traccia delle proprie assunzioni giornaliere dopo aver effettuato l'operazione di autenticazione
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

/// Inclusione delle librerie per la manipolazione di stringhe e caratteri
#ifndef STRING_LIB
#define STRING_LIB
	#include <string.h>
	#include <ctype.h>
#endif


#ifndef UTENTI_LIB
#define UTENTI_LIB

/// Inclusione della libreria per la gestione del menu_settimanale
#include "menu_settimanale.h"

/// Inclusione della libreria per la gestione degli alimenti
#include "alimenti.h"

/// Inclusione della libreria per la pulizia dei flussi di input
#include "pulizia_flussi.h"

/// Limiti e valori per determinati elementi:
#define MAX_UTENTI 6							/// Numero massimo di utenti memorizzabili
#define NUM_TIPI_CHAR 4							/// Numero di tipi di caratteri utilizzati per la generazione casuale
#define CHAR_SIMBOLO 1							/// Valore per la generazione di un simbolo
#define CHAR_CIFRA 2							/// Valore per la generazione di una cifre
#define CHAR_MAIUSCOLA 3						/// Valore per la generazione di una lettere maiuscola
#define CHAR_MINUSCOLA 4						/// Valore per la generazione di una lettera minuscola
#define MIN_ASCII_SIMBOLO 33					/// Valore minimo, nella tabella ASCII, per la generazione di un simbolo
#define MAX_ASCII_SIMBOLO 47					/// Valore massumo, nella tabella ASCII, per la generazione di un simbolo
#define MIN_ASCII_CIFRA 48						/// Valore minimo, nella tabella ASCII, per la generazionedi una cifra
#define MAX_ASCII_CIFRA 57						/// Valore massumo, nella tabella ASCII, per la generazione di una cifra
#define MIN_ASCII_MAIUSCOLA 66					/// Valore minimo, nella tabella ASCII, per la generazione di una lettera maiuscola
#define MAX_ASCII_MAIUSCOLA 90					/// Valore massumo, nella tabella ASCII, per la generazione di una lettera maiuscola
#define MIN_ASCII_MINUSCOLA 97					/// Valore minimo, nella tabella ASCII, per la generazione di una lettera minuscola
#define MAX_ASCII_MINUSCOLA 122					/// Valore massumo, nella tabella ASCII, per la generazione di una lettera minuscola
#define NUM_PREFERENZE 3 						/// Numero di preferenze alimentari per ogni utente
#define GEN_AUTOMATICA 1						/// Valore per selezionare la generaizone automatica
#define GEN_MANUALE 2							/// Valore per selezionare la generaizone manuale
#define CAMPO_NICKNAME 1						/// Valore per selezionare il campo nickname
#define CAMPO_PASSWORD 2						/// Valore per selezionare il campo password
#define CAMPO_PREFERENZE 3						/// Valore per selezionare il campo preferenze

/// Lunghezze di stringhe:
#define MIN_LUNG_NICKNAME 3						/// Lunghezza minima in caratteri per il nickname di un utente (escluso il terminatore '\0')
#define MAX_LUNG_NICKNAME 16 					/// Lunghezza massima in caratteri per il nickname di un utente (compreso il terminatore '\0')
#define LUNG_PREFERENZA 21 						/// Lunghezza massima in caratteri per la preferenza alimentare di ogni utente
#define LUNG_PASSWORD 9 						/// Lunghezza massima in caratteri per la password di ogni utente

/// Nome di file
#define FILE_DATABASE_UTENTI "database_utenti.dat"	/// Nome del file contenente le informazioni degli utenti memorizzati

/**
 * @typedef utente
 *
 * Tipo utilizzato per la memorizzazione delle informazioni degli utenti. Ogni utente è caratterizzato
 * da un nickname, una password con cui autenticarsi e un numero prefissato di preferenze alimentari.
 */
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
 * Funzione utilizzata per effettuare l'input di un nome utente.
 *
 * @return Puntatore alla stringa inserita dall'utente.
 */
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
utente input_utente();


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
 * Funzione utilizzata per la generazione di utenti.
 *
 * @return 1 Se la creazione avviene con successo.
 * @return 0 Se non è possibile effettuare la creazione.
 * @return -1 Se non possono essere salvati i dati creati.
 */
int crea_utenti();


/**
 * Procedura utilizzata per modificare i valori delle preferenze alimentari
 * di un utente.
 *
 * @param u Dati dell'utente su cui effettuare la modifica.
 */
void modifica_preferenze(utente* u);


/**
 * La funzione gestore_modifiche() permette di far effettuare l'accesso ad utente per modificare
 * i propri dati.
 *
 * @return 1 Se la modifica va a buon fine.
 * @return 0 Se la modifica viene annullata.
 * @return -1 Se non è possibile aprire il file.
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


/**
 * Funzione per la visualizzazione del database utenti.
 *
 * @return 1 Se la visualizzazione va a buon fine.
 * @return 0 Se il file non può essere aperto.
 */
int visualizza_database_utenti();

/**
 * Funzione per effettuare il conteggio degli utenti memorizzati.
 *
 * @return Numero di utenti memorizzati
 */
int conta_utenti();

/**
 * Funzione per effettuare l'eliminazione di un utente.
 *
 * @return -1 Se non è possibile aprire il file.
 * @return 0 Se viene annullata l'eliminazione.
 * @return 1 Se l'aliminazione va a buon fine.
 */
int elimina_utente();

#endif

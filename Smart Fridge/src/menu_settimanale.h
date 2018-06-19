/**
 * @file menu_settimanale.h
 *
 * @brief Libreria per la gestione e la creazione di un menu settimanale per ogni utente.
 *
 * La libreria menu_settimanale.h è stata implementata per dare la possibilità agli utenti
 * di creasi un proprio piano alimentare. A questo scopo sono state create delle struct per
 * la memorizzazione degli alimenti e/o ricette inserite. Per permettere la gestione di
 * quest'ultime, sono state implementate delle funzioni apposite.
 * Inoltre, sono state implementate una funzione per contare il numero di ricette presenti nel menu di
 * dato giorno e una procedura per popolare un array con il nome di tali ricette.
 *
 * @authors Alessandro Scarcia, Davide Quatela, Michela Salvemini
 */

//Inclusione delle librerie standard
#ifndef STD_LIB
#define STD_LIB
	#include <stdbool.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
#endif

//Inclusione della libreria matematica
#ifndef MATH_LIB
#define MATH_LIB
#include <math.h>
#endif

// Inclusione delle librerie per la manipolazione di stringhe e caratteri
#ifndef STRING_LIB
#define STRING_LIB
	#include <string.h>
	#include <ctype.h>
#endif

// Inclusione della libreria per la pulizia dei flussi di input
//#include "pulizia_flussi.h"

//Inclusione della libreria per la gestione degli utenti



#ifndef MENU_SETTIMANALE
#define MENU_SETTIMANALE

#include "utenti.h"

/// DEFINIZIONE DELLE COSTANTI SIMBOLICHE.


/// Numero di elementi di determinati oggetti:
#define NUM_ALIMENTO 5
#define NUM_PASTI 5
#define GIORNI_SETTIMANA 7

/// Lunghezze delle stringhe:
#define DIM_GIORNO 15
#define DIM_CIBO 40
#define DIM_PASTO 10
#define MAX_LUNG_NOMEFILE 30


#define FILE_MENU "menu_"


//Definizione dei tipi di di dato

/**
 * @typedef cibo
 *
 * Il tipo di dato "cibo" serve a memorizzare una ricetta o un singolo alimento.
 *  E' basato su di una struct i cui membri sono: nome_cibo, flag (viene impostato a zero
 *  se si tratta di un aliento singolo, ad 1 se si tratta di una ricetta) e quantità.
 *
 */

typedef struct {
	char nome_cibo[DIM_CIBO];
	short unsigned flag;
	float quantita; //quantità
}cibo;

/**
 * @typedef pasto
 *
 * Il tipo di dato "pasto" serve a memorizzare il cibo che l'utente si impegna a cosumare durante un dato pasto (pranzo, cena, ...).
 * E' basato su di una struct i cui membri sono: nome_pasto e alimento.
 * Alimento è un dato strutturato di tipo "cibo".
 */

typedef struct {
	char nome_pasto[DIM_PASTO];
	cibo alimento[NUM_ALIMENTO];
}pasto;

/**
 * @typedef giorno
 *
 * Il tipo di dato "giorno" serve a memorizzare il cibo che l'utente si impegna a cosumare durante un dato giorno della settimana.
 * E' basato su di una struct i cui membri sono: giorno e pasto.
 * Pasto è un dato strutturato di tipo "pasto".
 */

typedef struct {
	char giorno[DIM_GIORNO];
	pasto pasto[NUM_PASTI];

} giorno; //cambio nome

#endif /* MENU_SETTIMANALE */

/**
 * La procedura inizializzazione() chiama una serie di funzioni utili ad un inizializzazione di un vettore di 7
 * struct di tipo giorno, ognuna delle quali rappresenta un giorno della settimana
 *
 * @param no
 */
void inizializzazione ();

/**
 * La funzione inizializzazione_giorno() riceve in ingresso un puntatore
 * ad una variabile di tipo giorno e la sua posizione nel vettore creato nella funzione inizializzazione ().
 * La funzione assegnerà alla variabile giorno appartenente alla struct il nome di un giorno della settimana,
 * scegliendo tale nome sulla base della sua posizione nel vettore di struct a cui appartire.
 *
 * @param menu varibile da modificare, indice variabile per la scelta della modifica da effetuare
 * @return 0 se l'operazione va a buon fine
 */
int inizializzazione_giorno (giorno* menu, int indice);

/**
 * La funzione inizializzazione_pasti() riceve in ingresso un puntatore
 * ad una variabile di tipo giorno .
 * La funzione scorre nel vettore pasto e assegna ai suoi elementi, in base alla loro posizione nel vettore,
 *  il nome di un pasto, chiamando poi una funzione per l'inizializzazione dei singoli elementi appartenenti al pasto.
 *
 * @param menu varibile da modificare, num_pasto variabile per la scelta della modifica da effetuare
 * @return 0 se l'operazione va a buon fine
 */
int inizializzazione_pasti (giorno* menu);

/**
 * La funzione inizializzazione_pasti() riceve in ingresso un puntatore
 * ad una variabile di tipo giorno e l'indice del vettore della variabile al suo interno "pasto".
 * La funzione assegnerà alla variabile alimento, appartenente alla struct pasto, una stringa vuota al
 * nome_cibo, un flag impostato a -1 e una quantità pari a 0.
 *
 * @param menu varibile da modificare, num_pasto variabile per impostare l'indice della varaibile pasto.
 * @return 0 se l'operazione va a buon fine
 */
int inizializzazione_alimenti (giorno* menu, short int num_pasto);

/**
 * La funzione inizializzazione_file_menu() riceve in ingresso un vettore
 * di dato strutturato di tipo "menu".
 * La funzione scriverà su un file, il cui nome sarà il nickname dell'utente, il vettore ricevuto in ingresso.
 *
 * @param vettore menu da scrivere su file.
 * @return 0 se l'operazione va a buon fine
 */
int inizializzazione_file_menu (giorno write_menu[], char nomefile[]);



/**
 * La funzione visualizza_database_menu() riceve in ingresso un vettore di carattere, corrispondente
 * al nome file che andrà ad aprire/creare.
 * La funzione stamperà su schermo il contenuto del fine.
 *
 * @param vettore nomefile per aprire il file.
 * @return 0 se l'operazione va a buon fine
 */
int visualizza_database_menu (char nomefile[]);

/**
 * La funzione scelta_pasto() non riceve nulla in ingresso.
 * La funzione si occuperà dell'autenticazione dell'utente e quindi
 * dell'estrazione dei suoi dati dal database degli utenti.
 * Fatto ciò, chiederà all'utente di scegliere il giorno della settimana su cui desidera effettuare le modifiche.
 * Dopodiché chiederà all'utente di scegliere uno dei cinque pasti disponibili, sucessivamente passerà l'indice corrispondente al pasto che
 * ha scelto e menu ad un'altra funzione per procedere ad un ulteriore modifica sulla base della scelta effettuata.
 * Scriverà il nuovo menu su file e visualizzerà il contenuto dell'intero file su schermo con le modifiche effettuate.
 *
 * @param no
 * @return 0 se l'operazione va a buon fine
 */
int scelta_pasto();

/**
 * La funzione scelta_alimenti() riceve in ingresso un dato strutturato di tipo "giorno" ed un numero corrispondente all'indice del vettore
 * pasto contenuto nel dato strutturato menu.
 * La funzione chiederà all'utente di scegliere quale degli alimenti modificare, per poi procedere all'inserimento da tastiera dei nuovi
 * valori con cui sostituire i valori attuali.
 *
 * @param menu su cui effettuare le modifiche.
 * @return 0 se l'operazione va a buon fine
 */
int scelta_alimenti (giorno* menu, short int num_pasto);

/**
 * La funzione estrazione_struct() riceve in ingresso un puntatore ad una struct di tipo giorno, il nome dell'utente e l'indice corrispondente
 * al giorno della settimana.
 * La funzione estrarrà n-esimo elemento dal file, lo memorizzerà nella variabile di tipo giorno ricevuta in ingresso e terminerà la sua funzione.
 *
 * @param menu su cui memorizzare la struct da estrarre da file, nome_utente per aprire il menu corrispondente a tale utente, n per estrarre n-esimo
 * elemento del file.
 * @return 0 se l'operazione va a buon fine
 */
int estrazione_struct (giorno* menu, char nome_utente[], int n);

/**
 * La funzione stampa_menu() riceve in ingresso un dato strutturato di tipo giorno e lo stampa su schermo.
 * @param menu da stampare su schermo.
 * @return 0 se l'operazione va a buon fine
 */
void stampa_menu (giorno* menu);

/**
 * La procedura scrivi_menu() riceve in ingresso un dato strutturato di tipo giorno, il nome del file da aprire, la posizione dove andare a sovrascrivere.
 * Effettua una serie di concanetazioni per generare il nome del file su cui andare a scrivere basandosi sul nome utente passatogli, apre il file,
 * posizione il puntatore all'i-esima posizione e scrive in tale posizione il menu ricevuto in ingresso.
 * @param menu da scrivere su file, nome_utente da cui generare il nome del file su cui scrivere, i corrispondente alla posizione su cui scrivere nel file.
 *
 */
void scrivi_menu (giorno* menu, char nome_utente[], int i);

/**
 * La funzione conta_ricette_menu() riceve in ingresso un nome utente ed il numero corrispondente al giorno da estrarre da file ed analizzate.
 * Apre il file, estrare la struct nella posizione corrispondente al giorno_x, scorre tra gli alimenti contenuti nel menu di tale giorno analizzando il flag.
 * Quando trova un flag corrispondente ad 1 aumenta un contatore.
 * @param nome_utente da cui ricare il nome del file, giorno_x da cui ricavare la posizione del giorno che si intende analizzare.
 * @return contatore corrispondente al numero di ricette presenti nel giorno.
 */
short int conta_ricette_menu(char nome_utente[], short int giorno_x);


/**
 * La funzione conta_ricette_menu() riceve in ingresso un nome utente ed il numero corrispondente al giorno da estrarre da file ed analizzate.
 * Apre il file, estrare la struct nella posizione corrispondente al giorno_x, scorre tra gli alimenti contenuti nel menu di tale giorno analizzando il flag.
 * Quando trova un flag corrispondente ad 1 aumenta un contatore.
 * @param nome_utente da cui ricare il nome del file, giorno_x da cui ricavare la posizione del giorno che si intende analizzare.
 * @return contatore corrispondente al numero di ricette presenti nel giorno.
 */
void ricette_presenti (char vet_ricette[], char nome_utente[], short int giorno_x);

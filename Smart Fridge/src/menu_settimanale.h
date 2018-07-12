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
#ifndef MENU_SETTIMANALE_LIB
#define MENU_SETTIMANALE_LIB

/// Inclusione delle librerie standard
#ifndef STD_LIB
#define STD_LIB
	#include <stdbool.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
#endif

/// Inclusione della libreria matematica
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

#include "utenti.h"


/// DEFINIZIONE DELLE COSTANTI SIMBOLICHE.


/// Numero di elementi di determinati oggetti:
#define NUM_CIBI 5							/// numero massimo di cibi che l’utente può inserire per pasto
#define NUM_PASTI 5							///	numero dei pasti per ogni giorno pasto
#define NUM_GIORNI 7						/// numero dei  giorni della settimana
#define MIN_KCAL_GIORNATA 1800				/// kcal minime che un utente deve assumere per ogni giorno
#define MAX_KCAL_GIORNATA 5000				/// kcal massimo che un utente può assumere per ogni giorno

/// Lunghezze delle stringhe:
#define LUNG_GIORNO 15						/// lunghezza massima che la stringa contenente il nome del giorno della settimana può assumere
#define LUNG_CIBO 41						/// lunghezza massima che la stringa contenente il nome del cibo può assumere
#define LUNG_QUANTITA 11					/// lunghezza massima che la stringa contenente la quantità di un cibo
#define LUNG_PASTO 10						/// lunghezza massima che la stringa contenente il nome del pasto del giorno può assumere
#define LUNG_NOME_FILE_MENU 30				/// lunghezza massima che la stringa contenente il nome del file menu personale può assumere

#define FILE_MENU "../menu_"




//Definizione dei tipi di di dato

/**
 * @typedef cibo
 *
 * Il tipo di dato "cibo" serve a memorizzare una ricetta o un singolo alimento.
 *  E' basato su di una struct i cui membri sono: nome_cibo, flag (viene impostato a zero
 *  se si tratta di un alimento singolo, ad 1 se si tratta di una ricetta) e quantità.
 *
 */
typedef struct struct_cibo{
	char nome_cibo[LUNG_CIBO];
	char quantita[LUNG_QUANTITA]; //quantità
	short flag;
} cibo;

/**
 * @typedef pasto
 *
 * Il tipo di dato "pasto" serve a memorizzare il cibo che l'utente si impegna a cosumare durante un dato pasto (pranzo, cena, ...).
 * E' basato su di una struct i cui membri sono: nome_pasto e alimento.
 * Alimento è un dato strutturato di tipo "cibo".
 */
typedef struct struct_pasto {
	char nome_pasto[LUNG_PASTO];
	cibo cibi[NUM_CIBI];
} pasto;

/**
 * @typedef giorno
 *
 * Il tipo di dato "giorno" serve a memorizzare il cibo che l'utente si impegna a consumare durante un dato giorno della settimana.
 * E' basato su di una struct i cui membri sono: giorno, pasto e kcal.
 * Giorno indica il giorno della settimana.
 * Pasto è un dato strutturato di tipo "pasto".
 * kcal indica le kcal della giornata.
 */
typedef struct struct_giorno{
	int kcal;
	char nome_giorno[LUNG_GIORNO];
	pasto pasti[NUM_PASTI];
} giorno;


/**
 * La funzione crea_nome_file_menu() si occupa di concatenare delle stringhe con il nickname passato
 * in ingresso per poi restituire tale risultato
 *
 * @param nickname 			nome dell'utente per la generazione del nome del file
 */

char* crea_nome_file_menu(const char* nickname);


/**
 * La funzione inizializzazione() chiama una serie di funzioni utili ad un inizializzazione di un vettore di 7
 * struct di tipo giorno, ognuna delle quali rappresenta un giorno della settimana
 *
 * @param nome_utente      stringa da cui viene generato il nome file da inizializzare
 */
int inizializzazione (const char* nome_utente);

/**
 * La funzione inizializzazione_giorno() riceve in ingresso un puntatore
 * ad una variabile di tipo giorno e la sua posizione nel vettore creato nella funzione inizializzazione ().
 * La funzione assegnerà alla variabile giorno appartenente alla struct il nome di un giorno della settimana,
 * scegliendo tale nome sulla base della sua posizione nel vettore di struct a cui appartire.
 * Inoltre, imposta a zero le kcal della giornata.
 *
 * @param giornata varibile da modificare
 * @param num_giorno variabile per la scelta della modifica da effetuare
 * @return 0 se l'operazione va a buon fine
 */
int inizializzazione_giorno (giorno* giornata, int num_giorno);

/**
 * La funzione inizializzazione_pasti() riceve in ingresso un puntatore
 * ad una variabile di tipo giorno .
 * La funzione scorre nel vettore pasto e assegna ai suoi elementi, in base alla loro posizione nel vettore,
 *  il nome di un pasto, chiamando poi una funzione per l'inizializzazione dei singoli elementi appartenenti al pasto.
 *
 * @param menu varibile da modificare
 * @return 0 se l'operazione va a buon fine
 */
int inizializzazione_pasti (giorno* menu);

/**
 * La funzione inizializzazione_alimenti() riceve in ingresso un puntatore
 * ad una variabile di tipo giorno e l'indice del vettore della variabile al suo interno "pasto".
 * La funzione assegnerà alla variabile alimento, appartenente alla struct pasto, una stringa vuota al
 * nome_cibo, un flag impostato a -1 e una quantità pari a 0.
 *
 * @param giornata varibile da modificare
 * @param num_pasto variabile per impostare l'indice della varaibile pasto.
 * @return 0 se l'operazione va a buon fine
 */
int inizializzazione_alimenti (giorno* giornata, short int num_pasto);

/**
 * La funzione inizializzazione_file_menu() riceve in ingresso un vettore
 * di dato strutturato di tipo "menu".
 * La funzione scriverà su un file, il cui nome sarà il nickname dell'utente, il vettore ricevuto in ingresso.
 *
 * @param nome_utente nome da cui ricavare il nome file del menu personale
 * @param menu da scrivere su file
 * @return 0 se l'operazione va a buon fine
 */
int inizializzazione_file_menu (const char* nome_utente, giorno* menu );



/**
 * La funzione visualizza_database_menu() riceve in ingresso un vettore di carattere, corrispondente
 * al nome file che andrà ad aprire/creare.
 * La funzione stamperà su schermo il contenuto del fine.
 *
 * @param nome_utente per generare il nome del file del menu personale
 * @return 0 se l'operazione va a buon fine
 */
int visualizza_database_menu (char nome_utente[]);

/**
 * La funzione esiste_menu() riceve in ingresso il nome dell'utente
 * La funzione verificherà l'esistenza del menu dell'utente il cui nome è passato in ingrsso
 *
 * @param nome_utente per la generazione del nome file da controllare
 * @return 0 se il file non esiste, 1 se esiste, -1 se il nickname non è idoneo
 */
int esiste_menu(char* nome_utente);

/**
 * La funzione input_kcal_giornata() permette l'input delle kcal di una giornata.
 *
 * @return le kcal inserite dall'utente
 */
int input_kcal_giornata();

/**
 * La funzione input_numero_pasto() permette la selezione del pasto, ritornando tale valore.
 *
 * @return la selezione effettuata
 */
int input_numero_pasto();

/**
 * La funzione input_numero_giorno() permette la selezione del giorno che si intende modificare.
 *
 * @return la selezione effettuata
 */
int input_numero_giorno();

/**
 * La funzione input_nome_cibo() permette l'input da tastiera del nome del cibo da parte dell'utente.
 *
 * @return la stringa inserita da tastiera
 */
char* input_nome_cibo();

/**
 * La funzione input_quantita_cibo() permette l'input da tastiera della quantità del cibo, ritornando quest'ultima.
 *
 * @return la stringa inserita da tastiera
 */
char* input_quantita_cibo();

/**
 * La funzione input_flag_cibo() permette l'input da tastiera del flag corrispondente alla ricetto o all'alimento.
 *
 * @return la stringa inserita da tastiera
 */
int input_flag_cibo();



/**
 * La funzione scelta_pasto() non riceve nulla in ingresso.
 * La funzione si occuperà dell'autenticazione dell'utente e quindi
 * dell'estrazione dei suoi dati dal database degli utenti.
 * Fatto ciò, chiederà all'utente di scegliere il giorno della settimana su cui desidera effettuare le modifiche.
 * Dopodiché chiederà all'utente di scegliere uno dei cinque pasti disponibili, sucessivamente passerà l'indice corrispondente al pasto che
 * ha scelto e menu ad un'altra funzione per procedere ad un ulteriore modifica sulla base della scelta effettuata.
 * Scriverà il nuovo menu su file e visualizzerà il contenuto dell'intero file su schermo con le modifiche effettuate.
 *
 * @return 0 se l'operazione va a buon fine
 */
void modifica_menu();

/**
 * La funzione scelta_alimenti() riceve in ingresso un dato strutturato di tipo "giorno" ed un numero corrispondente all'indice del vettore
 * pasto contenuto nel dato strutturato menu.
 * La funzione chiederà all'utente di scegliere quale degli alimenti modificare, per poi procedere all'inserimento da tastiera dei nuovi
 * valori con cui sostituire i valori attuali.
 *
 * @param giornata su cui effettuare le modifiche.
 * @param num_pasto per selezionare il pasto da modificare
 * @return 0 se l'operazione va a buon fine
 */
void modifica_alimenti_pasto (giorno* giornata, short int num_pasto);

/**
 * La funzione estrazione_struct() riceve in ingresso un puntatore ad una struct di tipo giorno, il nome dell'utente e l'indice corrispondente
 * al giorno della settimana.
 * La funzione estrarrà n-esimo elemento dal file, lo memorizzerà nella variabile di tipo giorno ricevuta in ingresso e terminerà la sua funzione.
 *
 * @param giornata su cui memorizzare la struct da estrarre da file
 * @param nome_utente per aprire il menu corrispondente a tale utente
 * @param num_giorno per estrarre n-esimo elemento del file.
 * @return 0 se l'operazione va a buon fine
 */
int estrai_giorno (giorno* giornata, char* nome_utente, int num_giorno);


/**
 * La funzione estrai_kcal_menu() si occupa dell'estrazione delle kcal della giornata il cui numero corrispondente è passato in ingresso.
 *
 * @param kcal su cui memorizzare le kcal estratte
 * @param nome_utente per aprire il menu corrispondente a tale utente
 * @param num_giorno per estrarre n-esimo elemento del file
 * @return 0 se l'operazione va a buon fine
 */
int estrai_kcal_menu(int* kcal, char* nome_utente, int num_giorno);


/**
 * La funzione stampa_menu() riceve in ingresso un dato strutturato di tipo giorno e lo stampa su schermo.
 * @param giornata da stampare su schermo.
 * @return 0 se l'operazione va a buon fine
 */
void stampa_giorno (giorno* giornata);

/**
 * La procedura scrivi_menu() riceve in ingresso un dato strutturato di tipo giorno, il nome del file da aprire, la posizione dove andare a sovrascrivere.
 * Effettua una serie di concanetazioni per generare il nome del file su cui andare a scrivere basandosi sul nome utente passatogli, apre il file,
 * posizione il puntatore all'i-esima posizione e scrive in tale posizione il menu ricevuto in ingresso.
 * @param giornata da scrivere su file
 * @param nome_utente da cui generare il nome del file su cui scrivere
 * @param num_giorno corrispondente alla posizione su cui scrivere nel file.
 * @return esisto della funzione
 */
int scrivi_giorno (giorno* giornata, char* nome_utente, int num_giorno);


/**
 * La funzione scrivi_menu() si occupa dell'eliminazione del file corrispodente al nome_file passato in ingresso
 *
 * @param nome_file da scrivere su file
 * @return esisto della funzione
 */
int elimina_file_menu(char nome_file[LUNG_NOME_FILE_MENU]);


/**
 * La funzione gestore_visualizzazione_menu() si occupa della visualizzazione del menu
 *
 * @return esisto della funzione
 */
int gestore_visualizzazione_menu ();

/**
 * La procedura scrivi_menu() riceve in ingresso un vettore di cibi e confronta ciasciuna posizone per trascinsare
 * nelle posizioni più avanti quelli con nome diverso da stringa vuota.
 *
 * @param cibi puntatore a vettore di tipo char da analizzate
 *
 */
void ordina_cibi_pasto(cibo* cibi);

#endif

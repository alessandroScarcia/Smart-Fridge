/**
 * @file ricette.h
 *
 * @brief Libreria per la gestione delle ricette.
 *
* La libreria ricette.h è stata implementata per permettere la gestione delle ricette,
 * quindi le operazioni necessarie ai controlli, la manipolazione, il caricamento e il salvataggio
 * di informazioni legate alle ricette.
 * La gestione delle ricette in generale avviene tramite tre file: "nuove_ricette.csv", "database_ricette.csv".
 *  Rispettivamente sono utilizzati per: caricare nuove ricette per aggiungerle al database,
 * memorizzare le ricette in maniera da poterle consultare in qualsiasi momento o in alterniativa effettuare ricerche
 * mirate su quelle che é possibile preparare con gli alimenti disponibili.
 *
 *
 * @authors Alessandro Scarcia, Davide Quatela, Michela Salvemini
 */


#ifndef RICETTE_LIB
#define RICETTE_LIB

/// Inclusione delle librerie standard.
#ifndef STD_LIB
#define STD_LIB
	#include <stdbool.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
#endif

/// Inclusione delle librerie per la manipolazione di stringhe e caratteri.
#ifndef STRING_LIB
#define STRING_LIB
	#include <string.h>
	#include <ctype.h>
#endif

#include "consumi.h"

#include "alimenti.h"

#include "calorie.h"

#include "utenti.h"


	//***************************DEFINIZIONI DI COSTANTI***********************************

	//LUNGHEZZE DI STRINGHE
#define LUNG_NOME_RICETTA 21			/// Lunghezza in caratteri per la stringa del nome di una ricetta + carattere terminatore
#define LUNG_INGREDIENTE 21  			/// lunghezza massima della stringa che indica il nome di un ingrediente
#define LUNG_TEMPO_PREPARAZIONE 21		/// lunghezza massima della stringa che indica iltempo di preparazione
#define LUNG_PREPARAZIONE 501			/// lunghezza massima della stringa che indica la preparazione della ricetta
#define LUNG_COMPLESSITA 21				/// lunghezza massima della stringa che indica la complessità della ricetta
#define LUNG_TUPLA_RICETTE 1000			///lunghezza della tupla ossia una riga quanto deve essere grande
#define LUNG_STR_LAVORO_RIC 100 		///lunghezza della stringa che verrá utilizzata in varie funzioni per effettuare split o altre operazioni su stringhe
#define LUNG_UNITA_INGR 6				/// lunghezza dell'unita di misura utilizzata per l'ingrediente

	//NUMERO DI ELEMENTI DI ARRAY/MATRICI ECC
#define MAX_INGRD_SUGG 5					/// Limite degli alimenti che possono essere inseriti manualmente dall'utente per la ricerca di una ricetta
#define MAX_INGREDIENTI 20					/// numero massimo degli ingredienti che una ricetta può avere
#define MAX_NUM_FASI 20						/// numero massimo delle fasi che una ricetta può avere
#define MIN_DOSI 1							/// numero delle dosi minimo che un ingrediente può avere in una ricetta
#define MAX_DOSI 20							/// numero delle dosi massimo che un ingrediente può avere in una ricetta
#define NUM_CAMPI_RICETTA 6
#define NUM_DELIM 2
#define NUM_ALIM_SUGG_SCAD 2
#define NUM_CAMPI_INGREDIENTI 3

//COSTANTI DI RICERCA
#define MOD_RICERCA_AUTOMATICA 1 	///se passato 1 la ricerca delle ricette viene fatta in base agli alimenti presenti nel frigo e alla loro relativa quantitá
#define MOD_RICERCA_MANUALE 0		///se passato 0 la ricerca delle ricette viene fatta in base ai nomi degli alimenti inseriti dall'utente



// DIVISIONE CAMPI (leggi_campo_ricetta)
#define CAMPO_MULTIPLO 1					///ci aiuta a capire che lo split della stringa deve essere fatto contando il numero di caratteri delimitatori(vedi documentazione di leggi_campo_ricetta)
#define DELIMITATORE_PREPARAZIONE '-'		 ///carattere per la separazione della preparazione
#define SEQUENZA_DELIMITATORI "-"			///sequenza di caratteri su cui deve essere effettuato lo split della stringa quando non conosciamo il numero di campi



// Nomi dei file utilizzati:
#define FILE_DATABASE_RICETTE "../database_ricette.csv" ///file di memorizzione delle ricette prese in input
#define FILE_NUOVE_RICETTE "../nuove_ricette.csv" 		///file di estrazione delle ricette


//COSTANTI PER LA VISUALIZZAZIONE
#define MOSTRA_DATABASE "" 	   	///per mostrare l'intero database basta passare una stringa vuota alla funzione visualziza database
#define VISTA_TOTALE 0  	   	///ci permette di vedere tutte le informazioni riguardante/i la/e ricetta/e
#define VISTA_MINIMIZZATA 1    	///ci permette di vedere le informazioni di base riguardante/i la/e ricetta/e

	//*********************************DEFINIZIONI DI STRUCT***********************************

/**
 * @typedef ingrediente
 *
 * Il tipo di dato "ingrediente" è stato definito per memorizzare le caratteristiche di un ingrediente
 *  E' basato su di una struct i cui membri sono: nome, quantita, unita_misura ;
 *
 */
typedef struct {
	char nome[LUNG_INGREDIENTE];				  /// stringa per la memorizzazione del nome dell'ingrediente
	float quantita;              				 /// quantitativo da utilizzare nella ricetta di cui fa parte
	char unita_misura[LUNG_UNITA_INGR];			/// unita di misura dell'ingrediente (kg, litri, ...)
}ingrediente;

/**
 * @typedef ricette
 *
 * Il tipo di dato "ricette" è stato definito per memorizzare le caratteristiche di una ricetta
 *  E' basato su di una struct i cui membri sono: nome_ricetta, ingredienti, tempo_preparazione, preparazione, complessita, kcal_ricetta
 *  e dosi.
 *
 */
typedef struct {
	char nome_ricetta[LUNG_NOME_RICETTA];					/// stringa contenente il nome assegnato alla ricetta
	ingrediente ingredienti[MAX_INGREDIENTI];				/// vettore di tipo ingredienti per la memorizzazione degli ingredienti utili alla preparzione della ricetta
	char tempo_preparazione[LUNG_TEMPO_PREPARAZIONE];		/// vettore contenente il tempo di preparazione di una ricetta
	char preparazione[LUNG_PREPARAZIONE];					/// azioni da eseguire per preparare la ricetta
	char complessita[LUNG_COMPLESSITA];						/// complessità della ricetta (bassa, alta, ...)
	int kcal_ricetta;									    /// kcal totoali della ricetta in relazione agli ingredienti e alle loro quantità
	int dosi;												/// campione per le kcal calcolate
}ricetta;




//*********************************DEFINIZIONI DEI PROTOTIPI***********************************


int esiste_ricetta(char* nome_ricetta);


/**
 * Funzione che si occupa di estrarre gli ingredienti di una determinata ricetta.
 *
 * @param nome_ricetta			nome della ricetta da cui verranno estratti gli ingredienti
 * @param ingredienti			puntatore al primo elemento del vettore che conterrà gli ingredienti estratti
 * @return indice_ingrediente  numero ingredienti estratti dalla ricetta
 */
int estrazione_ingredienti(char nome_ricetta[LUNG_NOME_RICETTA], ingrediente* ingredienti);

/**
 * Funzione che si occupa di estrarre le kcal della ricetta con nome corrispondente passato in ingresso, scrivendo quest'ultime nella variabili a cui
 * punta il puntatore kcal passato in ingresso insieme alle dosi a cui corrispondono le kcal, che verranno inserite nella variabili a cui
 * punta il puntatore dosi.
 *
 * @param nome_ricetta				nome della ricetta da cui verranno estratte kcal e dosi per tali kcal
 * @param kcal						puntatore alla variabile dove verranno scritte le kcal estratte
 * @param dosi						puntatore alla variabile dove verranno scritte le dosi per le kcal estratte
 * @return 0 se la ricetta e/o il file non esiste, 1 se è riuscito a reperire le informazioni richieste
 */
int estrai_kcal_ricetta(char* nome_ricetta, int* kcal, int* dosi);

/**
 * Funzione che si occupa di controllare quali delle ricette del database hanno gli ingredienti corrispondenti
 * a il contenuto del frigo, calcolando perciò il numero delle ricette trovate.
 *
 * @return num_ricette_preparabili ovvero il numero delle ricette preparabili
 */
int conta_ricette_preparabili();

/**
 * Funzione che si occupa di ridurre le quantita degli alimenti prensenti nel frigo che vengono usati come ingredienti
 * della ricetta il quale nome e dosi vengono passate in ingresso alla funzione.
 *
 * @return 0 se la ricetta non viene viene trovata, i file non viene trovato o gli ingredienti non bastano per la preparazione della ricetta
 * @return 1 se vengono registrati i consumi correttamente
 */
int prepara_ricetta(char* nome_ricetta, int dosi_ricetta);

/**
 * Procedura che si occupa di verificare se l'utente vuole inserire il cosumo della ricetta nel proprio file assunzioni o
 * se preferisce preparare la ricetta in anonimo. Inoltre, suggerite le ricerte preprabili, l'utente inserirà il nome della
 * ricetta che vuole preprara, verificata la fattibilità di quest'ultima
 * verrà registrata tra le assunzioni dell'autente auteticato, se quest'ultimo non avrà preferito il consumo in forma anonima.
 *
 */
void gestore_prepara_ricetta();


/**
 * Serve ad effettuare una tokenizzazione della stringa che gli passiamo
 *
 * @param linea				stringa che dovrá essere divisa in token
 * @param num_campo			numero del campo da estrarre dalla stringa
 * @param flag_campo		tipologia di estrazione da effettuare. Qualora sia 1 bisogna effettuare il conteggio dei campi presenti all'interno
 * 							della stringa
 *
 * @return NULL				se abbiamo terminato di analizzare la riga
 * @return "vuoto"			se il campo stringa da estrarre é assente
 * @return tok				se l'estrazione ha avuto successo(il token sarebbe il valore)
 */
char* leggi_campo_ricetta(char* linea, int num_campo, short flag_campo);




/**
 * Serve a leggere da file .csv le ricette nuove
 *
 * @return 1				in caso di successo di estrazione dati da file
 */
int lettura_nuove_ricette();




/**
 * Funzione che serve a visualizzare i dati relativi ad una singola ricetta
 *
 * @param dati_ricette		struct che contiene i dati della ricetta
 * @param vista_ricetta		flag che in base ad un controllo permette di visualizzare tutti i dati della ricetta o solo quelli principali
 *
 * @return 1				in caso di successo nella visualizzazione
 */
int visualizza_ricetta(ricetta dati_ricetta, int vista_ricetta);




/**
 * permette la visualizzazione del database o di una singola ricetta in base all'id che passiamo.
 * Inoltre possiamo decidere in che modalitá vedere i dati(0-->totale, 1-->minimizzata)
 *
 * @param nome_ricetta		nome della ricetta da visualizzare in caso possieda un valore. In caso sia una stringa vuota viene mostrato l'intero database
 * @param id_ricetta_pers	id che viene mostrato a schermo a seguito di una ricerca o altre operazioni
 * @param vista				flag che in base ad un controllo permette di visualizzare tutti i dati della ricetta o solo quelli principali
 * @return 1				in caso di successo nella visualizzazione
 */
int visualizza_database_ricette(int vista);




/**
 * Serve a memorizzare la ricetta che passiamo all'interno di un file con accesso binario che fungerá da database
 *
 * @param nuova_ricetta		struct che possiede i dati della ricetta da aggiungere al database
 * @return 1				in caso di successo nell'aggiornamento del database
 */
int aggiorna_database_ricette(ricetta nuova_ricetta);




/**
 * Si occupa di eliminare una ricetta in base all'id inserito dall'utente. La riga che conteneva la ricetta viene inizializzata
 *
 * @return 1				in caso di successo nell'eliminazione
 */
int elimina_ricetta();




/**
 * La funzione suggerirá le ricette che si possono effettuare con gli ingredienti a disposizione nel frigo
 *
 * @param alimenti_frigo	Elenco degli alimenti del frigo su cui effettuare la ricerca della ricetta
 * @param num_alimenti		numero degli alimenti presenti nel frigo
 * @return 1				in caso di successo nella visualizzazione di un suggerimento
 */
int suggerimento_ricetta_automatico();




/**
 * Suggerisce le ricette che si possono fare con gli alimenti inseriti manualmente dall'utente
 *
 * @param num_alimenti		numero di ingredienti su cui effettuare la ricerca
 * @param nome_alimenti		nome degli alimenti su cui effettuare a ricerca della ricetta
 * @return 1				in caso di successo nella visualizzazione di un suggerimento
 */
int suggerimento_ricetta_manuale(int num_alimenti, char nome_ingredienti[MAX_INGRD_SUGG][LUNG_INGREDIENTE]);




/**
 * Ordina le ricette presenti nel database in base alle kcal che producono. L'ordinamento viene effettuato tramite shell sort
 *
 * @param ricette_database	array di struct che possiede tutte le ricette presenti nel database su cui effettuare l'ordinamento
 * @param num_ricette		numero delle ricette su cui effettuare l'ordinamento
 * @return 1				in caso di successo nell'ordinamento
 */
int ordina_ricette_kcal(ricetta* ricette_database, int num_ricette);




/**
 * Funzione che si occupa di inizializzare l'array di puntatori che conterrá il nome delle ricette preparabili con valori NULLI
 *
* @param num_ricette		 numero delle ricette che si devono inizializzare
* @param ricette_preparabili array di stringhe che devono essere inizializzate
* @return 1					 in caso di successo nell'inizializzazione
*/
int inizializza_ricette_preparabili(int num_ricette, char* ricette_preparabili[num_ricette]);




/**
 * conta le righe(le ricette) presenti nel database delle ricette escludendo le righe inizializzate(vuote)
 *
 * @return righe numero di righe presenti nel database
 */
int conta_ricette_database();




/**
 * popola un array di struct con le ricette presenti nel database.
 * Questa funzione serve a non effettuare ogni volta la lettura del file all'interno di altre funzioni
 *
 * @param ricette_database	array di struct che verrá riempito con le ricette del database
 * @return 1				in caso di successo nella lettura dei valori
 */
int lettura_database_ricette(ricetta* ricette_database);




/**
 * funzione di ricerca ricette, ottimizzata per gli alimenti che stanno per scadere all'interno del frigo
 *
 * @param alimenti_frigo	elenco degli alimenti del frigo(ordinati per scadenza) su cui effettuare la ricerca della ricetta
 * @param num_alimenti		numero di alimenti presenti nel frigo
 * @return 1				in caso di successo
 */
int ricette_alimenti_in_scadenza();




/**
 * funzione che serve ad inserire manualmente gli ingredienti da usare come paroli chiave per la ricerca di ricette che possiedono tali ingredienti
 *
 * @param nome_alimenti				matrice che conterrá i nomi degli ingredienti su cui effettuare la ricerca della ricetta
 * @return num_alimenti_inseriti++	indice incrementato che corrisponde al numero effettivo di ingredienti inseriti dall'utente
 */
int inserimento_manuale_ingredienti(char nome_alimenti[MAX_INGRD_SUGG][LUNG_INGREDIENTE]);




/**
 * funzione che si occupa di modificare un campo di una determinata ricetta inseriti dall'utente, con un nuovo valore anch'esso inserito
 * manualmente
 *
 * @return 1 			in caso di sucesso della funzione
 */
int modifica_ricetta();




/**
 * funzione che si occupa dell'inserimento di un nuovo valore per il campo porzione
 *
 * @return porzione				nuova porzione inserita dall'utente
 */
int input_dosi_ricetta();




/**
 * funzione che si occupa dell'inserimento di un nuovo ingrediente per una data ricetta
 *
 * @return ingrediente 			nuovo ingrediente inserito dall'utente
 */
char* input_ingredienti_ricetta();




/**
 * funzione che si occupa dell'inserimento di un nuovo valore per il campo preparazione
 *
 * @return preparazione 		nuova preparazione inserita dall'utente
 */
char* input_preparazione_ricetta();




/**
 * funzione che si occupa dell'inserimento di un nuovo valore per il campo complessitá
 *
 * @return complessita 			nuova complessita inserita dall'utente
 */
char* input_complessita_ricetta();




/**
 * funzione che si occupa dell'inserimento di un nuovo valore per il campo tempo di preparazione
 *
 * @return tempo_preparazione 	nuovo tempo di preparazione inserito dall'utente
 */
char* input_tempo_preparazione();




/**
 * funzione che si occupa dell'inserimento di un nuovo valore per il campo nome ricetta
 *
 * @return nome_ricetta		    nuovo nome della ricetta inserito dall'utente
 */
char* input_nome_ricetta();

/**
 * funzione che si occupa dell'inserimento di un id positivo di una ricetta
 *
 * @return id	     preso in input
 */
int input_id_ricetta(int max_id);

#endif

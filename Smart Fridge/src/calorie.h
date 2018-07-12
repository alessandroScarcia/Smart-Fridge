/**
 * @file calorie.h
 *
 * @brief Libreria per la gestione delle calorie.
 *
 * La libreria calorie.h è stata implementata per permettere all'utente
 * la gestione delle proprie assunzioni con il corrispettivo valore
 * calorico.
 *
 * @authors Alessandro Scarcia, Davide Quatela, Michela Salvemini
 */

#pragma once

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


#include "alimenti.h"

/// Inclusione della libreria per la gestione delle ricette
#include "ricette.h"

/// Inclusione della libreria per la gestione degli utenti
#include "utenti.h"

/// Inclusione della libreria per la gestione delle date
#include "date.h"

/// Inclusione della libreria per la gestione dei consumi
#include "menu_settimanale.h"

/// Inclusione della libreria per la pulizia dei flussi di input
#include "pulizia_flussi.h"


#define FLAG_INPUT_LIBERO  3
#define KCAL_MEDIE_GIORNALIERE 2000
#define CAMPIONE_ISTOGRAMMI 200

/// Costanti per i nome del file assunzione
#define PREFIX_FILE_ASSUNZIONI "../assunzioni_"		/// Parte iniziale del nome di un file assunzioni
#define SUFFIX_FILE_ASSUNZIONI ".dat"				/// Parte finale del nome di un file assunzioni
#define LUNG_FILE_ASSUNZIONI 50						/// Lunghezza massima in caratteri del nome del file assunzioni

#define LUNG_NOME_ASSUNZIONE 21						/// Lunghezza massima in caratteri del nome di un assunzione

#define MIN_KCAL 0									/// Valore minimo per le kcal usate nel calcolo relativo ad un assunzione
#define MAX_KCAL 5000								/// Valore minimo per le kcal usate nel calcolo relativo ad un assunzione

/**
 * @typedef assunzione
 *
 * Il tipo di dato "assunzione" serve a memorizzare un assunzione della giornata corrente di un utente.
 *  E' basato su di una struct i cui membri sono: nome, quantita, kcal, e flag (per identificare se si tratta di una ricetta o aliment).
 *
 */
typedef struct{
	char nome[LUNG_NOME_ASSUNZIONE];
	float quantita;
	unsigned short int kcal;
	int flag;
} assunzione;


/**
 * Questa funzione ha il compito di richiedere all'utente di inserire il nome relativo ad un assunzione.
 * Il valore inserito dall'utente viene controllato, verificando che sia della lunghezza massima stabilita.
 * Se corretto, viene restituito il puntatore alla stringa del nome inserito.
 *
 * @return puntatore alla stringa inserita dall'utente
 */
char* input_nome_assunzione();

/**
 * Questa funzione ha il compito di richiedere all'utente il valore di kcal relative ad un assunzione libera.
 * Ilvalore viene inserito, controllando che sia significativo nel formato e nel valore.
 * Se corretto, lo stesso viene restituito.
 *
 * @return kcal inserite dall'utente
 */
unsigned short int input_kcal_libere();


/**Questa funzione ha il compito di popolare una struct di tipo assunzione passatagli in ingresso, con i dati
 * inseriti dal'utente. Per fare ciò chiede all'utente di inserire 0 se ha intenzione di inserire un alimento
 * oppure 1 se si tratta di una ricetta. Fatta tale distinzione, vengono chiamate opportune funzioni per
 * l'estrazione dal database del tipo di dato che si deve gestire, permettendo il popolamento della struct
 * passata in ingresso.
 *
 * @param cibo			    struct che conterrà il nome, la quantità e le calorie dell'alimento assunto dall'utente
 *
 * @return -1				se l'utente vuole smettere di inserire alimenti
 * @return 1				se tutto avviene in modo corretto
 *
 */
int input_alimento_consumato ();




/**La funzione calcolo_kcal_alimento() ha il compito di calcolare le kcal di un alimento basandosi sulla quantità,
 * il tutto attraverso una proporzione.
 *
 * @param kcal				numero intero corrispondente alle kcal per un dato campione
 * @param campione			numero intero che rappresenta la quantità corrispondente alle kcal
 *
 * @return 0 se i dati inseriti non sono validi
 * @return risultato_kcal	kcal per il quantitativo inserito
 */
unsigned short int calcolo_kcal(unsigned short int kcal, int campione, float q_consumata);


/**La funzione aggiorno_database_calorie() ha il compito di scrive su file assunzioni relativo all'utente,
 * il quale nickname viene passato come parametro di ingresso alla fuzione, il cibo assunto.
 *
 * @param cibo			    struct da scrivere su file
 * @param nickname			nome con cui creare il nome del file su cui scrivere
 *
 *@return -1 				se il file non esiste e non è possibile crearne uno
 *@return  1				se la funzione riesce a svolgere il suo compito
 */
int aggiorno_database_calorie(char nome_consumo[], int flag_consumo, float quantita_consumo, char nickname[]);

/**La funzione inizializza_file_assunzione() ha il compito di resettare il file e scrivere in prima posizione la data
 * odierna.
 *
 * @param nickname			nome con cui creare il nome del file su cui scrivere
 *
 * @return -1 				se non riesce a creare il file
 * @return 	1			se riesce a svolgere il suo compito
 *
 */
int inizializza_file_assunzione(char nickname[]);

/**La funzione apertura_file_assunzioni() ha il compito di aprire il file se l'autenticazioen va a buon fine e
 * verificare che la data presente sia quella odierna servendosi di una funzione che calcola la differenza tra le
 * due date passategli in ingresso. Se la funzione restituisce zero, si procede all'input dell'alimento consumato,
 * altrimenti viene resettato il file e scritta la nuova data, solo dopo si procede all'input degli alimenti.
 *
 *@return 0 				se l'autenticazione non ha successo
 *@return -1 				se non riesce ad aprire e creare il file
 *@return 1					se la funzione riesce a svolgere il suo compito
 */
int apertura_file_assunzioni();

/**La funzione stampa_database_assunzioni() ha il compito di aprire il file tutti i suoi elementi al suo interno
 * stampandoli su schermo.
 *
 *@return -1 				se non riesce ad aprire e creare il file
 *@return 1					se la funzione riesce a svolgere il suo compito
 */
int stampa_database_assunzioni ();

/**La funzione calcolo_kcal_totali() ha il compito di aprire il file, posizionare il puntatore
 * dopo la data e leggere  tutti i suoi elementi al suo interno accumulando le kcal di ciauscino in una variabile
 * per poi essere stampata su schermo.
 *
 * @param nomefile			stringa utile all'apertura del file
 *
 *@return -1 				se non riesce ad aprire e creare il file
 *@return 1					se la funzione riesce a svolgere il suo compito
 */
unsigned short calcolo_kcal_totali(char* nomefile);

/**La funzione creazione_assunzioni() ha il compito di creare il file assunzioni.
 *
 *
 *@return 0				    se l'autenticazione non va a buon fine
 *@return 1					se la funzione riesce a svolgere il suo compito
 */
int creazione_assunzioni ();

/**La funzione modifica_assunzione() ha il compito di modifcare una delle struct presenti nel file, su richiesta dell'utente.
 *
 *@return 0 				se non riesce ad aprire il file o se l'autenticazione fallisce
 *@return 1					se la funzione riesce ad effetturare la modifica correttamente
 */

int modifica_assunzione ();

/**La funzione ricerca_assunzione_database() ha il compito di aprire il file assunzioni legato all'utente il cui nome gli viene passato in
 * ingresso, cercare nel file una struct di tipo assunzione che abbia la stringa nome uguale  alla varibile nome passata in ingresso.
 * Se la trova, ritorna la sua posizioen nel file.
 *
 *
 *@param nome				variabile per la ricerca nel file
 *@param nickname			variabile per aprire il file legato all'utente
 *
 *@return posizione 		se trova la struct con nome corrispondente al nome passato in ingresso
 *@return 0					se non trova una struct con nome corrispondente al nome passato in ingresso
 */
short int ricerca_assunzione_database (assunzione* nuova_assunzione, char nickname[]);

/**La funzione scrittura_diretta_assunzione() ha il compito di scrivere sul file corrispondente all'utente il quale nickname viene passato
 * in ingresso, la struct pasastagli in ingresso.
 *
 *
 *@param nickname			variabile per l'apertura del file corrispondente all'utente
 *@param cibo				struct da scrivere su file
 *
 *@return 1					se il file viene aggiornato correttamente
 */
int scrittura_diretta_assunzione (assunzione* cibo, char nickname[]);

/**La procedura istogrami() ha il compito di stampare su schermo le calorie che l'utente dovrebbe assumere in base al suo menu di riferimento
 * e quelle che effettivamente assunto in base al file assunzioni.
 *
 */
void istogrami ();



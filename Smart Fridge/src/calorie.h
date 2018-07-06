/*
 * calorie.h
 *
 *  Created on: 20 giu 2018
 *      Author:ciaone
 */
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


#ifndef UTENTI_LIB
#include "utenti.h"
#endif

/// Inclusione della libreria per la gestione delle date

#include "date.h"
/// Inclusione della libreria per la gestione delle ricette
#ifndef RICETTE_LIB
#include "ricette.h"
#endif

/// Inclusione della libreria per la gestione dei consumi

#include "menu_settimanale.h"




/// Inclusione della libreria per la pulizia dei flussi di input

#include "pulizia_flussi.h"

#ifndef CALORIE_H_
#define CALORIE_H_

#define LUNG_FILE_CALORIE 50
#define LUNG_NOME_ASSUNZIONE 50

typedef struct{
	char nome[LUNG_NOME_ASSUNZIONE];
	float quantita;
	unsigned short int kcal;
	int flag;
} assunzione;

#endif/* CALORIE_H_ */

/**Questa funzione ha il compito di popolare una struct di tipo assunzione passatagli in ingresso, con i dati
 * inseriti dal'utente. Per fare ciò chiede all'utente di inserire 0 se ha intenzione di inserire un alimento
 * oppure 1 se si tratta di una ricetta. Fatta tale distinzione, vengono chiamate opportune funzioni per
 * l'estrazione dal database del tipo di dato che si deve gestire, permettendo il popolamento della struct
 * passata in ingresso.
 *
 * @param cibo			    struct che conterrà il nome, la quantità e le calorie dell'alimento assunto dall'utente
 *
 * @return -1				se l'utente vuole smettere di inserire alimenti
 * @return 0				se tutto avviene in modo corretto
 *
 */
int input_alimento_consumato ();

/**La funzione calcolo_kcal_alimento() ha il compito di calcolare le kcal di un alimento basandosi sulla quantità,
 * il tutto attraverso una proporzione.
 *
 * @param kcal				numero intero corrispondente alle kcal per un dato campione
 * @param campione			numero intero che rappresenta la quantità corrispondente alle kcal
 *
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
 *@return  0				se la funzione riesce a svolgere il suo compito
 */
int aggiorno_database_calorie(char nome_consumo[], int flag_consumo, float quantita_consumo, char nickname[]);

/**La funzione scrivi_data() ha il compito di resettare il file e scrivere in prima posizione la data
 * odierna.
 *
 * @param nickname			nome con cui creare il nome del file su cui scrivere
 *
 * @return -1 				se non riesce a creare il file
 * @return 0				se riesce a svolgere il suo compito
 *
 */
int scrivi_data(char nickname[]);

/**La funzione apertura_file_assunzioni() ha il compito di aprire il file se l'autenticazioen va a buon fine e
 * verificare che la data presente sia quella odierna servendosi di una funzione che calcola la differenza tra le
 * due date passategli in ingresso. Se la funzione restituisce zero, si procede all'input dell'alimento consumato,
 * altrimenti viene resettato il file e scritta la nuova data, solo dopo si procede all'input degli alimenti.
 *
 *@return 1 				se l'autenticazione non ha successo
 *@return -1 				se non riesce ad aprire e creare il file
 *@return 0					se la funzione riesce a svolgere il suo compito
 */
int apertura_file_assunzioni();

/**La funzione stampa_database_assunzioni() ha il compito di aprire il file tutti i suoi elementi al suo interno
 * stampandoli su schermo.
 *
 *@return -1 				se non riesce ad aprire e creare il file
 *@return 0					se la funzione riesce a svolgere il suo compito
 */
int stampa_database_assunzioni ();

/**La funzione calcolo_kcal_totali() ha il compito di aprire il file, posizionare il puntatore
 * dopo la data e leggere  tutti i suoi elementi al suo interno accumulando le kcal di ciauscino in una variabile
 * per poi essere stampata su schermo.
 *
 * @param nomefile			stringa utile all'apertura del file
 *
 *@return -1 				se non riesce ad aprire e creare il file
 *@return 0					se la funzione riesce a svolgere il suo compito
 */
int calcolo_kcal_totali (char nomefile[]);

/**La funzione creazione_assunzioni() ha il compito di creare il file assunzioni.
 *
 *
 *@return 1				    se l'autenticazione non va a buon fine
 *@return 0					se la funzione riesce a svolgere il suo compito
 */
int creazione_assunzioni ();

/**La funzione modifica_assunzione() ha il compito di modifcare una delle struct presenti nel file, su richiesta dell'utente.
 *
 *@return 1 				se non riesce ad aprire il file o se l'autenticazione fallisce
 *@return 0					se la funzione riesce ad effetturare la modifica correttamente
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
short int ricerca_assunzione_database (char nome[], char nickname[]);

/**La funzione scrittura_diretta_assunzione() ha il compito di scrivere sul file corrispondente all'utente il quale nickname viene passato
 * in ingresso, la struct pasastagli in ingresso.
 *
 *
 *@param nickname			variabile per l'apertura del file corrispondente all'utente
 *@param cibo				struct da scrivere su file
 *
 *@return 0					se il file viene aggiornato correttamente
 */
int scrittura_diretta_assunzione (assunzione* cibo, char nickname[]);

/**La procedura istogrami() ha il compito di stampare su schermo le calorie che l'utente dovrebbe assumere in base al suo menu di riferimento
 * e quelle che effettivamente assunto in base al file assunzioni.
 *
 */
void istogrami ();


/**
 * file_operation.h
 *
 *  Created on: 26 apr 2018
 *      Author: david
 */


	///***********************************LIBRERIE INCLUSE***********************************
#ifndef STD_LIB
#define STD_LIB
	#include <stdbool.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
#endif

#ifndef STRING_LIB
#define STRING_LIB
	#include <string.h>
	#include <ctype.h>
#endif

#include "date.h"

#ifndef ALIMENTI_LIB
#define ALIMENTI_LIB

///DEFINIZIONE DELLE COSTANTI SIMBOLICHE

/// Lunghezze delle stringhe:
#define LUNG_NOME 20 					///	Lunghezza massima per il nome degli alimenti
#define LUNG_CAMPIONE 10 				///	Lunghezza della stringa che ospiterá il campione di riferimento per un alimento. Indica la quantitá che ha un determinato rapporto calorico
#define LUNG_SOGLIA 10 					/// Lunghezza della stringa che ospiterá la soglia minima degli alimenti(la quantitá minima da avere in frigo)
#define LUNG_NOME_FILE 100 				///	Lunghezza da riservare al nome del file
#define LUNG_UNITA_MISURA 5					/// Lunghezza della stringa che ospiterá l'unitá di misura
#define LUNG_TUPLA 100					/// Lunghezza della tupla ossia una riga quanto deve essere grande
#define LUNG_RIGA_FGETS 100 			/// Lunghezza massima in caratteri delle righe estratte con fgets() dai file

/// Numero di membri che compongono determinati elementi:
#define NUM_CAMPIONI 3 					///numero dei campioni di riferimento per gli alimenti
#define NUM_CAMPI_ALIMENTO_FRIGO 6 		///numero di colonne che possiede il file contenente i dati degli alimenti comprati

/// Nomi dei file utilizzati:
#define FILE_SPESA "spesa_effettuata.csv"
#define FILE_FRIGO "alimenti_frigo.dat"
#define FILE_DATABASE "database_alimenti.dat"

/// Unità di misura significative:
#define UNITA_KG "kg"					/// Unità di misura dei chilogrammi
#define UNITA_G "g"						/// Unità di misura dei grammi
#define UNITA_L "l"						/// Unità di misura dei litri
#define UNITA_ML "ml"					/// Unità di misura dei millilitri
#define UNITA_PZ "pz"					/// Unità di misura dei pezzi

/// Campioni di quantità per il calcolo delle calorie
#define CAMPIONE_G 100					/// Campione per i grammi
#define CAMPIONE_ML 100					/// Campione per i millilitri
#define CAMPIONE_PZ 1					/// Campione per i pezzi

/// Fattori di conversione delle unità di misura
#define KG_TO_G 1000					/// Conversione da chilogrammi a grammi
#define L_TO_ML 1000					/// Converzione da litri a millilitri

/// Limiti/soglie per determinati valori
#define OPZIONE_MINIMA 0 				///ALE: utilizza questa per controllare la scelta minima del menu
#define OPZIONE_MASSIMA 2 				///ALE: utilizza questa per controllare la scelta massima del menu
#define SOGLIA_PERC 20 					///la percentuale di riferimento al di sotto della quale un alimento sta finendo
#define	MIN_QUANTITA 1					/// Limiti per le quantità degli alimenti inseriti in relazione all'unità di misura
#define MAX_QUANTITA_G 10000			/// Limite superiore grammi
#define MAX_QUANTITA_ML 10000			/// limite superiore millilitri
#define MAX_QUANTITA_PZ 100				/// Limite superiore pezzi


///**********************************DEFINIZIONI DI STRUCT***********************************

///Questa struttura é utilizzata per i file binari per a memorizzazione dei dati relativi ad un alimento
typedef struct {
	char nome[LUNG_NOME];
	int campione_kcal;
	float soglia_spesa;
	char unita_misura[LUNG_UNITA_MISURA];
	unsigned short int kcal;
}alimento_database;

///Questa struttura é utilizzata per il file ad accesso sequenziale degli alimenti comprati
typedef struct{
	char nome[LUNG_NOME];
	float quantita;
	char unita_misura[LUNG_UNITA_MISURA];
	data scadenza;
}alimento_frigo;



void pulisci_stdin();


/**
 * La funzione controlla_unita_misura() determina se l'unità di misura ricevuta
 * è significativa o no.
 *
 * @param s Stringa contenente l'unità di misura da controllare
 * @return 1 se l'unita di misura è valida
 * @return 0 se l'unita di misura non è valida
 */
int controlla_unita_misura(char* unita_misura);


/**
 * La funzione controlla_quantita() determina se la quantità inserita rispetta
 * i limiti stabiliti relativamente all'unità di misura corrispondente.
 *
 * @param quantita Quantità su cui effettuare il controlo
 * @param unita_misura Unità di misura della quantita da controllare
 * @return 1 se la quantità è valida
 * @return 0 se la quantita non è valida
 */
int controlla_quantita(float quantita, char* unita_misura);


/**
 * La funzione controlla_data_scadenza() determina se la data di scadenza inserita è
 * significativa oppure no. Se è seignificativa stabilisce se l'alimento a cui si riferisce
 * è scaduto oppure no.
 *
 * @param scadenza Data con cui effettuare i controlli
 * @return 1 Se l'alimento non è scaduto
 * @return 0 Se l'alimento è scaduto
 * @return -1 Se la data non è significativa
 */
int controlla_data_scadenza(data scadenza);


/**
 * La funzione converti_unita_misura() permette di convertire una quantita
 * dalla sua unità di misura in una
 *
 * @param s
 * @return
 */
int converti_quantita(float *quantita, char *unita_misura);


/**
 * La funzione abbassa_maiuscole, riceve una stringa e ne converte le lettere
 * maiuscole in essa contenute in lettere minuscole.
 *
 * @param s Stringa in cui abbassare le lettere maiuscole
 */
void abbassa_maiuscole(char *s);


/**
 * La funzione input_alimento_frigo() permette la richiesta all'utente di inserire un alimento
 * destinanto al frigo.
 *
 * @return L'alimento inserito dall'utente
 */
alimento_frigo input_alimento_frigo();


/**
 * La procedura input_unita_misura() permette la richiesta all'untente di un unità di misura valida.
 */
void input_unita_misura(char *unita_misura);


/**
 * La funzione input_quantita() permette la richiesta all'untente di una quantita valida.
 *
 * @param unita_misura Unità di misura per la quantità da richiedere
 * @return Quantita inserita dall'utente
 */
float input_quantita(char *unita_misura);


/**
 * La funzione input_data_scadenza() permette di richiedere l'inserimento all'utente
 * di una data di scadenza valida.
 *
 * @return Data di scadenza inserita  dall'utente
 */
data input_data_scadenza();


/**La funzione aggiorna_database permette al programma(o al frigo se vogliamo avere una visione piú realistica del prodotto)
 * di "imparare" gli alimenti che l'utente consuma. Una volta memorizzati i dati dell'alimento quali kcal e campione di riferimento
 * sará piú semplice ed efficace gestire le abitudini dell'utente riguardo il consumo di kcal e la generazione di ricette con
 * un valore nutrizionale reale. Ovviamente la funzione deve conoscere gli alimenti che abbiamo inserito nel frigo e il relativo numero.
 * Una volta iniziato il confronto tra gli alimenti giá memorizzati e quelli recentemente aggiunti si decide, a seconda se un alimento é
 * conosciuto o meno, di aggiungerlo al database*/
int aggiorna_database(alimento_frigo alimento);


/**La funzione aggiorna_frigo una volta conosciuti il numero di alimenti da caricare e i dati a riguardo effettua la memorizzazione dei nuovi alimenti comprati
 * all'interno del file alimenti_frigo. La scrittura nel file in questo caso é ottimizzata in quanto si é previsto che, qualora ci siano righe inizializzate
 * ottenute dalla cancellazione di alimenti che non sono piú presenti nel frigo, esse vengano riutilizzate per una nuova memorizzazione. In questa maniera
 * evitiamo algoritmi di compattamento delle righe "piene"*/
int aggiorna_frigo(alimento_frigo alimento);


/**La funzione visualizza_database apre in lettura il file database_alimenti e ne mostra il contenuto. Questa funzione puó tornare utile non
 * solo all'utente ma anche al programmatore in fase di Debug*/
int  visualizza_database_alimenti();


/**La funzione visualizza_frigo oltre ad essere una funzione a se stante che permette di visualizzare il contenuto del frigo(del file alimenti_frigo.csv)
 * serve nella fase di riduzione della quantitá di un alimento. Avendo una lista a portata di mano con tanto di indice per ogni alimento
 * permettiamo all'utente di risparmiare tempo e di avere un quadro semplificato del contenuto del frigo*/
int visualizza_frigo();


/**La funzione riduci_q_alimenti é la funzione che permette all'utente di gestire la quantitá degli alimenti che vengono consumati.
 * Se l'utente ha comprato 6 uova e ne ha consumate 4 il file che contiene i dati relative alle quantitá deve essere aggiornato opportunamente
 * permettendo cosí in un secondo momento di mostrare la quantitá rimanente(nell'esempio ne verranno poi mostrate 2). Se siamo nel caso in cui invece
 * l'utente esaurisce un determinato alimento esso deve essere "cancellato" dalla lista degli alimenti presenti nel frigo. Dal punto di vista
* progettuale si é previsto che la riga contenente l'alimento ormai consumato non deve essere cancellata del tutto ma solo inizializzata. Questo
* perché attraverso un confronto dei nomi si puó risalire alla riga "vuota" per recuperarla inserendo cosí un nuovo alimento senza dover ogni volta
* effettuare un algoritmo di compattamento dei record del file*/
int riduci_q_alimenti();


/**La procedura lettura file si occupa principalmente di prelevare i dati da un file che chiameremo spesa_effettuata.csv
 * per poi inserirli di volta in volta all'interno di una struct realizzata su misura. Ovviamente i dati verranno inseriti
 * per ogni alimento(riga se vogliamo definirlo piú banalmente) che verrá estratto dal file. */
int caricamento_spesa();


/**
 *	La funzione input_kcal() permette la richiesta di inserimento all'utente dei valori di kcal relativi al corrispondente campione
 *	memorizzato nel database.
 * @param	nome_alimento Nome dell'alimento per cui inserire le kcal
 * @param	campione_kcal	Campione di alimento per cui inserire le kcal
 * @param	unita_misura	Unità di misura per il relativo alimento
 * @return	kcal inserite dall'utente
 */
int input_kcal(char *nome_alimento, int campione_kcal, char *unita_misura);


/**
 * La funzione calcola_campione_kcal() determina la quantita campione relativa alle calorie di un
 * determinato elemento, in base all'unità di misura ad esso associata.
 *
 * @param unita_misura Unità di misura dell'alimento inserito
 * @return campione da utilizzare per il calcolo delle calorie
 */
int calcola_campione_kcal(char *unita_misura);


/**
 * La funzione input_soglia_spesa() permette la richiesta all'utente di una soglia di quantità
 * al di sotto del quale un dato alimento deve essere inserito nella lista della spesa.
 *
 * @param nome_alimento	Nome dell'alimento per cui richiedere la soglia
 * @param unita_misura	Unità di misura dell'alimento per cui richiedere la soglia
 * @return Valore della soglia inserita
 */
float input_soglia_spesa(char *nome_alimento, char *unita_misura);


//in via di sviluppo per ricette
int leggi_frigo(alimento_frigo* lista_frigo);

#endif

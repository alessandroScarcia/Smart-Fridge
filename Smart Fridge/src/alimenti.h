/**
 * @file alimenti.h
 *
 * @brief Libreria per la gestione degli alimenti.
 *
 * La libreria alimenti.h è stata implementare per permettere la gestione degli alimenti,
 * quindi le operazioni necessarie ai controlli, la manipolazione, il caricamento e il salvataggio
 * di informazioni legate agli alimenti.
 * La gestione degli alimenti in generale avviene tramite tre file: "spesa_effettuata.csv", "alimenti_frigo.dat"
 * e "database_alimenti.dat". Rispettivamente sono utilizzati per: caricare nuovi alimenti per aggiungerli al
 * contenuto del frigo, memorizzare gli alimenti contenuti nel frigo con relativa quantità e data di scadenza,
 * memorizzare informazioni relative agli alimenti conosciuti dal frigocome kcal per campione di alimento, unità
 * di misura per la quantità dell'alimento e soglia al di sotto del quale l'alimento deve essere inserito nella
 * generazione della lista della spesa.
 *
 * @authors Davide Quatela, Alessandro Scarcia, Michela Salvemini
 */

#pragma once

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

#include "consumi.h"

#include "date.h"

#include "utenti.h"



/// DEFINIZIONE DELLE COSTANTI SIMBOLICHE.

/// Lunghezze delle stringhe:
#define LUNG_NOME_ALIMENTO 21 			///	Lunghezza massima per il nome degli alimenti
#define LUNG_CAMPIONE 11 				///	Lunghezza della stringa che ospiterá il campione di riferimento per un alimento. Indica la quantitá che ha un determinato rapporto calorico
#define LUNG_SOGLIA 11 					/// Lunghezza della stringa che ospiterá la soglia minima degli alimenti(la quantitá minima da avere in frigo)
#define LUNG_UNITA_MISURA 6				/// Lunghezza della stringa che ospiterá l'unitá di misura
#define LUNG_RIGA_FGETS 100 			/// Lunghezza massima in caratteri delle righe estratte con fgets() dai file

/// Numero di membri che compongono determinati elementi:
#define NUM_CAMPIONI 3 					///numero dei campioni di riferimento per gli alimenti
#define NUM_CAMPI_ALIMENTO_FRIGO 6 		///numero di colonne che possiede il file contenente i dati degli alimenti comprati

/// Nomi dei file utilizzati:
#define FILE_SPESA "../spesa_effettuata.csv"
#define FILE_FRIGO "../alimenti_frigo.dat"
#define FILE_DATABASE_ALIMENTI "../database_alimenti.dat"

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
#define MIN_ID_ALIMENTO 1				/// Valore minimo per l'id degli alimenti
#define SOGLIA_PERC 20 					///la percentuale di riferimento al di sotto della quale un alimento sta finendo
#define	MIN_QUANTITA 1					/// Limiti per le quantità degli alimenti inseriti in relazione all'unità di misura
#define MAX_QUANTITA_G 10000			/// Limite superiore grammi
#define MAX_QUANTITA_ML 10000			/// limite superiore millilitri
#define MAX_QUANTITA_PZ 100				/// Limite superiore pezzi
#define MAX_ALIM_SUGG 5					/// Limite degli alimenti che possono essere inseriti manualmente dall'utente per la ricerca di una ricetta
#define FLAG_ALIMENTO 0					/// valore che ci permette di capire se il prodotto consumato é un alimento
#define MIN_KCAL_CAMPIONE 0				/// valore minimo per le kcal relative ad un campione di un alimento
#define MAX_KCAL_CAMPIONE 2000			/// valore massimo per le kcal relative ad un campione di un alimento

///**********************************DEFINIZIONI DI STRUCT***********************************

///Questa struttura é utilizzata per i file binari per a memorizzazione dei dati relativi ad un alimento
typedef struct {
	char nome[LUNG_NOME_ALIMENTO];
	int campione_kcal;
	float soglia_spesa;
	char unita_misura[LUNG_UNITA_MISURA];
	unsigned short int kcal;
}alimento_database;

///Questa struttura é utilizzata per il file ad accesso sequenziale degli alimenti comprati
typedef struct{
	char nome[LUNG_NOME_ALIMENTO];
	float quantita;
	char unita_misura[LUNG_UNITA_MISURA];
	data scadenza;
}alimento_frigo;


float soglia_alimento(const char* nome_alimento);


/** funzione che ordina gli alimenti del frigo in base alla scadenza.
 *
 * @param alimenti_frigo
 * @param num_alimenti
 * @return 1
 */
int ordina_alimenti_scadenza(alimento_frigo* alimenti_frigo, int  num_alimenti);




/**funzione che conta gli alimenti presenti nel database
 *
 * @return righe ossia numero di alimenti presenti nel database
 */
int conta_alimenti_database();




/**funione che si occupa di popolare un array di struct inizializzato con gli alimenti e i dati presenti nel database
 *
 * @param lista_alimenti	array di struct che deve essere popolato
 * @return indice_alimento	elementi effettivamente caricati
 */
int leggi_database_alimenti(alimento_database* lista_alimenti);



/**Funzione che serve a popolare un'array di struct( passato per riferimento ) con gli alimenti presenti nel frigo e le relative informazioni
 *
 * @param lista_frigo			array di struct che deve essere popolato
 * @return num_alimenti_letti	elementi effettivamente caricati
 */
int leggi_frigo(alimento_frigo* lista_frigo);




/** funzione che si occupa di eliminare gli alimenti scaduti presenti nel frigo e di inizializzare tali righe
 *
 * @return 1 in caso di successo
 */
int eliminazione_alimenti_scaduti();




/**funzione che si occupa di contare gli alimenti presenti nel frigo e di restituire tale numero
 *
 * @return righe numero di alimenti presenti nel frigo
 */
int conta_alimenti_frigo();




/**funzione che si occupa di visualizzare gli alimenti presenti nel frigo che sono scaduti (si controlla la data di scadenza rispetto quella odierna)
 *
 * @return Il numero di alimenti scaduti visualizzati.
 */
int visualizza_alimenti_scaduti();




/**funzione che si occupa di contare gli alimenti scaduti nel frigo e di restiuire tale numero.
 *
 * @return contatore_alim_scad  numero degli alimenti scaduti
 */
int conta_alimenti_scaduti();




/**Funzione che dopo aver allocato una stringa di una determinata lunghezza, si occupa di ricevere in input il nome della ricetta
 *
 * @return nome_alimento
 */
char* input_nome_alimento();



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


/**
 * La funzione input_id_alimento() permette di richiedere l'inserimento all'utente di un numero n,
 * corrispondente all'n-esimo alimento presente nel frigo, fino ad ottenere un valore valido.
 *
 * @param num_alimenti_frigo Numero di alimenti presenti nel frigo
 * @return	id inserito dall'utente
 */
int input_id_alimento(int num_alimenti_frigo);


/**
 * La funzione ricerca_database() permette di ricercare nel database la presenza di un alimento
 * attraverso il suo nome e di estrarne le caratteristiche se viene individuato.
 *
 * @param nome_alimento 	Nome dell'alimento da ricercare
 * @param alimento_estratto Puntatore alla variabile che deve memorizzare l'alimento trovato
 * @return 1 				se l'alimento viene trovato
 * @return 0 				se l'alimento non viene trovato
 * @return -1				 se il file_database non può essere aperto
 */
int ricerca_alimento_database(char *nome_alimento, alimento_database *alimento_estratto);


/**La funzione aggiorna_database permette al programma(o al frigo se vogliamo avere una visione piú realistica del prodotto)
 * di "imparare" gli alimenti che l'utente consuma. Una volta memorizzati i dati dell'alimento quali kcal e campione di riferimento
 * sará piú semplice ed efficace gestire le abitudini dell'utente riguardo il consumo di kcal e la generazione di ricette con
 * un valore nutrizionale reale. Ovviamente la funzione deve conoscere gli alimenti che abbiamo inserito nel frigo e il relativo numero.
 * Una volta iniziato il confronto tra gli alimenti giá memorizzati e quelli recentemente aggiunti si decide, a seconda se un alimento é
 * conosciuto o meno, di aggiungerlo al database
 * @param nome_alimento 	Nome dell'alimento da inserire
 * @param unita_misura		Unitá di misura dell'alimento che deve essere salvata insieme al nome
 * @return 1 				se l'aggiornamento é terminato con successo
 * */
int aggiorna_database(char* nome_alimento, char* unita_misura);


/**La funzione aggiorna_frigo una volta conosciuti il numero di alimenti da caricare e i dati a riguardo effettua la memorizzazione dei nuovi alimenti comprati
 * all'interno del file alimenti_frigo. La scrittura nel file in questo caso é ottimizzata in quanto si é previsto che, qualora ci siano righe inizializzate
 * ottenute dalla cancellazione di alimenti che non sono piú presenti nel frigo, esse vengano riutilizzate per una nuova memorizzazione. In questa maniera
 * evitiamo algoritmi di compattamento delle righe "piene"
 * @param alimento 	Alimento da salvare nel file del frigo
 * @return 1 				se l'aggiornamento é terminato con successo
 * */
int aggiorna_frigo(alimento_frigo alimento);


/**La funzione visualizza_database apre in lettura il file database_alimenti e ne mostra il contenuto. Questa funzione puó tornare utile non
 * solo all'utente ma anche al programmatore in fase di Debug
 * @param
 * @return 1 				se l'aggiornamento é terminato con successo */
int  visualizza_database_alimenti();


/**La funzione visualizza_frigo oltre ad essere una funzione a se stante che permette di visualizzare il contenuto del frigo(del file alimenti_frigo.csv)
 * serve nella fase di riduzione della quantitá di un alimento. Avendo una lista a portata di mano con tanto di indice per ogni alimento
 * permettiamo all'utente di risparmiare tempo e di avere un quadro semplificato del contenuto del frigo
 * @param
 * @return 1 				se l'aggiornamento é terminato con successo */
int visualizza_frigo();


/***Funzione che si occupa di estrarre la quantitá di un alimento presente nel frigo.
 * @param nome_alimento 	Nome dell'alimento da inserire da cui estrarre la quantitá
 * @return quantita_alimento 	quantitá dell'alimento presente nel frigo
 */
float quantita_alimento(const char* nome_alimento);

/**Funzione che si occupa di ridurre la quantità di un certo alimento presente nel frigo.
 * @param nome_alimento 	Nome dell'alimento da inserire da cui ridurre la quantitá
 * @return 1 	            se la quantitá dell'alimento presente nel frigo é stata correttamente ridotta
 */
int riduci_alimento(const char* nome_alimento, float riduzione);

/**Funzione che si occupa di gestire l'operazione di riduzione di un alimento permettendo all'utente di registrare tale consumo.
 *
 * @return 1 				se l'aggiornamento é terminato con successo */
int gestore_riduzione_alimenti();


/**La procedura lettura file si occupa principalmente di prelevare i dati da un file che chiameremo spesa_effettuata.csv
 * per poi inserirli di volta in volta all'interno di una struct realizzata su misura. Ovviamente i dati verranno inseriti
 * per ogni alimento(riga se vogliamo definirlo piú banalmente) che verrá estratto dal file. */
int carica_spesa();


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


/**
 * La funzione modifica_soglia_spesa() permette all'utente di modificare la quantità di un alimento
 * al di sotto della quale, l'alimento deve essere inserito nella generazione della spesa.
 *
 * @return 1 se la modifica avviene in modo corretto
 * @return 0 se il file database non presenta alimenti
 * @return -1 se il file database non può essere aperto
 */
int modifica_soglia_spesa();


/**
 * La  funzione modifica_kcal() permette all'utente di modificare il valore di kcal per campione
 * di un determinato alimento.
 *
 * @return 1 se la modifica avviene in modo corretto
 * @return 0 se il file database non presenta alimenti
 * @return -1 se ilfile database non può essere aperto
 */
int modifica_kcal();


/**
 * La funzione alimento_casuale() estrae casualmente il nome di un alimento dal database
 * e ne restituisce la stringa corrispondete.
 *
 * @return Nome di un alimento estratto casualmente dal database.
 */
int alimento_casuale(char* nome_alimento);


/**
 * file_operation.h
 *
 *  Created on: 26 apr 2018
 *      Author: david
 */


	///***********************************LIBRERIE INCLUSE***********************************
#ifndef ALIMENTI_LIBRARY
	#include <stdbool.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
#endif

#ifndef ALIMENTI_STRUCT
	enum tipi_campioni { grammi=100, millilitri=100, pz=1}; ///fornisce i vari valori da attribuire come campione agli alimenti


	///****************************DEFINIZIONI DI COSTANTI***********************************

	///LUNGHEZZE DI STRINGHE
	#define LUNG_NOME 20 ///lunghezza del nome degli alimenti
	#define LUNG_STR_LAVORO 10 ///lunghezza della stringa che verrá utilizzata in varie funzioni per effettuare split o altre operazioni su stringhe
	#define LUNG_CAMPIONE 10 ///lunghezza della stringa che ospiterá il campione di riferimento per un alimento. Indica la quantitá che ha un determinato rapporto calorico
	#define LUNG_SOGLIA 10 ///lunghezza della stringa che ospiterá la soglia minima degli alimenti(la quantitá minima da avere in frigo)
	#define LUNG_NOME_FILE 100 ///lunghezza da riservare al nome del file
	#define LUNG_U_MISURA 5	///lunghezza della stringa che ospiterá l'unitá di misura
	#define LUNG_TUPLA 100///lunghezza della tupla ossia una riga quanto deve essere grande
	#define LUNG_BUFFER 100

	///NUMERO DI ELEMENTI DI ARRAY/MATRICI ECC
	#define NUM_CAMPIONI 3 ///numero dei campioni di riferimento per gli alimenti
	#define NUM_CAMPI_SPESA 3 ///numero di colonne che possiede il file contenente i dati degli alimenti comprati


	///ALTRE COSTANTI
	#define OPZIONE_MINIMA 0 ///ALE: utilizza questa per controllare la scelta minima del menu
	#define OPZIONE_MASSIMA 2 ///ALE: utilizza questa per controllare la scelta massima del menu
	#define SOGLIA_PERC 20 ///la percentuale di riferimento al di sotto della quale un alimento sta finendo


	///**********************************DEFINIZIONI DI STRUCT***********************************

	///Questa struttura é utilizzata per i file binari per a memorizzazione dei dati relativi ad un alimento
	typedef struct {
		short unsigned int id;
		char nome_alimento[LUNG_NOME];
		int quant_campione;
		float soglia_spesa;
		char unita_misura[LUNG_U_MISURA];
		unsigned short int kcal;
	}alimenti;

	///struttura relativa al formato della data
	typedef struct{
		unsigned short giorno;
		unsigned short mese;
		unsigned short anno;
	}data;

	///Questa struttura é utilizzata per il file ad accesso sequenziale degli alimenti comprati
	typedef struct{
		char nome_alimento[LUNG_NOME];
		float quantita;
		char unita_misura[LUNG_U_MISURA];
		data scadenza;
	}alim_spesa;

#endif


	/**La funzione aggiorna_database permette al programma(o al frigo se vogliamo avere una visione piú realistica del prodotto)
	 * di "imparare" gli alimenti che l'utente consuma. Una volta memorizzati i dati dell'alimento quali kcal e campione di riferimento
	 * sará piú semplice ed efficace gestire le abitudini dell'utente riguardo il consumo di kcal e la generazione di ricette con
	 * un valore nutrizionale reale. Ovviamente la funzione deve conoscere gli alimenti che abbiamo inserito nel frigo e il relativo numero.
	 * Una volta iniziato il confronto tra gli alimenti giá memorizzati e quelli recentemente aggiunti si decide, a seconda se un alimento é
	 * conosciuto o meno, di aggiungerlo al database*/
	void aggiorna_database(alim_spesa* catalogo_alimenti, int num_alim_spesa);



	///Per vedere se un file esiste basta aprire uno stream in lettura e verificare se effettivamente lo stream é attivo
	bool controllo_esist_file(const char * nome_file);



	/**La funzione leggi_campo avendo in ingresso la linea presa in considerazione in quel momento e il numero del campo in cui é presente il valore
	 *da estrarre permette di  restituire un valore che si desidera estrarre sotto forma di stringa. Piú precisamente sulla singola riga vengono
	 *applicati dei "tagli" tramite la funzione strtok che prende come riferimento il carattere delimitatore ";" presente nel file. Quindi
	 *una volta che  il contatore "num" arriva a 0 vuol dire che il token(puntatore tok) é all'inizio della stringa che ci interessa prelevare.
	 *Se é presente un altro campo dopo la stringa che abbiamo preso in considerazione avviene un troncamento a partire da dopo il valore interessato*/
	const char* leggi_campo(char* line, int num);



	/**La funzione aggiorna_frigo una volta conosciuti il numero di alimenti da caricare e i dati a riguardo effettua la memorizzazione dei nuovi alimenti comprati
	 * all'interno del file alimenti_frigo. La scrittura nel file in questo caso é ottimizzata in quanto si é previsto che, qualora ci siano righe inizializzate
	 * ottenute dalla cancellazione di alimenti che non sono piú presenti nel frigo, esse vengano riutilizzate per una nuova memorizzazione. In questa maniera
	 * evitiamo algoritmi di compattamento delle righe "piene"*/
	void aggiorna_frigo(alim_spesa* catalogo_alimenti, int num_alimenti);



	/**Funzione che conta il numero di righe in un file sequenziale avendo come unico paramentro il
	 * nome del file dove occorre contare il  numero di righe*/
	int conta_righe_file_sq(char nome_file[LUNG_NOME_FILE]);




	/**La funzione visualizza_database apre in lettura il file database_alimenti e ne mostra il contenuto. Questa funzione puó tornare utile non
	 * solo all'utente ma anche al programmatore in fase di Debug*/
	int  visualizza_database_alimenti();




	/**La funzione visualizza_frigo oltre ad essere una funzione a se stante che permette di visualizzare il contenuto del frigo(del file alimenti_frigo.csv)
	 * serve nella fase di riduzione della quantitá di un alimento. Avendo una lista a portata di mano con tanto di indice per ogni alimento
	 * permettiamo all'utente di risparmiare tempo e di avere un quadro semplificato del contenuto del frigo*/
	int visualizza_frigo();


	///funzione che restituisce l'unitá di misura della quantitá dell'alimento inserito nel frigo
	char* ottieni_u_misura(char quant_u_misura[LUNG_STR_LAVORO]);




	///funzione che restituisce l'effettiva quantitá dell'alimento inserito nel frigo
	float ottieni_quantita(char quant_u_misura[LUNG_STR_LAVORO]);




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
	int lettura_spesa();




	/**Funzione che permette di inserire manualmente il numero di kcal per ogni alimento
	 * "nuovo"che deve essere memorizzato nel database */
	int input_kcal(char nome_alimento[LUNG_NOME]);




	/**La funzione set_q_camp viene sfruttata per memorizzare all'interno del database la quantita del campione di un alimento
	 * questo dato é molto importante una volta che occorre calcolare il numero di kcal che una determinata quantitá di alimento produce.
	 * Ovviamente il campione deve essere differente a seconda dell'unitá di misura di ogni alimento */
	int set_q_camp(char unita_misura[LUNG_U_MISURA]);





	/**La funzione se_soglia_spesa permette di calcolarein maniera idonea la soglia al di sotto della quale
	 * occorre notificare l'utente dell'imminente fine di un alimento. Principalmente la funzione opera sulla quantita
	 * su cui viene effettuato un calcolo in percentuale cosí da ottenere la soglia. Una volta ottenuta si converte il
	 * numero in una stringa e si accoda l'unitá di misura corrispettiva*/
	float set_soglia_spesa(float quantita, char unita_misura[LUNG_U_MISURA]);





	//in via di sviluppo per ricette
	int leggi_frigo(alim_spesa* lista_frigo);

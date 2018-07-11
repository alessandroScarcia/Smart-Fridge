/**
 * @file lista_spesa.h
 *
 * @brief Libreria per la gestione della lista della spesa.
 *
 * La libreria lista_spesa.h è stata implementata per permettere all'utente
 * di generare dei promemoria sulla lista degli alimenti da comprare.
 * La libreria permette sia la gestione della lista personale di un utente
 * che di quella globale
 *
 * @authors Alessandro Scarcia, Davide Quatela, Michela Salvemini
 */


#ifndef LISTA_SPESA_LIB
#define LISTA_SPESA_LIB

#include "menu_settimanale.h" ///libreria che permette la gestione(modifica) del menu settimanale


#include "ricette.h"          ///libreria che permette la gestione(manipolazione,caricamento ecc) delle ricette


#include "alimenti.h"		 ///libreria che permette la gestione(manipolazione,caricamento ecc) degli alimenti


#define LISTA_SPESA_GLOBALE "../lista_spesa.txt"   //directory in cui trovare il file della spesa globale

//NOTA: il file della lista della spesa personale sará sempre nel seguente formato :   ../spesa_nickname.txt
#define PREFIX_FILE_SPESA "../spesa_"		   //prima parte nel nome file della lista personale
#define SUFFIX_FILE_SPESA ".txt"					   //formato del file della lista personale
#define LUNG_NOME_FILE_SPESA 30

/**Funzione che provvede a scrivere su file(il nome é specificato dal parametro in ingresso) il nome dell'alimento che passiamo alla funzione.
 *
 * @param nome_alimento
 * @param nome_file
 * @return 0
 */
int aggiorna_lista_spesa(char nome_alimento[LUNG_NOME_ALIMENTO], char nome_file[LUNG_NOME_FILE_SPESA]);





/**Funzione che si occupa di eliminare il nome del file specificato e di restituire in caso di successo il valore -1
 *
 * @param nome_alimento
 * @return 1			in caso sia stato eliminato correttamente il file
 * @return 0			in caso sia terminata la funzione saltando tutti i controlli
 * @return -1			in caso di errore nella apertura o eliminazione del file
 */
int elimina_file_spesa(char nome_alimento[LUNG_NOME_ALIMENTO]);




/**Funzione che si occupa di confrontare la soglia in ingresso con la quantità di quell'alimento.
 *
 * @param soglia_spesa
 * @param nome_alimento
 * @return 1			in caso la quantitá disponibile sia inferiore della soglia
 * @return 0			in caso la quantitá disponibile sia superiore della soglia
 * @return -1			in caso di errore
 */
int controllo_soglia(float soglia_spesa,char nome_alimento[LUNG_NOME_ALIMENTO]);


/** Funzione che si occupa di leggere dal database gli alimenti che possono essere inseriti nella lista della spesa.
 *
 * @return 1			in caso sia stato aggiornato correttamente il file della spesa
 * @return 0			in caso sia terminata la funzione saltando tutti i controlli
 * @return -1			in caso di errore nella apertura del file database
 */
int generatore_spesa_globale();

/**Funzione che si occupa di creare il file che conterrá gli alimenti che un utente specifico deve comprare.
 *
 * @return 1 in caso a funzione sia terminata con successo
 */
int generatore_spesa_personale();


/** Funzione che provvede a visualizzare gli alimenti che devono essere comprati dal file della lista personale o di quella globale.
 *
 * @param nome_file
 * @return 1
 */
int visualizza_lista_spesa(char nome_file[LUNG_NOME_FILE_SPESA]);


/** Funzione che provvede a visualizzare gli alimenti che devono essere comprati dal file della lista personale.
 *
 * @return 1
 * @return -1
 */
int visualizzazione_personale();
#endif

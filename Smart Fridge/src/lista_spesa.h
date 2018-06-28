/*
 * lista_spesa.h
 *
 *  Created on: 18 giu 2018
 *      Author: david
 */

#ifndef LISTA_SPESA_H_
#define LISTA_SPESA_H_


#include "menu_settimanale.h"

#include "ricette.h"


#define LISTA_SPESA_GLOBALE "../lista_spesa.txt"   //directory in cui trovare il file della spesa globale

//NOTA: il file della lista della spesa personale sará sempre nel seguente formato :   ../spesa_nickname.txt
#define PRIMA_PARTE_NOMEFILE "../spesa_"		   //prima parte nel nome file della lista personale
#define FORM_FILE_SPESA ".txt"					   //formato del file della lista personale


/**Funzione che provvede a scrivere su file(il nome é specificato dal parametro in ingresso) il nome dell'alimento che passiamo alla funzione.
 *
 * @param nome_alimento
 * @param nome_file
 * @return 0
 */
int aggiorna_lista_spesa(char nome_alimento[LUNG_NOME_ALIMENTO],char nome_file[LUNG_NOME_FILE]);


/**Funzione che dopo aver allocato una stringa di una determinata lunghezza, si occupa di ricevere in input il nome della ricetta
 *
 * @return nome_alimento
 */
char* input_nome_alimento();


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
int lettura_alimenti_acquistabili();

/**Funzione che si occupa di creare il file che conterrá gli alimenti che un utente specifico deve comprare.
 *
 * @return 1 in caso a funzione sia terminata con successo
 */
int gestore_spesa_personale();


/** Funzione che provvede a visualizzare gli alimenti che devono essere comprati dal file della lista personale o di quella globale.
 *
 * @param nome_file
 * @return 1
 */
int visualizza_lista_spesa(char nome_file[LUNG_NOME_FILE]);


/**Funzione che si occupa di estrarre gli ingredienti di una determinata ricetta.
 *
 * @param nome_ricetta
 * @param ingredienti
 * @return indice_ingrediente  numero ingredienti estratti dalla ricetta
 */
int estrazione_ingredienti(char nome_ricetta[DIM_CIBO], ingrediente* ingredienti);

#endif /* LISTA_SPESA_H_ */

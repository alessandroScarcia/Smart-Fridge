/*
 * file_ricette_operation.h
 *
 *  Created on: 16 mag 2018
 *      Author: david
 */
#include "alimenti.h"
#ifndef RICETTE_STRUCT
#define RICETTE_STRUCT
	//**********************************LIBRERIE INCLUSE***********************************
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alimenti.h"
#include "pulizia_flussi.h"

	//***************************DEFINIZIONI DI COSTANTI***********************************

	//LUNGHEZZE DI STRINGHE
#define LUNG_NOME_RIC 50
#define LUNG_INGR 20
#define LUNG_TEMP_PREP 20
#define LUNG_PREPARAZIONE 500
#define LUNG_COMPLESSITA 20
#define LUNG_TUPLA_RICETTE 1000//lunghezza della tupla ossia una riga quanto deve essere grande
#define LUNG_STR_LAVORO_RIC 100 //lunghezza della stringa che verrá utilizzata in varie funzioni per effettuare split o altre operazioni su stringhe
#define LUNG_NOME_FILE 100 //lunghezza da riservare al nome del file
#define LUNG_PRODOTTO 50

	//NUMERO DI ELEMENTI DI ARRAY/MATRICI ECC
#define MAX_NUM_INGR 20
#define MAX_NUM_FASI 20
#define MAX_NUM_U_MISURA 20
#define NUM_CAMPI_RICETTA 6
#define NUM_DELIM 2
#define NUM_ALIM_SUGG_SCAD 2
#define NUM_CAMPI_INGREDIENTI 3

//COSTANTI DI RICERCA
#define FLAG_RICETTA 1 //flag che ci aiuta a capire che bisogna memorizzare/estrarre un prodotto di tipo ricetta dal file dei consumi
#define MOD_RICERCA_AUTOMATICA 1 //se passato 1 la ricerca delle ricette viene fatta in base agli alimenti presenti nel frigo e alla loro relativa quantitá
#define MOD_RICERCA_MANUALE 0	//se passato 0 la ricerca delle ricette viene fatta in base ai nomi degli alimenti inseriti dall'utente



// DIVISIONE CAMPI (leggi_campo_ricetta)
#define CAMPO_MULTIPLO 1	//ci aiuta a capire che lo split della stringa deve essere fatto contando il numero di caratteri delimitatori(vedi documentazione di leggi_campo_ricetta)
#define DELIMITATORE_PREPARAZIONE '-' //carattere per la separazione della preparazione
#define SEQUENZA_DELIMITATORI "-"	//sequenza di caratteri su cui deve essere effettuato lo split della stringa quando non conosciamo il numero di campi



// Nomi dei file utilizzati:
#define FILE_DATABASE_RICETTE "../database_ricette.csv"
#define FILE_NUOVE_RICETTE "../nuove_ricette.csv"
#define FILE_CONSUMI "../consumi.csv"



//COSTANTI PER LA VISUALIZZAZIONE
#define MOSTRA_DATABASE "" //per mostrare l'intero database basta passare una stringa vuota alla funzione visualziza database
#define VISTA_TOTALE 0  //ci permette di vedere tutte le informazioni riguardante/i la/e ricetta/e
#define VISTA_MINIMIZZATA 1 //ci permette di vedere le informazioni di base riguardante/i la/e ricetta/e

	//*********************************DEFINIZIONI DI STRUCT***********************************

typedef struct {
	char nome_ingredienti[LUNG_INGR];
	float quantita_necessarie;
	char unita_misura[LUNG_UNITA_MISURA];
}ingrediente;


	//Questa struttura é utilizzata per i file binari per a memorizzazione dei dati relativi ad un alimento
typedef struct {
	int id_ricetta;
	char nome_ricetta[LUNG_NOME_RIC];
	ingrediente ingredienti[MAX_NUM_INGR];
	char tempo_prep[LUNG_TEMP_PREP];
	char preparazione[LUNG_PREPARAZIONE];
	char complessita[LUNG_COMPLESSITA];
	int kcal_ricetta;
	int porzione;
}ricetta;



typedef struct{
	char nome_prodotto[LUNG_PRODOTTO];
	short flag_prodotto;
	int frequenza;
}consumi;



#endif



//*********************************DEFINIZIONI DEI PROTOTIPI***********************************

///serve ad effettuare una tokenizzazione della stringa che gli passiamo
const char* leggi_campo_ricetta(char* line, int num, short flag_campo);

///serve a leggere da file .csv le ricette nuove
int lettura_nuove_ricette();

///permette la visualizzazione del database o di una singola ricetta in base all'id che passiamo. Inoltre possiamo decidere in che modalitá vedere i dati(0-->totale, 1-->minimizzata)
int visualizza_database_ricette(char nome_ricetta[LUNG_NOME_RIC], int id_ricetta_pers, int vista);

///serve a memorizzare la ricetta che passiamo all'interno di un file con accesso binario che fungerá da database
int aggiorna_database_ricette(ricetta nuove_ricette);

///si occupa di eliminare una ricetta in base all'id inserito dall'utente. La riga che conteneva la ricetta viene inizializzata
int elimina_ricetta();

/// La funzione suggerirá le ricette che si possono effettuare con gli ingredienti a disposizione nel frigo
int suggerimento_ricetta_automatico(alimento_frigo* alimenti_frigo,int num_alimenti);

///Suggerisce le ricette che si possono fare con gli alimenti inseriti manualmente dall'utente
int suggerimento_ricetta_manuale(int num_alimenti, char nome_alimenti[MAX_ALIM_SUGG][LUNG_NOME_ALIMENTO]);

///Ordina le ricette presenti nel database in base alle kcal che producono. L'ordinamento viene effettuato tramite shell sort
int ordina_ric_kcal(ricetta* ricette_database, int num_ricette);

///conta le righe(le ricette) presenti nel database delle ricette escludendo le righe inizializzate(vuote)
int conta_righe_database_ricette();

///popola un array di struct con le ricette presenti nel database. Questa funzione serve a non effettuare ogni volta la lettura del file all'interno di altre funzioni
int lettura_database_ricette(ricetta* ricette_database);

///DA SPOSTARE funzione che si occupa di memorizzare il prodotto consumato(0-->alimento,  1---->ricetta) all'interno di file che memorizza i vari consumi degli utenti
int registra_consumi(char nome_prodotto[LUNG_PRODOTTO], short flag_prodotto);

///funzione che ricerca il prodotto maggiormente consumato in base al valore del flag che viene passato(0-->alimento, 1--->ricetta) e lo stampa
int ricerca_prod_magg_cons(short flag_prodotto);

///funzione che calcola le calorie prodotte per una ricetta in base al numero e nome degli ingredienti che possiede
int conta_kcal_ricetta(ingrediente ingredienti[MAX_NUM_INGR], int num_ingredienti);

///funzione di ricerca ricette, ottimizzata per gli alimenti che stanno per scadere all'interno del frigo
int ricette_alimenti_in_scadenza(alimento_frigo* alimenti_frigo,int num_alimenti,char nome_alimenti[MAX_ALIM_SUGG][LUNG_NOME_ALIMENTO]);

///funzione che serve ad inserire manualmente gli ingredienti da usare come paroli chiave per la ricerca di ricette che possiedono tali ingredienti
int inserimento_manuale_ingredienti(char nome_alimenti[MAX_ALIM_SUGG][LUNG_NOME_ALIMENTO]);

///funzione che si occupa di modificare un campo di una determinata ricetta inseriti dall'utente, con un nuovo valore anch'esso inserito manualmente
int modifica_ricetta();

///funzione che si occupa dell'inserimento di un nuovo valore per il campo porzione
int input_porzione_ricetta();

///funzione che si occupa dell'inserimento di un nuovo ingrediente per una data ricetta
char* input_ingredienti_ricetta();

///funzione che si occupa dell'inserimento di un nuovo valore per il campo preparazione
char* input_preparazione_ricetta();

///funzione che si occupa dell'inserimento di un nuovo valore per il campo complessitá
char* input_complessita_ricetta();

///funzione che si occupa dell'inserimento di un nuovo valore per il campo tempo di preparazione
char* input_tempo_prep_ricetta();

///funzione che si occupa dell'inserimento di un nuovo valore per il campo nome ricetta
char* input_nome_ricetta();

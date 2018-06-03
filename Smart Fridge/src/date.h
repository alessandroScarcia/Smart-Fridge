/**
 * @file date.h
 *
 * Libreria per la gestione delle date.
 *
 * La libreria date.h è stata implementata per facilitare la manipolazione
 * delle date all'interno del caso di studio. A questo scopo è definito un nuovo
 * tipo di dato, "data", e funzioni per manipolarlo e generare informazioni da
 * dati di questo tipo.
 *
 * @authors Alessandro Scarcia
 */

//Inclusione delle librerie standard
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

#ifndef DATE_LIB
#define DATE_LIB

//Definizione delle costanti simboliche
#define NUM_CAMPI_DATA 3				// numero di campi che compongono la data
#define MIN_GIORNO 1					// valore minimo per il giorno di una data
#define MAX_GIORNO_30 30				// valore massimo per il giorno di una data (Aprile, Giugno, Settembre, Novembre)
#define MAX_GIORNO_31 31				// valore massimo per il giorno di una data (mesi diversi da Febbraio e quelli elencati prima)
#define MAX_FEBBRAIO 28					// valore massimo per il giorno di una data con mese Febbraio
#define MAX_FEBBRAIO_BIS 29				// valore massimo per il giorno di una data con mese Febbraio e anno bisestile
#define MIN_MESE 1						// valore minimo per il mese di una data
#define MAX_MESE 12						// valore massimo per il mese di una data
#define MIN_ANNO 1900					// valore minimo per l'anno di una data
#define MAX_ANNO 2100					// valore massimo per l'anno di una data
#define CONTROLLO_BISESTILE_1 4			// primo controllo di divisibilità per verificare se l'anno è bisestile
#define CONTROLLO_BISESTILE_2 100		// secondo controllo di divisibilità per verificare se l'anno è bisestile
#define CONTROLLO_BISESTILE_3 400		// terzo controllo di divisibilità per verificare se l'anno è bisestile
#define DATE_NON_VALIDE -1				// valore di ritorno della funzione confrontaDate, se le due date non sono valide
#define DATE_UGUALI 0					// valore di ritorno della funzione confrontaDate, se le due date sono uguali
#define PRIMA_DATA_ANTECEDENTE 1		// valore di ritorno della funzione confrontaDate, se la prima data è precedente alla seconda
#define SECONDA_DATA_ANTECEDENTE 2		// valore di ritorno della funzione confrontaDate, se la seconda data è precedente alla prima
#define BASE_MESE_CORRENTE 1			// valore da sommare a quello estratto dalla struttura della data odierna (funzione localtime,libreria time.h) per ottenere il mese corrente
#define BASE_ANNO_CORRENTE 1900			// valore da sommare a quello estraddo dalla struttura della data odierna (funzione localtime,libreria time.h) per ottenere l'anno corrente


//Definizione dei tipi di di dato

/**
 * @typedef data
 *
 * Il tipo di dato "data" è basato su di una struct i cui membri sono: giorno, mese e anno.
 * Tutti i membri sono di tipo unsigned short.
 */
typedef struct{
	unsigned short giorno;
	unsigned short mese;
	unsigned short anno;
}data;



/**
 * La funzione controlla_data() riceve in ingresso una variabile
 * di tipo data e stabilisce se questa rappresenta un informazione
 * significativa.
 *
 * @param d Variabile da analizzare
 * @return 0 se la variabile non è significativa
 * @return 1 se la variabile è significativa
 */
int controlla_data(data d);


/**
 * La funzione confronta_date() riceve in ingresso due variabili
 * di tipo data e, se esse sono significative, stabilisce se sono uguali
 * oppure quale delle due antecede l'altra.
 *
 * @param d1 Prima variabile del confronto
 * @param d2 Seconda Variabile del confronto
 * @return -1 se le date non sono valide
 * @return 0 se le date sono uguali
 * @return 1 se la prima data antecede la seconda
 * @return 2 se la seconda data antecede la prima
 */
int confronta_date(data d1, data d2);


/**
 * La funzione genera_data() riceve il puntatore ad una variabile
 * di tipo data, e ne assegna un valore generato nell'intervallo
 * rappresentato dalle altre due date ricevute in ingresso.
 *
 * @param dataGenerata Puntatore alla variabile da generare
 * @param d1 Primo estremo dell'intervallo in cui generare la data
 * @param d2 Secondo estremo dell'intervallo in cui generare la data
 * @return 0 se l'intervallo non è valido
 * @return 1 se la generazione è avvenuta con successo
 */
int genera_data(data* data_generata, data d1, data d2);


/**
 * La funzione data_odierna() è utilizzata per determinare
 * la data almomento dell'esecuzione.
 *
 * @return Un avariabile di tipo data il cui valore è la data al momento di esecuzione
 */
data data_odierna();


/**
 * La funzione giorno_giuliano() calcola il giorno giuliano di una data in formato
 * gregoriano.
 *
 * @param d1 Variabile di tipodata da convertire
 * @return Il valore di d1 in giornoGiuliano
 */
double giorno_giuliano(data d1);


/**
 * La funzione data_gregoriana() datermina la data in formato gregoriana
 * del giorno giuliano ricevuto in ingresso e la ritorna come dato di tipo data.
 *
 * @param dataGiuliana Giorno giuliano da convertire
 * @return Data in formato gregoriano corrispondente al giorno giuliano ricevuto
 */
data data_gregoriana(double data_giuliana);


/**
 * La funzione diff_date() determina il valore assoluto della differenza in giorni
 * fra due date ricevute in ingresso.
 *
 * @param d1 Prima data della differenza
 * @param d2 Seconda data della differenza
 * @return Valore assoluto della differenza in giorni fra le due date
 */
int diff_date(data d1, data d2);


/**
 * La funzione shift_data_odierna() genera una data sommando il valore indicato
 * da valoreShift alla data odierna al momento dell'esecuzione.
 *
 * @param valoreShift Giorni da sommare alla data di esecuzione
 * @return La data generata in formato gregoriano
 */
data shift_data_odierna(short valore_shift);


/**
 * La funzione input_data() permette di richiedere all'utente l'inserimento di una data
 * verificando la validità dell'input e ripetendo l'inserimento se necessario.
 *
 * @return La data inserita dall'utente
 */
data input_data();

#endif

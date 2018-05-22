/*
 * Libreria date.h per la gestione delle date
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

//Definizione delle costanti simboliche
#define MIN_GIORNI 1					/// valore minimo per il giorno di una data
#define MAX_GIORNI_1 30					/// valore massimo per il giorno di una data (Aprile, Giugno, Settembre, Novembre)
#define MAX_GIORNI_2 31					// valore massimo per il giorno di una data (mesi diversi da Febbraio e quelli elencati prima)
#define MAX_FEBBRAIO 28					// valore massimo per il giorno di una data con mese Febbraio
#define MAX_FEBBRAIO_BIS 29				// valore massimo per il giorno di una data con mese Febbraio e anno bisestile
#define MIN_MESI 1						// valore minimo per il mese di una data
#define MAX_MESI 12						// valore massimo per il mese di una data
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

//tipo di dato DATA
typedef struct{
	char giorno;
	char mese;
	unsigned short anno;
}data;



// funzione per la verifica della validità di una data
int controllaData(data d);


//  funzione per il confronto fra due date
int confrontaDate(data d1, data d2);


// funzione per generare una data dato un intervallo
int generaData(data* dataGenerata, data d1, data d2);


// procedura per ricavare la data dal sistema
data dataOdierna();


// funzione per il calcolo del giorno giuliano di una data
double giornoGiuliano(data d1);


// funzione per il calcolo della differenza fra due date
int diffDate(data d1, data d2);


// funzione per l'estrazione di una data da un giorno giuliano
data dataGregoriana(double dataGiuliana);


// funzione per la generazione di una data partendo dalla data odierna
// e dal valore di giorni per cui shiftare da quest'ultima
data shiftDataOdierna(short valoreShift);

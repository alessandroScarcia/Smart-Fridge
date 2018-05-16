// Implementazione delle fuznioni della libreria date.h
#ifndef DATE
#define date
#include "date.h"
#endif

/*
 * Funzione controllaData:
 * utilizzata per determinare se una data � significativa.
 * Parametri in ingresso:
 *  - data da analizzare.
 * Valori in uscita:
 *  - "0" se la data non � valida;
 *  - "1" se la data � valida.
 * Analizza la data in ingresso, partendo dall'anno. Su quest'ultimo esegue controlli
 * di divisibilit� per verificare se � bisestile. Memorizza l'esito di questo primo controllo
 * all'interno della variabile flagBisestile. Successivamente analizza il valore del giorno
 * relativamente al valore del mese: controlla che il giorno rientri nell'intervallo di giorni possibili
 * per ildeterminato mese. In particolare se il mese � febbraio, viene utilizzato anche il valore di
 * flagBisestile per determinare il corretto intervallo di giorni da considerare.
 */
int controllaData(data d){
	char flagBisestile = 0;

	// verifica per anno bisestile
	if((d.anno % CONTROLLO_BISESTILE_1 == 0 && d.anno % CONTROLLO_BISESTILE_2 != 0) || d.anno % CONTROLLO_BISESTILE_3 == 0)
		flagBisestile++;

	// verifica sul valore del mese
	if(d.mese < MIN_MESI || d.mese > MAX_MESI)
		return 0;

	// verifica che il valore del giorno non sia inferiore a MIN_GIORNI
	if(d.giorno < MIN_GIORNI)
		return 0;

	// verifica che il valore del giorno non sia maggiore del limite massimo relativo al mese della data
	switch(d.mese){
	// controllo per i mesi Aprile/Giugno/Settembre/Novembre
	case 4:
	case 6:
	case 9:
	case 11:
		if(d.giorno > MAX_GIORNI_1)
			return 0;
		break;
	case 2:
		if(d.giorno > ((flagBisestile)? MAX_FEBBRAIO_BIS : MAX_FEBBRAIO))
			return 0;
		break;
	default:
		if(d.giorno > MAX_GIORNI_2)
			return 0;
		break;
	}

	// se non vengono individuati errori nella data,viene restituito il valore "1"
	return 1;
}


/*
 * Funzione confrontaDate:
 * utilizzata per determinare se due date sono uguali o se una � antecede l'altra.
 * Parametri in ingresso:
 *  - le due date da confrontare.
 * Valori in uscita:
 *  - DATE_NON_VALIDE se le una delle due date non � significativa;
 *  - DATE_UGUALI se le due date hanno lo stesso valore;
 *  - PRIMA_DATA_ANTECEDENTE se la prima data precede la seconda;
 *  - SECONDA_DATA_ANTECEDENTE se la seconda data precede la prima.
 *  Analizza le due date verificando per prima cosa che siano entrambe significative, utilizzando
 *  la funzione controllaData. Se le due date hanno dei valori corretti, le confronta attraverso una serie
 *  costrutti di selezione.
 */
int confrontaDate(data d1, data d2){
	// verifica della validit� delle date
	if (!controllaData(d1) || !controllaData(d2))
		return DATE_NON_VALIDE;

	// verifica per l'uguaglianza fra le date
	if(d1.anno == d2.anno && d1.mese == d2.mese && d1.giorno == d2.giorno)
		return DATE_UGUALI;

	// determinazione della data precedente all'altra
	if(d1.anno <= d2.anno && d1.mese <= d2.mese && d1.giorno <= d2.giorno)
		return PRIMA_DATA_ANTECEDENTE;
	else
		return SECONDA_DATA_ANTECEDENTE;
}

/*
 * Funzione generaData:
 * utilizzata per generare una data dato un intervallo.
 * Parametri in ingresso:
 * 	- puntatore alla variabile in cui memorizzare il valore della data generata
 *  - due date che determinano l'intervallo da utilizzare per la generazione della data.
 * Valori in uscita:
 *  - "0" se le date che costituiscono l'intervallo non sono significative o sono uguali
 *  - "1" se la generazione della data va a buon fine
 *  Analizza le due date, verificando per prima cosa se siano valide e non uguali. Nel caso in cui
 *  si verificasse uno di questi due eventi, la funzione restituisce 0 che equivale ad errore. Altrimenti
 *  verifica se le date sono ordinate correttamente, cio� che d1 sia antecedente a d2. Se ci� non
 *  � vero la funzione ritorna il valore restituito dalla chiamata della funzione stessa ma
 *  con valori di d1 e d2 invertiti (riga 115). Se i parametri sono corretti, vengono generati
 *  i valori dellanuova data e memorizzati all'interno della variabile puntata dal puntatore passato in
 *  ingresso. Vengono perci� gestiti i casi limite della generazione, analizzano i dati di volta in volta
 *  generati e determinando corretti estremi per la generazione. I casi limite si verificano
 *  nel momento in cui la generazione di anno e mese, crea un valore uguale a quelli presenti
 *  in uno dei due estremi.
 *  Al termine della generazione, la funzione ritorna un valore pari a 1.
 */
int generaData(data* dataGenerata, data d1, data d2){
	int rnd; // numero casuale per la generazione dei valori della data
	char flagBisestile = 0;
	unsigned short min, max; // variabili per contenere gli estremi per la generazione casuale dei valori della data

	// verifica della validit� delle date che compongono l'intervallo
	int risConfronto = confrontaDate(d1, d2);
	if(risConfronto == DATE_NON_VALIDE || risConfronto == DATE_UGUALI)
		return 0;
	// ordinamento delle date, nel caso in cui d2 sia precedente a d1
	else if(risConfronto == SECONDA_DATA_ANTECEDENTE)
		return generaData(dataGenerata, d2, d1);

	// generazione di rnd
	rnd = rand();

	// generazione dell'anno
	(*dataGenerata).anno = rnd % (d2.anno - d1.anno + 1) + d1.anno;

	// verifica per anno bisestile
	if(((*dataGenerata).anno % CONTROLLO_BISESTILE_1 == 0 && (*dataGenerata).anno % CONTROLLO_BISESTILE_2 != 0) || (*dataGenerata).anno % CONTROLLO_BISESTILE_3 == 0)
		flagBisestile++;

	// generazione del mese
	if((*dataGenerata).anno == d1.anno){
		min = (*dataGenerata).mese;
		max = MAX_MESI;
	}else if((*dataGenerata).anno == d2.anno){
		min = MIN_MESI;
		max = d2.mese;
	}else{
		min = MIN_MESI;
		max = MAX_MESI;
	}
	(*dataGenerata).mese = rnd % (max - min +1) + min;

	// generazione del giorno
	switch((*dataGenerata).mese){
	case 4:
	case 6:
	case 9:
	case 11:
		if((*dataGenerata).mese == d1.mese){
			min = d1.giorno;
			max = MAX_GIORNI_1;
		}else if((*dataGenerata).mese == d2.mese){
			min = MIN_GIORNI;
			max = d2.giorno;
		}else{
			min = MIN_GIORNI;
			max = MAX_GIORNI_1;
		}
		(*dataGenerata).giorno = rnd % (max - min +1) + min;
		break;
	case 2:
		if((*dataGenerata).mese == d1.mese){
			min = d1.giorno;
			max = (flagBisestile)? MAX_FEBBRAIO_BIS : MAX_FEBBRAIO;
		}else if((*dataGenerata).mese == d2.mese){
			min = MIN_GIORNI;
			max = d2.giorno;
		}else{
			min = MIN_GIORNI;
			max = (flagBisestile)? MAX_FEBBRAIO_BIS : MAX_FEBBRAIO;;
		}
		(*dataGenerata).giorno = rnd % (max - min +1) + min;
		break;
	default:
		if((*dataGenerata).mese == d1.mese){
			min = d1.giorno;
			max = MAX_GIORNI_2;
		}else if((*dataGenerata).mese == d2.mese){
			min = MIN_GIORNI;
			max = d2.giorno;
		}else{
			min = MIN_GIORNI;
			max = MAX_GIORNI_2;
		}
		(*dataGenerata).giorno = rnd % (max - min +1) + min;
		break;
	}
	return 1;
}

/*
 * Procedura dataOdierna:
 * Parametri in ingresso:
 *  - puntatore alla variabile che conterra la data odierna
 * Assegna alla variabilepuntata dal puntatore ricevuto i valori della
 * data odierna ricavandola grazie alle sunzione time e localtime. La funzione time
 * restituisce il tempo del calendario di sistema nel formato aritmentico time_t,
 * ed � convertito nella struttura struct tm, presente nella libreria time.h, cos�
 * da poter ricavare i valori della data dai membri della struttura identificati come
 * tm_mday, tm_mon e tm_year.
 */
void dataOdierna(data* dataCorrente){
	//estrazione della data odierna
	time_t tmp = time(NULL);
	struct tm* tmp2 = localtime(&tmp);

	//assegnazione dei valori estratti nella variabile ricevuta
	(*dataCorrente).giorno = (*tmp2).tm_mday;
	(*dataCorrente).mese = (*tmp2).tm_mon + BASE_MESE_CORRENTE;
	(*dataCorrente).anno = (*tmp2).tm_year + BASE_ANNO_CORRENTE;
}

/*
 * Funzione giornoGiuliano:
 * La funzione riceve in ingresso una variabile di tipo data e restituisce
 * un valore di tipo double corrispondente al giorno giuliano corrispondente
 * alla data in ingresso, riferito alla mezzanotte di quel giorno.
 *
 * L'algoritmo utilizzato � stato preso dalla seguente pagina WikiPedia:
 * 		https://it.wikipedia.org/wiki/Giorno_giuliano#Conversione_da_data_normale_a_data_giuliana
 *
 * La funzione modf() della libreria math.h � utilizzata per estrarre la parte intera del primo
 * che le viene passato.
 */
double giornoGiuliano(data d1){
	// definizione delle variabili utilizzate nell'algoritmo
	double julian, a, b, c, d;
	data d2 = {15, 10, 1582};

	unsigned short anno = d1.anno;
	char mese = d1.mese;
	char giorno = d1.giorno;

	if(mese == 1 || mese == 2){
		anno -= 1;
		mese += 12;
	}

	if (confrontaDate(d1, d2) == PRIMA_DATA_ANTECEDENTE){
		a = 0;
		b = 0;
	}else{
		modf(anno/100, &a);
		modf(a / 4, &b);
		b += 2 - a;
	}

	modf(365.25 * anno, &c);
	modf(30.6001 * ( mese + 1), &d);

	// calcolo del giorno giuliano con riferimento alla mezzanotte dello stesso giorno
	julian = b + c + d + giorno + 1720994.50 - 2415020.50;

	return julian;
}

/*
 * Funzione diffDate:
 * La funzione riceve in ingresso le due date di cui calcolare la differenza.
 * Trasforma entrambe in giorno giuliano, utilizzando la funzione giornoGiuliano().
 * Di questi due valori ottenuti, ne calcola la differenza e ritorna il valore assoluto
 * di questa.
 */
int diffDate(data d1, data d2){
	return (int)(fabs(giornoGiuliano(d1) - giornoGiuliano(d2)));
}

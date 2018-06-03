// Implementazione delle fuznioni della libreria date.h

#include "date.h"

/**
 * La funzione ha il compito di verificare se la data d è significativa.
 * A tal scopo esegue determinati controlli sui valori che la compongono.
 * In primo luogo viene verificato se l'anno è bisestile oppure no, l'esito
 * è memorizzato in un apposito flag.
 * Successivamente viene verificato che il mese abbia un valore entro l'intervallo
 * [1, 12], dato che altri valori non sarebbero significativi.
 * L'ultima verifica avviene sul valore del giorno. Viene accertato che sia presente
 * nell'intervallo di giorni corretto in base al valore del mese e dell'anno.
 *
 */
int controlla_data(data d){
	char flag_bisestile = 0;

	// verifica che il valore dell'anno rispetti i limiti scelti (MIN_ANNO e MAX_ANNO)
	if(d.anno < MIN_ANNO || d.anno > MAX_ANNO)
		return 0;

	// verifica che il valore del mese rispetti i limiti (MIN_MESE e MAX_MESE)
	if(d.mese < MIN_MESE || d.mese > MAX_MESE)
		return 0;

	// verifica che il valore del giorno rispetti il limite inferiore (MIN_GIORNO)
	if(d.giorno < MIN_GIORNO)
		return 0;

	// verifica per anno bisestile
	if((d.anno % CONTROLLO_BISESTILE_1 == 0 && d.anno % CONTROLLO_BISESTILE_2 != 0) || d.anno % CONTROLLO_BISESTILE_3 == 0)
		flag_bisestile++;

	// verifica che il valore del giorno non sia maggiore del limite massimo relativo al mese della data
	switch(d.mese){
	// controllo per i mesi Aprile/Giugno/Settembre/Novembre (MAX_GIORNO_30)
	case 4:
	case 6:
	case 9:
	case 11:
		if(d.giorno > MAX_GIORNO_30)
			return 0;
		break;
	// controllo per il mese Febbraio (MAX_FEBBRAIO_BIS o MAX_FEBBRAIO)
	case 2:
		if(d.giorno > ((flag_bisestile)? MAX_FEBBRAIO_BIS : MAX_FEBBRAIO))
			return 0;
		break;
	// controllo per i mesi Gennaio/Marzo/Maggio/Luglio/Agosto/Ottobre/Dicembre (MAX_GIORNO)
	default:
		if(d.giorno > MAX_GIORNO_31)
			return 0;
		break;
	}

	// se non vengono individuati errori nella data, viene restituito il valore "1"
	return 1;
}


/*
 * Funzione confronta_date:
 * utilizzata per determinare se due date sono uguali o se una è antecede l'altra.
 * Parametri in ingresso:
 *  - le due date da confrontare.
 * Valori in uscita:
 *  - DATE_NON_VALIDE se le una delle due date non è significativa;
 *  - DATE_UGUALI se le due date hanno lo stesso valore;
 *  - PRIMA_DATA_ANTECEDENTE se la prima data precede la seconda;
 *  - SECONDA_DATA_ANTECEDENTE se la seconda data precede la prima.
 *  Analizza le due date verificando per prima cosa che siano entrambe significative, utilizzando
 *  la funzione controlla_data. Se le due date hanno dei valori corretti, le confronta attraverso una serie
 *  costrutti di selezione.
 */
int confronta_date(data d1, data d2){
	// verifica della validità delle date
	if (!controlla_data(d1) || !controlla_data(d2))
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
 * Funzione genera_data:
 * utilizzata per generare una data dato un intervallo.
 * Parametri in ingresso:
 * 	- puntatore alla variabile in cui memorizzare il valore della data generata
 *  - due date che determinano l'intervallo da utilizzare per la generazione della data.
 * Valori in uscita:
 *  - "0" se le date che costituiscono l'intervallo non sono significative o sono uguali
 *  - "1" se la generazione della data va a buon fine
 *  Analizza le due date, verificando per prima cosa se siano valide e non uguali. Nel caso in cui
 *  si verificasse uno di questi due eventi, la funzione restituisce 0 che equivale ad errore. Altrimenti
 *  verifica se le date sono ordinate correttamente, cioè che d1 sia antecedente a d2. Se ciò non
 *  è vero la funzione ritorna il valore restituito dalla chiamata della funzione stessa ma
 *  con valori di d1 e d2 invertiti (riga 115). Se i parametri sono corretti, vengono generati
 *  i valori dellanuova data e memorizzati all'interno della variabile puntata dal puntatore passato in
 *  ingresso. Vengono perciò gestiti i casi limite della generazione, analizzano i dati di volta in volta
 *  generati e determinando corretti estremi per la generazione. I casi limite si verificano
 *  nel momento in cui la generazione di anno e mese, crea un valore uguale a quelli presenti
 *  in uno dei due estremi.
 *  Al termine della generazione, la funzione ritorna un valore pari a 1.
 */
int genera_data(data* data_generata, data d1, data d2){
	int rnd; // numero casuale per la generazione dei valori della data
	char flag_bisestile = 0;
	unsigned short min, max; // variabili per contenere gli estremi per la generazione casuale dei valori della data

	// verifica della validità delle date che compongono l'intervallo
	int ris_confronto = confronta_date(d1, d2);
	if(ris_confronto == DATE_NON_VALIDE || ris_confronto == DATE_UGUALI)
		return 0;
	// ordinamento delle date, nel caso in cui d2 sia precedente a d1
	else if(ris_confronto == SECONDA_DATA_ANTECEDENTE)
		return genera_data(data_generata, d2, d1);

	// generazione di rnd
	rnd = rand();

	// generazione dell'anno
	(*data_generata).anno = rnd % (d2.anno - d1.anno + 1) + d1.anno;

	// verifica per anno bisestile
	if(((*data_generata).anno % CONTROLLO_BISESTILE_1 == 0 && (*data_generata).anno % CONTROLLO_BISESTILE_2 != 0) || (*data_generata).anno % CONTROLLO_BISESTILE_3 == 0)
		flag_bisestile++;

	// generazione del mese
	if((*data_generata).anno == d1.anno){
		min = (*data_generata).mese;
		max = MAX_MESE;
	}else if((*data_generata).anno == d2.anno){
		min = MIN_MESE;
		max = d2.mese;
	}else{
		min = MIN_MESE;
		max = MAX_MESE;
	}
	(*data_generata).mese = rnd % (max - min +1) + min;

	// generazione del giorno
	switch((*data_generata).mese){
	case 4:
	case 6:
	case 9:
	case 11:
		if((*data_generata).mese == d1.mese){
			min = d1.giorno;
			max = MAX_GIORNO_30;
		}else if((*data_generata).mese == d2.mese){
			min = MIN_GIORNO;
			max = d2.giorno;
		}else{
			min = MIN_GIORNO;
			max = MAX_GIORNO_30;
		}
		(*data_generata).giorno = rnd % (max - min +1) + min;
		break;
	case 2:
		if((*data_generata).mese == d1.mese){
			min = d1.giorno;
			max = (flag_bisestile)? MAX_FEBBRAIO_BIS : MAX_FEBBRAIO;
		}else if((*data_generata).mese == d2.mese){
			min = MIN_GIORNO;
			max = d2.giorno;
		}else{
			min = MIN_GIORNO;
			max = (flag_bisestile)? MAX_FEBBRAIO_BIS : MAX_FEBBRAIO;;
		}
		(*data_generata).giorno = rnd % (max - min +1) + min;
		break;
	default:
		if((*data_generata).mese == d1.mese){
			min = d1.giorno;
			max = MAX_GIORNO_31;
		}else if((*data_generata).mese == d2.mese){
			min = MIN_GIORNO;
			max = d2.giorno;
		}else{
			min = MIN_GIORNO;
			max = MAX_GIORNO_31;
		}
		(*data_generata).giorno = rnd % (max - min +1) + min;
		break;
	}
	return 1;
}

/*
 * Procedura data_odierna:
 * Ricava il valore della data odierna e la restituisce in ritorno, attraverso
 * le funzioni time() e localtime(). La funzione time() restituisce il tempo del calendario
 * di sistema nel formato aritmentico time_t, ed è convertito nella struttura struct tm,
 * presente nella libreria time.h attraverso la funzione localtime().
 * Da questa struttura è poi possibile estrarre il valore di giorno, mese e anno dai membri
 * identificati da tm_mday, tm_mon e tm_year. Per avere dati significativi, vengono sommati
 * i corrispondenti offeset alvalore del mese e dell'anno.
 */
data data_odierna(){
	data data_odierna;
	//estrazione della data odierna
	time_t tmp = time(NULL);
	struct tm* tmp2 = localtime(&tmp);

	//assegnazione dei valori estratti nella variabile ricevuta
	data_odierna.giorno = (*tmp2).tm_mday;
	data_odierna.mese = (*tmp2).tm_mon + BASE_MESE_CORRENTE;
	data_odierna.anno = (*tmp2).tm_year + BASE_ANNO_CORRENTE;

	return data_odierna;
}

/*
 * Funzione giorno_giuliano:
 * La funzione riceve in ingresso una variabile di tipo data e restituisce
 * un valore di tipo double corrispondente al giorno giuliano corrispondente
 * alla data in ingresso, riferito alla mezzanotte di quel giorno.
 *
 * L'algoritmo utilizzato è stato preso dalla seguente pagina WikiPedia:
 * 		https://it.wikipedia.org/wiki/Giorno_giuliano#Conversione_da_data_normale_a_data_giuliana
 *
 * La funzione modf() della libreria math.h è utilizzata per estrarre la parte intera del primo
 * che le viene passato.
 */
double giorno_giuliano(data d1){
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

	if (confronta_date(d1, d2) == PRIMA_DATA_ANTECEDENTE){
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
 * Funzione data_gregoriana:
 * La funzione riceve in ingresso una data nel formato di giorno giuliano,
 * su di essa applica un algoritmo e restituisce il valore ad essa corrispondente
 * in formato di tipo data.
 *
 * L'algoritmo applicato è stato preso dal seguente link Wikipedia:
 * 		https://it.wikipedia.org/wiki/Giorno_giuliano#Conversione_da_data_giuliana_a_data_normale
 */
data data_gregoriana(double data_giuliana){
	data data_gregoriana;
	double tmp, i, f, a, b, c, d, e, h, giorno, mese, anno;

	data_giuliana += 2415020.50;


	f = modf(data_giuliana + 0.5, &i);

	if(i <= 2299160){
		b = i;
	}else{
		modf((i - 1867216.25) / 36524.25, &a);
		modf(a / 4 , &tmp);
		b = i + 1 + a - tmp;
	}

	c = b + 1524;
	modf((c - 122.1) / 365.25, &d);
	modf(365.25 * d, &e);
	modf((c - e) / 30.6001, &h);

	modf(30.6001 * h, &tmp);
	giorno = c - e + f - tmp;

	if(h < 14){
		mese = h - 1;
	}else{
		mese = h - 13;
	}

	if(mese < 3){
		anno = d - 4715;
	}else{
		anno = d - 4716;
	}

	modf(giorno, &tmp);
	data_gregoriana.giorno = (unsigned short) tmp;
	modf(mese, &tmp);
	data_gregoriana.mese = (unsigned short) tmp;
	modf(anno, &tmp);
	data_gregoriana.anno = (unsigned short) tmp;

	return data_gregoriana;
}


/*
 * Funzione diff_date:
 * La funzione riceve in ingresso le due date di cui calcolare la differenza.
 * Trasforma entrambe in giorno giuliano, utilizzando la funzione giorno_giuliano().
 * Di questi due valori ottenuti, ne calcola la differenza e ritorna il valore assoluto
 * di questa.
 */
int diff_date(data d1, data d2){
	return (int)(fabs(giorno_giuliano(d1) - giorno_giuliano(d2)));
}


/*
 * Funzione shiftdata_odierna:
 * La funzione genera una data partendo dalla data odierna e shiftandola del valore
 * ricevuto in ingresso attraverso il parametro valoreShift.
 * In partiolare la data è generata trasformando prima la data odierna in
 * giorno giuliano. Al valore ottenuto viene sommato algebricamente valoreShift,
 * ottenendo il giorno giuliano della data da generare. Questa è convertita nel tipo di dato
 * data attraverso la funzione giornoGregoriano(). La data generata è restituita come valore di ritorno.
 */
data shift_data_odierna(short valore_shift){
	// calcolo della data odierna in giorno giuliano
	double data_odierna_giuliana = giorno_giuliano(data_odierna());

	// calcolo della data shiftata in giorno giuliano
	double data_generata_giuliana = data_odierna_giuliana + valore_shift;

	// conversione in data gregoriana della data generata
	data data_generata_gregoriana = data_gregoriana(data_generata_giuliana);

	return data_generata_gregoriana;
}


data input_data(){
	data data_inserita;		// variabile per contenere l'input dell'utente
	int esito_input;		// variabile per verificare l'esito dell'input

	// ciclo per ripetere la richiesta dell'input fino a ricevere una data valida
	do{
		printf("Inserire la data <gg/mm/aaaa>: ");
		esito_input = scanf("%hu/%hu/%hu", &data_inserita.giorno, &data_inserita.mese, &data_inserita.anno);
		fflush(stdin);

		// controllo sull'input ricevuto
		if(esito_input < NUM_CAMPI_DATA || controlla_data(data_inserita) == 0){
			puts("\nInserimento non valido. Ripeterlo.");
		}
	}while(esito_input < NUM_CAMPI_DATA || controlla_data(data_inserita) == 0);

	puts("Inserimento valido.");

	return data_inserita;
}

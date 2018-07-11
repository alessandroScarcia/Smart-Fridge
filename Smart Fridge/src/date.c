/// Implementazione delle funzioni della libreria date.h
#include "date.h"
/**
 * Funzione controlla_data(data d):
 *
 * La funzione ha il compito di verificare se la data d è significativa.
 *
 * A tal scopo esegue determinati controlli sui valori che la compongono.
 * In primo luogo viene verificato se l'anno è bisestile oppure no, l'esito
 * è memorizzato in un apposito flag.
 *
 * Successivamente viene verificato che il mese abbia un valore entro l'intervallo
 * [1, 12], dato che altri valori non sarebbero significativi.
 *
 * L'ultima verifica avviene sul valore del giorno. Viene accertato che sia nei limiti
 * dell'intervallo corretto, in base al valore del mese e dell'anno.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito deve essere controllato per verificare l'esito del controllo sulla data.
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


/**
 * Funzione giorno_giuliano(data d1):
 *
 * La funzione riceve in ingresso una variabile di tipo data e restituisce
 * un valore di tipo double equivalente al giorno giuliano corrispondente
 * alla data in ingresso.
 *
 * L'algoritmo utilizzato è stato preso dalla seguente pagina WikiPedia:
 * 		https://it.wikipedia.org/wiki/Giorno_giuliano#Conversione_da_data_normale_a_data_giuliana
 *
 * La funzione modf() della libreria math.h è utilizzata per estrarre la parte intera del primo
 * che le viene passato.
 *
 * @pre La data passata alla funzione deve avere un valore significativo.
 *
 * @post Il valore restituito deve essere diverso da DATA_NON_VALIDA
 *
 */
double giorno_giuliano(data d1){
	// Se la data inserita non è valida, viene restituiro DATA_NON_VALIDA
	if(controlla_data(d1) == 0){
		return DATA_NON_VALIDA;
	}
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


/**
 * Funzione data_gregoriana:
 * La funzione riceve in ingresso una data nel formato di giorno giuliano,
 * su di essa applica un algoritmo e restituisce il valore ad essa corrispondente
 * in formato di tipo data.
 *
 * L'algoritmo applicato è stato preso dal seguente link Wikipedia:
 * 		https://it.wikipedia.org/wiki/Giorno_giuliano#Conversione_da_data_giuliana_a_data_normale
 *
 * @pre La data passata alla funzione deve avere un valore significativo.
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


/**
 * Funzione diff_date(data d1, data d2):
 *
 * La funzione memorizza nella variabile puntata dal puntatore differenza,
 * la differenza in giorni fra le due date ricevute. La differenza
 * è calcolata utilizzando il corrispettivo valore giuliano delle due date.
 *
 * Se le date non sono valide, la funzione restituisce 0.
 * Altrimenti restituisce 1 se il calcolo della differenza avviene correttamente.
 *
 * @pre Le due date devono essere significative per un calcolo corretto della differenza.
 * @post Deve essere controllato il valore di ritornodella funzione per determinarne l'esito.
 */
int diff_date(int* differenza, data d1, data d2){
	// Controlllo sul significato delle due date
	if(controlla_data(d1) == 0 || controlla_data(d2) == 0){
		return DATA_NON_VALIDA;
	}

	// Calcolo della differenza fra le due date
	*differenza = (int)(giorno_giuliano(d1) - giorno_giuliano(d2));
	return 1;
}


/**
 * Funzione confronta_date(data d1, data d2):
 *
 * La funzione determina l'ugualianza o la disugualianza fra le due date ricevute,
 * analizzandone la differenza.
 *
 * Il valore che assume la differenza può essere di tre tipi:
 *  - Uguale a zero -> Le date sono uguali
 *
 *  - Maggiore di zero -> La seconda data antecede la prima
 *
 *  - Minore di zero -> La prima data antecede la seconda.
 *
 * Dopo aver verificato il significato delle date, se sono valide viene calcolata
 * la differenza e si determina in quale tipo precedentemente elencato rientra e viene
 * ritornato il corrispondente valore.
 *
 * Se una delle date o entrambe non sono significative, viene ritornato DATA_NON_VALIDA.
 *
 * @pre Le date utilizzate per richiamare la funzione devono essere significative.
 * @post Il valore di ritorno della funzione deve essere controllato per determinarne l'esito.
 *
 */
int confronta_date(data d1, data d2){
	int differenza_date; // Variabile per memorizzare la differenza di giorni fra le due date

	// Verifica della validità delle date
	if (controlla_data(d1) == 0 || controlla_data(d2) == 0)
		return DATA_NON_VALIDA;

	// Calcolo dei giprni di differenza fra le due date
	diff_date(&differenza_date, d1, d2);
	// verifica per l'uguaglianza fra le date
	if(differenza_date == 0)
		return DATE_UGUALI;

	// Determinazione della data precedente all'altra
	else if(differenza_date < 0)
		return PRIMA_DATA_ANTECEDENTE;
	else
		return SECONDA_DATA_ANTECEDENTE;
}

/**
 * Funzione genera_data(data* data_generata, data d1, data d2):
 *
 * La funzione permette di generare una data all'interno in un intervallo
 * fra due date. La data generata è memorizzata nella variabile puntata da data_generata.
 *
 * Per prima cosa la funzione si accerta che le date siano significative, non uguliali e che d1
 * sia antecedente a d2. Nei primi due vengono ritornati rispettivamente i valori DATA_NON VALIDA
 * o DATE_UGUALI. Nel terzo caso viene ritornato il valore ottenuto richiamando la funzione
 * ma con le date scambiate.
 *
 * Se la generazione va a buon fine, la funzione restituisce 1.
 *
 * N.B. Prima di calcolare un valore della data, vengono generati gli estremi per la generazione. Questo
 * serve ad evitare di generare date fuori dall'intervallo, a seguito di generazioni di valori di anno e mese
 * uguali ai valori degli estremi della data da generare.
 *
 * @pre Le date inserite devono essere significative e d1 deve precedere d2.
 * @post Il valore di ritorno della funzione deve essere controllato per determinare l'esito della generazione.
 */
int genera_data(data* data_generata, data d1, data d2){
	int rnd; // Numero casuale per la generazione dei valori della data
	char flag_bisestile = 0;
	unsigned short min, max; // Variabili per contenere gli estremi per la generazione casuale dei valori della data
	int esito_confronto;

	// Verifica della validità delle date che compongono l'intervallo
	esito_confronto = confronta_date(d1, d2);
	if(esito_confronto == DATA_NON_VALIDA || esito_confronto == DATE_UGUALI)
		return esito_confronto;
	// Ordinamento delle date, nel caso in cui d2 sia precedente a d1
	else if(esito_confronto == SECONDA_DATA_ANTECEDENTE)
		return genera_data(data_generata, d2, d1);

	// Generazione di rnd
	rnd = rand();

	// Generazione dell'anno
	(*data_generata).anno = rnd % (d2.anno - d1.anno + 1) + d1.anno;

	// Verifica per anno bisestile
	if(((*data_generata).anno % CONTROLLO_BISESTILE_1 == 0 && (*data_generata).anno % CONTROLLO_BISESTILE_2 != 0) || (*data_generata).anno % CONTROLLO_BISESTILE_3 == 0)
		flag_bisestile = 1;

	// Calcolo degli estremi corretti per la generazione del mese
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

	// Generazione del mese
	(*data_generata).mese = rnd % (max - min +1) + min;

	// Calcolo degli estremi corretti per la generazione del mese
	switch((*data_generata).mese){
	case 4:
	case 6:
	case 9:
	case 11:
		if((*data_generata).mese == d1.mese && (*data_generata).anno == d1.anno){
			min = d1.giorno;
			max = MAX_GIORNO_30;
		}else if((*data_generata).mese == d2.mese && (*data_generata).anno == d2.anno){
			min = MIN_GIORNO;
			max = d2.giorno;
		}else{
			min = MIN_GIORNO;
			max = MAX_GIORNO_30;
		}
		break;
	case 2:
		if((*data_generata).mese == d1.mese && (*data_generata).anno == d1.anno){
			min = d1.giorno;
			max = (flag_bisestile)? MAX_FEBBRAIO_BIS : MAX_FEBBRAIO;
		}else if((*data_generata).mese == d2.mese && (*data_generata).anno == d2.anno){
			min = MIN_GIORNO;
			max = d2.giorno;
		}else{
			min = MIN_GIORNO;
			max = (flag_bisestile)? MAX_FEBBRAIO_BIS : MAX_FEBBRAIO;;
		}
		break;
	default:
		if((*data_generata).mese == d1.mese && (*data_generata).anno == d1.anno){
			min = d1.giorno;
			max = MAX_GIORNO_31;
		}else if((*data_generata).mese == d2.mese && (*data_generata).anno == d2.anno){
			min = MIN_GIORNO;
			max = d2.giorno;
		}else{
			min = MIN_GIORNO;
			max = MAX_GIORNO_31;
		}
		break;
	}
	// Generazione del giorno
	(*data_generata).giorno = rnd % (max - min +1) + min;
	return 1;
}

/**
 * Funzione data_odierna():
 *
 * La funzione permette di determinare la data odierna utilizzando le funzioni della libreria <time.h>.
 * Estrapola la data almomento dell'esecuzione attraverso la funzione time(), ottenendo un valore di tipo time_t.
 * Questo valore viene poi convertito, con la funzione localtime(), nella struct tm., in cui i campi tm_mday,
 * tm_mon e tm_year contengono rispettivamente il giorno, il mese e l'anno della data odierna.
 *
 * @post Il valore restituito dalla funzione deve essere memorizzato in una variabile di tipo data.
 */
data data_odierna(){
	data data_odierna; // Variabile di tipo data che conterrà la data al momento d'esecuzione

	// Estrazione della data al momento di esecuzione
	time_t tmp = time(NULL);
	struct tm* tmp2 = localtime(&tmp);

	// Assegnazione dei valori estratti nella variabile ricevuta
	data_odierna.giorno = (*tmp2).tm_mday;
	data_odierna.mese = (*tmp2).tm_mon + BASE_MESE_CORRENTE;
	data_odierna.anno = (*tmp2).tm_year + BASE_ANNO_CORRENTE;

	return data_odierna;
}


/**
 * La funzione giorno_odierno() restituisce un numero, corrispondente al giorno della settimana,
 * nell'intervallo [0 - 6]. L'estrazione del giorno odierno della settimana avviene ricavando
 * la data odierna attraverso la funzione time(), convertendola in struct tm attraverso la funzione
 * localtime() ed estrapolandolo leggendo il valore del membro tm_wday.
 *
 * @post Il valore ritornato va interpretato come 0 Lunedì, 6 Domenica.
 */
int giorno_odierno(){
	// Estrazione dei dati della data odierna
	time_t tmp = time(NULL);
	struct tm* tmp2 = localtime(&tmp);

	// Va ritornato il numero corridponente al giorno odierno della settimana
	return (*tmp2).tm_wday;
}


/**
 * Funzione shiftdata_odierna:
 *
 * La funzione genera una data partendo dalla data odierna e shiftandola del valore
 * ricevuto in ingresso attraverso il parametro valoreShift.
 *
 * In partiolare la data è generata trasformando prima la data odierna in
 * giorno giuliano. Al valore ottenuto viene sommato algebricamente valoreShift,
 * ottenendo il giorno giuliano della data da generare. Questa è convertita nel tipo di dato
 * "data" attraverso la funzione giornoGregoriano().
 *
 * La data generata è restituita come valore di ritorno.
 *
 * @post La data generata restituita deve essere memorizzata in una variabile di tipo data.
 */
data shift_data_odierna(short valore_shift){
	// Calcolo della data odierna in giorno giuliano
	double data_odierna_giuliana = giorno_giuliano(data_odierna());

	// Calcolo della data shiftata in giorno giuliano
	double data_generata_giuliana = data_odierna_giuliana + valore_shift;

	// Conversione in data gregoriana della data generata
	data data_generata_gregoriana = data_gregoriana(data_generata_giuliana);

	return data_generata_gregoriana;
}

/**
 * Funzione input_data():
 *
 * La funzione permette la richiesta all'utente di una data. Quest'ultima viene controllata
 * e se non è significativa la richiesta di input viene ripetuta mostrando un messaggio di errore
 * generico. Il primo input valido viene restituito dalla funzione.
 *
 * @post La data restituita dalla funzione deve essere memorizzata in una variabile di tipo data.
 */
data input_data(){
	data data_inserita;		// Variabile per contenere l'input dell'utente
	int esito_input;		// Variabile per verificare l'esito dell'input

	// Ciclo per ripetere la richiesta dell'input fino a ricevere una data valida
	do{
		printf("Inserire la data <gg/mm/aaaa>: ");
		esito_input = scanf("%hu/%hu/%hu", &data_inserita.giorno, &data_inserita.mese, &data_inserita.anno);
		if(pulisci_stdin() == 1){ // Pulizia dell'input non consumato e intercettazione dell'errore
			esito_input = 0;
		}


		// Controllo sull'input ricevuto
		if(esito_input < NUM_CAMPI_DATA || controlla_data(data_inserita) == 0){
			puts("\nData inserita non significativa. Ripetere l'inserimento.");
		}
	}while(esito_input < NUM_CAMPI_DATA || controlla_data(data_inserita) == 0);

	puts("Data inserita significativa.");

	return data_inserita;
}

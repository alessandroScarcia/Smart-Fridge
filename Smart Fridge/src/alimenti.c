#include "alimenti.h"

/**
 * La funzione, avendo in ingresso gli alimenti del frigo con il relativo numero, si occupa di riordinare tali alimenti, o meglio l'array di struct che li contiene, in base
 * alla data di scadenza. L'ordinamento e' effettuato attraverso l'algoritmo shell sort in quanto risultava quello piú semplice ed efficace da implementare dato il quantitativo
 * di dati(non e' eccessivo considerato che il frigo avrá al massimo un centinaio al massimo di alimenti). Inoltre in questa versione di shell sort si e' optato di non creare a parte un
 * array contenente i gap (distanza per gli scambi), ma di sfruttare il numero di alimenti inizialmente e poi dividere per 2 di volta in volta la lunghezza.
 *
 * @pre  l'array di struct deve possedere almeno un alimento da ordinare e pertanto il numero di alimenti deve essere idoneo.
 * @post L'ordinamento deve essere stato effettuato con successo.
 *
 */
int ordina_alimenti_scadenza(alimento_frigo* alimenti_frigo, int  num_alimenti){

	alimento_frigo tmp; // Variabile si sostegno per effettuare gli scambi fra alimenti

	for (int gap = num_alimenti/2; gap > 0; gap /= 2){

		for (int i = gap; i < num_alimenti; i++){

			// salva il corrente elemento puntato da i in tmp
			tmp = alimenti_frigo[i];

			//sposta i precedenti elementi fino alla corretta locazione di alimenti_frigo[i] e' 	trovata
			int j;
			// diff_date(differenza,alimenti_frigo[j - gap].scadenza, data_odierna()) > diff_date(differenza,tmp.scadenza,data_odierna())
			for (j = i; j >= gap && confronta_date(alimenti_frigo[j - gap].scadenza, tmp.scadenza) == SECONDA_DATA_ANTECEDENTE; j -= gap){
				alimenti_frigo[j] = alimenti_frigo[j - gap];
			}
			//  inserisci tmp (l'originale alimenti_frigo[i]) nella sua corretta locazione
			alimenti_frigo[j] = tmp;
		}
	}

	printf("Ordinamento per scadenza effettuato con successo...ora puoi preparare una ricetta ottimizzata\n");
	return 1;


}


/**
 * Funzione che si occupa di contare gli alimenti presenti nel database.
 *
 * @pre  Nessuna pre condizione particolare
 * @post Il valore restituito deve essere un intero significativo (>=0)
 */
int conta_alimenti_database(){
	FILE* stream = NULL;						// Puntatore a FILE_DATABASE_ALIMENTI
	int num_alimenti = 0;

	// Apertura del file in modalità lettura binaria
	if((stream = fopen(FILE_DATABASE_ALIMENTI, "rb")) == NULL){
		return num_alimenti;
	}else{
		// Calcolo del numero alimenti del database, posizionando il puntatore alla fine del file
		// e dividendo la sua posizione per la grandezza di un signolo elemento
		fseek(stream, 0, SEEK_END);
		num_alimenti = ftell(stream) / sizeof(alimento_database);

	}

	fclose(stream); //chiudi il file
	return num_alimenti;
}


/**
 * Funzione che conta gli alimenti presenti nel frigo e ne restituisce il valore. Il conteggio viene effettuato escludendo le righe vuote nel database
 *
 * @pre  Nessuna pre condizione particolare
 * @post Il valore restituito deve essere un intero significativo (>=0)
 */
int conta_alimenti_frigo(){
	FILE* stream = NULL;					// Puntatore a FILE_FRIGO
	alimento_frigo info_alimento;			// genero una struct di riferimento che mi permette di scorrere all'interno del file di tipo binario

	int num_alimenti = 0;	// Numero di alimenti presenti nel frigo

	// Apertura di FILE_FRIGO
	if((stream = fopen(FILE_FRIGO, "rb")) == NULL){
		return num_alimenti; // Se il file non può essere aperto, viene ritornato 0
	}else{
		// Viene attraversato il contenuto del frigo, aumentando il numero di alimenti quando l'alimento letto ha nome diverso da stringa vuota
		while(fread(&info_alimento, sizeof(info_alimento), 1, stream) > 0){
			if(strcmp(info_alimento.nome, "") != 0)
				num_alimenti++; // Incremento il contatore del numero di alimenti

		}
	}

	fclose(stream);
	return num_alimenti;
}


/**
 * Dopo aver popolato un array di struct di tipo alimento frigo la funzione si occupa di confrontare la data di scadenza con la data odierna. Qualora la differenza tra date
 * produca un valore negativo vuol, dire che l'alimento e' scaduto e pertanto occorre incrementare il contatore. Questa funzione serve principalmente nel menu principale per
 * conteggiare il numero di notifiche che devono eesre viste dall'utente
 *
 * @pre  Nessuna pre condizione particolare
 * @post Il valore restituito deve essere un intero significativo (>=0)
 */
int conta_alimenti_scaduti(){
	int num_alimenti_scaduti = 0; // Numero di alimenti scaduti rilevati
	int num_alimenti = conta_alimenti_frigo(FILE_FRIGO); // Numero di alimenti contenuti nel frigo
	data data_esecuzione; // Data al momento di esecuzione

	// Se è presente almeno un alimento nel frigo, è necessario iniziare i controlli
	if(num_alimenti > 0){
		// Dichiarazione e caricamento degli alimenti nel frigo all'interno di un array
		alimento_frigo alimenti_frigo[num_alimenti];
		leggi_frigo(alimenti_frigo);

		// Determinazione della data al momento di esecuzione
		data_esecuzione = data_odierna();

		// Per ogni alimento del frigo, se la loro data è antecedente a data_esecuzione, va incrementato num_alimenti_scaduti
		for(int i = 0; i < num_alimenti; i++){
			//se la differenza tra data odierna e data di scadenza produce un valore negativo allora l'alimento e' scaduto
			if(confronta_date(alimenti_frigo[i].scadenza, data_esecuzione) == PRIMA_DATA_ANTECEDENTE){
				num_alimenti_scaduti++;
			}
		}
	}

	return num_alimenti_scaduti;
}





/**
 * Funzione che avendo ricevuto in ingresso l'array di struct vuoto che conterrá gli alimenti del database, si occuperá di riempire tale array con
 * i dati salvati all'interno del file e di restituire al termine il numero di tali alimenti salvati. Come per ogni altra funzione che conta gli
 *  elementi del file anche in questo caso si saltano le righe vuote
 *
 * @pre  Deve essere passato un array di struct della giusta dimensione
 * @post Il valore restituito deve essere un intero significativo (>=0)
 */
int leggi_database_alimenti(alimento_database* lista_alimenti){
	FILE* stream = NULL; // Puntatore a FILE_DATABASE_ALIEMNTI
	alimento_database alimento_letto;//creo una struttura di riferimento per scorrere all'interno del file
	int num_alimenti_letti = 0; // Indice dell'alimento letto dal database

	if((stream = fopen(FILE_DATABASE_ALIMENTI, "rb+")) == NULL){ // Apertura del file in modalità lettura binaria e aggiornamento
		return num_alimenti_letti; // Se il file non può essere aperto è ritronato 0
	}else{
		// Viene attraversato il file copiando ogni elemento nell'array ricevuto in lista_alimenti
		while(fread(&alimento_letto, sizeof(alimento_database), 1, stream) > 0){
			lista_alimenti[num_alimenti_letti] = alimento_letto;

			num_alimenti_letti++;
		}

		fclose(stream);
		return num_alimenti_letti;//se la funzione e' andata a buon fine restituisci il numero di elementi caricati
	}
}



/**
 * Funzione che viene richiamata direttamente dal menu e dal gestore delle notifiche. Essa si occupa  di scovare tutti gli alimenti scaduti(se la differenza tra data odierna e data di scadenza
 * produce un valore negativo allora l'alimento e' scaduto) e di inizializzare la riga in maniera tale da poterla recuperare in una prossima scrittura. Ovviamente nella ricerca vengono
 * sempre scartate le righe vuote
 * @pre  Nessuna pre condizione particolare
 * @post Deve essere stata effettuata con successo la scrittura su file
 */
int eliminazione_alimenti_scaduti(){
	FILE* stream = NULL;
	alimento_frigo alimento_letto;
	data data_esecuzione;

	// Apertura del file in lettura e aggiornamento binario
	if((stream = fopen(FILE_FRIGO, "rb+")) == NULL){
		return 0; // Se non può essere aperto il file viene ritornato 0
	}else{
		data_esecuzione = data_odierna(); // Estrazione della data al momento di esecuzione

		// Viene attraversato il contenuto del frigo eliminando gli alimenti rilevati scaduti
		while(fread(&alimento_letto, sizeof(alimento_frigo), 1, stream)>0){
			// Se la riga letta contiene un alimento
			if(strcmp(alimento_letto.nome,"") != 0){
				// Se l'alimento è scaduto, va modificato il nome in stringa vuota così da considerarlo eliminato
				if(confronta_date(alimento_letto.scadenza, data_esecuzione) == PRIMA_DATA_ANTECEDENTE){
					//inizializzo la riga per eliminare il contenuto
					strcpy(alimento_letto.nome,"");

					// Scrittura nel file dell'eliminazione, riposizionando il puntatore all'inizio dell'alimento appena letto
					fseek(stream, -sizeof(alimento_frigo), SEEK_CUR);
					fwrite(&alimento_letto,sizeof(alimento_frigo), 1, stream);
				}
			}
		}
	}

	fclose(stream);
	return 1;
}






/**
 *Funzione che viene richiamata direttamente dal gestore delle notifiche e si occupa di visualizzare gli alimenti scaduti presenti
 *nel frigo e di mostrare la relativa data di scadenza.
 *
 * @pre  Nessuna pre condizione particolare
 * @post Deve essere termianta con successo la visualizzazione
 */
int visualizza_alimenti_scaduti(){
	int num_alimenti_scaduti = 0;
	int num_alimenti = conta_alimenti_frigo();

	if(num_alimenti == 0){
		puts("Non ci sono alimenti nel frigo.");
		return num_alimenti_scaduti;
	}

	alimento_frigo alimenti_frigo[num_alimenti];
	leggi_frigo(alimenti_frigo);

	//per tutti gli alimenti del frigo viene confrontata la data di scadenza con la data odierna. Qualora la scadenza sia passata
	//viene notificato che l'alimento x e' scaduto
    for(int i = 0; i < num_alimenti; i++){

    	if(confronta_date(alimenti_frigo[i].scadenza, data_odierna()) == PRIMA_DATA_ANTECEDENTE){
    		num_alimenti_scaduti++;
    		printf("Alimento scaduto: %s data di scadenza: %hu/%hu/%hu\n", alimenti_frigo[i].nome, alimenti_frigo[i].scadenza.giorno,
    				alimenti_frigo[i].scadenza.mese, alimenti_frigo[i].scadenza.anno);
    	}
    }

	return num_alimenti_scaduti;
}


/** Funzione che si occupa di controllare se l'unitá di misura di un alimento e' valida o meno. Qualora lo sia viene restituito 1
 * in caso contrario 0.
 *
 * @pre  Nessuna pre condizione particolare
 * @post nessuna post consizione particolare
 */
int controlla_unita_misura(char* unita_misura){
	if(strcmp(unita_misura, UNITA_KG) == 0
		|| strcmp(unita_misura, UNITA_G) == 0
		|| strcmp(unita_misura, UNITA_L) == 0
		|| strcmp(unita_misura, UNITA_ML) == 0
		|| strcmp(unita_misura, UNITA_PZ) == 0){
		return 1;
	}else{
		return 0;
	}
}





/**Funzione che si occupa di controllare se la quantitá e' valida imponendone un limite massimo di accettabilitá in base alla unitá di misura
 * corrispondente
 *
 * @pre  la quantitá deve essere un valore positivo >=0
 * @post nessuna post condizione particolare
 */
int controlla_quantita(float quantita, char* unita_misura){
	int sup_quantita;// Estremo superiore relativo all'unità di misura ricevuta

	// Identificazione del valore corretto per l'estremo superiore
	if(strcmp(unita_misura, UNITA_G) == 0){
		sup_quantita = MAX_QUANTITA_G;
	}else if(strcmp(unita_misura, UNITA_ML) == 0){
		sup_quantita = MAX_QUANTITA_ML;
	}else if(strcmp(unita_misura, UNITA_PZ) == 0){
		sup_quantita = MAX_QUANTITA_PZ;
	}else{
		return -1;
	}

	//se la quantitá non ha un valore accettabile restituisce 0 altrimenti 1
	if(quantita < MIN_QUANTITA || quantita > sup_quantita){
		return 0;
	}else{
		return 1;
	}
}




/**Funzione che si occupa di controllare la data di scadenza rispetto la data odierna. Se la data di scadenza non e' valida viene restituito -1.
 * In caso la data odierna sia antecedente viene restituito 1. Nel caso in cui la scadenza sia antecedente viene restituito 0
 *
 * @pre  la data di scadenza sia in un formato valido anche se viene effettuato il controllo
 * @post Deve essere restituito un valore in base all'esito del controllo
 */
int controlla_data_scadenza(data scadenza){
	if(confronta_date(scadenza, data_odierna()) == DATA_NON_VALIDA){
		return -1;
	}else if(confronta_date(scadenza, data_odierna()) == SECONDA_DATA_ANTECEDENTE){
		return 1;
	}else{
		return 0;
	}
}




/**Funzione che riceve in ingresso la quantità e l'unitá di misura e si occupa di effettuare una equivalenza del valore nel caso in cui
 * siamo in presenza di una sopra-unitá. Nel nostro caso le uniche unitá di misura che ci interessa convertire sono i kg e i lt. Qualora
 * l'unitá di misura passata sia una di queste 2 viene moltiplicata la quantitá e viene restituito 1 nel caso in cui sia stata
 * effettuata una equivalenza e 0 in caso contrario
 *
 * @pre  la quantitá sia diversa da 0
 * @post Sia stata effettuta l'equivalenza generando un valore valido
 */
int converti_quantita(float *quantita, char *unita_misura){
	if(strcmp(unita_misura, UNITA_KG) == 0){
		strcpy(unita_misura, UNITA_G);
		*quantita *= KG_TO_G;

		return 1;
	}else if(strcmp(unita_misura, UNITA_L) == 0){
		strcpy(unita_misura, UNITA_ML);
		*quantita *= L_TO_ML;

		return 1;
	}

	return 0;
}




/**Funzione che si occupa, ricevuta in input una stringa, di rendere tutti i caratteri minuscoli. La funzione serve in quanto potremmo
 * essere nel caso in cui ad una prima estrazione di un alimento(uova) esso sia sconosciuto, ed a seguito di una seconda estrazione(Uova)
 * lo stesso alimento ma con la lettera iniziale maiuscola risulterebbe comunque sconosciuto.
 *
 * @pre  la stringa passata in input deve contenere almeno un carattere
 * @post la stringa generata deve essere con i caratteri tutti minuscoli
 */
void abbassa_maiuscole(char *s){
	int i = 0;

	while(s[i]){//consuma tutti i caratteri della stringa passata e ponili minuscoli
		s[i] = tolower(s[i]);
		i++;
	}
}




/**Funzione che si occupa dell'inserimento da tastiera di un alimento nel formato valido per il caricamento. La funzione si basa
 * principalmente su un ciclo che analizza l'input dell'utente controllando i vari valori inseriti. Qualora la scanf non produca errori e
 * restituisca il numero massimo di "hit", l'alimento é da considerarsi valido e pertanto puó essere restituito dalla funzione
 *
 * @pre  Nessuna pre condizione particolare
 * @post Deve essere restituito un alimento nel formato valido
 */
alimento_frigo input_alimento_frigo(){
	int esito_input;
	alimento_frigo alimento;

	// Ciclo per la ripetizione dell'inserimento fino a riceverlo nel formato corretto
	do{
		puts("Inserisci l'alimento <nome> <quantità> <unità di misura> <gg/mm/aaaa>: ");
		esito_input = scanf("%20[a-zA-Z] %f %5[a-zA-Z] %hu/%hu/%hu",
				alimento.nome, &alimento.quantita, alimento.unita_misura,
				&alimento.scadenza.giorno, &alimento.scadenza.mese, &alimento.scadenza.anno);

		if(pulisci_stdin() == 1){
			esito_input = 0;
		}
		//se l'inserimento non avviene correttamente per tutti i campi che costituiscono le informazioni dell'alimento si avvisa l'utente
		//di rieffettuare l'inserimento
		if(esito_input != NUM_CAMPI_ALIMENTO_FRIGO){
			puts("Inserimento non valido. Ripeterlo.\n");
		}
	}while(esito_input != NUM_CAMPI_ALIMENTO_FRIGO);

	return alimento;
}




/**Funzione che si occupa dell'inserimento di una unitá di misura in un formato valido. Qualora l'unitá di misura sia composta da sole lettere
 * viene richiamata la funzione controlla_unita_misura per effettuare un ulteriore check sulla esistenza di tale unitá di misura tra quelle
 * standard(g,lt,kg,ml). L'inserimento viene effettuato fino a quando non viene inserita una unitá di misura valida
 *
 * @pre  Nessuna pre condizione particolare
 * @post Deve essere memorizzata nel puntatore una unitá di misura valida
 */
void input_unita_misura(char *unita_misura){
	int esito_input;
	int esito_controllo;

	// Ciclo per la ripetizione dell'inserimento fino a riceverlo significativo
	do{
		puts("Inserisci l'unità di misura: ");
		esito_input = scanf("%5[a-zA-Z]", unita_misura);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		esito_controllo = controlla_unita_misura(unita_misura);

		if(esito_controllo != 1 || esito_input != 1){
			puts("Inserimento non valido. Ripeterlo.\n");
		}
	}while(esito_controllo != 1 || esito_input != 1);
}




/**Funzione che si occupa dell'inserimento di una quantitá in un formato valido. Qualora la quantitá sia composta da soli numeri(ricordiamo
 * che la quantitá puó essere con la virgola per quelle quantitá che prevedono unitá di misura come kg ed lt), viene richiamata
 * la funzione controlla_quantita per effettuare un ulteriore check sulla validitá di tale quantitá anche in base alla unitá di misura passata
 * ( avere 1000g di un alimento equivale a 1kg). L'inserimento viene effettuato fino a quando non viene inserita una quantitá valida
 *
 * @pre  Nessuna pre condizione particolare
 * @post Deve essere restituita una quantitá significativa
 */
float input_quantita(char *unita_misura){
	int esito_input;
	int esito_controllo;
	float quantita = 0;

	// Ciclo per la ripetizione dell'inserimento fino a riceverlo significativo
	do{
		puts("Inserisci la quantita: ");
		esito_input = scanf("%3f", &quantita);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		esito_controllo = controlla_quantita(quantita, unita_misura);//controlla validitá quantitá

		if(esito_controllo != 1 || esito_input != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}

	}while(esito_controllo != 1 || esito_input != 1);

	return quantita;
}





/**Funzione che si occupa dell'inserimento di una data di scadenza in un formato valido. Qualora la scadenza sia significativa viene richiamata
 * la funzione controlla_data_scadenza per effettuare un ulteriore check sulla validitá di tale data. L'inserimento viene effettuato fino a quando
 * il controllo sulla validitá non viene superato
 *
 * @pre  Nessuna pre condizione particolare
 * @post Deve essere restituita una data di scadenza valida
 */
data input_data_scadenza(){
	data data_inserita;
	int esito_controllo;

// Ciclo per la ripetizione dell'inserimento fino a riceverlo significativo
	do{
		data_inserita = input_data();//richiamo alla funzione che effettua l'inserimento della data in un formato valido

		esito_controllo = controlla_data_scadenza(data_inserita);//controllo sulla validitá della data di scadenza
		if(esito_controllo != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}
	}while(esito_controllo != 1);

	return data_inserita;
}







/**Funzione che si occupa dell'inserimento delle kcal di un alimento in un formato valido. L'inserimento viene effettuato fino a quando
 * il controllo sulla validitá non viene superato
 *
 * @pre  Nessuna pre condizione particolare
 * @post Devono essere restituite delle kcal valide
 */
int input_kcal(char *nome_alimento, int campione_kcal, char *unita_misura){
	float kcal;
	int esito_input;
	int esito_controllo;

// Ciclo per la ripetizione dell'inserimento fino a riceverlo significativo
	do{
		printf("Inserisci le kcal per <%d %s> dell'alimento <%s> : ", campione_kcal, unita_misura, nome_alimento);
		esito_input = scanf("%4f", &kcal);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(kcal < MIN_KCAL_CAMPIONE || kcal > MAX_KCAL_CAMPIONE){
			esito_controllo = 0;
		}else{
			esito_controllo = 1;
		}

		if(esito_input != 1 || esito_controllo != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}
	}while(esito_input != 1 || esito_controllo != 1);

	return kcal;
}




/**Funzione che si occupa dell'inserimento di un id valido e del controllo di tale valore in base al range in cui é possibile stabilirlo.
 * Se l'id dell'alimento ha un valore compreso tra il minimo consentito ed il numero degli alimenti del frigo allora é valido e quindi puó
 * essere restituito dalla funzione
 *
 * @pre  Deve essere passato un numero di alimenti maggiore o uguale di 0
 * @post Deve essere restituito un id valido
 */
int input_id_alimento(int num_alimenti_frigo){
	int id_alimento;
	int esito_input;
	int esito_controllo;

// Ciclo per la ripetizione dell'inserimento fino a riceverlo significativo
	do{
		printf("Inserisci l'id di un alimento: ");
		esito_input = scanf("%2d", &id_alimento);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		//controlli sulla validitá del valore inserito
		if(id_alimento < MIN_ID_ALIMENTO || id_alimento > num_alimenti_frigo){
			esito_controllo = 0;
		}else{
			esito_controllo = 1;
		}

		if(esito_input != 1 || esito_controllo != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}
	}while(esito_input != 1 || esito_controllo != 1);

	return id_alimento;
}




/**Funzione che si occupa di restituire in maniera opportuna un campione valido in base alla unitá di misura che passiamo alla funzione.
 * Qualora l'unitá di misura non sia valida viene restituito 0 per segnalare quindi un errore
 *
 * @pre  Deve essere passata una stringa con almeno un carattere
 * @post Deve essere restituito un campione valido in base alla unitá di misura oppure 0 per segnalare una invaliditá nello stabilire il campione
 */
int calcola_campione_kcal(char *unita_misura){
	// Analisi dell'unità di misura ricevuta e restituzione del valore di campione corrispondente
	if(strcmp(unita_misura, UNITA_G) == 0){
		return CAMPIONE_G;

	}else if(strcmp(unita_misura, UNITA_ML) == 0){
		return CAMPIONE_ML;

	}else if(strcmp(unita_misura, UNITA_PZ) == 0){
		return CAMPIONE_PZ;

	}

	// Se l'unità di misura non è valida, ritorna 0
	return 0;
}





/**Funzione che si occupa dell'input da tastiera della soglia della spesa. La soglia ricordiamo che é una quantitá al di sotto della quale un alimento
 * deve essere considerato come candidato per essere comprato. Alla funzione vengono passati il nome dell'alimento(deve essere mostrato in output
 * per fornire ulteriori informazioni sull'inserimento da effettuare) e l'unitá di misura per effettuare un controllo sulla validitá
 * del valore inserito come soglia. Qualora il valore sia significativo viene restituito dalla funzione altrimenti si continua a chiedere l'inserimento.
 *
 * @pre  Deve essere passato un nome di alimento con almeno un carattere e una unitá di misura significativa
 * @post Deve essere restituita una soglia valida
 */
float input_soglia_spesa(char *nome_alimento, char* unita_misura){
	float soglia;
	int esito_input;
	int esito_controllo;

	// Ciclo per la ripetizione dell'inserimento fino a riceverlo significativo
	do{
		printf("Inserisci la soglia per l'inserimento nella generazione\n della spesa dell'alimento <%s> [0 per escluderlo]: ", nome_alimento);
		esito_input = scanf("%3f", &soglia);


		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		//controlli sulla validitá del valore inserito
		if(esito_input == 1){
			if(soglia == 0){
				esito_controllo = 1;
			}else{
				esito_controllo = controlla_quantita(soglia, unita_misura);
			}
		}

		if(esito_controllo != 1 || esito_input != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}
	}while(esito_controllo != 1 || esito_input != 1);

	return soglia;
}




/** Funzione che si occupa di ricercare un alimento all'interno del database alimenti. Vengono passati come parametri il nome dell'alimento su cui
 * effettuare la ricerca e la struct che sará popolata con le informazioni dell'alimento qualora sia presemte nel database. Una volta effettuato
 * il caricamento delle informazioni viene restituito 1 e in caso contrario viene restituito 0
 *
 * @pre  il nome dell'alimento passato deve essere composto da almeno un carattere
 * @post In caso venga trovato l'alimento deve essere restituita la struct popolata da informazioni significative
 */
int ricerca_alimento_database(char *nome_alimento, alimento_database *alimento_estratto){
	alimento_database alimento;
	FILE *stream = NULL;

	if((stream = fopen(FILE_DATABASE_ALIMENTI, "rb")) == NULL){
		return -1;
	}else{
		//estrazione degli alimenti e confronto con il nome passato come parametro.
		while(feof(stream) == 0){
			fread(&alimento, sizeof(alimento_database), 1, stream);

		//se l'alimento é presente nel database vengono copiate le informazioni
			if(strcmp(alimento.nome, nome_alimento) == 0){
				*alimento_estratto =  alimento;
				return 1;
			}
		}

		return 0;
	}
}





/**Funzione che si occupa di inserire le informazioni riguardanti un alimento sconosciuto all'interno del database. In un primo momento
 * viene fatto un check sul nome dell'alimento qualora sia giá presente nel database(viene richiamata la funzione ricerca_database). Se
 * la ricerca non é andata a buon fine pertanto l'alimento non é presente nel database vengono memorizzate le informazioni passate come parametri
 * ossia il nome e l'unitá di misura(la scelta di passare questo dato é per non richiedere all'utente di reinserirlo in quanto viene direttamente
 * estratto altrove), ed in seguito le altre informazioni(richieste in input o calcolate dinamicamente) come le kcal, la soglia per fare la spesa
 * e il campione kcal.
 *
 * @pre  il nome dell'alimento e l'unitá di misura devono possedere almeno un carattere
 * @post Devono essere state memorizzate correttamente tutte le informazioni riguardanti un dato alimento
 */
int aggiorna_database(char* nome_alimento, char* unita_misura){
	FILE *stream = NULL;								// Puntatore al file database_alimenti
	alimento_database alimento_database;				// Variabile per comunicare con il database
	char flag_presenza = 0;								// Flag per memorizzare la presenza nel database dell'alimento in analisi

	// Tentativo di apertura del FILE_DATABASE
	if((stream = fopen(FILE_DATABASE_ALIMENTI, "rb+")) == NULL){
		// Se non esiste nessun FILE_DATABASE deve essere creato
		if((stream = fopen(FILE_DATABASE_ALIMENTI, "wb+")) == NULL){
			// Se non può essere creato il file, viene ritornato 0
			return -1;
		}
	}

	// Se si superano i controlli sull'apertura del file, possiamo effettuare l'aggiornamento del database

	fseek(stream, 0, SEEK_SET);

	flag_presenza=ricerca_alimento_database(nome_alimento, &alimento_database);

	// Se flag_presenza è uguale a zero, l'alimento è sconosciuto e bisogna aggiungerlo
	if(flag_presenza == 0){
		printf("L'alimento identificato come <%s> è sconosciuto.\n", nome_alimento);

		// Creazione della riga da inserire nel database
		// Nome dell'alimento
		strcpy(alimento_database.nome, nome_alimento);
		// Unità di misura
		strcpy(alimento_database.unita_misura, unita_misura);
		// Campione per il calcolo delle kcal
		alimento_database.campione_kcal = calcola_campione_kcal(alimento_database.unita_misura);
		// Kcal per campione di alimento
		alimento_database.kcal = input_kcal(alimento_database.nome, alimento_database.campione_kcal, alimento_database.unita_misura);
		// Soglia per la creazione della lista della spesa
		alimento_database.soglia_spesa = input_soglia_spesa(alimento_database.nome, alimento_database.unita_misura);

		fseek(stream, 0, SEEK_END);

		fwrite(&alimento_database, sizeof(alimento_database), 1, stream);

		fclose(stream);
		return 1;
	}else{
		fclose(stream);
		return 0;
	}
}




/**Funzione che si occupa di inserire nel file contente gli alimenti presenti nel frigo l'alimento(struct) che passiamo come parametro alla funzione.
 * L'aggiornamento é stato pensato come il salvataggio di un alimento per riga del file. Ogni alimento é diverso dagli altri per nome e data di scadenza.
 * Qualora venga caricato un alimento omonimo ad uno presente nel frigo viene fatto un controllo sulle scadenze. Se le scadenze sono uguali allora
 * nel complesso il prodotto é uguale quindi viene solo incrementata la quantitá di tale alimento presente nel frigo. Nel caso in cui siamo a che fare
 * con un alimento con nome diverso o con stesso nome ma con data di scadenza diversa, esso verrá salvato nella prima riga libera presente nel file.
 *
 * @pre  L'alimento passato come parametro deve essere significativo
 * @post Deve essere stato salvato correttamente l'alimento nel file
 */
int aggiorna_frigo(alimento_frigo alimento){
	FILE *stream = NULL;							// puntatore al file contenente gli alimenti del frigo
	alimento_frigo alimento_frigo;					// variabile per estrarre singolarmente gli alimenti del frigo
	fpos_t riga_libera = -1;						// puntatore alla posizione nel file della prima riga sovrascrivibile
	fpos_t riga_letta = -1;						// puntatore alla posizione nel file della riga letta
	char flag_posizionamento = 0;					// flag per determinare se il nuovo alimento da aggiungere è stato posizionato

	// tentativo di apertura del file frigo in lettura e scrittura
	if((stream = fopen(FILE_FRIGO, "rb+")) == NULL){
		// se il file non viene aperto, si decide di crearne uno nuovo
		if((stream = fopen(FILE_FRIGO, "wb+")) == NULL){
			// se non può essere creato il file in modalità aggiornamento, viene ritornato 0
			return 0;
		}
	}

	// Se vengono superati i precedenti controlli sul puntatore al file, vuol dire che è stato impostato correttamente
	// ed è possibile l'aggiornamento del frigo

	// Memorizzazione della posizione del puntatore ad inizio riga da leggere, così da effettuare facilemnte la scrittura se necessaria
	fgetpos(stream, &riga_letta);

	// Lettura delle righe del file e controlli sulla riga letta fino a quando non viene posizionato l'alimento
	while(flag_posizionamento == 0){

		fread(&alimento_frigo, sizeof(alimento_frigo), 1, stream);

		// Se si arriva alla fine del file, è necessario uscire dal ciclo
		if(feof(stream)){
			break;
		}
		// Se la riga letta è sovrascrivibile e il puntatore alla prima riga letta non è stato
		// impostato, viene salvata la posizione di tale riga
		if(strcmp(alimento_frigo.nome, "") == 0 && riga_libera == -1){
			riga_libera = riga_letta;
		}
		// Altrimenti se la riga letta presenta stesso nome e stessa scadenza dell'alimento da inserire
		// viene incrementata la quantita nella riga letta, riposizionandoci con fsetpos ad inizio riga
		else if(strcmp(alimento_frigo.nome, alimento.nome) == 0
				&& confronta_date(alimento_frigo.scadenza, alimento.scadenza) == 0){
			alimento_frigo.quantita += alimento.quantita;
			fsetpos(stream, &riga_letta);
			fwrite(&alimento_frigo, sizeof(alimento_frigo), 1, stream);
			flag_posizionamento = 1;
		}
		// Se il posizionamento non è avvenuto, viene memorizzata la posizione della prossima riga che verrà letta
		// per l'eventuale futura scrittura
		if(flag_posizionamento == 0){
			fgetpos(stream, &riga_letta);
		}
	}

	// Se non vengono individuati alimenti con stesso nome e scadenza dell'alimento da inserire
	// l'alimento è inserito nella prima riga letta o in coda al file
	if(flag_posizionamento == 0){
		if(riga_libera != -1){
			fsetpos(stream, &riga_libera);
			fwrite(&alimento, sizeof(alimento_frigo), 1, stream);
		}else{
			fwrite(&alimento, sizeof(alimento_frigo), 1, stream);
		}
		fclose(stream);
		return 2;
	}

	fclose(stream);

	// se l'aggiunta nel frigo avviene, può essere ritornato il valore 1
	return 1;
}



/**Funzione che si occupa di visualizzare tutti gli alimenti presenti nel database, con le relative informazioni .
 *
 * Qualora sia possibile aprire il file viene effettato un ciclo per la stampa di tutti gli alimenti presenti nel file. Al termine viene
 * restituito il numero di alimenti visualizzati in quanto tale dato potrebbe tornare utile in altre funzioni
 *
 * @pre  Nessuna pre condizione particolare
 * @post Nessuna post condizione particolare
 */
int visualizza_database_alimenti(){
	FILE *stream = NULL;					// Puntatore al FILE_DATABASE
	alimento_database alimento; 			// Variabile per contenere gli alimenti letti dal database
	int num_alimenti_database = 0;				// Variabile per contenere il  numero di alimenti presenti nel database

	if((stream = fopen(FILE_DATABASE_ALIMENTI, "rb")) == NULL){
		return -1;
	}else{
		printf("DATABASE ALIMENTI\n\n");
		printf("%4s | %-20s | %8s | %15s | %15s | %4s\n", "ID", "NOME ALIMENTO", "CAMPIONE", "QUANTITA SOGLIA", "UNITA DI MISURA", "KCAL");

		// Visualizzazione delle righe del database
		while(fread(&alimento, sizeof(alimento_database), 1, stream) == 1){
			if(feof(stream) != 0){
				break;
			}

			num_alimenti_database++;
			printf("%4d | %20s | %8d | %15.1f | %15s | %4hu\n",
					num_alimenti_database, alimento.nome, alimento.campione_kcal, alimento.soglia_spesa, alimento.unita_misura, alimento.kcal);
		}
	}
    return num_alimenti_database;
}


/**Funzione che si occupa di visualizzare tutti gli alimenti presenti nel frigo, con le relative informazioni .
 *
 * Qualora sia possibile aprire il file viene effettato un ciclo per la stampa di tutti gli alimenti presenti nel file. Al termine viene
 * restituito il numero di alimenti visualizzati in quanto tale dato potrebbe tornare utile in altre funzioni
 *
 * @pre  Nessuna pre condizione particolare
 * @post Nessuna post condizione particolare
 */
int visualizza_frigo(){
	FILE *stream = NULL;				// Puntatore a FILE_FRIGO
	unsigned short num_alimenti = 0;	// Variabile per memorizzare il numero degli alimenti visualizzato
	alimento_frigo alimento;			// Variabile per memorizzare l'alimento letto da FILE_FRIGO

	if((stream = fopen(FILE_FRIGO, "rb")) == NULL){
		return -1;
	}else{
		printf("CONTENUTO DEL FRIGO\n\n");
		printf("%4s | %-20s | %-14s | %16s\n", "ID", "NOME ALIMENTO", "QUANTITA", "DATA DI SCADENZA");

		while(fread(&alimento, sizeof(alimento_frigo), 1, stream) == 1){

			if(feof(stream) != 0){
				break;
			}

			if(strcmp(alimento.nome, "") != 0){
				num_alimenti++;
				printf("_________________________________________________________________\n");

				printf("\n%4d | %20s | %8.1f %-5s |      %hu/%hu/%hu\n",
						num_alimenti, alimento.nome, alimento.quantita, alimento.unita_misura, alimento.scadenza.giorno, alimento.scadenza.mese, alimento.scadenza.anno);

			}
		}

		if(num_alimenti == 0){
			puts("Non ci sono alimenti nel frigo.");
		}
	}

    return num_alimenti;///se la funzione e' andata a buon fine restituisci 1
}




/**Funzione che serve a popolare un'array di struct( passato per riferimento ) con gli alimenti presenti nel frigo e le relative informazioni.
 *
 * Qualora sia possibile aprire il file contenente gli alimenti del frigo viene effettauto un ciclo, all'interno del quale, l'alimento estratto
 * viene memorizzato in una opportuna posizione dell'array di struct. Al termine della estrazione viene restituito il numero di alimenti letti
 * in quanto puó tonrare utile per altre funzioni
 *
 * @pre  L'array di struct passato abbia una lunghezza pari o superiore al numero di elementi presenti nel frigo
 * @post Deve essere stati memorizzati tutti gli alimenti del file all'interno della struct
 */
int leggi_frigo(alimento_frigo* lista_frigo){
	int num_alimenti_letti = 0;
	alimento_frigo alimento_letto; // Variabile utilizzata per leggere gli elementi di FILE_FRIGO
	FILE* stream = NULL; // Puntatore a FILE_FRIGO

	// Apertura del file in modalità lettura e aggiornamento binario
	if((stream = fopen(FILE_FRIGO, "rb+")) == NULL){
		return num_alimenti_letti; // Viene ritornato 0 se il file non può essere aperto
	}else{
		// Attraversa il contenuto del frigo estraendo gli alimenti in esso presenti
		while(fread(&alimento_letto, sizeof(alimento_frigo), 1, stream) > 0){
			// Se l'elemento estratto ha nome diverso da stringavuota, è un alimento significativo
			if(strcmp(alimento_letto.nome, "") != 0){

				lista_frigo[num_alimenti_letti] = alimento_letto;

				num_alimenti_letti++;

			}
		}
	}
    return num_alimenti_letti;//se la funzione e' andata a buon fine restituisci il numero degli alimenti
}




/**Funzione che si occupa di estrarre dal file, che contiene la spesa effettuata, gli alimenti comprati con le info di base che lo riguardano come
 * nome,quantitá,unitá di misura,scadenza. In questo caso queste informazioni sono contenute all'interno di un file testuale che deve essere scansionato
 * riga per riga. La riga di un file é da considerarsi tale fino al carattere terminatore e ad ogni estrazione occorre pulire lo stream in caso
 * la riga presente nel file sia superiore a quella consentita. Inoltre per ogni riga é necessario l'intervento della sscanf per "tokenizzare" i vari
 * valori della riga e memorizzarli opportunamente all'interno della struct. Qualora la sscanf abbia prodotto un numero di hit inferiore a quelli
 * necessari per considerare l'alimento correttamente estratto, viene chiesto se si vuole correggere l'alimento oppure no. In caso affermativo viene
 * avviata la fase di correzione che si occupa di correggere un dato alimento richiamando opportunatamente altre funzioni per l'inserimento fino a
 * quando l'input non sia significativo.
 *
 * Al termine della fase di estrazione e correzione eventuale, viene richiamata la funzione che si occupa di aggiornare il database per far si che
 * vengano memorizzate le informazioni riguardanti alimenti sconosciuti al frigo. Una volta terminata la fase di aggiornamento del database viene
 * effettuato un check sulla validitá degli altri campi dell'alimento e in caso viene chiesto di effettuare una correzione su tali valori. Un esempio
 * é il caso di un alimento scaduto che viene erroneamente caricato nel frigo. A tal proposito viene chiesto all'utente se vuole correggere tale data
 * e proseguire con il caricamento. Una volta che la lista della spesa é stata completamente estratta viene avviata la fase di aggiornamento del frigo
 * con il richiamo alla opportuna funzione.
 *
 * @pre  Nessuna pre condizione particolare
 * @post Devono essere memorizzati in una struct gli alimenti con i relativi valori significativi.
 */
int carica_spesa(){
	FILE *stream = NULL;						// Puntatore al file contenente la spesa
	char s[LUNG_RIGA_FGETS];					// Stringa che ospita ogni riga del file per la lettura degli alimenti
	int num_alimenti_letti = 0;					// Numero di alimento letti dal file
	int num_alimenti_scartati = 0;				// Numero di alimenti scartati dal caricamento
	alimento_frigo alimento_letto;				// Variabile contente i dati di ogni alimento letto dal file
	int esito_lettura;							// Variabile per contenere l'esito delle letture degli alimenti
	int esito_input;							// Variabile per memorizzare l'esito degli inserimenti dell'utente
	int flag_inserimento;						// Flag per memorizzare la corretteza dell'alimento letto

	// tentativo di apertura della spesa in lettura
	if((stream = fopen(FILE_SPESA, "r")) == NULL){
		puts("Non esiste il file 'spesa_effettuata.csv' .");
		return -1; // se il file non può essere aperto viene ritornato il valore -1
	}else{
		// estrazione di ogni riga del file (riga == 1 alimento)
		while(fgets(s, LUNG_RIGA_FGETS, stream) != NULL){
			if(s[strlen(s) - 1] != '\n'){
				pulisci_riga_flusso(stream);
			}
			// modifica dei valori di flagInserimento e num_alimenti_caricati supponendo che l'alimento letto verrà inserito
			flag_inserimento = 1;
			num_alimenti_letti++;

			// estrazione dei campi della riga letta dal file
			esito_lettura = sscanf(s, "%20[a-zA-Z] %f %5[a-zA-Z] %hu/%hu/%hu",
					alimento_letto.nome, &alimento_letto.quantita, alimento_letto.unita_misura,
					&alimento_letto.scadenza.giorno, &alimento_letto.scadenza.mese, &alimento_letto.scadenza.anno);

			// Controlli sull'esito dell'estrazione, per verificare errori nel formato della riga:
			// se sono presenti errori di questo tipo viene richiesto di scegliere se correggere l'alimento manualmente
			// o saltarlo.
			if(esito_lettura != NUM_CAMPI_ALIMENTO_FRIGO){
				printf("Errore nella lettura dell'alimento numero %d.\n", num_alimenti_letti);
				// Richiesta di correzione manuale dell'alimento letto
				do{
					puts("Inserire 1 per correggerlo manualmente, 0 per saltarlo: ");
					esito_input = scanf("%d", &flag_inserimento);
					if(pulisci_stdin() == 1){
						esito_input = 0;
					}

					if((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1){
						puts("Inserimento non valido. Ripeterlo.\n");
					}
				}while((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1);


				// Correzione dell'alimento, se scelto dall'utente
				if(flag_inserimento == 1){

					puts("Correzione manuale dell'alimento.\n");
					alimento_letto = input_alimento_frigo();

				}else{
					// Se si sceglie di saltare l'alimento, deve essere decrementato il numero di alimenti caricati
					num_alimenti_scartati++;
				}
			}


			// se l'alimento deve essere aggiunto, è necessario rendere minuscole tutte le lettere del nome
			abbassa_maiuscole(alimento_letto.nome);

			// Controlli sul significato dei campi dell'alimento estratto:
			// Unità di misura
			abbassa_maiuscole(alimento_letto.unita_misura);

			// Controllo sul significato dell'unità di misura inserita
			if(flag_inserimento == 1 && controlla_unita_misura(alimento_letto.unita_misura) == 0){
				printf("Unità di misura dell'alimento <%s> non valida.\n", alimento_letto.nome);

				// Richiesta all'utente della correzione manuale
				do{
					printf("Inserire 1 per correggere l'unità di misura, 0 per ignorare l'alimento: ");
					esito_input = scanf("%d", &flag_inserimento);
					if(pulisci_stdin() == 1){
						esito_input = 0;
					}

					if((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1){
						puts("Inserimento non valido.\n");
					}
				}while((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1);

				// Correzione dell'unità di misura se deciso dall'utente
				if(flag_inserimento == 1){

					puts("Correzione manuale dell'unità di misura.");
					input_unita_misura(alimento_letto.unita_misura);

				}else{
					// Se si sceglie di saltare l'alimento, deve essere decrementato il numero di alimenti caricati
					num_alimenti_scartati++;
				}
			}


			// Verifica della congruenza fra l'unità di misura inserita per l'alimento
			// e quella, se l'alimento è già conosciuto, presente nel database
			if(flag_inserimento == 1){
				int esito_ricerca;
				alimento_database alimento_database;

				// Conversione dell'unità di misura inserita in quella utilizzata nel frigo
				if(converti_quantita(&alimento_letto.quantita, alimento_letto.unita_misura) == 1){
					printf("Quantita dell'alimento <%s> convertita.\n", alimento_letto.nome);
				}

				// Ricerca nel  database dell'alimento letto
				esito_ricerca = ricerca_alimento_database(alimento_letto.nome, &alimento_database);
				if(esito_ricerca == 1){
					// Confronto fra le due unità di misura
					if(strcmp(alimento_letto.unita_misura, alimento_database.unita_misura) != 0){
						printf("L'unità di misura dell'alimento <%s> non corrisponde a quella conosciuta nel database [%s].\n",
								alimento_letto.nome, alimento_database.unita_misura);

						// Richiesta all'utente della correzione manuale
						do{
							printf("Inserire 1 per correggere l'unità di misura con quella conosciuta, 0 per ignorare l'alimento: ");
							esito_input = scanf("%d", &flag_inserimento);
							if(pulisci_stdin() == 1){
								esito_input = 0;
							}

							if((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1){
								puts("Inserimento non valido.\n");
							}
						}while((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1);

						if(flag_inserimento == 1){
							// Copia dell'unità di misura corretta e azzeramento della quantità
							// così da provocare la correzione nel controllo successivo
							strcpy(alimento_letto.unita_misura, alimento_database.unita_misura);
							alimento_letto.quantita = 0;
						}else{
							num_alimenti_scartati++;
						}
					}
				}
			}



			// Quantità
			if(flag_inserimento == 1){

				if(controlla_quantita(alimento_letto.quantita, alimento_letto.unita_misura) == 0){
					printf("Quantita dell'alimento <%s> non valida.\n", alimento_letto.nome);

					// Richiesta all'utente della correzione manuale
					do{
						printf("Inserire 1 per correggere la quantità, 0 per ignorare l'alimento: ");
						esito_input = scanf("%d", &flag_inserimento);
						if(pulisci_stdin() == 1){
							esito_input = 0;
						}

						if((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1){
							puts("Inserimento non valido.\n");
						}
					}while((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1);

					// Correzione della quantità se deciso dall'utente
					if(flag_inserimento == 1){

						puts("Correzione manuale della quantità.\n");
						//esci controllo quantitá che sta nella funzione qua sotto
						alimento_letto.quantita = input_quantita(alimento_letto.unita_misura);

					}else{
						// Se si sceglie di saltare l'alimento, deve essere decrementato il numero di alimenti caricati
						num_alimenti_scartati++;
					}
				}
			}

			// Data di scadenza
			if(flag_inserimento == 1){
				if(controlla_data_scadenza(alimento_letto.scadenza) == -1){
					printf("Data di scadenza per l'alimento <%s> non valida.\n", alimento_letto.nome);
					flag_inserimento = 0;
				}else if(controlla_data_scadenza(alimento_letto.scadenza) == 0){
					printf("L'alimento <%s> è scaduto.\n", alimento_letto.nome);
					flag_inserimento = 0;
				}

				// Richiesta all'utente ella correzione manuale
				if(flag_inserimento == 0){
					do{
						printf("Inserire 1 per correggere la data di scadenza, 0 per ignorare l'alimento: ");
						esito_input = scanf("%d", &flag_inserimento);
						if(pulisci_stdin() == 1){
							esito_input = 0;
						}

						if((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1){
							puts("Inserimento non valido.\n");
						}
					}while((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1);

					// Correzione della data di scadenza se deciso dall'utente
					if(flag_inserimento == 1){

						puts("Correzione manuale della data di scadenza.");
						alimento_letto.scadenza = input_data_scadenza();

					}else{
						// Se si sceglie di saltare l'alimento, deve essere decrementato il numero di alimenti caricati
						num_alimenti_scartati++;
					}
				}
			}

			// In questo punto del codice, se flag_inserimento è uguale a 1, alimento_letto può essere aggiunto al frigo
			if(flag_inserimento == 1){
				int esito_aggiornamento;

				// aggiunta dell'alimento e propagazione dell'eventuale errore ricevuto
				esito_aggiornamento = aggiorna_frigo(alimento_letto);

				if(esito_aggiornamento == 2){
					esito_aggiornamento = aggiorna_database(alimento_letto.nome, alimento_letto.unita_misura);
					if(esito_aggiornamento == -1){
					 return -3;
					}
				}else if(esito_aggiornamento == -1){
					return -2;
				}

			}

		}
	}

	fclose(stream);

	return num_alimenti_letti - num_alimenti_scartati;
}





/**Funzione che si occupa di estrarre la soglia di un alimento presente nel database. Qualora sia possibile accedere al file viene fatto
 * controllo su tutto il database fino a quando non si trova l'alimento con il nome che abbiamo passato come parametro.
 * Una volta trovato viene restituita tale soglia.
 *
 * @pre  Che il nome passato come parametro sia di almeno un carattere
 * @post nessuna post condizione particolare
 */
float soglia_alimento(const char* nome_alimento){
	FILE* stream = NULL;
	float soglia_alimento = -1;
	alimento_database alimento_letto;

	//se non é possibile accedere al file viene restituito -1 altrimenti viene scansionato il file del database
	if((stream = fopen(FILE_DATABASE_ALIMENTI, "rb")) == NULL){
		return soglia_alimento;
	}else{
		while(fread(&alimento_letto, sizeof(alimento), 1, stream) > 0){

			if(feof(stream)!=0){
				break;
			}
			//se l'alimento viene trovato viene memorizzata tale soglia per poi essere restituita
			if(strcmp(alimento_letto.nome, nome_alimento) == 0){
				soglia_alimento = alimento_letto.soglia_spesa;
				break;
			}
		}

		fclose(stream);
		return soglia_alimento;
	}
}





/**Funzione che si occupa di estrarre la quantitá di un alimento presente nel frigo. Qualora sia possibile accedere al file viene fatto
 * controllo su tutto il file fino a quando non si trova l'alimento con il nome che abbiamo passato come parametro.
 * Una volta trovato viene restituita tale quantitá.
 *
 * @pre  Che il nome passato come parametro sia di almeno un carattere
 * @post nessuna post condizione particolare
 */
float quantita_alimento(const char* nome_alimento){
	FILE* stream = NULL;
	float quantita_alimento = 0;
	alimento_frigo alimento_letto;

	//se non é possibile accedere al file viene restituito 0 altrimenti viene scansionato il file del frigo
	if((stream = fopen(FILE_FRIGO, "rb")) == NULL){
		return quantita_alimento;
	}else{
		while(fread(&alimento_letto, sizeof(alimento), 1, stream) > 0){

			if(feof(stream)!=0){
				break;
			}

			//se l'alimento viene trovato viene memorizzata tale quantitá
			if(strcmp(alimento_letto.nome, nome_alimento) == 0){
				quantita_alimento += alimento_letto.quantita;
			}
		}

		fclose(stream);
		return quantita_alimento;
	}
}


/**
 * Funzione che dopo aver allocato una stringa di una determinata lunghezza, si occupa di ricevere in input il nome dell'alimento.
 *  L'inserimento viene effettuato fino a quando non é stata inserita una stringa significativa e pertanto fino a quando l'esito dell'input
 *  non risulta positivo
 *
 * @pre		Nessuna pre condizione particolare
 * @post	Deve essere restituita una stringa con almeno un carattere
 */
char* input_nome_alimento() {
	int esito_input;			// Variabile per memorizzare l'esito dell'input
	// Stringa da restituire contenente il nome dell'alimento
	char* nome_alimento = (char*) calloc(LUNG_NOME_ALIMENTO, sizeof(char));

	// Ciclo per la ripetizione dell'inserimento fino a riceverlo significativo
	do {
		printf("Inserisci il nome dell'alimento:\n>");
		scanf("%20[^\n]", nome_alimento);
		if (pulisci_stdin() == 1) {//viene pulito lo stream di input qualora sia stato inserito un valore non corretto per la scanf
			esito_input = 0;
		}

		if (esito_input == 0) {
			puts("Inserimento non valido. Ripeterlo.");
		}
	} while (esito_input == 0);

	return nome_alimento;

}







/**Funzione che si occupa di ridurre la quantità di un certo alimento presente nel frigo.
 *
 * Il nome dell'alimento é specificato come parametro di ingresso assieme alla quantitá da ridurre. La funzione pertanto controlla
 * in primo luogo se la quantitá presente nel frigo é maggiore o uguale di 0. Qualora la quantitá da ridurre sia superiore a quella
 * effettivamente disponibile nel frigo o se la quantitá da ridurre é uguale a 0 viene restituito 0 per segnalare che non é stata effettuata la
 * riduzione. Nel caso in cui la quantitá da ridurre e a quantitá disponibile siano significative viene aperto il file del frigo e viene effettuata
 * la riduzione della quantitá. Inoltre viene richiamata la funzione che si occupa di memorizzare i consumi per registrare l'alimento ridotto.
 *
 * @pre  Sia passato come nome di alimento una stringa con almeno un carattere
 * @post Deve essere restituito un valore per segnalare un errore o in alternativa deve essere stata effettuata con successo la riduzione
 */
int riduci_alimento(const char* nome_alimento, float riduzione){
	FILE* stream;
	alimento_frigo alimento_letto;

	//viene memorizzata la quantitá dell'alimento se presente nel frigo
	float q_alimento_riduzione = quantita_alimento(nome_alimento);

	//se la quantitá non é accettabile viene restituito un valore per segnalare un errore nell'operazione di riduzione
	//in caso contrario viene aperto il file del frigo
	if(q_alimento_riduzione == 0){
		return -1;
	}else if(q_alimento_riduzione < riduzione || riduzione == 0){
		return 0;
	}else{
		if((stream = fopen(FILE_FRIGO, "rb+")) == NULL){
			return -1;
		}else{
			while(riduzione > 0){

				fread(&alimento_letto, sizeof(alimento_frigo), 1, stream);

				if(feof(stream) != 0){
					break;
				}
				//se l'alimento letto é quello da ridurre applica la riduzione della quantitá
				if(strcmp(alimento_letto.nome, nome_alimento) == 0){
					alimento_letto.quantita -= riduzione;

					//qualora la quantitá sia scesa al di sotto di zero significa che occorre continuare l'operazione di riduzione, applicandola
					//ad uno stesso alimento ma che scade in un giorno differente (es ho bisogno di 6 uova, ma ne ho 4 che scadono in data xx/xx/xxxx
					//e 3 che scadono in data yy/yy/yyyy. A fine operazione ne rimarrá 1 nel frigo che scade in data yy/yy/yyyyy). In caso contrario
					//imponiamo la riduzione a 0 per uscire dal ciclo
					if(alimento_letto.quantita <= 0){
						//rendiamo la quantitá dell'alimento positiva per ottenere il valore della successiva riduzione da effettuare
						riduzione = abs(alimento_letto.quantita);

						//inizializziamo il nome per recuperare la riga all'interno del file
						strcpy(alimento_letto.nome, "");
					}else{
						riduzione = 0;
					}

					//aggiorniamo il file del frigo
					fseek(stream, -sizeof(alimento_frigo), SEEK_CUR);
					fwrite(&alimento_letto, sizeof(alimento_frigo), 1, stream);
				}
			}

			registra_consumo(alimento_letto.nome, FLAG_ALIMENTO);

			fclose(stream);
			return 1;
		}
	}
}





/**Funzione che si occupa di gestire l'operazione di riduzione di un alimento permettendo all'utente di registrare tale consumo.
 *
 *
 * Una volta effettuata la scelta da parte dell'utente se registrare il consumo ed effettuato un check su tale scelta, viene mostrato il contenuto del frigo
 * per poter permettere all'utente di scegliere l'alimento da consumare. Qualora la riduzione(funzione riduci alimento) abbia avuto sucesso viene
 * eventualmente registrato tale consumo e viene chiesto se si vuole rieffettuare l'inserimento.
 *
 * @pre  Nessuna pre condizione particolare
 * @post Deve essere stata effettuata con successo la riduzione ed eventualmente registrato il consumo.
 */
int gestore_riduzione_alimenti(){
	int esito_input;
	int esito_controllo = 0;
	int esito_riduzione;
	int scelta = 0;
	char nome_alimento[LUNG_NOME_ALIMENTO];
	float q_riduzione;
	utente persona;

	if(conta_alimenti_frigo()==0){
		printf("Non puoi effettuare l'operazione di riduzione in quanto non sono presenti alimenti nel frigo\n");
		return 0;
	}

	puts("Riduzione di un alimento.\n");

	//viene chiesto se si vuole registrare il consumo al termine della riduzione. La scelta viene controllata tramite il classico controllo sull'input
	do {
		printf("Inserire:\n"
				"[1] per autenticarsi e inserire le calorie nella propria giornata\n"
				"[0] per ridurre alimento in anonimo\n~");
		esito_input = scanf("%d", &scelta);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(scelta != 0 && scelta != 1){
			esito_controllo = 0;
		}else{
			esito_controllo = 1;
		}

		if(esito_input == 0 || esito_controllo == 0){
			puts("Inserimento non valido. Ripeterlo.");
		}
	} while (esito_input == 0 || esito_controllo == 0);

	do {
		//viene visualizzato il contenuto del frigo e viene chiesto di inserire l'alimento da ridurre tramite la funzione input nome
		visualizza_frigo();

		strcpy(nome_alimento,input_nome_alimento());

		//viene chiesto di inserire una quantitá significativa da ridurre
		do{
			printf("\nInserire la quantita della riduzione dell'alimento:\n>");
			esito_input = scanf("%f", &q_riduzione);
			if(pulisci_stdin() == 1){
				esito_input = 0;
			}

			if(esito_input != 1){
				puts("Inserimento non valido. Ripeterlo.");
			}

		}while(esito_input != 1);

		//viene effettuata la riduzione e viene controllato l'esito per poter notificare un feedback all'utente
		esito_riduzione = riduci_alimento(nome_alimento, q_riduzione);

		if(esito_riduzione == -1){
			puts("Alimento non presente nel frigo.");
		}else if(esito_riduzione == 0){
			puts("Quantità della riduzione non valida.");
		}else{
			puts("Riduzione effettuata con successo.");

			//qualora l'utente abbia inizialmente inserito 1 viene registrato il consumo dopo essersi autenticati
			if(scelta==1){

				if (autenticazione(&persona)==1){
					aggiorno_database_calorie(nome_alimento,FLAG_ALIMENTO,q_riduzione,persona.nickname);
				}
			}
		}


		//viene chiesto se si vuole rieffettuare l'inserimento e viene fatto un controllo su tale scelta
		do {
			printf("\nInserire [1] per ripetere la riduzione, [0] per annullare:\n>");
			esito_input = scanf("%d", &scelta);
			if(pulisci_stdin() == 1){
				esito_input = 0;
			}

			if(scelta != 1 && scelta != 0){
				esito_controllo = 0;
			}else{
				esito_controllo = 1;
			}

			if(esito_input != 1 || esito_controllo != 1){
				puts("Inserimento non valido. Ripeterlo");
			}
		} while (esito_input != 1 || esito_controllo != 1);
	} while (scelta != 0);

	return 1;

}



/**Funzione che si occupa di permettere la modifica della soglia della spesa.
 *
 * Il funzionamento si basa su una iniziale visualizzazione del database al fine da poter permettere all'utente di capire quale id corrisponde all'alimento
 * che vuole modificare. Una volta effettuata tale operazione(apertura del file permettendo), viene effettuato un ciclo per il posizionamento del puntatore
 * all'interno del file qualora venga inserito un id significativo e pertanto venga individuato tale alimento. Una volta fatto ció viene chiesto all'utente
 * di inserire il nuovo valore che verrá poi inserito all'interno del file per poter permettere cosí l'aggiornamento della soglia.
 *
 * @pre  Nessuna pre condizione particolare
 * @post Deve essere stata aggiornata con successo la soglia della spesa qualora sia possibile modificare tale informazione
 */
int modifica_soglia_spesa(){
	FILE* stream = NULL;

	alimento_database alimento;
	unsigned short flag_alimento = 0;//flag che ci aiuta a capire se l'alimento é stato modificato
	unsigned short id_alimento;		//informazione che ci aiuta a scorrere i vari alimenti presenti nel database

	int num_alimenti_database;
	unsigned short num_alimenti_letti = 0;	//informazione che ci aiuta a scorrere solo le righe del file effettivamente piene

	int soglia_spesa;

	if((stream = fopen(FILE_DATABASE_ALIMENTI, "rb+")) == NULL){
		puts("Non è possibile aprire 'database_alimenti.dat'.");
		return 0;
	}else{
		num_alimenti_database = visualizza_database_alimenti(); //vengono visualizzati gli alimenti e viene assegnato il numero di tali alimenti

		//viene chiesto all'utente di inserire l'id dell'alimento da modificare qualora sia presente almeno un alimento
		if(num_alimenti_database > 0){
			id_alimento = input_id_alimento(num_alimenti_database);

			while(flag_alimento == 0){
				if(feof(stream) != 0){
					break;
				}

				// Lettura di un alimento dal database
				fread(&alimento, sizeof(alimento_database), 1, stream);

				if(strcmp(alimento.nome, "") != 0){
					num_alimenti_letti++;

					//se siamo arrivati all'alimento corrispondente all'id inserito dall'utente viene posizionato il puntatore
					//all'iterno del file e viene aggiornato il flag
					if(num_alimenti_letti == id_alimento){
						fseek(stream, -sizeof(alimento_frigo), SEEK_CUR);
						flag_alimento++;
					}
				}
			}


		}else{
			fclose(stream);
			return num_alimenti_database;
		}

		// Input nuova soglia per la generazione della spesa e aggiornamento di tale valore
		soglia_spesa = input_quantita(alimento.unita_misura);

		alimento.soglia_spesa = soglia_spesa;

		fwrite(&alimento, sizeof(alimento_database), 1, stream);

		fclose(stream);
		return 1;
	}
}





/**Funzione che si occupa di permettere la modifica della kcal di un alimento.
 *
 * Il funzionamento si basa su una iniziale visualizzazione del database al fine da poter permettere all'utente di capire quale id corrisponde all'alimento
 * che vuole modificare. Una volta effettuata tale operazione(apertura del file permettendo), viene effettuato un ciclo per il posizionamento del puntatore
 * all'interno del file, qualora venga inserito un id significativo e pertanto venga individuato tale alimento. Una volta fatto ció viene chiesto all'utente
 * di inserire il nuovo valore che verrá poi inserito all'interno del file per poter permettere cosí l'aggiornamento delle kcal.
 *
 * @pre  Nessuna pre condizione particolare
 * @post Deve essere stato aggiornato con successo il valore delle kcal qualora sia possibile modificare tale informazione
 */
int modifica_kcal(){
	FILE* stream = NULL;

	alimento_database alimento;
	unsigned short flag_alimento = 0; //flag che ci aiuta a capire se l'alimento é stato modificato
	unsigned short id_alimento; //informazione che ci aiuta a scorrere i vari alimenti presenti nel database

	int num_alimenti_database;
	unsigned short num_alimenti_letti = 0;//informazione che ci aiuta a scorrere solo le righe del file effettivamente piene

	int kcal;

	if((stream = fopen(FILE_DATABASE_ALIMENTI, "rb+")) == NULL){
		puts("Non è possibile aprire 'database_alimenti.dat'.");
		return 0;
	}else{
		num_alimenti_database = visualizza_database_alimenti();//vengono visualizzati gli alimenti e viene assegnato il numero di tali alimenti

		//viene chiesto all'utente di inserire l'id dell'alimento da modificare qualora sia presente almeno un alimento
		if(num_alimenti_database > 0){
			id_alimento = input_id_alimento(num_alimenti_database);

			while(flag_alimento == 0){
				if(feof(stream) != 0){
					break;
				}

				// Lettura di un alimento dal database
				fread(&alimento, sizeof(alimento_database), 1, stream);

				if(strcmp(alimento.nome, "") != 0){
					num_alimenti_letti++;

					//se siamo arrivati all'alimento corrispondente all'id inserito dall'utente viene posizionato il puntatore
					//all'iterno del file e viene aggiornato il flag
					if(num_alimenti_letti == id_alimento){
						fseek(stream, -sizeof(alimento_frigo), SEEK_CUR);
						flag_alimento++;
					}
				}
			}


		}else{
			fclose(stream);
			return num_alimenti_database;
		}

		// Input nuov e kcal dell'alimento
		kcal = input_kcal(alimento.nome, alimento.campione_kcal, alimento.unita_misura);

		alimento.kcal = kcal;

		fwrite(&alimento, sizeof(alimento_database), 1, stream);

		fclose(stream);
		return 1;
	}
}




/**Funzione che si occupa di estrarre dal file del database il nome di alimento in maniera casuale.
 *
 *
 * Una volta aperto correttamente il file del database viene posizionato in maniera del tutto casuale il puntatore del file in base alla dimensione
 * di ogni riga. In questa maniera é possibile accedere ad uno specifico alimento e popolare la struct alimento_database, per poi estrarre l'unica
 * informazione a noi interessata quale il nome. Esso viene copiato nella stringa passata per riferimento e viene restituito 1
 *
 * @pre  Nessuna pre condizione particolare
 * @post Deve essere termianta con successo la visualizzazione
 */
int alimento_casuale(char* nome_alimento){
	FILE* stream = NULL;			// Puntatore a FILE_DATABASE
	alimento_database alimento;		// Alimento estratto random
	int dim_database;				// Dimensione di FILE_DATBASE

	// Tentativo di apertura di FILE_BINARIO in lettura binaria
	if((stream = fopen(FILE_DATABASE_ALIMENTI, "rb")) == NULL){
		return -1;	// Se non può essere aperto, ritorna -1
	}else{
		// Se il file può essere aperto ma è vuoto, ritorna 0
		if(feof(stream) != 0){
			return 0;
		}

		// Altrimenti calcola la dimensione del file e leggi un alimento random
		fseek(stream, 0, SEEK_END);
		dim_database = conta_alimenti_database();
		rewind(stream);

		// Spostamento del puntatore del file in una posizione random
		fseek(stream, (rand() % dim_database) * sizeof(alimento_database), SEEK_SET);

		fread(&alimento, sizeof(alimento_database), 1, stream);

		strcpy(nome_alimento, alimento.nome);

		// Se l'estrazione avviene correttamente, ritorna 1
		return 1;
	}
}

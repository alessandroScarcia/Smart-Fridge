
#include "ricette.h"




int esiste_ricetta(char* nome_ricetta){
	FILE* stream = NULL;
	ricetta ricetta_letta;
	char flag_esistenza = 0;

	if((stream = fopen(FILE_DATABASE_RICETTE, "rb+")) == NULL){
		return flag_esistenza;
	}else{
		while(fread(&ricetta_letta, sizeof(ricetta), 1, stream) > 0){
			if(feof(stream) != 0){
				break;
			}

			if(strcmp(ricetta_letta.nome_ricetta, nome_ricetta) == 0){
				flag_esistenza = 1;
				break;
			}
		}

		fclose(stream);
		return flag_esistenza;
	}
}


/**
 * Funzione che si occupa di estrarre gli ingredienti di una determinata ricetta.
 *
 * Una volta passato il nome della ricetta e l'array di struct di tipo ingrediente che deve essere popolato viene aperto in lettura il file
 * del database alimenti e viene fatto un check sul nome passato. Qualora venga trovata la ricetta passata si memorizzano gli ingredienti
 * di tale ricetta nella struct e si restituisce il numero di tali ingredienti. In caso non venga trovata la ricetta viene restituito -1
 *
 * @pre		Il nome del file della ricetta non deve essere vuoto e l'array di struct deve avere dimensione maggiore degli ingredienti della ricetta
 * @post	Deve essere restituita, in caso venga trovata la ricetta, il numero corretto di ingredienti estratti
 */
int estrazione_ingredienti(char nome_ricetta[LUNG_NOME_RICETTA], ingrediente* ingredienti) {
	FILE* stream_database;
	int flag_presente = 0;//flag che ci permette di segnalare se una ricetta e' presente oppure no
	int indice_ingrediente = 0; //indice che ci aiuta a popolare l''array di struct

	if ((stream_database = fopen(FILE_DATABASE_RICETTE, "rb+")) == NULL)
		return -1;

	ricetta analisi_ricetta;

	flag_presente = esiste_ricetta(nome_ricetta);

	if (flag_presente == 0)
		return -1;

	while (fread(&analisi_ricetta, sizeof(ricetta), 1, stream_database) > 0) {

		if (strcmp(nome_ricetta, analisi_ricetta.nome_ricetta) == 0) {

			do {

				//il controllo sul nome ci aiuta a capire quando stiamo controllando l'ingrediente successivo all'ultimo che e' una stringa vuota
				if (strcmp(analisi_ricetta.ingredienti[indice_ingrediente].nome , "") != 0) { //se l'ingrediente e' significativo allora...
					strcpy(ingredienti[indice_ingrediente].nome , analisi_ricetta.ingredienti[indice_ingrediente].nome);
					indice_ingrediente++;
				} else { //...altrimenti esci dal ciclo
					break;
				}

			} while (indice_ingrediente < MAX_INGREDIENTI); //ripeti fino al numero massimo di ingredienti concessi per ricetta

		}

	}

	fclose(stream_database);

	return indice_ingrediente;
}


int conta_ricette_preparabili(){
	char flag_preparazione;								// Flag per memorizzare se una ricetta è preparabile
	int num_ricette_preparabili = 0;					// Numero di ricette preparabili
	int num_ricette_database;							// Numero di ricette presenti nel database
	ricetta* ricette_database;							// Array che conterrà le ricette del database

	// Vengono contate le ricette conosciute
	num_ricette_database = conta_ricette_database();
	// Se le ricette conosciute sono pari a zero, non ci sono ricette preparabili
	if(num_ricette_database == 0){
		return num_ricette_preparabili;
	}

	// Estrazione delle ricette conosciute
	ricette_database = (ricetta*) calloc(num_ricette_database, sizeof(ricetta));
	lettura_database_ricette(ricette_database);

	// Viengono analizzate tutte le ricette conosciute
	for(int i=0; i < num_ricette_database; i++){
		// Viene impostato il flag ad 1 prima di analizzare tutti gli ingredienti
		flag_preparazione = 1;

		// Le quantità di ogni ingrediente della i-esima ricetta vengono confrontate con il contenuto del frigo
		int j = 0;
		while(strcmp(ricette_database[i].ingredienti[j].nome, "") != 0){
			// Viene estratta dal frigo la quantità posseduta del j-esimo ingrediente
			float quantita_posseduta = quantita_alimento(ricette_database[i].ingredienti[j].nome);

			if(quantita_posseduta < ricette_database[i].ingredienti[j].quantita){
				flag_preparazione = 0;
				break;
			}
		}

		if(flag_preparazione == 1){
			num_ricette_preparabili++;
		}
	}

	return num_ricette_preparabili;
}







int prepara_ricetta(char* nome_ricetta, int dosi_ricetta){
	FILE* stream = NULL;
	ricetta ricetta_letta;
	int flag_preparazione;

	if(esiste_ricetta(nome_ricetta) == 0){
		return 0;
	}

	if((stream = fopen(FILE_DATABASE_RICETTE, "rb")) == NULL){
		puts("Non ancora sono state memorizzate ricette.");
		return 0;
	}else{
		while(fread(&ricetta_letta, sizeof(ricetta), 1, stream) > 0){
			if(strcmp(ricetta_letta.nome_ricetta, nome_ricetta) == 0){
				break;
			}
		}

		flag_preparazione = 1;

		for(int i = 0; i < MAX_INGREDIENTI; i++){
			if(strcmp(ricetta_letta.ingredienti[i].nome, "") != 0){
				if(ricetta_letta.ingredienti[i].quantita > quantita_alimento(ricetta_letta.ingredienti[i].nome)){
					flag_preparazione = 0;
				}
			}else{
				break;
			}
		}

		if(flag_preparazione == 1){
			for(int i = 0; i < MAX_INGREDIENTI; i++){
				if(strcmp(ricetta_letta.ingredienti[i].nome, "") != 0){
					riduci_alimento(ricetta_letta.ingredienti[i].nome, ricetta_letta.ingredienti[i].quantita);
				}else{
					break;
				}
			}

			registra_consumo(nome_ricetta, FLAG_RICETTA);
			fclose(stream);
			return 1;
		}else{
			return 0;
		}
	}
}



void gestore_prepara_ricetta(){
	int esito_input;
	int esito_controllo;
	int scelta;
	int esito_autenticazione;
	int esito_preparazione;
	utente utente_autenticato;
	char nome_ricetta[LUNG_NOME_RICETTA];
	int dosi_ricetta;

	puts("PREPARAZIONE RICETTA:");

	do {
		printf("Inserire:\n"
				"[1] per autenticarsi e inserire le calorie nella propria giornata\n"
				"[0] per preparare la ricetta in anonimo\n>");
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

	if(scelta == 1){
		esito_autenticazione = autenticazione(&utente_autenticato);
		if(esito_autenticazione == 0){
			return;
		}
	}

	do {
		esito_preparazione=suggerimento_ricetta_automatico();

		if(esito_preparazione==-1){
			break;
		}

		strcpy(nome_ricetta, input_nome_ricetta());

		dosi_ricetta = input_dosi_ricetta();

		esito_preparazione=prepara_ricetta(nome_ricetta, dosi_ricetta);
		if(esito_preparazione == 0){
			esito_preparazione = 0;
			puts("Non è possibile preparare la ricetta scelta.");

			do {
				printf("Inserire:\n"
						"[1] per preparare un altra ricetta"
						"[0] per uscire");
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

			if(scelta==0){
				break;
			}

		}else{
			if(esito_autenticazione == 1){
				// calorie alla giornata dell'utente
				aggiorno_database_calorie(nome_ricetta, FLAG_RICETTA, dosi_ricetta, utente_autenticato.nickname);

			}
		}
	} while (esito_preparazione == 0);


}





/**
 *La funzione seguente riceve il puntatore alla linea da cui voglio estrarre un valore e il numero del campo che voglio analizzare e
 *un flag che indica che tipologia di campo. Il principale compito della funzione e' quello di effettuare una "tokenizzazione" della stringa
 *che gli passiamo. Nel caso di una stringa avente i campi formati da valori singoli nella forma VALORE-DELIMITATORE-VALORE la prima parte
 *fino a alla dichiarazione di tok viene ignorata. In questo preciso caso il token(ossia il puntatore tok) permette l'estrazione del valore
 *successivo al delimitatore su cui e' posizionato. In questo senso tok scandisce i vari delimitatori per trovare il numero del campo nel
 *quale e' presente il valore da estrarre. In caso il numero del campo e quindi il valore da estrarre sia valido e soprattutto esista viene
 *restituito sottoforma di puntatore di stringa. In caso contrario esso viene restituito come NULL
 *
 *
 *Ma adesso soffermiamoci nel caso in cui un campo sia a sua volta formato da campi e di cui non si sappia il numero come e' il caso della preparazione
 *perche' noi non conosciamo il numero di fasi inserite. Lo stratagemma e' quello di passare come num di campo l'indice della fase
 *(ATTENZIONE: qui non si ragiona con indici di array ma per indice si intende il numero del campo che parte da 1). A questo punto lo stratagemma e'
 *quello di contare i caratteri delimitatori(nel nostro caso i - per le fasi), in quanto sappiamo che se la stringa e' nella forma VALORE-DELIMITATORE-VALORE...
 *allora necessariamente il numero di campi sará uguale al numero di delimitatori+1. La prima parte pertanto fa un check da indice del campo e
 *caratteri delimitatori. Qualora il campo passato sia maggiore del numero di - allora abbiamo finito con l'estrazione e viene restituito NULL.
 *
 * @pre  che la stringa su cui effettuare lo split sia piena, che ci sia un numero di campo >0  e che il flag un valore uguale a 0 o 1
 * @post Il valore restituito deve essere una stringa oppure un puntatore null
 */
char* leggi_campo_ricetta(char* linea, int num_campo, short flag_campo){

	char * punta_delimitatore;//puntatore al carattere delimitatore della stringa che gli passiamo
	int num_delimitatori=0;//numero di delimitatori presenti all'interno della stringa
	char* tok; //dichiariamo un token ossia un puntatore alla parte di stringa che di volta in volta viene spezzata

	if(flag_campo==CAMPO_MULTIPLO){//se ci troviamo ad analizzare gli ingredienti o la procedura di preparazione assegna come carattere delimitatore il -

		punta_delimitatore=strchr(linea,DELIMITATORE_PREPARAZIONE ); //punta al primo trattino nella stringa

		while (punta_delimitatore!=NULL){//conta tutti i trattini presenti nella stringa

			num_delimitatori++;
			punta_delimitatore=strchr(punta_delimitatore+1,DELIMITATORE_PREPARAZIONE );//trova la prossima occorrenza del carattere delimitatore e piazza il puntatore

		}

		if(num_campo>num_delimitatori+1)// se il numero di ingredienti analizzati supera quello dei trattini vuol dire che siamo arrivati all'ultimo ingrediente. Restituisci NULL
		  return NULL;
	}


    tok = strtok(linea, SEQUENZA_DELIMITATORI);//inizializzazione della strtok con la stringa da tagliare e il/i carattere/i delimitatori. In questo caso basta il ; per come abbiamo strutturato il csv

    while (tok!=NULL) //smetti di tagliare la stringa nel momento in cui il token non punta ad alcuna sotto-stringa ottenuta dal carattere delimitatore ;
    {

    	num_campo--; // riduciamo di un campo la tupla presa in considerazione

        if (num_campo==0){//ridotta al minimo la tupla iniziale, otterró la stringa che mi interessa ossia il valore del campo che ho deciso di passare alla funzione

        	if(*tok=='\n'){//controllo necessario per quegli aliementi che non possiedono una data di scadenza
            	return "vuoto";//in tal caso si restituisce una stringa si riferimento
            }else{
            	return tok;//restituisco il valore puntato dal token ossia la stringa interessata
            }

		}

        tok = strtok(NULL, SEQUENZA_DELIMITATORI); //Praticamente la strtok mette degli '\0' nella stringa passata al posto del/i carattere/i "delimitatori",ricordiamoci che i caratteri delimitatori possono variare in questa dichiarazione
    }

    return NULL; //restituisci NULL se abbiamo finito di analizzare il file
}



/**
 * Funzione input_nome_ricetta():
 *
 * Permette di richiedere l'input del nome di una ricetta, consentendo gli spazi ma limitando
 * la lunghezza della stringa al massimo stabilito da LUNG_NOME_RICETTA. Se il limite viene superato,
 * l'input viene ripetuto.
 *
 * @pre		Nessuna pre condizione particolare
 * @post	Deve essere restituita una stringa con almeno un carattere.
 */
char* input_nome_ricetta(){
	int esito_input;				// Variabile per memorizzare l'esito dell'input
	// Stringa da restituire contenente il nome della ricetta
	char* nome_ricetta = (char*) calloc(LUNG_NOME_RICETTA, sizeof(char));

	do{
		printf("Inserisci il nome della ricetta:\n>");
		scanf("%20[^\n]", nome_ricetta);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(esito_input == 0){
			puts("Inserimento non valido. Ripeterlo.");
		}
	}while (esito_input == 0);

	return nome_ricetta;
}

/**
 * Funzione input_tempo_preparazione()
 *
 * Permette di richiedere l'input del tempo di preparazione di una ricetta, consentendo gli spazi
 * ma limitando la lunghezza della stringa al massimo stabilito da LUNG_TEMPO_PREPARAZIONE. Se il limite
 * viene superato, l'input viene ripetuto.
 * @pre		Nessuna pre condizione particolare
 * @post	Deve essere restituita una stringa con almeno un carattere
 */
char* input_tempo_preparazione(){
	int esito_input;
	char* tempo_preparazione=(char*) calloc(LUNG_TEMPO_PREPARAZIONE, sizeof(char));

	do {
		printf("Inserisci il tempo di preparazione:\n>");
		scanf("%20[^\n]", tempo_preparazione);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(esito_input == 0){
			puts("Inserimento non valido. Ripeterlo.");
		}
	} while (esito_input== 0);

	return tempo_preparazione;
}

/**
 * Funzione che dopo aver allocato una stringa di una determinata lunghezza, si occupa di ricevere in input la complessitá della ricetta
 * @pre		Nessuna pre condizione particolare
 * @post	Deve essere restituita una stringa con almeno un carattere
 */
char* input_complessita_ricetta(){
	int esito_input;
	char* complessita=(char*) calloc(LUNG_COMPLESSITA, sizeof(char));

	do {
		printf("Inserisci la complessità:\n>");
		scanf("%20[^\n]", complessita);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(esito_input == 0){
			puts("Inserimento non valido. Ripeterlo.");
		}
	} while (esito_input == 0);

	return complessita;
}

/**
 * Funzione che dopo aver allocato una stringa di una determinata lunghezza, si occupa di ricevere in input la preparazione della ricetta
 * @pre		Nessuna pre condizione particolare
 * @post	Deve essere restituita una stringa con almeno un carattere
 */
char* input_preparazione_ricetta(){
	int esito_input;
	int esito_controllo;
	int scelta;
	char s[LUNG_PREPARAZIONE];
	char* preparazione = (char*) calloc(LUNG_PREPARAZIONE, sizeof(char));

	do {
		printf("Inserire una fase della preparazione, premendo invio per confermare l'inserimento.\n[max. %d caratteri]:\n>", LUNG_PREPARAZIONE - strlen(s) - 1);
		esito_input = scanf("%500[^\n]", s);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}else if(strlen(preparazione) + strlen(s) > LUNG_PREPARAZIONE - 1){
			esito_input = 0;
		}

		if(esito_input == 0){
			puts("Inserimento non valido. Ripeterlo.");
		}else{
			do {
				printf("Inserire 1 per inserire una nuova fase, 0 per terminare l'inserimento");
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

			esito_input = 1;

			strcat(preparazione, s);

			if(scelta == 1){
				strcat(preparazione, " - ");
			}
		}
	} while (esito_input == 0 || scelta == 1);

	return preparazione;
}

/**
 * Funzione che dopo aver allocato una stringa di una determinata lunghezza, si occupa di ricevere in input l'ingrediente x della ricetta. Ovviamente in questo caso sono ammessi
 * gli spazi in quanto sappiamo che il formato dell'ingrediente e' :  quantitá  unitá_di_misura nome_ingrediente
 * @pre		Nessuna pre condizione particolare
 * @post	Deve essere restituita una stringa con almeno un carattere
 */
char* input_ingredienti_ricetta(){
	int esito_input;
	char* ingrediente = (char*) calloc(LUNG_STR_LAVORO_RIC, sizeof(char));

	do {
		printf("Inserisci il nuovo ingrediente (\"null\" per terminare l'inserimento):\n>");
		esito_input = scanf ("%[^\n]", ingrediente);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(esito_input != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}
	} while (esito_input != 1);

	return ingrediente;
}

/**
 ** Funzione che  si occupa di ricevere in input il numero di porzioni della ricetta
 * @pre		Nessuna pre condizione particolare
 * @post	Deve essere restituita una stringa con almeno un carattere
 */
int input_dosi_ricetta(){
	int esito_input;
	int esito_controllo;
	int dosi;

	do {
		printf("Inserisci le dosi per la ricetta:\n>");
		esito_input = scanf ("%d", &dosi);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(dosi < MIN_DOSI || dosi > MAX_DOSI){
			esito_controllo = 0;
		}else{
			esito_controllo = 1;
		}

		if(esito_input != 1 || esito_controllo != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}
	} while (esito_input != 1 || esito_controllo != 1);

	return dosi;
}


int input_id_ricetta(int max_id){
	int id;
	int esito_input;
	int esito_controllo;

	do{
		printf("Inserire l'id della ricetta da selezionare [1 - %d]:\n>", max_id);
		esito_input = scanf("%d", &id);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(id < 1 || id > max_id){
			esito_controllo = 0;
		}else{
			esito_controllo = 1;
		}

		if(esito_input != 1 || esito_controllo != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}
	}while(esito_input != 1 || esito_controllo != 1);

	return id;
}


/**
 * La funzione modifica ricette si occupa di modificare un determinato campo di una determinata ricetta. In una prima fase verrá aperto il file che rappresenta il dtabase delle ricette.
 * In seguito verrá mostrato all'utente una vista minimizzata delle varie ricette salvate e verrá chiesto l'id della ricetta a cui apportare le modifiche. Una volta inserito viene
 * effettuata una ricerca sequenziale, evitando le tuple vuote presenti nel file. Una volta trovato la ricetta nel file del database verrá chiesto di inserire il numero del campo
 * da modificare. Tramite uno switch viene individuato il campo e viene richiamata la corrispettiva funzione di input. Per quanto riguarda gli ingredienti una volta ricevuto in ingresso
 * la stringa da parte dell'utente viene effettuato uno split dei valori per popolare opportunatamente la struct di tipo ingrediente. Al termine delle modifiche viene salvato il tutto
 * su file.
 * @pre		Nessuna pre condizione particolare
 * @post	Deve essere stato effettuata e salvata la modifica su file
 */
int modifica_ricetta(){
	FILE* stream = NULL;
	ricetta ricetta_scelta;							// struct di riferimento per scorrere il file
	int num_ricette_database;
	int id_ricetta;										// Variabile per memorizzare il numero corrispondente alla ricetta selezionata dall'utente
	int num_ricette_lette = 0;							//
	int scelta_campo = 0;								// Variabile per memorizzare la scelta dell'utente di quale campo modificare
	int esito_input;
	int esito_controllo;
	char divisione_ingrediente[LUNG_STR_LAVORO_RIC]; 	// Stringa per effettuare la tokenizzazione degli ingredienti
	int indice_ingredienti = 0;							// indice per l'array di struct ingrediente memorizzato, a sua volta , nella array di tipo ricetta


	// Visualizzazione del database ottenendo il numero di ricette in esso presenti
	num_ricette_database = visualizza_database_ricette(VISTA_MINIMIZZATA);

	if(num_ricette_database == 0){
		puts("Impossibilie effettuare alcuna modifica.");
		return 0;
	}

	// Apertura del file in modalità lettura e aggiornamento binario
	if((stream = fopen(FILE_DATABASE_RICETTE, "rb+")) == NULL){
		puts("Impossibilie effettuare alcuna modifica.");
		return 0;
	}else{

		id_ricetta = input_id_ricetta(num_ricette_database);

		while(fread(&ricetta_scelta, sizeof(ricetta_scelta), 1, stream) > 0){//leggi fino a quando e' presente una riga

			if(strcmp(ricetta_scelta.nome_ricetta, "") != 0){
				num_ricette_lette++;
			}

			// Se l'id selezionato dall'utente è pari al numero di ricette lette, è stata identificata la ricetta da modificare
			if(id_ricetta == num_ricette_lette){
				break;
			}
		 }

		do {
			fseek(stream, -sizeof(ricetta), SEEK_CUR);

			visualizza_ricetta(ricetta_scelta, VISTA_TOTALE);

			printf("Scegli quale campo modificare:\n"
					"[1] Nome\n"
					"[2] Tempo preparazione\n"
					"[3] Complessita\n"
					"[4] Preparazione(per favorire di una migliore visibilitá, separare le varie fasi con il carattere - )\n"
					"[5] Ingredienti (si ricorda che ogni ingrediente deve essere nella forma:\n    <quantita>   <unita' di misura>   <nome_ingrediente>)\n"
					"[6] Numero porzioni\n"
					"[0] Esci\n");


			do {
				printf("\n Seleziona il campo da modificare:\n>");
				esito_input = scanf("%d", &scelta_campo);
				if(pulisci_stdin() == 1){
					esito_input = 0;
				}

				if(scelta_campo < 0 || scelta_campo > 6){
					esito_controllo = 0;
				}else{
					esito_controllo = 1;
				}

				if(esito_input != 1 || esito_controllo != 1){
					puts("inserimentonon valido. Ripeterlo.");
				}
			} while (esito_input != 1 || esito_controllo != 1);

			switch(scelta_campo){
			case 0:
				break;
			case 1:
				strcpy(ricetta_scelta.nome_ricetta, input_nome_ricetta());
				break;
			case 2:
				strcpy(ricetta_scelta.tempo_preparazione, input_tempo_preparazione());
				break;
			case 3:
				strcpy(ricetta_scelta.complessita, input_complessita_ricetta());
				break;
			case 4:
				strcpy(ricetta_scelta.preparazione, input_preparazione_ricetta());
				break;
			case 5:
				/*Qualora fosse il campo ingrediente ad essere scelto, una volta ricevuti in input i dati dell'ingrediente si effettua la divisione della stringa
				 *  di lavoro(divisione_ingrediente) per memorizzare separatamente i dati dell'ingrediente. Inoltre al termine dell'inserimento occorre ricalcolare le kcal per ricetta
				 */
				do{
					strcpy(divisione_ingrediente, input_ingredienti_ricetta());

					if(strcmp(divisione_ingrediente, "null") != 0){

						int esito_lettura;
						esito_lettura = sscanf(divisione_ingrediente,
								"%f %[a-zA-Z] %[a-zA-Z]",
								&ricetta_scelta.ingredienti[indice_ingredienti].quantita, ricetta_scelta.ingredienti[indice_ingredienti].unita_misura,
								ricetta_scelta.ingredienti[indice_ingredienti].nome);


						if(controlla_unita_misura(ricetta_scelta.ingredienti[indice_ingredienti].unita_misura) == 0){
							esito_lettura = 0;
						}else{
							alimento_database dati_ingrediente;
							if(ricerca_alimento_database(ricetta_scelta.ingredienti[indice_ingredienti].nome, &dati_ingrediente) == 1){
								if(strcmp(dati_ingrediente.unita_misura, ricetta_scelta.ingredienti[indice_ingredienti].unita_misura) != 0){
									esito_lettura = 0;
								}else if(controlla_quantita(ricetta_scelta.ingredienti[indice_ingredienti].quantita,
										ricetta_scelta.ingredienti[indice_ingredienti].unita_misura) != 1){
									esito_lettura = 0;
								}
							}
						}

						if(esito_lettura == 0){
							printf("Ingrediente inserito non valido. Ripetere inserimento");
						}else{
							indice_ingredienti++;
						}

					}else{
						break;
					}

				}while(indice_ingredienti < MAX_INGREDIENTI);

				ricetta_scelta.kcal_ricetta = 0;// conta_kcal_ricetta(catalogo_ricetta.ingredienti,indice_ingredienti+1);//memorizzo le kcal della ricetta passando i dati di essa
				break;

			case 6:
				ricetta_scelta.dosi = input_dosi_ricetta();
				break;
			}

			// scrivo la riga "inizializzata" alla posizione della ricetta rimossa
			fwrite(&ricetta_scelta, sizeof(ricetta), 1, stream);//scrivi il contenuto della struct aggiornata
			//esci e termina l'inserimento
		} while (scelta_campo != 0);
	}

	fclose(stream);
	return 1;
}




/**
 *La funzione elimina_ricetta apre il database delle ricette, ne mostra il contenuto, e chiede quale delle ricette mostrate occorre cancellare.
 *L'individuazione della ricetta avviene tramite l'inserimento dell'ID. In questa maniera si risale alla riga dove e' memorizzata la ricetta
 *inizializzandone il contenuto e salvando poi le modifiche. Al termine la funzione restituisce 1.
 * @pre		Nessuna pre condizione particolare
 * @post	Deve essere stato effettuata e salvata l'eliminazione su file
 */
int elimina_ricetta(){
	int id_ricetta;//l'id ci servirá per capire quale ricetta e' stata selezionata dall'utente e quindi a scorrere le varie ricette
	int num_ricette_lette = 0;//indice che ci aiuta a capire a quale riga siamo arrivati e quindi quale ricetta stiamo analizzando
	int num_ricette_database;
	FILE* stream = NULL;
	ricetta ricetta_scelta;//struct di riferimento per scorrere il file

	puts("Cancellazione ricetta.\n");

	num_ricette_database = visualizza_database_ricette(VISTA_MINIMIZZATA); //viene stampato il contenuto attuale del database delle ricette

	if(num_ricette_database == 0){
		puts("Non sono state ancora memorizzate delle ricette.");
		return 0;
	}


	if((stream = fopen(FILE_DATABASE_RICETTE, "rb+")) == NULL){
		puts("Impossibile aprire il file database ricette.");
		return 0;
	}else{
		id_ricetta = input_id_ricetta(num_ricette_database);

		while(fread(&ricetta_scelta, sizeof(ricetta), 1, stream) > 0){//leggi fino a quando e' presente una riga

			if(strcmp(ricetta_scelta.nome_ricetta, "")!=0){//se la linea non e' vuota e ha il nome di una ricetta
				num_ricette_lette++;//incrementa il numero di linee effettivamente piene del file database ricette
			}

			if(id_ricetta == num_ricette_lette){//se l'id della ricetta che si e' preso come riferimento e' uguale alla linea su cui ci troviamo abbiamo trovato la ricetta  a cui apportare le modifiche

				strcpy(ricetta_scelta.nome_ricetta, "");

				// scrivo la riga "inizializzata" alla posizione della ricetta rimossa
				fseek(stream, -sizeof(ricetta), SEEK_CUR);//posiziona il puntatore sulla locazione di memoria da cui iniziare a scrivere
				fwrite(&ricetta_scelta, sizeof(ricetta), 1, stream);//scrivi il contenuto della struct aggiornata
				break;//esci e termina l'inserimento
			}
		}
	}

	fclose(stream);
	return 1;
}

/**
 * La funzione visualizza_ricette riceve come parametro di ingresso la struct dove sono memorizzate le info della ricetta da mostrare.
 * La stampa dei campi composti da informazioni singole come il nome e il tempo di preparazione vengono mostrate tramite un'unica printf
 * Nel caso degli ingredienti viene stampato di volta in volta il contenuto della matrice tramite un ciclo. Per quanto riguarda la preparazione
 * (considerato che dal punto di vista progettuale si considerano le fasi come un unico testo) nella fase di visualizzazione le singole fasi,
 * che sono separate nel testo con il carattere delimitatore (-),  vengono opportunatamente divise tramite la funzione leggi_campo_ricetta.
 * Una volta ricavata ogni fase essa viene stampata con il suo id di riferimento.
 * @pre		Nessuna pre condizione particolare
 * @post	Deve essere stato mostrato il contenuto della ricetta
 */
int visualizza_ricetta(ricetta dati_ricette, int vista_ricetta){
	int indice_ingrediente = 0; //indice che ci serve per scorrere tra gli ingredienti
	int indice_preparazione = 0; //indice che ci serve per scorrere tra le fasi della preparazione

	printf("%14s%8s%14s%10d%10d\n", dati_ricette.nome_ricetta,dati_ricette.tempo_preparazione, dati_ricette.complessita,dati_ricette.kcal_ricetta, dati_ricette.dosi);

	if(vista_ricetta==VISTA_TOTALE){
		printf("\n INGREDIENTI:\n\n");
		printf(" -------------------------------------------------------------\n");
		printf("|ID| NOME INGREDIENTE| QUANTITA' NECESSARIA | UNITA DI MISURA |\n");
		printf(" -------------------------------------------------------------\n");

		while(strcmp(dati_ricette.ingredienti[indice_ingrediente].nome,"")!=0){

			printf("%2d|%18s|%22.1f|%17s|\n",
					indice_ingrediente, dati_ricette.ingredienti[indice_ingrediente].nome,
					dati_ricette.ingredienti[indice_ingrediente].quantita,dati_ricette.ingredienti[indice_ingrediente].unita_misura);
			printf(" -------------------------------------------------------------\n");

			indice_ingrediente++;
		}

		printf("\n\n PREPARAZIONE:\n");
		while(indice_preparazione < MAX_NUM_FASI){

			char* copia_riga = strdup(dati_ricette.preparazione); //le modifiche (i "tagli" per trovare la fase) vengono fatte sulla copia

			if(leggi_campo_ricetta(copia_riga,indice_preparazione+1,CAMPO_MULTIPLO)==NULL){ //se la prossima fase non esiste esci.
				break;

			}else{

				copia_riga = strdup(dati_ricette.preparazione);
				printf(" %d. %s\n", indice_preparazione+1, leggi_campo_ricetta(copia_riga,indice_preparazione+1,CAMPO_MULTIPLO));//stampa la fase con l'indice
				indice_preparazione++;

			}

		}


	printf("\n*********************************************************************\n"
			"*********************************************************************\n");
	}else{
		printf(" ____________________________________________________________\n");
	}
	return 1;

}

/**
 * Visualizza_database_ricette mostra il contenuto del database delle ricette in base alla modalitá con cui scegliamo di mostrare i dati. Il parametro
 * in ingresso nome_ricetta, infatti, qualora fosse una stringa vuota mostrerá l'intero contenuto del database. Nel caso in cui venga passato un nome
 * di una ricetta verranno in quel caso estratte dal database le informazioni riguardo quella ricetta qualora sia presete nel database.
 * Inoltre il parametro in ingresso id_ricetta permette di mostrare a schermo un id che magari e' calcolato esternamente alla funzione(si faccia
 * riferimento alla funzione suggerimento_ricetta_automatico o suggerimento_ricetta_manuale per capire in quale contesto la si utilizza). Qualora
 * invece non sia nessun interesse o nel caso in cui debba essere mostrato l'intero database verrá passato come id personalizzato 0.
 * @pre		Deve essere passata una stringa o vuota o piena, l'id deve essere un numero >=1 e la vista deve essere un valore pari a 0 o 1
 * @post	Deve essere stata effettuata la visualizzazione opportuna
 */
int visualizza_database_ricette(int vista){

	int flag_mostra_int = 0;
	int num_ricette_lette = 0;
	FILE* stream = NULL;
	ricetta ricetta_letta;//creo una struct di tipo alimenti ome riferimento per scorrere all'interno del file

	if((stream = fopen(FILE_DATABASE_RICETTE, "rb")) == NULL){
		puts("Non sono state ancora memorizzate delle ricette.");
		return num_ricette_lette;
	}else{
		//fino a quando puoi prelevare righe stampa il contenuto della struct ossia gli elementi di ogni riga
		while(fread(&ricetta_letta, sizeof(ricetta), 1, stream) > 0){

			if(feof(stream) != 0){
				break;
			}

			if(strcmp(ricetta_letta.nome_ricetta, "") != 0){

				num_ricette_lette++;

				 if(flag_mostra_int == 0){
					printf(" ____________________________________________________________\n");
					printf("|ID| NOME RICETTA | DURATA |  COMPLESSITA' | KCAL | PORZIONI |\n");
					printf(" ____________________________________________________________\n");
				 }

				printf("%3d", num_ricette_lette);
				visualizza_ricetta(ricetta_letta, vista);
			}

			if(vista==VISTA_MINIMIZZATA){
				flag_mostra_int = 1;
			}

		}
	}

	if(num_ricette_lette == 0){
		puts("Non sono state ancora memorizzate delle ricette.");
	}

	fclose(stream);
    return num_ricette_lette;
}



/**
 * La funzione riceve in ingresso un array di struct passato per riferimento e lo popola prendendo le varie informazioni delle ricette,
 * memorizzate all'interno del database. Ovviamente vengono memorizzate solo le righe che non sono inizializzate ossia quelle che non vuote.
 * Per far ció si confronta il nome. Qualora il nome della ricetta sia una stringa vuota vuol dire che siamo in presenza di una riga vuota
 * e pertanto essa va ignorata.
 * @pre		L'array di struct che passiamo deve essere vuoto
 * @post	Deve essere stato caricato correttamente l'array di struct inizialmente passato
 */
int lettura_database_ricette(ricetta* ricette_database){
	FILE* stream = NULL;
	ricetta ricetta_letta;
	int num_ricette_lette = 0;

	if((stream = fopen(FILE_DATABASE_RICETTE, "rb+")) == NULL){
		return -1;
	}else{

		while(fread(&ricetta_letta, sizeof(ricetta), 1, stream)>0){

			if(feof(stream) != 0){
				break;
			}

			if(strcmp(ricetta_letta.nome_ricetta, "")){
				ricette_database[num_ricette_lette] = ricetta_letta;
				num_ricette_lette++;
			}
		}
	}

	return num_ricette_lette;
}

/**
 * La funzione apre il file del database delle ricette in modalitá lettura, estrae tutte le righe che possiede e conta solo le righe effettivamente
 * piene ignorando quelle inizializzate. Il risultato viene poi restiuito
 * @pre		Nessuna pre condizione particolare
 * @post	Deve essere passato un numero di righe con un valore significativo(>=0)
 */
int conta_ricette_database(){
	FILE* stream = NULL; //apri il file in modalitá "lettura binaria"
	int num_ricette = 0;				    //variabile che conta le righe del file
	ricetta ricetta_letta;			//genero una struct di riferimento che mi permette di scorrere all'interno del file di tipo binario

	if((stream = fopen(FILE_DATABASE_RICETTE,"rb")) == NULL){
		return num_ricette;
	}else{

		while(fread(&ricetta_letta, sizeof(ricetta), 1, stream) > 0){//fino a quando riesci a leggere righe dal file

			if(feof(stream) != 0){
				break;
			}

			if(strcmp(ricetta_letta.nome_ricetta, "") != 0){
				num_ricette++;		//incremento il contatore del numero di righe
			}
		}

		fclose(stream); //chiudi il file
		return num_ricette;
	}
}


/**
 * Funzione che ha in ingresso un array di puntatori di tipo stringa che rappresentano i nomi delle ricette preparabili e il numero di ricette
 * presenti nel database. Questo perche' nella migliore delle ipotesi tutte le ricette del database possono essere preparabili. La funzione inizializza
 * tutto l'array di puntatori a NULL
 * @pre		Il numero di ricette deve essere un valore maggiore di 0
 * @post	Deve essere stata effettuata correttamente l'inizializzazione
 */
int inizializza_ricette_preparabili(int num_ricette, char* ricette_preparabili[num_ricette]){

	if(num_ricette <= 0){
		return 0;
	}

	for(int i=0;i<num_ricette;i++){
		ricette_preparabili[i] = NULL;
	}

	return 1;
}


/**
 * La funzione seguente si occupa,avendo in ingresso il num di ricette complessive e l'array di struct che costituisce la lista di tutte le ricette,
 * di ordinare le ricette in base alle kcal prodotte, tramite l'algoritmo di ordinamento shell sort. A differenza del classico shell sort che
 * sfrutta un array formato dai vari gap ossia dalle varie distanze con cui confrontare e scambiare gli elementi, qui si e' preferito sfruttare
 * inizialmente il num di ricette diviso 2 come gap di partenza e successivamente dividere il gap fino a quando non risultasse minore di 0.
 * Ovviamente lo scambio viene fatto in base al campo delle kcal. Al termine viene stampato l'ordinamento effettuato
 * @pre		l'array di struct con le ricette deve possedere almeno 1 ricetta e il numero di ricette deve essere un valore significativo(>0)
 * @post	Deve essere stato effettuato correttamente l'ordinamento
 */
int ordina_ricette_kcal(ricetta* ricette_database, int num_ricette){

	ricetta x ;//struct di appoggio per scambiare 2 righe

	for (int gap = num_ricette/2; gap > 0; gap /= 2){

		for (int i = gap; i < num_ricette; i += 1){

			x = ricette_database[i];
			int j;
			for (j = i; j >= gap && ricette_database[j - gap].kcal_ricetta > x.kcal_ricetta; j -= gap)
				ricette_database[j] = ricette_database[j - gap];

			ricette_database[j] = x;
		}
	}

	for (int i=0; i < num_ricette; i++){
		printf("%s %d\n", ricette_database[i].nome_ricetta, ricette_database[i].kcal_ricetta);
	}

	return 1;
}




/**
 * In questa funzione viene passata la lista degli alimenti attualmente nel frigo con il relativo numero e una matrice che rappresenta
 * il nome degli alimenti che stanno per scadere. Una volta chiamata la funzione che si occupa del riordinamento degli alimenti in base alla scadenza
 * vengono memorizzati i primi 2 alimenti che stanno per scadere all'interno di una matrice di tipo char. In seguito viene fatto un richiamo alla funzione che suggerisce le ricette
 * in base agli alimenti che gli si passano. In questa maniera avremo la lista delle ricette che possiamo preparare con gli alimenti in scadenza
 * @pre		La listadegli alimenti deve possedere almeno un alimento e il numero  di alimenti deve essere un valore significativo
 * @post	Deve essere terminata con successo la ricerca
 */
int ricette_alimenti_in_scadenza(alimento_frigo* alimenti_frigo, int num_alimenti){

	char alimenti_in_scadenza[MAX_ALIM_SUGG][LUNG_NOME_ALIMENTO];

	ordina_alimenti_scadenza(alimenti_frigo, num_alimenti);

	printf("Gli alimenti che stanno per scadere sono: %s e %s\n",alimenti_frigo[0].nome,alimenti_frigo[1].nome);

	for(int i=0;i<NUM_ALIM_SUGG_SCAD;i++){
		strcpy(alimenti_in_scadenza[i],alimenti_frigo[i].nome);
	}
	suggerimento_ricetta_manuale(NUM_ALIM_SUGG_SCAD,alimenti_in_scadenza);
	return 1;
}

//controllo sulla risposta
/**
 *La funzione riceve in ingresso una matrice vuota che rappresenta i nomi degli ingredienti su cui svolgere la ricerca. Dopo aver
 *pulito il buffer di input si passa all'inserimento degli alimenti chiave da parte dell'utente. Qualora sia stata inserita la sequenza
 *pulito di escape si termina con l'inserimento altrimenti si continua fino al numero massimo di ingredienti consentito.
 * @pre		Deve essere passata una matrice vuota
 * @post	Deve essere stato inserito almeno un alimento
 */
int inserimento_manuale_ingredienti(char nome_ingredienti[MAX_ALIM_SUGG][LUNG_NOME_ALIMENTO]){
	int num_alimenti_inseriti = 0;
	int esito_input = 0;
	printf("Cercare una ricetta in base agli ingredienti e' semplice. \n"
			"1.Inserisci il nome dell'ingrediente con cui vuoi realizzare una tua ricetta\n"
			"2.Decidi se continuare o meno con l'inserimento  (premi ctrl+z per terminare)\n"
			"Ti ricordiamo che potrai inserire fino ad un massimo di %d ingredienti\n\n",MAX_ALIM_SUGG );

	printf("Inserisci nome ingrediente su cui effettuare la ricerca\n");
	do{

		printf("%d.", num_alimenti_inseriti+1);

		if(gets(nome_ingredienti[num_alimenti_inseriti])==NULL){
			break;
		}else if(strlen(nome_ingredienti[num_alimenti_inseriti]) > LUNG_NOME_ALIMENTO){
			esito_input = 0;
		}else{
			esito_input = 1;
		}

		if(esito_input == 0){
			puts("Inserimento non valido. Ripeterlo.");
		}else{
			num_alimenti_inseriti++;
		}


		printf("Inserisci prossimo ingrediente (o termina con ctrl+z)\n");
	}while(esito_input != 1 || num_alimenti_inseriti < MAX_ALIM_SUGG);


	return num_alimenti_inseriti;
}



/**
 *La funzione suggerimento_ricetta_manuale avendo in ingresso il numero(num_alimenti_sugg) e i nomi degli ingredienti(nome_ingredienti)
 *su cui effettuare la ricerca fornisce il nome delle ricette che si possono preparare con tali ingredienti. La funzione e' composta da un ciclo
 *su piú esterno che scandisce le vare ricette presenti ne database, e 2 cicli piú interni che si occupano di scandire di volta gli ingredienti
 *passati come parametro alla funzione con quelli che possiede ogni ricetta. Nel caso in cui una ricetta abbia gli ingredienti che abbiamo
 *passato essa va memorizzata come ricetta preparabile. Al termine del controllo sulle ricette vengono stampate a schermo quelle preparabili
 *passato qualora ce ne fossero o un messaggio di avvertenza che segnala che non esistono ricette con la combinazione di ingredienti fornita in input
 *
 * @pre		Deve essere passato almeno un alimento e quindi il numero di alimenti deve essere anch'esso un valore significativo
 * @post	Deve essere stato mostrato un messaggio in base all'esito della ricerca
 */
int suggerimento_ricetta_manuale(int num_alimenti_sugg, char nome_ingredienti[MAX_ALIM_SUGG][LUNG_NOME_ALIMENTO]){
	int num_ricette = conta_ricette_database();
	int presenza_alimento=0; // serve a capire se un ingrediente e' presente o meno nella ricetta
	int indice_ingrediente=0; // serve a scandire i vari ingredienti della ricetta
	ricetta ricette_preparabili[num_ricette]; //serve a memorizzare i nomi delle ricette che si possono preparare. Nel migliore dei casi esso avrá lunghezza pari al numero di ricette presente nel database
	int indice_ric_prep=0; //serve a contare e a spostarsi nell'array delle ricette preparabili

	ricetta ricette_database[num_ricette];// array di struct che memorizza le varie ricette
	lettura_database_ricette(ricette_database); //popolamento delle ricette presenti nel database

	printf("RICETTE PREPARABILI:\n");
	//inizializza_ricette_preparabili(num_ricette, ricette_preparabili);//occorre inizializzare l'array di puntatori per effetuare controlli futuri

	for(int i=0;i<num_ricette;i++){

		for(int j=0;j<num_alimenti_sugg;j++){//ripeto per il numero di alimenti suggeriti

			while(strcmp(ricette_database[i].ingredienti[indice_ingrediente].nome, "")!=0){//prendiamo in considerazione solo le righe piene

				if(strcmp(ricette_database[i].ingredienti[indice_ingrediente].nome, nome_ingredienti[j])==0){
					presenza_alimento = 1;//puoi segnalare che l'ingrediente e' presente
					break;
				}
				indice_ingrediente++;
			}

			indice_ingrediente=0;
			if(presenza_alimento == 0){//in caso il flag di presenza sia rimasto a 0 passa alla ricetta successiva
				break;
			}else{
				if(num_alimenti_sugg != 1 && j != num_alimenti_sugg-1) //se la lista degli alimenti non e' composta solo da un elemento e se non siamo arrivati all'ultimo ingrediente suggerito
					presenza_alimento = 0;//riporta il flag di presenza a 0 per poter analizzare il prossimo ingrediente
			}

		}

		if(presenza_alimento==1){//se l'ultimo flag di presenza e' 1 vuol dire che la ricetta che ho analizzato e' valida pertanto la salvo
			ricette_preparabili[indice_ric_prep] = ricette_database[i];
			indice_ric_prep++;
		}

		indice_ingrediente=0;
		presenza_alimento=0;
	}


	if(indice_ric_prep==0){//qualora non siano stati memorizzati ricette preparabili notifica l'utente
		printf("Non c'e' alcuna ricetta che puoi preparare con la combinazione di ingredienti da te inserita\n");
	}else{
		for(int i=0;i<indice_ric_prep;i++)
			visualizza_ricetta(ricette_preparabili[i],VISTA_MINIMIZZATA);
	}

	return 1;

}



/**
 *La funzione suggerimento_ricetta_automatica avendo in ingresso gli alimenti del frigo e il numero di tali alimenti fornisce il nome delle ricette
 *che si possono preparare con tali ingredienti. La funzione e' composta da un ciclo piú esterno che scandisce le vare ricette presenti ne database, e
 *2 cicli piú interni che si occupano di scandire di volta gli alimenti presenti nel frigo con quelli che possiede ogni ricetta.
 *Nel caso in cui l'ingrediente di una ricetta sia presente bisogna controllare se la quatitá dell'alimento a disposizione sia sufficiente.
 *Il controllo viene effettuato pertanto anche qualora vi sia piú di una occorrenza di uno stesso alimento. A tal proposito sappiamo che stessi
 *alimenti con diversa scadenza sono memorizzati su record diversi. In questo caso devo sommare le quantitá degli alimenti con stesso nome
 *ESEMPIO: ho 2 uova che scadono x/xx/xxxx e 2 uova che scadono yy/yy/yyyy. Voglio preparare la torta di mele che ne richiede 4. Posso prepararla perche'
 *ho 4 uova a disposizione complessivamente.
 *Al termine del controllo sulle ricette vengono stampate a schermo quelle preparabili
 *passato qualora ce ne fossero o un messaggio di avvertenza che segnala che non esistono ricette con la combinazione di ingredienti fornita in input
 * @pre		Deve essere passato almeno un alimento e quindi il numero di alimenti deve essere anch'esso un valore significativo
 * @post	Deve essere stato mostrato un messaggio in base all'esito della ricerca
 */
int suggerimento_ricetta_automatico(){

	int num_ricette = conta_ricette_database();
	int flag_preparazione;
	ricetta ricette_preparabili[num_ricette]; //serve a memorizzare i nomi delle ricette che si possono preparare. Nel migliore dei casi esso avrá lunghezza pari al numero di ricette presente nel database
	int indice_ric_prep = 0;

	ricetta ricette_database[num_ricette];
	lettura_database_ricette(ricette_database);

	for(int i=0; i < num_ricette; i++){
		flag_preparazione = 1;
		for(int j = 0; j < MAX_INGREDIENTI; j++){
			if(strcmp(ricette_database[i].ingredienti[j].nome, "") != 0){
				if(ricette_database[i].ingredienti[j].quantita > quantita_alimento(ricette_database[i].ingredienti[j].nome)){
					flag_preparazione = 0;
				}
			}else{
				break;
			}
		}

		if(flag_preparazione == 1){
			ricette_preparabili[indice_ric_prep] = ricette_database[i];
			indice_ric_prep++;
		}
	}

	if(indice_ric_prep == 0){
			printf("Non c'e' alcuna ricetta che puoi preparare con gli alimenti attuali. Fai la spesa\n");
			return -1;
	}else{
		for(int i=0; i < indice_ric_prep; i++){
			visualizza_ricetta(ricette_preparabili[i], VISTA_MINIMIZZATA);
		}
	}

	return 1;
}




/**
 * La funzione riceve in input la struct contenente i dati inerenti ad una singola ricetta e procede con il controllo sull'esistenza e apertura del file databse_ricette. Qualora non
 * sia possibile aprire il file in lettura viene creato per scriverci sopra la prima volta. La prima operazione da compiere qualora il file esista e' controllare se la ricetta
 * che vogliamo salvare non sia giá presente nel database. Per far ció leggiamo sequenzialmente il file e se troviamo una ricetta con lo stesso nome aggiorniamo il flag di presenza.
 * Nel caso in cui la ricetta abbia superato il controllo e pertanto non sia presente nel database, essa viene salvata alla prima riga vuota presente nel file(ricordiamo che
 * il progetto in generale si basa sul recupero delle righe all'interno di un file).
 * @pre		Deve essere passata una struct contenente valori significativi
 * @post	Devono essere stati scritti su file i dati relativi alla ricetta
 */
int aggiorna_database_ricette(ricetta nuova_ricetta){
    ricetta analisi_ricetta;//genero una struct di rifermiento per scorrere all'interno del file
	FILE* stream = NULL;
	fpos_t riga_vuota = -1;

	if((stream = fopen(FILE_DATABASE_RICETTE, "rb+")) == NULL){// se il file degli alimenti presenti nel frigo esiste
		if((stream = fopen(FILE_DATABASE_RICETTE, "wb+")) == NULL){
			return 0;
		}
	}

	// Viene ricercata nel file una riga marcata come cancellata (nome uguale a stringa vuota)
	while(fread(&analisi_ricetta, sizeof(analisi_ricetta), 1, stream) > 0){

		if(feof(stream) != 0){
			break;
		}

		// Se viene individuata una riga marcata come cancellata, ne viene memorizzata la posizione
		if(strcmp(analisi_ricetta.nome_ricetta, "") == 0){
			fseek(stream, -sizeof(ricetta), SEEK_CUR);
			fgetpos(stream, &riga_vuota);
			break;
		}
	}

	// Se la riga vuota è stata identificata, va impostato il puntatore a quella riga
	if(riga_vuota != -1){
		fsetpos(stream, &riga_vuota);
	}else{ // Altrimenti viene impostato il puntatore alla fine del file
		fseek(stream, 0, SEEK_END);
	}

	// Viene memorizzata la nuova ricetta nel file
	fwrite(&nuova_ricetta, sizeof(ricetta), 1, stream);

    fclose(stream);
  	return 1;
}



/**
 * La funzione lettura_nuove_ricette si occupa di leggere un file in formato csv con accesso sequenziale e di memorizzare nei vari campi le informazioni riguardanti una nuova ricetta.
 * Viene pertanto scandita con la fgets ogni linea presente nel file,estratta e poi splittata tramite la sscanf. Qualora fossero rinvenuti errori come l'assenza di un campo viene chiesto
 * se si vuole modificare la ricetta oppure no. Qualora l'estrazione da file abbia avuto successo viene effettuato lo split degli ingredienti per memorizzare opportunamente i valori all'interno
 * dell'array di struct. Infine vengono calcolate le calorie prodotte dalla ricetta e viene richiamata la funzione che si occupa della memorizzazione dei dati raccolti. L'operazione viene
 * ripetuta fino a quando e' possibile estrarre una riga dal file ossia fino a quando e' presente una ricetta nel file
 * @pre		Nessuna particolare pre-condizione
 * @post	Deve essere stato effettuato con successo almeno un'estrazione di una ricetta o in caso contrario deve essere mostrato un messaggio di errore
 */
int lettura_nuove_ricette(){
	FILE* stream;//apro il file in lettura
	ricetta ricetta_letta; //dichiaro un'array di struct che ospiterá i valori dei singoli alimenti comprati

	alimento_database dati_ingrediente;

	int flag_inserimento;
	int num_ricette_lette = 0;
	int num_ricette_scartate = 0;
	int num_ingredienti = 0;
	char linea[LUNG_TUPLA_RICETTE];//dichiariamo la stringa che conterrá l'intera tupla proveniente dal file
	char ingredienti[LUNG_STR_LAVORO_RIC];

	int esito_lettura;							// Variabile per contenere l'esito delle letture degli alimenti
	char* divisione_ingrediente;

	// tentativo di apertura della spesa in lettura
	if((stream = fopen(FILE_NUOVE_RICETTE, "r")) == NULL){
		puts("Non è possibile aprire il file 'nuove_ricette.csv'.");
		return -1; // se il file non può essere aperto viene ritornato il valore -1
	}else{
		// estrazione di ogni riga del file (riga == 1 alimento)
		while(fgets(linea, LUNG_TUPLA_RICETTE, stream) != NULL){
			num_ricette_lette++;
			flag_inserimento = 1;
			// Pulitura dallo stream della parte di riga non letta
			if(linea[strlen(linea) - 1] != '\n'){
				pulisci_riga_flusso(stream);
			}

			// azzeramento del numero di ingredienti per la lettura della nuova ricetta
			num_ingredienti = 0;

			//inizializzazione ingredienti e preparazione
			for(int i=0;i<MAX_INGREDIENTI;i++){
				strcpy(ricetta_letta.ingredienti[i].nome, "");
			}

			// estrazione dei campi della riga letta dal file
			esito_lettura = sscanf(linea, "%20[a-zA-Z];%100[^;];%20[^;];%500[^;];%20[a-zA-Z];%d",
					ricetta_letta.nome_ricetta, ingredienti, ricetta_letta.tempo_preparazione, ricetta_letta.preparazione, ricetta_letta.complessita, &ricetta_letta.dosi);

			// Viene verificato il formato della prima estrazione
			if(esito_lettura != NUM_CAMPI_RICETTA){

				printf("Errore nella lettura della ricetta numero %d. Verrà ignorata.\n", num_ricette_lette);
				flag_inserimento = 0;

			}else if(esiste_ricetta(ricetta_letta.nome_ricetta) == 1){

				printf("Errore ricetta %d: Nome <%s> già in uso. Verrà ignorata.", num_ricette_lette, ricetta_letta.nome_ricetta);
				flag_inserimento = 0;

			}else if(strlen(ingredienti) == 0){
				printf("Errore ricetta %d: Ingredienti non presenti. Verrà ignorata.", num_ricette_lette);
				flag_inserimento = 0;
			}else{// Se il formato della ricetta era corretto vengono estratti gli ingredienti dalla stringa ingredienti

				while(num_ingredienti < MAX_INGREDIENTI){

					divisione_ingrediente = strdup(ingredienti);//duplico la riga che contiene gli ingredienti

					//se il prossimo ingrediente che estraggo non esiste termino di memorizzare gli ingredienti altrimenti estraggo i dati ingrediente
					if(leggi_campo_ricetta(divisione_ingrediente, num_ingredienti + 1, CAMPO_MULTIPLO) == NULL){
						break;

					}else{
						//duplico nuovamente la stringa contenente gli ingredienti in quanto il contenuto e' stato perso nella fase di check sopra
						divisione_ingrediente = strdup(ingredienti);

						//memorizzo nella stringa di lavoro l'ingrediente estratto dalla linea contenente tutti gli ingredienti
						strcpy(divisione_ingrediente, leggi_campo_ricetta(divisione_ingrediente,num_ingredienti + 1,CAMPO_MULTIPLO));

						//divido ulteriormente la stringa contenente l'ingrediente cosí da avere i singoli dati quali nome,quantita e relativa unitá di misura
						sscanf(divisione_ingrediente, "%f %[a-zA-Z] %[a-zA-Z]",
								&ricetta_letta.ingredienti[num_ingredienti].quantita, ricetta_letta.ingredienti[num_ingredienti].unita_misura , ricetta_letta.ingredienti[num_ingredienti].nome);


						// Abbassamento delle maiuscole negli ingredienti così da omologare il nomee l'unità di misura dell'ingrediente al contenuto del database
						// alimenti
						abbassa_maiuscole(ricetta_letta.ingredienti[num_ingredienti].nome);
						abbassa_maiuscole(ricetta_letta.ingredienti[num_ingredienti].unita_misura);

						// Conversione della quantità per omologarla al ontenuto del database alimenti
						converti_quantita(&ricetta_letta.ingredienti[num_ingredienti].quantita, ricetta_letta.ingredienti[num_ingredienti].unita_misura);


						if (controlla_unita_misura(ricetta_letta.ingredienti[num_ingredienti].unita_misura) == 1) {
							if(ricerca_alimento_database(ricetta_letta.ingredienti[num_ingredienti].nome, &dati_ingrediente) != 1){
								aggiorna_database(ricetta_letta.ingredienti[num_ingredienti].nome, ricetta_letta.ingredienti[num_ingredienti].unita_misura);
							}else if(strcmp(ricetta_letta.ingredienti[num_ingredienti].unita_misura, dati_ingrediente.unita_misura) != 0){
								puts("La ricetta <%s> ha degli ingredienti non validi per il database degli alimenti. Verrà ignorata.");
								flag_inserimento = 0;
							}
						}else{
							puts("La ricetta <%s> ha degli ingredienti non validi per il database degli alimenti. Verrà ignorata.");
							flag_inserimento = 0;
						}

						num_ingredienti++;
					}

				}

			}

			if(flag_inserimento == 1){
				alimento_database alimento;
				ricetta_letta.kcal_ricetta=0;

				for (int i=0; i<num_ingredienti; i++){
					ricerca_alimento_database(ricetta_letta.ingredienti[i].nome, &alimento);
					ricetta_letta.kcal_ricetta +=calcolo_kcal(alimento.kcal, alimento.campione_kcal, ricetta_letta.ingredienti[i].quantita);
				}

				aggiorna_database_ricette(ricetta_letta);// aggiorno il database, memorizzando la ricetta
			}else{
				num_ricette_scartate++;
			}
		}
	}

	fclose(stream);
	return num_ricette_lette - num_ricette_scartate;
}



int estrai_kcal_ricetta(char* nome_ricetta, int* kcal, int* dosi){
	FILE* stream = NULL;
	ricetta ricetta_letta;

	if(esiste_ricetta(nome_ricetta) == 0){
		return 0;
	}

	if((stream = fopen(FILE_DATABASE_RICETTE, "rb")) == NULL){
		return 0;
	}else{
		while(fread(&ricetta_letta, sizeof(ricetta), 1, stream) > 0){
			if(strcmp(ricetta_letta.nome_ricetta, nome_ricetta) == 0){
				*kcal = ricetta_letta.kcal_ricetta;
				*dosi= ricetta_letta.dosi;
				break;
			}
		}

		fclose(stream);
		return 1;
	}
}




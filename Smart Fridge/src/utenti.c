
#include "utenti.h"

/**
 * Funzione esiste_nickname ():
 *
 * La funzione ha il compito di verificare se il nickname passato è già presente nel database.
 *
 * A tal scopo acquisisce la lunghezza del nickname passato in ingresso, memorizzandola in una variabile, controllare
 * se tale lunghezza è idonea. Fatto ciò apre il file utnetni, legge ogni utente presente al suo interno fino a quando
 * non trova un utente con tale nickname, restituindo così 1.
 * nel caso non lo trovasse, il valore restituito sarebbe zero.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */

int esiste_nickname(const char* nickname){
	int lung_nickname = strlen(nickname);

	if(lung_nickname < MIN_LUNG_NICKNAME || lung_nickname > MAX_LUNG_NICKNAME - 1){
		return 0;
	}

	FILE* stream = NULL;
	utente u;

	if((stream = fopen(FILE_DATABASE_UTENTI, "rb")) == NULL){
		return 0;
	}else{
		while(feof(stream) == 0){

			//lettua di ogbi struct
			fread(&u, sizeof(utente), 1, stream);
			//se viene trovata una struct con nome corrispondente, termina ritornando 1
			if(strcmp(u.nickname, nickname) == 0){
				fclose(stream);
				return 1;
			}
		}
	}

	fclose(stream);
	return 0;
}


/**
 * Funzione input_nickname ():
 *
 * La funzione ha il compito di richiedere l'inserimento di un nickname destinato ad un nuovo utente.
 *
 * A tal scopo viene allorato una spazio di memoria per contenere il nickname inserito da tastiera da parte dell'utente.
 * Viene richiesto di inserire da tastiera il nickname fino a quanto non viene inserito un nickname che non è esistente.
 * La funzione ritorna il nickname.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito è un puntatore alla stringa contenente il nickname.
 *
 */
char* input_nickname(){
	char* nickname = (char*) calloc(MAX_LUNG_NICKNAME, sizeof(char));
	int esito_input;
	int esito_controllo;
	int lung_nickname;

	do{
		printf("Inserisci il nickname [max. 15 lettere, min. 3 lettere]:\n~");
		esito_input = scanf("%15[a-zA-Z]", nickname);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}
		//acquisizione della lunghezza effettiva del nickname inserito
		lung_nickname = strlen(nickname);

		//controllo se già esiste un utente con tale nickname
		esito_controllo = esiste_nickname(nickname);

		if(lung_nickname < MIN_LUNG_NICKNAME || lung_nickname > MAX_LUNG_NICKNAME - 1){
			esito_controllo = 0;
		}else{
			esito_controllo = 1;
		}

		if(esito_input != 1 || esito_controllo != 1){
			puts("Inserimento non valido. Ripeterlo.\n");
		}

	}while(esito_input != 1 || esito_controllo != 1);

	return nickname;
}

/**
 * Funzione genera_nickname ():
 *
 * La funzione ha il compito di  generare un nickname in modo casuale.
 *
 * A tal scopo, dopo aver allocato uno spazio di dimensione adatta al contenimento del nickname, posiziona il carattere
 * terminale alla fine. Ripete un ciclo che permette continua fino a quando il nickname non raggiunge una dimesione
 * accettata e/o il nickname esiste già. La generazione si serve del codice ASCII, generando random un numero corrispondente
 * ad un carattere minuscolo dell'alfabeto.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito è un puntatore alla stringa contenente il nickname.
 *
 */

char* genera_nickname(){
	int dim_nickname = rand() % (MAX_LUNG_NICKNAME - MIN_LUNG_NICKNAME + 1) + MIN_LUNG_NICKNAME;
	char* nickname = (char*) calloc(dim_nickname, sizeof(char));

	nickname[dim_nickname - 1] = '\0';

	do{
		for(int i = 0; i < dim_nickname - 1; i++){
			nickname[i] = rand() % (MAX_ASCII_MINUSCOLA - MIN_ASCII_MINUSCOLA + 1) + MIN_ASCII_MINUSCOLA;
		}
	}while(esiste_nickname(nickname) == 1);

	return nickname;
}

/**
 * Funzione input_password ():
 *
 * La funzione permette la richiesta diinserimento di una password.
 *
 * A tal scopo, dopo aver allocato uno spazio di dimensione adatta al contenimento della password, richiede l'inserimento
 * di quest'ultima da tastiera. Esegue vari controlli sulla dimensione e sul singolo input per verificare che abbia
 * i requisiti richiesti.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito è un puntatore alla stringa contenente la password.
 *
 */
char* input_password(){
	char* password = (char*) calloc(LUNG_PASSWORD, sizeof(char));
	int esito_input;
	int esito_controllo;
	int lung_password;

	do{
		printf("Inserire la password [8 caratteri]:\n~");
		esito_input = scanf("%8s", password);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		lung_password = strlen(password);

		if(lung_password != LUNG_PASSWORD - 1){
			esito_controllo = 0;
		}else{
			esito_controllo = 1;
		}

		if(esito_input != 1){
			puts("Inserimento non valido. Ripeterlo.\n");
		}else if(esito_controllo != 1){
			puts("Sono stati inseriti meno di 8 caratteri.\n"
					"La password deve necessariamente essere di 8 caratteri.\nRipetere l'inserimento.");
		}
	}while(esito_input != 1 || esito_controllo != 1);
	return password;
}


/**
 * Funzione genera_password ():
 *
 * La funzione permette la crezione randomica di una password.
 *
 * A tale scopo, viene generato un numero e cercato il corrispondente carattere sulla tabella ASCII.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito è un puntatore alla stringa contenente la password.
 *
 */
char* genera_password(){
	char* password = (char*) calloc(LUNG_PASSWORD, sizeof(char));
	char tipo_char;
	char generazione_char = 0;

	for(int i = 0; i < LUNG_PASSWORD - 1; i++){
		tipo_char = rand() % NUM_TIPI_CHAR + 1;

		switch(tipo_char){

		case CHAR_SIMBOLO:
			generazione_char = rand() % (MAX_ASCII_SIMBOLO - MIN_ASCII_SIMBOLO + 1) + MIN_ASCII_SIMBOLO;
			break;

		case CHAR_CIFRA:
			generazione_char = rand() % (MAX_ASCII_CIFRA - MIN_ASCII_CIFRA + 1) + MIN_ASCII_CIFRA;
			break;

		case CHAR_MAIUSCOLA:
			generazione_char = rand() % (MAX_ASCII_MAIUSCOLA - MIN_ASCII_MAIUSCOLA + 1) + MIN_ASCII_MAIUSCOLA;
			break;

		case CHAR_MINUSCOLA:
			generazione_char = rand() % (MAX_ASCII_MINUSCOLA - MIN_ASCII_MINUSCOLA + 1) + MIN_ASCII_MINUSCOLA;
			break;
		}

		password[i] = generazione_char;
	}

	password[LUNG_PASSWORD - 1] = '\0';

	return password;
}


/**
 * Funzione generatore_password ():
 *
 * La funzione permette la scelta tra generazione password automatica e l'inserimento manuale.
 *
 * A tal scopo, dopo aver allocato uno spazio di dimensione adatta al contenimento della password, richiede l'inserimento
 * di (1) per generare automaticamente una password, (2) per l'inserimento manuale.
 * Superati i controlli sull'input, uno switch effettua un controllo sulla risposta e chiama una funzione in base
 * ad essa.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito è un puntatore alla stringa contenente la password.
 *
 */
void generatore_password(char* password_utente){
	char* password_generata;
	int modalita_generazione = 0;
	int esito_input;
	short esito_controllo;

	puts("\nSelezionare la modalità di generazione della password.");

	do{
		printf("Inserire:\n 1 - Generazione automatica.\n 2 - Inserimento manuale.\n>");
		esito_input = scanf("%d", &modalita_generazione);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}
		//controllo sull'input
		if(modalita_generazione != GEN_AUTOMATICA && modalita_generazione != GEN_MANUALE){
			esito_controllo = 0;
		}else{
			esito_controllo = 1;
		}

		if(esito_input != 1 || esito_controllo != 1){
			puts("Inserimento non valido. Ripeterlo.\n");
		}

	}while(esito_input != 1 || esito_controllo != 1);

	//scelta della funzione necessaria in base all'input
	switch(modalita_generazione){
	case GEN_AUTOMATICA:
		password_generata = genera_password();
		break;

	case GEN_MANUALE:
		password_generata = input_password();
	}

	strcpy(password_utente, password_generata);
}


/**
 * Funzione input_preferenza ():
 *
 * La funzione permette l'input da tastiera da parte dell'utente delle sue preferenze alimentari (massimo 3 preferenze).
 *
 * A tal scopo, viene richiesto l'inserimento da tastiera, che se supera i controlli, viene memorizzato nella stringa
 * passata in ingresso alla funzione.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito identifica l'esito della funzione.
 */
int input_preferenza(char* preferenza){
	int esito_input;

	do{
		printf("Inserire il valore della preferenza (\"null\" per terminare l'inserimento) [max. 20 lettere minuscole]:\n~");
		esito_input = scanf("%20[^\n]", preferenza);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(esito_input != 1){
			puts("Inserimento non valido. Ripeterlo.");

		} //se l'utente non vuole più inserire preferenza, fine funzione
		else if(strcmp(preferenza, "null") == 0){
			return 0;
		}
	}while(esito_input != 1);

	return 1;
}


/**
 * Funzione input_utente ():
 *
 * La funzione è una funzione di base che richiama tutte le funzioni necessarie per riempire una struct di tipo utente.
 * Dopo aver creato una struct capace di contenere i dati richiesti.
 * Una strcpy copierà la stringa restituita dalla funzione posta come secondo argomento della stessa.
 * Una funzione permetterà la generazione di una password (scelta tra due modalità diverse).
 * Un for ripeterà la chiamata alla funzione per l'inserimento delle preferenze (alimentari), se la funzione resituisce 0,
 * automaticamente le preferenze rimanenti verranno rimpite attraverso un for con "null".
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito è una struct di tipo utente rimpita con dati significativi.
 *
 */
utente input_utente(){
	utente nuovo_utente;

	puts("DATI DEL NUOVO UTENTE:");

	strcpy(nuovo_utente.nickname, input_nickname());

	generatore_password(nuovo_utente.password);

	for(int i = 0; i < NUM_PREFERENZE; i++){
		int esito_input = input_preferenza(nuovo_utente.preferenze[i]);

		if(esito_input == 0){
			for(int j = i; j < NUM_PREFERENZE; j++){
				strcpy(nuovo_utente.preferenze[j], "null");
			}
			break;
		}
	}

	return nuovo_utente;
}


/**
 * Funzione genera_utente ():
 *
 * La funzione ha il compito di generare randomicamente un utente.
 *
 * A tale scopo, viene creata una struct capace di contenere i dati relativi all'utente.
 * Il nickname viene generato grazie una funzione genera_nickname() (anch'essa genera in modo randomico), grazie ad una
 * funzione che copia il valore restituito nella struct creata. Stessa cosa vale per la password, generata tramite
 * genera_password(). Un ciclo for permette il rimpimento delle aree di memoria riservate alle preferenze alimentari,
 * grazie alla funzione alimento_causuale. Se non può essere estratto nessun nome per la preferenza viene assegnato "null".
 * Le preferenze che seguono una contenete "null", verranno riempite con "null" anch'esse.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito è una struct di tipo utente rimpita con dati significativi.
 *
 */
utente genera_utente(){
	utente utente_generato; // Variabile che conterrà l'utente generato

	// Generazione del nickname
	strcpy(utente_generato.nickname, genera_nickname());

	// Generazione della password
	strcpy(utente_generato.password, genera_password());

	// Generazione delle preferenze dell'utente
	for(int i = 0; i < NUM_PREFERENZE; i++){
		int esito_estrazione;							// Variabile per memorizzare l'esito dell'estrazione
		int esito_controllo;							// Variabile per memorizzare gli esiti dei controlli
		char preferenza_casuale[LUNG_PREFERENZA];	// Stringa per contenere il nome estratto dal database

		// Estrazione casuale di un nome di un alimento
		esito_estrazione = alimento_casuale(preferenza_casuale);

		// Se non può essere estratto nessun nome, imposta tutte le preferenze alla stringa "null"
		if(esito_estrazione < 1){
			for(int j = i; i < NUM_PREFERENZE; j++){
				strcpy(utente_generato.preferenze[j], "");
			}
			break;
		}else{
			// Se possono essere letti gli alimenti, controlla che quello estratto
			// non sia già presente nelle preferenze precedenti dell'utente
			esito_controllo = 1;
			for(int j = 0; j < i; j++){
				if(strcmp(preferenza_casuale, utente_generato.preferenze[j]) == 0){
					esito_controllo = 0;
				}
			}

			// Se il nome estratto è già presente nelle preferenze, scrivi nella nuova preferenza "null"
			if(esito_controllo == 0){
				strcpy(utente_generato.preferenze[i], "null");
			}else{
				// Se il nome estratto non è già presente nelle preferenze dell'utente
				// controlla se una preferenza precedente a quella da inserire è uguale a "null"
				esito_controllo = -1;
				for(int j = 0; j < i; j++){
					if(strcmp(utente_generato.preferenze[j], "null") == 0){
						esito_controllo = j;
					}
				}

				// Se nessuna preferenza precedente è uguale a "null", scrivi la nuova preferenza
				if(esito_controllo == -1){
					strcpy(utente_generato.preferenze[i], preferenza_casuale);
				}else{
					// Altrimenti nella posizione del nuovo inseriemnto scrivi "null"
					// E nella posizione che contiene "null" scrivi la nuova preferenza
					strcpy(utente_generato.preferenze[i], "null");
					strcpy(utente_generato.preferenze[esito_controllo], preferenza_casuale);
				}
			}
		}
	}

	return utente_generato;
}


/**
 * Funzione genera_n_utenti ():
 *
 * La funzione ha il compito di generare randomicamente il numero degli utenti passato in ingresso alla funzione.
 *
 * A tale scopo, un for ripete la funzione genera_utente per n volte, riempend un vettore di struct che viene
 * restuituito dalla funzione.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito è un puntatore al vettore di struct di tipo utente.
 *
 */
utente* genera_n_utenti(int n){
	utente* utenti_generati = (utente*) calloc(n, sizeof(utente));

	for(int i = 0; i < n; i++){
		utenti_generati[i] = genera_utente();
	}

	return utenti_generati;
}


/**
 * Funzione salva_n_utenti ():
 *
 * La funzione ha il compito di scrivere su file il vettore di struct passatagli in ingresso, di dimensione n.
 *
 * A tale scopo, dopo aver superato gli opportuni controlli nell'apertura del database
 * contenente gli utenti, while legge tutti gli utenti gia prensenti nel database e se trova uno con nickname con
 * stringa vuota, lo sovrascrivere con una delle struct presenti nel vettore passato in ingresso.
 * Se non ci sono utenti con nickname vuoto, si procede alla scrittura su file in coda.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito è l'esito della funzione
 *
 */
int salva_n_utenti(utente* utenti, int n){
	FILE* stream;
	utente utente_letto;
	int flag_posizionamento;

	if((stream = fopen(FILE_DATABASE_UTENTI, "rb+")) == NULL){
		if((stream = fopen(FILE_DATABASE_UTENTI, "wb+")) == NULL){
			return 0;
		}
	}

	for(int i = 0; i < n; i++){
		flag_posizionamento = 0;
		fseek(stream, 0, SEEK_SET);

		//lettura di ogni utente già presente nel database
		while(feof(stream) == 0){
			fread(&utente_letto, sizeof(utente), 1, stream);

			//controllo che i valori letti siano effettivamente validi
			if(strcmp(utente_letto.nickname, "") == 0){

				//sovrascrittura se i valori non sono validi
				fseek(stream, -sizeof(utente), SEEK_CUR);
				fwrite(&utenti[i], sizeof(utente), 1, stream);

				//flag per indicare che la struct è stata memorizzata su file
				flag_posizionamento = 1;

				break;
			}
		}


		//se la struct non è stata memorizzata su file, viene scritta alla fine di esso
		if(flag_posizionamento == 0){
			fseek(stream, 0, SEEK_END);
			fwrite(&utenti[i], sizeof(utente), 1, stream);
		}
	}

	fclose(stream);
	return 1;
}


/**
 * Funzione crea_utenti ():
 *
 * La funzione ha il compito di permettere la creazione di un utente, tramite la scelta di due modalità: casuale o manuale.
 *
 * A tale scopo, viene creata una variabile destinata a contenere il numero degli utenti creabili su database
 * (n_utenti_creabili). Se tale variabile è minore o uguale a zero, vuol dire che non è possibile creare ulteriori utenti.
 * Se è possibile, viene richiesto l'inserimento di (1) per la generazione automatica, (2) per l'inserimento manuale.
 * In base alla scelta, vengono chiamte funzioni apposite. In caso di generazione automatica, viene richiesto
 * il numero degli utenti da generare.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito è l'esito della funzione
 *
 */
int crea_utenti(){
	int esito_input;
	int esito_controllo;
	int scelta;
	int n_utenti_creabili;
	unsigned short n_utenti = 0;
	utente* utenti_creati;

	// conteggio utenti creabili
	n_utenti_creabili = MAX_UTENTI - conta_utenti();

	//Messaggio di errore se sono gia stati creati il numero di utenti massimo
	if(n_utenti_creabili <= 0){
		puts("Non è possibile creare ulteriori utenti. Numero massimo di utenti raggiunto.");
		return 0;
	}

	do{
		esito_controllo = 1;

		printf("Selezionare la modalità di creazione di utente/i:\n1 - Automatica\n2 - Manuale\n~");
		esito_input = scanf("%d", &scelta);

		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(scelta != 1 && scelta != 2){
			esito_controllo = 0;
		}
		//Se non si inseriscono i valori richiesti
		if(esito_input != 1 || esito_controllo != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}

	}while(esito_input != 1 || esito_controllo != 1);

	//controllo della scelta
	switch(scelta){
	case GEN_AUTOMATICA:
		do{
			esito_controllo = 1;
			//Possibilità di creare più di un utente automaticamente
			printf("Inserire il numero di utenti da generare [max. %hu]:\n~", n_utenti_creabili);
			esito_input = scanf("%hu", &n_utenti);
			if(pulisci_stdin() == 1){
				esito_input = 0;
			}

			if(n_utenti > n_utenti_creabili){
				esito_controllo = 0;
			}

			if(esito_input != 1 || esito_controllo != 1){
				puts("Inserimento non valido. Ripeterlo.");
			}

		}while(esito_input != 1 || esito_controllo != 1);

		//Allocazione della memoria per contenere gli utenti richiesti da generare
		utenti_creati = (utente*) calloc(n_utenti, sizeof(utente));

		utenti_creati = genera_n_utenti(n_utenti);

		break;

	case GEN_MANUALE:
		n_utenti = 1;
		//allocazione della memoria per 1 unico utente
		utenti_creati = (utente*) calloc(n_utenti, sizeof(utente));

		*utenti_creati = input_utente();

		break;
	}
	//salvataggio degli utenti creati su file
	if(salva_n_utenti(utenti_creati, n_utenti) == 0){
		return -1;
	}

	return 1;
}

/**
 * Funzione modifica_preferenze ():
 *
 * La funzione ha il compito di permettere la modifica delle preferenze di un utente.
 *
 * A tale scopo, viene passata la struct contenente i dati dell'utente, vengono controllate le preferenze
 * effettivamente piene e se ci sono quindi preferenze vuote.
 * Nel caso di presenza di preferenze vuote, viene richiesto all'utente se si desidere aggiungere o modficare
 * una preferenza, in base alla scelte, di proseguirà alla modifica o della preferenza scelta o della prima prefereza
 * risultante vuota.
 * Se non vengono individuate preferenze vuote, si procede alla richiesta dell'inserimento del numero della preferenza
 * che si desidera modificare.
 *
 * @pre La struct in ingresso non può essere vuota.
 *
 */
void modifica_preferenze(utente* u){
	int num_preferenze_utente;
	int scelta;
	int esito_input;
	int esito_controllo;

	puts("");

	//conteggio delle prefenze contenenti dati significativi
	num_preferenze_utente = output_preferenze(*u);

	//Se esistono preferenze con valore "null"
	if(num_preferenze_utente > 0 && num_preferenze_utente < NUM_PREFERENZE){
		do{
			printf("Inserire [1] per modificare una preferenza esistente, [2] per aggiungerne una nuova:\n~");
			esito_input = scanf("%d", &scelta);
			if(pulisci_stdin() == 1){
				esito_input = 0;
			}

			if(scelta != 1 && scelta != 2){
				esito_controllo = 0;
			}else{
				esito_controllo = 1;
			}

			if(esito_input != 1 || esito_controllo != 1){
				puts("Inserimento non valido. Ripeterlo.\n");
			}
		}while(esito_input != 1 || esito_controllo != 1);
	}else if(num_preferenze_utente == NUM_PREFERENZE){
		scelta = 1;
	}else{
		scelta = 2;
	}

	if(scelta == 2){
		puts("Aggiunta di una nuova preferenza:");
		input_preferenza((*u).preferenze[num_preferenze_utente]);
	}else{
		do{
			printf("Inserire il numero della preferenza da modificare:\n~");
			esito_input = scanf("%d", &scelta);
			if(pulisci_stdin() == 1){
				esito_input = 0;
			}

			if(scelta < 1 || scelta > num_preferenze_utente){
				esito_controllo = 0;
			}else{
				esito_controllo = 1;
			}

			if(esito_input != 1 || esito_controllo != 1){
				puts("Inserimento non valido. Ripeterlo");
			}
		}while(esito_input != 1 || esito_controllo != 1);

		input_preferenza((*u).preferenze[scelta-1]);
	}
}


/**
 * Funzione gestore_modifiche ():
 *
 * La funzione ha il compito di gestire qualsiasi modifica l'utente voglia effettuare sui suoi dati
 *
 * A tale scopo, viene richiesto chiamata una funzione (autenticazione()) per la verifica dei diritti alla modifiche
 * Controllato l'esito di tale funzione, se positovo, viene richiesto quale campo si desider modificare.
 * In base alla scelta, vengono chiamate funzioni oppurtune.
 * Fatte le modifiche, viene salvato su file.
 *
 * @pre
 * @post Il valoredi ritorno rappresental'esito della funzione.
 *
 */
int gestore_modifiche(){
	FILE* stream = NULL;
	utente utente_letto;
	utente utente_modificato;
	int esito_input;
	int esito_controllo;
	int scelta;

	if((stream = fopen(FILE_DATABASE_UTENTI, "rb+")) == NULL){
		return 0;
	}else{
		printf("Effettuare l'accesso ad un utente per modificarlo:\n");
		esito_controllo = autenticazione(&utente_modificato);

		//se l'autenticazione non va a buon fine
		if(esito_controllo != 1){
			puts("Annullamento modifica.");

			fclose(stream);
			return 0;
		}

		do{ //Posizonamento puntatore alla struct corrispondente a quella da modificare
			fread(&utente_letto, sizeof(utente), 1, stream);
		}while(strcmp(utente_letto.nickname, utente_modificato.nickname) != 0);

		do{
			//stampa utente su schermo
			output_utente(utente_modificato);
			//riposizionamento del puntatore all'inzio della struct prelevata
			fseek(stream, -sizeof(utente), SEEK_CUR);

			do{
				puts("\nSelezionare il campo da modificare:");
				printf("1 - Nickname\n2 - Password\n3 - Preferenze\n0 - Esci\n\n~");
				esito_input = scanf("%d", &scelta);
				if(pulisci_stdin() == 1){
					esito_input = 0;
				}

				if(scelta != CAMPO_NICKNAME && scelta != CAMPO_PASSWORD && scelta != CAMPO_PREFERENZE && scelta != 0){
					esito_controllo = 0;
				}else{
					esito_controllo = 1;
				}

				if(esito_input != 1 || esito_controllo != 1){
					puts("Inserimento non valido. Ripeterlo.");
				}
			}while(esito_input != 1);

			//Controllo della scelta e richiamo alle funzioni di modifca in base ad essa
			switch(scelta){
			case CAMPO_NICKNAME:
				strcpy(utente_modificato.nickname, input_nickname());
				break;
			case CAMPO_PASSWORD:
				strcpy(utente_modificato.password, input_password());
				break;
			case CAMPO_PREFERENZE:
				modifica_preferenze(&utente_modificato);
				break;
			default:
				break;
			}

			//scrittura su file

			fwrite(&utente_modificato, sizeof(utente), 1, stream);

		}while(scelta != 0);

		fclose(stream);
		return 1;
	}
}


/**
 * Funzione visualizza_database_utenti ():
 *
 * La funzione ha il compito di visualizzare su schermo tutti gli utenti memorizzati su file.
 *
 * A tale scopo, un if controlla se ci sono effettivamente utenti registrati grazie una funzione conta_utenti.
 * Se esistono, vengono letti uno per volta e passati ad una funzione di stampa (output_utente()).
 *
 * @post Il valore di ritorno rappresental'esito della funzione.
 *
 */
int visualizza_database_utenti(){
	FILE* stream;
	utente u;

	if((stream = fopen(FILE_DATABASE_UTENTI, "rb")) == NULL){
		puts("Non è possibile aprire il file 'database_utenti.dat'.");
		return 0;
	}else{
		puts("DATABASE UTENTI:");

		//conteggio utenti presenti su file
		if(conta_utenti() > 0){
			while(fread(&u, sizeof(utente), 1, stream)){
				if(feof(stream) != 0){
					break;
				}
				//se l'utente contine valori significativi, stampa su schermo
				if(strcmp(u.nickname, "") != 0){
					output_utente(u);
				}
			}
		}else{
			puts("Non esistono utenti.");
		}
	}

	fclose(stream);
	return 1;
}



/**
 * Funzione autenticazione ():
 *
 * La funzione ha il compito di verificare se l'utente abbia i diritti di accesso
 *
 * A tale scopo, viene richiesto l'input del nickname, se questo esiste e corrisponde ad un utente, viene richiesta
 * la password di tale utente. Se quella inserita corrisponde effettivamente alla password dell'utente, la funzione
 * restituisce (1), altrimenti (0). Se la password è sbagliata, viene chiesto all'utente se desidera riprovare per un
 * massimo di volte stabilito.
 *
 *@pre non sono presenti pre requisiti.
 * @post Il valore di ritorno rappresental'esito della funzione.
 *
 */
int autenticazione(utente* u){
	FILE* stream = NULL;				// Variabile puntatore a FILE_DATABASE_UTENTI
	utente utente_letto;				// Variabile utilizzata per memorizzare l'utente letto dal file
	int esito_input;					// Variabile utilizzata per memorizzare l'esito di un input
	int esito_controllo;				// Variabile utilizzata per memorizzare l'esito di un controllo su di un input
	int scelta;							// Variabile per memorizzare la scelta inserita dall'utente
	char nickname[MAX_LUNG_NICKNAME];	// Stringa corrispondente al nickname inserito per effettuare l'accesso
	char password[LUNG_PASSWORD];		// Stringa corrispondete alla password inserita per effettuare l'accesso

	// Apertura di FILE_DATABASE_UTENTI in lettura binaria
	if((stream = fopen(FILE_DATABASE_UTENTI, "rb")) == NULL){
		return -1; // Se il file non può essere aperto viene ritornato -1
	}else{
		// Richiesta dei dati di accesso
		do{
			// Richiesta input nickname
			strcpy(nickname, input_nickname());
			// Controllo sull'esistenza nel database di un utente con il nickname inserito
			esito_controllo = esiste_nickname(nickname);

			// Richiesta input password
			strcpy(password, input_password());

			// Messaggio di errore nel caso in cui il nickname non sia valido
			if(esito_controllo != 1){
				puts("Il nickname inserieto non appartiene a nessun utente.");
			}else{
				fseek(stream, 0, SEEK_SET);
				// Se il nickname è valido, va cercato l'utente identificato dallo stesso
				do{

					fread(&utente_letto, sizeof(utente), 1, stream);

				}while(strcmp(utente_letto.nickname, nickname) != 0);

				// Confronto fra la password inserita per accedere e quella dell'account memorizzato
				if(strcmp(utente_letto.password, password) != 0){
					// Se non coincidono viene mostrato l'errore
					puts("La password inserita non è corretta.");
					esito_controllo = 0;
				}else{
					esito_controllo = 1;
				}
			}

			// Se i dati dell'accesso non sono validi, viene richiesto se ripetere l'accesso o annullarlo
			if(esito_controllo != 1){

				do{
					printf("Inserire [1] per ripetere l'accesso, [0] per annullare:\n~");
					esito_input = scanf("%d", &scelta);
					if(pulisci_stdin() == 1){
						esito_input = 0;
					}

					if(esito_input == 1){
						if(scelta != 1 && scelta != 0){
							puts("Inserimento non valido. Ripeterlo.");
							esito_input = 0;
						}
					}else{
						puts("Inserimento non valido. Ripeterlo.");
					}
				}while(esito_input != 1);

			}else{
				// Se l'accesso avviene correttamente, impostiamo scelta a zero così da uscire dal while
				scelta = 0;
			}

		}while(scelta != 0);

		// Se l'accesso è avvenuto, esito_controllo deve essere pari a 1
		if(esito_controllo == 1){
			// Memorizzazione dei dati dell'utente autenticato
			(*u) = utente_letto;

			fclose(stream);
			return 1;
		}else{
			fclose(stream);
			return 0;
		}
	}
}


/**
 * Funzione output_preferenze ():
 *
 * La funzione ha il compito di stampare su schermo la preferenze appartenenti
 * alla struct  di tipo utente passata in ingresso.
 *
 * A tale scopo, un for ripete un ciclo che stampa le preferenze se diverse da "null".
 * Se non sono presenti preferenze conteneti valori diversi da "null", viene stampato un messaggio di
 * assenza delle preferenze.
 *
 *@pre struct in ingresso non vuota
 * @post Il valore di ritorno è il numero delle preferenze con valori significativi.
 *
 */
int output_preferenze(const utente u){
	int num_preferenze = 0;

	puts("Preferenze:");

	for(int i = 0; i < NUM_PREFERENZE; i++){
		if(strcmp(u.preferenze[i], "null") != 0){
			num_preferenze++;
			printf("%d - %s\n", num_preferenze, u.preferenze[i]);
		}
	}

	if(num_preferenze == 0){
		puts("Non ci sono preferenze alimentari.");
	}

	return num_preferenze;
}



/**
 * Funzione output_utente ():
 *
 * La funzione ha il compito di stampare su schermo la struct passata in ingresso.
 *
 *
 * @pre nla struct non deve essere vuota
 *
 */
void output_utente(const utente u){
	printf("Nickname: %s\nPassword: %s\n",
			u.nickname, u.password);

	output_preferenze(u);
}

/**
 * Funzione conta_utenti ():
 *
 * La funzione ha il compito di contare il numero degli utenti presenti su file.
 *
 * A tale scopo, viene aperto il file utenti, letto ciascuna delle struct presenti, se la struct contiene
 * valori signficativi, un contatore viene incrementato.
 *
 *
 * @post Il valore di ritorno rappresenta il numero degli utenti presenti su file.
 *
 */
int conta_utenti(){
	utente u;
	int num_utenti = 0;
	FILE* stream = NULL;

	if((stream = fopen(FILE_DATABASE_UTENTI, "rb")) == NULL){
		return num_utenti;
	}else{
		while(fread(&u, sizeof(utente), 1, stream)){
			if(feof(stream) != 0){
				break;
			}

			if(strcmp(u.nickname, "") != 0){
				num_utenti++;
			}
		}
	}

	fclose(stream);
	return num_utenti;

}


/**
 * Funzione elimina_utente ():
 *
 * La funzione ha il compito eliminare l'utente da file.
 *
 * A tale scopo, viene richiesto di effettuare l'accesso, se esso va a buon fine ed esistono quindi utenti su file,
 * il nickname dell'utente viene sostituito da una stringa vuota.
 * Si chiede conferma per la riscrittura su file, se positiva, la struct viene salvata con le modifiche.
 *
 * @post Il valore di ritorno rappresenta l'esito della funzione
 *
 */
int elimina_utente(){
	FILE* stream = NULL;
	utente utente_eliminare;
	utente utente_letto;
	int scelta;
	int esito_input;
	int esito_controllo;

	//se non ci sono utenti o il file non esiste, stampa messaggio di errore

	if((stream = fopen(FILE_DATABASE_UTENTI,"rb+")) == NULL || conta_utenti() == 0){
		puts("Non esistono utenti eliminabili.");
		return -1;
	}else{
		puts("Effettuare l'accesso all'utente da eliminare:");

		//effettuo accesso
		esito_controllo = autenticazione(&utente_eliminare);
		//Se l'accesso non va a buon fine, termina funzione
		if(esito_controllo == 0){
			puts("Annullamento eliminazione utente.");

			fclose(stream);
			return 0;
		}else{

			//Viene letto da file l'utente corrispondente a quello con cui è stata effettuata l'autenticazione
			do{
				fread(&utente_letto, sizeof(utente), 1, stream);
			}while(strcmp(utente_letto.nickname, utente_eliminare.nickname) != 0);

			//riposizionamento del puntatore ad inizio struct letta
			fseek(stream, -sizeof(utente), SEEK_CUR);

			do{

				printf("Inserire [1] per confermare l'eliminazione, [0] per annullarla:\n~");
				esito_input = scanf("%d", &scelta);
				if(pulisci_stdin() == 1){
					esito_input = 0;
				}

				if(scelta != 1 && scelta != 0){
					esito_controllo = 0;
				}else{
					esito_controllo =  1;
				}

				if(esito_input != 1 || esito_controllo != 1){
					puts("Inserimento non valido. Ripeterlo.");
				}
			}while(esito_input != 1 || esito_controllo != 1);

			//sostituzione nickname con stringa vuota
			if(scelta == 1){
				strcpy(utente_eliminare.nickname, "");
				fwrite(&utente_eliminare, sizeof(utente), 1, stream);

				puts("Eliminazione utente effettuata.");
				fclose(stream);


				char file_menu_settimanale[LUNG_NOME_FILE_MENU];
				strcpy(file_menu_settimanale, FILE_MENU);
				strcat(file_menu_settimanale, utente_eliminare.nickname);
				elimina_file_menu(file_menu_settimanale);


				return 1;
			}else{
				puts("Annullamento eliminazione utente");
				fclose(stream);
				return 0;
			}
		}
	}
}

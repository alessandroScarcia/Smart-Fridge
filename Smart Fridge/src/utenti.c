/*
 * utenti.c
 *
 *  Created on: 10 mag 2018
 *      Author: Michela
 */

#include "utenti.h"


int esiste_nickname(char* nickname){
	FILE* stream = NULL;
	utente u;

	if((stream = fopen(FILE_DATABASE_UTENTI, "rb")) == NULL){
		return 0;
	}else{
		while(feof(stream) == 0){
			fread(&u, sizeof(utente), 1, stream);

			if(strcmp(u.nickname, nickname) == 0){
				fclose(stream);
				return 1;
			}
		}
	}

	fclose(stream);
	return 0;
}


char* input_nuovo_nickname(){
	char* nickname = (char*) calloc(MAX_LUNG_NICKNAME, sizeof(char));
	int esito_input;
	int esito_controllo;

	do{

		printf("Inserisci un nickname [max. 15 lettere, min. 5 lettere]:\n>");
		esito_input = scanf("%15[a-zA-Z]", nickname);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		esito_controllo = esiste_nickname(nickname);

		if(esito_input != 1){
			puts("Inserimento non valido. Ripeterlo.\n");
		}else if(esito_controllo == 1){
			puts("Il nickname inserito non è disponibile. Sceglierne un altro.");
		}

	}while(esito_input != 1 || esito_controllo == 1);

	return nickname;
}


char* input_nickname(){
	char* nickname = (char*) calloc(MAX_LUNG_NICKNAME, sizeof(char));
	int esito_input;

	do{
		printf("Inserisci il nickname [max. 15 lettere, min. 5 lettere]:\n>");
		esito_input = scanf("%15[a-zA-Z]", nickname);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(esito_input != 1){
			puts("Inserimento non valido. Ripeterlo.\n");
		}

	}while(esito_input != 1);

	return nickname;
}


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


char* input_password(){
	char* password = (char*) calloc(LUNG_PASSWORD, sizeof(char));
	int esito_input;

	do{
		printf("Inserire la password [max. 8 caratteri]:\n>");
		esito_input = scanf("%8s", password);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(esito_input != 1){
			puts("Inserimento non valido. Ripeterlo.\n");
		}
	}while(esito_input != 1);
	return password;
}


char* genera_password(){
	char* password = (char*) calloc(LUNG_PASSWORD, sizeof(char));
	char tipo_char;
	char generazione_char;

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


/*Sommario: Funzione base per la creazione della password, con la possibilità di scelta tra la generazione casuale
            e l'inserimento manuale da parte dell'utente esterno, il tutto attraverso la chiamata di fuzioni oppurtune.
Parametri: puntatore di tipo utente, dove sono memorizzati gli offset dell'utente, quest'ultimo verrà passato alle funzioni chiamate.
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

		if(modalita_generazione != GEN_AUTOMATICA && modalita_generazione != GEN_MANUALE){
			esito_controllo = 0;
		}else{
			esito_controllo = 1;
		}

		if(esito_input != 1 || esito_controllo != 1){
			puts("Inserimento non valido. Ripeterlo.\n");
		}

	}while(esito_input != 1 || esito_controllo != 1);

	switch(modalita_generazione){
	case GEN_AUTOMATICA:
		password_generata = genera_password();
		break;

	case GEN_MANUALE:
		password_generata = input_password();
	}

	strcpy(password_utente, password_generata);
}


int input_preferenza(char* preferenza){
	int esito_input;

	do{
		printf("Inserire il valore della preferenza (\"null\" per terminare l'inserimento) [max. 20 lettere minuscole]:\n>");
		esito_input = scanf("%20[a-z]", preferenza);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(esito_input != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}else if(strcmp(preferenza, "null") == 0){
			return 0;
		}
	}while(esito_input != 1);

	return 1;
}


/*
Sommario: Creazione di una variabile di tipo utente, che verrà riempita attraverso input da tastiera.
          La funzione chiamerà a sua volta un'altra funzione, che eseguirà altre specifiche spiegate in seguito
Parametri: i rappresenta n-esimo utente registrato, quindi l'ultimo indice assegnato.
 */
utente input_utente(){
	utente nuovo_utente;

	puts("DATI DEL NUOVO UTENTE:");

	strcpy(nuovo_utente.nickname, input_nuovo_nickname());

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
		char preferenza_casuale[LUNG_NOME_ALIMENTO];	// Stringa per contenere il nome estratto dal database

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


utente* genera_n_utenti(int n){
	utente* utenti_generati = (utente*) calloc(n, sizeof(utente));

	for(int i = 0; i < n; i++){
		utenti_generati[i] = genera_utente();
	}

	return utenti_generati;
}


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

		while(feof(stream) == 0){
			fread(&utente_letto, sizeof(utente), 1, stream);

			if(strcmp(utente_letto.nickname, "") == 0){
				fseek(stream, -sizeof(utente), SEEK_CUR);
				fwrite(&utenti[i], sizeof(utente), 1, stream);

				flag_posizionamento = 1;

				break;
			}
		}

		if(flag_posizionamento == 0){
			fseek(stream, 0, SEEK_END);
			fwrite(&utenti[i], sizeof(utente), 1, stream);
		}
	}

	fclose(stream);
	return 1;
}


int crea_utenti(){
	int esito_input;
	int esito_controllo;
	int scelta;
	int n_utenti_creabili;
	unsigned short n_utenti = 0;
	utente* utenti_creati;

	n_utenti_creabili = MAX_UTENTI - conta_utenti();

	if(n_utenti_creabili <= 0){
		puts("Non è possibile creare ulteriori utenti. Numero massimo di utenti raggiunto.");
		return 0;
	}

	do{
		esito_controllo = 1;

		printf("Selezionare la modalità di creazione di utente/i:\n1 - Automatica\n2 - Manuale\n>");
		esito_input = scanf("%d", &scelta);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(scelta != 1 && scelta != 2){
			esito_controllo = 0;
		}

		if(esito_input != 1 || esito_controllo != 1){
			puts("Inserimento non valido. Ripeterlo");
		}

	}while(esito_input != 1 || esito_controllo != 1);

	switch(scelta){
	case GEN_AUTOMATICA:
		do{
			esito_controllo = 1;

			printf("Inserire il numero di utenti da generare [max. %hu]:\n>", n_utenti_creabili);
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

		utenti_creati = (utente*) calloc(n_utenti, sizeof(utente));

		utenti_creati = genera_n_utenti(n_utenti);

		break;

	case GEN_MANUALE:
		n_utenti = 1;

		utenti_creati = (utente*) calloc(n_utenti, sizeof(utente));

		*utenti_creati = input_utente();

		break;
	}

	if(salva_n_utenti(utenti_creati, n_utenti) == 0){
		return -1;
	}

	return 1;
}


void modifica_preferenze(utente* u){
	int num_preferenze_utente;
	int scelta;
	int esito_input;
	int esito_controllo;

	puts("");
	num_preferenze_utente = output_preferenze(*u);

	if(num_preferenze_utente > 0 && num_preferenze_utente < NUM_PREFERENZE){
		do{
			printf("Inserire [1] per modificare una preferenza esistente, [2] per aggiungerne una nuova:\n>");
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
			printf("Inserire il numero della preferenza da modificare:\n>");
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

		if(esito_controllo != 1){
			puts("Annullamento modifica.");

			fclose(stream);
			return 0;
		}

		do{
			fread(&utente_letto, sizeof(utente), 1, stream);
		}while(strcmp(utente_letto.nickname, utente_modificato.nickname) != 0);

		do{
			output_utente(utente_modificato);

			fseek(stream, -sizeof(utente), SEEK_CUR);

			do{
				puts("\nSelezionare il campo da modificare:");
				printf("1 - Nickname\n2 - Password\n3 - Preferenze\n0 - Esci\n\n>");
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

			switch(scelta){
			case CAMPO_NICKNAME:
				strcpy(utente_modificato.nickname, input_nuovo_nickname());
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

			fwrite(&utente_modificato, sizeof(utente), 1, stream);

		}while(scelta != 0);

		fclose(stream);
		return 1;
	}
}


int visualizza_database_utenti(){
	FILE* stream;
	utente u;

	if((stream = fopen(FILE_DATABASE_UTENTI, "rb")) == NULL){
		return 0;
	}else{
		puts("DATABASE UTENTI:");
		if(conta_utenti() > 0){
			while(fread(&u, sizeof(utente), 1, stream)){
				if(feof(stream) != 0){
					break;
				}

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
 *
 * @pre
 *
 * @post
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
				// Se il nickname è valido, va cercato l'utente identificato dallo stesso
				do{
					fread(&utente_letto, sizeof(utente), 1, stream);
				}while(strcmp(utente_letto.nickname, nickname) != 0);

				// Confronto fra la password inserita per accedere e quella dell'account memorizzato
				if(strcmp(utente_letto.password, password) != 0){
					// Se non coincidono viene mostrato l'errore
					puts("La password inserita non è corretta.");
					esito_controllo = 0;
				}
			}

			// Se i dati dell'accesso non sono validi, viene richiesto se ripetere l'accesso o annullarlo
			if(esito_controllo != 1){
				do{
					printf("Inserire [1] per ripetere l'accesso, [0] per annullare:\n>");
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
 *
 *
 */
void output_utente(const utente u){
	printf("Nickname: %s\nPassword: %s\n",
			u.nickname, u.password);

	output_preferenze(u);
}


/*Sommario: Funzione che stampa le varie operazione che si possono effettuare, leggerà la risposa inserita da tastiera ed in base al
 * numero inserito, verrà chiamata una funzione opportuna;
paramentri: i è un indice da passare che servirà per il salvataggio dell'utente, corrisponde all'ultimo indice assegnato ad un utente
 */
int menu_database_utenti (int i){

	int response;

	do {
		printf("\n- Inserire [1] per la creazione di un nuovo profilo;");
		printf("\n- Inserire [2] per la modifica del nome di un profilo esistente;");
		printf("\n- Inserire [3] per la modifica del cognome di un profilo esistente;");
		printf("\n- Inserire [4] per la modifica delle preferenze alimentari di un profilo esistente;");
		printf("\n- Inserire [5] per la modifica della password di un profilo esistente;");
		printf("\n- Inserire [6] per la visualizzazione di un profilo esistente;");
		printf("\n- Inserire [7] per la visualizzazione di tutti gli utenti;");
		printf("\n- Inserire [0] per terminare programma.\n");


		scanf("%d", &response);

		switch(response) {

		case 1:
			input_utente(i);
			i++;
			break;
		case 2:
			//modifica_nome();
			break;
		case 3:
			//modifica_cognome();
			break;
		case 4:
			//modifica_preferenza();
			break;
		case 5:
			//modifica_password();
			break;
		case 6:
			//output_utente();
			break;
		case 7:
			visualizza_database_utenti();
			break;

		}

	} while (response!=0);

	return 0;
}


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

int elimina_utente(){
	FILE* stream = NULL;
	utente utente_eliminare;
	utente utente_letto;
	int scelta;
	int esito_input;
	int esito_controllo;

	if((stream = fopen(FILE_DATABASE_UTENTI,"rb+")) == NULL || conta_utenti() == 0){
		puts("Non esistono utenti eliminabili.");
		return -1;
	}else{
		puts("Effettuare l'accesso all'utente da eliminare:");
		esito_controllo = autenticazione(&utente_eliminare);

		if(esito_controllo == 0){
			puts("Annullamento eliminazione utente.");

			fclose(stream);
			return 0;
		}else{
			do{
				fread(&utente_letto, sizeof(utente), 1, stream);
			}while(strcmp(utente_letto.nickname, utente_eliminare.nickname) != 0);

			fseek(stream, -sizeof(utente), SEEK_CUR);

			do{
				printf("Inserire [1] per confermare l'eliminazione, [0] per annullarla:\n>");
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

			if(scelta == 1){
				strcpy(utente_eliminare.nickname, "");
				fwrite(&utente_eliminare, sizeof(utente), 1, stream);

				puts("Eliminazione utente effettuata.");
				fclose(stream);
				return 1;
			}else{
				puts("Annullamento eliminazione utente");
				fclose(stream);
				return 0;
			}
		}
	}
}

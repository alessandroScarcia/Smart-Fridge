
#include "calorie.h"


/**
 * Funzione input_nome_assunzione()
 *
 * La funzione ha il compito di richiedere l'input all'utente del nome di un assunzione.
 * Il valore inserito deve esere una stringa, di massimo 20 caratteri. Se non viene rispettato
 * questo limite l'inserimento è considerato non valido.
 *
 * L'input viene iterato fino a quanndo il valore inserito non è valido.
 *
 * @pre Non ci sono pre condizioni
 *
 * @post Il valore di ritorno deve essere ricevuto in una variabile con adeguato tipo
 */
char* input_nome_assunzione(){
	// variabile per memorizzare l'esito dell'input
	int esito_input;
	// variabile nel quale ricevere la stringa inserita dall'utente
	char* nome_assunzione = (char *) calloc(LUNG_NOME_ASSUNZIONE, sizeof(char));

	do{
		printf("Inserire il nome dell'assunzione[max. 20 caratteri]:\n~");
		esito_input = scanf("%20s", nome_assunzione);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(esito_input != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}
	}while(esito_input != 1);

	return nome_assunzione;
}

/**
 * Funzione input_kcal_libere()
 *
 * La funzione ha il compito di richiedere l'input all'utente delle calorie
 * relative ad un assunzione libera. Tale inserimento deve essere un valore short int,
 * e deve essere rispettare i limiti di MIN_KCAL e MAX_KCAL.
 * L'input viene iterato fino a quanndo il valore inserito non è valido.
 *
 * @pre Non ci sono pre condizioni
 *
 * @post Il valore di ritorno deve essere ricevuto in una variabile con adeguato tipo
 */
unsigned short int input_kcal_libere(){
	unsigned short int kcal; // variabile nellaquale ricevere l'input dell'utente
	int esito_input; // variabileper memorizzare l'esito dell'input
	int esito_controllo; // variabile per memorizzare l'esito del controllo sul valore

	do{
		printf("Inserire le calorie dell'assunzione:\n~");
		esito_input = scanf("%hu", &kcal);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(kcal < MIN_KCAL || kcal > MAX_KCAL){
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

/**
 * Funzione input_alimento_consumato ():
 *
 * La funzione ha il compito di prendere in input da tastiera un alimento/ricetta non presente nel database, per memorizzarlo
 * nel file delle calorie assunte dell'utente.
 *
 * A tal scopo esegue la funzione auteticazzione, per verificare che l'utente abbia i diritti di accesso, in caso contrario
 * termina. Verificato ciò, il flag viene impostato automaticamente con una costante che indica si tratti di un alimento/ricetta
 * non presente nei database. La quantità, per le stesse ragioni, viene impostata a 0. Viene richiesto, inoltre, di inserire
 * le calorie in quanto non è possibile calcolarle in quanto non si hann abbastanza informazioni relative a ciò che ha inserito.
 * Opportuni controlli verificheranno che l'input sia corretto.
 * Fatto ciò, verrà chiamata una funzione di scrittura diretta su file.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */
int input_alimento_consumato() {

	utente persona; // Variabile utilizzata per effettuare l'autenticazione dell'utente

	// Autenticazione dell'utente
	if (autenticazione(&persona)==1){

		assunzione cibo; // variabile utilizzata per scrivere la nuova assunzione

		cibo.flag=FLAG_INPUT_LIBERO; //Valore che identifica un alimento/ricetta non presente nei database dello smart fridge

		cibo.quantita=0; //Quantità non necessaria

        strcpy(cibo.nome, input_nome_assunzione());

        do{
        	cibo.kcal = input_kcal_libere();

        	if(cibo.kcal == 0){
        		puts("Inserimento non valido. Ripeterlo.");
        	}
        }while(cibo.kcal == 0);

		scrittura_diretta_assunzione (&cibo, persona.nickname);

		return 1;
	}

	return 0;

}


/**
 * Funzione calcolo_kcal ():
 *
 * La funzione ha il compito di calcolare tramite una proporzione le calorie assunte sulla base delle quantità passategli
 * in ingresso.
 *
 * A tal scopo utilizzera le variabile di ingresso kcal e campione, dove kcal sta per il valore calorico del campione ricevuto
 * e la quantità.
 *
 * @pre Il campione non può essere pari a 0, kcal non può essere negativo
 * @post Il valore restituito rappresenta l'apporto calorico calcolato.
 *
 */

unsigned short int calcolo_kcal(unsigned short int kcal, int campione, float q_consumata) {
	unsigned short int risultato_kcal;

	// Controlli sui valori ricevuti
	if(kcal < MIN_KCAL_CAMPIONE || kcal > MAX_KCAL_CAMPIONE){
		return 0;
	}

	if(campione != CAMPIONE_G
		&& campione != CAMPIONE_PZ
		&& campione != CAMPIONE_ML){
		return 0;
	}

	if (q_consumata==0){
		return 0; //se la quantità è nulla, ritorno zero
	}

	// Calcolo delle calorie
	risultato_kcal = (q_consumata * kcal) / campione;

	return risultato_kcal;

}

/**
 * Funzione aggiorno_database_calorie ():
 *
 * La funzione ha il compito di creare una struct di tipo assunzioni e rimpirla con dati significativi, sulla base dei
 * dati passati in ingresso alla stessa.
 *
 * A tal scopo, dopo aver creato la struct di tipo assunzioni denominata cibo, copierà il nome_consumo passato in ingresso
 * nel campo della struct denominato nome. Imposterà il flag con quello passatogli in ingresso e farà altrettanto con la quantità.
 * Fatto ciò, controllerà il flag. Se il flag identifica una ricetta, verrano create due variabile di tipo int (kcal_ricetta e
 * dosi_ricetta) utilizzate in combinazione con una funzione in moda da poter estrarre da un database di ricette
 * le kcal per un tot di dosi. Grazie a queste informazioni sarà possibile calcolare le kcal per il quantitativo assunto
 * dall'utente tramite una funzione.
 * Stesso lavoro verrà fatto in caso di un flag che identifiche un alimento. Infatti, verrà usata una funzione diversa
 * per estrarre da un database apposito l'alimento che verrà memorizzato in una struct creata appositamente.
 *
 * In fine, rimpita la struct cibo, verrà passata ad una funzione di scrittura su file.
 *
 * @pre Nome_consumo non può essere vuoto, flag_consumo può assumero solo i valori 1 o 0, il nickname deve appartenere
 * ad uno degli utenti, quindi non può essere vuoto.
 *
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */

int aggiorno_database_calorie(char nome_consumo[], int flag_consumo, float quantita_consumo, char nickname[]) {

	// Controlli sui dati ricevuti in ingresso
	if(esiste_nickname(nickname) != 1){
		return 0;
	}

	if(flag_consumo != FLAG_ALIMENTO && flag_consumo != FLAG_RICETTA){
		return 0;
	}

	int l_nickname = strlen(nickname);
	if(l_nickname < MIN_LUNG_NICKNAME || l_nickname > MAX_LUNG_NICKNAME){
		return 0;
	}

	assunzione cibo;

	strcpy(cibo.nome, nome_consumo);
	cibo.flag = flag_consumo;
	cibo.quantita = quantita_consumo;

	// Costruzione del dato da inserire nel database assunzioni
	if (cibo.flag==FLAG_RICETTA){

	  int  kcal_ricetta;
	  int dosi_ricetta;

	  // Estrazione delle kcal della ricetta
	  if(estrai_kcal_ricetta(cibo.nome, &kcal_ricetta, &dosi_ricetta) != 0){
		  // Se non è possibile individuare la ricetta, viene ritornato -1
		  return -1;
	  }
	  cibo.kcal = calcolo_kcal(kcal_ricetta, dosi_ricetta, cibo.quantita);

	} else if (cibo.flag==FLAG_ALIMENTO) {

		alimento_database ricerca_a;
		// Estrazione delle info sull'alimento consumato
		if(ricerca_alimento_database(cibo.nome, &ricerca_a) != 1){
			// Se non è possibile estrarre l'alimento, viene ritornato -1
			return -1;
		}
		cibo.kcal=calcolo_kcal(ricerca_a.kcal, ricerca_a.campione_kcal, cibo.quantita);

	}

	scrittura_diretta_assunzione (&cibo, nickname);

	return 1;
}


/**
 * Funzione inizializza_file_assunzione ():
 *
 * La funzione ha il compito creare una struct di tipo data e assegnarle la data prese dal computer. Fatto ciò, grazie
 * al nickname passato in ingresso aprirà il file, lo resetterà e ci scriverà all'inzio la data.
 *
 * A tal scopo, creata la struct di dipo data, chiamerà una funzione per riempirla. Apre il file in modalità wb e scrive la data
 * tramite un fseek all'inizio del file.
 *
 * @pre Il nickname deve corrispondere a quello di un utente effettivamente esistente, per mantenere un senso logico.
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */
int inizializza_file_assunzione(char nickname[]) {

	data data_attuale = data_odierna();

	if(esiste_nickname(nickname) != 1){
		return 0;
	}

	char nome_file[LUNG_FILE_ASSUNZIONI] = PREFIX_FILE_ASSUNZIONI;
	strcat(nome_file, nickname);
	strcat(nome_file, SUFFIX_FILE_ASSUNZIONI);

	FILE* f;

	if ((f = fopen(nome_file, "wb")) == NULL){
		return -1;
	}

	fseek(f, 0, SEEK_SET);
	fwrite(&data_attuale, sizeof(data), 1, f);

	fclose(f);

	return 1;
}


/**
 * Funzione stampa_database_assunzioni ():
 *
 * La funzione ha il compito aprire un file assunzioni legato ad un utente e stampare il suo contenuto su schermo.
 *
 * A tal scopo, effetua un autenticazione per veficare i diritti di accesso a tali informazioni. Chiamata la funzione che si
 * occupa di tale compito, grazie alla stessa prende i dati relativi all'utente auteticato e li usa per aprire il file
 * corrispondente. Effettua un cotrollo sull'esistenza del file, in caso non esista lo crea tramite scrivi_data()
 * Nel caso esista, posiziona il puntatore ad inzio file e legge la data scritta il tale posizione, stampando su schermo tale
 * data. Procede succesivamente alla lettura delle assunzioni e della loro stampa su schermo.
 * Infine, procede alla chiamata di una funzione per il calcolo delle kcal totali.
 *
 * @pre Non sono presenti pre requisiti.
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */

int stampa_database_assunzioni() {
	int num_assunzioni = 0;
	utente persona;

	if (autenticazione(&persona) == 1) {

		data data_letta;
		assunzione cibo;

		//Creazione del nome file da aprire
		char nome_file[LUNG_FILE_ASSUNZIONI] = PREFIX_FILE_ASSUNZIONI;
		strcat(nome_file, persona.nickname);
		strcat(nome_file, SUFFIX_FILE_ASSUNZIONI);

		FILE* f;

		if((f = fopen(nome_file, "rb+"))==NULL){
			if(inizializza_file_assunzione(persona.nickname) != 1) { //creazione e scrittura della data in caso il file non esistesse
				return -1;
			}else if((f = fopen(nome_file, "rb+"))==NULL){
				return -1;
			}
		}

		//Leggo la data posizionata all'inizio
		fseek(f, 0, SEEK_SET);
		fread(&data_letta, sizeof(data), 1, f);

		printf("Assunzioni per %hu/%hu/%hu\n", data_letta.giorno,
				data_letta.mese, data_letta.anno);

		while (fread(&cibo, sizeof(assunzione), 1, f) > 0) {
			if (cibo.kcal != 0){
				num_assunzioni++;
			    printf("Nome: %s\nQuantita': %.2f\nkcal: %hu\n\n", cibo.nome,
				cibo.quantita, cibo.kcal);
			}
		}

		if(num_assunzioni == 0){
			puts("Non sono ancora state registrate assunzioni.");
		}

		fclose(f);

	    printf("Calorie assunte in totale: %d\n", calcolo_kcal_totali(nome_file));


	}else{
		return -1;
	}

	return 0;
}


/**
 * Funzione calcolo_kcal_totali ():
 *
 * La funzione ha il compito aprire un file assunzioni legato ad un utente, leggere tutte le struct relative alle assunzioni
 * presenti e memorizzare in una variabile accumulatore le kcal di ciascuna assunzione.
 *
 * A tal scopo, viene creata una struct di tipo assunzione, per contenere i dati presenti su file, e una variabile tot_kcal
 * di accumulazione.
 * Aperto il file, in puntatore va spostato oltre la data posizionata all'inizio, per poi proseguire una corretta
 * lettura delle sole assunzioni.
 *
 * @pre nome_file non vuoto
 * @post Il valore restituito è il calcolo delle calorie assunte in totale
 *
 */

unsigned short calcolo_kcal_totali(char* nomefile) {

	FILE* f;
	assunzione cibo;
	unsigned short tot_kcal=0;

	// Apertura del file contenente le assunzioni di un utente
	if((f = fopen(nomefile, "rb"))==NULL){
		if(inizializza_file_assunzione (nomefile) == 1) {
			return -1;
		}else if((f = fopen(nomefile, "rb"))==NULL){
			return -1;
		}
	}

	fseek(f, sizeof(data), SEEK_SET);
	while (fread(&cibo, sizeof(assunzione), 1, f) > 0) {
		if ( cibo.kcal !=0){
	    	 tot_kcal += cibo.kcal;
	    }
	}

	fclose(f);


	return tot_kcal;
}


/**
 * Funzione modifica_assunzione ():
 *
 * La funzione ha il compito aprire un file assunzioni legato ad un utente, leggere tutte le struct presenti e estrarre e modificare
 * la struct richiesta dall'untente, per poi salvarne le modifiche.
 *
 * A tal scopo, effettuata l'autenticazione, viene aperto il file corrispondente all'utente autenticato. Viene
 * richiesto all'utente di inserire il nome dell'alimento/ricetta da modicare, ricercata tale ricetta tramite una funzione
 * e modificata in modo diverso in base al suo flag.
 * Se il flag indica un input libero, viene richiesto esclusivamente l'input delle kcal, zero per eliminare l'alimento.
 * se il flag indica una ricetta o un alimento, verrà richiesto l'input della nuova quantità e le kcal verranno calcolate grazie a
 * delle funzioni apposite.
 *
 * @pre non sono presenti pre requisiti.
 * @post Il valore restituito indica l'esito della funzione.
 *
 */
int modifica_assunzione (){

	utente persona;	// Variabile di tipo utente utilizzata per l'autenticazione prima della modifica

	// // Tentativo di autenticazione
	if(autenticazione(&persona) == 1){

		FILE* stream;

		// generazione del nome del file
		char nome_file[LUNG_FILE_ASSUNZIONI] = PREFIX_FILE_ASSUNZIONI;
		strcat(nome_file, persona.nickname);
		strcat(nome_file, SUFFIX_FILE_ASSUNZIONI);

		assunzione nuova_assunzione;
		short int posizione; 			//varibile per memorizzare la posizione dove scrivere il nuovo record
		int esito_input;


		strcpy(nuova_assunzione.nome, input_nome_assunzione());


		posizione=ricerca_assunzione_database (&nuova_assunzione, persona.nickname);

		//Se si presenta uno dei due casi, vuol dire che il file non esiste o l'elemento cercato non è stato trovato
		if (posizione==0 || posizione==-1){
			printf("Impossibile terminare la modifica.\nFunzione terminata.\n\n");
			return 0;
		}

		if (nuova_assunzione.flag==FLAG_INPUT_LIBERO){
			puts("Il valore 0 può essere utilizzato per rimuovere l'assunzione.");
			nuova_assunzione.kcal = input_kcal_libere();
		} else {

			do {
				printf("Inserire la nuova quantita' (0 per elimare il consumo):\n~");
				esito_input = scanf("%f", &nuova_assunzione.quantita);
				if(pulisci_stdin() == 1){
					esito_input = 0;
				}

				if(esito_input == 0){
					puts("Inserimento non valido. Ripeterlo.");
				}
			}while (esito_input == 0);

			if (nuova_assunzione.flag==FLAG_RICETTA){

			  int  kcal_ricetta;
			  int dosi_ricetta;

			  // Estrazione delle kcal della ricetta
			  if(estrai_kcal_ricetta(nuova_assunzione.nome, &kcal_ricetta, &dosi_ricetta) != 0){
				  // Se non è possibile individuare la ricetta, viene ritornato -1
				  return -1;
			  }
			  nuova_assunzione.kcal=calcolo_kcal(kcal_ricetta, dosi_ricetta, nuova_assunzione.quantita);

			} else if (nuova_assunzione.flag==FLAG_ALIMENTO){

				alimento_database a;

				// Estrazione delle info sull'alimento consumato
				if(ricerca_alimento_database(nuova_assunzione.nome, &a) != 1){
					// Se non è possibile estrarre l'alimento, viene ritornato -1
					return -1;
				}
				nuova_assunzione.kcal=calcolo_kcal(a.kcal, a.campione_kcal, nuova_assunzione.quantita);

			}

		}

		if(nuova_assunzione.flag==FLAG_INPUT_LIBERO && nuova_assunzione.kcal==0){
			strcpy(nuova_assunzione.nome,"");
		}

		if((stream = fopen(nome_file, "rb+")) == NULL){
			puts("Errore nell'apertura del file assunzioni.");
			return -1;
		}


		fseek(stream, sizeof(data)+posizione*sizeof(assunzione)-sizeof(assunzione), SEEK_SET);
		fwrite(&nuova_assunzione, sizeof(assunzione), 1, stream);

		fclose(stream);
		return 1;

	} else {
		return 0;
	}
}


/**
 * Funzione ricerca_assunzione_database ():
 *
 * La funzione ha il compito aprire un file assunzioni legato ad un utente, il cui nickname viene passato in ingresso, e ricercare
 * nel file un assunzione con il nome corrispondente al nome passato in ingresso.
 *
 * A tal scopo, aperto il file, sposta il puntatore dopo la data scritta su file e inizia a leggere il suo contenuto attraverso
 * un while, se viene trovata la struct con nome corrispondente al nome passato in ingresso, un return permette l'uscita dalla
 * funzione. Nel while è presente un contatore, in modo che la funzione possa restituire tale contatore, quindi la posizione
 * della struct trovata su file.
 *
 * @pre nome dell'assunzione da cercare e nickname non vuoti.
 * @post Il valore restituito indica l'esito della funzione o la posizione della struct trovata.
 *
 */
short int ricerca_assunzione_database (assunzione* nuova_assunzione, char nickname[]){

	assunzione lettura;
	short int posizione=0; //variabile di memorizzazione della posizione della struct da trovare

	abbassa_maiuscole (nuova_assunzione->nome);

	char nome_file[LUNG_FILE_ASSUNZIONI] = PREFIX_FILE_ASSUNZIONI;
	strcat(nome_file, nickname);
	strcat(nome_file, SUFFIX_FILE_ASSUNZIONI);

	FILE* f;

	if((f = fopen(nome_file, "rb+"))==NULL){
			return -1;
	}

	fseek(f, sizeof(data), SEEK_SET); //sposto il puntatore oltre la data scritta ad inzio file

	while(feof(f) == 0){

		fread(&lettura, sizeof(assunzione), 1, f);
		if(strcmp(lettura.nome,"") != 0){
			posizione++;//leggo, incremento
		}
		if(strcmp(lettura.nome, nuova_assunzione->nome) == 0){
			nuova_assunzione->flag=lettura.flag;
			fclose (f);
			return posizione;
		}
	}
	fclose (f);
	return 1;
}


/**
 * Funzione scrittura_diretta_assunzione ():
 *
 * La funzione ha il compito aprire un file assunzioni legato ad un utente, il cui nickname viene passato in ingresso, e scrivere
 * su file la struct cibo passata in ingresso
 *
 * A tal scopo, aperto il file, si effettua un controllo sulla data. Se la data corrispondente alla data odierna, si passa
 * alla scrittura su file, altrimenti viene chiamata una funzione che resetta il file e scrive la data odierna su di esso,
 * procedendo alla scrittura solo successivamente.
 *
 * @pre nome nickname non vuoto.
 * @post Il valore restituito indica l'esito della funzione.
 *
 */
int scrittura_diretta_assunzione (assunzione* cibo, char nickname[]){

	if(esiste_nickname(nickname) != 1){
		return 0;
	}

	char nome_file[LUNG_FILE_ASSUNZIONI] = PREFIX_FILE_ASSUNZIONI;
	strcat(nome_file, nickname);
	strcat(nome_file, SUFFIX_FILE_ASSUNZIONI);

	FILE* stream;
	data controllo_data;

	if((stream = fopen(nome_file, "rb+"))==NULL){
		if(inizializza_file_assunzione(nickname) != 1){
			return -1;
		}else if((stream = fopen(nome_file, "rb+"))==NULL){
			return -1;
		}
	}

	fseek(stream, 0, SEEK_SET);
	fread(&controllo_data, sizeof(data), 1, stream);

	//controllo che la data scritta su file corrisponda a quella odierta, altrimenti resetto il file
	if (confronta_date(controllo_data, data_odierna()) == PRIMA_DATA_ANTECEDENTE) {
		if(inizializza_file_assunzione(nickname) != 1){
			return -1;
		}
	}

	fseek(stream, 0, SEEK_END);
	fwrite(cibo, sizeof(assunzione), 1, stream);

	if(strcmp(cibo->nome,"") != 0){
		printf("%s scritto correttamente in %s\n", cibo->nome, nome_file);
	}

	fclose(stream);

	return 1;
}


/**
 * Funzione istogrami ():
 *
 * La funzione ha il compito aprire un file assunzioni legato ad un utente, il cui nickname è ottenuto tramite una funzione di
 * auteticazione se viene soddisfatta.
 * Si estraggono le calorie che l'utente dovrebbe assumere sulla base di ciò che ha inserito sul menu settimanale,
 * tali calorie vengono identificate con degli asterischi: un asterisco (*) rappresenta 200 calorie.
 * Si legge su file quante calorie sono state effettivamente assunto e si stampa su schermo utilizzando lo stesso
 * metodo.
 * L'arrotondamento è fatto per eccesso.
 *
 *
 * @pre nome nickname non vuoto.
 * @post Il valore restituito indica l'esito della funzione.
 *
 */

void istogrami (){

	utente persona;
	int kcal_totali=0;

	if (autenticazione(&persona)== 1){

		char nome_file[LUNG_FILE_ASSUNZIONI] = PREFIX_FILE_ASSUNZIONI;
		strcat(nome_file, persona.nickname);
		strcat(nome_file, SUFFIX_FILE_ASSUNZIONI);

		int kcal_giornaliere;

		//estraggo le kcal dal menu della giornata di oggi (il giorno della settimana lo ricevo da una funzione)
		estrai_kcal_menu(&kcal_giornaliere, persona.nickname, giorno_odierno());

		printf("Assunzioni persona media: 2000 kcal: ");
		for (int i=0; i<KCAL_MEDIE_GIORNALIERE; i+=CAMPIONE_ISTOGRAMMI){
			printf("%c", 219);
		}

		//stampo l'istogramma con le kcal estratte dal menu
		printf("\n\nIl tuo obiettivo: %d", kcal_giornaliere);
		for (int i=0; i < kcal_giornaliere; i+=CAMPIONE_ISTOGRAMMI){
			printf("%c", 219);
		}

		//Stampo l'istogramma relativo a quelle registrate nel file assunzioni
		printf("\n\nIl tuo stato attuale:                ");
		kcal_totali=calcolo_kcal_totali(nome_file);

		for (int i=0; i < kcal_totali; i+=CAMPIONE_ISTOGRAMMI){
			printf("%c", 219);
		}
		printf("\nCalorie assunte in totale: %d\n", kcal_totali);

	}

}



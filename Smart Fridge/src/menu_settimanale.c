#include "menu_settimanale.h"

char* crea_nome_file_menu(const char* nickname){
	char* nome_file = (char*) calloc(LUNG_NOME_FILE_MENU, sizeof(char));

	int lung_nickname = strlen(nickname);

	if(lung_nickname < MIN_LUNG_NICKNAME || lung_nickname > MAX_LUNG_NICKNAME - 1){
		return NULL;
	}else{
		strcat(nome_file, FILE_MENU);
		strcat(nome_file, nickname);
		strcat(nome_file, ".dat");

		return nome_file;
	}
}





/**
 * Funzione che si occupa di eliminare il nome del file specificato e di restituire in caso di successo il valore -1
 *
 * @pre		Il nome del file deve essere significativo(il file deve esistere)
 * @post	In caso il file esista esso deve essere eliminato correttamente
 */
int elimina_file_menu(char nome_file[LUNG_NOME_FILE_MENU]) {
	FILE *fp;

	if ((fp = fopen(nome_file, "r")) == NULL) {
		return -1;
	}
	fclose(fp);

	int ret = remove(nome_file);//rimuove il file dal nome specificato

	if (ret == 0) {
		printf("\n %s correttamente eliminato\n", nome_file);
		return 1;

	} else {

		return -1;

	}

	return 0;
}



/**
 * Procedura inizializzazione ():
 *
 * La procedura ha il compito di chiamare una serie di funzioni utili ad un inizializzazione di un vettore di 7
 * struct di tipo giorno, ognuna delle quali rappresenta un giorno della settimana.
 *
 * A tal scopo esegue un ciclo per sette volte, in ognuno dei cicli viene passato uno dei sette elementi
 * appartenente al vettore di tipo giorno.
 *
 * Successivamente viene chiamata una funzione per la scrittura su file di tale vettore ora contente valori di inizializzazione.
 *
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'esito della precedura.
 *
 */
int inizializzazione(const char* nome_utente) {
	giorno menu[NUM_GIORNI]; //vettore che conterrà i 7 menu corrispondenti ai 7 giorni della settimana

	int lung_nome_utente = strlen(nome_utente);

	if(lung_nome_utente < MIN_LUNG_NICKNAME || lung_nome_utente > MAX_LUNG_NICKNAME - 1){
		return 0;
	}else if(esiste_nickname(nome_utente) == 0){
		return 0;
	}

	for (int i = 0; i < NUM_GIORNI; i++) {     //scorri nel vettore menu

		inizializzazione_giorno(&menu[i], i); //inizializza il singolo elemento appartenente al vettore
		inizializzazione_pasti(&menu[i]);

	}

	//scrivi su file il vettore inizializzato
	if (inizializzazione_file_menu(nome_utente, menu) == 0) {
		return 0;
	} else {
		return 1;
	}

}

/**
 * Funzione inizializzazione_giorno ():
 *
 * La funzione ha il compito di assegnare alla variabile giorno appartenente alla struct passatagli il nome di un giorno della settimana,
 * scegliendo tale nome sulla base della sua posizione nel vettore di struct a cui appartire. Tale posizione gli è passata dalla funzunzione
 * chiamante. Inoltre imposta le kcal della giornata a 0.
 *
 * A tal scopo esegue un controllo sull'indice passatogli per l'identificazione della sua possizione nel vettore a cui appartiene.
 * In base al suo valore, verrà chiamata una funzione che copierà nella variabile di tipo stringa "giorno" una stringa corrispondente
 * al nome di uno dei giorni della settimana.
 *
 *
 *
 * @pre l'indice deve essere compreso tra 0 e 6
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */
int inizializzazione_giorno(giorno* giornata, int num_giorno) {

	giornata->kcal = 0;

	switch (num_giorno) {
	case 0:
		strcpy(giornata->nome_giorno, "Lunedi");
		break;
	case 1:
		strcpy(giornata->nome_giorno, "Martedi");
		break;
	case 2:
		strcpy(giornata->nome_giorno, "Mercoledi");
		break;
	case 3:
		strcpy(giornata->nome_giorno, "Giovedi");
		break;
	case 4:
		strcpy(giornata->nome_giorno, "Venerdi");
		break;
	case 5:
		strcpy(giornata->nome_giorno, "Sabato");
		break;
	case 6:
		strcpy(giornata->nome_giorno, "Domenica");
		break;
	default:
		break;
	}

	return 0;
}

/**
 * Funzione inizializzazione_pasti ():
 *
 * La funzione ha il compito di scorrere nel vettore pasto e assegnare ai suoi elementi, in base alla loro posizione nel vettore,
 *  il nome di un pasto, chiamando poi una funzione per l'inizializzazione dei singoli elementi appartenenti al pasto.
 *
 * A tal scopo viene inizializzato un contatore, che servirà ad un ciclo che verrà ripetuto cinque volte (cinque pasti).
 * Il contatore rappresenterà l'indice del vettore pasto, in base al l'indice e quindi alla posizione della variabile nel vettore,
 * verrà assegnata alla variabile nome_pasto appartenente al dato strutturato pasto una stringa di caratteri.
 * Ad ogni posizione, viene associato un nome pasto diverso. Inoltre, viene chiamata un ulteriore funzione per l'inizializzazione del vettore
 * alimenti.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */
int inizializzazione_pasti(giorno* menu) {

	for (int i = 0; i < NUM_PASTI; i++) { //scorrimento nel vettore pasto

		//ad ogni case, copio una stringa che identifica quel pasto
		switch (i) {
		case 0:
			strcpy(menu->pasti[i].nome_pasto, "Colazione");

			inizializzazione_alimenti(menu, i); //inizializzo i singoli alimenti del pasto
			break;
		case 1:
			strcpy(menu->pasti[i].nome_pasto, "Spuntino 1");

			inizializzazione_alimenti(menu, i);
			break;
		case 2:
			strcpy(menu->pasti[i].nome_pasto, "Pranzo");

			inizializzazione_alimenti(menu, i);
			break;
		case 3:
			strcpy(menu->pasti[i].nome_pasto, "Spuntino 2");

			inizializzazione_alimenti(menu, i);
			break;
		case 4:
			strcpy(menu->pasti[i].nome_pasto, "Cena");

			inizializzazione_alimenti(menu, i);
			break;

		}

	}

	return 0;

}

/**
 * Funzione inizializzazione_alimenti ():
 *
 * La funzione ha il compito di assegnare al dato strutturato alimento, appartenente alla struct pasto, una stringa vuota al
 * nome_cibo, un flag impostato a -1 e una quantità pari a 0. Verrà passata anche l'indice del vettore pasto, per poter rimanere su un dato
 * elemento del vettore.
 *
 * Al tal scopo viene  inizializzato un contattore che sarà utilizzato in un ciclo che permetterà di scorrere per il vettore alimento.
 * Fermi su un dato elemento del vettore, verrà effetuato l'accesso e la modifica di ogni suo sotto-elemento.
 *
 * @pre num_pasto deve essere compreso tra 0 e 4
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */
int inizializzazione_alimenti(giorno* giornata, short int num_pasto) {

	for (int i = 0; i < NUM_CIBI; i++) { //scorrimento nel vettore alimenti

		giornata->pasti[num_pasto].cibi[i].flag = -1;

		strcpy(giornata->pasti[num_pasto].cibi[i].nome_cibo, ""); // Copio una striga vuota nel nome dell'i-esimo alimento

		strcpy(giornata->pasti[num_pasto].cibi[i].quantita, ""); // imposto una quantità pari a 0

	}

	return 0;
}

/**
 * Funzione inizializzazione_file_menu ():
 *
 * La funzione ha il compito di assegnare al dato strutturato alimento, appartenente alla struct pasto, una stringa vuota al
 * nome_cibo, un flag impostato a -1 e una quantità pari a 0. Verrà passata anche l'indice del vettore pasto, per poter rimanere su un dato
 * elemento del vettore.
 *
 * Al tal scopo viene  inizializzato un contattore che sarà utilizzato in un ciclo che permetterà di scorrere per il vettore alimento.
 * Fermi su un dato elemento del vettore, verrà effetuato l'accesso e la modifica di ogni suo sotto-elemento.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */
int inizializzazione_file_menu(const char* nome_utente, giorno* menu) {
	char nomefile[LUNG_NOME_FILE_MENU];
	strcpy(nomefile, crea_nome_file_menu(nome_utente));

	FILE *f;

	if ((f = fopen(nomefile, "wb+")) == NULL) {
		return 0;
	} else {
		//scrivo uno per uno i singolo elementi del vettore di struct passato in ingresso
		for (int i = 0; i < NUM_GIORNI; i++) {
			fwrite(&menu[i], sizeof(giorno), 1, f);
		}
	}

	fclose(f);

	return 1;

}

/**
 * Funzione esiste_menu ():
 *
 * La funzione ha il compito di controllare che il file menu, legato all'utente il cui nicknama è passato in ingresso, esista.
 *
 * Al tal scopo viene  effettuato un controllo sul nickname passato in ingresso, per assicurarsi che tale nickname sia idoneo.
 * Sucessivamente, viene tentata l'apertura del file. In base alla successo dell'operazione, viene restuituito un valore
 * opportuno dalla funzione.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */
int esiste_menu(char* nome_utente){
	int lung_nome_utente = strlen(nome_utente);

	//controllo sul nome utente
	if(lung_nome_utente < MIN_LUNG_NICKNAME || lung_nome_utente > MAX_LUNG_NICKNAME - 1){
		return -1;
	}else{
		FILE* stream = NULL;
		char* nome_file = crea_nome_file_menu(nome_utente);
		//apertura file
		if((stream = fopen(nome_file, "rb")) == NULL){
			return 0;
		}else{
			fclose(stream);
			return 1;
		}
	}
}

/**
 * Funzione input_kcal_giornata ():
 *
 * La funzione ha il compito di permettere l'input da parte dell'utente delle calorie della gionata di riferimento.
 *
 * Al tal scopo viene  implementato un do while, che ripete la richiesta di input fino a quando l'input non risulti
 * idoneo. La funzione ritornerà il valore inserito dall'utente.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'input dato.
 *
 */
int input_kcal_giornata(){
	int esito_input;
	int esito_controllo;

	int kcal_giornata;

	do {
		printf("Inserisci le kcal per la giornata:\n~");
		esito_input = scanf("%d", &kcal_giornata);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(kcal_giornata < MIN_KCAL_GIORNATA || kcal_giornata > MAX_KCAL_GIORNATA){
			esito_controllo = 0;
		}else{
			esito_controllo = 1;
		}

		if(esito_input != 1 || esito_controllo != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}
	} while (esito_input != 1 || esito_controllo != 1);

	return kcal_giornata;
}


int input_numero_giorno(){
	int esito_input;
	int esito_controllo;
	int num_giorno;

	do {
		printf("Seleziona un giorno della settimana:"
				"\n[0] Lunedi\n[1] Martedi\n[2] Mercoledi\n[3] Giovedi"
				"\n[4] Venerdi\n[5] Sabato\n[6] Domenica\n\n~");

		esito_input = scanf("%d", &num_giorno); //Scelta e memorizzazione della posizione della struct
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(num_giorno < 0 || num_giorno > 6){
			esito_controllo = 0;
		}else{
			esito_controllo = 1;
		}

		if(esito_input != 1 || esito_controllo != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}
	} while (esito_input != 1 || esito_controllo != 1);

	return num_giorno;
}

/**
 * Funzione input_numero_pasto ():
 *
 * La funzione ha il compito di permettere la selezione di uno dei pasti della giornata presa in esame, restituiendo tale valore.
 *
 * Al tal scopo viene  implementato un do while, che ripete la richiesta di input fino a quando l'input non risulti
 * idoneo. La funzione ritornerà il valore inserito dall'utente.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'input dato.
 *
 */
int input_numero_pasto(){
	int esito_input;
	int esito_controllo;
	int num_pasto;

	do {

		printf("Seleziona un pasto:"
				"\n\t[0] Colazione\n\t[1] Spuntino di meta' mattina\n\t[2]Pranzo\n\t[3]Snack pomeridiano\n\t[4]Cena\n~");
		esito_input = scanf("%d", &num_pasto); //Scelta e memorizzazione della posizione della struct
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(num_pasto < 0 || num_pasto > NUM_PASTI - 1){
			esito_controllo = 0;
		}else{
			esito_controllo = 1;
		}

		if(esito_input != 1 || esito_controllo != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}
	} while (esito_input != 1 || esito_controllo != 1);

	return num_pasto;
}


/**
 * Funzione input_nome_cibo ():
 *
 * La funzione ha il compito di permettere all'utente di inserire il nome di un cibo, ritornando quest'ultimo.
 *
 * Al tal scopo viene  allocata una parte di memoria al vettore che conterrà il nome del cibo
 * inoltre viene implementato un do while, che ripete la richiesta di input fino a quando l'input non risulti
 * idoneo. La funzione ritornerà la stringa inserita dall'utente
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'input dato.
 *
 */
char* input_nome_cibo(){
	int esito_input;
	char* nome_cibo = (char*) calloc(LUNG_CIBO, sizeof(char));

	do {

		printf("Inserisci il nome del cibo:\n~");
		esito_input = scanf("%40[^\n]", nome_cibo);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(esito_input != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}

	} while (esito_input != 1);

	return nome_cibo;
}

/**
 * Funzione input_quantita_cibo ():
 *
 * La funzione ha il compito di permettere all'utente di inserire la quantita del cibo,
 *
 * Al tal scopo viene allocata una parte di memoria per il contenimento della quantita
 * inoltre, viene  implementato un do while, che ripete la richiesta di input fino a quando l'input non risulti
 * idoneo. La funzione ritornerà la stringa inserita dall'utente
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'input dato.
 *
 */
char* input_quantita_cibo(){
	int esito_input;
	char* quantita = (char*) calloc(LUNG_QUANTITA, sizeof(char));

	do {

		printf("Inserisci la quantità del cibo[\"null\" per una quantita nulla]:\n~");
		esito_input = scanf("%10s", quantita); //Scelta e memorizzazione della posizione della struct
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(esito_input != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}

	} while (esito_input != 1);

	return quantita;
}


/**
 * Funzione input_flag_cibo ():
 *
 * La funzione ha il compito di permettere all'utente di inserire il flag corrispondente a ciò che sta inserendo:
 * 0 per un alimento
 * 1 per la ricetta
 *
 * Al tal scopo viene  implementato un do while, che ripete la richiesta di input fino a quando l'input non risulti
 * idoneo. La funzione ritornerà la stringa inserita dall'utente
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'input dato.
 *
 */
int input_flag_cibo(){
	int esito_input;
	int esito_controllo;
	int flag;

	do{
		printf("Inserisci la tipologia del cibo:\n\t[0] Alimento singolo\n\t[1] Ricetta\n~");
		esito_input = scanf("%d", &flag);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(flag < 0 || flag > 1){
			esito_controllo = 0;
		}else{
			esito_controllo = 1;
		}

		if(esito_input != 1 || esito_controllo != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}
	}while(esito_input != 1 || esito_controllo != 1);

	return flag;
}



/**
 * Funzione visualizza_database_menu ():
 *
 * La funzione ha il compito di aprire un file, il cui nome verrà generato grazie al paramentro in ingresso nome_utente ed il richiamo
 * ad una funzione che si occupa della sua generazione, e leggere il suo contenuto fino al raggiungimento della fine del file.
 * Tale lettura sarà possibile grazie alla funzione fread che momerizzerà ciò che legge in menu e
 * passerà esso ad una funzione che si occuperà della stampa su schermo.
 *
 *
 * @pre Nome utente non vuoto
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */
int visualizza_database_menu(char nome_utente[]) {

	if(esiste_nickname(nome_utente) == 0){
		puts("Il nome utente ricevuto non è valido.");
		return 0;
	}

	//Genero il nome del file
	char* nomefile = crea_nome_file_menu(nome_utente);

	giorno giornata;
	FILE* f;

	if ((f = fopen(nomefile, "rb")) == NULL) { //Se il file non viene aperto
		printf("Non è possibile aprire il file '%s'.", nomefile);
		return 1;
	}

	while (fread(&giornata, sizeof(giorno), 1, f) > 0) {

		stampa_giorno(&giornata);

	}

	fclose(f);

	return 1;
}

/**
 * Procedura ordina_cibi_pasto ():
 *
 * La funzione ha il compito di ordinare il vettore di cibi che riceve in ingresso mettendo nelle prime posizioni i cibi con nome diverso
 * da stringa vuota.
 *
 * A tale scopo viene implementato un for che scorre nel vettore e controlla che la stringa del nome sia vuota, se vuota, viene scambiata con quella
 * in posizione inferiore.
 *
 *
 * @pre non ci sono specifiche pre condizioni
 *
 */
void ordina_cibi_pasto(cibo* cibi){
	cibo tmp;

	for(int i = 1; i < NUM_CIBI; i++){
		if(strcmp(cibi[i].nome_cibo, "") != 0){
			if(strcmp(cibi[i-1].nome_cibo, "") == 0){
				tmp = cibi[i-1];
				cibi[i-1] = cibi[i];
				cibi[i] = tmp;
			}
		}
	}
}


/**
 * Funzione scelta_pasto ():
 *
 * La funzione ha il compito di permettere all'utente di scegliere il menu di quale giorno su cui effettuare l'aggiormento,
 * per poi scegliere ulteriormente il pasto su cui effetturare le modifiche.
 *
 * Al tal scopo viene crata una struct di tipo utente per permettere l'autentificazione dell'utente, se essa va a buon fine, sarà possibile scegliere
 * il giorno ed estrarre tale giorno dal file attraverso la funzione estrazione_struct. Verrà stampato su schermo il menu estratto grazie alla procedura
 * stampa_menu ed a questo punto viene richiamata una funzione di selezione del pasto ed un'altra per la selezione dell'alimento.
 * A questo punto verrà chiamata una funzione che interviene sulla modifica del singolo alimento, fatto ciò la procedura scrivi_menu memorizzerà su file
 * il nuovo menu.
 * In fine, verrà visualizzato il database del menu settimanale dell'utente per intero.
 * Verrà richiesto se si desidera modificare altro, se affermativo verranno ripetute la procedura si selezione e di modifica altrimenti la funzione terminerà.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */
void modifica_menu() {
	int scelta;
	int esito_input;
	int esito_controllo;
	int num_pasto;
	int num_giorno;
	giorno giornata;
	utente u;

	if (autenticazione(&u) == 1) { //Se l'autenticazione va a buon fine esegui:

		if(esiste_menu(u.nickname) == 0){
			if(inizializzazione(u.nickname) == 0){
				puts("Impossibile effettuare la modifica.");
				return;
			}
		}

		do{

			num_giorno = input_numero_giorno();

			if(estrai_giorno(&giornata, u.nickname, num_giorno) != 1){
				puts("Errore nell'estrazione della giornata.\n"); //da estrarre da file
				return;
			}

			stampa_giorno(&giornata); //stampa struct estratta

			do{
				printf("Inserisci:\n\t[1] per modificare le kcal della giornata\n\t[0] per modificare un pasto\n~");
				esito_input = scanf("%d", &scelta);
				if(pulisci_stdin() == 1){
					esito_input = 0;
				}

				if(scelta < 0 || scelta > 1){
					esito_controllo = 0;
				}else{
					esito_controllo = 1;
				}

				if(esito_input != 1 || esito_controllo != 1){
					puts("Inserimento non valido. Ripeterlo.");
				}
			}while(esito_input != 1 || esito_controllo != 1);

			if (scelta == 1) {
				giornata.kcal = input_kcal_giornata();
			} else if (scelta == 0) {
				//Scelta del pasto da modificare
				num_pasto = input_numero_pasto();;

				modifica_alimenti_pasto(&giornata, num_pasto);

			}

			scrivi_giorno(&giornata, u.nickname, num_giorno);

			visualizza_database_menu(u.nickname);

			do{
				printf("Inserisci:\n\t[1] per effettuare una nuova modifica sul menu\n\t[0] per terminare le modifiche sul menu\n~");
				esito_input = scanf("%d", &scelta);
				if(pulisci_stdin() == 1){
					esito_input = 0;
				}

				if(scelta < 0 || scelta > 1){
					esito_controllo = 0;
				}else{
					esito_controllo = 1;
				}

				if(esito_input != 1 || esito_controllo != 1){
					puts("Inserimento non valido. Ripeterlo.");
				}
			}while(esito_input != 1 || esito_controllo != 1);
		}while(scelta == 1);

	}
}

/**
 * Procedura modifica_alimenti_pasto ():
 *
 * La procedura ha il compito di modificare un singolo alimento su richiesta dell'utente
 *
 * Al tal scopo viene fatto un controllo su ognuno degli aliemnti presenti nel pasto corrispondente all'indice passato in ingresso,
 * se la stringa è diversa dalla stringa vuota, un contatore verrà aumentato di uno.
 * Fatto tutti i confronti, se il contatore è minore del numero degli alimenti, verrà richiesto se si desidera modificare uno di quelli gia
 * esistenti o di aggiungerne uno nuovo. Se si scegli di inserire uno nuovo, si effettua una richiesta di tale input e verrà memorizzata nella
 * prima posizione disponibile nel vettore degli alimento, altrimenti verrà modifcato il cibo selezionato. Vengono oridinati i cibi.
 * Fatto ciò, viene richiesto se si desidera modificare altro in relazione a questo pasto, se si vengono effettuate tutte le operazioni di scelta
 * e di modifca sopracitate, altrimenti la funzione si conclude.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */
void modifica_alimenti_pasto(giorno* giornata, short int num_pasto) {
	int esito_input;
	int esito_controllo;
	int scelta;
	short num_cibi = 0;

	//controllo che il nome dei cibi sia diverso da stringa vuota ed incremento il contatore nel caso così fosse
	for(int i = 0; i < NUM_CIBI; i++){
		if(strcmp(giornata->pasti[num_pasto].cibi[i].nome_cibo, "") != 0){
			num_cibi++;
		}
	}

	do{
		//se ci sono nomi cibi con stringa vuota do la possibilità di modifcare quelli già esistenti e di aggiungere nelle
		//posizioni con stringa vuota nuovi alimenti
		if(num_cibi > 0 && num_cibi < NUM_CIBI){

			do {
				printf("Inserisci:\n\t[1] per modificare un cibo\n\t[0] per aggiungerne uno\n~");
				esito_input = scanf("%d", &scelta);
				if(pulisci_stdin() == 1){
					esito_input = 0;
				}

				if(scelta < 0 || scelta > 1){
					esito_controllo = 0;
				}else{
					esito_controllo = 1;
				}

				if(esito_input != 1 || esito_controllo != 1){
					puts("Inserimento non valido. Ripeterlo.");
				}
			} while (esito_input != 1 || esito_controllo != 1);

		}else{
			if(num_cibi == 0){
				scelta = 0;
			}else if(num_cibi == NUM_CIBI){
				scelta = 1;
			}
		}

		//scrivo in una dei cibi con nome con stringa vuota
		if(scelta == 0){

			puts("Aggiungi un nuovo cibo:\n");

			giornata->pasti[num_pasto].cibi[num_cibi].flag = input_flag_cibo();

			//controllo che il nome non sia già presente nel pasto
			do {
				strcpy(giornata->pasti[num_pasto].cibi[num_cibi].nome_cibo, input_nome_cibo());

				for(int i = 0; i < num_cibi; i++){
					if(giornata->pasti[num_pasto].cibi[i].flag == giornata->pasti[num_pasto].cibi[num_cibi].flag){
						if(strcmp(giornata->pasti[num_pasto].cibi[i].nome_cibo, giornata->pasti[num_pasto].cibi[num_cibi].nome_cibo) == 0){
							esito_input = 0;
							break;
						}
					}else{
						esito_input = 1;
					}
				}
				//se è presente faccio ripetere l'input
				if(esito_input == 0){
					puts("Nome cibo già presente nel pasto. Inserirne uno diverso.");
				}
			} while (esito_input == 0);
			//se la quantità non è idonea, non salvo il cibo inserito
			strcpy(giornata->pasti[num_pasto].cibi[num_cibi].quantita, input_quantita_cibo());

			if(strcmp(giornata->pasti[num_pasto].cibi[num_cibi].quantita, "null") == 0){
				strcpy(giornata->pasti[num_pasto].cibi[num_cibi].nome_cibo, "");
				puts("Nuovo cibo non inserito.");
			}else{
				num_cibi++; //incremento il contatore per segnalare che un alimento è stato aggiunto e che non fa più
				            // parte degli alimenti con stringa vuota
			}
			//se si scegli di modifcare un cibo già esistente
		}else if(scelta == 1){
			int indice_cibo_scelto = 0;
			char nome_cibo[LUNG_CIBO];

			puts("Modifica un cibo:\n");

			do {
				strcpy(nome_cibo, input_nome_cibo());
				//controllo se il cibo è gia presente
				for(int i = 0; i < num_cibi; i++){
					if(strcmp(giornata->pasti[num_pasto].cibi[i].nome_cibo, nome_cibo) != 0){
						esito_input = 0;
					}else{
						indice_cibo_scelto = i;
						esito_input = 1;
						break;
					}
				}

				if(esito_input == 0){
					puts("Cibo non presente nel pasto. Sceglierne uno presente.");
				}
			} while (esito_input == 0);

			strcpy(giornata->pasti[num_pasto].cibi[indice_cibo_scelto].quantita, input_quantita_cibo());
			//se la quantità è uguale a null, il cibo viene eliminato
			if(strcmp(giornata->pasti[num_pasto].cibi[indice_cibo_scelto].quantita, "null") == 0){
				strcpy(giornata->pasti[num_pasto].cibi[indice_cibo_scelto].nome_cibo, "");
				puts("Cibo eliminato.");
				//ordino il vettore
				ordina_cibi_pasto(giornata->pasti[num_pasto].cibi);
			}
		}


		//verifico se l'utente vuole o non vuole modifcare altro in relazione con questo pasto
		do{
			printf("Inserire:\n\t[1] per effettuare nuove operazioni su questo pasto\n\t[0] per terminare le modifiche su questo pasto\n~");
			esito_input = scanf("%d", &scelta);
			if(pulisci_stdin() == 1){
				esito_input = 0;
			}

			if(scelta < 0 || scelta > 1){
				esito_controllo = 0;
			}else{
				esito_controllo = 1;
			}

			if(esito_input != 1 || esito_controllo != 1){
				puts("Inserimento non valido. Ripeterlo.");
			}
		}while(esito_input != 1 || esito_controllo != 1);

	}while(scelta == 1);
}


/**
 * Funzione estrai_giorno ():
 *
 * La funzione ha il compito di estrarre dal file menu di un utente la struct all'n-esima posizione.
 *
 * Al tal scopo viene riesco passata alla funzione un puntatore a menu, dove verrà memorizzata la struct estratta. Il nome_utente servirà per
 * generare il nome del file corrispondente al menu dell'utente. Fatto ciò, verrà a aperto il file, estratta la struct alla posizione indicata da num_giorno
 * e momorizzata in menu. Il file viene chiuso e la funzione termina.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */
int estrai_giorno(giorno* giornata, char* nome_utente, int num_giorno) {

	if(num_giorno < 0 || num_giorno > NUM_GIORNI - 1){
		return -1;
	}

	if(esiste_nickname(nome_utente) == 0){
		return -1;
	}

	FILE* stream = NULL;

	char* nome_file = crea_nome_file_menu(nome_utente);

	if((stream = fopen(nome_file, "rb")) == NULL){
		return 0;
	}else{

		fseek(stream, num_giorno * sizeof(giorno), SEEK_SET); //posiziono il puntatore all'num_giorno-esima posizione
		fread(giornata, sizeof(giorno), 1, stream);

		fclose(stream);
		return 1;
	}
}

/**
 * Funzione estrai_kcal_menu ():
 *
 * La funzione ha il compito di estrarre dal file menu le kcal del giorno indicato da num_giorno.
 *
 * Al tal scopo viene riesco passata alla funzione il nome utente per la generazione del nome file, un puntatore a kcal dove verranno memorizzate le kcal
 * estratte ed il num_giorno. Effettuato un controllo sul num_giorno, viene aperto il file dopo la creazione del suo nome e letta la struct
 * corrispondente, momorizzando in kcal le kcal lette del giorno estratto.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */
int estrai_kcal_menu(int* kcal, char* nome_utente, int num_giorno){
	if(num_giorno < 0 || num_giorno > NUM_GIORNI - 1){
		return -1;
	}

	if(esiste_nickname(nome_utente) == 0){
		return -1;
	}

	FILE* stream = NULL;
	giorno giornata;
	char* nome_file = crea_nome_file_menu(nome_utente);

	if((stream = fopen(nome_file, "rb")) == NULL){
		return 0;
	}else{

		fseek(stream, num_giorno * sizeof(giorno), SEEK_SET); //posiziono il puntatore all'num_giorno-esima posizione
		fread(&giornata, sizeof(giorno), 1, stream);

		//memorizzo le kcal della giornata estratta
		*kcal = giornata.kcal;

		fclose(stream);
		return 1;
	}
}


/**
 * Procedura scrivi_menu ():
 *
 * La procedura ha il compito di scrivere su file il dato strutturato menu passatogli in ingresso al num_esima-esima posizione.
 *
 * Al tal scopo viene generato il nome del file attraverso nome_utente passatogli in ingresso.
 * fatto ciò viene aperto il file, posizionato il puntatore alla num_giorno-esima posizione e scritto menu in tale posizione.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post esito della funzione
 *
 */
int scrivi_giorno(giorno* giornata, char* nome_utente, int num_giorno) {

	if(num_giorno < 0 || num_giorno > NUM_GIORNI - 1){
		return -1;
	}

	if(esiste_nickname(nome_utente) != 1){
		return -1;
	}

	FILE* stream = NULL;

	char* nomefile = crea_nome_file_menu(nome_utente);

	if((stream = fopen(nomefile, "rb+")) == NULL){
		return 0;
	}else{

		fseek(stream, num_giorno * sizeof(giorno), SEEK_SET);
		fwrite(giornata, sizeof(giorno), 1, stream);

		fclose(stream);
		return 1;

	}

}

/**
 * Procedura stampa_menu ():
 *
 * La procedura ha il compito stampare su schermo il contenuto del dato strutturato a cui punta il puntatore di tipo giorno passatogli in ingresso.
 *
 * Al tal scopo vengono implementatati due for innestate che permettore lo scorrimento nel vettore pasto ed un ulteriore scorrimento nel vettore alimenti.
 * ad ogni scorrimento, viene stampato su schermo la varibile interessata.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post no
 *
 */
void stampa_giorno(giorno* giornata) {

	int num_cibi_letti;

	printf(
			"*********************************************************************************************************\n");
	printf("%s kcal: ", giornata->nome_giorno);

	//se non ci sono le kcal della giornata
	if(giornata->kcal == 0){
		puts("Non ancora assegnate.");
	}else{
		//se ci sono le kcal per la giornata
		printf("%d\n", giornata->kcal);
	}
	//stampo struct passata in ingresso
	for (int i = 0; i < NUM_PASTI; i++) {
		num_cibi_letti = 0;

		printf("# %s:\n", giornata->pasti[i].nome_pasto);

		for (int j = 0; j < NUM_PASTI - 1; j++) {

			//se la stringa non è vuota la stampa
			if(strcmp(giornata->pasti[i].cibi[j].nome_cibo, "") != 0){
				printf("\t- %s %s\n", giornata->pasti[i].cibi[j].nome_cibo, giornata->pasti[i].cibi[j].quantita);
				//incremento il contatore nei cibi con nome diverso da stringa vuota
				num_cibi_letti++;
			}

		}

		//stampa in caso non ci siano nomi dei bici diversi da stringa vuota
		if(num_cibi_letti == 0){
			printf("\t- Nessun cibo.\n");
		}
	}
	printf(
			"*********************************************************************************************************\n");
}


/**
 * Funzione gestore_visualizzazione_menu ():
 *
 * La funzione ha gestire la visualizzazione del menu.
 *
 * Al tal scopo vengono effettuato un controllo sulla possibilita dell'utente di accedere a tali informazioni, attraverso l'autenticazione.
 * se l'autenticazione va a buon fini, i diritti di tale utente alla visualizzazione sono idenei, altrimenti non viene stampato.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post esito della funzione
 *
 */
int gestore_visualizzazione_menu (){
	utente u;
	if(autenticazione(&u)){
		visualizza_database_menu(u.nickname);
	}else{
		printf("Operazione di utenticazione fallita");
		return 0;
	}

	return 1;
}

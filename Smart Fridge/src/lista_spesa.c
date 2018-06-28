/*
 * lista_spesa.c
 *
 *  Created on: 18 giu 2018
 *      Author: david
 */

#include "lista_spesa.h"

/**
 * Funzione che provvede a scrivere su file(il nome é specificato dal parametro in ingresso) il nome dell'alimento che passiamo alla funzione.
 *
 * Dopo aver eseguito i vari controlli sulla modalitá di apertura del file della spesa(esso puó riferirsi a quello personale o a quello globale),
 * viene scritto su di esso il nome dell'alimento. Qualora l'inserimento non vada a termine viene notificato un messaggio di errore e si chiede
 * di riscrivere l'alimento
 *
 * @pre		Che il nome del file sia significativo e che la stringa che rappresenta il nome dell'alimento non sia vuota
 * @post	Deve essere scritto su file l'alimento passato
 */
int aggiorna_lista_spesa(char nome_alimento[LUNG_NOME_ALIMENTO],char nome_file[LUNG_NOME_FILE]) {
	FILE *fp;
	int esito_scrittura;
	int flag_presenza = 0;
	char alimento_letto[LUNG_NOME_ALIMENTO];


	// tentativo di apertura del file frigo in lettura e scrittura
	if ((fp = fopen(nome_file, "r+")) == NULL) {

		// se il file non viene aperto, si decide di crearne uno nuovo
		if ((fp = fopen(nome_file, "w+")) == NULL)
			return 0;

	}else{

		//vengono estratti gli alimenti giá presenti nel file e per evitare caricamento di alimenti omonimi. Inoltre viene troncato lo \n
		//per far si che strcmp lavori senza intoppi.
		while (fgets(alimento_letto, LUNG_NOME_ALIMENTO, fp) != NULL) {
			if(alimento_letto[strlen(alimento_letto) - 1] != '\n'){
				pulisci_riga_flusso(fp);
			}

			sscanf(alimento_letto, "%20[a-zA-Z]", alimento_letto);
			if (strcmp(alimento_letto, nome_alimento) == 0) {
				flag_presenza = 1;
				break;
			}
		}

	}

	if (flag_presenza == 1) {
		return 0;
	}

	esito_scrittura = fprintf(fp, "%s\n", nome_alimento);

	if (esito_scrittura == 0) {

		printf("Errore nella scrittura di %s. Si prega di inserire manualmente l'alimento",nome_alimento);

		do {

			esito_scrittura = fprintf(fp, "%s\n", input_nome_alimento());

		} while (esito_scrittura == 0);

	}

	fclose(fp);
	return 0;

}

/**
 * Funzione che dopo aver allocato una stringa di una determinata lunghezza, si occupa di ricevere in input il nome della ricetta
 *
 * @pre		Nessuna pre condizione particolare
 * @post	Deve essere restituita una stringa con almeno un carattere
 */
char* input_nome_alimento() {
	int esito_input;			// Variabile per memorizzare l'esito dell'input
	// Stringa da restituire contenente il nome della ricetta
	char* nome_alimento = (char*) calloc(LUNG_NOME_ALIMENTO, sizeof(char));

	do {
		printf("Inserisci il nome dell'alimento:\n>");
		scanf("%25[^\n]", nome_alimento);
		if (pulisci_stdin() == 1) {
			esito_input = 0;
		}

		if (esito_input == 0) {
			puts("Inserimento non valido. Ripeterlo.");
		}
	} while (esito_input == 0);

	return nome_alimento;

}



/**
 * Funzione che provvede a visualizzare gli alimenti che devono essere comprati dal file della lista personale o di quella globale.
 *
 * Qualora non sia possibile aprire il file in lettura(ricordiamo che il nome del file della spesa viene generato esternamente alla funzione),
 * viene restituito -1. Nel caso in cui sia possibile aprire il file da cui desideriamo estrarre gli alimenti da acquistare viene effettuata
 * la stampa fino al termine del file.
 *
 * @pre		Il nome del file deve essere significativo(il file deve esistere e deve possedere almeno un alimento)
 * @post	Deve essere stampato almeno un alimento
 */
int visualizza_lista_spesa(char nome_file[LUNG_NOME_FILE]) {

	FILE *stream = NULL;
	char nome_alimento[LUNG_NOME_ALIMENTO];

	if ((stream = fopen(nome_file, "r")) == NULL) {

		return -1;

	} else {

		while (feof(stream) == 0) {

			fscanf(stream, "%s\n", nome_alimento);
			printf("%s\n", nome_alimento);

		}

	}

	return 1;
}



/**
 * Funzione che si occupa di eliminare il nome del file specificato e di restituire in caso di successo il valore -1
 *
 * @pre		Il nome del file deve essere significativo(il file deve esistere)
 * @post	In caso il file esista esso deve essere eliminato correttamente
 */
int elimina_file_spesa(char nome_file[LUNG_NOME_FILE]) {
	FILE *fp;

	if ((fp = fopen(nome_file, "r")) == NULL) {
		return -1;
	}
	fclose(fp);

	int ret = remove(nome_file);//rimuove il file dal nome specificato

	if (ret == 0) {

		return 1;

	} else {

		return -1;

	}

	return 0;
}




/**
 * Funzione che si occupa di leggere dal database gli alimenti che possono essere inseriti nella lista della spesa. Per far ció viene
 * effettuata la lettura del database alimenti e viene fatto un controllo sulla soglia tramite la funzione controllo soglia. Se la condizione
 * é vera(vedi funione controllo_soglia per avere un'idea sui valori restituiti) allora l'alimento viene passato alla funzione aggiorna_lista_spesa
 * Al termine viene restituito 1
 *
 * @pre		nessuna particolare pre condizione
 * @post	nessuna particolare post condizione
 */
int lettura_alimenti_acquistabili() {
	alimento_database alimento;
	FILE *stream = NULL;

	if ((stream = fopen(FILE_DATABASE_ALIMENTI, "rb")) == NULL) {

		return -1;

	} else {

		while (fread(&alimento, sizeof(alimento_database), 1, stream) > 0) {
			//se la soglia é maggiore di 0 e se la quantitá disponibile é minore di tale soglia allora occorre memorizzare l'alimento
			if (controllo_soglia(alimento.soglia_spesa, alimento.nome))
				aggiorna_lista_spesa(alimento.nome, LISTA_SPESA_GLOBALE);

		}

		return 1;
	}

	return 0;
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
int estrazione_ingredienti(char nome_ricetta[DIM_CIBO], ingrediente* ingredienti) {
	FILE* stream_database;
	int flag_presente = 0;//flag che ci permette di segnalare se una ricetta é presente oppure no
	int indice_ingrediente = 0; //indice che ci aiuta a popolare l''array di struct

	if ((stream_database = fopen(FILE_DATABASE_RICETTE, "rb+")) == NULL)
		return -1;

	ricetta analisi_ricetta;

	flag_presente = esiste_ricetta(nome_ricetta);

	if (flag_presente == 0)
		return -1;

	while (fread(&analisi_ricetta, sizeof(analisi_ricetta), 1, stream_database) > 0) {

		if (strcmp(nome_ricetta, analisi_ricetta.nome_ricetta) == 0) {

			do {

				//il controllo sul nome ci aiuta a capire quando stiamo controllando l'ingrediente successivo all'ultimo che é una stringa vuota
				if (strcmp(analisi_ricetta.ingredienti[indice_ingrediente].nome , "") != 0) { //se l'ingrediente é significativo allora...
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




/**
 * Funzione che si occupa di creare il file che conterrá gli alimenti che un utente specifico deve comprare.
 *
 * Per far ció la prima operazione da compiere é l'autenticazione dell'utente. Qualora essa vada a buon fine viene sfruttato il nickname
 * dell'utente per creare un file della spesa personalizzato. La seconda operazione necessaria é quella di estrarre dal menu settimanale
 * gli alimenti e le ricette che l'utente consuma giornalmente. Per ogni pasto viene fatto un confronto sulla tipologia dell'alimento.
 * Se siamo in presenza di una ricetta viene popolato un array di struct(di tipo ingrediente) che avrá tutti gli ingredienti che compongono la
 * ricetta. Se il numero di ingredienti di una data ricetta é -1 vuol dire che la ricetta non é presente nel database e viene notificato un messaggio
 * all'utente. Se viene restituito un valore diverso vuol dire che gli ingredienti sono candidati ad essere memorizzati nella lista della spesa,
 * ma occorre effettuare un controllo con la soglia e la quantitá disponibile. Sela quantitá disponibile é inferiore alla soglia l'ingrediente viene
 * memorizzato. Stessa ragionamento per gli alimenti singoli presenti nel menú settimanale.
 *
 * @pre		Nessuna pre condizione
 * @post	Venga generato il file della spesa dell'utente
 */
int gestore_spesa_personale() {
	utente u;
	giorno menu;

	//autenticazione(con controllo su esito) dell'utente
	if (autenticazione(&u) == -1){
		printf("Operazione di utenticazione fallita");
		return -1;
	}

	//generazione del nome del file che ospiterá la spesa personale
	char nome_file[MAX_LUNG_NOMEFILE] = PRIMA_PARTE_NOMEFILE;
	strcat(nome_file, u.nickname);
	strcat(nome_file, FORM_FILE_SPESA);

	elimina_file_spesa(nome_file);//eliminazione del file "vecchio" se presente

	for (int i_giorno = 1; i_giorno <= 7; i_giorno++) {

		estrazione_struct(&menu, u.nickname, i_giorno); //viene estratto il menu giornaliero

		for (int i_pasto = 0; i_pasto < NUM_PASTI; i_pasto++) {

			for (int i_alimento = 0; i_alimento < NUM_ALIMENTO; i_alimento++) {

				if (menu.pasto[i_pasto].alimento[i_alimento].flag == FLAG_RICETTA) {//se l'alimento che si sta analizzando é una ricetta
					//vengono estratti gli ingredienti
					int num_ingredienti;
					ingrediente ingredienti[MAX_INGREDIENTI];
					num_ingredienti = estrazione_ingredienti(menu.pasto[i_pasto].alimento[i_alimento].nome_cibo,ingredienti);

					if (num_ingredienti == -1) {//controllo che ci aiuta a capire se la ricetta non esiste nel database

						printf(
								"Non e' stato possibile estrarre gli alimenti che compongono la ricetta: %s\n",
								menu.pasto[i_pasto].alimento[i_alimento].nome_cibo);
						printf(
								"Si cosiglia di caricare la ricetta nel database delle ricette per facilitare la creazione della spesa\n");

					} else {
						//se la ricetta in questione esiste, gli ingredienti estratti verranno analizzati in base alla loro soglia e aggiunti
						// in caso alla lista
						for (int i_ingrediente = 0; i_ingrediente < num_ingredienti; i_ingrediente++){

							if(controllo_soglia(soglia_alimento(ingredienti[i_ingrediente].nome),ingredienti[i_ingrediente].nome) == 1){
								aggiorna_lista_spesa(ingredienti[i_ingrediente].nome, nome_file);
							}

						}

					}

				} else {
					//altimenti se ció che si sta analizzando é un alimento si effettua un controllo diretto sulla soglia e sulla quantitá
					//disponibile
					if (menu.pasto[i_pasto].alimento[i_alimento].flag == FLAG_ALIMENTO){

						if(controllo_soglia(soglia_alimento(menu.pasto[i_pasto].alimento[i_alimento].nome_cibo),
								menu.pasto[i_pasto].alimento[i_alimento].nome_cibo) == 1){

							aggiorna_lista_spesa(menu.pasto[i_pasto].alimento[i_alimento].nome_cibo, nome_file);

						}else{

							if(soglia_alimento(menu.pasto[i_pasto].alimento[i_alimento].nome_cibo)==0){
								printf("Non e' stato possibile estrarre l'alimento perche' sconosciuto al database: %s\n",
										menu.pasto[i_pasto].alimento[i_alimento].nome_cibo);
							}

						}

					}

				}

			}
		}
	}

	return 1;

}




/**
 * Funzione che si occupa di confrontare la soglia in ingresso con la quantità di quell'alimento. Qualora la quantitá disponibile nel frigo
 * sia inferiore della soglia prestabilita viene restituito 1 e 0 in caso contrario. Nel caso in cui invece la soglia di un determinato alimento
 * sia settata a 0 viene restituito, allo stesso modo, 0. Qualora i controlli non vadano a buon fine viene restituito -1
 *
 * @pre		nessuna particolare pre condizione
 * @post	Che venga effettuato correttamente il confronto tra quantitá e soglia dell'alimento
 */
int controllo_soglia(float soglia_spesa, char nome_alimento[LUNG_NOME_ALIMENTO]) {

	if (soglia_spesa > 0) {

		if (quantita_alimento(nome_alimento) < soglia_spesa) {
			return 1;
		} else {
			return 0;
		}

	} else {

		return 0;

	}

	return -1;

}


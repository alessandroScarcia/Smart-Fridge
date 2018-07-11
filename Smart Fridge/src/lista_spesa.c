
#include "lista_spesa.h"

/**
 * Funzione che provvede a scrivere su file(il nome e' specificato dal parametro in ingresso) il nome dell'alimento che passiamo alla funzione.
 *
 * Dopo aver eseguito i vari controlli sulla modalitá di apertura del file della spesa(esso puó riferirsi a quello personale o a quello globale),
 * viene scritto su di esso il nome dell'alimento. Qualora l'inserimento non vada a termine viene notificato un messaggio di errore e si chiede
 * di riscrivere l'alimento
 *
 * @pre		Che il nome del file sia significativo e che la stringa che rappresenta il nome dell'alimento non sia vuota
 * @post	Deve essere scritto su file l'alimento passato
 */
int aggiorna_lista_spesa(char nome_alimento[LUNG_NOME_ALIMENTO],char nome_file[LUNG_NOME_FILE_SPESA]) {
	FILE *fp;
	int esito_scrittura;
	int flag_presenza = 0;
	char alimento_letto[LUNG_NOME_ALIMENTO + 1];


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

	if (esito_scrittura < 0) {

		printf("Errore nella scrittura di %s. Si prega di inserire manualmente l'alimento\n",nome_alimento);

		do {

			esito_scrittura = fprintf(fp, "%s\n", input_nome_alimento());

		} while (esito_scrittura == 0);

	}

	fclose(fp);
	return 0;

}


/** Funzione che si occupa di effettuare l'autenticazione dell'utente per poter poi poter mostrare la lista della spesa personale
 *
 * @pre		Nessuna particolare pre condizione
 * @post	nessuna particolare post condizione
 */
int visualizzazione_personale(){
	utente u;
	//autenticazione(con controllo su esito) dell'utente
	if (autenticazione(&u) == -1){
		printf("Operazione di utenticazione fallita\n");
		return -1;
	}

	//generazione del nome del file che ospiterá la spesa personale
	char nome_file[LUNG_NOME_FILE_SPESA] = PREFIX_FILE_SPESA;
	strcat(nome_file, u.nickname);
	strcat(nome_file, SUFFIX_FILE_SPESA);
	visualizza_lista_spesa(nome_file);

	return 1;
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
int visualizza_lista_spesa(char nome_file[LUNG_NOME_FILE_SPESA]) {

	FILE *stream = NULL;
	char nome_alimento[LUNG_NOME_ALIMENTO + 1];
	int num_alimenti = 0;

	printf("Lista della spesa: %s\n", nome_file);

	if ((stream = fopen(nome_file, "r")) == NULL) {

		puts("Non è possibile aprire il file.");

		return -1;

	} else {

		while (fgets(nome_alimento, LUNG_NOME_ALIMENTO, stream) != NULL) {

			if(nome_alimento[strlen(nome_alimento) - 1] != '\n'){
				pulisci_riga_flusso(stream);
			}

			num_alimenti++;
			sscanf(nome_alimento, "%20[a-zA-Z]", nome_alimento);
			printf("%s\n", nome_alimento);

		}

		if(num_alimenti == 0){
			puts("Non sono presenti alimenti nella lista della spesa.\n");
		}
	}

	fclose(stream);
	return 1;
}



/**
 * Funzione che si occupa di eliminare il nome del file specificato e di restituire in caso di successo il valore -1
 *
 * @pre		Il nome del file deve essere significativo(il file deve esistere)
 * @post	In caso il file esista esso deve essere eliminato correttamente
 */
int elimina_file_spesa(char nome_file[LUNG_NOME_FILE_SPESA]) {
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
 * e' vera(vedi funione controllo_soglia per avere un'idea sui valori restituiti) allora l'alimento viene passato alla funzione aggiorna_lista_spesa
 * Al termine viene restituito 1
 *
 * @pre		nessuna particolare pre condizione
 * @post	nessuna particolare post condizione
 */
int generatore_spesa_globale() {
	int num_alimenti_database;

	num_alimenti_database = conta_alimenti_database();

	if(num_alimenti_database == 0){
		return 0;
	}
	elimina_file_spesa(LISTA_SPESA_GLOBALE);

	alimento_database alimenti_database[num_alimenti_database];

	if (leggi_database_alimenti(alimenti_database) == 0 ) {
		return 0;

	} else {

		for(int i = 0; i < num_alimenti_database; i++) {
			//se la soglia e' maggiore di 0 e se la quantitá disponibile e' minore di tale soglia allora occorre memorizzare l'alimento
			if (controllo_soglia(alimenti_database[i].soglia_spesa, alimenti_database[i].nome))
				aggiorna_lista_spesa(alimenti_database[i].nome, LISTA_SPESA_GLOBALE);

		}

		return 1;
	}

	return 0;
}




/**
 * Funzione che si occupa di creare il file che conterrá gli alimenti che un utente specifico deve comprare.
 *
 * Per far ció la prima operazione da compiere e' l'autenticazione dell'utente. Qualora essa vada a buon fine viene sfruttato il nickname
 * dell'utente per creare un file della spesa personalizzato. La seconda operazione necessaria e' quella di estrarre dal menu settimanale
 * gli alimenti e le ricette che l'utente consuma giornalmente. Per ogni pasto viene fatto un confronto sulla tipologia dell'alimento.
 * Se siamo in presenza di una ricetta viene popolato un array di struct(di tipo ingrediente) che avrá tutti gli ingredienti che compongono la
 * ricetta. Se il numero di ingredienti di una data ricetta e' -1 vuol dire che la ricetta non e' presente nel database e viene notificato un messaggio
 * all'utente. Se viene restituito un valore diverso vuol dire che gli ingredienti sono candidati ad essere memorizzati nella lista della spesa,
 * ma occorre effettuare un controllo con la soglia e la quantitá disponibile. Sela quantitá disponibile e' inferiore alla soglia l'ingrediente viene
 * memorizzato. Stessa ragionamento per gli alimenti singoli presenti nel menú settimanale.
 *
 * @pre		Nessuna pre condizione
 * @post	Venga generato il file della spesa dell'utente
 */
int generatore_spesa_personale() {
	utente u;
	giorno menu;
	float soglia_spesa;

	//autenticazione(con controllo su esito) dell'utente
	if (autenticazione(&u) == -1){
		printf("Operazione di utenticazione fallita\n");
		return -1;
	}

	//generazione del nome del file che ospiterá la spesa personale
	char nome_file[LUNG_NOME_FILE_SPESA] = PREFIX_FILE_SPESA;
	strcat(nome_file, u.nickname);
	strcat(nome_file, SUFFIX_FILE_SPESA);

	elimina_file_spesa(nome_file);//eliminazione del file "vecchio" se presente

	for (int i_giorno = 1; i_giorno <= 7; i_giorno++) {

		estrai_giorno(&menu, u.nickname, i_giorno); //viene estratto il menu giornaliero

		for (int i_pasto = 0; i_pasto < NUM_PASTI; i_pasto++) {

			for (int i_alimento = 0; i_alimento < NUM_CIBI; i_alimento++) {

				if (menu.pasti[i_pasto].cibi[i_alimento].flag == FLAG_RICETTA) {//se l'alimento che si sta analizzando e' una ricetta
					//vengono estratti gli ingredienti
					int num_ingredienti;
					ingrediente ingredienti[MAX_INGREDIENTI];
					num_ingredienti = estrazione_ingredienti(menu.pasti[i_pasto].cibi[i_alimento].nome_cibo, ingredienti);

					if (num_ingredienti == -1) {//controllo che ci aiuta a capire se la ricetta non esiste nel database

						printf(
								"Non e' stato possibile estrarre gli alimenti che compongono la ricetta: %s\n",
								menu.pasti[i_pasto].cibi[i_alimento].nome_cibo);
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

				} else if(menu.pasti[i_pasto].cibi[i_alimento].flag == FLAG_ALIMENTO){
					//altrimenti se ció che si sta analizzando e' un alimento si effettua un controllo diretto sulla soglia e sulla quantitá
					//disponibile

					soglia_spesa = soglia_alimento(menu.pasti[i_pasto].cibi[i_alimento].nome_cibo);

					if(soglia_alimento(menu.pasti[i_pasto].cibi[i_alimento].nome_cibo) == -1){
						printf("Non e' stato possibile estrarre l'alimento perche' sconosciuto al database: %s\n",
								menu.pasti[i_pasto].cibi[i_alimento].nome_cibo);

					}else if(controllo_soglia(soglia_spesa, menu.pasti[i_pasto].cibi[i_alimento].nome_cibo) == 1){

						aggiorna_lista_spesa(menu.pasti[i_pasto].cibi[i_alimento].nome_cibo, nome_file);

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
	int lung_nome_alimento = strlen(nome_alimento);
	if(lung_nome_alimento < 1 || lung_nome_alimento > LUNG_NOME_ALIMENTO){
		return -1;

	}

	if(soglia_spesa < 0){
		return -1;

	}

	if (soglia_spesa > 0) {

		if (quantita_alimento(nome_alimento) < soglia_spesa) {
			return 1;

		} else {
			return 0;

		}

	} else {
		return 0;

	}

}


/*
 * calorie.c
 *
 *  Created on: 20 giu 2018
 *      Author: ciaone
 */

#include "calorie.h"

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

	utente persona;

	if (autenticazione(&persona)==1){

		assunzione cibo;

		int esito_input; //variabile per il controllo dell'input

		cibo.flag=FLAG_INPUT_LIBERO; //Valore che identifica un alimento/ricetta non presente nei database dello smart fridge

		cibo.quantita=0; //Quantità non necessaria

        do {
			printf("\nInserire il nome di cio' che hai assunto\n>");
			scanf("%20[a-zA-Z]", cibo.nome);

			//controllo input
			if(pulisci_stdin() == 1){
				esito_input = 0;
			}

			if(esito_input == 0){

				puts("Inserimento non valido. Ripeterlo.");
			}
		}while (esito_input == 0);

		do {
			printf("\nInserire calorie\n>");
			scanf("%hu", &cibo.kcal);

			if(pulisci_stdin() == 1){
					esito_input = 0;
			}

			if(esito_input == 0){
				puts("Inserimento non valido. Ripeterlo.");
			}
		}while (esito_input == 0);



		scrittura_diretta_assunzione (&cibo, persona.nickname);

		return 0;
	}

	return 1;

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
 * @pre Il campione non può essere pari a 0
 * @post Il valore restituito rappresenta l'apporto calorico calcolato.
 *
 */

unsigned short int calcolo_kcal(unsigned short int kcal, int campione, float q_consumata) {

	if (q_consumata==0){
		return 0; //se la quantità è nulla, ritorno zero
	}

	unsigned short int risultato_kcal;

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
 * nel campo della struct denominato nome. Imposterà il flag con quello passatogli in gresso e farà altrettanto con la quantità.
 * Fatto ciò, controllerà il flag. Se il flag identifica una ricetta, verrano create due variabile di tipo int (kcal_ricetta e
 * dosi_ricetta) utilizzate in combinazione con una funzione in moda da poter estrarre da un database di ricette
 * le kcal per un tot di dosi. Grazie a queste informazioni sarà possibile calcolare le kcal per il quantitativo assunto
 * dall'utente tramite una funzione.
 * Stesso lavoro verrà fatto in caso di un flag che identifiche un alimento. Infatti, verrà usanta una funzione diversa
 * per estrarre da un database apposito l'alimento che verrà memorizzato in una struct creata appositamente.
 * In fine, rimpita la struct cibo, verrà passata ad una funzione di scrittura su file.
 *
 * @pre Nome_consumo non può essere vuoto, flag_consumo può assumero solo i valori 1 o 0, il nickname deve appartenere
 * ad uno degli utenti, quindi non può essere vuoto.
 *
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */

int aggiorno_database_calorie(char nome_consumo[], int flag_consumo, float quantita_consumo, char nickname[]) {

	char nome_file[LUNG_FILE_CALORIE] = "../assunzioni_";
	strcat(nome_file, nickname);
	strcat(nome_file, ".dat");

	assunzione cibo;


	strcpy(cibo.nome, nome_consumo);
	cibo.flag=flag_consumo;
	cibo.quantita=quantita_consumo;

	if (cibo.flag==FLAG_RICETTA){ //flag

	  int  kcal_ricetta;
	  int dosi_ricetta;
	  estrai_kcal_ricetta(cibo.nome, &kcal_ricetta, &dosi_ricetta);
	  cibo.kcal=calcolo_kcal(kcal_ricetta, dosi_ricetta, cibo.quantita);

	} else if (cibo.flag==FLAG_ALIMENTO) {

		alimento_database ricerca_a;
		ricerca_alimento_database(cibo.nome, &ricerca_a);
		cibo.kcal=calcolo_kcal(ricerca_a.kcal, ricerca_a.campione_kcal, cibo.quantita);

	}

	scrittura_diretta_assunzione (&cibo, nickname);



	return 0;
}


/**
 * Funzione scrivi_data ():
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
int scrivi_data(char nickname[]) {

	data data_attuale = data_odierna();

	char nome_file[LUNG_FILE_CALORIE] = "../assunzioni_";
	strcat(nome_file, nickname);
	strcat(nome_file, ".dat");

	FILE* f;

	if ((f = fopen(nome_file, "wb"))==NULL){
		return -1;
	}

	fseek(f, 0, SEEK_SET);
	fwrite(&data_attuale, sizeof(data), 1, f);

	fclose(f);

	return 0;
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

	utente persona;

	if (autenticazione(&persona) == 1) {

		data data_letta;
		assunzione cibo;

		//Creazione del nome file da aprire
		char nome_file[LUNG_FILE_CALORIE] = "../assunzioni_";
		strcat(nome_file, persona.nickname);
		strcat(nome_file, ".dat");

		FILE* f;

		if((f = fopen(nome_file, "rb+"))==NULL){
			if(scrivi_data(persona.nickname)==-1) { //creazione e scrittura della data in caso il file non esistesse
				return -1;
			}
		}

		//Leggo la data posizionata all'inizio
		fseek(f, 0, SEEK_SET);
		fread(&data_letta, sizeof(data), 1, f);

		printf("Assunzioni per %hu/%hu/%hu\n\n", data_letta.giorno,
				data_letta.mese, data_letta.anno);

		while (fread(&cibo, sizeof(assunzione), 1, f) > 0) {
			if (cibo.kcal !=0){
			       printf("Nome: %s\nQuantita': %.2f\nkcal: %hu\n\n", cibo.nome,
					cibo.quantita, cibo.kcal);
			}
		}

		fclose(f);

		calcolo_kcal_totali(nome_file);
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

unsigned short calcolo_kcal_totali(char nomefile[]) {

	FILE* f;
	assunzione cibo;
	unsigned short tot_kcal=0;

	if((f = fopen(nomefile, "rb+"))==NULL){

		if(scrivi_data (nomefile)==-1) {
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

    if(tot_kcal>0){
    	printf("Calorie assunte in totale: %hu", tot_kcal);
    }
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

	utente persona;



	if (autenticazione(&persona) == 1) {

		FILE* stream;

		char nome_file[LUNG_FILE_CALORIE] = "../assunzioni_";
		strcat(nome_file, persona.nickname);
		strcat(nome_file, ".dat");

		assunzione nuova_assunzione;
		short int posizione; //varibile per memorizzare la posizione dove scrivere il nuovo record
		int esito_input;

		do {
			printf("Inserisci il nome del cosumo da modifcare\n>");
			scanf("%20[a-zA-Z]", nuova_assunzione.nome);
			if(pulisci_stdin() == 1){
				esito_input = 0;
			}

			if(esito_input == 0){
				puts("Inserimento non valido. Ripeterlo.");
			}
		}while (esito_input == 0);

		posizione=ricerca_assunzione_database (nuova_assunzione.nome, persona.nickname);


		//Se si presenta uno dei due casi, vuol dire che il file non esiste o l'elemento cercato non è stato trovato
		if (posizione==0 || posizione==-1){
			printf("Impossibile terminare la modifica.\nFunzione terminata.\n\n");
			return 0;
		}

		if (nuova_assunzione.flag==FLAG_INPUT_LIBERO){
			do {
				printf("\nInserire le nuove kcal (0 per elimanre il consumo)\n>");
				scanf("%hu", &nuova_assunzione.kcal);
				if(pulisci_stdin() == 1){
					esito_input = 0;
				}

				if(esito_input == 0){
					puts("Inserimento non valido. Ripeterlo.");
				}
			}while (esito_input == 0);
		} else {

				do {
					printf("Inserire nuova quantita' (0 per elimare il consumo)\n>");
				    scanf("%f", &nuova_assunzione.quantita);
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

						  estrai_kcal_ricetta(nuova_assunzione.nome, &kcal_ricetta, &dosi_ricetta);

						  nuova_assunzione.kcal=calcolo_kcal(kcal_ricetta, dosi_ricetta, nuova_assunzione.quantita);

					} else if (nuova_assunzione.flag==FLAG_ALIMENTO){

						alimento_database a;

						ricerca_alimento_database (nuova_assunzione.nome, &a);

						nuova_assunzione.kcal=calcolo_kcal(a.kcal, a.campione_kcal, nuova_assunzione.quantita);

					}

		 }

			if((stream = fopen(nome_file, "rb+"))==NULL){

				printf("Errore\n");
				return 1;

			}

			fseek(stream, sizeof(data)+posizione*sizeof(assunzione)-sizeof(assunzione), SEEK_SET);
			fwrite(&nuova_assunzione, sizeof(assunzione), 1, stream);

			return 0;

		} else {
			return 1;
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
short int ricerca_assunzione_database (char nome[], char nickname[]){

	assunzione lettura;
	short int posizione; //variabile di memorizzazione della posizione della struct da trovare

	abbassa_maiuscole (nome);

	char nome_file[LUNG_FILE_CALORIE] = "../assunzioni_";
	strcat(nome_file, nickname);
	strcat(nome_file, ".dat");

	FILE* f;

	if((f = fopen(nome_file, "rb+"))==NULL){
			return -1;
	}

	fseek(f, sizeof(data), SEEK_SET); //sposto il puntatore oltre la data scritta ad inzio file

	while(feof(f) == 0){

				fread(&lettura, sizeof(assunzione), 1, f);
				posizione++;//leggo, incremento
				if(strcmp(lettura.nome, nome) == 0){
					return posizione;
				}
			}
	fclose (f);

	return 0;
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

	char nome_file[LUNG_FILE_CALORIE] = "../assunzioni_";
	strcat(nome_file, nickname);
	strcat(nome_file, ".dat");

	FILE* stream;
	data controllo_data;
	int differenza;

	if((stream = fopen(nome_file, "rb+"))==NULL){
			scrivi_data(nickname);
	}

	fseek(stream, 0, SEEK_SET);
	fread(&controllo_data, sizeof(data), 1, stream);

	if (diff_date(&differenza, controllo_data, data_odierna()) != 0) {
		//reset
		scrivi_data(nickname);
	}

	fseek(stream, 0, SEEK_END);
	fwrite(cibo, sizeof(assunzione), 1, stream);

	return 0;
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

	if (autenticazione(&persona)== 1){

		char nome_file[LUNG_FILE_CALORIE] = "../assunzioni_";
		strcat(nome_file, persona.nickname);
		strcat(nome_file, ".dat");

		int kcal_giornaliere;

		estrai_kcal_menu(&kcal_giornaliere, persona.nickname, giorno_odierno());

		 printf("Assunzioni persona media: 2000 kcal: ");
		 for (int i=0; i<KCAL_MEDIE_GIORNALIERE; i+=CAMPIONE_ISTOGRAMMI){
			 printf("%c", 219);
		 }
		 printf("\nIl tuo obiettivo: %d", kcal_giornaliere);
		 for (int i=0; i < kcal_giornaliere; i+=CAMPIONE_ISTOGRAMMI){
			 printf("%c", 219);
		 }
		 printf("\nIl tuo stato attuale: \n");
		 for (int i=0; i < calcolo_kcal_totali(persona.nickname); i+=CAMPIONE_ISTOGRAMMI){
		 	 printf("%c", 219);
		 }

	}

}



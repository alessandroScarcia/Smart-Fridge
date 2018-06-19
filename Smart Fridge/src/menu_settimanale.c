/*
 * menu_settimanale.c
 *
 *  Created on: 25 mag 2018
 *      Author: Michela
 */
#include "menu_settimanale.h"
#include "utenti.h"

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
void inizializzazione (){

	giorno menu[GIORNI_SETTIMANA]; //vettore che conterrà i 7 menu corrispondenti ai 7 giorni della settimana
	utente u;

	if (autenticazione(&u)==1){ //Se l'autenticazione va a buon fine esegui

		int i;

		for(i=0;i<GIORNI_SETTIMANA;i++){     //scorri nel vettore menu

		inizializzazione_giorno(&menu[i], i); //inizializza il singolo elemento appartenente al vettore
		inizializzazione_pasti(&menu[i]);

		}

		inizializzazione_file_menu(menu, u.nickname); //scrivi su file il vettore inizializzato



	}
}


/**
 * Funzione inizializzazione_giorno ():
 *
 * La funzione ha il compito di assegnare alla variabile giorno appartenente alla struct passatagli il nome di un giorno della settimana,
 * scegliendo tale nome sulla base della sua posizione nel vettore di struct a cui appartire. Tale posizione gli è passata dalla funzunzione
 * chiamante
 *
 * A tal scopo esegue un controllo sull'indice passatogli per l'identificazione della sua possizione nel vettore a cui appartiene.
 * In base al suo valore, verrà chiamata una funzione che copierà nella variabile di tipo stringa "giorno" una stringa corrispondente
 * al nome di uno dei giorni della settimana.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */
int inizializzazione_giorno (giorno* menu, int indice) {

			switch (indice) {
			case 0:
				strcpy(menu->giorno, "Lunedi");
				break;
			case 1:
				strcpy(menu->giorno, "Martedi");
				break;
			case 2:
				strcpy(menu->giorno, "Mercoledi");
				break;
			case 3:
				strcpy(menu->giorno, "Giovedi");
				break;
			case 4:
				strcpy(menu->giorno, "Venerdi");
				break;
			case 5:
				strcpy(menu->giorno, "Sabato");
				break;
			case 6:
				strcpy(menu->giorno, "Domenica");
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
int inizializzazione_pasti (giorno* menu) {

	short int i;


	for (i=0;i<NUM_PASTI;i++){ //scorrimento nel vettore pasto


		//ad ogni case, copio una stringa che identifica quel pasto
		switch (i) {
		case 0:
			strcpy(menu->pasto[i].nome_pasto, "Colazione");

			inizializzazione_alimenti(menu, i); //inizializzo i singoli alimenti del pasto
			break;
		case 1:
			strcpy(menu->pasto[i].nome_pasto, "Spuntino");

			inizializzazione_alimenti(menu, i);
			break;
		case 2:
			strcpy(menu->pasto[i].nome_pasto, "Pranzo");

			inizializzazione_alimenti(menu, i );
			break;
		case 3:
			strcpy(menu->pasto[i].nome_pasto, "Snack");

			inizializzazione_alimenti(menu, i);
			break;
		case 4:
			strcpy(menu->pasto[i].nome_pasto, "Cena");

			inizializzazione_alimenti(menu, i );
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
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */
int inizializzazione_alimenti (giorno* menu, short int num_pasto){

	int i;

	for(i=0;i<=NUM_ALIMENTO;i++){ //scorrimento nel vettore alimenti

		menu->pasto[num_pasto].alimento[i].flag=-1; //imposto un flag che non identifica ne un singolo alimento ne una ricetta (flag non significativo)


		strcpy( menu->pasto[num_pasto].alimento[i].nome_cibo, ""); // Copio una striga vuota nel nome dell'i-esimo alimento

		menu->pasto[num_pasto].alimento[i].quantita=0; // imposto una quantità pari a 0

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
int inizializzazione_file_menu (giorno write_menu[], char nome_utente[]){

	//Genero il nome del file
	char nomefile[MAX_LUNG_NOMEFILE]="menu_";
	strcat(nomefile, nome_utente);
    strcat(nomefile, ".dat");
    //fine generazione nome file

	FILE *f;
	int i;

	f=fopen(nomefile, "wb+");

	//scrivo uno per uno i singolo elementi del vettore di struct passato in ingresso
	for(i=0;i<GIORNI_SETTIMANA;i++){

	fseek(f, 0, SEEK_END); //posizione il puntatore alla fine del file per permettere la scrittura in ordine rispetto ai giorni della settimana
	fwrite(&write_menu[i],sizeof(giorno),1, f);

	}

	fclose (f);

	return 0;

}


/**
 * Funzione visualizza_database_menu ():
 *
 * La funzione ha il compito di aprire un file, il cui nome verrà generato grazie al paramentro in ingresso nome_utente ed una serie di concatenazioni
 * con nomefile, e leggere il suo contenuto fino al raggiungimento della fine del file.
 * Tale lettura sarà possibile grazie alla funzione fread che momerizzerà ciò che legge in menu e
 * passerà esso ad una funzione che si occuperà della stampa su schermo.
 *
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */
int visualizza_database_menu (char nome_utente[]){


    //Genero il nome del file
	char nomefile[MAX_LUNG_NOMEFILE] = "menu_";
	strcat(nomefile,nome_utente);
	strcat(nomefile,".dat");
	//fine generazione nome file

	giorno menu;
	FILE* f;


	if((f=fopen(nomefile,"rb"))==NULL){ //Se il file non viene aperto
		printf("\nIMPOSSIBILI APRIRE IL FILE");
		return 1;
	}


	while(fread(&menu, sizeof(menu),1,f)>0){

	     stampa_menu(&menu);

	}
	fclose (f);


	return 0;
}




/**
 * Funzione scelta_pasto ():
 *
 * La funzione ha il compito di permettere all'utente di scegliere il menu di quale giorno su cui effettuare l'aggiormento,
 * per poi scegliere ulteriormente il pasto su cui effetturare le modifiche.
 *
 * Al tal scopo viene crata una struct di tipo utente per permettere l'autentificazione dell'utente, se essa va a buon fine, sarà possibile scegliere
 * il giorno ed estrarre tale giorno dal file attraverso la funzione estrazione_struct. Verrà stampato su schermo il menu estratto grazie alla procedura
 * stampa_menu ed a questo punto verrà chiesto quale pasto si intende modificare (colazione, spuntino di metà mattina, ecc). Memorizzata la risposta
 * inserita da tastiera, verrà decrementata tale risposta (il numero corrispodente alla risposta permette l'accesso all'array pasto, che però parte da
 * 0 e quindi richiede necessariamente il decremento di tale risposta che riceve numeri da 1 a 5).
 * A questo punto verrà chiamata una funzione che interviene sulla modifica del singolo alimento, fatto ciò la procedura scrivi_menu memorizzerà su file
 * il nuovo menu.
 * In fine, verrà visualizzato il database del menu settimanale dell'utente per intero.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */
int scelta_pasto (){

	short int num_pasto;
	giorno menu;
	utente u;
	int i;

	if (autenticazione (&u)==1){ //Se l'autenticazione va a buon fine esegui:

	printf("\n1.Lunedi\n2.Martedi\n3.Mercoledi\n4.Giovedi\n5.Venerdi\n6.Sabato\n7.Domenica\n (Inserire il numero corrispondente al giorno)");
	scanf("%d", &i); //Scelta e memorizzazione della posizione della struct
	estrazione_struct(&menu, u.nickname, i); //da estrarre da file

	stampa_menu(&menu); //stampa struct estratta


    //Scelta del pasto da modificare
	printf("Quale pasto intendi modificare? \n");
	printf("1.Colazione\n2.Spuntino di meta' mattina\n3.Pranzo\n4.Snack pomeridiano\n5.Cena\n");
	scanf("%hd", &num_pasto); //memorizzazione scelta

	 num_pasto--;

	scelta_alimenti (&menu, num_pasto);


	scrivi_menu(&menu, u.nickname, i);

	visualizza_database_menu(u.nickname);

	return 0;
	} else {
		return 1;
	}
}


/**
 * Funzione scelta_alimenti ():
 *
 * La funzione ha il compito di modificare un singolo alimento su richiesta dell'utente
 *
 * Al tal scopo viene riesco all'utente di decidere quale degli alimenti presenti modificare, sucessivamente di inserire un flag di riferimento
 * per distinguere l'inserimento di un singolo alimento dall'inserimento di una ricetta.
 * In fine, viene richiesto il nome di tale alimento/ricetta.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */
int scelta_alimenti (giorno* menu, short int num_pasto){

	short int risposta;

	//Controlli sulla stampa solo degli elementi non vuoti

	//Scelta dell'alimento da sovrascrivere
	printf("Scegli l'alimento o la ricetta da modificare: ");
	scanf("%hd", &risposta);


	printf("\nInserire 1 se si sta inserendo una ricetta, 0 se si sta inserendo un alimento singolo: ");
	scanf ("%hd", &menu->pasto[num_pasto].alimento[risposta-1].flag);

	printf("\nInserire il nome: ");
	scanf("%s", menu->pasto[num_pasto].alimento[risposta-1].nome_cibo);

	printf("\nInserire porzioni (in kg, litri o numero di porzioni): ");
	scanf("%f", &menu->pasto[num_pasto].alimento[risposta-1].quantita);

	return 0;

}



/**
 * Funzione estrazione_struct ():
 *
 * La funzione ha il compito di estrarre dal file menu di un utente la struct all'n-esima posizione.
 *
 * Al tal scopo viene riesco passata alla funzione un puntatore a menu, dove verrà memorizzata la struct estratta. Il nome_utente servirà per
 * generare il nome del file corrispondente al menu dell'utente. Fatto ciò, verrà a aperto il file, estratta la struct alla posizione indicata da n
 * e momorizzata in menu. Il file viene chiuso e la funzione termina.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post Il valore restituito rappresenta l'esito della funzione.
 *
 */
int estrazione_struct (giorno* menu, char nome_utente[], int n){


	FILE *f;


	char nomefile[MAX_LUNG_NOMEFILE] = "menu_";
	strcat(nomefile,nome_utente);
	strcat(nomefile,".dat");

	f=fopen(nomefile, "rb");

	fseek(f, n*sizeof(giorno)-sizeof(giorno), SEEK_SET); //posiziono il puntatore all'n-esima posizione
	fread(menu, sizeof(giorno), 1, f);

	fclose(f);

	return 0;

}


/**
 * Procedura scrivi_menu ():
 *
 * La procedura ha il compito di scrivere su file il dato strutturato menu passatogli in ingresso all'i-esima posizione.
 *
 * Al tal scopo viene generato il nome del file attraverso una serie di concanetazioni che includono nome_utente passatogli in ingresso.
 * fatto ciò viene aperto il file, posizionato il puntatore all'i-esima posizione e scritto menu in tale posizione.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post no
 *
 */
void scrivi_menu (giorno* menu, char nome_utente[], int i){


	FILE* f;
	char nomefile[MAX_LUNG_NOMEFILE] = "menu_";
	strcat(nomefile,nome_utente);
	strcat(nomefile,".dat");


	f=fopen(nomefile, "rb+");


	fseek(f, i*sizeof(giorno)-sizeof(giorno), SEEK_SET);
	fwrite(menu, sizeof(giorno), 1, f);

	fclose(f);

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
void stampa_menu (giorno* menu){

	short int j, k;

	printf("*********************************************************************************************************");
	printf("\n%s\n", menu->giorno);
	for (j=0;j<5;j++){
		printf("- %s:\n",menu->pasto[j].nome_pasto );
		for (k=0;k<5;k++){

		printf("\t%s\n", menu->pasto[j].alimento[k].nome_cibo);
		}
	}
	printf("*********************************************************************************************************\n");

}



/**
 * Funzione conta_ricette_menu ():
 *
 * La funzione ha il compito analizzare il menu interessato per individuare il numero presente in quel dato menu.
 *
 * Al tal scopo vengono passati alla funzione un nome_utente che permettere la generazione del nome del file da cui estrarre la struct e
 * un variabile che indentifica in quale posizione estrarre la struct.
 * Quindi, viene chiamata la funzone estrazione_struct, che permettere la memorizzaizone della struct in menu.
 * Sucessivamente vengono implementati due cicli for innestati che permettore lo scorrimento prima nel vettore pasto e poi un ulteriore scorrimento
 * nel vettore alimenti. Viene analizzato il flag appartenente al alimenti ad ogni singolo scorrimento.
 * Grazie ad un if quaando si incontra un flag pari a 1, viene aumentato un contatore.
 * Tale contatore viene restituito una volta terminata l'analisi di tutta la struct.
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post num_ricette che restituisce il numero delle ricette presente nel menu di un giorno specifico.
 *
 */
short int conta_ricette_menu(char nome_utente[], short int giorno_x){
    short int num_ricette;
    giorno menu;

    short int j, k;

    estrazione_struct (&menu,nome_utente, giorno_x);

    for (j=0;j<5;j++){

    	for(k=0;k<5;k++){

    		if (menu.pasto[j].alimento[k].flag==1){
    			num_ricette++;
    		}
    	}
    }


	return num_ricette;
}


/**
 * Procedura ricette_presenti ():
 *
 * La procedura ha il compito popolare un vettore passotogli in ingresso con i nomi delle ricette presenti nel menu di un giorno specifico.
 *
 * Al tal scopo vengono passati alla funzione un nome_utente che permettere la generazione del nome del file da cui estrarre la struct,
 * un variabile che indentifica in quale posizione estrarre la struct e un vettore bidimensionale su cui memorizzare i nomi delle ricette
 * Quindi, viene chiamata la funzone estrazione_struct, che permettere la memorizzaizone della struct in menu.
 * Sucessivamente vengono implementati due cicli for innestati che permettore lo scorrimento prima nel vettore pasto e poi un ulteriore scorrimento
 * nel vettore alimenti. Viene analizzato il flag appartenente al alimenti ad ogni singolo scorrimento.
 * Grazie ad un if quaando si incontra un flag pari a 1, viene copiato il nome della variabile con tale flag nell i-esima posizione del vettore
 * bidimensionale (vet_ricette), quindi viene aumentato il contatore i per evitare una sovrascrittura nel vet_ricette.
 *
 *
 * @pre Non vi sono specifiche pre-condizioni per la funzione
 * @post no.
 *
 */
void ricette_presenti (char vet_ricette[], char nome_utente[], short int giorno_x){
	 short int j, k, i;
     giorno menu;


     estrazione_struct (&menu,nome_utente, giorno_x);

     for (j=0;j<5;j++){

         	for(k=0;k<5;k++){

         		if (menu.pasto[j].alimento[k].flag==1){
         			strcpy(&vet_ricette[i], menu.pasto[j].alimento[k].nome_cibo );
         			i++;
         		}
         	}
         }

}

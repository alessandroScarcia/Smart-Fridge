/*
 * utenti.c
 *
 *  Created on: 10 mag 2018
 *      Author: Michela
 */

#include"utenti.h"


/*
Sommario: Creazione di una variabile di tipo utente, che verrà riempita attraverso input da tastiera.
          La funzione chiamerà a sua volta un'altra funzione, che eseguirà altre specifiche spiegate in seguito
Parametri: i rappresenta n-esimo utente registrato, quindi l'ultimo indice assegnato.
*/
int Salvataggio_utenti(int i){

		utente persona;

		persona.indice=i+1;   //Essendo i l'ultimo indice assegnato, verrà incrementato di uno per far in modo che ogni utente abbia un indice univoco

		printf("\nInserisci il nome utente: ");
		scanf("%s",persona.nome);

		printf("\nInserisci cognome: ");
		scanf("%s",persona.cognome);

		printf("\nInserisci la tua prima preferenza: ");
		scanf("%s", persona.preferenze[0]);


		printf("\nInserisci la tua seconda preferenza: ");
		scanf("%s", persona.preferenze[1]);

		printf("\nInserisci la tua terza preferenza: ");
		scanf("%s", persona.preferenze[2]);
        //controllo sulla prefenza

		Generatore_Password(&persona);
		Aggiorna_databaseutenti(&persona);
		visualizza_database();
		//Stampa_utente(persona.indice);

        return 1;
}

/*Sommario: Funzione base per la creazione della password, con la possibilità di scelta tra la generazione casuale
            e l'inserimento manuale da parte dell'utente esterno, il tutto attraverso la chiamata di fuzioni oppurtune.
Parametri: puntatore di tipo utente, dove sono memorizzati gli offset dell'utente, quest'ultimo verrà passato alle funzioni chiamate.
*/
int Generatore_Password(utente* persona){


	char response;  //variabile per la memorizzazione della risposta inserita dall'utente


	printf("\nPreferisci la generazione automantica della password? (inserire y per il si, n per il no): ");
			response=getchar();
			fflush(stdin); //Pulisco il buffer
    while(response=='\n'){ //Ignoro l'invio nell'acquisizione del carattere
    	response=getchar();
    }
	//Un while utile nel caso l'utente decidesse di inserire una risposta diversa da quelle accettate
	/*function_response()
	 *
	//Un while utile nel caso l'utente decidesse di inserire una risposta diversa da quelle accettate
	while (response!= 'y' && response!='n' && response!='Y' &&  response!='N' ){
		   printf("Risposta non valida!\nInserire 'y' per la generazione di una password, 'n' per crearla:");
		   response=getchar();
		   fflush(stdin);
	}*/


	//Controllo sulla risposta, che nel caso sia affermativa una funzione genererà una password automaticamente
	if (response=='y'||response=='Y'){
	   AutoGeneratore_Password(persona);
	}
	else {
	//In caso contrario, verrà chiamata una funzione per oppurtini controlli sulla password inserita dall'utente
		Inserimento_password_manuale(LUNGHEZZA_PASSWORD, persona);
	}

    return 0;
}

/*Sommario: Funzione per la generazione casuale della password. Possibilità di scelta tra diversi livelli di sicurezza
            Livello 1 = tre caratteri totali tra numeri e caratteri speciali;
            Livello 2 = la somma tra caratteri speciali e numeri deve essere compresa tra 3 e 5;
            Livello 3= la somma tra caratteri sepciali e numeri deve essere pari a 5;
            La generazione della password verrà fatta sfruttando la dabella ASCII, utilizzando una funzione che genererà
            un numero compreso tra 33 e 122, corrispondente ad un numero, una lettere o un carattere speciale nella tabella ASCII.
            Una funzione chiamata alla fine svolgerà il compito di scrittura nel file.
Parametri: puntatore di tipo utente, dove sono memorizzati gli offset dell'utente, quest'ultimo verrà passato alle funzione chiamata per
           la scrittura nel file .cvs.
*/
int AutoGeneratore_Password(utente* persona){

	int livello_sicurezza=0;
	int numeri_caratteri_speciali=0; //inizializza il numero di caratteri speciali
	int numeri_caratteri_cifre=0; //inizializza il numero di cifre


	strcpy(persona->password, ""); // Rendo il campo della passeword vuoto

	printf("Livelli di sicurezza:\n- Livello 1(Basso)\n- Livello 2(medio)\n- Livello 3(alto).\n");
    scanf("%d", &livello_sicurezza); //assegnazione del valore ottenuto dalla scanf


    if(livello_sicurezza==SICUREZZA_MINIMA){


				do{ //ripeti fino a quando non é stato inserito almeno un numero e un carattere speciale e la loro quantitá é minore di 3

					numeri_caratteri_speciali=0; //inizializza il num di caratteri speciali
					numeri_caratteri_cifre=0; //inizializza il num di cifre

					for(int j=0; j<LUNGHEZZA_PASSWORD; j++){ //ripeti per la lunghezza della password

						persona->password[j]= '!'+(rand() % CARATTERI_ASCII_RISERVATI); //genera un carattere comreso tra 33 e 122 della tabella del codice ASCII

						if(!isalnum(persona->password[j])){
							numeri_caratteri_speciali++;
						}

						if(isdigit(persona->password[j])){
							numeri_caratteri_cifre++;
						}

						if(j==LUNGHEZZA_PASSWORD-1){ //all'ultima posizione inserisci il carattere terminatore
							persona->password[j]= '\0';
						}


					}

				}while(numeri_caratteri_speciali<NUM_CARATTERI_RICHIESTI || numeri_caratteri_cifre<NUM_CARATTERI_RICHIESTI || numeri_caratteri_speciali+numeri_caratteri_cifre>=SOMMA_CARATTERI_RICHIESTI_MIN);
		}else if(livello_sicurezza==SICUREZZA_MEDIA){

		  do{ //ripeti fino a quando non é stato inserito almeno un numero e un carattere speciale e la loro quantitá é tra 3 e 5

					numeri_caratteri_speciali=0;
					numeri_caratteri_cifre=0;

					for(int j=0; j<LUNGHEZZA_PASSWORD; j++){

						persona->password[j]= '!'+(rand() % CARATTERI_ASCII_RISERVATI);

						if(!isalnum(persona->password[j])){
							numeri_caratteri_speciali++;
						}

						if(isdigit(persona->password[j])){
							numeri_caratteri_cifre++;
						}

						if(j==LUNGHEZZA_PASSWORD-1){
							persona->password[j]= '\0';
						}

					}

				}while(numeri_caratteri_speciali<NUM_CARATTERI_RICHIESTI || numeri_caratteri_cifre<NUM_CARATTERI_RICHIESTI || numeri_caratteri_speciali+numeri_caratteri_cifre<SOMMA_CARATTERI_RICHIESTI_MIN || numeri_caratteri_speciali+numeri_caratteri_cifre>SOMMA_CARATTERI_RICHIESTI_MAX  );
		}else{

			do{//ripeti fino a quando non é stato inserito almeno un numero e un carattere speciale e la loro quantitá é tra 3 e 5

					numeri_caratteri_speciali=0;
					numeri_caratteri_cifre=0;

					for(int j=0; j<LUNGHEZZA_PASSWORD; j++){

						persona->password[j]= '!'+(rand() % CARATTERI_ASCII_RISERVATI);

						if(!isalnum(persona->password[j])){
							numeri_caratteri_speciali++;
						}

						if(isdigit(persona->password[j])){
							numeri_caratteri_cifre++;
						}

						if(j==LUNGHEZZA_PASSWORD-1){
							persona->password[j]= '\0'; //inserimento carattere terminatore all'ultima posizione della stringa
						}

					}

				}while(numeri_caratteri_speciali<NUM_CARATTERI_RICHIESTI || numeri_caratteri_cifre<NUM_CARATTERI_RICHIESTI || numeri_caratteri_speciali+numeri_caratteri_cifre<=SOMMA_CARATTERI_RICHIESTI_MAX  );


		}
		printf("Password: %s\n\n", persona->password);

		return 1;
}



/*Sommario: Funzione per i controlli opportuni della password. La password deve essere di otto elementi, di cui almeno una lettera maiuscola e un numero.
            Inoltre calcolerà il livello di sicurezza, nel caso di corretta dell'input, e la stampo su schermo.
            Livello 1 = quattro lettere, di cui una maiuscola, e quattro numeri prelevati da un vettore contenenti i numeri da 1 a 9 (zero non utilizzato
                        per evitare il riscio che l'utente possa confonderlo con la lettera O);
            Livello 2 = un carattere speciale più i requisiti standard;
            Livello 3= più di un carattere speciale e almeno due lettere maiuscole;
            Una funzione chiamata alla fine svolgerà il compito di scrittura nel file.
Parametri: puntatore di tipo utente, dove sono memorizzati gli offset dell'utente, quest'ultimo verrà passato alle funzione chiamata per
           la scrittura nel file .cvs;
           LENGTH per decidere la lunghezza minima della password.
*/
int Inserimento_password_manuale (int length, utente *persona){

	int livello=0, digit=0, carattere=0, special=0, j;
	//livello serve per il calcolo del livello di sicurezza della password inserita dall'utente, nel caso sia idonea;
	//digit contatore dei numeri; carattere contatore dei caratteri; special contatore dei caratteri sppeciali;
	// j contatore di un ciclo.


	printf("Inserisci un password di %d caratteri (almeno una lettera maiuscola ed un numero): ", length-1);
	scanf("%s", persona->password);  //Acquisizione password da tastira

	//Controllo sul nemero di caratteri inseriti, stampa un messaggio di errore nel caso sia superiore ad 8 caratteri
	while (strlen(persona->password)!=length-1){
		printf("Password di dimensione non corretta.Inserisci un password di esattamente 8 caratteri (almeno una lettera maiuscola ed un numero): ");
	    scanf("%s", persona->password);
	}

	 while (livello==0){
         //Attraberso un for mi muovo nell'array di caratteri per capire di cosa c'è in ciasciuna posizione (numero,carattere,ecc..)
		 for (j=0;j<length-1;j++) {

		    if(isdigit(persona->password[j])) {
				   digit++;                                                //Aumento il contatore dei numeri

			 } else if (isupper(persona->password[j])) {

				carattere++;                                               //Aumento il contatore delle maiuscole

			 } else if(isalpha(persona->password[j])==0) {

					special++;                                             //Nel caso non sia ne un numero, ne una lettera (Maiuscola o minuscola) automaticamente lo associa ad un carattere speciale

			 }

		 }

		 if(digit>0 && carattere>0) {

				livello++;

		 }
	 }

	livello=1;                                                               //Parto con il presupposto che la password sia di livello 1

	//Nel caso ci sia più di un carattere speciale e più lettere maiuscole, la password la considero di livello 3
	if (carattere>1 && special>=2) {

	   livello=3;

	} else if (special>0) {  //Nel caso ci sia solo un carattere speciale più i requisiti standard la considere di livello 2

		livello=2;

	}

	//Stampa della password
	printf("\nLa password e'  ");
	printf("%s",persona->password);

	printf("\n");
	//fine stampa

	printf("La tua password e' di livello %d\n\n", livello);


	return 1;

}


/*Sommario: Funzione per la modifica del nome appartenente allo stream con l'id corrispondente a quello inserito dall'utente esterno.
            Verrà aperto il file .cvs contenente gli stream, attraverso un while il puntatore verrà spostato e lo stream puntato letto
            fino a quanto persona.indice non corrisponderà all'id inserito dall'utente.
            Trovato lo stream corrispondente, verrà stampato il nome e verrà chiesto l'inserimento del nuove nome.
            Le modifiche verranno applicate solo se la funzione Identificazione restituirà un valore di verità (TRUE).
            Il nuovo stream verrà scrutto nel file .cvs;
No Parametri.

*/
int Modifica_nome(){

	FILE *f;
    char filename[LUNG_NOME_FILE];
    strcpy(filename,"");
    strcat(filename,"../database-utenti.csv");

	utente persona;
	char new_name[NOME_COGNOME_LENGTH];
    int id;    //variabile per la memorizzazione dell'indice utente inserito da tastiera

    printf("Inserisci il tuo id identificativo:");
    scanf("%d", &id); //acquisizione indice utente sul quale effutare la modifica

    f=fopen(filename, "rb+");

    /*
	   if ((f=fopen("C:\Users\Michela\git-utenti\01-Utenti\01fileutenti.cvs","rb+"))==NULL)
	   {
	     printf ("Err\n");
	     return;
	   } */

       //Scorrimento del puntatore sugli stream, lettura degli stessi, ricerca dello stream con id corrispondente
	   while((fread(&persona,sizeof(persona),1,f)) > 0)
	   {

	    if(persona.indice==id) //stream trovato
	    {
	       break; //esco dal while
	    }
	  }

	   printf("Il tuo nome attuale e' %s\n", persona.nome); //stampa del nome prima che venga effettuata la modifica

	  	      printf("Inserire il nuovo nome: ");
	  	      scanf("%s", new_name); //acquisizione del nuovo nome

	  	      strcpy(persona.nome,new_name);

	  	      //Se identificazione vera, la struct con il nuovo nome verrà scritta sul file
	  	      if (Identificazione (persona.password, LUNGHEZZA_PASSWORD )== TRUE){
	  			  fseek(f, ftell(f)-sizeof(persona), SEEK_SET);
	  			  fwrite(&persona,sizeof(persona),1,f);
	  	      } else { //In caso di identificazione fallita, avviso ad utente esterno, la nuova struct non verrà memorizzata su file

	  	    	  printf("\nIdentificazione fallita, non e' stato possibile effetturare le modifiche richieste.\n");

	  	      }
	  fclose (f);
      return 0;

}

/*Sommario: Funzione per la modifica del cognome appartenente allo stream con l'id corrispondente a quello inserito dall'utente esterno.
            Verrà aperto il file .csv contenente gli stream, attraverso un while il puntatore verrà spostato e lo stream puntato letto
            fino a quanto persona.indice non corrisponderà all'id inserito dall'utente.
            Trovato lo stream corrispondente, verrà stampato il nome e verrà chiesto l'inserimento del nuove nome.
            Le modifiche verranno applicate solo se la funzione Identificazione restituirà un valore di verità (TRUE).
            Il nuovo stream verrà scrutto nel file .cvs;
No Parametri.
*/
int Modifica_cognome(){
	FILE *f;
    char filename[LUNG_NOME_FILE];
    strcpy(filename,"");
    strcat(filename,"../database-utenti.csv");

	char new_surname[NOME_COGNOME_LENGTH];
	utente persona;
	int id;


	printf("Inserisci il tuo id identificativo:");
	scanf("%d", &id); //acquisizione indice dell'utente da modificare

	f=fopen(filename, "rb+");

	printf("Inserire il nuovo cognome: ");
	scanf("%s", new_surname); //acquisizione nuovo cognome

	//Scorrimento del puntatore sugli stream, lettura degli stessi, ricerca dello stream con id corrispondente
    while((fread(&persona,sizeof(persona),1,f)) > 0) { //assegno la lettura ad una variabile di tipo intero, se la lettura restituisce valori idonei read sarà maggiore di 0
		if(persona.indice==id) {

        break; //stream trovato, esco dal while

	    }
   }

	  strcpy(persona.cognome,new_surname);

      //Come per la funzione modifica_nome, la scrittura su file avverà solo se Identificazione restituirà TRUE
	  if (Identificazione (persona.password, LUNGHEZZA_PASSWORD )==TRUE){
		  printf("Identificazione avvenuta con successo, modifica effettuata.\n");
		  fseek(f, ftell(f)-sizeof(utente), SEEK_SET);
		  fwrite(&persona,sizeof(persona),1,f);
	  } else {
		  printf("\nIdentificazione fallita, non e' stato possibile effetturare le modifiche richieste.\n");
	  }

   fclose (f);
   return 0;
}

/*Sommario: Funzione per la modifica delle preferenze appartenente allo stream con l'id corrispondente a quello inserito dall'utente esterno.
            Verrà aperto il file .csv contenente gli stream, attraverso un while il puntatore verrà spostato e lo stream puntato letto
            fino a quanto persona.indice non corrisponderà all'id inserito dall'utente.
            Trovato lo stream corrispondente, verranno stampate le preferenze, verrà chiesto l'inserimento del numero corrispondente alla preferenza da modificare e
            verrà richiesta con quale sostituirla.
            Le modifiche verranno applicate solo se la funzione Identificazione restituirà un valore di verità (TRUE).
            Il nuovo stream verrà scrutto nel file .cvs;
No Parametri.

*/
int Modifica_preferenza(){

	FILE *f;
	char filename[LUNG_NOME_FILE];
	strcpy(filename,"");
	strcat(filename,"../database-utenti.csv");

	int id;
	utente persona;
	int pref_modifica;

    printf("Inserisci il tuo id identificativo:");
	scanf("%d", &id); //acquisizione indice dell'utente da modificare

	f=fopen(filename, "rb+");
	while((fread(&persona,sizeof(persona),1,f)) > 0) {

		  if(persona.indice==id) {
			 break; //stream trovato, uscita dal while
		  }
	 }

	printf("Le tue preferenze attuali sono:\n1.%s\n2.%s\n3.%s", persona.preferenze[0], persona.preferenze[1],persona.preferenze[2]);

	 do {

		 printf("Quale desideri modificare? (Inserisci 1, 2 o 3)");
		 scanf("%d", &pref_modifica); //acquisizione della preferenza da modificare

		 //controlli da togliere e sostituire con quelli fatti da Alessandro (?)
		 if (pref_modifica!=1 && pref_modifica!=2 && pref_modifica!=3){ //Nel caso di valori non ammessi
			printf("Valore non valido.\n");
	     }

	 }while (pref_modifica!=1 && pref_modifica!=2 && pref_modifica!=3 ); //while per ripetere l'operazione fino a quando i valori inseriti non saranno idonei

	 //switch per il controllo di quale preferenza si è scelta
	switch (pref_modifica){
	case 1:
		printf("Hai scelto di modificare %s\nInserire nuova preferenza: ", persona.preferenze[0]);
		scanf("%s", persona.preferenze[0]); //modifica della prima preferenza
		break;

	case 2:
		printf("Hai scelto di modificare %s\nInserire nuova preferenza: ", persona.preferenze[1]);
		scanf("%s", persona.preferenze[1]); //modifica della prima preferenza
		break;

	case 3:
		printf("Hai scelto di modificare %s\nInserire nuova preferenza: ", persona.preferenze[2]);
		scanf("%s", persona.preferenze[2]); //modifica della prima preferenza
		break;

	 } //fine switch (Implementare default per correttezza ?)
	/*Nel main implementare:
	do{
	printf("Desideri modificare altre preferenze? (Inserire y per il si, n per il no)")
	response=getchar();
			fflush(stdin); //Pulisco il buffer (questa funzione non funziona su repl)

			         //Un while utile nel caso l'utente decidesse di inserire una risposta diversa da quelle accettate
			while (response!= 'y' && response!='n' && response!='Y' &&  response!='N'){
		           printf("Risposta non valida, inserire 'y' per la generazione di una password, 'n' per crearla:");
				   response=getchar();
			}
	*/

	 //Come per le funzioni modifica_nome e modifica_cognome, la scrittura su file avverà solo se Identificazione restituirà TRUE
	if (Identificazione (persona.password, LUNGHEZZA_PASSWORD )==TRUE) {
		printf("Identificazione avvenuta con successo, modifica effettuata.\n");
		fseek(f, ftell(f)-sizeof(persona), SEEK_SET);
		fwrite(&persona,sizeof(persona),1, f);

	} else {
		printf("\nIdentificazione fallita, non e' stato possibile effetturare le modifiche richieste.\n");
    }

	fclose(f);
	return 0;
}

/*Sommario: Funzione per la modifica della password appartenente allo stream con l'id corrispondente a quello inserito dall'utente esterno.
            Verrà aperto il file .csv contenente gli stream, attraverso un while il puntatore verrà spostato e lo stream puntato letto
            fino a quanto persona.indice non corrisponderà all'id inserito dall'utente.
            Trovato lo stream corrispondente, in questo caso l'identificazione verrà effettuata prima della richiesta di modifica.
            Le modifiche verranno applicate solo se la funzione Identificazione restituirà un valore di verità (TRUE).
            Verranno richiamate le funzioni chamate in Salvataggio_utenti per la generazione della password.
            Il nuovo stream verrà scrutto nel file .cvs;
No Parametri.

*/
int Modifica_password(){
	FILE *f;
	char filename[LUNG_NOME_FILE];
	strcpy(filename,"");
	strcat(filename,"../database-utenti.csv");

	utente persona;
	int id;  //variabile per la memorizzazione dell'indice univo dell'utente su cui effetturale le modifiche


	printf("Inserisci il tuo id identificativo:");
	scanf("%d", &id); //acquisizione indice dell'utente da modificare

	f=fopen(filename, "rb+");

    while((fread(&persona,sizeof(persona),1,f)) > 0) {
		if(persona.indice==id) {
			break; }

    }
    if (Identificazione (persona.password, LUNGHEZZA_PASSWORD )==TRUE){
    	printf("Identificazione avvenuta con successo, modifica effettuata.\n");
  		  Generatore_Password(&persona); //richiamo una funzione di generazione della password
  		  fseek(f, ftell(f)-sizeof(persona), SEEK_SET);
  		  fwrite(&persona,sizeof(persona),1, f);
  		  } else {
  			  printf("\nIdentificazione fallita, non e' stato possibile effetturare le modifiche richieste.\n");
  		  }

    fclose (f);
    return 0;
}


/*Sommario: Lettura del file .csv.
Parametri: puntatore di tipo utente

*/

int visualizza_database(){
	char filename[LUNG_NOME_FILE];
	strcpy(filename,"");
	strcat(filename, "../database-utenti.csv");

	FILE* stream_database = fopen(filename, "rb+");
	utente persona;//creo una struct di tipo alimenti ome riferimento per scorrere all'interno del file

	//fino a quando puoi prelevare righe stampa il contenuto della struct ossia gli elementi di ogni riga
    while(fread(&persona, sizeof(utente),1,stream_database)>0){
    	 printf("\nId=%d\nNome= %s\nCognome=%s\nPassword= %s\nPreferenze=\n- %s\n- %s\n- %s\n",persona.indice, persona.nome,persona.cognome, persona.password, persona.preferenze[0], persona.preferenze[1], persona.preferenze[2] );
    }

    return 0;
}



/*Sommario: Scrittura di ciò a cui punta il puntatore di tipo utente passato alla funzione sul file.
Parametri: puntatore di tipo utente

*/
int Aggiorna_databaseutenti(utente* persona){

	FILE *f;
    char filename[LUNG_NOME_FILE];
    strcpy(filename,"");
    strcat(filename,"../database-utenti.csv");

    f=fopen(filename, "wb+");


	fwrite(persona,sizeof(utente),1, f); //scrittura su file

    fclose (f);
    return 1;
}


/*Sommario: Funzione per confronto della password passata alla funzione e quella inserita dall'utente esterno.
 *
Parametri: stringa di caratteri, lunghezza della stringa.

Return: valore di verità se la striga passata alla funzione e quella inserita dall'utente fossero uguali.
*/
int Identificazione (char* password, int length){
	char identificator[length];

	printf("Per confermare le modifiche, inserire password utente: ");
	scanf("%s", identificator);

	if(strcmp(password,identificator)==0){
	return TRUE; }
	else {
	return FALSE;
	}
}

/*Sommario: Funzione per la stampa dello stream con l'id corrispondente a quello passato alla funzione.
            Verrà aperto il file .csv contenente gli stream, attraverso un while il puntatore verrà spostato e lo stream puntato letto
            fino a quanto persona.indice non corrisponderà all'id inserito dall'utente.
            Trovato lo stream corrispondente, verrà stampato su schermo.

Parametri: Indice utente usato per la ricerca dello stream da stampare
*/
int Stampa_utente(){

	FILE *f;
    char filename[LUNG_NOME_FILE];
    strcpy(filename,"");
    strcat(filename,"../database-utenti.csv");

    utente persona;
    int read;
    int id; //variabile per la memorizzazione dell'indice univoco dell'utente scritto su file

    printf("Inserire id del profilo da visualizzare: ");
    scanf("%d", &id); //acquisizione indice univoco

    f=fopen(filename, "rb+");
    do{
    	read = (fread(&persona,sizeof(persona),1,f));
    	//Se persona.indice corrisponde all'id inserito da tastiera, si esce dal while attraverso un break
 		  if(persona.indice==id) {

  			/* fscanf(f, "%d%s%s%s",&persona.indice, persona.nome,persona.cognome, persona.password);
  		      for(int i=0;i<RIGHE;i++) {
  		          for(int j=0;j<COLONNE;j++) {
  		             fscanf(f, "%c\n", &persona.preferenza[i][j]);
  		          }
  		      }*/
  			 break;
          }
    }while(read > 0) ;


	 fclose (f);
	 //Stampa dell'utente

	 printf("\nId=%d\nNome= %s\nCognome=%s\nPassword= %s\nPreferenze=\n%s\n%s\n%s",persona.indice, persona.nome,persona.cognome, persona.password,persona.preferenze[0],persona.preferenze[1],persona.preferenze[2]);
	 return 0;
}


/*Sommario: Funzione che stampa le varie operazione che si possono effettuare, leggerà la risposa inserita da tastiera ed in base al
 * numero inserito, verrà chiamata una funzione opportuna;
paramentri: i è un indice da passare che servirà per il salvataggio dell'utente, corrisponde all'ultimo indice assegnato ad un utente
*/
int Menu_databaseutenti (int i){

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
			Salvataggio_utenti(i);
			i++;
			break;
		case 2:
			Modifica_nome();
			break;
		case 3:
			Modifica_cognome();
			break;
		case 4:
			Modifica_preferenza();
			break;
		case 5:
			Modifica_password();
			break;
		case 6:
			Stampa_utente();
			break;
		case 7:
			visualizza_database();
			break;

	}

	} while (response!=0);

	return 0;
}


/*Sommario: Conteggio del numero degli utenti scritti su file.
  No parametri.

*/
int Conteggio_utenti (){

	FILE *f;
	char filename[LUNG_NOME_FILE];
	strcpy(filename,"");
	strcat(filename,"../database-utenti.csv");

	int counter=0;
	utente persona;

	f=fopen(filename,"rb");

	while (fread(&persona,sizeof(persona),1,f)>0) {
		counter++;
	}

	fclose(f);
	if (counter>0){
		return counter;

	}	else {
		printf("File vuoto\n");
		return 0;
	}

}

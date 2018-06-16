/*
 * file_ricette_operation.c
 *
 *  Created on: 16 mag 2018
 *      Author: david
 */
#include "ricette.h"



/*
 * Modifica la prima funzione in base alle modifiche che hai apportato a riduci quantitá alimenti
 * PREPARA RICETTE: controlli su apertura file del frigo, inserimento di un valido id e possibilitá di effetuare una differenza tra le quantitá disponibili nel frigo e quelle espresse
 * dagli ingredienti delle ricette. Controllo anche su effettiva scrittura e opportuno richiamo alla funzione dei consumi che sta in fondo alla funzione
 * FUNZIONE LEGGI_CAMPO_RICETTA: non penso siano necessari controlli in quanto le stringhe passate si presume siano pulite
 * FUNZIONE input_x: tutte le funzioni precedute dalla paraola input devono essere controllate con un classico controllo sulle stringhe e interi e sulla accettibilitá dei valori. Solo per l'input ingrediente magari va prestata
 *  piú attenzione per il formato
 *  FUNZIONE modifica_ricetta(): controllo apertura file e presenza di valori, controllo su id inserito, controllo su campo inserito, controllo su successo modifica
 * FUNZIONE elimina_ricetta(): controllo su file(i soliti) e su id correttamente inserito
 * FUNZIONE visualizza_ricetta():dai un'occhiata e aggiusta la visualizzazione se non ti garba
 * FUNZIONE visualizza_database_ricette: controllo su parametri in ingresso, su apertura file  e sullo stile della funzione
 * FUNZIONE lettura_database_ricette: dovrebbe essere passato un valore pulito quindi gli unici controlli sono su file e su effettiva scrittura
 * FUNZIONE conta_righe_database_ricette:controlli su file
 * FUNZIONE inizializza_ricette_preparabili:non penso ci siano controlli
 * FUNZIONE ordina_ric_kcal: controllo su stile e...non penso ci siano controlli di rilevanza
 * FUNZIONE  registra_consumi: controllo su parametri passati e controllo su file(scrittura,lettura)
 * FUNZIONE ricerca_prod_magg_cons: controlli su file e controllo corretta restituzione di un valore
 *  FUNZIONE ricette_alimenti_in_scadenza: controllo su parametri passati alle altre funzioni e nessun tipo di controllo. Questa funzione é una sorta di ponte che ho evitato di mettere tutto nella switch del menu di ricerca
 * FUNZIONE inserimento_manuale_ingredienti: controllo  su valori inseriti dall'utente
 * FUNZIONE suggerimento_ricetta_manuale: controllo su file e su corretto inserimento id(la struttura della funzione non ti piacerá, e ci terrei che la modificassi in quanto funziona ma non é carina)
 * FUNZIONE suggerimento_ricetta_automatico: controllo su file e su corretto inserimento id(la struttura della funzione non ti piacerá, e ci terrei che la modificassi in quanto funziona ma non é carina). Ë uguale alla precedente solo che
 * cambia leggermente dal punto di vista strutturale.
 *FUNZIONE aggiorna_database_ricette: fai gli stessi controlli di alimenti
 *FUNZIONE lettura_nuove_ricette(): fai gli stessi controlli di alimenti(ho impostato la struttura simile alla tua)
 *FUNZIONE conta_kcal_ricetta: controllo su effettiva presenza dei dati degli ingredienti all'interno del file del database degli alimenti
 *
 */

//MODIFICA IN BASE A RIDUCI QUANTITÄ
int prepara_ricetta(int num_ricette, ricetta* ricette_database){
	int i_info_ricetta=0;
	int scelta_prep;
	int indice_ingrediente=0;
	float q_riduzione;

	FILE *fp= fopen(FILE_FRIGO,"rb+"); //apri il file in modalitá "lettura binaria"
	alimento_frigo alimenti_frigo;//struct di riferimento per scorrere il file


	printf("Inserisci id ricetta che vuoi preparare");
	scanf("%d", &scelta_prep);


		while(strcmp(ricette_database[i_info_ricetta].ingredienti[indice_ingrediente].nome_ingredienti,"")!=0){
			q_riduzione=ricette_database[i_info_ricetta].ingredienti[indice_ingrediente].quantita_necessarie;

			//printf("%.1f\n", q_riduzione);
		    while(fread(&alimenti_frigo,sizeof(alimenti_frigo),1,fp)>0){//leggi fino a quando é presente una riga

		    	if(strcmp(alimenti_frigo.nome, ricette_database[i_info_ricetta].ingredienti[indice_ingrediente].nome_ingredienti)==0){//se la linea non é vuota e possiede un alimento
		    		int currPos = ftell(fp);//scopro la locazione di memoria su cui mi trovo al momento

		    		if(alimenti_frigo.quantita>=q_riduzione){
		    			alimenti_frigo.quantita=alimenti_frigo.quantita-q_riduzione;//riduco la quantitá dell'alimento
		    			q_riduzione=0;
		    		}else{
		    			printf("Quantita disponibile prima %.1f\n", alimenti_frigo.quantita );
		    			q_riduzione=q_riduzione-alimenti_frigo.quantita;
		    			printf("Quantita ancora necessaria %.1f\n", q_riduzione );
		    			alimenti_frigo.quantita=0;
		    		}

		      		if(alimenti_frigo.quantita==0){//se l'alimento si é esaurito notifica l'utente e inizializza la riga
						strcpy(alimenti_frigo.nome,"");
						strcpy(alimenti_frigo.unita_misura,"");
						alimenti_frigo.scadenza.giorno=0;
						alimenti_frigo.scadenza.mese=0;
						alimenti_frigo.scadenza.anno=0;
		      		}

		    		fseek(fp,currPos-sizeof(alimenti_frigo),SEEK_SET);//posiziona il puntatore sulla locazione di memoria da cui iniziare a scrivere
		        	fwrite(&alimenti_frigo,sizeof(alimenti_frigo),1,fp);//scrivi il contenuto della struct aggiornata
		        	rewind(fp);
		    		if(q_riduzione==0)
		    			break;


		    	}


			  }

		    indice_ingrediente++;

		}

	fclose(fp); //chiudi il file
	registra_consumi(ricette_database[i_info_ricetta].nome_ricetta,FLAG_RICETTA);
	return 0;
}







/**
 *La funzione seguente riceve il puntatore alla linea da cui voglio estrarre un valore e il numero del campo che voglio analizzare e
 *un flag che indica che tipologia di campo. Il principale compito della funzione é quello di effettuare una "tokenizzazione" della stringa
 *che gli passiamo. Nel caso di una stringa avente i campi formati da valori singoli nella forma VALORE-DELIMITATORE-VALORE la prima parte
 *fino a alla dichiarazione di tok viene ignorata. In questo preciso caso il token(ossia il puntatore tok) permette l'estrazione del valore
 *successivo al delimitatore su cui é posizionato. In questo senso tok scandisce i vari delimitatori per trovare il numero del campo nel
 *quale é presente il valore da estrarre. In caso il numero del campo e quindi il valore da estrarre sia valido e soprattutto esista viene
 *restituito sottoforma di puntatore di stringa. In caso contrario esso viene restituito come NULL
 *
 *
 *Ma adesso soffermiamoci nel caso in cui un campo sia a sua volta formato da campi e di cui non si sappia il numero come é il caso della preparazione
 *perché noi non conosciamo il numero di fasi inserite. Lo stratagemma é quello di passare come num di campo l'indice della fase
 *(ATTENZIONE: qui non si ragiona con indici di array ma per indice si intende il numero del campo che parte da 1). A questo punto lo stratagemma é
 *quello di contare i caratteri delimitatori(nel nostro caso i - per le fasi), in quanto sappiamo che se la stringa é nella forma VALORE-DELIMITATORE-VALORE...
 *allora necessariamente il numero di campi sará uguale al numero di delimitatori+1. La prima parte pertanto fa un check da indice del campo e
 *caratteri delimitatori. Qualora il campo passato sia maggiore del numero di - allora abbiamo finito con l'estrazione e viene restituito NULL.
 *
 * @param linea
 * @param num_campo
 * @param flag_campo
 * @return
 */
const char* leggi_campo_ricetta(char* linea, int num_campo, short flag_campo){

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
 * Funzione che dopo aver allocato una stringa di una determinata lunghezza, si occupa di ricevere in input il nome della ricetta
 * @return nome_ricetta
 */
char* input_nome_ricetta(){
	char* nome_ricetta=(char *)malloc(sizeof(LUNG_NOME_RIC));
	printf("Inserisci nuovo nome");
	scanf("%50s", nome_ricetta);
	return nome_ricetta;

}

/**
 * Funzione che dopo aver allocato una stringa di una determinata lunghezza, si occupa di ricevere in input il tempo di preparazione della ricetta
 * @return tempo_preparazione
 */
char* input_tempo_prep_ricetta(){
	char* tempo_preparazione=(char *)malloc(sizeof(LUNG_TEMP_PREP));
	printf("Inserisci nuovo tempo di preparazione");
	scanf("%20s", tempo_preparazione);
	return tempo_preparazione;

}

/**
 * Funzione che dopo aver allocato una stringa di una determinata lunghezza, si occupa di ricevere in input la complessitá della ricetta
 * @return complessita
 */
char* input_complessita_ricetta(){
	char* complessita=(char *)malloc(sizeof(LUNG_COMPLESSITA));
	printf("Inserisci nuova complessita'");
	scanf("%20s", complessita);
	return complessita;
}

/**
 * Funzione che dopo aver allocato una stringa di una determinata lunghezza, si occupa di ricevere in input la preparazione della ricetta
 * @return preparazione
 */
char* input_preparazione_ricetta(){
	char* preparazione=(char *)malloc(sizeof(LUNG_PREPARAZIONE));
	printf("Inserisci nuova preparazione");
	scanf("%s", preparazione);
	return preparazione;
}

/**
 * Funzione che dopo aver allocato una stringa di una determinata lunghezza, si occupa di ricevere in input l'ingrediente x della ricetta. Ovviamente in questo caso sono ammessi
 * gli spazi in quanto sappiamo che il formato dell'ingrediente é :  quantitá  unitá_di_misura nome_ingrediente
 * @return ingrediente
 */
char* input_ingredienti_ricetta(){
	char* ingrediente=(char *)malloc(sizeof(LUNG_STR_LAVORO_RIC));
	printf("Inserisci nuovo ingrediente:  ");
	scanf ("%[^\n]%*c", ingrediente);
	return ingrediente;
}

/**
 ** Funzione che  si occupa di ricevere in input il numero di porzioni della ricetta
 * @return porzione
 */
int input_porzione_ricetta(){
	int porzione;
	printf("Inserisci nuovo ingrediente:  ");
	scanf ("%d", &porzione);
	return porzione;
}





/**
 * La funzione modifica ricette si occupa di modificare un determinato campo di una determinata ricetta. In una prima fase verrá aperto il file che rappresenta il dtabase delle ricette.
 * In seguito verrá mostrato all'utente una vista minimizzata delle varie ricette salvate e verrá chiesto l'id della ricetta a cui apportare le modifiche. Una volta inserito viene
 * effettuata una ricerca sequenziale, evitando le tuple vuote presenti nel file. Una volta trovato la ricetta nel file del database verrá chiesto di inserire il numero del campo
 * da modificare. Tramite uno switch viene individuato il campo e viene richiamata la corrispettiva funzione di input. Per quanto riguarda gli ingredienti una volta ricevuto in ingresso
 * la stringa da parte dell'utente viene effettuato uno split dei valori per popolare opportunatamente la struct di tipo ingrediente. Al termine delle modifiche viene salvato il tutto
 * su file.
 * @return 1
 */
int modifica_ricetta(){
	int id_ricetta;//l'id ci servirá per capire quale ricetta é stata selezionata dall'utente e quindi a scorrere le varie ricette
	int linea=0;//indice che ci aiuta a capire a quale riga siamo arrivati e quindi quale ricetta stiamo analizzando
	int scelta_campo=0;//indica il campo della ricetta che deve essere modificato
	char divisione_ingrediente[LUNG_STR_LAVORO_RIC]; //stringa di lavoro che accogliera' la riga ingrediente da dividere in quantitá, unitá di misura e nome
	int indice_ingredienti=0;//indice per l'array di struct ingrediente memorizzato, a sua volta , nella array di tipo ricetta

	ricetta catalogo_ricetta;//struct di riferimento per scorrere il file

	FILE *fp= fopen(FILE_DATABASE_RICETTE,"rb+");

	visualizza_database_ricette(MOSTRA_DATABASE,1,VISTA_MINIMIZZATA); //viene stampato il contenuto attuale del database delle ricette

	//CONTROLLO SU ID VALIDO ALE sfrutta la funzione conta righe binary se necessario
	printf("\nInserisci id ricetta da modificare: ");
	scanf("%d", &id_ricetta);

    while(fread(&catalogo_ricetta,sizeof(catalogo_ricetta),1,fp)>0){//leggi fino a quando é presente una riga

    	if(strcmp(catalogo_ricetta.nome_ricetta, "")!=0)//se la linea non é vuota e ha il nome di una ricetta
    		++linea;//incrementa il numero di linee effettivamente piene del file database ricette

    	if(id_ricetta==linea){//se l'id della ricetta che si é preso come riferimento é uguale alla linea su cui ci troviamo abbiamo trovato la ricetta  a cui apportare le modifiche

			int currPos = ftell(fp);//scopro la locazione di memoria su cui mi trovo al momento e inizializzo la riga con strighe vuote e valori nulli
			visualizza_database_ricette(catalogo_ricetta.nome_ricetta,1,VISTA_TOTALE);

			printf("Scegli quale campo modificare:\n"
					"[1]Nome\n"
					"[2]Tempo preparazione\n"
					"[3]Complessita\n"
					"[4]Preparazione(per favorire di una migliore visibilitá, separare le varie fasi con il carattere - )\n"
					"[5]Ingredienti(si ricorda che ogni ingrediente deve essere nella forma:\n    quantita   unita' di misura   nome_ingrediente)\n"
					"[6]Numero porzioni");


			printf("\nInserisci numero campo da modificare  ");
			scanf("%d", &scelta_campo);

			pulisci_stdin();//puliamo il buffer di input prima di ricevere in ingresso nuovi dati

			switch(scelta_campo){
			case 1:
				strcpy(catalogo_ricetta.nome_ricetta,input_nome_ricetta());
				break;
			case 2:
				strcpy(catalogo_ricetta.tempo_prep,input_tempo_prep_ricetta());
				break;
			case 3:
				strcpy(catalogo_ricetta.complessita,input_complessita_ricetta());
				break;
			case 4:
				strcpy(catalogo_ricetta.preparazione,input_preparazione_ricetta());
				break;
			case 5:
				/*Qualora fosse il campo ingrediente ad essere scelto, una volta ricevuti in input i dati dell'ingrediente si effettua la divisione della stringa
				 *  di lavoro(divisione_ingrediente) per memorizzare separatamente i dati dell'ingrediente. Inoltre al termine dell'inserimento occorre ricalcolare le kcal per ricetta
				 */
				do{
					strcpy(divisione_ingrediente, input_ingredienti_ricetta());

					if(strcmp(divisione_ingrediente,"")!=0){

						int esito_lettura=0;
						esito_lettura=sscanf(divisione_ingrediente, "%f %[a-zA-Z] %[a-zA-Z]", &catalogo_ricetta.ingredienti[indice_ingredienti].quantita_necessarie, catalogo_ricetta.ingredienti[indice_ingredienti].unita_misura , catalogo_ricetta.ingredienti[indice_ingredienti].nome_ingredienti);

						if(esito_lettura==NUM_CAMPI_INGREDIENTI){
							indice_ingredienti++;
						}else if(esito_lettura==-1){
							break;
						}else{
							printf("Attenzione ingrediente inserito non valido. Reinserire ingrediente");
						}


					}else{
						break;
					}

				}while(indice_ingredienti<MAX_NUM_INGR);

				catalogo_ricetta.kcal_ricetta=conta_kcal_ricetta(catalogo_ricetta.ingredienti,indice_ingredienti+1);//memorizzo le kcal della ricetta passando i dati di essa
				break;

			case 6:
				catalogo_ricetta.porzione=input_porzione_ricetta();
				break;
			default:
				printf("Valore inserito non valido");
			}


			// scrivo la riga "inizializzata" alla posizione della ricetta rimossa
			fseek(fp, currPos-sizeof(catalogo_ricetta),SEEK_SET);//posiziona il puntatore sulla locazione di memoria da cui iniziare a scrivere
			fwrite(&catalogo_ricetta,sizeof(catalogo_ricetta),1,fp);//scrivi il contenuto della struct aggiornata
			break;//esci e termina l'inserimento
    	}
	  }


	fclose(fp);
	return 1;
}




/**
 *La funzione elimina_ricetta apre il database delle ricette, ne mostra il contenuto, e chiede quale delle ricette mostrate occorre cancellare.
 *L'individuazione della ricetta avviene tramite l'inserimento dell'ID. In questa maniera si risale alla riga dove é memorizzata la ricetta
 *inizializzandone il contenuto e salvando poi le modifiche. Al termine la funzione restituisce 1.
 * @return 1
 */
int elimina_ricetta(){
	int id_ricetta;//l'id ci servirá per capire quale ricetta é stata selezionata dall'utente e quindi a scorrere le varie ricette
	int linea=0;//indice che ci aiuta a capire a quale riga siamo arrivati e quindi quale ricetta stiamo analizzando
	ricetta catalogo_ricetta;//struct di riferimento per scorrere il file

	FILE *fp= fopen(FILE_DATABASE_RICETTE,"rb+");

	visualizza_database_ricette(MOSTRA_DATABASE,1,VISTA_MINIMIZZATA); //viene stampato il contenuto attuale del database delle ricette

	//CONTROLLO SU ID VALIDO ALE sfrutta la funzione conta righe binary se necessario
	printf("\nInserisci id ricetta da eliminare: ");
	scanf("%d", &id_ricetta);

    while(fread(&catalogo_ricetta,sizeof(catalogo_ricetta),1,fp)>0){//leggi fino a quando é presente una riga

    	if(strcmp(catalogo_ricetta.nome_ricetta, "")!=0)//se la linea non é vuota e ha il nome di una ricetta
    		++linea;//incrementa il numero di linee effettivamente piene del file database ricette

    	if(id_ricetta==linea){//se l'id della ricetta che si é preso come riferimento é uguale alla linea su cui ci troviamo abbiamo trovato la ricetta  a cui apportare le modifiche

			//scopro la locazione di memoria su cui mi trovo al momento e inizializzo la riga con strighe vuote e valori nulli
			int currPos = ftell(fp);

			catalogo_ricetta.id_ricetta=0;
			strcpy(catalogo_ricetta.nome_ricetta,"");
			strcpy(catalogo_ricetta.tempo_prep,"");
			strcpy(catalogo_ricetta.complessita,"");
			strcpy(catalogo_ricetta.complessita,"");
			strcpy(catalogo_ricetta.preparazione, "");
			catalogo_ricetta.kcal_ricetta=0;
			for(int i=0;i<MAX_NUM_INGR;i++){
				strcpy(catalogo_ricetta.ingredienti[i].nome_ingredienti, "");
			}


			// scrivo la riga "inizializzata" alla posizione della ricetta rimossa
			fseek(fp, currPos-sizeof(catalogo_ricetta),SEEK_SET);//posiziona il puntatore sulla locazione di memoria da cui iniziare a scrivere
			fwrite(&catalogo_ricetta,sizeof(catalogo_ricetta),1,fp);//scrivi il contenuto della struct aggiornata
			break;//esci e termina l'inserimento
    	}

	  }

	fclose(fp);
	return 1;
}

/**
 * La funzione visualizza_ricette riceve come parametro di ingresso la struct dove sono memorizzate le info della ricetta da mostrare.
 * La stampa dei campi composti da informazioni singole come il nome e il tempo di preparazione vengono mostrate tramite un'unica printf
 * Nel caso degli ingredienti viene stampato di volta in volta il contenuto della matrice tramite un ciclo. Per quanto riguarda la preparazione
 * (considerato che dal punto di vista progettuale si considerano le fasi come un unico testo) nella fase di visualizzazione le singole fasi,
 * che sono separate nel testo con il carattere delimitatore (-),  vengono opportunatamente divise tramite la funzione leggi_campo_ricetta.
 * Una volta ricavata ogni fase essa viene stampata con il suo id di riferimento.
 * @param lista_ricette
 * @return 1
 */
int visualizza_ricetta(ricetta lista_ricette, int vista_ricetta){
	int indice_ingrediente=0; //indice che ci serve per scorrere tra gli ingredienti
	int indice_preparazione=0; //indice che ci serve per scorrere tra le fasi della preparazione


	printf("%14s%8s%14s%10d%10d\n", lista_ricette.nome_ricetta,lista_ricette.tempo_prep, lista_ricette.complessita,lista_ricette.kcal_ricetta, lista_ricette.porzione);

	if(vista_ricetta==VISTA_TOTALE){
		printf("\n INGREDIENTI:\n\n");
		printf(" -------------------------------------------------------------\n");
		printf("|ID| NOME INGREDIENTE| QUANTITA' NECESSARIA | UNITA DI MISURA |\n");
		printf(" -------------------------------------------------------------\n");
		while(strcmp(lista_ricette.ingredienti[indice_ingrediente].nome_ingredienti,"")!=0){
			printf("%2d|%18s|%22.1f|%17s|\n", indice_ingrediente+1, lista_ricette.ingredienti[indice_ingrediente].nome_ingredienti, lista_ricette.ingredienti[indice_ingrediente].quantita_necessarie,lista_ricette.ingredienti[indice_ingrediente].unita_misura);
			printf(" -------------------------------------------------------------\n");
			indice_ingrediente++;
		}

		printf("\n\n PREPARAZIONE:\n");
		while(indice_preparazione<MAX_NUM_FASI){

			char *copia_riga=strdup(lista_ricette.preparazione); //le modifiche(i "tagli" per trovare la fase) vengono fatte sulla copia
			if(leggi_campo_ricetta(copia_riga,indice_preparazione+1,CAMPO_MULTIPLO)==NULL){ //se la prossima fase non esiste esci.
				break;
			}else{
				copia_riga=strdup(lista_ricette.preparazione);
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
 * Inoltre il parametro in ingresso id_ricetta permette di mostrare a schermo un id che magari é calcolato esternamente alla funzione(si faccia
 * riferimento alla funzione suggerimento_ricetta_automatico o suggerimento_ricetta_manuale per capire in quale contesto la si utilizza). Qualora
 * invece non sia nessun interesse o nel caso in cui debba essere mostrato l'intero database verrá passato come id personalizzato 0.
 * @param nome_ricetta
 * @param id_ricetta_pers
 * @return
 */
int visualizza_database_ricette(char nome_ricetta[LUNG_NOME_RIC], int id_ricette, int vista){

	int flag_mostra_int=0;

	FILE* stream_database = fopen(FILE_DATABASE_RICETTE, "rb+");
	ricetta lista_ricette;//creo una struct di tipo alimenti ome riferimento per scorrere all'interno del file

	//fino a quando puoi prelevare righe stampa il contenuto della struct ossia gli elementi di ogni riga
    while(fread(&lista_ricette,sizeof(lista_ricette),1,stream_database)>0){


    	if(strcmp(nome_ricetta, MOSTRA_DATABASE)==0){// se il nome passato corrisponde alla stringa vuota allora stampero'...

    		if(strcmp(lista_ricette.nome_ricetta, "")){//...solo le righe non vuote

    			 if(flag_mostra_int==0 || id_ricette==1){
    		    	printf(" ____________________________________________________________\n");
    		    	printf("|ID| NOME RICETTA | DURATA |  COMPLESSITA' | KCAL | PORZIONI |\n");
    		    	printf(" ____________________________________________________________\n");
    			 }

 		    	printf("%3d",id_ricette);
    			visualizza_ricetta(lista_ricette,vista);
    		}
			id_ricette++; //passo alla ricetta successiva

    	}else{//altrimenti se il nome passato come parametro non é vuoto...

    		if(strcmp(lista_ricette.nome_ricetta, nome_ricetta)==0){//...stampero' la ricetta corrispondente a quel nome
    		    if(flag_mostra_int==0 || id_ricette==1){
    		    	printf(" ____________________________________________________________\n");
    		    	printf("|ID| NOME RICETTA | DURATA |  COMPLESSITA' | KCAL | PORZIONI |\n");
    		    	printf(" ____________________________________________________________\n");
    			}
    		    printf("%3d",id_ricette);
    			visualizza_ricetta(lista_ricette,vista);
    		   	break;
    		}

    	}

		 if(vista==VISTA_MINIMIZZATA)
			 flag_mostra_int=1;

    }

    return 1;

}



/**
 * La funzione riceve in ingresso un array di struct passato per riferimento e lo popola prendendo le varie informazioni delle ricette,
 * memorizzate all'interno del database. Ovviamente vengono memorizzate solo le righe che non sono inizializzate ossia quelle che non vuote.
 * Per far ció si confronta il nome. Qualora il nome della ricetta sia una stringa vuota vuol dire che siamo in presenza di una riga vuota
 * e pertanto essa va ignorata.
 * @param ricette_database
 * @return 1
 */
int lettura_database_ricette(ricetta* ricette_database){
		int id_ricette=0;
		int indice_ingrediente=0;

		FILE* stream_database = fopen(FILE_DATABASE_RICETTE, "rb+");
		ricetta lista_ricette;

	    while(fread(&lista_ricette,sizeof(lista_ricette),1,stream_database)>0){

	    if(strcmp(lista_ricette.nome_ricetta,"")){

				strcpy(ricette_database[id_ricette].nome_ricetta,lista_ricette.nome_ricetta);
				strcpy(ricette_database[id_ricette].tempo_prep,lista_ricette.tempo_prep);
				strcpy(ricette_database[id_ricette].complessita,lista_ricette.complessita);


				while(strcmp(lista_ricette.ingredienti[indice_ingrediente].nome_ingredienti,"")){
					strcpy(ricette_database[id_ricette].ingredienti[indice_ingrediente].nome_ingredienti,lista_ricette.ingredienti[indice_ingrediente].nome_ingredienti);
					ricette_database[id_ricette].ingredienti[indice_ingrediente].quantita_necessarie=lista_ricette.ingredienti[indice_ingrediente].quantita_necessarie;
					indice_ingrediente++;
				}


				strcpy(ricette_database[id_ricette].preparazione,lista_ricette.preparazione);
				ricette_database[id_ricette].kcal_ricetta=lista_ricette.kcal_ricetta;
				ricette_database[id_ricette].porzione=lista_ricette.porzione;

				id_ricette++;
				indice_ingrediente=0;

	    	}
	    }

	return 1;
}

/**
 * La funzione apre il file del database delle ricette in modalitá lettura, estrae tutte le righe che possiede e conta solo le righe effettivamente
 * piene ignorando quelle inizializzate. Il risultato viene poi restiuito
 * @return righe
 */
int conta_righe_database_ricette(){
		FILE *fp= fopen(FILE_DATABASE_RICETTE,"rb"); //apri il file in modalitá "lettura binaria"
		int righe=0;				    //variabile che conta le righe del file
		ricetta lista_ricette;			//genero una struct di riferimento che mi permette di scorrere all'interno del file di tipo binario

        while(fread(&lista_ricette,sizeof(lista_ricette),1,fp)>0){//fino a quando riesci a leggere righe dal file

        	if(strcmp(lista_ricette.nome_ricetta,""))
        		righe++;		//incremento il contatore del numero di righe

		  }

		fclose(fp); //chiudi il file
		return righe;
}


/**
 * Funzione che ha in ingresso un array di puntatori di tipo stringa che rappresentano i nomi delle ricette preparabili e il numero di ricette
 * presenti nel database. Questo perché nella migliore delle ipotesi tutte le ricette del database possono essere preparabili. La funzione inizializza
 * tutto l'array di puntatori a NULL
 * @param num_ricette
 * @param ricette_preparabili
 * @return 1
 */
int inizializza_ricette_preparabili(int num_ricette, char* ricette_preparabili[num_ricette]){

	for(int i=0;i<num_ricette;i++){
		ricette_preparabili[i]=NULL;
	}

	return 1;
}


/**
 * La funzione seguente si occupa,avendo in ingresso il num di ricette complessive e l'array di struct che costituisce la lista di tutte le ricette,
 * di ordinare le ricette in base alle kcal prodotte, tramite l'algoritmo di ordinamento shell sort. A differenza del classico shell sort che
 * sfrutta un array formato dai vari gap ossia dalle varie distanze con cui confrontare e scambiare gli elementi, qui si é preferito sfruttare
 * inizialmente il num di ricette diviso 2 come gap di partenza e successivamente dividere il gap fino a quando non risultasse minore di 0.
 * Ovviamente lo scambio viene fatto in base al campo delle kcal. Al termine viene stampato l'ordinamento effettuato
 * @param ricette_database
 * @param num_ricette
 * @return
 */
int ordina_ric_kcal(ricetta* ricette_database, int num_ricette){

	ricetta x ;//struct di appoggio per scambiare 2 righe

	  for (int gap = num_ricette/2; gap > 0; gap /= 2)
	    {

	        for (int i = gap; i < num_ricette; i += 1)
	        {

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
 *La funzione registra_consumi riceve in ingresso il nome di un prodotto dove per prodotto intendiamo il nome di una ricetta o di un alimento e
 *un flag dove 0 indica che siamo in presenza di un alimento e 1 che indica che siamo in presenza di una ricetta. Inizialmente viene fatto un check
 *un all'interno del file dei consumi per vedere se il prodotto é giá stato consumato in passato e qualora lo sia, basta solo incrementare la sua
 *un frequenza. Nel caso in cui sia la prima volta che lo si consuma viene creata una nuova riga in cui memorizzare nome, la categoria di appartenenza
 *e la frequenza iniziale
 * @param nome_prodotto
 * @param flag_prodotto
 * @return
 */
int registra_consumi(char nome_prodotto[LUNG_PRODOTTO], short flag_prodotto){
	int flag_trovato=0; //flag che ci aiuta a scorrere il file dei consumi e a trovare, qualora ci fosse, un prodotto giá salvato in passato
	int currPos;// ci aiuta a salvare la posizione corrente del puntatore al file
	consumi prodotto_consumo;
	FILE *fp= fopen(FILE_CONSUMI,"rb+");

	 while(fread(&prodotto_consumo,sizeof(prodotto_consumo),1,fp)>0){//leggi fino a quando é presente una riga
		 //DEBUG: printf("Nome_ricetta consumata %s, frequenza: %d, Flag: %d\n", prodotto_consumo.nome_prodotto,prodotto_consumo.frequenza,prodotto_consumo.flag_prodotto);

		 //se ho trovato il nome del prodotto che ho passato alla funzione all'interno del file aggiorno la sua frequenza di consumo
		 if(strcmp(prodotto_consumo.nome_prodotto, nome_prodotto)==0){//se la linea non é vuota e possiede un alimento
			prodotto_consumo.frequenza++;
			currPos = ftell(fp);//scopro la locazione di memoria su cui mi trovo al momento
			fseek(fp,currPos-sizeof(prodotto_consumo),SEEK_SET);//posiziona il puntatore sulla locazione di memoria da cui iniziare a scrivere
			fwrite(&prodotto_consumo,sizeof(prodotto_consumo),1,fp);//scrivi il contenuto della struct aggiornata
			flag_trovato=1;//segnalo che é stato trovato
			break;
		}

	 }


	 //se il prodotto non é stato trovato all'interno del file vuol dire che é la prima volta che lo si consuma
	 if(flag_trovato==0){
		//DEBUG: printf("Il prodotto consumato per la prima volta e' %s", prodotto_consumo.nome_prodotto);
		strcpy(prodotto_consumo.nome_prodotto, nome_prodotto);//copio il nome del prodotto
		prodotto_consumo.flag_prodotto=flag_prodotto;//memorizzo la tipologia di prodotto
		prodotto_consumo.frequenza=1;
		fwrite(&prodotto_consumo,sizeof(prodotto_consumo),1,fp);
	 }

	fclose(fp);
	return 0;
}




/**
 * Questa funzione avendo in ingresso la tipologia di prodotto, stampa il prodotto di quella categoria maggiormente consumato. Ricordiamo che il flag
 * puó avere  1 in caso di ricette e 0 in caso di alimenti. Tramite un ciclo che analizza tutto il file dei consumi viene rintracciato la ricetta o
 * l'alimento con la frequenza maggiore. In caso questi non sia presente e la stringa del prodotto maggiormente consumato rimanga vuota verrá
 * stampato un mess di avviso
 * @param flag_prodotto
 * @return
 */
int ricerca_prod_magg_cons(short flag_prodotto){
	FILE *fp= fopen(FILE_CONSUMI,"rb+"); //apri il file in modalitá "lettura binaria"
	consumi prodotto_consumo;

	int freq_consumo=0;
	char prod_magg_cons[LUNG_PRODOTTO];
	strcpy(prod_magg_cons,""); //occorre inizializzare la stringa che conterrá il prodotto in quanto se non sono presenti alimenti o ricette consumate viene stampato un messaggio di avviso

	while(fread(&prodotto_consumo,sizeof(prodotto_consumo),1,fp)>0){//leggi fino a quando é presente una riga
		//DEBUG: printf("Nome_ricetta consumata %s, frequenza: %d, Flag: %d\n", prodotto_consumo.nome_prodotto,prodotto_consumo.frequenza,prodotto_consumo.flag_prodotto);

		/**se il prodotto correntemente analizzato ha una frequenza maggiore ed é del tipo specificato dal flag(ricordiamo 1->ricette e 0->alimenti)
		 	 allora lo salvo come il prodotto maggiormente consumato**/
		if(prodotto_consumo.frequenza>freq_consumo && prodotto_consumo.flag_prodotto==flag_prodotto){
			freq_consumo=prodotto_consumo.frequenza;
			strcpy(prod_magg_cons,prodotto_consumo.nome_prodotto);
		}
	}

	//qualora sia stato trovato un prodotto lo stampo opportunatamente. In caso contrario avviso l'utente
	if(strcmp(prod_magg_cons,"")!=0){
		if(flag_prodotto==FLAG_ALIMENTO){
			printf("L'alimento maggiormente consumato e' %s con una frequenza di %d\n", prod_magg_cons,freq_consumo);
		}else{
			printf("La ricetta maggiormente consumata e' %s con una frequenza di %d\n", prod_magg_cons,freq_consumo);
		}
	}else{
		printf("Non sono presenti prodotti con il paramentro richiesto\n");
	}


	fclose(fp); //chiudi il file
	return 1;
}




/**
 * In questa funzione viene passata la lista degli alimenti attualmente nel frigo con il relativo numero e una matrice che rappresenta
 * il nome degli alimenti che stanno per scadere. Una volta chiamata la funzione che si occupa del riordinamento degli alimenti in base alla scadenza
 * vengono memorizzati i primi 2 alimenti che stanno per scadere. In seguito viene fatto un richiamo alla funzione che suggerisce le ricette
 * in base agli alimenti che gli si passano. In questa maniera avremo la lista delle ricette che possiamo preparare con gli alimenti in scadenza
 * @param alimenti_frigo
 * @param num_alimenti
 * @param nome_alimenti
 * @return 1
 */
int ricette_alimenti_in_scadenza(alimento_frigo* alimenti_frigo,int num_alimenti,char alimenti_in_scadenza[MAX_ALIM_SUGG][LUNG_NOME_ALIMENTO]){
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
 * @param nome_alimenti
 * @return num_alimenti_inseriti++
 */
int inserimento_manuale_ingredienti(char nome_ingredienti[MAX_ALIM_SUGG][LUNG_NOME_ALIMENTO]){
	int num_alimenti_inseriti=0;
	int risposta=0;
	printf("Cercare una ricetta in base agli ingredienti e' semplice. \n"
			"1.Inserisci il nome dell'ingrediente con cui vuoi realizzare una tua ricetta\n"
			"2.Decidi se continuare con l'inserimento o meno (premi ctrl+z per terminare)\n"
			"Ti ricordiamo che potrai inserire fino ad un massimo di %d ingredienti\n\n",MAX_ALIM_SUGG );


//richiama la funzione che pulisce
	pulisci_stdin();


	do{
		printf("Inserisci nome ingrediente da cercare ");

		if(gets(nome_ingredienti[num_alimenti_inseriti])==NULL){//riceve in
			break;
		}

		//vanno i  controlli sulla stringa
		num_alimenti_inseriti++;

		if(num_alimenti_inseriti==MAX_ALIM_SUGG-1){
			break;
		}

	}while(risposta!=1);

	return num_alimenti_inseriti;
}



/**
 *La funzione suggerimento_ricetta_manuale avendo in ingresso il numero(num_alimenti_sugg) e i nomi degli ingredienti(nome_ingredienti)
 *su cui effettuare la ricerca fornisce il nome delle ricette che si possono preparare con tali ingredienti. La funzione é composta da un ciclo
 *su piú esterno che scandisce le vare ricette presenti ne database, e 2 cicli piú interni che si occupano di scandire di volta gli ingredienti
 *passati come parametro alla funzione con quelli che possiede ogni ricetta. Nel caso in cui una ricetta abbia gli ingredienti che abbiamo
 *passato essa va memorizzata come ricetta preparabile. Al termine del controllo sulle ricette vengono stampate a schermo quelle preparabili
 *passato qualora ce ne fossero o un messaggio di avvertenza che segnala che non esistono ricette con la combinazione di ingredienti fornita in input
 * @param alimenti_frigo
 * @param num_alimenti
 * @param nome_alimenti
 * @return 1
 */
int suggerimento_ricetta_manuale(int num_alimenti_sugg, char nome_ingredienti[MAX_ALIM_SUGG][LUNG_NOME_ALIMENTO]){
	int num_ricette=conta_righe_database_ricette();
	int presenza_alimento=0; // serve a capire se un ingrediente é presente o meno nella ricetta
	int indice_ingrediente=0; // serve a scandire i vari ingredienti della ricetta
	char* ricette_preparabili[num_ricette]; //serve a memorizzare i nomi delle ricette che si possono preparare. Nel migliore dei casi esso avrá lunghezza pari al numero di ricette presente nel database
	int indice_ric_prep=0; //serve a contare e a spostarsi nell'array delle ricette preparabili

	ricetta ricette_database[num_ricette];// array di struct che memorizza le varie ricette
	lettura_database_ricette(ricette_database); //popolamento delle ricette presenti nel database

	printf("RICETTE CHE SI POSSONO FARE:\n");
	inizializza_ricette_preparabili(num_ricette, ricette_preparabili);//occorre inizializzare l'array di puntatori per effetuare controlli futuri

	for(int i=0;i<num_ricette;i++){

		for(int j=0;j<num_alimenti_sugg;j++){//ripeto per il numero di alimenti suggeriti

			while(strcmp(ricette_database[i].ingredienti[indice_ingrediente].nome_ingredienti,"")!=0){//prendiamo in considerazione solo le righe piene

					if(strcmp(ricette_database[i].ingredienti[indice_ingrediente].nome_ingredienti, nome_ingredienti[j])==0){
						presenza_alimento=1;//puoi segnalare che l'ingrediente é presente
						break;
					}
					indice_ingrediente++;
			}

			indice_ingrediente=0;
			if(presenza_alimento==0){//in caso il flag di presenza sia rimasto a 0 passa alla ricetta successiva
				break;
			}else{
				if(num_alimenti_sugg!=1 && j!=num_alimenti_sugg-1) //se la lista degli alimenti non é composta solo da un elemento e se non siamo arrivati all'ultimo ingrediente suggerito
					presenza_alimento=0;//riporta il flag di presenza a 0 per poter analizzare il prossimo ingrediente
			}

		}

		if(presenza_alimento==1){//se l'ultimo flag di presenza é 1 vuol dire che la ricetta che ho analizzato é valida pertanto la salvo
			ricette_preparabili[indice_ric_prep]=ricette_database[i].nome_ricetta;
			indice_ric_prep++;
		}

		indice_ingrediente=0;
		presenza_alimento=0;
	}


	if(indice_ric_prep==0){//qualora non siano stati memorizzati ricette preparabili notifica l'utente
			printf("Non c'e' alcuna ricetta che puoi preparare con la combinazione di ingredienti da te inserita\n");
	}else{
		for(int i=0;i<indice_ric_prep;i++)
			visualizza_database_ricette(ricette_preparabili[i],i+1,VISTA_MINIMIZZATA);
	}

	return 1;

}



/**
 *La funzione suggerimento_ricetta_automatica avendo in ingresso gli alimenti del frigo e il numero di tali alimenti fornisce il nome delle ricette
 *che si possono preparare con tali ingredienti. La funzione é composta da un ciclo piú esterno che scandisce le vare ricette presenti ne database, e
 *2 cicli piú interni che si occupano di scandire di volta gli alimenti presenti nel frigo con quelli che possiede ogni ricetta.
 *Nel caso in cui l'ingrediente di una ricetta sia presente bisogna controllare se la quatitá dell'alimento a disposizione sia sufficiente.
 *Il controllo viene effettuato pertanto anche qualora vi sia piú di una occorrenza di uno stesso alimento. A tal proposito sappiamo che stessi
 *alimenti con diversa scadenza sono memorizzati su record diversi. In questo caso devo sommare le quantitá degli alimenti con stesso nome
 *ESEMPIO: ho 2 uova che scadono x/xx/xxxx e 2 uova che scadono yy/yy/yyyy. Voglio preparare la torta di mele che ne richiede 4. Posso prepararla perché
 *ho 4 uova a disposizione complessivamente.
 *Al termine del controllo sulle ricette vengono stampate a schermo quelle preparabili
 *passato qualora ce ne fossero o un messaggio di avvertenza che segnala che non esistono ricette con la combinazione di ingredienti fornita in input
 * @param alimenti_frigo
 * @param num_alimenti
 * @return
 */
int suggerimento_ricetta_automatico(alimento_frigo* alimenti_frigo,int num_alimenti){

	int num_ricette=conta_righe_database_ricette();
	int presenza_alimento=0;
	int indice_ingrediente=0;
	char* ricette_preparabili[num_ricette];
	int indice_ric_prep=0;
	float quantita_disponibile=0;

	ricetta ricette_database[num_ricette];
	lettura_database_ricette(ricette_database);


	printf("RICETTE CHE SI POSSONO FARE:\n");

	inizializza_ricette_preparabili(num_ricette, ricette_preparabili);

	for(int i=0;i<num_ricette;i++){

		while(strcmp(ricette_database[i].ingredienti[indice_ingrediente].nome_ingredienti,"")!=0){//per ogni ingrediente

			for(int j=0;j<num_alimenti;j++){//ripeti il confronto con gli alimenti del frigo

					if(strcmp(ricette_database[i].ingredienti[indice_ingrediente].nome_ingredienti, alimenti_frigo[j].nome)==0){
						quantita_disponibile=quantita_disponibile+alimenti_frigo[j].quantita;//incrementa la quantitá disponibile
						if(quantita_disponibile>=ricette_database[i].ingredienti[indice_ingrediente].quantita_necessarie){//se la quantitá disponibile é maggiore di quella necessaria
							presenza_alimento=1;//puoi segnalare che l'ingrediente é presente
							break;
						}

					}
			}

			if(presenza_alimento==0){
				printf("Non puoi fare %s perche manca: %s  %d\n",ricette_database[i].nome_ricetta, ricette_database[i].ingredienti[indice_ingrediente].nome_ingredienti,indice_ingrediente);
				break;
			}else{
				indice_ingrediente++;
				quantita_disponibile=0;
				if(strcmp(ricette_database[i].ingredienti[indice_ingrediente].nome_ingredienti,"")==0)//ho finito la ricetta
					break;
				presenza_alimento=0;
			}
		}



		if(presenza_alimento==1){
			ricette_preparabili[indice_ric_prep]=ricette_database[i].nome_ricetta;
			indice_ric_prep++;
		}

		indice_ingrediente=0;
		presenza_alimento=0;
		quantita_disponibile=0;
	}
	if(indice_ric_prep==0){
			printf("Non c'e' alcuna ricetta che puoi preparare con gli alimenti attuali. Fai la spesa\n");
	}else{
		for(int i=0;i<indice_ric_prep;i++)
			visualizza_database_ricette(ricette_preparabili[i],i+1,VISTA_TOTALE);
		//prepara_ricetta(indice_ric_prep,ricette_preparabili,ricette_database);
	}

	return 1;
}




/**
 * La funzione riceve in input la struct contenente i dati inerenti ad una singola ricetta e procede con il controllo sull'esistenza e apertura del file databse_ricette. Qualora non
 * sia possibile aprire il file in lettura viene creato per scriverci sopra la prima volta. La prima operazione da compiere qualora il file esista é controllare se la ricetta
 * che vogliamo salvare non sia giá presente nel database. Per far ció leggiamo sequenzialmente il file e se troviamo una ricetta con lo stesso nome aggiorniamo il flag di presenza.
 * Nel caso in cui la ricetta abbia superato il controllo e pertanto non sia presente nel database, essa viene salvata alla prima riga vuota presente nel file(ricordiamo che
 * il progetto in generale si basa sul recupero delle righe all'interno di un file).
 *
 * @param nuove_ricette
 * @return 1
 */
int aggiorna_database_ricette(ricetta nuove_ricette){
	int id_ricette=1;
    int flag_presente=0;//questo flag viene attivato se la nuova ricetta che si vuole caricare nel database é giá stata memorizzata in passato
	FILE* stream_database;

	if((stream_database = fopen(FILE_DATABASE_RICETTE, "rb+")) == NULL){// se il file degli alimenti presenti nel frigo esiste
		stream_database = fopen(FILE_DATABASE_RICETTE, "wb+");//apre uno stream di lettura e scrittura in coda sul file del database
	}else{
		stream_database = fopen(FILE_DATABASE_RICETTE, "rb+");//apre uno stream di lettura e scrittura in coda sul file del database
	}

	ricetta analisi_ricetta;//genero una struct di rifermiento per scorrere all'interno del file



	while(fread(&analisi_ricetta,sizeof(analisi_ricetta),1,stream_database)>0){// ripeti fino a quando é possibile prelevare una riga

		//se l'elemento x della lista degli alimenti comprati e uguale ad un alimento presente nel database
		if(strcmp(nuove_ricette.nome_ricetta, analisi_ricetta.nome_ricetta)==0){
			flag_presente=1;//attiva il flag della presenza
			printf("ricetta %s presente nel database\n", analisi_ricetta.nome_ricetta);
		}
	}
	rewind(stream_database);//viene riportato il puntatore all'inizio del file



	if(flag_presente==0){//se la ricetta non é presente nel database

		while(fread(&analisi_ricetta,sizeof(analisi_ricetta),1,stream_database)>0){

			if(strcmp(analisi_ricetta.nome_ricetta, "")==0){// se la riga é inizializzata ossia non ci sono dati relativi a ricette
				nuove_ricette.id_ricetta=id_ricette;//viene memorizzato l'id nella struct
				int currPos = ftell(stream_database);//viene memorizzata la locazione attuale di inizio riga
				fseek(stream_database,currPos-sizeof(analisi_ricetta),SEEK_SET);//viene posizionato il puntatore del file ad inizio riga cosí da poter effettuare l'opzione di modifica
				fwrite(&nuove_ricette,sizeof(nuove_ricette),1,stream_database);//viene memorizzato la ricetta x all'interno del file
				rewind(stream_database);//viene riportato il puntatore all'inizio del file
				flag_presente=-1;
				break;
			}
		}

		if(flag_presente!=-1){
			nuove_ricette.id_ricetta=id_ricette;//viene memorizzato l'id nella struct
			fwrite(&nuove_ricette,sizeof(nuove_ricette),1,stream_database);//viene memorizzata la struct(il nuovo alimento) nel file
		}
	}

    fclose(stream_database);
  	return 1;
}



/**
 * La funzione lettura_nuove_ricette si occupa di leggere un file in formato csv con accesso sequenziale e di memorizzare nei vari campi le informazioni riguardanti una nuova ricetta.
 * Viene pertanto scandita con la fgets ogni linea presente nel file,estratta e poi splittata tramite la sscanf. Qualora fossero rinvenuti errori come l'assenza di un campo viene chiesto
 * se si vuole modificare la ricetta oppure no. Qualora l'estrazione da file abbia avuto successo viene effettuato lo split degli ingredienti per memorizzare opportunamente i valori all'interno
 * dell'array di struct. Infine vengono calcolate le calorie prodotte dalla ricetta e viene richiamata la funzione che si occupa della memorizzazione dei dati raccolti. L'operazione viene
 * ripetuta fino a quando é possibile estrarre una riga dal file ossia fino a quando é presente una ricetta nel file
 * @return
 */
int lettura_nuove_ricette(){
		int indice_ingredienti=0;
		int indice_ricetta=0;//indice che servirá a scorrere l'array di struct lista_spesa durante la lettura del file
		char linea[LUNG_TUPLA_RICETTE];//dichiariamo la stringa che conterrá l'intera tupla proveniente dal file
		char ingredienti[LUNG_STR_LAVORO_RIC];//

		FILE* stream;//apro il file in lettura
		ricetta carica_ricette; //dichiaro un'array di struct che ospiterá i valori dei singoli alimenti comprati

		int esito_lettura;							// Variabile per contenere l'esito delle letture degli alimenti
		int esito_input;							// Variabile per memorizzare l'esito degli inserimenti dell'utente
		int flag_inserimento;						// Flag per memorizzare la corretteza dell'alimento letto
    	char* divisione_ingrediente;

		// tentativo di apertura della spesa in lettura
		if((stream = fopen(FILE_NUOVE_RICETTE, "r")) == NULL){
			return -1; // se il file non può essere aperto viene ritornato il valore -1
		}else{
			// estrazione di ogni riga del file (riga == 1 alimento)
			while(fgets(linea, LUNG_TUPLA_RICETTE, stream) != NULL){

				// modifica dei valori di flagInserimento e num_alimenti_caricati supponendo che l'alimento letto verrà inserito
				flag_inserimento = 1;
				indice_ricetta++;
				indice_ingredienti=0;

				//inizializzazione ingredienti e preparazione
				for(int i=0;i<MAX_NUM_INGR;i++){
					strcpy(carica_ricette.ingredienti[i].nome_ingredienti, "");
				}

				// estrazione dei campi della riga letta dal file
				esito_lettura = sscanf(linea, "%[a-zA-Z];%[^;];%[a-zA-Z0-9];%[^;];%[a-zA-Z];%d",
						carica_ricette.nome_ricetta, ingredienti, carica_ricette.tempo_prep, carica_ricette.preparazione, carica_ricette.complessita, &carica_ricette.porzione);

				// Controlli sull'esito dell'estrazione, per verificare errori nel formato della riga:
				// se sono presenti errori di questo tipo viene richiesto di scegliere se correggere l'alimento manualmente
				// o saltarlo.
				if(esito_lettura != NUM_CAMPI_RICETTA){
					printf("Errore nella lettura della ricetta numero %d.\n", indice_ricetta);
					// Richiesta di correzione manuale dell'alimento letto
					do{
						puts("Inserire 1 per correggerlo manualmente, 0 per saltarlo: ");
						esito_input = scanf("%d", &flag_inserimento);
						//pulisci_stdin();

						if((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1){
							puts("Inserimento non valido. Ripeterlo.\n");
						}
					}while((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1);


					// Correzione dell'alimento, se scelto dall'utente
					if(flag_inserimento == 1){

						puts("Correzione manuale dell'alimento.\n");
						//alimento_letto = input_alimento_frigo();

					}else{
						// Se si sceglie di saltare l'alimento, deve essere decrementato il numero di alimenti caricati
						indice_ricetta--;
					}
				}

				//se la lettura é andata a buon fine passo alla memorizzazione degli ingredienti, al calcolo delle kcal e all'aggiornamento del database
				if(esito_lettura == NUM_CAMPI_RICETTA){

						while(indice_ingredienti<MAX_NUM_INGR){

							divisione_ingrediente=strdup(ingredienti);//duplico la riga che contiene gli ingredienti

							//se il prossimo ingrediente che estraggo non esiste termino di memorizzare gli ingredienti altrimenti estraggo i dati ingrediente
							if(leggi_campo_ricetta(divisione_ingrediente,indice_ingredienti+1,CAMPO_MULTIPLO)==NULL){
								break;
							}else{
								//duplico nuovamente la stringa contenente gli ingredienti in quanto il contenuto é stato perso nella fase di check sopra
								divisione_ingrediente=strdup(ingredienti);

								//memorizzo nella stringa di lavoro l'ingrediente estratto dalla linea contenente tutti gli ingredienti
								strcpy(divisione_ingrediente, leggi_campo_ricetta(divisione_ingrediente,indice_ingredienti+1,CAMPO_MULTIPLO));

								//divido ulteriormente la stringa contenente l'ingrediente cosí da avere i singoli dati quali nome,quantita e relativa unitá di misura
								sscanf(divisione_ingrediente, "%f %[a-zA-Z] %[a-zA-Z]", &carica_ricette.ingredienti[indice_ingredienti].quantita_necessarie, carica_ricette.ingredienti[indice_ingredienti].unita_misura , carica_ricette.ingredienti[indice_ingredienti].nome_ingredienti);

								indice_ingredienti++;
							}

						}

						carica_ricette.kcal_ricetta=conta_kcal_ricetta(carica_ricette.ingredienti,indice_ingredienti+1);//memorizzo le kcal della ricetta passando i dati di essa
						aggiorna_database_ricette(carica_ricette);// aggiorno il database, memorizzando la ricetta
				}

			}
		}


		fclose(stream);


	return 1;
	}




/**
 * Funzione che calcola le kcal della ricetta che si sta prendendo in considerazione. In input verrá pertanto fornito l'array di struct
 * avente gli ingredienti e il numero di tali ingredienti. A questo punto  é necessario popolare un array di struct avente
 * le informazioni degli alimenti presenti nel database. In questa maniera siamo in grado di trovare una corrispondenza tra ingredienti e alimenti
 * presenti nel database, estraendo cosí il corrispettivo fabbisogno calorico per poi sommarlo a quello complessivo della ricetta. L'operazione
 * viene fatta per ogni ingrediente e al termine vengono fornite in output le kcal di quell'alimento.
 * @param nuove_ricette
 * @return kcal_ricetta
 */
int conta_kcal_ricetta(ingrediente ingredienti[MAX_NUM_INGR], int num_ingredienti){
	int kcal_ricetta=0;
	int indice_ingrediente=0;

	int num_alimenti=conta_alimenti_database();//numero di alimenti effettivamente presenti nel database
	alimento_database alimenti_database[num_alimenti]; //creo un array di struct corrispondente al num di alimenti presenti nel database
	leggi_database_alimenti(alimenti_database);//popolo l'array di struct

	for(int i=0;i<num_ingredienti;i++){
		for(int j=0;j<num_alimenti;j++){//ripeti il confronto con gli alimenti del database

			//se ho trovato corrispondenza con un alimento del frigo aggiungo le calorie di quell'alimento a quelle complessive della ricetta
			if(strcmp(ingredienti[indice_ingrediente].nome_ingredienti, alimenti_database[j].nome)==0){
				kcal_ricetta=kcal_ricetta+(alimenti_database[j].kcal*ingredienti[indice_ingrediente].quantita_necessarie)/alimenti_database[j].campione_kcal;
				break;
			}


		}
		indice_ingrediente++;
	}

	return kcal_ricetta;
}

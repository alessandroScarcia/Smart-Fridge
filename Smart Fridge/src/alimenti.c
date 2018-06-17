
#include "alimenti.h"





//NUOVE FUNZIONI ALIMENTI

/**
 * La funzione, avendo in ingresso gli alimenti del frigo con il relativo numero, si occupa di riordinare tali alimenti, o meglio l'array di struct che li contiene, in base
 * alla data di scadenza. L'ordinamento é effettuato attraverso l'algoritmo shell sort in quanto risultava quello piú semplice ed efficace da implementare dato il quantitativo
 * di dati(non é eccessivo considerato che il frigo avrá al massimo un centinaio al massimo di alimenti). Inoltre in questa versione di shell sort si é optato di non creare a parte un
 * array contenente i gap (distanza per gli scambi), ma di sfruttare il numero di alimenti inizialmente e poi dividere per 2 di volta in volta la lunghezza.
 * @pre  l'array di struct deve possedere almeno un alimento da ordinare e pertanto il numero di alimenti deve essere idoneo
 * @post L'ordinamento deve essere stato effettuato con successo
 *
 */
int ordina_alimenti_scadenza(alimento_frigo* alimenti_frigo, int  num_alimenti){

	alimento_frigo tmp;
	int* differenza=NULL;
	for (int gap = num_alimenti/2; gap > 0; gap /= 2)
		    {

		        for (int i = gap; i < num_alimenti; i++)
		        {
					// salva il corrente elemento puntato da i in tmp
		        	tmp = alimenti_frigo[i];

		            //sposta i precedenti elementi fino alla corretta locazione di alimenti_frigo[i] é 	trovata
		        	int j;
		            for (j = i; j >= gap && diff_date(differenza,alimenti_frigo[j - gap].scadenza, data_odierna()) > diff_date(differenza,tmp.scadenza,data_odierna()); j -= gap)
		            	alimenti_frigo[j] = alimenti_frigo[j - gap];

		            //  inserisci tmp (l'originale alimenti_frigo[i]) nella sua corretta locazione
		            alimenti_frigo[j] = tmp;
		        }
		    }

		/* DEBUG: for (int i=0; i < num_alimenti; i++){
				printf("%s %d/%d/%d\n", alimenti_frigo[i].nome_alimento, alimenti_frigo[i].scadenza.giorno,alimenti_frigo[i].scadenza.mese,alimenti_frigo[i].scadenza.anno);
		 }*/
	printf("Ordinamento per scadenza effettuato con successo...ora puoi preparare una ricetta ottimizzata\n");
	return 1;


}


/**
 * Funzione che si occupa di contare le effettive righe presenti nel database escludendo quelle vuote
 * @pre  Nessuna pre condizione particolare
 * @post Il valore restituito deve essere un intero significativo (>=0)
 */
int conta_alimenti_database(){
	FILE *fp= fopen(FILE_DATABASE_ALIMENTI,"rb"); //apri il file in modalitá "lettura binaria"
	alimento_database info_alimento;			//genero una struct di riferimento che mi permette di scorrere all'interno del file di tipo binario
	int righe=0;

    while(fread(&info_alimento,sizeof(info_alimento),1,fp)>0){//fino a quando riesci a leggere righe dal file
    	if(strcmp(info_alimento.nome,"")!=0)
    		righe++;									//incremento il contatore del numero di righe
	  }

	fclose(fp); //chiudi il file
	return righe;
}


/**
 * Funzione che conta gli alimenti presenti nel frigo e ne restituisce il valore. Il conteggio viene effettuato escludendo le righe vuote nel database
 * @pre  Nessuna pre condizione particolare
 * @post Il valore restituito deve essere un intero significativo (>=0)
 */
int conta_alimenti(){

		FILE *fp= fopen(FILE_FRIGO,"rb"); //apri il file in modalitá "lettura binaria"
		alimento_frigo info_alimento;			//genero una struct di riferimento che mi permette di scorrere all'interno del file di tipo binario
		int righe=0;

        while(fread(&info_alimento,sizeof(info_alimento),1,fp)>0){//fino a quando riesci a leggere righe dal file
        	if(strcmp(info_alimento.nome,"")!=0)
        		righe++;									//incremento il contatore del numero di righe

        }
		//DEBUG:printf("%d\n", lines);

		fclose(fp); //chiudi il file
		return righe;

}


/**
 * Dopo aver popolato un array di struct di tipo alimento frigo la funzione si occupa di confrontare la data di scadenza con la data odierna. Qualora la differenza tra date
 * produca un valore negativo vuol, dire che l'alimento é scaduto e pertanto occorre incrementare il contatore. Questa funzione serve principalmente nel menu principale per
 * conteggiare il numero di notifiche che devono eesre viste dall'utente
 * @pre  Nessuna pre condizione particolare
 * @post Il valore restituito deve essere un intero significativo (>=0)
 */
int conta_alimenti_scaduti(){
	int num_alimenti=conta_alimenti(FILE_FRIGO);
	alimento_frigo alimenti_frigo[num_alimenti];
	leggi_frigo(alimenti_frigo);

	int contatore_alim_scad=0;
	int* differenza=NULL;
    for(int i=0;i<num_alimenti;i++){
    	if(diff_date(differenza, alimenti_frigo[i].scadenza, data_odierna()) <0){//se la differenza tra data odierna e data di scadenza produce un valore negativo allora l'alimento é scaduto
    		contatore_alim_scad++;

    	}
    }
	return contatore_alim_scad;
}


/**
 * Funzione che avendo ricevuto in ingresso l'array di struct vuoto che conterrá gli alimenti del database, si occuperá di riempire tale array con i dati salvati all'interno del file
 * e di restituire al termine il numero di tali alimenti salvati. Come per ogni altra funzione che conta gli elementi del file anche in questo caso si saltano le righe vuote
 * @pre  Deve essere passato un array di struct della giusta dimensione
 * @post Il valore restituito deve essere un intero significativo (>=0)
 */
int leggi_database_alimenti(alimento_database* lista_alimenti){
	FILE *fp; //creo un puntatore di tipo file
	fp=fopen(FILE_DATABASE_ALIMENTI,"rb+"); //apri il file in modalitá "scrittura in coda

	alimento_database info_alimento;//creo una struttura di riferimento per scorrere all'interno del file

	int indice_alimento=0;
    while(fread(&info_alimento,sizeof(info_alimento),1,fp)>0){//ripeti fino a quando é possibile leggere righe con le dimensioni della struct
    	if(strcmp(info_alimento.nome, "")!=0){//stampa solo le righe non inizializzate
    		strcpy(lista_alimenti[indice_alimento].nome,info_alimento.nome);
    		lista_alimenti[indice_alimento].campione_kcal=info_alimento.campione_kcal;
    		lista_alimenti[indice_alimento].soglia_spesa=info_alimento.soglia_spesa;
    		lista_alimenti[indice_alimento].kcal=info_alimento.kcal;
    		strcpy(lista_alimenti[indice_alimento].unita_misura,info_alimento.unita_misura);
			indice_alimento++;
    	}
    }
    fclose(fp);
    return indice_alimento;//se la funzione é andata a buon fine restituisci il numero di elementi caricati
}



/**
 * Funzione che viene richiamata direttamente dal menu e dal gestore delle notifiche. Essa si occupa  di scovare tutti gli alimenti scaduti(se la differenza tra data odierna e data di scadenza
 * produce un valore negativo allora l'alimento é scaduto) e di inizializzare la riga in maniera tale da poterla recuperare in una prossima scrittura. Ovviamente nella ricerca vengono
 * sempre scartate le righe vuote
 * @pre  Nessuna pre condizione particolare
 * @post Deve essere stata effettuata con successo la scrittura su file
 */
int eliminazione_alimenti_scaduti(){
	alimento_frigo alimenti_frigo;
	FILE* fp = fopen(FILE_FRIGO, "rb+");
	int* differenza=NULL;
	while(fread(&alimenti_frigo,sizeof(alimenti_frigo),1,fp)>0){//ripeti fino a quando é possibile leggere una riga dal file
		if(diff_date(differenza,alimenti_frigo.scadenza, data_odierna())<0 && strcmp(alimenti_frigo.nome,"")!=0){//esegue il blocco solo le righe non sono inizializzate e se l'alimento é scaduto

			//inizializzo la riga per eliminare il contenuto
			strcpy(alimenti_frigo.nome,"");
			strcpy(alimenti_frigo.unita_misura,"");
			alimenti_frigo.scadenza.giorno=0;
			alimenti_frigo.quantita=0;
			alimenti_frigo.scadenza.mese=0;
			alimenti_frigo.scadenza.anno=0;


			int currPos = ftell(fp);//scopro la locazione di memoria su cui mi trovo al momento
			fseek(fp,currPos-sizeof(alimenti_frigo),SEEK_SET);//posiziona il puntatore sulla locazione di memoria da cui iniziare a scrivere
			fwrite(&alimenti_frigo,sizeof(alimenti_frigo),1,fp);//scrivi il contenuto della struct aggiornata
			rewind(fp);
		}
	}
	fclose(fp);
	return 1;
}






/**
 *Funzione che viene richiamata direttamente dal gestore delle notifiche e si occupa di visualizzare gli alimenti scaduti presenti nel frigo e di mostrare la relativa data di scadenza.
 * @pre  Nessuna pre condizione particolare
 * @post Deve essere termianta con successo la visualizzazione
 */
int visualizza_alimenti_scaduti(){
	int num_alimenti=conta_alimenti();
	alimento_frigo alimenti_frigo[num_alimenti];
	leggi_frigo(alimenti_frigo);
	int* differenza=NULL;
    for(int i=0;i<num_alimenti;i++){
    	if(diff_date(differenza, alimenti_frigo[i].scadenza, data_odierna()) <0)//se la differenza tra data odierna e data di scadenza produce un valore negativo allora l'alimento é scaduto
    		printf("Alimento scaduto: %s data di scadenza: %hu/%hu/%hu\n", alimenti_frigo[i].nome, alimenti_frigo[i].scadenza.giorno,
    				alimenti_frigo[i].scadenza.mese, alimenti_frigo[i].scadenza.anno);
    }
	return 1;
}


















int controlla_unita_misura(char* unita_misura){
	if(strcmp(unita_misura, UNITA_KG) == 0
		|| strcmp(unita_misura, UNITA_G) == 0
		|| strcmp(unita_misura, UNITA_L) == 0
		|| strcmp(unita_misura, UNITA_ML) == 0
		|| strcmp(unita_misura, UNITA_PZ) == 0){
		return 1;
	}else{
		return 0;
	}
}


int controlla_quantita(float quantita, char* unita_misura){
	int sup_quantita;// Estremo superiore relativo all'unità di misura ricevuta

	// Identificazione del valore corretto per l'estremo superiore
	if(strcmp(unita_misura, UNITA_G) == 0){
		sup_quantita = MAX_QUANTITA_G;
	}else if(strcmp(unita_misura, UNITA_ML) == 0){
		sup_quantita = MAX_QUANTITA_ML;
	}else if(strcmp(unita_misura, UNITA_PZ) == 0){
		sup_quantita = MAX_QUANTITA_PZ;
	}else{
		return -1;
	}

	if(quantita < MIN_QUANTITA || quantita > sup_quantita){
		return 0;
	}else{
		return 1;
	}
}


int controlla_data_scadenza(data scadenza){
	if(confronta_date(scadenza, data_odierna()) == DATA_NON_VALIDA){
		return -1;
	}else if(confronta_date(scadenza, data_odierna()) == SECONDA_DATA_ANTECEDENTE){
		return 1;
	}else{
		return 0;
	}
}


int converti_quantita(float *quantita, char *unita_misura){
	if(strcmp(unita_misura, UNITA_KG) == 0){
		strcpy(unita_misura, UNITA_G);
		*quantita *= KG_TO_G;

		return 1;
	}else if(strcmp(unita_misura, UNITA_L) == 0){
		strcpy(unita_misura, UNITA_ML);
		*quantita *= L_TO_ML;

		return 1;
	}

	return 0;
}


void abbassa_maiuscole(char *s){
	int i = 0;

	while(s[i]){
		s[i] = tolower(s[i]);
		i++;
	}
}


alimento_frigo input_alimento_frigo(){
	int esito_input;
	alimento_frigo alimento;

	// Ciclo per la ripetizione dell'inserimento fino a riceverlo nel formato corretto
	do{
		puts("Inserisci l'alimento <nome> <quantità> <unità di misura> <gg/mm/aaaa>: ");
		esito_input = scanf("%20[a-zA-Z] %f %5[a-zA-Z] %hu/%hu/%hu",
				alimento.nome, &alimento.quantita, alimento.unita_misura,
				&alimento.scadenza.giorno, &alimento.scadenza.mese, &alimento.scadenza.anno);
		pulisci_stdin();

		if(esito_input != NUM_CAMPI_ALIMENTO_FRIGO){
			puts("Inserimento non valido. Ripeterlo.\n");
		}
	}while(esito_input != NUM_CAMPI_ALIMENTO_FRIGO);

	return alimento;
}


void input_unita_misura(char *unita_misura){
	int esito_input;
	int esito_controllo;

	// Ciclo per la ripetizione dell'inserimento fino a riceverlo significativo
	do{
		puts("Inserisci l'unità di misura: ");
		esito_input = scanf("%5[a-zA-Z]", unita_misura);
		pulisci_stdin();

		esito_controllo = controlla_unita_misura(unita_misura);

		if(esito_controllo != 1 || esito_input != 1){
			puts("Inserimento non valido. Ripeterlo.\n");
		}
	}while(esito_controllo != 1 || esito_input != 1);
}


float input_quantita(char *unita_misura){
	int esito_input;
	int esito_controllo;
	float quantita = 0;

	do{
		puts("Inserisci la quantita: ");
		esito_input = scanf("%f", &quantita);
		pulisci_stdin();

		esito_controllo = controlla_quantita(quantita, unita_misura);

		if(esito_controllo != 1 || esito_input != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}
	}while(esito_controllo != 1 || esito_input != 1);

	return quantita;
}


data input_data_scadenza(){
	data data_inserita;
	int esito_controllo;

	do{
		data_inserita = input_data();

		esito_controllo = controlla_data_scadenza(data_inserita);
		if(esito_controllo != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}
	}while(esito_controllo != 1);

	return data_inserita;
}



int input_kcal(char *nome_alimento, int campione_kcal, char *unita_misura){
	float kcal;
	int esito_input;

	do{
		printf("Inserisci le kcal per <%d %s> dell'alimento <%s> : ", campione_kcal, unita_misura, nome_alimento);
		esito_input = scanf("%f", &kcal);
		pulisci_stdin();

		if(esito_input != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}
	}while(esito_input != 1);

	return kcal;
}


int input_id_alimento(int num_alimenti_frigo){
	int id_alimento;
	int esito_input;
	int esito_controllo;

	do{
		printf("Inserisci l'id di un alimento: ");
		esito_input = scanf("%d", &id_alimento);
		pulisci_stdin();

		if(id_alimento < MIN_ID_ALIMENTO || id_alimento > num_alimenti_frigo){
			esito_controllo = 0;
		}else{
			esito_controllo = 1;
		}

		if(esito_input != 1 || esito_controllo != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}
	}while(esito_input != 1 || esito_controllo != 1);

	return id_alimento;
}

int calcola_campione_kcal(char *unita_misura){
	// Analisi dell'unità di misura ricevuta e restituzione del valore di campione corrispondente
	if(strcmp(unita_misura, UNITA_G) == 0){
		return CAMPIONE_G;

	}else if(strcmp(unita_misura, UNITA_ML) == 0){
		return CAMPIONE_ML;

	}else if(strcmp(unita_misura, UNITA_PZ) == 0){
		return CAMPIONE_PZ;

	}

	// Se l'unità di misura non è valida, ritorna 0
	return 0;
}


/**
 *
 * @param quantita
 * @param unita_misura
 * @return quantita_minima
 */
float input_soglia_spesa(char *nome_alimento, char* unita_misura){
	float soglia;
	int esito_input;
	int esito_controllo;

	do{
		printf("Inserisci la soglia per l'inserimento nella generazione\ndella spesa dell'alimento <%s> [0 per escluderlo]: ", nome_alimento);
		esito_input = scanf("%f", &soglia);
		pulisci_stdin();

		if(esito_input == 1){
			if(soglia == 0){
				esito_controllo = 1;
			}else{
				esito_controllo = controlla_quantita(soglia, unita_misura);
			}
		}

		if(esito_controllo != 1 || esito_input != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}
	}while(esito_controllo != 1 || esito_input != 1);

	return soglia;
}


int ricerca_database(char *nome_alimento, alimento_database *alimento_estratto){
	alimento_database alimento;
	FILE *stream = NULL;

	if((stream = fopen(FILE_DATABASE_ALIMENTI, "rb")) == NULL){
		return -1;
	}else{
		while(feof(stream) == 0){
			fread(&alimento, sizeof(alimento_database), 1, stream);

			if(strcmp(alimento.nome, nome_alimento) == 0){
				*alimento_estratto =  alimento;
				return 1;
			}
		}

		return 0;
	}
}


int aggiorna_database(alimento_frigo alimento){
	FILE *stream = NULL;								// Puntatore al file database_alimenti
	alimento_database alimento_database;				// Variabile per comunicare con il database
	char flag_presenza = 0;								// Flag per memorizzare la presenza nel database dell'alimento in analisi

	// Tentativo di apertura del FILE_DATABASE
	if((stream = fopen(FILE_DATABASE_ALIMENTI, "rb+")) == NULL){
		// Se non esiste nessun FILE_DATABASE deve essere creato
		if((stream = fopen(FILE_DATABASE_ALIMENTI, "wb+")) == NULL){
			// Se non può essere creato il file, viene ritornato 0
			return -1;
		}
	}

	// Se si superano i controlli sull'apertura del file, possiamo effettuare l'aggiornamento del database

	fseek(stream, 0, SEEK_SET);

	// Analisi di tutte le righe del database per determinare se si conosce l'alimento
	while(flag_presenza == 0){

		fread(&alimento_database, sizeof(alimento_database), 1, stream);

		// Se si raggiunge la fine del file è necessario uscire dal ciclo
		if(feof(stream) != 0){
			break;
		}

		// Confronto fra l'alimento letto dal database e quello ipoteticamente sconosciuto
		if(strcmp(alimento_database.nome, alimento.nome) == 0){
			// Se l'alimento è conosciuto, va modificato il valore di flag_presenza
			flag_presenza = 1;
		}
	}

	// Se flag_presenza è uguale a zero, l'alimento è sconosciuto e bisogna aggiungerlo
	if(flag_presenza == 0){
		printf("L'alimento identificato come <%s> è sconosciuto.\n", alimento.nome);

		// Creazione della riga da inserire nel database
		// Nome dell'alimento
		strcpy(alimento_database.nome, alimento.nome);
		// Unità di misura
		strcpy(alimento_database.unita_misura, alimento.unita_misura);
		// Campione per il calcolo delle kcal
		alimento_database.campione_kcal = calcola_campione_kcal(alimento_database.unita_misura);
		// Kcal per campione di alimento
		alimento_database.kcal = input_kcal(alimento_database.nome, alimento_database.campione_kcal, alimento_database.unita_misura);
		// Soglia per la creazione della lista della spesa
		alimento_database.soglia_spesa = input_soglia_spesa(alimento_database.nome, alimento_database.unita_misura);

		fseek(stream, 0, SEEK_END);

		fwrite(&alimento_database, sizeof(alimento_database), 1, stream);

		fclose(stream);
		return 1;
	}else{
		fclose(stream);
		return 0;
	}
}


///ALE: serve un controllo sulla memorizzazione di alimenti con lo stesso nome e scadenza.In tal caso occorre aumentare la quantitá(se a questo hai difficoltá me la vedo io)
/**
 *
 * @param alimenti_comprati
 * @param num_alimenti
 * @return
 */
int aggiorna_frigo(alimento_frigo alimento){
	FILE *stream = NULL;							// puntatore al file contenente gli alimenti del frigo
	alimento_frigo alimento_frigo;					// variabile per estrarre singolarmente gli alimenti del frigo
	fpos_t riga_libera = -1;						// puntatore alla posizione nel file della prima riga sovrascrivibile
	fpos_t riga_letta = -1;						// puntatore alla posizione nel file della riga letta
	char flag_posizionamento = 0;					// flag per determinare se il nuovo alimento da aggiungere è stato posizionato

	// tentativo di apertura del file frigo in lettura e scrittura
	if((stream = fopen(FILE_FRIGO, "rb+")) == NULL){
		// se il file non viene aperto, si decide di crearne uno nuovo
		if((stream = fopen(FILE_FRIGO, "wb+")) == NULL){
			// se non può essere creato il file in modalità aggiornamento, viene ritornato 0
			return 0;
		}
	}

	// Se vengono superati i precedenti controlli sul puntatore al file, vuol dire che è stato impostato correttamente
	// ed è possibile l'aggiornamento del frigo

	// Memorizzazione della posizione del puntatore ad inizio riga da leggere, così da effettuare facilemnte la scrittura se necessaria
	fgetpos(stream, &riga_letta);

	// Lettura delle righe del file e controlli sulla riga letta fino a quando non viene posizionato l'alimento
	while(flag_posizionamento == 0){

		fread(&alimento_frigo, sizeof(alimento_frigo), 1, stream);

		// Se si arriva alla fine del file, è necessario uscire dal ciclo
		if(feof(stream)){
			break;
		}
		// Se la riga letta è sovrascrivibile e il puntatore alla prima riga letta non è stato
		// impostato, viene salvata la posizione di tale riga
		if(strcmp(alimento_frigo.nome, "") == 0 && riga_libera == -1){
			riga_libera = riga_letta;
		}
		// Altrimenti se la riga letta presenta stesso nome e stessa scadenza dell'alimento da inserire
		// viene incrementata la quantita nella riga letta, riposizionandoci con fsetpos ad inizio riga
		else if(strcmp(alimento_frigo.nome, alimento.nome) == 0
				&& confronta_date(alimento_frigo.scadenza, alimento.scadenza) == 0){
			alimento_frigo.quantita += alimento.quantita;
			fsetpos(stream, &riga_letta);
			fwrite(&alimento_frigo, sizeof(alimento_frigo), 1, stream);
			flag_posizionamento = 1;
		}
		// Se il posizionamento non è avvenuto, viene memorizzata la posizione della prossima riga che verrà letta
		// per l'eventuale futura scrittura
		if(flag_posizionamento == 0){
			fgetpos(stream, &riga_letta);
		}
	}

	// Se non vengono individuati alimenti con stesso nome e scadenza dell'alimento da inserire
	// l'alimento è inserito nella prima riga letta o in coda al file
	if(flag_posizionamento == 0){
		if(riga_libera != -1){
			fsetpos(stream, &riga_libera);
			fwrite(&alimento, sizeof(alimento_frigo), 1, stream);
		}else{
			fwrite(&alimento, sizeof(alimento_frigo), 1, stream);
		}
		fclose(stream);
		return 2;
	}

	fclose(stream);

	// se l'aggiunta nel frigo avviene, può essere ritornato il valore 1
	return 1;
}



///FUNZIONI MENU
/**
 *
 * @return 1 in caso di successo
 */
int visualizza_database_alimenti(){
	FILE *stream = NULL;					// Puntatore al FILE_DATABASE
	alimento_database alimento; 			// Variabile per contenere gli alimenti letti dal database
	int num_alimenti_database = 0;				// Variabile per contenere il  numero di alimenti presenti nel database

	if((stream = fopen(FILE_DATABASE_ALIMENTI, "rb")) == NULL){
		return -1;
	}else{
		printf("DATABASE ALIMENTI\n\n");
		printf("%4s | %-20s | %8s | %15s | %15s | %4s\n", "ID", "NOME ALIMENTO", "CAMPIONE", "QUANTITA SOGLIA", "UNITA DI MISURA", "KCAL");

		// Visualizzazione delle righe del database
		while(fread(&alimento, sizeof(alimento_database), 1, stream) == 1){
			if(feof(stream) != 0){
				break;
			}

			num_alimenti_database++;
			printf("%4d | %20s | %8d | %15.1f | %15s | %4hu\n",
					num_alimenti_database, alimento.nome, alimento.campione_kcal, alimento.soglia_spesa, alimento.unita_misura, alimento.kcal);
		}
	}
    return num_alimenti_database;
}


/**
 *
 * @return 1 in caso di successo
 */
int visualizza_frigo(){
	FILE *stream = NULL;				// Puntatore a FILE_FRIGO
	unsigned short num_alimento = 0;	// Variabile per memorizzare il numero dell'alimento visualizzato
	alimento_frigo alimento;			// Variabile per memorizzare l'alimento letto da FILE_FRIGO

	if((stream = fopen(FILE_FRIGO, "rb")) == NULL){
		return -1;
	}else{
		printf("CONTENUTO DEL FRIGO\n\n");
		printf("%4s | %-20s | %-14s | %16s\n", "ID", "NOME ALIMENTO", "QUANTITA", "DATA DI SCADENZA");

		while(fread(&alimento, sizeof(alimento_frigo), 1, stream) == 1){

			if(feof(stream) != 0){
				break;
			}

			if(strcmp(alimento.nome, "") != 0){
				num_alimento++;
				printf("_________________________________________________________________\n");

				printf("\n%4d | %20s | %8.1f %-5s |      %hu/%hu/%hu\n",
						num_alimento, alimento.nome, alimento.quantita, alimento.unita_misura, alimento.scadenza.giorno, alimento.scadenza.mese, alimento.scadenza.anno);

			}
		}
	}

    return num_alimento;///se la funzione é andata a buon fine restituisci 1
}


/**
 *
 * @param lista_frigo
 * @return 1 in caso di successo
 */
int leggi_frigo(alimento_frigo* lista_frigo){
	FILE *fp; ///creo un puntatore di tipo file
	fp=fopen("../alimenti_frigo.csv","rb+"); ///apri il file in modalitá "scrittura in coda

	alimento_frigo catalogo;///creo una struttura di riferimento per scorrere all'interno del file

	int indice_alimento=0;
    while(fread(&catalogo,sizeof(catalogo),1,fp)>0){///ripeti fino a quando é possibile leggere righe con le dimensioni dllla struct
    	if(strcmp(catalogo.nome, "")!=0){///stampa solo le righe non inizializzate
    		strcpy(lista_frigo[indice_alimento].nome,catalogo.nome);
    		lista_frigo[indice_alimento].quantita=catalogo.quantita;
    		strcpy(lista_frigo[indice_alimento].unita_misura,catalogo.unita_misura);
    		lista_frigo[indice_alimento].scadenza.giorno=catalogo.scadenza.giorno;
			lista_frigo[indice_alimento].scadenza.mese=catalogo.scadenza.mese;
			lista_frigo[indice_alimento].scadenza.anno=catalogo.scadenza.anno;
			indice_alimento++;
    	}
    }

    return 1;///se la funzione é andata a buon fine restituisci 1
}

/**
 *
 * @return 1 in caso di successo
 */
int carica_spesa(){
	FILE *stream = NULL;						// Puntatore al file contenente la spesa
	char s[LUNG_RIGA_FGETS];					// Stringa che ospita ogni riga del file per la lettura degli alimenti
	int num_alimenti_letti = 0;					// Numero di alimento letti dal file
	int num_alimenti_scartati = 0;				// Numero di alimenti scartati dal caricamento
	alimento_frigo alimento_letto;				// Variabile contente i dati di ogni alimento letto dal file
	int esito_lettura;							// Variabile per contenere l'esito delle letture degli alimenti
	int esito_input;							// Variabile per memorizzare l'esito degli inserimenti dell'utente
	int flag_inserimento;						// Flag per memorizzare la corretteza dell'alimento letto

	// tentativo di apertura della spesa in lettura
	if((stream = fopen(FILE_SPESA, "r")) == NULL){
		return -1; // se il file non può essere aperto viene ritornato il valore -1
	}else{
		// estrazione di ogni riga del file (riga == 1 alimento)
		while(fgets(s, LUNG_RIGA_FGETS, stream) != NULL){
			if(s[strlen(s) - 1] != '\n'){
				pulisci_riga_flusso(stream);
			}
			// modifica dei valori di flagInserimento e num_alimenti_caricati supponendo che l'alimento letto verrà inserito
			flag_inserimento = 1;
			num_alimenti_letti++;

			// estrazione dei campi della riga letta dal file
			esito_lettura = sscanf(s, "%20[a-zA-Z] %f %5[a-zA-Z] %hu/%hu/%hu",
					alimento_letto.nome, &alimento_letto.quantita, alimento_letto.unita_misura,
					&alimento_letto.scadenza.giorno, &alimento_letto.scadenza.mese, &alimento_letto.scadenza.anno);

			// Controlli sull'esito dell'estrazione, per verificare errori nel formato della riga:
			// se sono presenti errori di questo tipo viene richiesto di scegliere se correggere l'alimento manualmente
			// o saltarlo.
			if(esito_lettura != NUM_CAMPI_ALIMENTO_FRIGO){
				printf("Errore nella lettura dell'alimento numero %d.\n", num_alimenti_letti);
				// Richiesta di correzione manuale dell'alimento letto
				do{
					puts("Inserire 1 per correggerlo manualmente, 0 per saltarlo: ");
					esito_input = scanf("%d", &flag_inserimento);
					pulisci_stdin();

					if((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1){
						puts("Inserimento non valido. Ripeterlo.\n");
					}
				}while((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1);


				// Correzione dell'alimento, se scelto dall'utente
				if(flag_inserimento == 1){

					puts("Correzione manuale dell'alimento.\n");
					alimento_letto = input_alimento_frigo();

				}else{
					// Se si sceglie di saltare l'alimento, deve essere decrementato il numero di alimenti caricati
					num_alimenti_scartati++;
				}
			}


			// se l'alimento deve essere aggiunto, è necessario rendere minuscole tutte le lettere del nome
			abbassa_maiuscole(alimento_letto.nome);

			// Controlli sul significato dei campi dell'alimento estratto:
			// Unità di misura
			abbassa_maiuscole(alimento_letto.unita_misura);

			// Controllo sul significato dell'unità di misura inserita
			if(flag_inserimento == 1 && controlla_unita_misura(alimento_letto.unita_misura) == 0){
				printf("Unità di misura dell'alimento <%s> non valida.\n", alimento_letto.nome);

				// Richiesta all'utente della correzione manuale
				do{
					printf("Inserire 1 per correggere l'unità di misura, 0 per saltare l'alimento: ");
					esito_input = scanf("%d", &flag_inserimento);
					pulisci_stdin();

					if((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1){
						puts("Inserimento non valido.\n");
					}
				}while((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1);

				// Correzione dell'unità di misura se deciso dall'utente
				if(flag_inserimento == 1){

					puts("Correzione manuale dell'unità di misura.");
					input_unita_misura(alimento_letto.unita_misura);

				}else{
					// Se si sceglie di saltare l'alimento, deve essere decrementato il numero di alimenti caricati
					num_alimenti_scartati++;
				}
			}


			// Verifica della congruenza fra l'unità di misura inserita per l'alimento
			// e quella, se l'alimento è già conosciuto, presente nel database
			if(flag_inserimento == 1){
				int esito_ricerca;
				alimento_database alimento_database;

				// Conversione dell'unità di misura inserita in quella utilizzata nel frigo
				if(converti_quantita(&alimento_letto.quantita, alimento_letto.unita_misura) == 1){
					printf("Quantita dell'alimento <%s> convertita per adattarla al contenuto del frigo.\n", alimento_letto.nome);
				}

				// Ricerca nel  database dell'alimento letto
				esito_ricerca = ricerca_database(alimento_letto.nome, &alimento_database);
				if(esito_ricerca == 1){
					// Confronto fra le due unità di misura
					if(strcmp(alimento_letto.unita_misura, alimento_database.unita_misura) != 0){
						printf("L'unità di misura dell'alimento <%s> non corrisponde a quella conosciuta nel database [%s].\n",
								alimento_letto.nome, alimento_database.unita_misura);

						// Richiesta all'utente della correzione manuale
						do{
							printf("Inserire 1 per correggere l'unità di misura con quella conosciuta, 0 per saltare l'alimento: ");
							esito_input = scanf("%d", &flag_inserimento);
							pulisci_stdin();

							if((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1){
								puts("Inserimento non valido.\n");
							}
						}while((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1);

						if(flag_inserimento == 1){
							// Copia dell'unità di misura corretta e azzeramento della quantità
							// così da provocare la correzione nel controllo successivo
							strcpy(alimento_letto.unita_misura, alimento_database.unita_misura);
							alimento_letto.quantita = 0;
						}else{
							num_alimenti_scartati++;
						}
					}
				}
			}



			// Quantità
			if(flag_inserimento == 1){

				if(controlla_quantita(alimento_letto.quantita, alimento_letto.unita_misura) == 0){
					printf("Quantita dell'alimento <%s> non valida.\n", alimento_letto.nome);

					// Richiesta all'utente della correzione manuale
					do{
						printf("Inserire 1 per correggere la quantità, 0 per saltare l'alimento: ");
						esito_input = scanf("%d", &flag_inserimento);
						pulisci_stdin();

						if((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1){
							puts("Inserimento non valido.\n");
						}
					}while((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1);

					// Correzione della quantità se deciso dall'utente
					if(flag_inserimento == 1){

						puts("Correzione manuale della quantità.");
						alimento_letto.quantita = input_quantita(alimento_letto.unita_misura);

					}else{
						// Se si sceglie di saltare l'alimento, deve essere decrementato il numero di alimenti caricati
						num_alimenti_scartati++;
					}
				}
			}

			// Data di scadenza
			if(flag_inserimento == 1){
				if(controlla_data_scadenza(alimento_letto.scadenza) == -1){
					printf("Data di scadenza per l'alimento <%s> non valida.\n", alimento_letto.nome);
					flag_inserimento = 0;
				}else if(controlla_data_scadenza(alimento_letto.scadenza) == 0){
					printf("L'alimento <%s> è scaduto.\n", alimento_letto.nome);
					flag_inserimento = 0;
				}

				// Richiesta all'utente ella correzione manuale
				if(flag_inserimento == 0){
					do{
						printf("Inserire 1 per correggere la data di scadenza, 0 per saltare l'alimento: ");
						esito_input = scanf("%d", &flag_inserimento);
						pulisci_stdin();

						if((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1){
							puts("Inserimento non valido.\n");
						}
					}while((flag_inserimento != 0 && flag_inserimento != 1) || esito_input != 1);

					// Correzione della data di scadenza se deciso dall'utente
					if(flag_inserimento == 1){

						puts("Correzione manuale della data di scadenza.");
						alimento_letto.scadenza = input_data_scadenza();

					}else{
						// Se si sceglie di saltare l'alimento, deve essere decrementato il numero di alimenti caricati
						num_alimenti_scartati++;
					}
				}
			}

			// In questo punto del codice, se flag_inserimento è uguale a 1, alimento_letto può essere aggiunto al frigo
			if(flag_inserimento == 1){
				int esito_aggiornamento;

				// aggiunta dell'alimento e propagazione dell'eventuale errore ricevuto
				esito_aggiornamento = aggiorna_frigo(alimento_letto);

				if(esito_aggiornamento == 2){
					esito_aggiornamento = aggiorna_database(alimento_letto);
					if(esito_aggiornamento == -1){
					 return -3;
					}
				}else if(esito_aggiornamento == -1){
					return -2;
				}

			}

		}
	}

	fclose(stream);

	return num_alimenti_letti - num_alimenti_scartati;
}


/**
 *
 * @return 1 in caso di successo
 */
int riduci_alimento(){
	FILE *stream = NULL;

	alimento_frigo alimento;
	unsigned short flag_alimento = 0;
	unsigned short id_alimento;

	int num_alimenti_frigo;
	unsigned short num_alimenti_letti = 0;

	int quantita_riduzione;

	if((stream = fopen(FILE_FRIGO, "rb+")) == NULL){
		return 0;
	}else{
		// visualizzazione del frigo all'utente
		num_alimenti_frigo = visualizza_frigo();

		if(num_alimenti_frigo > 0){
			// Inseriemnto id alimento da ridurre
			id_alimento = input_id_alimento(num_alimenti_frigo);

			// Ricerca alimento
			while(flag_alimento == 0){
				if(feof(stream) != 0){
					break;
				}

				// Lettura di un alimento dal frigo
				fread(&alimento, sizeof(alimento_frigo), 1, stream);

				if(strcmp(alimento.nome, "") != 0){
					num_alimenti_letti++;

					if(num_alimenti_letti == id_alimento){
						fseek(stream, -sizeof(alimento_frigo), SEEK_CUR);
						flag_alimento++;
					}
				}
			}
		}else{
			fclose(stream);
			return num_alimenti_frigo;
		}

		// Input quantità della riduzione
		do{
			quantita_riduzione = input_quantita(alimento.unita_misura);

			if(quantita_riduzione > alimento.quantita){
				puts("La quantità inserita supera quella disponibile. Ripetere l'inserimento.");
			}

		}while(quantita_riduzione > alimento.quantita);

		alimento.quantita -= quantita_riduzione;
		if(alimento.quantita == 0){
			strcpy(alimento.nome, "");
		}

		fwrite(&alimento, sizeof(alimento_frigo), 1, stream);

		fclose(stream);
		return 1;
	}
}

int modifica_soglia_spesa(){
	FILE* stream = NULL;

	alimento_database alimento;
	unsigned short flag_alimento = 0;
	unsigned short id_alimento;

	int num_alimenti_database;
	unsigned short num_alimenti_letti = 0;

	int soglia_spesa;

	if((stream = fopen(FILE_DATABASE_ALIMENTI, "rb+")) == NULL){
		return 0;
	}else{
		num_alimenti_database = visualizza_database_alimenti();

		if(num_alimenti_database > 0){
			id_alimento = input_id_alimento(num_alimenti_database);

			while(flag_alimento == 0){
				if(feof(stream) != 0){
					break;
				}

				// Lettura di un alimento dal database
				fread(&alimento, sizeof(alimento_database), 1, stream);

				if(strcmp(alimento.nome, "") != 0){
					num_alimenti_letti++;

					if(num_alimenti_letti == id_alimento){
						fseek(stream, -sizeof(alimento_frigo), SEEK_CUR);
						flag_alimento++;
					}
				}
			}


		}else{
			fclose(stream);
			return num_alimenti_database;
		}

		// Input nuova soglia per la generazione della spesa
		soglia_spesa = input_quantita(alimento.unita_misura);

		alimento.soglia_spesa = soglia_spesa;

		fwrite(&alimento, sizeof(alimento_database), 1, stream);

		fclose(stream);
		return 1;
	}
}

int modifica_kcal(){
	FILE* stream = NULL;

	alimento_database alimento;
	unsigned short flag_alimento = 0;
	unsigned short id_alimento;

	int num_alimenti_database;
	unsigned short num_alimenti_letti = 0;

	int kcal;

	if((stream = fopen(FILE_DATABASE_ALIMENTI, "rb+")) == NULL){
		return 0;
	}else{
		num_alimenti_database = visualizza_database_alimenti();

		if(num_alimenti_database > 0){
			id_alimento = input_id_alimento(num_alimenti_database);

			while(flag_alimento == 0){
				if(feof(stream) != 0){
					break;
				}

				// Lettura di un alimento dal database
				fread(&alimento, sizeof(alimento_database), 1, stream);

				if(strcmp(alimento.nome, "") != 0){
					num_alimenti_letti++;

					if(num_alimenti_letti == id_alimento){
						fseek(stream, -sizeof(alimento_frigo), SEEK_CUR);
						flag_alimento++;
					}
				}
			}


		}else{
			fclose(stream);
			return num_alimenti_database;
		}

		// Input nuova soglia per la generazione della spesa
		kcal = input_kcal(alimento.nome, alimento.campione_kcal, alimento.unita_misura);

		alimento.kcal = kcal;

		fwrite(&alimento, sizeof(alimento_database), 1, stream);

		fclose(stream);
		return 1;
	}
}

int alimento_casuale(char* nome_alimento){
	FILE* stream = NULL;			// Puntatore a FILE_DATABASE
	alimento_database alimento;		// Alimento estratto random
	long dim_database;				// Dimensione di FILE_DATBASE

	// Tentativo di apertura di FILE_BINARIO in lettura binaria
	if((stream = fopen(FILE_DATABASE_ALIMENTI, "rb")) == NULL){
		return -1;	// Se non può essere aperto, ritorna -1
	}else{
		// Se il file può essere aperto ma è vuoto, ritorna 0
		if(feof(stream) != 0){
			return 0;
		}

		// Altrimenti calcola la dimensione del file e leggi un alimento random
		fseek(stream, 0, SEEK_END);
		dim_database = ftell(stream) / sizeof(alimento_database);
		rewind(stream);

		// Spostamento del puntatore del file in una posizione random
		fseek(stream, (rand() % dim_database) * sizeof(alimento_database), SEEK_SET);

		fread(&alimento, sizeof(alimento_database), 1, stream);

		strcpy(nome_alimento, alimento.nome);

		// Se l'estrazione avviene correttamente, ritorna 1
		return 1;
	}
}

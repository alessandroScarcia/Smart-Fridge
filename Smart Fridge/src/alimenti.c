
/**
* @file file_operation.c
*
* @version 0.1
*
* @date 26 apr 2018
*
* @authors Michela Salvemini, Alessandro Scarcia, Davide Quatela
*
* @bug Nessun bug di rilevanza individuato
*/
#include "alimenti.h"

/*CONTROLLI DA APPLICARE(le righe ti danno un idea orientativa di dove applicare il controllo e dove trovare la funzione ovviamente ti puoi gestire al meglio
 * Dai un'occhiata se trovi commenti in riga con ALE. Nel caso mi sono SCIRRATO controlli :-) )
 *
 *
 * 1.Controllo se vengono inseriti alimenti nel file spesa effettuata con maiuscole e minuscole(FUNZ lettura_spesa  RIGA:410)
 * 2.Controllare se é stata inserita o meno la data di scadenza e in caso di mancata assegnazione generare data valida shiftata di 3 giorni(FUNZ lettura_spesa  RIGA:410)
 * 3. Controllare se file é effettivamente aperto e se esiste(FUNZ lettura_spesa  RIGA:400)
 * 4. Migliorare controllo  in caso di errore (funz aggiorna_frigo RIGA 284)
 * 4.1 Controllo per alimenti con stessa scadenza accorparli e aumentare quantitá (funz aggiorna_frigo RIGA 290)
 * 5. Controllo esistenza file ed effettiva apertura(funz aggiorna database RIGA:245)
 * 6. controllo apertura stream (conta_righe_file_sq RIGA: 80)
 * 7. Controllo input intero in un range definito da te. Ricordati di aggiungere le constanti a file_alimenti_operation.h ( input_kcal  RIGA 102)
 * 8. Funzione set_q_camp RIGA: 114. Controlla se sono state inserite quantitá sproporzionate oppure <=0 e dalle un'occhiata di sfuggita per vedere se é tutto ok
 * 9. Controlla i valori float generati sia per le quantitá espresse in grammi, millilitri e controlla soprattutto la genera
 * 	  zione di un valore intero per gli alimenti che hanno unitá di misura pz. (set_soglia_spesa RIGA: 140 per ogni if)
 * 10.Controlla se é avvenuta con successo l'operazione confrontando  unitá di misura valide(ottieni_u_misura RIGA: 161)
 * 11.IGNORA:(leggi_campo RIGA 194)
 * 12. Controlla se il file si é aperto correttamente e se esiste. visualizza_database_alimenti() RIGA 324
 * 13. Controlla se il file si é aperto correttamente e se esiste. visualizza_frigo() RIGA 345
 * 14. IGNORA( ancora in via di sviluppo per ricette) leggi_frigo RIGA: 372
 * 15. Controlla apertura corretta del file(riduci_q_alimenti RIGA 435)
 * 16. Controllo inserimento valido id per la cancellazione (riduci_q_alimenti RIGA 439)
 * 17. Controllo inserimento valida quantitá da ridurre (riduci_q_alimenti RIGA 457)
 * 18. Controllo operazione di riduzione (riduci_q_alimenti RIGA 460)
*/

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
		esito_input = scanf("%[a-zA-Z] %f %[a-zA-Z] %hu/%hu/%hu",
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
		esito_input = scanf("%[a-zA-Z]", unita_misura);
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
		printf("Inserisci le kcal per %d %s dell'alimento %s: ", campione_kcal, unita_misura, nome_alimento);
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

	do{
		printf("Inserisci la soglia per l'inserimento nella generazione\ndella spesa dell'alimento <%s> [0 per escluderlo]: ", nome_alimento);
		esito_input = scanf("%f", &soglia);
		pulisci_stdin();

		if(controlla_quantita(soglia, unita_misura) != 1 || esito_input != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}
	}while(controlla_quantita(soglia, unita_misura) != 1 || esito_input != 1);

	return soglia;
}


int ricerca_database(char *nome_alimento, alimento_database *alimento_estratto){
	alimento_database alimento;
	FILE *stream = NULL;

	if((stream = fopen(FILE_DATABASE, "rb")) == NULL){
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
	if((stream = fopen(FILE_DATABASE, "rb+")) == NULL){
		// Se non esiste nessun FILE_DATABASE deve essere creato
		if((stream = fopen(FILE_DATABASE, "wb+")) == NULL){
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

	if((stream = fopen(FILE_DATABASE, "rb")) == NULL){
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
			pulisci_riga_flusso(stream);
			// modifica dei valori di flagInserimento e num_alimenti_caricati supponendo che l'alimento letto verrà inserito
			flag_inserimento = 1;
			num_alimenti_letti++;

			// estrazione dei campi della riga letta dal file
			esito_lettura = sscanf(s, "%[a-zA-Z] %f %[a-zA-Z] %hu/%hu/%hu",
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

	if((stream = fopen(FILE_DATABASE, "rb+")) == NULL){
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

	if((stream = fopen(FILE_DATABASE, "rb+")) == NULL){
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


/**
* @file file_operation.c
* Synthetic description.
*
* Detailed description here.
* @version 0.1
* @date 26 apr 2018
* @authors Michela Salvemini, Alessandro Scarcia, Davide Quatela
* @bug Nessun bug di rilevanza individuato
* @warning Riga:160 . Variabile non usata in istruzioni di rilevanza ma utilizzata solo come appoggio
*/
#include "file_alimenti_operation.h"
#define ALIMENTI_STRUCT
#define ALIMENTI_LIBRARY

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







/**
 * FUNZIONI CONTROLLI
 * @param nome_file
 * @return bool type
 */
bool controllo_esist_file(const char * nome_file){

	FILE* stream = fopen(nome_file, "r");
	if (stream)///se il file si apre in lettura allora restituisci true altrimenti di default restituisci false
	{
		return true;
	}

	return false;
}







/**FUNZIONI FILE
 *
 * @param nome_file
 * @return righe
 */
int conta_righe_file_sq(char nome_file[LUNG_NOME_FILE]){

		FILE *stream= fopen(nome_file,"r"); ///apri il file in modalitá "lettura"
		int righe=0;				   ///variabile che conta le righe del file
		char  buffer[LUNG_BUFFER];		   ///stringa che accoglie la riga in lettura dal file

		while (fgets(buffer, sizeof (buffer), stream)) ///fino a quando riesci a leggere righe dal file
		    righe++;							   ///incremento il contatore del numero di righe

		fclose(stream); 				  ///chiudi il file
		return righe; 				  ///restituisci il numero di righe del file

}




/**
 *
 * @param nome_alimento
 * @return kcal
 */
int input_kcal(char nome_alimento[LUNG_NOME]){
	int kcal;
	printf("Inserisci le kcal per %s", nome_alimento );

	scanf("%d", &kcal );///controllo_input() di ALE
	return kcal;
}



/**
 *
 * @param unita_misura
 * @return campione di riferimento per le kcal o 0 in caso di errore
 */
int set_q_camp(char unita_misura[LUNG_U_MISURA]){
	if(strcmp(unita_misura, "g")==0 || strcmp(unita_misura, "kg")==0){/// se l'unitá di misura dell'alimento e il g allora
		return grammi;///restituisci 100 come campione
	}else if(strcmp(unita_misura, "ml")==0 || strcmp(unita_misura, "lt")==0){/// se l'unitá di misura dell'alimento e il ml o lt allora
		return millilitri;///restituisci 100 come campione
	}else{/// se l'unitá di misura dell'alimento e il pz allora
		return pz;///restituisci 1 come campione
	}
	return 0; ///restituisci 0 se qualcosa é andata storta
}



/**
 *
 * @param quantita
 * @param unita_misura
 * @return quantita_minima
 */
float set_soglia_spesa(float quantita, char unita_misura[LUNG_U_MISURA]){
	float quantita_minima;///variabile che ospiterá il risultato ottenuto per calcolare la soglia

	///ALE: effettua gli opportuni controlli qui sotto per arrontondare la quantitá in caso siano pz e soprattutto controlla
	///che il valore restituito in percentuale sia valido


	///se le unitá di misura sono tipizzate allora:
	if(strcmp(unita_misura, "g")==0 || strcmp(unita_misura, "ml")==0 || strcmp(unita_misura, "lt")==0){
		quantita_minima=(SOGLIA_PERC*quantita)/(float)100;///calcola x% della prima quantitá che si memorizza nel frigo

	}else{///altrimenti se la quantitá é in pz QUESTO NON VA BENE PERTANTO VA QUA IL TUO CONTROLLO
		quantita_minima=(SOGLIA_PERC*quantita)/(float)100;

	}
	return quantita_minima;///restituisci la quantitá minima da aveere nel frigo
}


/**
 *
 * @param quant_u_misura
 * @return parte_stringa
 */
char* ottieni_u_misura(char quant_u_misura[LUNG_STR_LAVORO]){

	char *parte_stringa; ///variabile "puntatore" che verrá sfruttato per individuare la parte non numerica della stringa

	int parte_intera;///variabile che accoglierá il numero presente nella stringa

	///la funzione strtol restituisce un long presente in una stringa qualora sia presente nel nostro caso il valore nuemrico della quantitá
	parte_intera = strtol(quant_u_misura, &parte_stringa, 10);


	return parte_stringa;///restituisci l'unitá di misura dell'alimento che deve essere memorizzato
}



/**
 *
 * @param quant_u_misura
 * @return parte_intera
 */
float ottieni_quantita(char quant_u_misura[LUNG_STR_LAVORO]){
	char *parte_stringa; ///variabile "puntatore" che verrá sfruttato per individuare la parte non numerica della stringa
	int parte_intera;///variabile che accoglierá il numero presente nella stringa

	///la funzione strtol restituisce un long presente in una stringa qualora sia presente nel nostro caso il valore nuemrico della quantitá
	parte_intera = strtol(quant_u_misura, &parte_stringa, 10);


	return parte_intera; ///restituisci la quantitá dell'alimento che deve essere memorizzato nel frigo
}


/**
 *
 * @param line
 * @param num
 * @return token o stringa di riferimento
 */
const char* leggi_campo(char* line, int num){///passo il puntatore alla linea da cui voglio estrarre il campo e il numero del campo che voglio analizzare

    char* token; ///dichiariamo un token ossia un puntatore alla parte di stringa che di volta in volta viene spezzata
    token = strtok(line, ";");///inizializzazione della strtok con la stringa da tagliare e il/i carattere/i delimitatori. In questo caso basta il ; per come abbiamo strutturato il csv

    while (token!=NULL) ///smetti di tagliare la stringa nel momento in cui il token non punta ad alcuna sotto-stringa ottenuta dal carattere delimitatore ;
    {

    	num--; /// riduciamo di un campo la tupla presa in considerazione
        if (num==0){///ridotta al minimo la tupla iniziale, otterró la stringa che mi interessa ossia il valore del campo che ho deciso di passare alla funzione
            if(*token=='\n'){///controllo necessario per quegli aliementi che non possiedono una data di scadenza
            	return "vuoto";///in tal caso si restituisce una stringa si riferimento
            }else{
            	///DEBUG: printf("%s", tok);
            	return token;///restituisco il valore puntato dal token ossia la stringa interessata
            }
		}

        token = strtok(NULL, ";"); ///Praticamente la strtok mette degli '\0' nella stringa passata al posto del/i carattere/i "delimitatori",ricordiamoci che i caratteri delimitatori possono variare in questa dichiarazione

    }

    return token; ///restituisci NULL se abbiamo finito di analizzare il file
}


/**
 *
 * @param cat_alimenti_spesa
 * @param num_alim_spesa
 * @return
 */
void aggiorna_database(alim_spesa* cat_alimenti_spesa, int num_alim_spesa){
	char nome_file[LUNG_NOME_FILE];///
	strcpy(nome_file,"");
	strcat(nome_file, "../database_alimenti.csv");
	FILE* stream_database = fopen(nome_file, "rb+");///apre uno stream di lettura e scrittura in coda sul file del database

    int flag_presente=0;///questo flag viene attivato se un nuovo alimento che si vuole caricare nel frigo é giá stato memorizzato in passato
    int id=0;///l'id non é un dato memorizzato all'interno del file ma serve all'utente visivamente per velocizzare la sua interazione con il programma

	alimenti alimenti_database;///genero una struct di rifermiento per scorrere all'interno del file


  	for(int j=0;j<num_alim_spesa;j++){///ripeti per il numero di alimenti che sono stati caricati nel frigo
    	if(controllo_esist_file(nome_file)){///se il file del database ha almeno una riga

            while(fread(&alimenti_database,sizeof(alimenti_database),1,stream_database)>0){/// ripeti fino a quando é possibile prelevare una riga

            		///se l'elemento x della lista degli alimenti comprati e uguale ad un alimento presente nel database
					if(strcmp(cat_alimenti_spesa[j].nome_alimento, alimenti_database.nome_alimento)==0)
						flag_presente=1;///attiva il flag della presenza

				}
    	}

    	if(flag_presente==0){///se l'alimento non é presente nel database

    		printf("\n%s assente nel database\n", cat_alimenti_spesa[j].nome_alimento );///avviso l'utente su quale alimento manca
    		id++;///l'id viene incrementato alla "prossima riga" che al momento é vuota
    		alimenti_database.id=id;///viene memorizzato l'id nella struct
    		strcpy(alimenti_database.nome_alimento,cat_alimenti_spesa[j].nome_alimento);///viene memorizzato il nome del nuovo alimento nella struct
    		alimenti_database.quant_campione=set_q_camp(cat_alimenti_spesa[j].unita_misura);/// viene memorizzata la quantitá del campione che é stata calcolata tramite la funzione set_q_camp
    		strcpy(alimenti_database.unita_misura,cat_alimenti_spesa[j].unita_misura);
    		alimenti_database.soglia_spesa=set_soglia_spesa(cat_alimenti_spesa[j].quantita, cat_alimenti_spesa[j].unita_misura);/// viene memorizzata la quantitá minima da avere in frigo che é stata calcolata tramite la funzione set_soglia_spesa
    		alimenti_database.kcal=input_kcal(cat_alimenti_spesa[j].nome_alimento);///vengono memorizzate le kcal dell'alimento tramite la funzione input kcal
            fwrite(&alimenti_database,sizeof(alimenti_database),1,stream_database);///viene memorizzata la struct(il nuovo alimento) nel file

    	}

    	flag_presente=0;///viene reinizializzato il flag di presenza per l'alimento successivo
    	rewind(stream_database);///si riporta il puntatore del file all'inizio
    }

}


///ALE: serve un controllo sulla memorizzazione di alimenti con lo stesso nome e scadenza.In tal caso occorre aumentare la quantitá(se a questo hai difficoltá me la vedo io)
/**
 *
 * @param alimenti_comprati
 * @param num_alimenti
 * @return
 */
void aggiorna_frigo(alim_spesa* alimenti_comprati, int num_alimenti){

	char nome_file[LUNG_NOME_FILE];
	strcpy(nome_file, "../alimenti_frigo.csv");
	FILE *fp; ///creo un puntatore di tipo file

	if(controllo_esist_file(nome_file)){/// se il file degli alimenti presenti nel frigo esiste

		fp=fopen(nome_file,"rb+"); ///apri il file in modalitá "lettura binaria"
		alim_spesa struct_riferimento;///viene creata una struct di riferimento pre scorrere all'interno del file

		for(int i=0;i<num_alimenti;i++){///ripeti per il numero di alimenti che si devono memorizzare

			while(fread(&struct_riferimento,sizeof(struct_riferimento),1,fp)>0){///ripeti fino a quando é possibile leggere una riga dal file

				if(strcmp(struct_riferimento.nome_alimento, "")==0){/// se la riga é inizializzata ossia non ci sono dati relativi ad alimenti

						int currPos = ftell(fp);///viene memorizzata la locazione attuale di inizio riga
						fseek(fp,currPos-sizeof(struct_riferimento),SEEK_SET);///viene posizionato il puntatore del file ad inizio riga cosí da poter effettuare l'opzione di modifica
						fwrite(&alimenti_comprati[i],sizeof(alimenti_comprati[i]),1,fp);///viene memorizzato l'alimento x all'interno del file
						rewind(fp);///viene riportato il puntatore all'inizio del file
						i++;/// si passa all'alimento successivo da memorizzare

				}

			}

			fwrite(&alimenti_comprati[i],sizeof(alimenti_comprati[i]),1,fp);///se non sono state trovate righe da recuerare scrivi in coda al file
			rewind(fp);///riporta all'inizio il puntatore del file
		}

		fclose(fp);///una volta finita la scrittura chiudi il file

	}

	aggiorna_database(alimenti_comprati,num_alimenti);///richiama la funzione per aggiornare il database

}



///FUNZIONI MENU
/**
 *
 * @return 1 in caso di successo
 */
int visualizza_database_alimenti(){
	char nome_file[LUNG_NOME_FILE];
	strcpy(nome_file,"../database_alimenti.csv");

	FILE* stream_database = fopen(nome_file, "rb+");
	alimenti lista_alimenti;///creo una struct di tipo alimenti ome riferimento per scorrere all'interno del file
	printf("***************************************VISUALIZZA DATABASE****************************************\n");
	printf("  ID   | NOME ALIMENTO |   CAMPIONE    |   QUANTITA SOGLIA  |   UNITA DI MISURA  |      KCAL     |\n");
	///fino a quando puoi prelevare righe stampa il contenuto della struct ossia gli elementi di ogni riga
    while(fread(&lista_alimenti,sizeof(lista_alimenti),1,stream_database)>0){
        printf("\n  %5d|%15s|%15d|%20.1f|%20s|%15d|",lista_alimenti.id, lista_alimenti.nome_alimento,lista_alimenti.quant_campione,lista_alimenti.soglia_spesa, lista_alimenti.unita_misura, lista_alimenti.kcal);
    }

    return 1;
}


/**
 *
 * @return 1 in caso di successo
 */
int visualizza_frigo(){
	FILE *fp; ///creo un puntatore di tipo file
	fp=fopen("../alimenti_frigo.csv","rb+"); ///apri il file in modalitá "scrittura in coda"
	int id_alimento=0;///l'id non é un dato memorizzato all'interno del file ma serve all'utente visivamente per velocizzare la sua interazione con il programma

	alim_spesa alimenti_frigo;///creo una struttura di riferimento per scorrere all'interno del file
	printf("***************************************VISUALIZZA FRIGO*******************************************\n");
	printf("  ID  |    NOME_ALIMENTO    |      QUANTITA'      |    UNITA' MISURA    |      DATA SCADENZA     |");
    while(fread(&alimenti_frigo,sizeof(alimenti_frigo),1,fp)>0){///ripeti fino a quando é possibile leggere righe con le dimensioni dllla struct
    	if(strcmp(alimenti_frigo.nome_alimento, "")!=0){///stampa solo le righe non inizializzate
    		id_alimento++;///incrementa l'id solo per quegli alimenti che sono effettivamente nel frigo
    		printf("\n_________________________________________________________________________________________________\n");
    		///stampa il contenuto del file alimenti_frigo
    		printf("\n %5d| %20s| %20.1f| %20s| \t%10hu/%hu/%hu|",id_alimento, alimenti_frigo.nome_alimento,alimenti_frigo.quantita,alimenti_frigo.unita_misura,alimenti_frigo.scadenza.giorno,alimenti_frigo.scadenza.mese,alimenti_frigo.scadenza.anno);

    	}
    }

    return 1;///se la funzione é andata a buon fine restituisci 1
}


/**
 *
 * @param lista_frigo
 * @return 1 in caso di successo
 */
int leggi_frigo(alim_spesa* lista_frigo){
	FILE *fp; ///creo un puntatore di tipo file
	fp=fopen("../alimenti_frigo.csv","rb+"); ///apri il file in modalitá "scrittura in coda

	alim_spesa catalogo;///creo una struttura di riferimento per scorrere all'interno del file

	int indice_alimento=0;
    while(fread(&catalogo,sizeof(catalogo),1,fp)>0){///ripeti fino a quando é possibile leggere righe con le dimensioni dllla struct
    	if(strcmp(catalogo.nome_alimento, "")!=0){///stampa solo le righe non inizializzate
    		strcpy(lista_frigo[indice_alimento].nome_alimento,catalogo.nome_alimento);
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
int lettura_spesa(){
		char str_lavoro[LUNG_STR_LAVORO];///stringa che ospiterá la stringa avente quantitá e relativa unitá di misura. Questa stringa verrá poi opportunamente divisa

		char nome_file[LUNG_NOME_FILE];///dichiaro quanto deve essere grande il nome del file
		strcpy(nome_file, "../spesa_effettuata.csv");///inizializzo la variabile di tipo stringa al nome del file la path dove si trova il file

		int num_tuple=conta_righe_file_sq(nome_file);///creo una variabile che ospiterá il numero di righe del file da cui si vogliono recuperare gli alimenti

		alim_spesa lista_spesa[num_tuple]; ///dichiaro un'array di struct che ospiterá i valori dei singoli alimenti comprati
	    FILE* stream = fopen(nome_file, "r");///apro il file in lettura
	   	for(int indice_alimento=0;indice_alimento<num_tuple;indice_alimento++){/// ripeti fino a quando é possibile prelevare una riga

	   			fscanf(stream,"%s %s %hu/%hu/%hu", lista_spesa[indice_alimento].nome_alimento, str_lavoro, &lista_spesa[indice_alimento].scadenza.giorno, &lista_spesa[indice_alimento].scadenza.mese, &lista_spesa[indice_alimento].scadenza.anno);
				strcpy(lista_spesa[indice_alimento].unita_misura, ottieni_u_misura(str_lavoro));///memorizzo ció che é rimasto come stringa ossia l'unitá di misura della quantitá
				lista_spesa[indice_alimento].quantita=ottieni_quantita(str_lavoro);///memorizzo la quantitá numerica opportunamente nella struct

		    }

		///una volta caricato l'array di struct passiamo all'aggiornamento effettivo del frigo e alla memorizzazione degli alimenti
		aggiorna_frigo(lista_spesa,num_tuple);

		return 1;
	}


/**
 *
 * @return 1 in caso di successo
 */
int riduci_q_alimenti(){
	int id_alimento;///l'id non é un dato memorizzato nel file ma é un dato che aiuta l'utente nella fase di selezione
	int linea=0;///indice che ci aiuta a capire a quale riga siamo arrivati
	float q_riduzione;///come per la quantitá memorizzata nel file quella che deve essere sottratta deve essere un float

	char nome_file[LUNG_NOME_FILE];
	strcpy(nome_file,"../alimenti_frigo.csv");
	FILE *fp= fopen(nome_file,"rb+"); ///apri il file in modalitá "lettura binaria"

	visualizza_frigo(); ///viene stampato il contenuto attuale de frigo

	///viene chiesto la quantitá da sottrarre a quella attuale
	printf("\nInserisci id alimento a cui ridurre quantitá: ");
	///CONTROLLO SU ID VALIDO ALE sfrutta la funzione conta righe binary se necessario
	scanf("%d", &id_alimento);

	alim_spesa alimenti_frigo;///struct di riferimento per scorrere il file

    while(fread(&alimenti_frigo,sizeof(alimenti_frigo),1,fp)>0){///leggi fino a quando é presente una riga

    	if(strcmp(alimenti_frigo.nome_alimento, "")!=0)///se la linea non é vuota e possiede un alimento
    		++linea;///incrementa il numero di linee effettivamente piene del file del frigo

    	if(id_alimento==linea){///se l'id dell'alimento che si é preso come riferimento é uguale alla linea su cui ci troviamo abbiamo trovato l'aliemnto a cui apportarele modifiche
    		printf("\nInserisci quantita' da ridurre: ");
    		///ALE QUI DOUBLE CONTROL:controllo su validitá quantitá perché io non posso prendere 5 uova se ne ho 1
    		///in piú dovrai gestire bene come poter fare la differenza tra quantitá. Mi spiego. La quantitá giustamente é
    		/// un float. Se inserisco 2 al posto di 2.0 come quantitá che voglio prelevare il programma non esegue la differenza.
    		/// La domanda é : far inserire 2.0 é una cosa carina? Prevedi che vengano inserite quantitá intere e che il programma
    		///faccia comunque il suo lavoro
    		scanf("%f", &q_riduzione);

    		int currPos = ftell(fp);///scopro la locazione di memoria su cui mi trovo al momento
    		alimenti_frigo.quantita=alimenti_frigo.quantita-q_riduzione;///riduco la quantitá dell'alimento

      		if(alimenti_frigo.quantita==0){///se l'alimento si é esaurito notifica l'utente e inizializza la riga
      			printf("Attento: hai finito %s\n", alimenti_frigo.nome_alimento);
				strcpy(alimenti_frigo.nome_alimento,"");
				strcpy(alimenti_frigo.unita_misura,"");
				alimenti_frigo.scadenza.giorno=0;
				alimenti_frigo.quantita=0;
				alimenti_frigo.scadenza.mese=0;
				alimenti_frigo.scadenza.anno=0;
      		}
    		fseek(fp,currPos-sizeof(alimenti_frigo),SEEK_SET);///posiziona il puntatore sulla locazione di memoria da cui iniziare a scrivere
        	fwrite(&alimenti_frigo,sizeof(alimenti_frigo),1,fp);///scrivi il contenuto della struct aggiornata
        	break;///esci e termina l'inserimento
    	}

	  }


	fclose(fp); ///chiudi il file


	return 1;
}







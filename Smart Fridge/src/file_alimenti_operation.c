/*
 * file_operation.c
 *
 *  Created on: 26 apr 2018
 *      Author: david
 */

#include "file_alimenti_operation.h"
#define ALIMENTI_STRUCT



//FUNZIONI CONTROLLI

bool file_exists(const char * filename){

	FILE* stream = fopen(filename, "r");
	if (stream)//se il file si apre in lettura allora restituisci true altrimenti di default restituisci false
	{
		return true;
	}

	return false;
}







//FUNZIONI FILE

int conta_righe_file(char filename[LUNG_NOME_FILE]){

		FILE *fp= fopen(filename,"r"); //apri il file in modalitá "lettura"
		int righe=0;				   //variabile che conta le righe del file
		char  buffer[BUFSIZ];		   //stringa che accoglie la riga in lettura dal file

		while (fgets(buffer, sizeof buffer, fp)) //fino a quando riesci a leggere righe dal file
		    righe++;							   //incremento il contatore del numero di righe

		fclose(fp); 				  //chiudi il file
		return righe; 				  //restituisci il numero di righe del file

}



int conta_righe_file_binary(char filename[LUNG_NOME_FILE]){

		FILE *fp= fopen(filename,"rb"); //apri il file in modalitá "lettura binaria"
		int righe=0;				    //variabile che conta le righe del file
		alim_spesa catalogo;			//genero una struct di riferimento che mi permette di scorrere all'interno del file di tipo binario

        while(fread(&catalogo,sizeof(catalogo),1,fp)>0){//fino a quando riesci a leggere righe dal file
		    righe++;									//incremento il contatore del numero di righe
		  }
		//DEBUG:printf("%d\n", lines);

		fclose(fp); //chiudi il file
		return righe;

	}




int input_kcal(char nome_alimento[LUNG_NOME]){
	int kcal;
	printf("Inserisci le kcal per %s", nome_alimento );

	scanf("%d", &kcal );//controllo_input() di ALE
	return kcal;
}



const char* set_q_camp(char unita_misura[LUNG_U_MISURA]){
	char* campioni[NUM_CAMPIONI]={"100g", "100ml", "pz"};//memorizziamo dei campioni tipo all'interno di un array di puntatori

	if(strcmp(unita_misura, "g")==0 || strcmp(unita_misura, "kg")==0){// se l'unitá di misura dell'alimento e il g allora
		return *campioni;//restituisci 100g come campione
	}else if(strcmp(unita_misura, "ml")==0 || strcmp(unita_misura, "lt")==0){// se l'unitá di misura dell'alimento e il ml o lt allora
		return *(campioni+1);//restituisci 100ml come campione
	}else{// se l'unitá di misura dell'alimento e il pz allora
		return *(campioni+2);//restituisci pz come campione
	}
	return ""; //restituisci stringa vuota se qualcosa é andata storta
}




const char* set_soglia_spesa(float quantita, char unita_misura[LUNG_U_MISURA]){
	char* soglia_spesa[LUNG_SOGLIA];//definisco un puntatore char che punterá alla soglia una volta terminata la generazione
	float quantita_minima;
	char buf[LUNG_SOGLIA];//qui verrá memorizzata la quantitá minima convertita in stringa con in coda l'unitá di misura

	//ALE: effettua gli opportuni controlli qui sotto per arrontondare la quantitá in caso siano pz e soprattutto controlla
	//che il valore restituito in percentuale sia valido


	//se le unitá di misura sono tipizzate allora:
	if(strcmp(unita_misura, "g")==0 || strcmp(unita_misura, "ml")==0 || strcmp(unita_misura, "lt")==0){
		quantita_minima=(SOGLIA_PERC*quantita)/(float)100;//calcola x% della prima quantitá che si memorizza nel frigo
	    sprintf(buf, "%.1f", quantita_minima);//converti la quantitá in stringa
	    strcat(buf,unita_misura);//accoda l'unitá di misura

	}else{//altrimenti se la quantitá é in pz QUESTO NON VA BENE PERTANTO VA QUA IL TUO CONTROLLO
		quantita_minima=(SOGLIA_PERC*quantita)/(float)100;
		sprintf(buf, "%.1f", quantita_minima);
	    strcat(buf,unita_misura);

	}
	strcpy(*soglia_spesa, buf);//copia il valore di buf e fallo puntare da soglia_spesa
	return *soglia_spesa;//restituisci il puntatore della soglia
}




int split_data(alim_spesa* catalogo, char stringa[LUNG_STR_LAVORO], int num_alimento){

	char *parte_stringa; //dichiaro una variabile "puntatore" che punterá alla stringa della data che verrá di volta in colta tagliata
	char *token = strtok(stringa, "/");	//creo un puntatore che punterá alla prima locazione valida dopo che é stato effettuato il taglio prendendo come
										//delimitatore il carattere /

	for (int i=0; token != NULL; i++){//ripeti fino a quando il token non punta a null

		if(i==0){//se il contatore é 0 vuol dire che si sta puntando al giorno
			catalogo[num_alimento].scadenza.giorno= strtol(token, &parte_stringa, 10);//taglio il primo numero intero e lo memorizzo nella struct
		}else if(i==1){//se il contatore é 1 vuol dire che si sta puntando al mese
			catalogo[num_alimento].scadenza.mese= strtol(token, &parte_stringa, 10);//taglio il secondo numero intero e lo memorizzo nella struct
		}else{//se il contatore é diversodai valori precedenti vuol dire che si sta puntando all'anno
			catalogo[num_alimento].scadenza.anno= strtol(token, &parte_stringa, 10);//taglio l'ultimo intero rimanente e lo memorizzo nella struct
		}
		//DEBUG: printf("%s\n", token);
		token = strtok(NULL, "/");//il puntatore si deve spostare di / in / in quanto siamo nel formato data gg/mm/aaaa
	}

	//ALE: dentro l'if va la generazione della data 3 giorni dopo quella attuale. Non eliminare il commento di sotto ma questo :-)

	//questo controllo é utile nel caso non sia presente la data di scadenza nel file e pertanto occorre memorizzare una scadenza "orientativa"
	if(catalogo[num_alimento].scadenza.giorno==0 || catalogo[num_alimento].scadenza.mese==0 || catalogo[num_alimento].scadenza.anno==0){
		//DA BUTTARE. Memorizza una data valida
		catalogo[num_alimento].scadenza.giorno=1;
		catalogo[num_alimento].scadenza.mese=2;
		catalogo[num_alimento].scadenza.anno=2018;
	}
	//DEBUG: printf("%d/%d/%d\n", catalogo[num_alimento].scadenza.giorno,catalogo[num_alimento].scadenza.mese,catalogo[num_alimento].scadenza.anno );
	return 1;
}



int split_u_misura(alim_spesa* catalogo, char quant_u_misura[LUNG_STR_LAVORO], int num_alimento){

	char *parte_stringa; //variabile "puntatore" che verrá sfruttato per individuare la parte non numerica della stringa
	int parte_intera;//variabile che accoglierá il numero presente nella stringa

	//la funzione strtol restituisce un long presente in una stringa qualora sia presente nel nostro caso il valore nuemrico della quantitá
	parte_intera = strtol(quant_u_misura, &parte_stringa, 10);

	catalogo[num_alimento].quantita=parte_intera;//memorizzo la quantitá numerica opportunamente nella struct

	strcpy(catalogo[num_alimento].unita_misura, parte_stringa);//memorizzo ció che é rimasto come stringa ossia l'unitá di misura della quantitá

	//DEBUG
	//printf("%.2f\n", catalogo[num_alimento].quantita);
	//printf("%s\n", catalogo[num_alimento].unita_misura);

	return 1;
}



const char* leggi_campo(char* line, int num){//passo il puntatore alla linea da cui voglio estrarre il campo e il numero del campo che voglio analizzare

    char* tok; //dichiariamo un token ossia un puntatore alla parte di stringa che di volta in volta viene spezzata
    tok = strtok(line, ";");//inizializzazione della strtok con la stringa da tagliare e il/i carattere/i delimitatori. In questo caso basta il ; per come abbiamo strutturato il csv

    while (tok!=NULL) //smetti di tagliare la stringa nel momento in cui il token non punta ad alcuna sotto-stringa ottenuta dal carattere delimitatore ;
    {

    	num--; // riduciamo di un campo la tupla presa in considerazione
        if (num==0){//ridotta al minimo la tupla iniziale, otterró la stringa che mi interessa ossia il valore del campo che ho deciso di passare alla funzione
            if(*tok=='\n'){//controllo necessario per quegli aliementi che non possiedono una data di scadenza
            	return "vuoto";//in tal caso si restituisce una stringa si riferimento
            }else{
            	//DEBUG: printf("%s", tok);
            	return tok;//restituisco il valore puntato dal token ossia la stringa interessata
            }
		}

        tok = strtok(NULL, ";"); //Praticamente la strtok mette degli '\0' nella stringa passata al posto del/i carattere/i "delimitatori",ricordiamoci che i caratteri delimitatori possono variare in questa dichiarazione

    }

    return tok; //restituisci NULL se abbiamo finito di analizzare il file
}



int aggiorna_database(alim_spesa* cat_alimenti_spesa, int num_alim_spesa){
	char filename[LUNG_NOME_FILE];//
	strcpy(filename,"");
	strcat(filename, "../database_alimenti.csv");

	int num_tuple_database=conta_righe_file_binary(filename);//vengono contate le righe del file binario database_alimenti e viene memorizzato il risultato

	FILE* stream_database = fopen(filename, "ab+");//apre uno stream di lettura e scrittura in coda sul file del database

    int flag_presente=0;//questo flag viene attivato se un nuovo alimento che si vuole caricare nel frigo é giá stato memorizzato in passato
    int id= 0;//l'id non é un dato memorizzato all'interno del file ma serve all'utente visivamente per velocizzare la sua interazione con il programma

	alimenti lista_alimenti;//genero una struct di rifermiento per scorrere all'interno del file

	//DEBUG: visualizza_database();

  	for(int j=0;j<num_alim_spesa;j++){//ripeti per il numero di alimenti che sono stati caricati nel frigo
    	if(num_tuple_database!=0){//se il file del database ha almeno una riga

            while(fread(&lista_alimenti,sizeof(lista_alimenti),1,stream_database)>0){// ripeti fino a quando é possibile prelevare una riga

            		//se l'elemento x della lista degli alimenti comprati e uguale ad un alimento presente nel database
					if(strcmp(cat_alimenti_spesa[j].nome_alimento, lista_alimenti.nome_alimento)==0)
						flag_presente=1;//attiva il flag della presenza

				}
    	}

    	if(flag_presente==0){//se l'alimento non é presente nel database

    		printf("\n%s assente nel database\n", cat_alimenti_spesa[j].nome_alimento );//avviso l'utente su quale alimento manca
    		id++;//l'id viene incrementato alla "prossima riga" che al momento é vuota
    		lista_alimenti.id=id;//viene memorizzato l'id nella struct
    		strcpy(lista_alimenti.nome_alimento,cat_alimenti_spesa[j].nome_alimento);//viene memorizzato il nome del nuovo alimento nella struct
    		strcpy(lista_alimenti.quant_campione,set_q_camp(cat_alimenti_spesa[j].unita_misura));// viene memorizzata la quantitá del campione che é stata calcolata tramite la funzione set_q_camp
    		strcpy(lista_alimenti.soglia_spesa,set_soglia_spesa(cat_alimenti_spesa[j].quantita, cat_alimenti_spesa[j].unita_misura));// viene memorizzata la quantitá minima da avere in frigo che é stata calcolata tramite la funzione set_soglia_spesa
    		lista_alimenti.kcal=input_kcal(cat_alimenti_spesa[j].nome_alimento);//vengono memorizzate le kcal dell'alimento tramite la funzione input kcal
            fwrite(&lista_alimenti,sizeof(lista_alimenti),1,stream_database);//viene memorizzata la struct(il nuovo alimento) nel file

    	}

    	flag_presente=0;//viene reinizializzato il flag di presenza per l'alimento successivo
    	rewind(stream_database);//si riporta il puntatore del file all'inizio
    }

  	return 1;
}


//ALE: serve un controllo sulla memorizzazione di alimenti con lo stesso nome e scadenza.In tal caso occorre aumentare la quantitá(se a questo hai difficoltá me la vedo io)
int aggiorna_frigo(alim_spesa* catalogo_alimenti, int num_alimenti){

	char filename[LUNG_NOME_FILE];
	strcpy(filename,"");
	strcat(filename, "../alimenti_frigo.csv"); //accodo al nome del file il formato(in questo caso csv)
	FILE *fp; //creo un puntatore di tipo file

	if(file_exists(filename)){// se il file degli alimenti presenti nel frigo esiste

		fp=fopen(filename,"rb+"); //apri il file in modalitá "lettura binaria"
		alim_spesa struct_riferimento;//viene creata una struct di riferimento pre scorrere all'interno del file

		for(int i=0;i<num_alimenti;i++){//ripeti per il numero di alimenti che si devono memorizzare

			while(fread(&struct_riferimento,sizeof(struct_riferimento),1,fp)>0){//ripeti fino a quando é possibile leggere una riga dal file

				if(strcmp(struct_riferimento.nome_alimento, "")==0){// se la riga é inizializzata ossia non ci sono dati relativi ad alimenti

						int currPos = ftell(fp);//viene memorizzata la locazione attuale di inizio riga
						fseek(fp,currPos-sizeof(struct_riferimento),SEEK_SET);//viene posizionato il puntatore del file ad inizio riga cosí da poter effettuare l'opzione di modifica
						fwrite(&catalogo_alimenti[i],sizeof(catalogo_alimenti[i]),1,fp);//viene memorizzato l'alimento x all'interno del file
						rewind(fp);//viene riportato il puntatore all'inizio del file
						i++;// si passa all'alimento successivo da memorizzare

				}

			}

			fwrite(&catalogo_alimenti[i],sizeof(catalogo_alimenti[i]),1,fp);//se non sono state trovate righe da recuerare scrivi in coda al file
			rewind(fp);//riporta all'inizio il puntatore del file
		}

		fclose(fp);//una volta finita la scrittura chiudi il file

	}

	aggiorna_database(catalogo_alimenti,num_alimenti);//richiama la funzione per aggiornare il database

	return 1;
}



//FUNZIONI MENU

int visualizza_database(){
	char filename[LUNG_NOME_FILE];
	strcpy(filename,"");
	strcat(filename, "../database_alimenti.csv");

	FILE* stream_database = fopen(filename, "rb+");
	alimenti lista_alimenti;//creo una struct di tipo alimenti ome riferimento per scorrere all'interno del file

	//fino a quando puoi prelevare righe stampa il contenuto della struct ossia gli elementi di ogni riga
    while(fread(&lista_alimenti,sizeof(lista_alimenti),1,stream_database)>0){
        printf("\n \t%d\t%s\t%s\t%s\t%d",lista_alimenti.id, lista_alimenti.nome_alimento,lista_alimenti.quant_campione,lista_alimenti.soglia_spesa, lista_alimenti.kcal);
    }

    return 1;
}


int visualizza_frigo(){
	FILE *fp; //creo un puntatore di tipo file
	fp=fopen("../alimenti_frigo.csv","rb+"); //apri il file in modalitá "scrittura in coda"
	int id= 0;//l'id non é un dato memorizzato all'interno del file ma serve all'utente visivamente per velocizzare la sua interazione con il programma

	alim_spesa catalogo;//creo una struttura di riferimento per scorrere all'interno del file

    while(fread(&catalogo,sizeof(catalogo),1,fp)>0){//ripeti fino a quando é possibile leggere righe con le dimensioni dllla struct
    	if(strcmp(catalogo.nome_alimento, "")!=0){//stampa solo le righe non inizializzate
    		id++;//incrementa l'id solo per quegli alimenti che sono effettivamente nel frigo

    		//stampa il contenuto del file alimenti_frigo
    		printf("\n \t%d\t%s\t%.1f\t%s \t%d/%d/%d",id, catalogo.nome_alimento,catalogo.quantita,catalogo.unita_misura,catalogo.scadenza.giorno,catalogo.scadenza.mese,catalogo.scadenza.anno);

    	}
    }

    return 1;//se la funzione é andata a buon fine restituisci 1
}



int lettura_spesa(){
		char line[LUNG_TUPLA];//dichiariamo la stringa che conterrá l'intera tupla proveniente dal file
		char str_lavoro[LUNG_STR_LAVORO];//

		char filename[LUNG_NOME_FILE];//dichiaro quanto deve essere grande il nome del file
		strcpy(filename, "../spesa_effettuata.csv");//inizializzo la variabile di tipo stringa al nome del file la path dove si trova il file

		int num_tuple=conta_righe_file(filename);//creo una variabile che ospiterá il numero di righe del file da cui si vogliono recuperare gli alimenti
		//DEBUG: printf("%d\n",num_tuple);


		alim_spesa lista_spesa[num_tuple]; //dichiaro un'array di struct che ospiterá i valori dei singoli alimenti comprati
	    FILE* stream = fopen(filename, "r");//apro il file in lettura
	    //apertura_stream(stream);//controllo se lo stream é effettivamente attivo



	    int indice_alimento=0;//indice che servirá a scorrere l'array di struct lista_spesa durante la lettura del file
		while (fgets(line, LUNG_TUPLA, stream))// ripeti fino a quando é possibile prelevare una riga
		    {
		        for(int i=1;i<=NUM_CAMPI_SPESA;i++){//per ogni campo di ogni riga

		        	/*duplico la riga che fgets sta puntando. Questa operazione é necessaria per ogni campo in quanto essendo una variabile di
		        	 * tipo puntatore  le modifiche(i tagli dei campi che non ci servono) vengono effettuate sulla stringa stessa
		        	 */
		        	char* tmp = strdup(line);


		        	switch(i){//controllo il campo
						case 1:// se il campo é il primo(il nome dell'alimento):

							/*Copio il valore restituito dalla funzione leggi campo, a cui passo la riga duplicata che ho prelevato e l'indice
							 * del campo da cui estrarre il valore(in questo caso il campo 1 ossia del nome), all'interno del campo nome
							 * dell'array di struct*/
			        		strcpy(lista_spesa[indice_alimento].nome_alimento, leggi_campo(tmp, i));
			        		break;//

						case 2:// se il campo é il secondo(la quantita dell'alimento):

							/*Copio il valore restituito dalla funzione leggi campo, a cui passo la riga duplicata che ho prelevato e l'indice
							 * del campo da cui estrarre il valore(in questo caso il campo 2 ossia della quantitá), all'interno di una stringa
							 * di lavoro che verrá poi passata alla funzione split_u_misura(insieme all'array di struct e indice dell'alimento
							 * su cui si sta lavorando) per ottenere la quantitá numerica del prodotto e la rispettiva unitá di misura*/
							strcpy(str_lavoro, leggi_campo(tmp, i));
							split_u_misura(lista_spesa,str_lavoro,indice_alimento);
			        		break;

						case 3:// se il campo é il terzo(la scadenza dell'alimento):

							/*Copio il valore restituito dalla funzione leggi campo, a cui passo la riga duplicata che ho prelevato e l'indice
							 * del campo da cui estrarre il valore(in questo caso il campo 3 ossia della scadenza), all'interno di una stringa
							 * di lavoro che verrá poi passata alla funzione split_data(insieme all'array di struct e indice dell'alimento
							 * su cui si sta lavorando) per ottenere la il giorno, il mese e l'anno di scadenza*/
								strcpy(str_lavoro, leggi_campo(tmp, i));
								split_data(lista_spesa,str_lavoro,indice_alimento);
			        		break;

						default:
							break;
		        	}

		        }

		        indice_alimento++;//passa ad alimento successivo

		    }

		//una volta caricato l'array di struct passiamo all'aggiornamento effettivo del frigo e alla memorizzazione degli alimenti
		aggiorna_frigo(lista_spesa,indice_alimento);

		return 1;
	}


int riduci_q_alimenti(){
	int id_alimento;//l'id non é un dato memorizzato nel file ma é un dato che aiuta l'utente nella fase di selezione
	int linea=0;//indice che ci aiuta a capire a quale riga siamo arrivati
	float q_riduzione;//come per la quantitá memorizzata nel file quella che deve essere sottratta deve essere un float

	char filename[LUNG_NOME_FILE];
	strcpy(filename,"");
	strcat(filename, "../alimenti_frigo.csv");
	FILE *fp= fopen(filename,"rb+"); //apri il file in modalitá "lettura binaria"

	visualizza_frigo(); //viene stampato il contenuto attuale de frigo

	//viene chiesto la quantitá da sottrarre a quella attuale
	printf("\nInserisci id alimento a cui ridurre quantitá: ");
	//CONTROLLO SU ID VALIDO ALE sfrutta la funzione conta righe binary se necessario
	scanf("%d", &id_alimento);

	alim_spesa catalogo;//struct di riferimento per scorrere il file

    while(fread(&catalogo,sizeof(catalogo),1,fp)>0){//leggi fino a quando é presente una riga

    	if(strcmp(catalogo.nome_alimento, "")!=0)//se la linea non é vuota e possiede un alimento
    		++linea;//incrementa il numero di linee effettivamente piene del file del frigo

    	if(id_alimento==linea){//se l'id dell'alimento che si é preso come riferimento é uguale alla linea su cui ci troviamo abbiamo trovato l'aliemnto a cui apportarele modifiche
    		printf("\nInserisci quantita' da ridurre: ");
    		//ALE QUI DOUBLE CONTROL:controllo su validitá quantitá perché io non posso prendere 5 uova se ne ho 1
    		//in piú dovrai gestire bene come poter fare la differenza tra quantitá. Mi spiego. La quantitá giustamente é
    		// un float. Se inserisco 2 al posto di 2.0 come quantitá che voglio prelevare il programma non esegue la differenza.
    		// La domanda é : far inserire 2.0 é una cosa carina? Prevedi che vengano inserite quantitá intere e che il programma
    		//faccia comunque il suo lavoro
    		scanf("%f", &q_riduzione);

    		int currPos = ftell(fp);//scopro la locazione di memoria su cui mi trovo al momento
      		catalogo.quantita=catalogo.quantita-q_riduzione;//riduco la quantitá dell'alimento

      		if(catalogo.quantita==0){//se l'alimento si é esaurito notifica l'utente e inizializza la riga
      			printf("Attento: hai finito %s\n", catalogo.nome_alimento);
				strcpy(catalogo.nome_alimento,"");
				strcpy(catalogo.unita_misura,"");
				catalogo.scadenza.giorno=0;
				catalogo.quantita=0;
				catalogo.scadenza.mese=0;
				catalogo.scadenza.anno=0;
      		}
    		fseek(fp,currPos-sizeof(catalogo),SEEK_SET);//posiziona il puntatore sulla locazione di memoria da cui iniziare a scrivere
        	fwrite(&catalogo,sizeof(catalogo),1,fp);//scrivi il contenuto della struct aggiornata
        	break;//esci e termina l'inserimento
    	}

	  }

    //printf("%d", linea);
	fclose(fp); //chiudi il file


	return 1;
}







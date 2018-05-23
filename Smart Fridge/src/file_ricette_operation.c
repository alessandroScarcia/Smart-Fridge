/*
 * file_ricette_operation.c
 *
 *  Created on: 16 mag 2018
 *      Author: david
 */
#include "file_ricette_operation.h"
#define RICETTE_STRUCT
#include "file_alimenti_operation.h"


const char* leggi_campo_ricetta(char* line, int num){//passo il puntatore alla linea da cui voglio estrarre il campo e il numero del campo che voglio analizzare

    char* tok; //dichiariamo un token ossia un puntatore alla parte di stringa che di volta in volta viene spezzata
    tok = strtok(line, "-");//inizializzazione della strtok con la stringa da tagliare e il/i carattere/i delimitatori. In questo caso basta il ; per come abbiamo strutturato il csv

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

        tok = strtok(NULL, "-"); //Praticamente la strtok mette degli '\0' nella stringa passata al posto del/i carattere/i "delimitatori",ricordiamoci che i caratteri delimitatori possono variare in questa dichiarazione

    }

    return tok; //restituisci NULL se abbiamo finito di analizzare il file
}






int lettura_ricette(){
		char line[LUNG_TUPLA_RICETTE];//dichiariamo la stringa che conterrá l'intera tupla proveniente dal file
		char str_lavoro[LUNG_STR_LAVORO_RIC];//

		char filename[LUNG_NOME_FILE];//dichiaro quanto deve essere grande il nome del file
		strcpy(filename, "../nuove_ricette.csv");//inizializzo la variabile di tipo stringa al nome del file la path dove si trova il file

		int num_tuple=0;//creo una variabile che ospiterá il numero di righe del file da cui si vogliono recuperare gli alimenti
		//DEBUG: printf("%d\n",num_tuple);


		ricetta carica_ricette[num_tuple]; //dichiaro un'array di struct che ospiterá i valori dei singoli alimenti comprati
	    FILE* stream = fopen(filename, "r");//apro il file in lettura
	    //apertura_stream(stream);//controllo se lo stream é effettivamente attivo



	    int indice_ricetta=0;//indice che servirá a scorrere l'array di struct lista_spesa durante la lettura del file
		while (fgets(line, LUNG_TUPLA, stream))// ripeti fino a quando é possibile prelevare una riga
		    {
		        for(int i=1;i<=NUM_CAMPI_RICETTA;i++){//per ogni campo di ogni riga

		        	/*duplico la riga che fgets sta puntando. Questa operazione é necessaria per ogni campo in quanto essendo una variabile di
		        	 * tipo puntatore  le modifiche(i tagli dei campi che non ci servono) vengono effettuate sulla stringa stessa
		        	 */
		        	char* tmp = strdup(line);


		        	switch(i){//controllo il campo
						case CMP_NOME_RIC:// se il campo é il primo(il nome dell'alimento):
							strcpy(carica_ricette[indice_ricetta].nome_ricetta, leggi_campo(tmp,i));

			        		break;//

						case CMP_INGR:// se il campo é il secondo(la quantita dell'alimento):


						case CMP_TEMP_PREP:// se il campo é il terzo(la scadenza dell'alimento):
							strcpy(carica_ricette[indice_ricetta].tempo_prep, leggi_campo(tmp,i));

			        		break;
						case CMP_PREP:// se il campo é il terzo(la scadenza dell'alimento):

			        		break;

						case CMP_COMPLES:// se il campo é il terzo(la scadenza dell'alimento):
							strcpy(carica_ricette[indice_ricetta].complessita, leggi_campo(tmp,i));

			        		break;

						default:
							break;
		        	}

		        }

		        indice_ricetta++;//passa ad alimento successivo

		    }

		//una volta caricato l'array di struct passiamo all'aggiornamento effettivo del frigo e alla memorizzazione degli alimenti
		//aggiorna_frigo(lista_spesa,indice_alimento);

		for(int i=0;i<1;i++){
			printf("%s\n%s\n%s\n", carica_ricette[i].nome_ricetta, carica_ricette[i].tempo_prep, carica_ricette[i].complessita);
		}

		return 1;
	}

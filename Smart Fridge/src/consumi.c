#ifndef CONSUMI_LIB
#include "consumi.h"
#endif
/**
 *La funzione registra_consumi riceve in ingresso il nome di un prodotto dove per prodotto intendiamo il nome di una ricetta o di un alimento e
 *un flag dove 0 indica che siamo in presenza di un alimento e 1 che indica che siamo in presenza di una ricetta. Inizialmente viene fatto un check
 *un all'interno del file dei consumi per vedere se il prodotto é giá stato consumato in passato e qualora lo sia, basta solo incrementare la sua
 *un frequenza. Nel caso in cui sia la prima volta che lo si consuma viene creata una nuova riga in cui memorizzare nome, la categoria di appartenenza
 *e la frequenza iniziale
 * @pre		Il nome del prodotto deve essere una stringa con lunghezza maggiore di 0 e il flag deve avere un valore significativo (0 o 1)
 * @post	Deve essere stato effettuata e salvata la modifica su file
 */
int registra_consumo(char* nome_prodotto, short flag_prodotto){
	FILE* stream = NULL;
	fpos_t riga_consumo = -1;// ci aiuta a salvare la posizione corrente del puntatore al file
	consumo prodotto_consumo;

	if((stream = fopen(FILE_CONSUMI,"rb+")) == NULL){
		if((stream = fopen(FILE_CONSUMI,"wb+")) == NULL){
			return 0;
		}
	}

	while(fread(&prodotto_consumo, sizeof(consumo), 1, stream) > 0){//leggi fino a quando é presente una riga
		if(feof(stream) != 0){
			break;
		}

		//se ho trovato il nome del prodotto che ho passato alla funzione all'interno del file aggiorno la sua frequenza di consumo
		if(strcmp(prodotto_consumo.nome_prodotto, nome_prodotto) == 0){//se la linea non é vuota e possiede un alimento
			fseek(stream, -sizeof(consumo), SEEK_CUR);
			fgetpos(stream, &riga_consumo);
			break;
		}
	}


	//se il prodotto non é stato trovato all'interno del file vuol dire che é la prima volta che lo si consuma
	if(riga_consumo == -1){
		strcpy(prodotto_consumo.nome_prodotto, nome_prodotto);//copio il nome del prodotto
		prodotto_consumo.flag_prodotto = flag_prodotto;//memorizzo la tipologia di prodotto
		prodotto_consumo.frequenza = 1;
	}else{
		fsetpos(stream, &riga_consumo);
		prodotto_consumo.frequenza++;
	}

	fwrite(&prodotto_consumo, sizeof(consumo), 1, stream);

	fclose(stream);
	return 1;
}




/**
 * Questa funzione avendo in ingresso la tipologia di prodotto, stampa il prodotto di quella categoria maggiormente consumato. Ricordiamo che il flag
 * puó avere  1 in caso di ricette e 0 in caso di alimenti. Tramite un ciclo che analizza tutto il file dei consumi viene rintracciato la ricetta o
 * l'alimento con la frequenza maggiore. In caso questi non sia presente e la stringa del prodotto maggiormente consumato rimanga vuota verrá
 * stampato un mess di avviso
 * @pre		Il flag deve avere un valore significativo (0 o 1)
 * @post	Deve essere stato mostrato un messaggio in base all'esito e la ricerca deve essere terminata con successo
 */
int ricerca_prod_magg_cons(short flag_prodotto){
	FILE* stream = NULL; //apri il file in modalitá "lettura binaria"
	consumo prodotto_consumo;

	int freq_consumo = 0;
	char prod_magg_cons[LUNG_NOME_PRODOTTO];

	if((stream = fopen(FILE_CONSUMI, "rb+")) == NULL){
		return 0;
	}else{
		while(fread(&prodotto_consumo, sizeof(consumo), 1, stream) > 0){//leggi fino a quando é presente una riga

			if(feof(stream) != 0){
				break;
			}
			/*se il prodotto correntemente analizzato ha una frequenza maggiore ed é del tipo specificato dal flag(ricordiamo 1->ricette e 0->alimenti)
				 allora lo salvo come il prodotto maggiormente consumato*/
			if(prodotto_consumo.frequenza > freq_consumo && prodotto_consumo.flag_prodotto==flag_prodotto){
				freq_consumo = prodotto_consumo.frequenza;
				strcpy(prod_magg_cons,prodotto_consumo.nome_prodotto);
			}
		}

		//qualora sia stato trovato un prodotto lo stampo opportunatamente. In caso contrario avviso l'utente
		if(strcmp(prod_magg_cons,"") != 0){
			if(flag_prodotto == FLAG_ALIMENTO){
				printf("L'alimento maggiormente consumato e' %s con una frequenza di %d\n", prod_magg_cons, freq_consumo);
			}else if(flag_prodotto == FLAG_RICETTA){
				printf("La ricetta maggiormente consumata e' %s con una frequenza di %d\n", prod_magg_cons, freq_consumo);
			}
		}else{
			printf("Non sono presenti prodotti con il paramentro richiesto\n");
		}
	}

	fclose(stream); //chiudi il file
	return 1;
}

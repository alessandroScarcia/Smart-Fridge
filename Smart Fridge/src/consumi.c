/**
 * @file consumi.c
 *
 * Implementazione delle funzioni di cosumi.h
 */
#include "consumi.h"

/**
 * Funzione registra_consumo:
 * La funzione ha il compito di registrare la consumazione di un prodotto. Ciò significa incrementare il valore
 * di frequenza, nel record corrispondente al prodotto, in FILE_CONSUMI. A questo scopo, apre il file o lo crea
 * se non esiste. Successivamente effettua una scansione dei record del file. Se individua il record
 * corridpondente al prodotto, memorizza la sua posizione. Se viene memorizzata la posizione del record, viene
 * posizionato il puntatore a quel record e ne viene incrementato il valore di frequenza. Altrimenti viene
 * creato un nuovo record con valore di frequenza pari a 1.
 *
 * @pre		Il nome del prodotto deve essere una stringa non vuota e non più lunga di LUNG_NOME_PRODOTTO
 * @post	Deve essere controllato il valore di ritorno per verificare l'esito della registrazione
 */
int registra_consumo(char* nome_prodotto, short flag_prodotto){
	// Controlli sui parametri attuali
	int l_nome_prodotto = strlen(nome_prodotto);
	if(l_nome_prodotto < 1 || l_nome_prodotto >= LUNG_NOME_PRODOTTO){
		return 0;
	}

	if(flag_prodotto != FLAG_RICETTA && flag_prodotto != FLAG_ALIMENTO){
		return 0;
	}

	FILE* stream = NULL;				// Puntatore a FILE_CONSUMI
	fpos_t riga_consumo = -1;			// Posizione del record esistente per il prodotto interessato
	consumo prodotto_letto;				// Variabile utilizzata per I/O su FILE_CONSUMI

	// Apertura del file in lettura e aggiornamento binario, o creazione del file nuovo se inesistente
	if((stream = fopen(FILE_CONSUMI,"rb+")) == NULL){
		if((stream = fopen(FILE_CONSUMI,"wb+")) == NULL){
			// Se non può essere creato il file ritorna -1
			return -1;
		}
	}

	// Scansione del contenuto di FILE_CONSUMI
	while(fread(&prodotto_letto, sizeof(consumo), 1, stream) > 0){
		// Se si raggiunge la fine del file, è necessario terminare l'iterazione
		if(feof(stream) != 0){
			break;
		}

		// Se viene identificato il record del prodotto interessato, ne viene memorizzata la posizione
		if(strcmp(prodotto_letto.nome_prodotto, nome_prodotto) == 0){
			fseek(stream, -sizeof(consumo), SEEK_CUR);
			fgetpos(stream, &riga_consumo);
			break;
		}
	}


	// Se non è stato individuato il record del prodotto, viene posizionato il puntatore alla fine del file
	if(riga_consumo == -1){
		fseek(stream, 0, SEEK_END);

		// Viene generato il record da aggungere
		strcpy(prodotto_letto.nome_prodotto, nome_prodotto);//copio il nome del prodotto
		prodotto_letto.flag_prodotto = flag_prodotto;//memorizzo la tipologia di prodotto
		prodotto_letto.frequenza = 1;
	}else{
		// Altrimenti viene posizionato il puntatore al record del prodotto
		fsetpos(stream, &riga_consumo);

		// Viene incrementato il valore di frequenza nel record del prodotto torvato
		prodotto_letto.frequenza++;
	}

	// Scrittura del record creato o aggiornato
	fwrite(&prodotto_letto, sizeof(consumo), 1, stream);

	fclose(stream);
	return 1;
}



/**
 *
 *
 * @pre		Il flag deve avere un valore significativo.
 * @post	Deve essere controllato l'esito della ricerca, per verificare l'esito della funzione.
 */
int ricerca_prod_magg_cons(short flag_prodotto){
	// Controlli sui parametri attuali
	if(flag_prodotto != FLAG_RICETTA && flag_prodotto != FLAG_ALIMENTO){
		return 0;
	}

	FILE* stream = NULL; 						// Puntatore a FILE_CONSUMI
	consumo prodotto_letto;						// Variabile per le operazioni di I/O su FILE_CONSUMI

	int freq_consumo = 0;						// Variabile la frequenza del prodotto maggiormente consumato
	char prod_magg_cons[LUNG_NOME_PRODOTTO] = "";	// Stringa contenente il nome del prodotto maggiormente consumato


	// Apertura del file in lettura binaria
	if((stream = fopen(FILE_CONSUMI, "rb")) == NULL){
		puts("Non sono mai stati effettuati consumi.");
		return -1;
	}else{
		// Scansione di FILE_CONSUMI, memorizzando il prodotto maggiormente consumato
		while(fread(&prodotto_letto, sizeof(consumo), 1, stream) > 0){
			// Termina l'iterazione una volta raggionta la fine del file
			if(feof(stream) != 0){
				break;
			}

			if(prodotto_letto.frequenza > freq_consumo && prodotto_letto.flag_prodotto==flag_prodotto){
				freq_consumo = prodotto_letto.frequenza;
				strcpy(prod_magg_cons,prodotto_letto.nome_prodotto);
			}
		}

		// Stampa del prodotto maggiormente consumato, o di un messaggio di errore se non viene individuato
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

	fclose(stream);
	return 1;
}

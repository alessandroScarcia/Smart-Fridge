/*
 * calorie.c
 *
 *  Created on: 20 giu 2018
 *      Author: ciaone
 */

#include "calorie.h"



int input_alimento_consumato(assunzione* cibo) {

	short int risposta;
	ricetta r;
	alimento_database a;
	char nome [LUNG_NOME_RICETTA];

	do {

		printf("\nInserire 1 per registrare una ricetta, 0 per registrare un singolo alimento (-1 per uscire) :\n>");
		scanf("%hd", &risposta);


	}while (risposta!= 1 && risposta != 0 && risposta != -1);

	if (risposta == 1) {

		cibo->flag=1;

		printf("Inserisci nome della ricetta\n> ");
		scanf("%s", nome);

		if(esiste_ricetta(nome)==1){
			strcpy(cibo->nome, r.nome_ricetta);
			//cibo.kcal= calcolo calorie ricetta
		} else {
			//aggiorna database
		}
	} else if (risposta == 0) {

		cibo->flag=0;

		printf("Inserisci nome alimento:\n>");
		scanf("%s", nome);

		if(	ricerca_alimento_database(nome, &a)==1){
			strcpy(cibo->nome, a.nome);
			cibo->kcal = calcolo_kcal_alimento(a.kcal, a.campione_kcal);
		} else {
			//aggiorna_database
		}

	} else if(risposta==-1){

		return -1;

	}

	return 0;

}



unsigned short int calcolo_kcal_alimento(unsigned short int kcal, int campione) {

	unsigned short int risultato_kcal;
	float q_consumata;


	printf("\nInserire la quantita' consumata\n>");
	scanf("%f", &q_consumata);

	risultato_kcal = (q_consumata * kcal) / campione;

	return risultato_kcal;

}

int aggiorno_database_calorie(assunzione* cibo, char nickname[]) {

	char nome_file[LUNG_NOME_FILE] = "../assunzioni_";
	strcat(nome_file, nickname);
	strcat(nome_file, ".dat");

	FILE* stream;

	if((stream = fopen(nome_file, "rb+")) == NULL){
			// Se non esiste nessun file assunzioni deve essere creato
		if(creazione_assunzioni ()==-1){
					return -1;
				}
	}

	fseek(stream, 0, SEEK_END); //posizione il puntatore alla fine del file per permettere la scrittura in ordine rispetto ai giorni della settimana
	fwrite(cibo, sizeof(assunzione), 1, stream);

	fclose(stream);

	return 0;
}

int scrivi_data(char nickname[]) {

	data data_attuale = data_odierna();

	char nome_file[LUNG_NOME_FILE] = "../assunzioni_";
	strcat(nome_file, nickname);
	strcat(nome_file, ".dat");

	FILE* f;

	if ((f = fopen(nome_file, "wb"))==NULL){
		return -1;
	}

	fseek(f, 0, SEEK_SET);
	fwrite(&data_attuale, sizeof(data), 1, f);

	fclose(f);

	stampa_database_assunzioni() ;

	return 0;
}

int apertura_file_assunzioni() {

	utente persona;

	if (autenticazione(&persona) == 1) {
		data data_letta;
		assunzione cibo;
		int differenza;

		char nome_file[LUNG_NOME_FILE] = "../assunzioni_";
		strcat(nome_file, persona.nickname);
		strcat(nome_file, ".dat");

		FILE* f;

		if((f = fopen(nome_file, "rb+"))==NULL){
			if(creazione_assunzioni ()==-1) {
						return -1;
			}
		}

		fseek(f, 0, SEEK_SET);
		fread(&data_letta, sizeof(data), 1, f);

		fclose(f);

		if (diff_date(&differenza, data_letta, data_odierna()) == 0) {
			//reset
			scrivi_data(persona.nickname);
		}

		while (input_alimento_consumato(&cibo)==1) {

			aggiorno_database_calorie(&cibo, persona.nickname);
			printf("\nPer uscire, inserire -1\n");

		}

	stampa_database_assunzioni();
	return 0;
	} else {
		return 1;
	}
}

int stampa_database_assunzioni() {

	utente persona;

	if (autenticazione(&persona) == 1) {

		data data_letta;
		assunzione cibo;
		char nome_file[LUNG_NOME_FILE] = "../assunzioni_";
		strcat(nome_file, persona.nickname);
		strcat(nome_file, ".dat");

		FILE* f;

		if((f = fopen(nome_file, "rb+"))==NULL){
			if(creazione_assunzioni ()==-1) {
				return -1;
			}
		}

		fseek(f, 0, SEEK_SET);
		fread(&data_letta, sizeof(data), 1, f);

		printf("Assunzioni per %hu/%hu/%hu\n\n", data_letta.giorno,
				data_letta.mese, data_letta.anno);

		while (fread(&cibo, sizeof(assunzione), 1, f) > 0) {
			printf("Nome: %s\nQuantita': %.2f\nkcal: %hu\n\n", cibo.nome,
					cibo.quantita, cibo.kcal);
		}

		fclose(f);

		calcolo_kcal_totali(nome_file);
	}

	return 0;
}

int calcolo_kcal_totali(char nomefile[]) {

	FILE* f;
	assunzione cibo;
	unsigned short tot_kcal=0;

	if((f = fopen(nomefile, "rb+"))==NULL){

		if(creazione_assunzioni ()==-1) {
					return -1;
		}
	}

	fseek(f, sizeof(data), SEEK_SET);
	while (fread(&cibo, sizeof(assunzione), 1, f) > 0) {
		tot_kcal += cibo.kcal;
	}

	fclose(f);

    if(tot_kcal>0){
    	printf("Calorie assunte in totale: %hu", tot_kcal);
    }
	return 0;
}

int creazione_assunzioni (){

	utente persona;

	if (autenticazione(&persona) == 1) {

		scrivi_data(persona.nickname);
	} else {
		return 1;
	}

	return 0;
}

int modifica_consumo (){

	utente persona;



	if (autenticazione(&persona) == 1) {

		FILE* stream;

		char nome_file[LUNG_NOME_FILE] = "../assunzioni_";
		strcat(nome_file, persona.nickname);
		strcat(nome_file, ".dat");

		assunzione nuova_assunzione;
		short int posizione;

		printf("Inserisci il nome del cosumo da modifcare\n>");
		scanf("%s", nuova_assunzione.nome);

		posizione=ricerca_assunzione_database (&nuova_assunzione, persona.nickname);

		if (posizione==0 || posizione==-1){
			printf("Impossibile terminare la modifica.\nFunzione terminata.\n\n");
			return 0;
		}

		if (nuova_assunzione.quantita==0){
			//cancella da database
		} else {
			if (nuova_assunzione.flag==1){
				//calcolo kcal
			} else {
				alimento_database a;
				ricerca_alimento_database(nuova_assunzione.nome, &a);
				nuova_assunzione.kcal=calcolo_kcal_alimento(a.kcal, a.campione_kcal);
			}

			if((stream = fopen(nome_file, "rb+"))==NULL){

					if(creazione_assunzioni ()==-1) {
								return -1;
					}
				}
			fseek(stream, sizeof(data)+posizione*sizeof(assunzione), SEEK_SET);
			fwrite(&nuova_assunzione, sizeof(assunzione), 1, stream);
		}

		} else {
			return 1;
		}

		return 0;
}

short int ricerca_assunzione_database (assunzione* cibo, char nickname[]){

	assunzione lettura;
	short int posizione;

	abbassa_maiuscole (cibo->nome);

	char nome_file[LUNG_NOME_FILE] = "../assunzioni_";
	strcat(nome_file, nickname);
	strcat(nome_file, ".dat");

	FILE* f;

	if((f = fopen(nome_file, "rb+"))==NULL){
			return -1;
	}

	fseek(f, sizeof(data), SEEK_SET);

	while(feof(f) == 0){
				fread(&lettura, sizeof(assunzione), 1, f);
				posizione++;
				if(strcmp(lettura.nome, cibo->nome) == 0){
					return posizione;
				}
			}
	fclose (f);

	return 0;
}

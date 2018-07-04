/*
 * calorie.c
 *
 *  Created on: 20 giu 2018
 *      Author: ciaone
 */

#include "calorie.h"



int input_alimento_consumato() {

	utente persona;

	if (autenticazione(&persona)==1){
		assunzione cibo;
		int esito_input;
		cibo.flag=3;
		cibo.quantita=0;
        do {
			printf("\nInserire il nome di cio' che hai assunto\n>");
			scanf("%20s", cibo.nome);

			if(pulisci_stdin() == 1){
						esito_input = 0;
					}

			if(esito_input == 0){
				puts("Inserimento non valido. Ripeterlo.");
			}
		}while (esito_input == 0);

		do {
			printf("\nInserire calorie\n>");
			scanf("%hu", &cibo.kcal);
			if(pulisci_stdin() == 1){
					esito_input = 0;
			}

			if(esito_input == 0){
				puts("Inserimento non valido. Ripeterlo.");
			}
		}while (esito_input == 0);



		scrittura_diretta_assunzione (&cibo, persona.nickname);

		return 0;
	}

	return 1;

}



unsigned short int calcolo_kcal(unsigned short int kcal, int campione, float q_consumata) {

	if (q_consumata==0){
		return 0;
	}

	unsigned short int risultato_kcal;

	risultato_kcal = (q_consumata * kcal) / campione;

	return risultato_kcal;

}
// nome, flag, quantità
//controllo data
int aggiorno_database_calorie(char nome_consumo[], int flag_consumo, float quantita_consumo, char nickname[]) {

	char nome_file[LUNG_FILE_CALORIE] = "../assunzioni_";
	strcat(nome_file, nickname);
	strcat(nome_file, ".dat");

	assunzione cibo;


	strcpy(cibo.nome, nome_consumo);
	cibo.flag=flag_consumo;
	cibo.quantita=quantita_consumo;

	if (cibo.flag==1){

	  int  kcal_ricetta;
	  int dosi_ricetta;
	  estrai_kcal_ricetta(cibo.nome, &kcal_ricetta, &dosi_ricetta);
	  cibo.kcal=calcolo_kcal(kcal_ricetta, dosi_ricetta, cibo.quantita);

	} else if (cibo.flag==0) {

		alimento_database ricerca_a;
		ricerca_alimento_database(cibo.nome, &ricerca_a);
		cibo.kcal=calcolo_kcal(ricerca_a.kcal, ricerca_a.campione_kcal, cibo.quantita);

	}

	scrittura_diretta_assunzione (&cibo, nickname);



	return 0;
}

int scrivi_data(char nickname[]) {

	data data_attuale = data_odierna();

	char nome_file[LUNG_FILE_CALORIE] = "../assunzioni_";
	strcat(nome_file, nickname);
	strcat(nome_file, ".dat");

	FILE* f;

	if ((f = fopen(nome_file, "wb"))==NULL){
		return -1;
	}

	fseek(f, 0, SEEK_SET);
	fwrite(&data_attuale, sizeof(data), 1, f);

	fclose(f);

	return 0;
}



int stampa_database_assunzioni() {

	utente persona;

	if (autenticazione(&persona) == 1) {

		data data_letta;
		assunzione cibo;
		char nome_file[LUNG_FILE_CALORIE] = "../assunzioni_";
		strcat(nome_file, persona.nickname);
		strcat(nome_file, ".dat");

		FILE* f;

		if((f = fopen(nome_file, "rb+"))==NULL){
			if(scrivi_data(persona.nickname)==-1) {
				return -1;
			}
		}

		fseek(f, 0, SEEK_SET);
		fread(&data_letta, sizeof(data), 1, f);

		printf("Assunzioni per %hu/%hu/%hu\n\n", data_letta.giorno,
				data_letta.mese, data_letta.anno);

		while (fread(&cibo, sizeof(assunzione), 1, f) > 0) {
			if (cibo.kcal !=0){
			       printf("Nome: %s\nQuantita': %.2f\nkcal: %hu\n\n", cibo.nome,
					cibo.quantita, cibo.kcal);
			}
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

		if(scrivi_data (nomefile)==-1) {
					return -1;
		}
	}

	fseek(f, sizeof(data), SEEK_SET);
	while (fread(&cibo, sizeof(assunzione), 1, f) > 0) {
		if ( cibo.kcal !=0){
	    	 tot_kcal += cibo.kcal;
	    }
	}

	fclose(f);

    if(tot_kcal>0){
    	printf("Calorie assunte in totale: %hu", tot_kcal);
    }
	return 0;
}



int modifica_assunzione (){

	utente persona;



	if (autenticazione(&persona) == 1) {

		FILE* stream;

		char nome_file[LUNG_FILE_CALORIE] = "../assunzioni_";
		strcat(nome_file, persona.nickname);
		strcat(nome_file, ".dat");

		assunzione nuova_assunzione;
		short int posizione;
		int esito_input;

		do {
			printf("Inserisci il nome del cosumo da modifcare\n>");
			scanf("%s", nuova_assunzione.nome);
			if(pulisci_stdin() == 1){
				esito_input = 0;
			}

			if(esito_input == 0){
				puts("Inserimento non valido. Ripeterlo.");
			}
		}while (esito_input == 0);

		posizione=ricerca_assunzione_database (nuova_assunzione.nome, persona.nickname);

		if (posizione==0 || posizione==-1){
			printf("Impossibile terminare la modifica.\nFunzione terminata.\n\n");
			return 0;
		}

		printf("%d\n", nuova_assunzione.flag );
		if (nuova_assunzione.flag==3){
			do {
				printf("\nInserire le nuove kcal (0 per elimanre il consumo)\n>");
				scanf("%hu", &nuova_assunzione.kcal);
				if(pulisci_stdin() == 1){
					esito_input = 0;
				}

				if(esito_input == 0){
					puts("Inserimento non valido. Ripeterlo.");
				}
			}while (esito_input == 0);
		} else {

				do {
					printf("Inserire nuova quantita' (0 per elimare il consumo)\n>");
				    scanf("%f", &nuova_assunzione.quantita);
				    if(pulisci_stdin() == 1){
						esito_input = 0;
					}

					if(esito_input == 0){
						puts("Inserimento non valido. Ripeterlo.");
					}
				}while (esito_input == 0);

				if (nuova_assunzione.flag==1){

						  int  kcal_ricetta;
						  int dosi_ricetta;
						  estrai_kcal_ricetta(nuova_assunzione.nome, &kcal_ricetta, &dosi_ricetta);
						  nuova_assunzione.kcal=calcolo_kcal(kcal_ricetta, dosi_ricetta, nuova_assunzione.quantita);

					} else if (nuova_assunzione.flag==0){

						alimento_database a;
						ricerca_alimento_database (nuova_assunzione.nome, &a);
						nuova_assunzione.kcal=calcolo_kcal(a.kcal, a.campione_kcal, nuova_assunzione.quantita);

					}

		 }

			if((stream = fopen(nome_file, "rb+"))==NULL){

				printf("Errore\n");
				return 1;

			}
			fseek(stream, sizeof(data)+posizione*sizeof(assunzione)-sizeof(assunzione), SEEK_SET);
			fwrite(&nuova_assunzione, sizeof(assunzione), 1, stream);

			return 0;

		} else {
			return 1;
		}
}

short int ricerca_assunzione_database (char nome[], char nickname[]){

	assunzione lettura;
	short int posizione;

	abbassa_maiuscole (nome);

	char nome_file[LUNG_FILE_CALORIE] = "../assunzioni_";
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
				if(strcmp(lettura.nome, nome) == 0){
					return posizione;
				}
			}
	fclose (f);

	return 0;
}

int scrittura_diretta_assunzione (assunzione* cibo, char nickname[]){

	char nome_file[LUNG_FILE_CALORIE] = "../assunzioni_";
	strcat(nome_file, nickname);
	strcat(nome_file, ".dat");

	FILE* stream;
	data controllo_data;
	int differenza;

	if((stream = fopen(nome_file, "rb+"))==NULL){
			scrivi_data(nickname);
	}

	fseek(stream, 0, SEEK_SET);
	fread(&controllo_data, sizeof(data), 1, stream);

	if (diff_date(&differenza, controllo_data, data_odierna()) != 0) {
		//reset
		scrivi_data(nickname);
	}

	fseek(stream, 0, SEEK_END);
	fwrite(cibo, sizeof(assunzione), 1, stream);

	return 0;
}


void istogrami (){

	utente persona;

	if (autenticazione(&persona)== 1){

		char nome_file[LUNG_FILE_CALORIE] = "../assunzioni_";
		strcat(nome_file, persona.nickname);
		strcat(nome_file, ".dat");

		/*FILE* stream;
		data controllo_data;
		int differenza;*/
		int kcal_giornaliere;

		/*if((stream = fopen(nome_file, "rb+"))==NULL){
				scrivi_data(persona.nickname);
		}

		fseek(stream, 0, SEEK_SET);
		fread(&controllo_data, sizeof(data), 1, stream);

		if (diff_date(&differenza, controllo_data, data_odierna()) != 0) {
			//reset
			scrivi_data(persona.nickname);

		}*/

		estrai_kcal_menu(&kcal_giornaliere, persona.nickname, giorno_odierno());

		 printf("Assunzioni persona media: 2000 kcal: ");
		 for (int i=0; i<2000; i+=200){
			 printf("%c", 219);
		 }
		 printf("\nIl tuo obiettivo: %d", kcal_giornaliere);
		 for (int i=0; i < kcal_giornaliere; i+=200){
			 printf("%c", 219);
		 }
		 printf("\nIl tuo stato attuale: \n");
		 for (int i=0; i < calcolo_kcal_totali(persona.nickname); i+=200){
		 	 printf("%c", 219);
		 }

	}

}



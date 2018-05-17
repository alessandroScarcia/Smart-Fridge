/*
 * file_ricette_operation.h
 *
 *  Created on: 16 mag 2018
 *      Author: david
 */

#ifndef RICETTE_STRUCT
	//**********************************LIBRERIE INCLUSE***********************************
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



	//enum tipi_campioni { g, ml, lt, pz}; questa era pensato prima di creare la funzione set_q_campo


	//***************************DEFINIZIONI DI COSTANTI***********************************

	//LUNGHEZZE DI STRINGHE
#define LUNG_NOME_RIC 50
#define LUNG_INGR 20
#define LUNG_TEMP_PREP 20
#define LUNG_FASE 100
#define LUNG_COMPLESSITA 20
#define LUNG_TUPLA_RICETTE 1000//lunghezza della tupla ossia una riga quanto deve essere grande
#define LUNG_STR_LAVORO_RIC 100 //lunghezza della stringa che verrá utilizzata in varie funzioni per effettuare split o altre operazioni su stringhe
#define LUNG_NOME_FILE 100 //lunghezza da riservare al nome del file


	//NUMERO DI ELEMENTI DI ARRAY/MATRICI ECC
#define MAX_NUM_INGR 20
#define MAX_NUM_FASI 20
#define NUM_CAMPI_RICETTA 5



	//ALTRE COSTANTI
#define CMP_NOME_RIC 1
#define CMP_INGR 2
#define CMP_TEMP_PREP 3
#define CMP_PREP 4
#define CMP_COMPLES 5



	//*********************************DEFINIZIONI DI STRUCT***********************************

	//Questa struttura é utilizzata per i file binari per a memorizzazione dei dati relativi ad un alimento
typedef struct {
	char nome_ricetta[LUNG_NOME_RIC];
	char ingredienti[MAX_NUM_INGR][LUNG_INGR];
	char tempo_prep[LUNG_TEMP_PREP];
	char preparazione[MAX_NUM_FASI][LUNG_FASE];
	char complessita[LUNG_COMPLESSITA];
}ricetta;




#endif




const char* leggi_campo_ricetta(char* line, int num);
int lettura_ricetta();

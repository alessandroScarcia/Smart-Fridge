/*
 * utenti.h
 *
 *  Created on: 10 mag 2018
 *      Author: Michela
 */

#ifndef UTENTI_H_
#define UTENTI_H_
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define NOME_COGNOME_LENGTH 15 //lunghezza massima di un nome o un cognome
#define TRUE 0 //valori di verità
#define FALSE 1
#define LUNG_NOME_FILE 100 //Lunghezza massima del nome di un file
#define RIGHE_PREFERENZA 3 //Numero di righe della matrice preferenze appartenente alla struct utente, rappresenza la possibilità di avere
 	 	 	 	 	 	   // tre preferenze alimentari diverse
#define COLONNE_PREFERENZA 30 //Numero di righe della matrice preferenze appartenente alla struct utente, rappresenza la lunghezza massima di ciasciuna preferenza
#define LUNGHEZZA_PASSWORD 9 //Lunghezza della password della struct utente

#define SICUREZZA_MINIMA 1  //livelli di sicurezza della password
#define SICUREZZA_MEDIA 2
#define SICUREZZA_ALTA 3

#define SOMMA_CARATTERI_RICHIESTI_MIN 3 //somma di caratteri speciali e numerici minimi riservati per i controlli sulla password
#define SOMMA_CARATTERI_RICHIESTI_MAX 5 //somma di caratteri speciali e numerici massimi riservati per i controlli sulla password
#define NUM_CARATTERI_RICHIESTI 1 //il numero minimo di caratteri da inserire sia per i numeri che per i caratteri speciali
#define CARATTERI_ASCII_RISERVATI 89 //range di caratteri riservati per l'inserimento random di caratteri



typedef struct{
	int indice;
	char nome[NOME_COGNOME_LENGTH];
	char cognome[NOME_COGNOME_LENGTH];
    char password[LUNGHEZZA_PASSWORD];
    char preferenze[RIGHE_PREFERENZA][COLONNE_PREFERENZA]; //per preferenza si intende una preferenza di tipo alimentare (es: cioccolato, latte, etc..)
}utente;


#endif

void Salvataggio_utenti(int i); //salva le struct di tipo utente su un file .cvs
char* Generatore_Password(); //Genera una password da memorizzare nel campo password della struct di tipo utente.
int Aggiorna_databaseutenti(utente* persona); //Scrive su file .cvs las struct passata
int Modifica_nome(); //Modifa il nome di una delle struct salvate sul file
int Modifica_cognome(); //Modifa il cognome di una delle struct salvate sul file
int Modifica_password(); //Modifa la password di una delle struct salvate sul file
int Modifica_preferenza(); //Modifa una delle preferenze di una delle struct salvate sul file
char* AutoGeneratore_Password(); // Genera una password in modo casuale
char* Inserimento_password_manuale (int length); //Effettua controlli sulla stringa inserita da tastiera per verificare
                              	  	  	  	  	  	  	  	  	// che rispetti determinati criteri
int Identificazione (char* password, int length); //Confronta la password acquisita da tastiera con il campo password della struct letta da file
int Stampa_utente(); //Stampa su schermo una struct di tipo utente letta da file
int visualizza_database_utenti(); //visualizza il contenuto di un file
int Menu_databaseutenti (int i); //Stampa su schermo un messaggio e permette di scegliere tra diverse opzioni, chiamando funzioni oppurtune alla scelta acquisita da tastiera
int Conteggio_utenti (); //Conta il numero di struct di tipo utenti presenti su file

/**
 * @file pulizia_flussi.c
 *
 * Questo file contiene le implementazioni delle funzioni definite all'interno del file
 * "pulizia_flussi.h".
 */
#include "pulizia_flussi.h"

/**
 * La funzione pulisci_stdin() utilizza la funzione getchar() per consumare
 * tutti i caratteri presenti nel flusso stdin, fino al raggiungimento di EOF.
 *
 * @post Il valore di ritorno deve essere controllato per verificare se è stata necessaria la pulizia.
 */
int pulisci_stdin(){
	int esito = 0;
	char c;
	while((c = getchar()) != '\n' && c != EOF){
		esito = 1;
	}
	return esito;
}


/**
 * La procedura pulisci_riga_flusso() utilizza la funzione fgetc() per consumare
 * tutti i caratteri presenti in un flusso, puntato dal puntatore a file ricevuto come parametro stream,
 * fino al raggiungimento del primo carattere di newline.
 *
 * @param stream Flusso nel quale consumare i caratteri.
 */
void pulisci_riga_flusso(FILE* stream){
	char c;
	while((c = fgetc(stream)) != '\n'){
		if(c == EOF){
			break;
		}
	}
}

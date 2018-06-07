/**
 * @file pulizia_flussi.h
 *
 * Libreria per effettuare la pulizia dei flussi di input.
 *
 * Le funzioni implementate in questa libreria in generale permettono eliminare parti o l'intero contenuto
 * di flussi aperti. L'obiettivo è quello di sopperire agli errori fra un operazione di input e l'altra, dovuti
 * alla presenza di input non consumato.
 */

// Inclusione delle librerie standard
#ifndef STD_LIB
#define STD_LIB
	#include <stdbool.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
#endif

#ifndef PULIZIA_FLUSSI_LIB
#define PULIZIA_FLUSSI_LIB

/**
 * La procedura pulizia_stdin() permette di consumare l'input rimasto non consumato
 * all'interno del flusso stdin.
 */
void pulisci_stdin();

/**
 * La procedura pulizia_riga_flusso() permette di consumare l'input rimasto non
 * consumato da una riga di un file (fino al primo carattere di new line).
 */
void pulisci_riga_flusso(FILE* stream);

#endif

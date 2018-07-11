/**
 * @file main.c
 *
 * @brief Main del programma Smart Fridge
 *
 * Le funzionalit� del programma sono implementate atttraverso 8 librerie diverse,
 * supportate da altre due per gestioni generiche (date.h e pulizia_flussi.h). L'interazione
 * con l'utente avviene principalmente attraverso la libreria interfaccia.h, la quale permette
 * di selezionare le funzionalit� che si vogliono utilizzare.
 */

#include "interfaccia.h"

int main(){

	menu_principale();

	return 0;
}

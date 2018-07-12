/**
 * @file interfaccia.h
 *
 * @brief Libreria per l'interfaccia utente.
 *
 * La libreria interfaccia.h è stata implementata per permettere all'utente
 * l'interazione con il programma attraverso dei menu che mostrano le varie
 * operazioni applicabili per un determinato argomento
 *
 * @authors Alessandro Scarcia, Davide Quatela, Michela Salvemini
 */

#ifndef INTERFACCIA_LIB
#define INTERFACCIA_LIB

#ifndef STD_LIB
#define STD_LIB
	#include <stdbool.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>
#endif

///Inclusione della libreria matematica
#ifndef MATH_LIB
#define MATH_LIB
	#include <math.h>
#endif

/// Inclusione delle librerie per la manipolazione di stringhe e caratteri
#ifndef STRING_LIB
#define STRING_LIB
	#include <string.h>
	#include <ctype.h>
#endif

#include "pulizia_flussi.h"

#include "alimenti.h"

#include "ricette.h"

#include "utenti.h"

#include "menu_settimanale.h"


#include "calorie.h"

#include "lista_spesa.h"

#include "consumi.h"



///COSTANTI PER LA VISUALIZZAZIONE DELLE VOCI DEL MENU
#define MENU_PRINCIPALE 0						///flag rappresentate il menu principale e permetterne la stampa
#define MENU_ALIMENTI 1							/// flag rappresentate il menu alimenti e permetterne la stampa
#define MENU_RICETTE 2						   /// flag rappresentate il menu ricette e permetterne la stampa
#define MENU_UTENTI 3							/// flag rappresentate il menu utenti e permetterne la stampa
#define OPZIONI_MENU_SETTIMANALE 4              ///  flag rappresentate il menu settimanale e permetterne la stampa
#define MENU_CALORIE 5       					/// flag rappresentate il menu calorie e permetterne la stampa
#define MENU_LISTA_SPESA 6   					///  flag rappresentate il menu lista spesa e permetterne la stampa
#define MENU_RICERCA 7							/// flag rappresentate il menu ricercae permetterne la stampa



#define CONTORNO_SUPERIORE 1					/// flag che rappresentala riga superiore che deve essere visualizzata durante la visualizzazione della schermata dei menu
#define CONTORNO_INFERIORE 0					/// flag che rappresentala riga inferiore che deve essere visualizzata durante la visualizzazione della schermata dei menu


///COSTANTI PER DETERMINARE IL RANGE DEI VALORI DI SCELTA NEL MENU
#define MIN_SCELTA_PRINCIPALE 0					/// scelta minima che l’utente può effettuare in menu principale
#define MAX_SCELTA_PRINCIPALE 99				///scelta massima che l’utente può effettuare in menu principale

#define MIN_SCELTA_RICETTA 0					///scelta minima che l’utente può effettuare in menu ricetta
#define MAX_SCELTA_RICETTA 6					/// scelta massima che l’utente può effettuare in menu ricetta

#define MIN_SCELTA_UTENTE 0						/// scelta minima che l’utente può effettuare in menu utente
#define MAX_SCELTA_UTENTE 4						///  scelta massima che l’utente può effettuare in menu utente

#define MIN_SCELTA_ALIMENTI 0					/// scelta minima che l’utente può effettuare in menu alimenti
#define MAX_SCELTA_ALIMENTI 6					///	scelta massima che l’utente può effettuare in menu alimenti

#define MIN_SCELTA_RICERCA 0					/// scelta minima che l’utente può effettuare in menu ricerca
#define MAX_SCELTA_RICERCA 6					/// scelta massima che l’utente può effettuare in menu ricerca

#define MIN_SCELTA_SPESA 0						/// scelta minima che l’utente può effettuare in menu spesa
#define MAX_SCELTA_SPESA 4						///  scelta massima che l’utente può effettuare in menu spesa

#define MIN_SCELTA_MENU_SET 0					/// scelta minima che l’utente può effettuare in menu settimanale
#define MAX_SCELTA_MENU_SET  4					/// scelta massima che l’utente può effettuare in menu settimanale

#define MIN_SCELTA_MENU_CALORIE 0				/// scelta minima che l’utente può effettuare in menu calorie
#define MAX_SCELTA_MENU_CALORIE  4				/// scelta massima che l’utente può effettuare in menu calorie



/**funzione che si occupa di controllare se sono stati inseriti effettivamente valori numerici da parte dell'utente. Qualora sia stato inserito un valore non valido viene notificato un
 * errore e viene chiesto di reinserire il valore. In caso contrario viene restituito il valore inserito.
 * @param valore_minimo
 * @param valore_massimo
 * @return
 */
int input_scelta(int valore_minimo, int valore_massimo);

/**menu che mostra le operazioni che si possono effettuare sulla sezione alimenti
 *
 * @return
 */
int menu_alimenti();

/**menu che mostra le operazioni che si possono effettuare sulla sezione principale
 *
 * @return
 */
int menu_principale();

/**menu che mostra le operazioni che si possono effettuare sulla sezione ricette
 *
 * @return
 */
int menu_ricette();




/**menu che mostra le operazioni che si possono effettuare sulla sezione ricette
 *
 * @return
 */
int menu_lista_spesa();




/**menu che mostra le operazioni che si possono effettuare sulla sezione ricette
 *
 * @return
 */
int menu_utenti();



/**menu che mostra le operazioni che si possono effettuare sulla sezione ricette
 *
 * @return
 */
int menu_calorie();



/**menu che mostra le operazioni che si possono effettuare sulla sezione ricette
 *
 * @return
 */
int gestore_menu_settimanale();




/**menu che mostra le operazioni che si possono effettuare sulla sezione di ricerca delle ricette
 *
 * @return
 */
int menu_ricerca();

/**funzione che si occupa di contare le notifiche
 *
 * @return
 */
int conta_notifiche();

/**funzione che si occupa di mostrare le notifiche da segnalare all'utente
 *
 * @return
 */
int visualizza_notifiche();

/**funzione che si occupa di mostrare le varie opzioni disponibili per un menu in base al valore corrispondente del menu
 *
 * @param voce
 * @return
 */
int visualizza_voci_menu(int voce);

/**funzione che si occupa di...mostrare i creatori del programma
 *
 * @return
 */
int visualizza_crediti();

/**
 * Funzione che si occupa della visualizzazione del contorno del titolo
 */
void riga_contorno(int posizione);

/**
 * Funzione che si occupa della visualizzazione della riga inferiore del contorno della schermata
 */
void parte_inferiore_tabella();

/**
 * Funzione che si occupa della visualizzazione delle voci del menu principale
 */
void voci_principale();

/**
 * Funzione che si occupa della visualizzazione delle voci del menu alimenti
 */
void voci_alimenti();

/**
 * Funzione che si occupa della visualizzazione delle voci del menu ricette
 */
void voci_ricette();

/**
 * Funzione che si occupa della visualizzazione delle voci del menu utenti
 */
void voci_utenti();

/**
 * Funzione che si occupa della visualizzazione delle voci del menu ricerca
 */
void voci_ricerca();

/**
 * Funzione che si occupa della visualizzazione delle voci del menu lista della spesa
 */
void voci_lista_spesa();

/**
 * Funzione che si occupa della visualizzazione delle voci del menu settimanale
 */
void voci_menu_settimanale();

/**
 * Funzione che si occupa della visualizzazione delle voci del menu calorie
 */
void voci_calorie();

#endif

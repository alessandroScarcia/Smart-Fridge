/**
 * menu_alimenti.h
 *
 *  Created on: 27 apr 2018
 *      Author: david
 */

#define LUNG_SCELTA 2//lunghezza della stringa che accoglierá la scelta inserita dall'utente


//COSTANTI PER LA VISUALIZZAZIONE DELLE VOCI DEL MENU
#define MENU_PRINCIPALE 1
#define MENU_ALIMENTI 2
#define MENU_RICETTE 3
#define SOTT_MENU_RICETTE 4


//COSTANTI PER DETERMINARE IL RANGE DEI VALORI DI SCELTA NEL MENU
#define MIN_SCELTA_PRINCIPALE 0
#define MAX_SCELTA_PRINCIPALE 99

#define MIN_SCELTA_RICETTA 0
#define MAX_SCELTA_RICETTA 6


#define MIN_SCELTA_ALIMENTI 0
#define MAX_SCELTA_ALIMENTI 5

#define MIN_SCELTA_RICERCA_RIC 0
#define MAX_SCELTA_RICERCA_RIC 4


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

/**menu che mostra le operazioni che si possono effettuare sulla sezione di ricerca delle ricette
 *
 * @return
 */
int menu_ricette_ricerca();

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

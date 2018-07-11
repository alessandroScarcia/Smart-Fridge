/**
 * @file date.h
 *
 * @brief Libreria per l'interfaccia utente.
 *
 * La libreria interfaccia.h è stata implementata per permettere all'utente
 * l'interazione con il programma attraverso dei menu che mostrano le varie
 * operazioni applicabili per un determinato argomento
 *
 * @authors Alessandro Scarcia, Davide Quatela, Michela Salvemini
 */


#include "alimenti.h"

#include "date.h"

#include "ricette.h"

#include "lista_spesa.h"

#include "utenti.h"

#include "menu_settimanale.h"

#include "calorie.h"

#define LUNG_SCELTA 2//lunghezza della stringa che accoglierá la scelta inserita dall'utente


///COSTANTI PER LA VISUALIZZAZIONE DELLE VOCI DEL MENU
#define MENU_PRINCIPALE 0
#define MENU_ALIMENTI 1
#define MENU_RICETTE 2
#define MENU_UTENTI 3
#define OPZIONI_MENU_SETTIMANALE 4
#define MENU_CALORIE 5
#define MENU_LISTA_SPESA 6
#define MENU_RICERCA 7



#define SUPERIORE 1
#define INFERIORE 0


///COSTANTI PER DETERMINARE IL RANGE DEI VALORI DI SCELTA NEL MENU
#define MIN_SCELTA_PRINCIPALE 0
#define MAX_SCELTA_PRINCIPALE 99

#define MIN_SCELTA_RICETTA 0
#define MAX_SCELTA_RICETTA 6

#define MIN_SCELTA_UTENTE 0
#define MAX_SCELTA_UTENTE 4

#define MIN_SCELTA_ALIMENTI 0
#define MAX_SCELTA_ALIMENTI 6

#define MIN_SCELTA_RICERCA 0
#define MAX_SCELTA_RICERCA 6

#define MIN_SCELTA_SPESA 0
#define MAX_SCELTA_SPESA 4

#define MIN_SCELTA_MENU_SET 0
#define MAX_SCELTA_MENU_SET  4

#define MIN_SCELTA_MENU_CALORIE 0
#define MAX_SCELTA_MENU_CALORIE  4



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



int menu_calorie();

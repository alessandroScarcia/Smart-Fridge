/*
 * menu.c
 *
 *  Created on: 27 apr 2018
 *      Author: david
 */


#include "menu.h"
#include "alimenti.h"
#include "date.h"
#include "ricette.h"
#include "lista_spesa.h"
#include "utenti.h"
#include "menu_settimanale.h"

/**
 * La funzione input scelta si occupa di ricevere in input il comando per accedere ad una determinata voce dei menu. Si � pensato che la scelta
 * essendo un numero avr� anche esso un range. Se la scelta � compresa nel range vuol dire che � accettabile altrimenti verr� scartata e verr�
 * richiesto di reinserire il valore. Con la funzione  strtol siamo in grado di capire inoltre se viene inserito anche un carattere all'interno
 * della scelta e pertanto siamo in grado di prevenire un errore di tipo per l'input
 *
 * @pre  Che vengano passati dei valori significativi di massimo e minimo
 * @post Il valore restituito deve essere un intero
 */
int input_scelta(int valore_minimo, int valore_massimo) {
	char num_input[LUNG_SCELTA]; //dichiaro una stringa composta da 10 caratteri che ospiter� l'input( la stringa ricordati che � un array di caratteri)
	char *parte_stringa; //dichiaro una variabile "puntatore" che punter� ad una locazione di memoria dell'array di caratteri sopra dichiarato
	long parte_intera; //dichiaro una variabile di tipo long che ospiter� la parte intera(il numero) della stringa num_input

	do {//ripeti fino a quando...
		scanf("%2s", num_input); // assegno alla stringa num_input massimo 10 caratteri in ingresso(se inserisco la stringa aaaaaaaaaaaaaaaaaaaaaaaaaaa prende i primi 10 caratteri e questo perch� hai a disposizione "10 celle" di caratteri
		parte_intera = strtol(num_input, &parte_stringa, 10); //divido la stringa in ingresso in parte numerica(assegnata a parte intera) e la parte di caratteri(il puntatore parte_stringa punter� alla locazione contenente un carattere alfa

		if (*parte_stringa != 0 || parte_intera > valore_massimo || parte_intera < valore_minimo) {//se il puntatore punta a un carattere alfabetico(non punta alla locazione 0) e i vincoli di min e max sono rispettati allora:
			printf("Attenzione input non valido:"); //messaggio di avviso
			scanf("%*[^\n ]"); //libera il buffer della scanf
		}
	} while (*parte_stringa != 0 || parte_intera > valore_massimo || parte_intera < valore_minimo); //...sono presenti numeri e lettere(parte stringa punta ad una locazione di input_int nella stringa in ingresso e i vincoli non sono rispettati
	pulisci_stdin();
	return (int)parte_intera;//restituisci il numero che � stato inserit
}




/**
 * La funzione visualizza_voci_menu allggerisce il codice delle funzioni dei menu e si occupa di mostrare opportunatamente le voci appertenenti ad un determinato menu
 * @pre  Che la variabile voce sia un valore significativo
 * @post Che siano state visualizzate le opzioni corrette per ogni menu
 */
int visualizza_voci_menu(int voce){
	system("cls");
	switch(voce){
		case MENU_PRINCIPALE:
			printf(" --------------------                                                        ---------------\n");
			printf("|Benvenuto Professore|                                                      |Ha %d notifiche|\n" , conta_notifiche());
			printf(" --------------------                                                        ---------------\n");
			printf("    ||                                                                              ||\n");
			printf(" __________________________________________________________________________________________\n");
			printf("|                                *****MENU PRINCIPALE*****                                 |\n");
			printf("|[1] per accedere al menu alimenti                                                         |\n");
			printf("|[2] per accedere al menu ricette                                                          |\n");
			printf("|[3]  per accedere al menu utenti                                                          |\n");
			printf("|[4]  per accedere al menu spesa                                                           |\n");
			printf("|[5]  per accedere al menu menu settimanale                                                |\n");
			printf("|[6] per visualizzare le notifiche                                                         |\n");
			printf("|[0] per uscire                                                                            |\n");
			printf(" __________________________________________________________________________________________\n");
			printf("       ||                         ||  (c)redit  ||                                        \n");
			printf("       ||                         ________________                                   \n");
			printf("       ||                                                              \n");
			printf("       ||                                                                 \n");
			break;
		case MENU_ALIMENTI:
			printf(" ________________________________________________________________________________________\n");
			printf("|                                *****MENU ALIMENTI*****                                 |\n");
			printf("|[1] per aggiungere alimenti da file                                                     |\n");
			printf("|[2] riduci quantita' alimento da frigo                                                  |\n");
			printf("|[3] per visualizzare il database degli alimenti memorizzati                             |\n");
			printf("|[4] per visualizzare alimenti attualmente nel frigo                                     |\n");
			printf("|[5] visualizza alimenti maggiormente consumati                                          |\n");
			printf("|[0] per uscire                                                                          |\n");
			printf(" ________________________________________________________________________________________\n");
			printf("       ||                                                                               \n");
			break;
		case MENU_RICETTE:
			printf(" _______________________________________________________________________________________\n");
			printf("|                                *****MENU RICETTE*****                                 |\n");
			printf("|[1] per aggiungere ricette al frigo                                                    |\n");
			printf("|[2] per visualizzare ricette aggiunte al database                                      |\n");
			printf("|[3] per eliminare ricetta dal database                                                 |\n");
			printf("|[4] per modificare una ricetta dal database                                            |\n");
			printf("|[5] ricerca ricetta                                                                    |\n");
			printf("|[6] funzione ricette maggiormente consumate                                            |\n");
			printf("|[0] per uscire                                                                         |\n");
			printf(" _______________________________________________________________________________________\n");
			printf("       ||                                                                               \n");
			break;
		case SOTT_MENU_RICETTE:
			printf(" ________________________________________________________________________________________________\n");
			printf("|                                *****MENU RICERCA*****                                          |\n");
			printf("|[1] per visualizzare ricette suggerite in base ad ingredienti disponibili nel frigo             |\n");
			printf("|[2] per visualizzare ricette suggerite in base ad ingredienti inseriti  manualmente             |\n");
			printf("|[3] visualizzare ricette in ordine di kcal                                                      |\n");
			printf("|[4] suggerisci ricetta in base ad alimenti in scadenza                                          |\n");
			printf("|[0] per tornare indietro                                                                        |\n");
			printf(" ________________________________________________________________________________________________\n");
			printf("       ||                                                                               \n");
			break;
		case MENU_LISTA_SPESA:
			printf(" ________________________________________________________________________________________________\n");
			printf("|                      *****MENU LISTA DELLA SPESA*****                                          |\n");
			printf("|[1] per generare lista della spesa personale             |\n");
			printf("|[2] per visualizzare lista della spesa globale |\n");
			printf("|[3] per generare lista della spesa personale                                                     |\n");
			printf("|[4] per visualizzare lista della spesa personale                                                     |\n");
			printf("|[0] per tornare indietro                                                                        |\n");
			printf(" ________________________________________________________________________________________________\n");
			printf("       ||                                                                               \n");
			break;
		case OPZIONI_MENU_SETTIMANALE:
			printf(" ________________________________________________________________________________________________\n");
			printf("|                      *****GESTIONE MENU SETTIMANALE*****                                      |\n");
			printf("|[1] per inizializzare il menu settimanale |\n");
			printf("|[2] per modificare menu personale             |\n");
			printf("|[3] per visualizzare menu personale             |\n");
			printf("|[0] per tornare indietro                                                                        |\n");
			printf(" ________________________________________________________________________________________________\n");
			printf("       ||                                                                               \n");
			break;
	}
	printf("------------------\n");

	return 1;

}


/**Funzione che si occupa di mostare le operazioni urgenti che l'utente ddeve effettuare. Nel caso in cui il valore delle notifiche sia maggiore
 * di 1 e quindi il contatore degli alimenti scaduti segnala che  un alimento o pi� sono scaduti occorre chiedere se l'utente intende eliminare tali
 * alimenti o tenerli. Nel caso in cui l'utente accettasse occorre richiamare la funzione che si occupa dell'eliminazione degli alimenti scaduti.
 * In caso non ci siano notifiche da segnalare viene mostrato un messaggio.
 * @pre  Nessuna pre condizione particolare
 * @post Che venga effettuato con successo l'eliminazione degli alimenti scaduti in caso di accettazione da parte dell'utente
 */
int visualizza_notifiche(){
	int flag_elimina_alimenti;
	if(conta_notifiche()==0){
		printf("Non ci sono notifiche da segnalare\n");

	}else{
		if(conta_alimenti_scaduti()>0){
			printf("Sono presenti alimenti scaduti\n");
			visualizza_alimenti_scaduti();
			printf("Vuoi eliminarli automaticamente? [1]==>s    [0]==>n \n");
			scanf("%d", &flag_elimina_alimenti);
			if(flag_elimina_alimenti==1){
				eliminazione_alimenti_scaduti();
			}
		}
	}


	return 1;
}


/**
 * Funzione che serve a contare tutte le tipologie di notifiche che vanno mostrate nel men� principale
 * @pre  Nessuna pre condizione particolare
 * @post Il valore restituito deve essere un intero significativo (>=0)
 *  */
int conta_notifiche (){
	int avvisi=0;
	avvisi=avvisi+conta_alimenti_scaduti();
	return avvisi;
}



int menu_lista_spesa(){
			int scelta; //memorizza la scelta effettuata dall'utente
			utente u;

			visualizza_voci_menu(MENU_LISTA_SPESA);
			printf("Inserisci comando ~ ");

			do{//ripeti fino a quando...

				scelta=input_scelta(MIN_SCELTA_SPESA,MAX_SCELTA_SPESA);

				switch (scelta) {//in base alla scelta vai alla funzione corrispondente
				case 0://esci
				  return 1;
				  break;

				case 1:
					lettura_alimenti_acquistabili();
					break;

				case 2:
					visualizza_lista_spesa(LISTA_SPESA_GLOBALE);
					break;

				case 3:
					gestore_spesa_personale();
					break;

				case 4:

					//autenticazione(con controllo su esito) dell'utente
					if (autenticazione(&u) == -1){
						printf("Operazione di utenticazione fallita");
						return -1;
					}

					//generazione del nome del file che ospiter� la spesa personale
					char nome_file[MAX_LUNG_NOMEFILE] = PRIMA_PARTE_NOMEFILE;
					strcat(nome_file, u.nickname);
					strcat(nome_file, FORM_FILE_SPESA);
					visualizza_lista_spesa(nome_file);
					break;

				default:
					puts("Incorrect command");
					break;

				}


				if(scelta){// se la scelta non � quella di uscire allora chiedi di inserire un nuovo valore
					system("pause");
					visualizza_voci_menu(MENU_LISTA_SPESA);
					printf("\nSeleziona un'altra scelta del menu ricette ~ ");
				}


			}while(scelta);//l'utente non esce

			return 0;
}




int gestore_menu_settimanale(){
			int scelta; //memorizza la scelta effettuata dall'utente
			visualizza_voci_menu(OPZIONI_MENU_SETTIMANALE);
			utente u;
			printf("Inserisci comando ~ ");

			do{//ripeti fino a quando...

				scelta=input_scelta(MIN_SCELTA_MENU_SET,MAX_SCELTA_MENU_SET);

				switch (scelta) {//in base alla scelta vai alla funzione corrispondente
				case 0://esci
				  return 1;
				  break;

				case 1:
					inizializzazione();
					break;

				case 2:
					scelta_pasto();
					break;

				case 3:
					if(autenticazione(&u))
						visualizza_database_menu(u.nickname);
					break;

				default:
					puts("Incorrect command");
					break;

				}


				if(scelta){// se la scelta non � quella di uscire allora chiedi di inserire un nuovo valore
					system("pause");
					visualizza_voci_menu(OPZIONI_MENU_SETTIMANALE);
					printf("\nSeleziona un'altra scelta del menu ricette ~ ");
				}


			}while(scelta);//l'utente non esce

			return 0;
}









/**
 * Funzione che si occupa di passare gli alimenti presenti nel frigo e le ricette salvate nel database e di effettuare ricerche e ordinamenti su
 * tali dati estratti.
 * @pre  Nessuna pre condizione particolare
 * @post Che sia stato mostrato il giusto risultato in ase alla richiesta dell'utente
 */
int menu_ricette_ricerca(){
	int scelta;
	char nome_alimenti[MAX_ALIM_SUGG][LUNG_NOME_ALIMENTO];
	visualizza_voci_menu(SOTT_MENU_RICETTE);

	int num_ricette=conta_ricette_database();
	ricetta ricette_database[num_ricette];
	lettura_database_ricette(ricette_database);

	int num_alimenti=conta_alimenti_frigo();
	alimento_frigo alimenti_frigo[num_alimenti];
	leggi_frigo(alimenti_frigo);


	printf("Inserisci comando ~ ");

	do{
		scelta=input_scelta(MIN_SCELTA_RICERCA_RIC,MAX_SCELTA_RICERCA_RIC);

		switch (scelta) {//in base alla scelta vai alla funzione corrispondente
					case 0://esci
					  break;

					case 1:
						suggerimento_ricetta_automatico(alimenti_frigo,num_alimenti);
						break;

					case 2:
						num_alimenti=inserimento_manuale_ingredienti(nome_alimenti);
						suggerimento_ricetta_manuale(num_alimenti,nome_alimenti);
						break;

					case 3:
						ordina_ricette_kcal(ricette_database,num_ricette);
						break;
					case 4:
						ricette_alimenti_in_scadenza(alimenti_frigo,num_alimenti);
						break;

					default:
						puts("Incorrect command");
						break;
		}

		if(scelta){// se la scelta non � quella di uscire allora chiedi di inserire un nuovo valore
			system("pause");
			visualizza_voci_menu(SOTT_MENU_RICETTE);
			printf("\nSeleziona un'altra scelta del menu ~ ");
		}

	}while(scelta);
	return 1;
}


/**Funzione che si occupa di far selezionare all'utente le operazioni possibili da effettuare sui dati di tipo ricetta. La scelta dell'opzione
 * viene fatta attraverso una switch
 * @pre  Nessuna pre condizione particolare
 * @post Il risultato deve essere quello che l'opzione scelta si occupa
 */
int menu_ricette(){
			int scelta; //memorizza la scelta effettuata dall'utente

			visualizza_voci_menu(MENU_RICETTE);
			printf("Inserisci comando ~ ");

			do{//ripeti fino a quando...
				//CONTROLLO SULLA SCELTA ALE fallo momentaneamente su questo mini menu cos� ci alleggeriamo pi� avanti
				scelta=input_scelta(MIN_SCELTA_RICETTA,MAX_SCELTA_RICETTA);

				switch (scelta) {//in base alla scelta vai alla funzione corrispondente
				case 0://esci
				  return 1;
				  break;

				case 1:
					lettura_nuove_ricette();//visualizza gli alimenti che abbiamo memorizzato nel frigo
					break;

				case 2:
					visualizza_database_ricette(VISTA_TOTALE);//visualizza gli alimenti che abbiamo memorizzato nel frigo
					break;

				case 3:
					elimina_ricetta();//visualizza gli alimenti che abbiamo memorizzato nel frigo
					break;

				case 4:
					modifica_ricetta();
					break;

				case 5:
					menu_ricette_ricerca();
					break;

				case 6:
					ricerca_prod_magg_cons(FLAG_RICETTA);
					break;


				default:
					puts("Incorrect command");
					break;

				}


				if(scelta){// se la scelta non � quella di uscire allora chiedi di inserire un nuovo valore
					system("pause");
					visualizza_voci_menu(MENU_RICETTE);
					printf("\nSeleziona un'altra scelta del menu ricette  ~ ");
				}


			}while(scelta);//l'utente non esce

			return 0;
}


/**Funzione che si occupa di far selezionare all'utente le operazioni possibili da effettuare sui dati di tipo alimenti. La scelta dell'opzione
 * viene fatta attraverso una switch
 * @pre  Nessuna pre condizione particolare
 * @post Il risultato deve essere quello che l'opzione scelta si occupa
 */
int menu_alimenti(){
	  	int scelta; //memorizza la scelta effettuata dall'utente

	  	visualizza_voci_menu(MENU_ALIMENTI);
		printf("Inserisci comando ~ ");
		do{//ripeti fino a quando...
			//CONTROLLO SULLA SCELTA ALE fallo momentaneamente su questo mini menu cos� ci alleggeriamo pi� avanti
			scelta=input_scelta(MIN_SCELTA_ALIMENTI,MAX_SCELTA_ALIMENTI);

			switch (scelta) {//in base alla scelta vai alla funzione corrispondente
			case 0://esci
				return 1;

			case 1:
				elimina_file_spesa(LISTA_SPESA_GLOBALE);
				carica_spesa();//leggi file della spesa
				break;

			case 2:
				 gestore_riduzione_alimenti();//elimina alimenti dal frigo
				break;

			case 3:
				visualizza_database_alimenti();//visualizza gli alimenti che si sono memorizzati nel database
				break;

			case 4:
				visualizza_frigo();//visualizza gli alimenti che abbiamo memorizzato nel frigo
				break;

			case 5:
				ricerca_prod_magg_cons(FLAG_ALIMENTO);
				break;
			default:
				puts("Incorrect command");
				break;

			}


			if(scelta!=0){// se la scelta non � quella di uscire allora chiedi di inserire un nuovo valore
				system("pause");
				visualizza_voci_menu(MENU_ALIMENTI);
			    printf("\nSeleziona un'altra scelta del menu alimenti  ~  ");
			}



		}while(scelta);//l'utente non esce

		return 0;

}

/**Funzione che si occupa di far selezionare all'utente le operazioni possibili da effettuare sul menu principale. La scelta dell'opzione
 * viene fatta attraverso una switch
 * @pre  Nessuna pre condizione particolare
 * @post Il risultato deve essere quello che l'opzione scelta si occupa
 */
int menu_principale(){
  	int scelta; //memorizza la scelta effettuata dall'utente

	visualizza_voci_menu(MENU_PRINCIPALE);
	printf("Inserisci comando ~ ");

	do{//ripeti fino a quando...

		scelta=input_scelta(MIN_SCELTA_PRINCIPALE,MAX_SCELTA_PRINCIPALE);

		switch (scelta) {
		case 0:
		  printf("Buona giornata");
		  break;

		case 1:
			menu_alimenti();
			break;

		case 2:
			menu_ricette();
			break;

		case 3:
			menu_database_utenti(1);
			break;

		case 4:
			menu_lista_spesa();
			break;

		case 5:
			gestore_menu_settimanale();
			break;

		case 6:
			visualizza_notifiche();
			break;

		case 99://easter egg?
			visualizza_crediti();
			break;

		default:
			puts("Incorrect command");
			break;

		}


		if(scelta!=0){
			system("pause");
			visualizza_voci_menu(MENU_PRINCIPALE);
			printf("\nSeleziona un'altra scelta del menu principale  ~ ");
		}



	}while(scelta);//l'utente non esce
	return 1;
}




/**
 * @pre  Nessuna pre condizione particolare
 * @post Il valore restituito deve essere una stringa oppure un puntatore null
 */
int visualizza_crediti(){
	printf("Software a cura di : \n"
			"Alessandro Scarcia :futuro imperatore del mondo\n"
			"Michela Salvemini: futura programmatrice di alto livello\n"
			"Davide Quatela: un semplice programmatore che scrivera'la storia\n"
			"Speriamo le sia piaciuto questo piccolo Easter Egg e ovviamente tutto il progetto \n");
	return 1;
}


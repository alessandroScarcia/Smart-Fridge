
#include "interfaccia.h"

/**
 * La funzione input scelta si occupa di ricevere in input il comando per accedere ad una determinata voce dei menu. Si e' pensato che la scelta
 * essendo un numero avrá anche esso un range. Se la scelta e' compresa nel range vuol dire che e' accettabile altrimenti verrá scartata e verrá
 * richiesto di reinserire il valore.
 *
 * @pre  Che vengano passati dei valori significativi di massimo e minimo
 * @post Il valore restituito deve essere un intero
 */
int input_scelta(int valore_minimo, int valore_massimo) {
	// Controlli sui valori attuali
	if(valore_minimo >= valore_massimo){
		return -1;
	}

	if(valore_minimo < 0 || valore_massimo < 0){
		return -1;
	}

	int esito_input;
	int esito_controllo;
	int scelta;

	do {
		esito_input = scanf("%d", &scelta);
		if(pulisci_stdin() == 1){
			esito_input = 0;
		}

		if(scelta < valore_minimo || scelta > valore_massimo){
			esito_controllo = 0;
		}else{
			esito_controllo = 1;
		}

		if(esito_input != 1 || esito_controllo != 1){
			puts("Inserimento non valido. Ripeterlo.");
		}
	} while (esito_input != 1 || esito_controllo != 1);


	return scelta;
}


/**Funzione che si occupa della visualizzazione del contorno del titolo
 * @pre  Nessuna particolare pre condizione
 * @post Che sia visualizzata correttamente la linea di contorno in base alla posizione
 */
void riga_contorno(int posizione){
	if(posizione==CONTORNO_SUPERIORE){
		printf("%c",201);
	}else{
		printf("%c",200);
	}


	for(int i=0;i<90;i++){
		printf("%c",205);
	}

	if(posizione==CONTORNO_SUPERIORE){
		printf("%c\n",187);
	}else{
		printf("%c\n",188);
	}
}

/**Funzione che si occupa della visualizzazione della riga inferiore del contorno della schermata
 * @pre  Nessuna particolare pre condizione
 * @post Che sia stata visualizzata correttamente la parte la linea di contorno
 */
void parte_inferiore_tabella(){
	printf("%c                                                                                          %c\n", 179, 179);
	printf("%c",192);
	for(int i=0;i<90;i++){
		printf("%c",196);
	}
	printf("%c\n",217);
}



/**Funzione che si occupa della visualizzazione delle voci del menu principale
 * @pre  Nessuna particolare pre condizione
 * @post Che siano state visualizzate le opzioni corrette per il menu
 */
void voci_principale(){
	if(conta_notifiche()>0){
		printf(" ##############################\n"
		" # Hai degli alimenti scaduti #\n"
		" ############################## \n");
	}

	riga_contorno(CONTORNO_SUPERIORE);
	printf("%c                                     MENU PRINCIPALE                                      %c\n", 186,186);
	riga_contorno(CONTORNO_INFERIORE);
	printf("%c                                                                                          %c\n", 179, 179);
	printf("%c [1] per accedere al menu alimenti                                                        %c\n", 179, 179);
	printf("%c [2] per accedere al menu ricette                                                         %c\n", 179, 179);
	printf("%c [3] per accedere al menu utenti                                                          %c\n", 179, 179);
	printf("%c [4] per accedere al menu settimanale                                                     %c\n", 179, 179);
	printf("%c [5] per accedere al menu calorie                                                         %c\n", 179, 179);
	printf("%c [6] per accedere al menu spesa                                                           %c\n", 179, 179);
	printf("%c [7] per accedere al menu di ricerca                                                      %c\n", 179, 179);
	printf("%c [8] per visualizzare le notifiche                                                        %c\n", 179, 179);
	printf("%c [0] per uscire                                                                           %c\n", 179, 179);
	parte_inferiore_tabella();
	printf("                                                                              %c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",218,196,196,196,196,196,196,196,196,196,196,196,196,191);
	printf("                                                                              %c  (c)redit  %c \n",179,179);
	printf("                                                                              %c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",192,196,196,196,196,196,196,196,196,196,196,196,196,217);

}



/**Funzione che si occupa della visualizzazione delle voci del menu alimenti
 * @pre  Nessuna particolare pre condizione
 * @post Che siano state visualizzate le opzioni corrette per il menu
 */
void voci_alimenti(){
	riga_contorno(CONTORNO_SUPERIORE);
	printf("%c                                     MENU ALIMENTI                                        %c\n", 186,186);
	riga_contorno(CONTORNO_INFERIORE);
	printf("%c                                                                                          %c\n", 179, 179);
	printf("%c [1] caricamento spesa                                                                    %c\n", 179, 179);
	printf("%c [2] per visualizzare alimenti attualmente nel frigo                                      %c\n", 179, 179);
	printf("%c [3] riduci quantita' alimento da frigo                                                   %c\n", 179, 179);
	printf("%c [4] per visualizzare il database degli alimenti memorizzati                              %c\n", 179, 179);
	printf("%c [5] modifica soglia spesa alimento                                                       %c\n", 179, 179);
	printf("%c [6] modifica kcal alimento                                                               %c\n", 179, 179);
	printf("%c [0] per uscire                                                                           %c\n", 179, 179);
	parte_inferiore_tabella();

}


/**Funzione che si occupa della visualizzazione delle voci del menu ricette
 * @pre  Nessuna particolare pre condizione
 * @post Che siano state visualizzate le opzioni corrette per il menu
 */
void voci_ricette(){
	riga_contorno(CONTORNO_SUPERIORE);
	printf("%c                                     MENU RICETTE                                         %c\n", 186,186);
	riga_contorno(CONTORNO_INFERIORE);
	printf("%c                                                                                          %c\n", 179, 179);
	printf("%c[1] per aggiungere nuove ricette                                                          %c\n", 179, 179);
	printf("%c[2] per visualizzare ricette                                                              %c\n", 179, 179);
	printf("%c[3] per modificare una ricetta dal database                                               %c\n", 179, 179);
	printf("%c[4] per eliminare ricetta dal database                                                    %c\n", 179, 179);
	printf("%c[5] per accedere al gestore di preparazione ricetta                                       %c\n", 179, 179);
	printf("%c[0] per uscire                                                                            %c\n", 179, 179);
	parte_inferiore_tabella();

}



/**Funzione che si occupa della visualizzazione delle voci del menu utenti
 * @pre  Nessuna particolare pre condizione
 * @post Che siano state visualizzate le opzioni corrette per il menu
 */
void voci_utenti(){
	riga_contorno(CONTORNO_SUPERIORE);
	printf("%c                                     MENU UTENTE                                          %c\n", 186,186);
	riga_contorno(CONTORNO_INFERIORE);
	printf("%c                                                                                          %c\n", 179, 179);
	printf("%c[1] per creare nuovi utenti                                                               %c\n", 179, 179);
	printf("%c[2] per visualizzare tutti gli utenti                                                     %c\n", 179, 179);
	printf("%c[3] per modificare il tuo profilo                                                         %c\n", 179, 179);
	printf("%c[4] per eliminare un utente                                                               %c\n", 179, 179);
	printf("%c[0] per uscire                                                                            %c\n", 179, 179);
	parte_inferiore_tabella();

}


/**Funzione che si occupa della visualizzazione delle voci del menu ricerca
 * @pre  Nessuna particolare pre condizione
 * @post Che siano state visualizzate le opzioni corrette per il menu
 */
void voci_ricerca(){
	riga_contorno(CONTORNO_SUPERIORE);
	printf("%c                                     MENU RICERCA                                         %c\n", 186,186);
	riga_contorno(CONTORNO_INFERIORE);
	printf("%c                                                                                          %c\n", 179, 179);
	printf("%c[1] visualizza alimenti piu' consumati                                                    %c\n", 179, 179);
	printf("%c[2] visualizza ricette  piu' consumate                                                    %c\n", 179, 179);
	printf("%c[3] visualizzare ricette in ordine di kcal                                                %c\n", 179, 179);
	printf("%c[4] ricerca ricette preparabili                                                           %c\n", 179, 179);
	printf("%c[5] ricerca ricette con ingredienti chiave                                                %c\n", 179, 179);
	printf("%c[6] ricerca ricetta in base ad alimenti in scadenza                                       %c\n", 179, 179);
	printf("%c[0] per tornare indietro                                                                  %c\n", 179, 179);
	parte_inferiore_tabella();

}



/**Funzione che si occupa della visualizzazione delle voci del menu lista della spesa
 * @pre  Nessuna particolare pre condizione
 * @post Che siano state visualizzate le opzioni corrette per il menu
 */
void voci_lista_spesa(){
	riga_contorno(CONTORNO_SUPERIORE);
	printf("%c                                     MENU LISTA DELLA SPESA                               %c\n", 186,186);
	riga_contorno(CONTORNO_INFERIORE);
	printf("%c                                                                                          %c\n", 179, 179);
	printf("%c[1] per generare lista della spesa globale                                                %c\n", 179, 179);
	printf("%c[2] per visualizzare lista della spesa globale                                            %c\n", 179, 179);
	printf("%c[3] per generare lista della spesa personale                                              %c\n", 179, 179);
	printf("%c[4] per visualizzare lista della spesa personale                                          %c\n", 179, 179);
	printf("%c[0] per tornare indietro                                                                  %c\n", 179, 179);
	parte_inferiore_tabella();

}



/**Funzione che si occupa della visualizzazione delle voci del menu settimanale
 * @pre  Nessuna particolare pre condizione
 * @post Che siano state visualizzate le opzioni corrette per il menu
 */
void voci_menu_settimanale(){
	riga_contorno(CONTORNO_SUPERIORE);
	printf("%c                                     GESTIONE MENU SETTIMANALE                            %c\n", 186,186);
	riga_contorno(CONTORNO_INFERIORE);
	printf("%c                                                                                          %c\n", 179, 179);
	printf("%c[1] per modificare menu personale                                                         %c\n", 179, 179);
	printf("%c[2] per visualizzare menu personale                                                       %c\n", 179, 179);
	printf("%c[0] per tornare indietro                                                                  %c\n", 179, 179);
	parte_inferiore_tabella();

}



/**Funzione che si occupa della visualizzazione delle voci del menu calorie
 * @pre  Nessuna particolare pre condizione
 * @post Che siano state visualizzate le opzioni corrette per il menu
 */
void voci_calorie(){
	riga_contorno(CONTORNO_SUPERIORE);
	printf("%c                                     GESTIONE MENU CALORIE                                %c\n", 186,186);
	riga_contorno(CONTORNO_INFERIORE);
	printf("%c                                                                                          %c\n", 179, 179);
	printf("%c[1] inserire assunzione anonima                                                           %c\n", 179, 179);
	printf("%c[2] per visualizzare le proprie assunzioni                                                %c\n", 179, 179);
	printf("%c[3] per modificare le proprie assunzioni                                                  %c\n", 179, 179);
	printf("%c[4] per visualizzare istogramma assunzioni personali                                      %c\n", 179, 179);
	printf("%c[0] per tornare indietro                                                                  %c\n", 179, 179);
	parte_inferiore_tabella();

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
			voci_principale();
			break;
		case MENU_ALIMENTI:
			voci_alimenti();
			break;
		case MENU_RICETTE:
			voci_ricette();
			break;
		case MENU_UTENTI:
			voci_utenti();
			break;
		case OPZIONI_MENU_SETTIMANALE:
			voci_menu_settimanale();
			break;
		case MENU_CALORIE:
			voci_calorie();
			break;
		case MENU_LISTA_SPESA:
			voci_lista_spesa();
			break;
		case MENU_RICERCA:
			voci_ricerca();
			break;
		default:
			break;



	}

	return 1;

}


/**Funzione che si occupa di mostare le operazioni urgenti che l'utente ddeve effettuare. Nel caso in cui il valore delle notifiche sia maggiore
 * di 1 e quindi il contatore degli alimenti scaduti segnala che  un alimento o piú sono scaduti occorre chiedere se l'utente intende eliminare tali
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
 * Funzione che serve a contare tutte le tipologie di notifiche che vanno mostrate nel menú principale
 * @pre  Nessuna pre condizione particolare
 * @post Il valore restituito deve essere un intero significativo (>=0)
 *  */
int conta_notifiche (){
	int avvisi=0;
	avvisi=avvisi+conta_alimenti_scaduti();
	return avvisi;
}



/**Funzione che si occupa di far selezionare all'utente le operazioni possibili da effettuare sul menu calorie. La scelta dell'opzione
 * viene fatta attraverso una switch
 * @pre  Nessuna pre condizione particolare
 * @post Il risultato deve essere quello che l'opzione scelta si occupa
 */
int menu_calorie(){
			int scelta; //memorizza la scelta effettuata dall'utente

			visualizza_voci_menu(MENU_CALORIE);
			printf("Inserisci comando ~ ");

			do{//ripeti fino a quando...

				scelta=input_scelta(MIN_SCELTA_MENU_CALORIE,MAX_SCELTA_MENU_CALORIE);

				switch (scelta) {//in base alla scelta vai alla funzione corrispondente
				case 0://esci
				  return 1;
				  break;

				case 1:
					input_alimento_consumato();
					break;

				case 2:
					stampa_database_assunzioni();
					break;

				case 3:
					modifica_assunzione();
					break;


				case 4:
					istogrami ();
					break;


				default:
					puts("Incorrect command");
					break;

				}


				if(scelta){// se la scelta non e' quella di uscire allora chiedi di inserire un nuovo valore
					system("pause");
					visualizza_voci_menu(MENU_CALORIE);
					printf("\nSeleziona un'altra scelta del menu calorie ~ ");
				}


			}while(scelta);//l'utente non esce

			return 0;
}


/**Funzione che si occupa di far selezionare all'utente le operazioni possibili da effettuare sul menu lista della spesa. La scelta dell'opzione
 * viene fatta attraverso una switch
 * @pre  Nessuna pre condizione particolare
 * @post Il risultato deve essere quello che l'opzione scelta si occupa
 */
int menu_lista_spesa(){
			int scelta; //memorizza la scelta effettuata dall'utente

			visualizza_voci_menu(MENU_LISTA_SPESA);
			printf("Inserisci comando ~ ");

			do{//ripeti fino a quando...

				scelta=input_scelta(MIN_SCELTA_SPESA,MAX_SCELTA_SPESA);

				switch (scelta) {//in base alla scelta vai alla funzione corrispondente
				case 0://esci
				  return 1;
				  break;

				case 1:
					generatore_spesa_globale();
					break;

				case 2:
					visualizza_lista_spesa(LISTA_SPESA_GLOBALE);
					break;

				case 3:
					generatore_spesa_personale();
					break;

				case 4:
					visualizzazione_personale();
					break;

				default:
					puts("Incorrect command");
					break;

				}


				if(scelta){// se la scelta non e' quella di uscire allora chiedi di inserire un nuovo valore
					system("pause");
					visualizza_voci_menu(MENU_LISTA_SPESA);
					printf("\nSeleziona un'altra scelta del menu ricette ~ ");
				}


			}while(scelta);//l'utente non esce

			return 0;
}



/**Funzione che si occupa di far selezionare all'utente le operazioni possibili da effettuare sul menu settimanale. La scelta dell'opzione
 * viene fatta attraverso una switch
 * @pre  Nessuna pre condizione particolare
 * @post Il risultato deve essere quello che l'opzione scelta si occupa
 */
int gestore_menu_settimanale(){
			int scelta; //memorizza la scelta effettuata dall'utente
			visualizza_voci_menu(OPZIONI_MENU_SETTIMANALE);

			printf("Inserisci comando ~ ");

			do{//ripeti fino a quando...

				scelta=input_scelta(MIN_SCELTA_MENU_SET,MAX_SCELTA_MENU_SET);

				switch (scelta) {//in base alla scelta vai alla funzione corrispondente
				case 0://esci
				  return 1;
				  break;

				case 1:
					modifica_menu();
					break;

				case 2:
					gestore_visualizzazione_menu();
					break;

				default:
					puts("Incorrect command");
					break;

				}


				if(scelta){// se la scelta non e' quella di uscire allora chiedi di inserire un nuovo valore
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
int menu_utente(){
	int scelta;
	visualizza_voci_menu(MENU_UTENTI);
	printf("Inserisci comando ~ ");

	do{
		scelta=input_scelta(MIN_SCELTA_UTENTE,MAX_SCELTA_UTENTE);

		switch (scelta) {//in base alla scelta vai alla funzione corrispondente
					case 0://esci
					  break;

					case 1:
						crea_utenti();
						break;

					case 2:
						visualizza_database_utenti();

						break;

					case 3:
						gestore_modifiche();
						break;

					case 4:
						elimina_utente();
						break;

					default:
						puts("Incorrect command");
						break;
		}

		if(scelta){// se la scelta non e' quella di uscire allora chiedi di inserire un nuovo valore
			system("pause");
			visualizza_voci_menu(MENU_UTENTI);
			printf("\nSeleziona un'altra scelta del menu ~ ");
		}

	}while(scelta);
	return 1;
}





/**
 * Funzione che si occupa di passare gli alimenti presenti nel frigo e le ricette salvate nel database e di effettuare ricerche e ordinamenti su
 * tali dati estratti.
 * @pre  Nessuna pre condizione particolare
 * @post Che sia stato mostrato il giusto risultato in ase alla richiesta dell'utente
 */
int menu_ricerca(){
	int scelta;
	char nomi_ingredienti[MAX_INGRD_SUGG][LUNG_NOME_ALIMENTO];

	int num_ricette=conta_ricette_database();
	ricetta ricette_database[num_ricette];
	lettura_database_ricette(ricette_database);

	int num_alimenti;

	visualizza_voci_menu(MENU_RICERCA);
	printf("Inserisci comando ~ ");

	do{
		scelta=input_scelta(MIN_SCELTA_RICERCA,MAX_SCELTA_RICERCA);

		switch (scelta) {//in base alla scelta vai alla funzione corrispondente
					case 0://esci
					  break;

					case 1:
						ricerca_prod_magg_cons(FLAG_ALIMENTO);
						break;

					case 2:
						ricerca_prod_magg_cons(FLAG_RICETTA);
						break;

					case 3:
						ordina_ricette_kcal(ricette_database,num_ricette);
						break;
					case 4:
						suggerimento_ricetta_automatico();
						break;
					case 5:
						num_alimenti=inserimento_manuale_ingredienti(nomi_ingredienti);
						suggerimento_ricetta_manuale(num_alimenti, nomi_ingredienti);
						break;
					case 6:
						ricette_alimenti_in_scadenza();
						break;

					default:
						puts("Comando non corretto");
						break;
		}

		if(scelta){// se la scelta non e' quella di uscire allora chiedi di inserire un nuovo valore
			system("pause");
			visualizza_voci_menu(MENU_RICERCA);
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
				//CONTROLLO SULLA SCELTA ALE fallo momentaneamente su questo mini menu cosí ci alleggeriamo piú avanti
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
					modifica_ricetta();
					break;

				case 4:
					elimina_ricetta();//visualizza gli alimenti che abbiamo memorizzato nel frigo
					break;

				case 5:
					gestore_prepara_ricetta();
					break;


				default:
					puts("Incorrect command");
					break;

				}


				if(scelta){// se la scelta non e' quella di uscire allora chiedi di inserire un nuovo valore
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
			//CONTROLLO SULLA SCELTA ALE fallo momentaneamente su questo mini menu cosí ci alleggeriamo piú avanti
			scelta=input_scelta(MIN_SCELTA_ALIMENTI,MAX_SCELTA_ALIMENTI);

			switch (scelta) {//in base alla scelta vai alla funzione corrispondente
			case 0://esci
				return 1;

			case 1:
				elimina_file_spesa(LISTA_SPESA_GLOBALE);
				carica_spesa();//leggi file della spesa
				break;

			case 2:
				visualizza_frigo();//visualizza gli alimenti che abbiamo memorizzato nel frigo
				break;

			case 3:
				gestore_riduzione_alimenti();//elimina alimenti dal frigo
				break;

			case 4:
				visualizza_database_alimenti();//visualizza gli alimenti che si sono memorizzati nel database
				break;

			case 5:
				modifica_soglia_spesa();
				break;

			case 6:
				modifica_kcal();

				break;
			default:
				puts("Incorrect command");
				break;

			}


			if(scelta!=0){// se la scelta non e' quella di uscire allora chiedi di inserire un nuovo valore
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
			menu_utente();
			break;

		case 4:
			gestore_menu_settimanale();
			break;

		case 5:
			menu_calorie();
			break;

		case 6:
			menu_lista_spesa();
			break;

		case 7:
			menu_ricerca();
			break;

		case 8:
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
			printf("Seleziona un'altra scelta del menu principale  ~ ");
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


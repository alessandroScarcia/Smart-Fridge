/*
 * menu.c
 *
 *  Created on: 27 apr 2018
 *      Author: david
 */


#include "file_alimenti_operation.h"
#include "file_ricette_operation.h"


//menu che in base alla scelta effettuata(al numero premuto) richiama la funzione ad essa asscociata
int menu_alimenti(){
	  	int scelta; //memorizza la scelta effettuata dall'utente

		printf("*************************************MENU**************************\n");
		printf("Selezionare [1] per aggiungere alimenti da file\n");
		printf("Selezionare [2] riduci quantita' alimento da frigo\n");
		printf("Selezionare [3] per visualizzare il database degli alimenti memorizzati\n");
		printf("Selezionare [4] per visualizzare alimenti attualmente nel frigo\n");
		printf("Selezionare [5] per aggiungere ricette al frigo\n");
		printf("Selezionare [0] per uscire\n");


		do{//ripeti fino a quando...
			//CONTROLLO SULLA SCELTA ALE fallo momentaneamente su questo mini menu cosí ci alleggeriamo piú avanti
			scanf("%d", &scelta);

			switch (scelta) {//in base alla scelta vai alla funzione corrispondente
			case 0://esci
			  printf("Buona giornata");
			  return 1;
			  break;

			case 1:
				lettura_spesa();//leggi file della spesa
				break;

			case 2:
				riduci_q_alimenti();//elimina alimenti dal frigo
				break;

			case 3:
				visualizza_database();//visualizza gli alimenti che si sono memorizzati nel database
				break;

			case 4:
				visualizza_frigo();//visualizza gli alimenti che abbiamo memorizzato nel frigo
				break;

			case 5:
				lettura_ricette();//visualizza gli alimenti che abbiamo memorizzato nel frigo
				break;

			default:
				puts("Incorrect command");
				break;

			}


			if(scelta)// se la scelta non é quella di uscire allora chiedi di inserire un nuovo valore
			  printf("\nSeleziona un'altra scelta del menu");



		}while(scelta);//l'utente non esce

		return 0;

}

/*Ogni volta che si apre uno stream occorre verificare che esso sia effettivamente attivo. Ricorriamo ad una procedura che notifica l'utente
 * in caso di errore e ritorniamo al menu per effettuare una nuova scelta


void apertura_stream(FILE* stream){
    if(stream == NULL) {
          printf("Errore apertura file");
          menu_alimenti();
       }
} */

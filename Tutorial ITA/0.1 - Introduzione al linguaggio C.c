/*
* Lezione 0.1 - Introduzione alla programmazione in C
* Autore: Macaluso Francesco
* GitHub: https://github.com/FrancescoMaca
* 
* Premessa: Questi tutorial NON mirano a dare un corso completo sul linguaggio C. Molti aspetti teorici riguardanti
* questo linguaggio (es. strong typed, case sensitive, ecc...) non verranno spiegati. E' possibile trovare menzioni
* riguardanti queste caratteristiche.
* 
* Questo e' il programma di inizio di ogni programmatore, il comunemente chiamato "Ciao Mondo!". In questa serie di
* tutorial/file vedremo come iniziare a creare programmi base con l'aiuto di un compilatore o di un IDE.
* 
* Se non sapete cos'e' un compilatore o un IDE potete dare un occhiata ai link sottostanti:
* Compilatore - https://it.wikipedia.org/wiki/Compilatore
* IDE - https://it.wikipedia.org/wiki/Integrated_development_environment
* 
* Il primo programma che scriveremo sara' molto breve ed introdurra' alcune basi sulla sintassi da utilizzare nei 
* programmi in C.
* 
* Qui sotto possiamo vedere un pezzo di codice che ci permette di includere una libreria. Cosa significa ti starai
* chiedendo. Bene, in C esistono dei file, scritti da persone molto piu brave di me, che ci aiutano a interfacciarsi
* con varie funzionalita' che questo linguaggio ci offre (come ad esempio la stampa a video e moltre altre cose).
* Quando includiamo una libreria significa che il nostro codice utilizzera' delle funzionalita' "esterne" che fanno
* parte dello standard C.
* 
* Nel nostro caso quello che facciamo e' utilizzare il "comando" #include per includere la libreria indicata con le
* parentesi angolari '<stdio.h>'. Stiamo includendo, quindi, un file chiamato "stdio" con estensione .h (estensione
* comune per le librerie di C). Questo file ci permette di interfacciarsi con la console quando compiliamo il codice.
* Ovvero ci offre le funzionalita' per scrivere a schermo del testo e, come andremo a vedere nei prossimi tutorial, 
* anche a leggerlo.
* 
*/
#include <stdio.h>

/*
* Questo e' chiamato "punto d'entrata" (in inglese Entry Point) ed e' l'inizio del nostro programma.
* Possiamo ignorare il perche' scriviamo "int main()...", per ora prendiamolo per buono. Nota bene che il nostro codice
* sta venendo scritto tra le parentesi graffe!
*/
int main()
{
	/*
	* Questa funzione fa parte della libreria stdio.h che abbiamo incluso prima, il suo funzionamento e' molto base,
	* ci permette di stampare la frase a schermo.
	* 
	* La sua sintassi e' un po particolare, si scrive il nome della funzione seguita dall'apertura e chiusura delle
	* parentesi tonde. Tra le parentesi tonde si deve inserire la frase che si vuole stampare a schermo compresa tra
	* i doppi apici (" ").
	* La sequenza di caratteri '\n' e' considerata un carattere speciale, serve per andare alla nuova linea.
	*/
	printf("Ciao mondo!\n");
}
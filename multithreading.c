#include <stdio.h>   // libraria standarde për input dhe output
#include <stdlib.h>  // libraria standarde për funksione ndihmëse si konvertimet, menaxhimi i memories, etj.
#include <unistd.h>  // libraria për funksione POSIX dhe manipulim të proceseve
#include <sys/types.h>  // libraria për definime të tipave primitivë të përdorur nga system call-et
#include <pthread.h>  // libraria për përdorimin e Pthreads (POSIX threads) për multithreading
#include <errno.h>   // libraria për trajtimin e gabimeve
#include <string.h>  // libraria për manipulimin e stringjeve

// Makro për trajtimin e gabimeve. Në rast se një funksion kthen një gabim, do të shfaqë mesazhin e gabimit dhe do të dalë nga programi
#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

// Deklarimet globale
int c = 0; // Variabla globale c e cila na sherben per numerim
pthread_t thread_2_id; // Identifikuesi i thread-it te dyte

// Prototipat e funksioneve
void* thread_i_pare(void* p);
void* thread_i_dyte(void* p);

int main(int argc, char *argv[]) {
    int th;
    pthread_t t1, t2; // Deklarimi i thread-ave
    pthread_attr_t attr; // Deklaron atributet (madhesine e stack-ut dhe scheduling informacionet) e thread 

    // Kontrollimi i argumenteve
    if (argc != 3) {
        fprintf(stderr, "Keni shkruar komanden gabim!\nSintaksa: %s <numri i shkronjave per fjalen e gjeneruar> <kufiza per numrin e gjeneruar>\n", argv[0]);
        return -1;
    }

    // Kontrollimi i vlefshmërisë së argumenteve
    if (atoi(argv[1]) < 0 || atoi(argv[2]) < 0) {
        fprintf(stderr, "%d ose %d duhet te jete >= 0\n", atoi(argv[1]), atoi(argv[2]));
        return -1;
    }

    // Inicimi i atributeve të thread-it
    th = pthread_attr_init(&attr);
    if (th != 0)
        handle_error_en(th, "pthread_attr_init");

    // Krijimi i thread-ave
    th = pthread_create(&t1, &attr, thread_i_pare, argv[1]);
    if (th != 0)
        handle_error_en(th, "pthread_create t1");

    th = pthread_create(&t2, &attr, thread_i_dyte, argv[2]);
    if (th != 0)
        handle_error_en(th, "pthread_create t2");

    // Shkatërrimi i atributeve të thread-it
    th = pthread_attr_destroy(&attr);
    if (th != 0)
        handle_error_en(th, "pthread_attr_destroy");

    // Pritja për përfundimin e thread-ave
    th = pthread_join(t1, NULL);
    if (th != 0)
        handle_error_en(th, "pthread_join t1");

    th = pthread_join(t2, NULL);
    if (th != 0)
        handle_error_en(th, "pthread_join t2");

    printf("\nPerfundimi i programit\n");
    return 0;
}

// Funksioni për thread-in e parë
void* thread_i_pare(void* p) {
    while (1) {
        printf("\nFillo gjenerimin e fjales...\n");

        int n = atoi((char*)p); // Konvertimi i stringut në integer
        static const char alfabeti[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"; // Deklarimi i nje char array me shkronja te alfabetit
        char res[n + 1]; // Variabla ku shtohen shkronjat te marra ne menyre te rastesishme per te krijuar fjalen 

        if (n > 0) { // Nese numri i dhene i shkronjave ne terminal eshte me i madh se 0, fillon gjenerimi i fjales
            for (int i = 0; i < n; i++)
                res[i] = alfabeti[rand() % (sizeof(alfabeti) - 1)]; // Zgjedh shkronjat ne menyre te rastesishme
            res[n] = '\0'; // Shto karakterin e përfundimit të stringut
        }

        printf("Fjala e gjeneruar: %s\n", res);
        sleep(1); // Prit 1 sekonde
        c++;

        if (c == 5) { // Pasi c te behet 5 kerkohet anulimi i thread-it te dyte dhe del nga thread-i i tanishem
            pthread_cancel(thread_2_id);
            pthread_exit(NULL);
        }
    }
}

// Funksioni për thread-in e dytë
void* thread_i_dyte(void* p) {
    thread_2_id = pthread_self(); // Ruaje ID e thread-it te dyte

    while (1) {
        printf("\nFillo gjenerimin e numrit...\n");
        int m = atoi((char*)p); // Konvertimi i stringut ne integer
        printf("Numri i gjeneruar: %d\n", rand() % m); // Gjenerimi i numrit te rastesishem
        sleep(2); // Prit 2 sekonda
    }
}

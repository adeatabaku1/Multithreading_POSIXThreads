#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>  //libraria per pthread
#include <errno.h>   //libraria per errore

#define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)  //definimi i error-it

int c = 0;  //variabla c e cila me poshte na sherben per numerim
pthread_t thread_2_id;   //identifikimi i thread te dyte

void* thread_i_pare(void* p);   
void* thread_i_dyte(void* p);   //thirrja e funksioneve

int main(int args, char *arg[]) {
   
   int th;
   pthread_t t1, t2;      // deklarimi i thread-ave
   pthread_attr_t attr;   ///deklaron atributet (madhesine e stack-ut dhe scheduling informacionet) e thread 
   
   if (args != 3){    //ne kete pjese kontrollojme numrin e argumenteve, perkatesisht nese numri i argumenteve te dhena nuk eshte 3, 
				//atehere na paraqitet nje error me mesazh
       fprintf(stderr,"Keni shkruar komanden gabim!\nSintaksa: a.out <numri i shkronjave per fjalen e gjeneruar> <kufiza per numrin e gjeneruar>\n");
       return -1;      
    }

   if (atoi(arg[1]) < 0||atoi(arg[2])<0){  //ne kete pjese kontrollojme a jane argumentet e dhena pozitive apo jo,
   						// nese njeri argument apo te dy argumentet jane negative, na paraqitet nje error   
        fprintf(stderr,"%d ose %d duhet te jete >= 0\n", atoi(arg[1]),atoi(arg[2]));
         return -1;
    } 
    
   th=pthread_attr_init(&attr);   // marrja e atributeve te thread-it
   
   if (th != 0)
        handle_error_en(th, "pthread_attr_init");  //nese funksioni i mesiperm nuk funksionon, atehere paraqitet nje error
        
   //krijimi i dy thread-ave 
   th=pthread_create(&t1, &attr, thread_i_pare, arg[1]);
   th=pthread_create(&t2, &attr, thread_i_dyte, arg[2]);
   
   if (th != 0)
        handle_error_en(th, "pthread_create");  //nese funksioni i mesiperm nuk funksionon, atehere paraqitet nje error
   th=pthread_attr_destroy(&attr);  //shkatërrimi i objektit të atributeve të thread-ave
   if (th != 0)
        handle_error_en(th, "pthread_attr_destroy");  //nese funksioni i mesiperm nuk funksionon, atehere paraqitet nje error
               
   //prit derisa te kompletohen thread-at
   th=pthread_join(t1, NULL);
   th=pthread_join(t2, NULL);
   
   if (th != 0)
        handle_error_en(th, "pthread_join"); //nese funksioni i mesiperm nuk funksionon, atehere paraqitet nje error
   
   printf("\nPerfundimi i programit\n");
}

void* thread_i_pare(void* p) {

   while (1) {
      
      printf("\nFillo gjenerimin e fjales...\n"); 
      
      int i, n = atoi(p);  //konvertimi i stringut ne integer
      static const char alfabeti[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";  //deklarimi i nje char array me shkronja te alfabetit
      char res[n]; // variabla ku shtohen shkronjat te marra ne menyre te rastesishme per te krijuar fjalen 
      
      if(n>0){ //nese numri i dhene i shkronjave ne terminal eshte me i madh se 0, fillon gjenerimi i fjales
      
      for (int i = 0; i < n; i++)  
        res[i] = alfabeti[rand() % (sizeof(alfabeti) - 1)];  //zgjedh shkronjat ne menyre te rastesishme
      
      }
      
      printf("Fjala e gjeneruar: %s\n",res);
      sleep(1); // prit 1 sekonde
      c++;
      
      if (c == 5) {
         //pasi c te behet 5 kerkohet anulimi i thread-it te dyte dhe del nga thread-i i tanishem
         pthread_cancel(thread_2_id);
         pthread_exit(NULL);
      
      }
   }
}

void* thread_i_dyte(void* p) {

   thread_2_id = pthread_self(); //ruaje id e thread-it te dyte
   
   while (1) {
   
      printf("\nFillo gjenerimin e numrit...\n"); 
      int m = atoi(p);  //konvertimi i stringut ne integer
      printf("Numri i gjeneruar: %d\n", rand() % m); // gjenerimi i numrit te rastesishem
      sleep(2); // prit 2 sekonda
   } 
}
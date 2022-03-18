#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "hello.h"

#define NB_STUDENTS 200
#define NB_MARKS  2 
    
int static DB[NB_STUDENTS][NB_MARKS] ;

void fillDatabase( ) {
    srand(time(NULL));
   for (int i = 0; i < NB_STUDENTS; ++i) {
      for(int j = 0; j < NB_MARKS ; j++) {
          int random_value = rand() % 20 ;
          DB[i][j] = random_value ;
      }

   }
}

void showDatabase() {
   for (int i = 0; i < NB_STUDENTS; ++i) {
      for(int j = 0; j < NB_MARKS ; j++) {
          printf("yes -> %d\n", DB[i][j]) ;
      }

   }	
}

void * computePartialAverage(float averages[4], int avg_ind, int start_index, int end_index) {
    printf("CPA \n") ;
    double avg ,
        sum = 0 ;
    for( int i = start_index; i <= end_index; i++ ) {
        double std_avg, std_sum = 0 ; 
        for(int j = 0; j < NB_MARKS ; j++) {
          std_sum += DB[i][j] ;
        }
        std_avg = std_sum / NB_MARKS ;
        sum += std_avg ;
    }
    int nb_students = end_index - start_index + 1 ;
    avg = sum / nb_students ;
    printf("CPA (%d) ::: sum: %f ---  nb_students:%d --  NB_MARKS: %d -- AVG: %f \n", avg_ind, sum, nb_students, NB_MARKS, avg);
    averages[avg_ind] = avg;
    printf("end CPA \n") ;
    
}

float computeAverage() {
	pthread_t tid;
	float averages[4] = {-1, -1 , -1, -1} ;
	double final_avg, final_sum = 0 ;
    printf("CP \n");
	for(int i=0; i<4; i++) {
	    int start = 50 * i ;
	    int end = (50 * (i + 1) ) - 1 ;
    	pthread_create(&tid, NULL, computePartialAverage(averages, i, start, end), NULL);
        //printf("PARTIAL AVG : %f \n", averages[i]) ;
    }
    printf("AFTER CP \n");    
    //pthread_join(tid, NULL) ;
    
    for(int i=0; i<4; i++) {
        //printf("PARTIAL AVG : %f \n", averages[i]) ;
        final_sum += averages[i] ;
    }
    final_avg = final_sum / 4 ;

    return final_avg;
}

int main () {

   fillDatabase();
   //showDatabase();
   float avg = computeAverage() ;
   printf("\n\n MOYENNE FINALE : %f\n", avg) ;
   //Hello();
   return 0;
}
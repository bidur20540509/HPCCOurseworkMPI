
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <crypt.h>
#include <mpi.h>

/*****************************************************************************
The variable names and the function names of this program is same as provided by the university.
The added variable and function are the only changes made to this program.
  

To compile:
     mpicc -o passwordcrackingmpiV2 passwordcrackingmpiV2.c -lcrypt
     
  To run 3 processes on this computer:
    mpirun -n 3 ./mr.py ./passwordcrackingmpiV2 | grep Time
*****************************************************************************/


int n_passwords = 4;
pthread_t t1, t2;
char *encrypted_passwords[] = {
"$6$KB$yR3OFAgf953GBOELX1REHrZJRYUOLgKLMuKTykAy442DAuoZNWKLrFY0FpHG862h/qEz5dX7hwUVdks29JJ0a/",

"$6$KB$yR3OFAgf953GBOELX1REHrZJRYUOLgKLMuKTykAy442DAuoZNWKLrFY0FpHG862h/qEz5dX7hwUVdks29JJ0a/",

"$6$KB$.QKmXKFTvqroDVMEyAyot92xc3USQHzpY4OjSGjCrPV9Cja2wxAi7EaF5mc8diclww7a7Rb6qNaUSgJaeWCrk.",

"$6$KB$JbOcg/X221O8DwMaRCWRMO4G5cbGhu.qaApt7vZGjgELOVS2fyoQrvCK4Pej7WZO6hvSnYOVZKCLmLc7qoKgO/"
};

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}
  
void kernel_function1(char *salt_and_encrypted){
  int a, b, digit1,digit2;     
  char salt[7];    

  char plain[7];   
  char *enc;       
  int count = 0;   

  substr(salt, salt_and_encrypted, 0, 6);
  
  for(a='A'; a<='M';a++){
    for(b='A'; b<='Z'; b++){
      for(digit1=0; digit1<=99; digit1++){
	for(digit2=0; digit2<=99; digit2++){
	//printf("Instance 1:");
	sprintf(plain, "%c%c%02d%02d",a, b, digit1,digit2);
	enc = (char *) crypt(plain, salt);
	count++;
	if(strcmp(salt_and_encrypted, enc) == 0){
	  printf("#%-8d%s %s\n", count, plain, enc);
	}// else {
	  //printf(" %-8d%s %s\n", count, plain, enc);
	//}
	}
      }
    }
  }
  printf("%d solutions explored\n", count);
}
void kernel_function2(char *salt_and_encrypted){
  int a, b, digit1,digit2;     
  char salt[7];    

  char plain[7];   
  char *enc;       
  int count = 0;  

  substr(salt, salt_and_encrypted, 0, 6);
  
  for(a='N'; a<='Z'; a++){
    for(b='A'; b<='Z'; b++){
      for(digit1=0; digit1<=99; digit1++){
	for(digit2=0; digit2<=99; digit2++){
		//printf("Instance 2:");
		sprintf(plain, "%c%c%02d%02d",a, b, digit1,digit2);
		enc = (char *) crypt(plain, salt);
		count++;
		if(strcmp(salt_and_encrypted, enc) == 0){
	 	  printf("#%-8d%s %s\n", count, plain, enc);
	} //else {
	  //printf(" %-8d%s %s\n", count, plain, enc);
	 //}
	}
      }
    }
  }
  printf("%d solutions explored\n", count);
}


int time_difference(struct timespec *start, struct timespec *finish,
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

int main(int argc, char** argv) {
 struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);

 
  int size, rank;
int i;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(size != 3) {
    if(rank == 0) {
      printf("This program needs to run on exactly 3 processes\n");
    }
  } else {
    if(rank ==0){

      int x;
    
      MPI_Send(&x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      MPI_Send(&x, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);


 
    } else if(rank==1){
      int number;
      MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, 
                         MPI_STATUS_IGNORE);
        for(i=0;i<n_passwords;i<i++) {
          kernel_function1(encrypted_passwords[i]);
        }
      }
      else{
      int number;
      MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, 
                         MPI_STATUS_IGNORE);
        for(i=0;i<n_passwords;i<i++) {
          kernel_function2(encrypted_passwords[i]);
        }
      }
    }
    MPI_Finalize(); 
 clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
         (time_elapsed/1.0e9)); 

  return 0;
}

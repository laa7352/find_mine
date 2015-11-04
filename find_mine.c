// Fine A mine in c
#include <stdio.h>
#include <time.h>
#include <string.h>

void random(void){
  srand(time(NULL));
}


int main(void){
  int i,j,k;
  int ix=10,iy=10;
  int imine=10;
  int MineMap [ix*iy];
  int MineList [imine];
 // memset(MineMap, 0, ix*iy*sizeof(int));

// Create random seed
  random();

// Initial MineMap
  for (i=0; i<ix*iy; i++){
    MineMap[i]=i;
  }

//  printf("test\nMineMap: ");
//  for (k=0; k<ix*iy ; k++){
//    printf("%d,", MineMap[k]);
//  }
//  printf("\n");
 
// Random create MineList
  for (i=0; i<imine ; i++){
    int irand=rand()%(ix*iy-i);
    int temp=MineMap[irand];
    MineList[i]=temp;
//    printf("Random number 1-400: %d\n",irand);
//    printf("Random x, y: %d, %d\n", temp/ix, temp%iy);
//    printf("Random ix*iy-1: %d, (ix*iy-1)-i: %d, temp: %d, irand: %d\n" , ix*iy-i, (ix*iy-1)-i, temp, irand);
    for(j=irand; j<(ix*iy-1)-i; j++){
       MineMap[j]=MineMap[j+1];
    }
    MineMap[ix*iy-1-i]=temp;
//    printf("test\nMineMap: ");
//    for (k=0; k<ix*iy ; k++){
//      printf("%d,", MineMap[k]);
//    }
//    printf("\n\n");
  }

// Initial MineMap with 0
  for (i=0; i<ix*iy; i++){
    MineMap[i]=0;
  }
  for (i=0; i<imine; i++){
    MineMap[MineList[i]]=1;
  }


//  printf("test\nMineMap: ");
//  for (k=0; k<ix*iy ; k++){
//    printf("%d,", MineMap[k]);
//  }
//  printf("\n");


// Print MineMap


  printf(" \\ ");
  for(i=0; i<ix; i++){
    printf(" -");
  }
  printf("  / ");
  printf("\n");

  for(j=0; j<iy; j++){
    printf(" | ");
    for(i=0; i<ix; i++){
      if(MineMap[j*10+i] == 1){
        printf(" B");
      }else{
        printf(" #");
      }
    }
    printf("  | ");
    printf("\n");
  }

  printf(" / ");
  for(i=0; i<ix; i++){
    printf(" -");
  }
  printf("  \\ ");
  printf("\n");
  
  return 0;
}

// Fine A mine in c
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <curses.h>
#define MineFlag 0x0001
#define CleanFlag 0x0002
#define MarkFlag 0x0003


void random(void){
  srand(time(NULL));
}

#if 0
void printMap(int * MM, int ix, int iy){
  int i,j;
 
  printf(" \\ ");
  for(i=0; i<ix; i++){
    printf(" -");
  }
  printf("  / ");
  printf("\n");

  for(j=0; j<iy; j++){
    printf(" | ");
    for(i=0; i<ix; i++){
      if(MM[j*iy+i]&MineFlag ){
        printf(" B");
      }else{
        printf("  ");
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

}
#endif

void CreateMineMap(int * MineMap, int MapSize, int * MineList, int MineN){
  int i,j;
  //int sum=0;

// Create random seed
  random();

// Initial MineMap
  for (i=0; i<MapSize; i++){
    MineMap[i]=i;
  }

// Random create MineList
  for (i=0; i<MineN ; i++){
    int irand=rand()%(MapSize-i);
    int temp=MineMap[irand];
    MineList[i]=temp;
    for(j=irand; j<(MapSize-1)-i; j++){
       MineMap[j]=MineMap[j+1];
    }
    MineMap[MapSize-1-i]=temp;
  }

// Initial MineMap with 0, Mine set as MineFlag
  for (i=0; i<MapSize; i++){
    MineMap[i]=0;
  }
  for (i=0; i<MineN; i++){
    MineMap[MineList[i]]=MineMap[MineList[i]]|MineFlag;
//    printf("x: %d, y: %d, %d\n", MineList[i]%20, MineList[i]/20, MineMap[MineList[i]]);
 //   sum++;

  }
//  printf("sum: %d\n",sum);
 // getch();
}

void init_window(){
  initscr(); //  ←┐
  cbreak;    //    │ 啟動 curses 模式
  noecho();  //    │
  nonl();    //  ←┘
  refresh();
}

void Create_MineWin(WINDOW * MineWin, int iy, int ix, int * MineMap){
  int i,j,mx,my;

  MineWin=newwin(iy+2, 2*(ix+1)+1, 0, 0);
  box(MineWin, '|', '-');
  touchwin(MineWin);
  wrefresh(MineWin);
#if 1
  for(j=0; j<iy; j++){
    //sleep(1);
//    printf(" | ");
    for(i=0; i<ix; i++){
      my=j+1;
      mx=2*(i+1);
      wmove(MineWin, my, mx);
      if(*(MineMap+j*iy+i)&MineFlag ){
        waddch(MineWin, 'B');
      }else{
        waddch(MineWin, '#');
      //  printf("  ");
      }
    }
 //   printf("  | ");
  //  printf("\n");
  }
#endif
  wrefresh(MineWin);
}

int main(void){
  int ix=20,iy=20;
  int imine=10;
  int MineMap [ix*iy];
  int MineList [imine];
  WINDOW *MineWin;

 // memset(MineMap, 0, ix*iy*sizeof(int));
  CreateMineMap(MineMap, ix*iy, MineList, imine);

// Print MineMap
  init_window();
  Create_MineWin(MineWin, iy, ix, MineMap);
#if 0
  MineWin=newwin(iy+2, 2*(ix+2)+1, 0, 0);
  box(MineWin, '|', '-');
  touchwin(MineWin);
  wrefresh(MineWin);
  getch();
#endif

  getch();
  endwin();

//  printMap(MineMap, ix, iy);
  return 0;
}

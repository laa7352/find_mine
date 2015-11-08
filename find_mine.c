// Fine A mine in c
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <curses.h>
#define MineFlag  0x00000001  // ... 0000 0001  unsigned int flag
#define CleanFlag 0x00000002  // ... 0000 0010
#define MarkFlag  0x00000004  // ... 0000 0100


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

void CreateMineMap(unsigned int * MineMap, int MapSize, int * MineList, int MineN){
  int i,j;

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
  for (i=0; i<MapSize; i++) MineMap[i]=0;
  for (i=0; i<MineN; i++) MineMap[MineList[i]]=MineMap[MineList[i]]|MineFlag;

}

void init_window(){
  initscr(); //  ←┐
  cbreak;    //    │ 啟動 curses 模式
//  noecho();  //    │
  nonl();    //  ←┘
  intrflush(stdscr,FALSE);
  keypad(stdscr,TRUE);
  refresh();
}

void Create_MineWin(WINDOW ** MineWin, int iy, int ix, unsigned int * MineMap, int inity, int initx){
  int i,j,mx,my;

  *MineWin=newwin(iy+2, 2*(ix+1)+1, inity, initx);
  box(*MineWin, '|', '-');
  touchwin(*MineWin);
  wrefresh(*MineWin);
  for(j=0; j<iy; j++){
    for(i=0; i<ix; i++){
      my=j+1;
      mx=2*(i+1);
      wmove(*MineWin, my, mx);
      waddch(*MineWin, '#');
    }
  }
//  wrefresh(*MineWin);
}

void Check_MineWin(WINDOW * MineWin, int iy, int ix, unsigned int * MineMap, int cy, int cx){
  int i,j,cm=0;

  touchwin(MineWin);
  wrefresh(MineWin);
  if(!(*(MineMap+cy*iy+cx)&MarkFlag) ){
    if(*(MineMap+cy*iy+cx)&MineFlag ){
      *(MineMap+cy*iy+cx)|=CleanFlag;
      wmove(MineWin, (cy+1), 2*(cx+1));
      waddch(MineWin, 'B');
    }else{
      wmove(MineWin, (cy+1), 2*(cx+1));
      *(MineMap+cy*iy+cx)|=CleanFlag;
      for(i=(cx>0?cx-1:0);i<=(cx<ix-1?cx+1:ix-1);i++){
      for(j=(cy>0?cy-1:0);j<=(cy<iy-1?cy+1:iy-1);j++){
        if(*(MineMap+j*iy+i)&MineFlag ) cm++;
      }}
      if(cm >0 ){
        wmove(MineWin, (cy+1), 2*(cx+1));
        waddch(MineWin, cm+48);  // In ACSII table, number 0 is '48'.
      }else{
        waddch(MineWin, ' ');
        for(i=(cx>0?cx-1:0);i<=(cx<ix-1?cx+1:ix-1);i++){
        for(j=(cy>0?cy-1:0);j<=(cy<iy-1?cy+1:iy-1);j++){
          if(!(*(MineMap+j*iy+i)&CleanFlag)) Check_MineWin(MineWin, iy, ix, MineMap, j, i);
        }}
      }
    }
  }
}


void Mark_MineWin(WINDOW * MineWin, int iy, int ix, unsigned int * MineMap, int my, int mx){

  touchwin(MineWin);
  wrefresh(MineWin);

  if(!(*(MineMap+my*iy+mx)&MarkFlag) & !(*(MineMap+my*iy+mx)&CleanFlag) ){
    *(MineMap+my*iy+mx)|=MarkFlag;
    wmove(MineWin, (my+1), 2*(mx+1));
    waddch(MineWin, '?');
  }else if(!(*(MineMap+my*iy+mx)&CleanFlag)){
    *(MineMap+my*iy+mx)&= (~MarkFlag);
    wmove(MineWin, (my+1), 2*(mx+1));
    waddch(MineWin, '#');
  }

}

void Move_MineWin(WINDOW * MineWin, int iy, int ix, unsigned int * MineMap){
  int mx=ix/2,my=iy/2,ch,tx=ix/2,ty=iy/2;
  int action=1;

  touchwin(MineWin);
  wrefresh(MineWin);
  while(action){

    wmove(MineWin, (ty+1), 2*(tx+1)-1);
    waddch(MineWin, ' ');
    wmove(MineWin, (ty+1), 2*(tx+1)+1);
    waddch(MineWin, ' ');
    wmove(MineWin, (my+1), 2*(mx+1)-1);
    waddch(MineWin, '<');
    wmove(MineWin, (my+1), 2*(mx+1)+1);
    waddch(MineWin, '>');
    ty=my;
    tx=mx;
    wmove(MineWin, (my+1), 2*(mx+1));
    wrefresh(MineWin);

    ch=getch();
    switch (ch){
      case KEY_UP:
        if( my > 0 )my--;
        break;
      case KEY_DOWN:
        if( my < iy-1 )my++;
        break;
      case KEY_LEFT:
        if( mx > 0 )mx--;
        break;
      case KEY_RIGHT:
        if( mx < ix-1 )mx++;
        break;
      case 32:  // space
        Check_MineWin(MineWin, iy, ix, MineMap, my, mx);
/*        if(*(MineMap+my*iy+mx)&MineFlag ){
          waddch(MineWin, 'B');
        }else{
          waddch(MineWin, ' ');
        }*/
        break;
      case 'f': // Mark flag
        Mark_MineWin(MineWin, iy, ix, MineMap, my, mx);
        break;
      case 'q':
        action=0;
        break;
      default:
        break;
    }
  }
}

int Action_MenuWin(int * level, WINDOW ** MenuWin){
  int mx=3,my=2;
  int action=1;
  int ch;

  *MenuWin=newwin(6, 40, LINES/2-3, COLS/2-20);
  box(*MenuWin, '|', '-');
  mvwaddstr(*MenuWin,1,1,"Please choose the following mode.");
  mvwprintw(*MenuWin,2,1," <*> : %dx%d grids, %d mines", *level, *(level+1), *(level+2));
  mvwprintw(*MenuWin,3,1,"  *  : %dx%d grids, %d mines", *(level+3), *(level+3+1), *(level+3+2));
  mvwprintw(*MenuWin,4,1,"  *  : %dx%d grids, %d mines", *(level+6), *(level+6+1), *(level+6+2));
  wmove(*MenuWin, my, mx);
  touchwin(*MenuWin);
  wrefresh(*MenuWin);

  while(action){
    ch=getch();
    switch (ch){
      case KEY_UP:
        mvwaddstr(*MenuWin,my,1,"  * ");
        my=(my>2?my-1:4);
        mvwaddstr(*MenuWin,my,1," <*>");
//        my--;

        wmove(*MenuWin, my, mx);
    //    if( my > 0 )my--;
        break;
      case KEY_DOWN:
        mvwaddstr(*MenuWin,my,1,"  * ");
        my=((my<4)?my+1:2);
        mvwaddstr(*MenuWin,my,1," <*>");
        //my++;

        wmove(*MenuWin, my, mx);
     //   if( my < iy-1 )my++;
        break;
      case 32:  // space
//        Check_MineWin(MineWin, iy, ix, MineMap, my, mx);
/*        if(*(MineMap+my*iy+mx)&MineFlag ){
          waddch(MineWin, 'B');
        }else{
          waddch(MineWin, ' ');
        }*/
        break;
      case '\r':
        return my-2;
        break;
      case 'q':
        action=0;
        break;
      default:
        break;
    }
    touchwin(*MenuWin);
    wrefresh(*MenuWin);
  }
/*  for(j=0; j<iy; j++){
    for(i=0; i<ix; i++){
      my=j+1;
      mx=2*(i+1);
      wmove(*MineWin, my, mx);
      waddch(*MineWin, '#');
    }
  }*/
  touchwin(stdscr);
  return 1;
}

int main(void){
  int ix=16,iy=15;
  int imine=10;
  int level[3][3] ={{10, 10, 10},{15, 15, 40},{30, 15, 100}};
  unsigned int MineMap [ix*iy];
  int MineList [imine], ilevel;
  WINDOW *MineWin, *MenuWin;


//     win[0]=newwin(LINES/2-1,COLS-1,0,0);       /* 設定兩個視窗的大小*/
//     win[1]=newwin(LINES/2-1,COLS-1,LINES/2,0);

 // memset(MineMap, 0, ix*iy*sizeof(int));

// Print MineMap
  init_window();

//  ilevel=Action_MenuWin(level, &MenuWin);
 // printf("ilevel: %d, ix: %d, iy: %d, imine: %d\n",ilevel, level[ilevel][0], level[ilevel][1], level[ilevel][2]);

  refresh();
#if 0
  CreateMineMap(MineMap, level[ilevel][1]*level[ilevel][0], MineList, level[ilevel][2]);
  Create_MineWin(&MineWin, level[ilevel][1], level[ilevel][0], MineMap, 1, 1);
  Move_MineWin(MineWin, level[ilevel][1], level[ilevel][0], MineMap); 
#else

  CreateMineMap(MineMap, ix*iy, MineList, imine);
  Create_MineWin(&MineWin, iy, ix, MineMap, 1, 1);
  Move_MineWin(MineWin, iy, ix, MineMap); 
#endif

  endwin();

//  printMap(MineMap, ix, iy);
  return 0;
}

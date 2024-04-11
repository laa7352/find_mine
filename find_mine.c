// Fine A mine in c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <curses.h>
#define MineFlag     0x00000001  // ... 0000 0001  unsigned int flag
#define CleanFlag    0x00000002  // ... 0000 0010
#define MarkFlag     0x00000004  // ... 0000 0100
#define QuestionFlag 0x00000008  // ... 0000 1000
#define Quit_Or_Nothing 0
#define Mine_Opened     1
#define All_Clean       2

#define DEBUG 0

void init_random(void){
  srand(time(NULL));
}

#if DEBUG
void printMap(unsigned int * MM, int ix, int iy){
  int i,j;

  printf(" ┌");
  for(i=0; i<ix; i++){
    printf("--");
  }
  printf("-┐ ");
  printf("\n");

  for(j=0; j<iy; j++){
    printf(" |");
    for(i=0; i<ix; i++){
      if(MM[j*ix+i]&MineFlag ){
        printf(" B");
      }else{
        printf("  ");
      }
    }
    printf(" |");
    printf("\n");
  }

  printf(" └");
  for(i=0; i<ix; i++){
    printf("--");
  }
  printf("-┘");
  printf("\n");

}
#endif

void CreateMineMap(unsigned int * MineMap, int MapSize, int * MineList, int MineN){
  int i,j;

// Create random seed
  init_random();

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
  cbreak;    //   │ 啟動 curses 模式
  noecho();  //   │
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
#if DEBUG
      if(*(MineMap+j*ix+i)&MineFlag)
        waddch(*MineWin, 'B');
      else
#endif
        waddch(*MineWin, '#');
    }
  }
//  wrefresh(*MineWin);
}

int Check_MineWin(WINDOW * MineWin, int iy, int ix, unsigned int * MineMap, int cy, int cx, int CheckMarkOrQuestionFlag){
  int i,j,cm=0;
  int count_mine_and_clean = 0;
  int count_clean = 0;

  touchwin(MineWin);
//  wrefresh(MineWin);
  wmove(MineWin, (cy+1), 2*(cx+1));
  if((!(*(MineMap+cy*ix+cx)&MarkFlag) && !(*(MineMap+cy*ix+cx)&QuestionFlag))
      || !CheckMarkOrQuestionFlag){
    if(*(MineMap+cy*ix+cx)&MineFlag){
      *(MineMap+cy*ix+cx)|=CleanFlag;
 //     wmove(MineWin, (cy+1), 2*(cx+1));
      waddch(MineWin, 'B');
      return Mine_Opened;
    }else{
  //    wmove(MineWin, (cy+1), 2*(cx+1));
      *(MineMap+cy*ix+cx)|=CleanFlag;
      for(i=(cx>0?cx-1:0);i<=(cx<ix-1?cx+1:ix-1);i++){
      for(j=(cy>0?cy-1:0);j<=(cy<iy-1?cy+1:iy-1);j++){
        if(*(MineMap+j*ix+i)&MineFlag ) cm++;
      }}
      if(cm >0 ){
   //     wmove(MineWin, (cy+1), 2*(cx+1));
        waddch(MineWin, cm+48);  // In ACSII table, number 0 is '48'.
      }else{
        waddch(MineWin, ' ');
        for(i=(cx>0?cx-1:0);i<=(cx<ix-1?cx+1:ix-1);i++){
        for(j=(cy>0?cy-1:0);j<=(cy<iy-1?cy+1:iy-1);j++){
          if(!(*(MineMap+j*ix+i)&CleanFlag)) Check_MineWin(MineWin, iy, ix, MineMap, j, i, 0);
        }}
      }
    }
  }
  if (CheckMarkOrQuestionFlag) {
    /*
       check all map is clear or not
       1. count MineFlag and !CleanFlag
       2. count all !CleanFlag
       3. if 1 == 2, the game is passed
     */
    for(j=0; j<iy; j++){
    for(i=0; i<ix; i++){
      if(!(*(MineMap+j*ix+i)&CleanFlag)) {
        count_clean++;
        if(*(MineMap+j*ix+i)&MineFlag){
          count_mine_and_clean++;
        }
      }
    }}
    if (count_clean == count_mine_and_clean) {
      return All_Clean;
    }
    //mvprintw(1,1," count_clean: %d, count_mine_and_clean: %d", count_clean, count_mine_and_clean);
  }

  return Quit_Or_Nothing;
}


void Mark_MineWin(WINDOW * MineWin, int iy, int ix, unsigned int * MineMap, int my, int mx){

  touchwin(MineWin);
  wrefresh(MineWin);

  if(!(*(MineMap+my*ix+mx) & CleanFlag)) {
    if(!(*(MineMap+my*ix+mx) & MarkFlag) && !(*(MineMap+my*ix+mx) & QuestionFlag)) {
      *(MineMap+my*ix+mx) |= MarkFlag;
      wmove(MineWin, (my+1), 2*(mx+1));
      waddch(MineWin, 'M');
    } else if ((*(MineMap+my*ix+mx) & MarkFlag) && !(*(MineMap+my*ix+mx) & QuestionFlag)) {
      *(MineMap+my*ix+mx) &= (~MarkFlag);
      *(MineMap+my*ix+mx) |= QuestionFlag;
      wmove(MineWin, (my+1), 2*(mx+1));
      waddch(MineWin, '?');
    } else {
      *(MineMap+my*ix+mx) &= (~MarkFlag);
      *(MineMap+my*ix+mx) &= (~QuestionFlag);
      wmove(MineWin, (my+1), 2*(mx+1));
      waddch(MineWin, '#');
    }
  }
}

int Move_MineWin(WINDOW * MineWin, int iy, int ix, unsigned int * MineMap){
  int mx=ix/2,my=iy/2,ch,tx=ix/2,ty=iy/2;
  int action=1;
  int rc = 0;
  int result = 0;

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
        rc = Check_MineWin(MineWin, iy, ix, MineMap, my, mx, 1);
        if (rc > 0) {
          result = rc;
          action = 0;
        }
        wrefresh(MineWin);
        break;
      case 'f': // Mark flag
        Mark_MineWin(MineWin, iy, ix, MineMap, my, mx);
        break;
      case 'q':
        action=0;
        result = Quit_Or_Nothing;
        break;
      default:
        break;
    }
    // For debug flag
    //mvprintw(1,1," mx: %d, my: %d, flag: %d\n", mx, my, *(MineMap+my*ix+mx));
  }
  return result;
}

int Action_MenuWin(int *level, WINDOW ** MenuWin){
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
        wmove(*MenuWin, my, mx);
        break;
      case KEY_DOWN:
        mvwaddstr(*MenuWin,my,1,"  * ");
        my=((my<4)?my+1:2);
        mvwaddstr(*MenuWin,my,1," <*>");
        wmove(*MenuWin, my, mx);
        break;
      case 32:     // space
      case '\r':   // enter
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
  touchwin(stdscr);
  return 1;
}

int main(void){
  int ix=16,iy=15;
  int imine=10;
  int level[3][3] ={{10, 10, 10},{15, 15, 40},{30, 15, 100}};
  unsigned int * MineMap;
  int * MineList, ilevel;
  WINDOW *MineWin, *MenuWin, *MineTitleWin;
  int rc = 0;

// Print MineMap
  init_window();

  ilevel=Action_MenuWin(level[0], &MenuWin);
  ix=level[ilevel][0];
  iy=level[ilevel][1];
  imine=level[ilevel][2];
  touchwin(stdscr);

  MineMap=(unsigned int*)malloc(ix*iy*sizeof(unsigned int));
  MineList=(int*)malloc(imine*sizeof(int));

  refresh();

  MineTitleWin = newwin(2, 80, 0, 2);
  mvwprintw(MineTitleWin, 0, 0, "ilevel: %d, ix: %d, iy: %d, imine: %d",ilevel, ix, iy, imine);
  mvwprintw(MineTitleWin, 1, 0, "'f': swith flag, 'space': open a cell, 'q': quit game, 'arrow key': move cursor");
  wrefresh(MineTitleWin);


  CreateMineMap(MineMap, ix*iy, MineList, imine);
  Create_MineWin(&MineWin, iy, ix, MineMap, 2, 2);
  rc = Move_MineWin(MineWin, iy, ix, MineMap);

  wmove(MineTitleWin, 1, 0);
  winsdelln(MineTitleWin, 1);
  if (rc == Quit_Or_Nothing)
    mvwprintw(MineTitleWin, 1, 0, "Quit. Press any key.");
  else if (rc == Mine_Opened)
    mvwprintw(MineTitleWin, 1, 0, "Oops, you open a mine...Press any key.");
  else if (rc == All_Clean)
    mvwprintw(MineTitleWin, 1, 0, "All clean, you win! Press any key.");

  wrefresh(MineTitleWin);

  getch();
  endwin();

#if DEBUG
  printMap(MineMap, ix, iy);
#endif
  free(MineMap);
  free(MineList);
  return 0;
}

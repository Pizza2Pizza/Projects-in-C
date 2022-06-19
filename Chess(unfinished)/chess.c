#include <stdio.h>
#include <stdlib.h>

#define WHITE 2
#define BLACK 1
#define NOCOLOR 0

int turn_count = 0;
/*char Board[8][8] = {
  {'T','S','L','D','K','L','S','T'},
  {'B','B','B','B','B','B','B','B'},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {'b','b','b','b','b','b','b','b'},
  {'t','s','l','d','k','l','s','t'},
};
*/
char Board[8][8] = {
  {'T','S','L','D','K','L','S','T'},
  {'B','B','B','B','d','B','B','B'},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {'b','b','b','b','b','b','b','b'},
  {'t','s','l','d','k','l','s','t'},
};

void printBoard(){
  printf("    A   B   C   D   E   F   G   H   \n");
  for(int i=0; i<8; i++){
    printf("  --------------------------------\n");
    printf("%d",8-i);

    for(int j=0; j<8; j++){
      if(Board[i][j] == 0) printf(" |  ");

      else printf(" | %c", Board[i][j]);
    }
    printf(" |");
    printf("%d",8-i);
    printf("\n");
  }
  printf("  --------------------------------\n");
  printf("    a   b   c   d   e   f   g   h   \n");
}

int get_color(int x, int y){
  char buchstabe = Board[x][y];
  if(buchstabe >='a' && buchstabe <= 'z')
    return WHITE;//weisse figure
  if(buchstabe >='A' && buchstabe <= 'Z')
    return BLACK;//schwarze
  if(buchstabe == 0)
    return NOCOLOR;//nichts steht
}

//funktionen geben 1 zuruck when der Zug gültig ist
int Konig(int x, int y, int x_new, int y_new){
  if((x_new ==x+1 && y_new == y+1) || (x_new ==x+1 && y_new == y) || (x_new ==x+1 && y_new == y-1) ||
    ( x_new ==x && y_new == y+1)   || (x_new ==x && y_new == y-1) ||
    (x_new ==x-1 && y_new == y+1)  || (x_new ==x-1 && y_new == y) || (x_new ==x-1 && y_new == y-1)){
      return 1;
    }
    return 0;
}

int Springer(int x, int y, int x_new, int y_new){
  if((x_new == x+2 && y_new == y+1) || (x_new == x+2 && y_new == y-1) ||
    (x_new == x+1 && y_new == y+2)  || (x_new == x+1 && y_new == y-2) ||
    (x_new == x-1 && y_new == y+2)  || (x_new == x-1 && y_new == y-2) ||
    (x_new == x-2 && y_new == y+1)  || (x_new == x-2 && y_new == y-1)){
  return 1;
}
return 0;
}

int Turm(int x, int y, int x_new, int y_new){
  if(y_new == y){
    for(int i =(x_new >x)? x+1 : x-1; i!= x_new; (x_new >i)? i++ : i-- ){
      if(Board[i][y] != 0)
        return 0;
    }
  }
  if(x_new ==x){
    for(int i =(y_new >y)? y+1 : y-1; i!= y_new; (y_new >i)? i++ : i-- ){
      if(Board[x][i] != 0)
        return 0;
    }
  }
return 1;
}

int Laufer(int x, int y, int x_new, int y_new){
  if(abs(x_new-x)== abs(y_new-y)){
    while(!(x==x_new && y==y_new)){
      x +=(x<x_new) ? 1:-1;
      y +=(y<y_new) ? 1:-1;
      if (Board[x][y] !=0) //check the board
          return 0;
    }
  }
  else return 0;

  return 1;
}

int Dame(int x, int y, int x_new, int y_new){
  if(abs(x_new-x)== abs(y_new-y)){
    return Laufer(x, y, x_new, y_new);
  }
  if((y_new == y) || (x_new ==x)){
    return Turm(x, y, x_new, y_new);
  }
  else return 0;
}

#define MARK printf("At line %d\n", __LINE__);

int Bauer(int x, int y, int x_new, int y_new){
  int schwarz = (Board[x][y] =='B'? 1:0);
  if(abs(x_new -x) ==2){//die erste zug
    if((schwarz && x!=1)||(!schwarz && x!=6)){
      return 0;
  }
}
  if(((y_new == y +1)&& (x_new == x+1))||((y_new == y-1) && (x_new == x+1))){//diagonal
    if(schwarz && get_color(x_new,y_new) == WHITE){// weisse figure auf die diagonal- kann schlagen
      return 1;
    }
    else {
      return 0;//wenn schwarze oder nicht steht_ kann nicht gehen
    }
  }
  if(((y_new == y +1)&& (x_new == x-1))||((y_new == y-1) && (x_new == x-1))){
      if(!schwarz && get_color(x_new,y_new) == BLACK){
        return 1;
      }
      else {
      return 0;
    }
  }
  for(int i =(x_new >x)? x+1 : x-1; 1; (x_new >i)? i++ : i-- ){//die zuge nach vorne sind gultig wenn nichts steht
    //printf("checking (%d,%d)", i, y_new);

    if(Board[i][y_new] != 0){
      return 0;
    }
    if(i == x_new)
      break;
  }

  return 1;
}
void score_board(char figure){
  FILE *score = fopen("score.txt", "r+");
  int score_white =0;
  int score_black=0;
  rewind(score);
  fscanf(score,"White: %d Black: %d", &score_white, &score_black);
  printf("%d %d\n",score_white, score_black);
  switch(figure){
    case 'B': score_white +=1;break;
    case 'b': score_black +=1;break;
    case 'T': score_white +=5;break;
    case 't': score_black +=5;break;
    case 'L': score_white +=3;break;
    case 'l': score_black +=3;break;
    case 'S': score_white +=3;break;
    case 's': score_black +=3;break;
    case 'K': score_white +=10;break;
    case 'k': score_black +=10;break;
    case 'D': score_white +=9;break;
    case 'd': score_black +=9;break;
  }
  rewind(score);
  fprintf(score, "%s %d %s %d\n", "White:", score_white, "Black:", score_black);
  printf("%d %d \n",score_white, score_black );
  fclose(score);

}
char* check_move(int x, int y, int x_new, int y_new){
  char p = Board[x][y];
  if (p == 0){
    return "Selected field is empty";
  }
  int ok = 1;//check
  if(p == 'K' || p == 'k'){
    ok = Konig(x, y, x_new, y_new);
  } else if(p == 'D' || p == 'd'){
      ok = Dame(x, y, x_new, y_new);
  }else if(p == 'T' || p == 't'){
      ok = Turm(x, y, x_new, y_new);
  }else if(p == 'S' || p == 's'){
      ok = Springer(x, y, x_new, y_new);
  }else if(p == 'L' || p == 'l'){
      ok = Laufer(x, y, x_new, y_new);
  }else if(p == 'B' || p == 'b'){
      ok = Bauer(x, y, x_new, y_new);
  }

  if (ok == 0) {
    return "Error: ungultig Zug fuer figur";
  }
  //welche Farbe hat die figure
  if (get_color(x,y) == get_color(x_new,y_new)){
    return "Selected figur is from the same team";
  }
  return 0;
}

void move(int x, int y, int x_new, int y_new){
  //check if it's the turn of the selected color
  if(turn_count %2 != 0 && get_color(x,y) == WHITE){
    printf("Error by selected color\n");
    return;
  }
  if(turn_count %2 == 0 && get_color(x,y) == BLACK){
    printf("Error by selected color\n");
    return;
  }

  char p = Board[x][y];
  char* msg = check_move(x,y,x_new,y_new);
  if(msg != 0) {
    printf("Move is not possible: %s\n", msg);
    return;
  }
  if(msg == 0){
    if(Board[x_new][y_new] !=0){
      char figure = Board[x_new][y_new];
      score_board(figure);
     }

    Board[x][y] = 0;
    Board[x_new][y_new] = p;
    turn_count++;
  }
}

//return 1 for check, 0 for not check
int check(int color){
  int k_x, k_y; //position of the king
  for(int i = 0; i<8; i++){
    for(int j = 0; j<8; j++){
      if(color == WHITE && Board[i][j] == 'k'){
        k_x = i;
        k_y = j;
      }
      if(color == BLACK && Board[i][j] == 'K'){
        k_x = i;
        k_y = j;
      }
    }
  }
  printf("Find the king in the position %d %d of color %d\n",k_x, k_y, color);
  for(int i = 0; i<8; i++){
    for(int j = 0; j<8; j++){
      if(color == WHITE && get_color(i,j) == BLACK){
        if(check_move(i,j,k_x,k_y) == 0){
          return 1;
        }
      }
      if (color == BLACK && get_color(i,j) == WHITE){
        printf("Checking if the piece %c in the position %d %d can move to %d %d\n",Board[i][j], i, j, k_x, k_y);
        if(check_move(i,j,k_x,k_y) == 0){
          return 1;
        }
        else{
          char *msg = check_move(i,j,k_x,k_y);
          printf("%s\n", msg);
        }
      }
    }
  }
  return 0;
}

int main(){
FILE *score = fopen("score.txt", "r+");
fprintf(score, "White: %d Black: %d", 0, 0);
fclose(score);
char c1,c2;
int s1,s2;
while(1){
printBoard();
if(turn_count %2 != 0)
  printf("It's black turn\n");
else printf("It's white's turn\n");
printf("Your move from (x,y) to (x1,y1):\n");
scanf("%c%d - %c%d", &c1,&s1, &c2, &s2);

int x = c1 -'a';
int x_new = c2 -'a';
int y = 8 -s1;
int y_new = 8-s2;
move(y,x,y_new,x_new);
if(turn_count %2 != 0){
  int isCheck = check(BLACK);
  if(isCheck != 0) printf("Black is in check\n");
}
else {
  int isCheck = check(WHITE);
  if(isCheck != 0) printf("White is in check\n");
}
}
return 0;
}

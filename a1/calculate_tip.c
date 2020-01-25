#include <stdio.h>
#define THE_BILL 272

void Tip(int n){
  double tip = n * 0.2;
    printf("$ %.2f.",tip);
}
int main(){
    Tip(THE_BILL);
}

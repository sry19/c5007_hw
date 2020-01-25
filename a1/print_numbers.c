#include <stdio.h>

void PrintNumbers(int n){
    for (int i=1; i<=n; i++){
        for (int j=0;j<i;j++){
            printf("%d",i);
        }
       
    }
}

int main(){
  PrintNumbers(5);
}

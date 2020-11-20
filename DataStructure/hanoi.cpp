#include<stdio.h>
void move(char a,char b){
	printf("移动盘子从%c到%c\n",a,b);
}
void Hanoi(int n,char a,char b,char c){
	if(n==1){
		move(a,c);
	}
	else{
		Hanoi(n-1,a,c,b);
		move(a,c);
		Hanoi(n-1,b,a,c);
	}
} 
int main(){
	int n=100;
	Hanoi(n,'A','B','C');
	return 0;
}

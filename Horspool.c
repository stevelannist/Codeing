//
//  main.c
//  Algorithm
//
//  Created by 罗政 on 10/29/20.
//  Copyright © 2020 Ray. All rights reserved.
//

#include <stdio.h>
int Table[256];
int ShiftTable(char *p,int m){ // 输入为模式以及模式长度
    int i=0;
    
    for (int i=0;i<256;i++){
        Table[i]=m; // 先将移动表全部确定为模式长
    }
    for(int j=0;j<m-1;j++){
        Table[p[i]]=m-1-j; // 确定模式中字符移动长为 模长-当前字符索引
    }
    return *Table;
    
}

int Horspool(char *T,char *p,int n,int m){
    // 输入为 模式p 文本T，文本长度n ，模式长度为m
    ShiftTable(p,m);  // 第一步：生产移动表
    int i=m-1;   // i 指向 模式最右端位置
    while(i<=n-1)
        {
            int k=0;  // 做从右到左的匹配
            while(k<=m-1&&p[m-1-k]==T[i-k])
                {
                    k=k+1;
                }
            if(k==m)
                return i-m+1;
            else
                i=i+Table[T[i]];
            
        }
    return -1;
    

}


int main() {
    char *p="BARBER";
    char *T="JIM_SAW_ME_IN_A_BARBERSHOP";
    int i=Horspool(T,p,26,6);
    printf("字符匹配的首位为%d",i);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define OVERFLOW -2
#define STR_INIT_SIZE 100
#define STRINCREMENT 10
typedef int Status;
typedef struct
{
    char *ch; //空串时指向NULL,非空串时按串长分配存储区
    int length;
} HString;
Status InitString(HString *T) //初始化字符串
{
    //指针指向NULL,长度为0即可
    //p.s.申请内存空间的过程在赋值中完成
    T->ch = NULL;
    T->length = 0;
    return OK;
}
Status StrAssign(HString *T, char *p) //字符串赋值
{
    //1.判断T是否已有内容,有则释放
    //2.判断赋值的内容是否为空,为空则不赋值
    //3.根据长度向内存申请空间,遍历赋值给T,长度等于字符串长度
    //p.s.在这里赋值不赋\0,在打印时通过长度来判断字符串结尾
    int i, len = strlen(p);
    if (T->ch)
        free(T->ch);
    if (!len)
    {
        T->ch = NULL;
        T->length = 0;
        return ERROR;
    }
    else
    {
        T->ch = (char *)malloc(len * sizeof(char));
        if(!T->ch)
            exit(OVERFLOW);
        for (i = 0; i < len; ++i)
            T->ch[i] = p[i];
        T->length = len;
        return OK;
    }
}
Status StrPrint(HString T) //打印字符串
{
    //通过长度判断打印的字符数
    int i;
    for (i = 0; i < T.length; ++i)
        printf("%c", T.ch[i]);
    printf("\n");
}
Status StrLength(HString T) //字符串长度
{
    return T.length;
}
Status StrEmpty(HString T) //字符串判空
{
    if (T.length == 0)
        return TRUE;
    else
        return FALSE;
}
Status Concat(HString *T, HString S1, HString S2) //字符串联接
{
    //1.申请长度为S1和S2之和的字符串空间
    //2.先将S1的元素逐个赋值到T中
    //3.再将S2的元素逐个赋值到T中
    int i;
    if (T->ch)
        free(T->ch);
    T->ch = (char *)malloc((S1.length + S2.length) * sizeof(char));
    if (!T->ch)
        exit(OVERFLOW);
    for (i = 0; i < S1.length; ++i)
        T->ch[i] = S1.ch[i];
    for (i = 0; i < S2.length; ++i)
        T->ch[i + S1.length] = S2.ch[i];
    T->length = S1.length + S2.length;
    return OK;
}
Status StrDelete(HString *T, int pos, int len) //删除字符串中某个位置固定长度的子串
{
    //pos是字符串中的位置,删除包括pos的len长度
    int i;
    if (pos >= T->length)
        return ERROR;
    else if(pos + len > T->length)
        len = T->length - pos + 1;
    for (i = pos - 1; i < T->length - len; ++i)
        T->ch[i] = T->ch[i + len];
    T->length -= len;
    T->ch = (char *)realloc(T->ch, T->length * sizeof(char));
    if (!T->ch)
        exit(OVERFLOW);
    return OK;
}
Status StrInsert(HString *S, int pos, HString T)
{
    //pos是字符串中的位置,插入时原来的元素(包括pos位)后移
    int i, len;
    --pos;
    len = StrLength(T);
    S->ch = (char *)realloc(S->ch, (S->length + len) * sizeof(char));
    if (pos > S->length)
        pos = S->length;
    for (i = S->length - 1; i > pos - 1; --i)
        S->ch[i + len] = S->ch[i];
    for (i = 0; i < len; ++i)
        S->ch[i + pos] = T.ch[i];
    S->length += len;
    if (!S->ch)
        exit(OVERFLOW);
    return OK;
}
Status Index(HString S, HString T, int pos) //在字符串S中索引位置pos之后的子串t
{
    //同定长顺序存储表示法
    //p.s.传入的pos是字符串的位置,从1开始
    //p.s.初始状态下T为非空串
    if (StrEmpty(T))
        return ERROR;
    int i = pos - 1, j = 0;
    while(i < S.length && j < T.length)
    {
        if (S.ch[i] == T.ch[j])
        {
            ++i;
            ++j;
        }
        else
        {
            i = i - j + 1;
            j = 0;
        }
    }
    if (j >= T.length)
        return i - j + 1;
    else
        return 0;
}
Status Replace(HString *T, HString S1, HString S2) //将字符串T中等于S1的子串替换成为S2
{
    //循环索引子串S1在字符串T中的位置(每次的位置从上一次位置后开始查找)
    //从查找到的位置-1开始替换
    //p.s.初始状态下S1为非空串
    int pos = 0;
    if (StrEmpty(S1))
        return ERROR;
    //当pos存在时循环,当全部索引完毕后pos为0
    //将索引到的该位置对应的子串删除后再插入新的子串
    do
    {
        pos = Index(*T, S1, pos);
        if (pos)
        {
            StrDelete(T, pos, StrLength(S1));
            StrInsert(T, pos, S2);
        }
    }
    while(pos);
    return OK;
}
Status SubString(HString *Sub, HString S, int pos, int len)
{
    int i;
    if (pos < 1 || len > S.length || len < 0 || len > S.length - pos + 1)
        exit(OVERFLOW);
    if (Sub->ch)
        free(Sub->ch);
    //如果查询的长度为0,则子串置空
    if (len == 0)
    {
        Sub->ch = NULL;
        Sub->length = 0;
    }
    else
    {
        Sub->ch = (char *)malloc(len * sizeof(char));
        for (i = 0; i < len; ++i)
            Sub->ch[i] = S.ch[pos + i - 1];
        Sub->length = len;
    }
    return OK;
}
int main()
{
    int pos;
    HString t, s, r;
    char *p = "Hello,String!", *q = "Bye,Bye!";
    printf("String *p: %s\n", p);
    InitString(&t);
    StrAssign(&t, p);
    printf("StrAssign... OK.\nString t : ");
    StrPrint(t);
    printf("------------------------------\n");
    printf("StrLength... OK.\nString Length : %d\n", StrLength(t));
    printf("StrEmpty... OK.\n");
    if (StrEmpty(t))
        printf("String is Empty.\n");
    else
        printf("String is not Empty.\n");
    printf("------------------------------\n");
    InitString(&s);
    StrAssign(&s, q);
    printf("String s : ");
    StrPrint(s);
    printf("------------------------------\n");
    InitString(&r);
    Concat(&r, t, s);
    printf("Concat... OK.\n");
    printf("String r : ");
    StrPrint(r);
    printf("------------------------------\n");
    printf("StrDelete... OK.\n");
    StrDelete(&r, 14, 4);
    printf("String r : ");
    StrPrint(r);
    printf("------------------------------\n");
    printf("StrInsert... OK.\n");
    StrAssign(&t, "Bye,Bye,Bye!");
    StrInsert(&r, 14, t);
    printf("String r : ");
    StrPrint(r);
    printf("------------------------------\n");
    StrAssign(&t, "ye");
    printf("Index... ");
    StrPrint(t);
    pos = 1;
    while(pos)
    {
        pos = Index(r, t, pos + 1);
        if (!pos)
            break;
        printf("Position : %d\n", pos);
    }
    printf("------------------------------\n");
    StrAssign(&t, "ye");
    StrAssign(&s, "oo");
    Replace(&r, t, s);
    printf("Replace ye -> ooo ... OK.\n");
    printf("String r : ");
    StrPrint(r);
    printf("------------------------------\n");
    SubString(&t, r, 7, 4);
    printf("SubString... OK.\n");
    printf("String SubString : ");
    StrPrint(t);
    printf("------------------------------\n");
    return OK;
}

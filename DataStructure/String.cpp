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
    char *ch; //�մ�ʱָ��NULL,�ǿմ�ʱ����������洢��
    int length;
} HString;
Status InitString(HString *T) //��ʼ���ַ���
{
    //ָ��ָ��NULL,����Ϊ0����
    //p.s.�����ڴ�ռ�Ĺ����ڸ�ֵ�����
    T->ch = NULL;
    T->length = 0;
    return OK;
}
Status StrAssign(HString *T, char *p) //�ַ�����ֵ
{
    //1.�ж�T�Ƿ���������,�����ͷ�
    //2.�жϸ�ֵ�������Ƿ�Ϊ��,Ϊ���򲻸�ֵ
    //3.���ݳ������ڴ�����ռ�,������ֵ��T,���ȵ����ַ�������
    //p.s.�����︳ֵ����\0,�ڴ�ӡʱͨ���������ж��ַ�����β
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
Status StrPrint(HString T) //��ӡ�ַ���
{
    //ͨ�������жϴ�ӡ���ַ���
    int i;
    for (i = 0; i < T.length; ++i)
        printf("%c", T.ch[i]);
    printf("\n");
}
Status StrLength(HString T) //�ַ�������
{
    return T.length;
}
Status StrEmpty(HString T) //�ַ����п�
{
    if (T.length == 0)
        return TRUE;
    else
        return FALSE;
}
Status Concat(HString *T, HString S1, HString S2) //�ַ�������
{
    //1.���볤��ΪS1��S2֮�͵��ַ����ռ�
    //2.�Ƚ�S1��Ԫ�������ֵ��T��
    //3.�ٽ�S2��Ԫ�������ֵ��T��
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
Status StrDelete(HString *T, int pos, int len) //ɾ���ַ�����ĳ��λ�ù̶����ȵ��Ӵ�
{
    //pos���ַ����е�λ��,ɾ������pos��len����
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
    //pos���ַ����е�λ��,����ʱԭ����Ԫ��(����posλ)����
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
Status Index(HString S, HString T, int pos) //���ַ���S������λ��pos֮����Ӵ�t
{
    //ͬ����˳��洢��ʾ��
    //p.s.�����pos���ַ�����λ��,��1��ʼ
    //p.s.��ʼ״̬��TΪ�ǿմ�
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
Status Replace(HString *T, HString S1, HString S2) //���ַ���T�е���S1���Ӵ��滻��ΪS2
{
    //ѭ�������Ӵ�S1���ַ���T�е�λ��(ÿ�ε�λ�ô���һ��λ�ú�ʼ����)
    //�Ӳ��ҵ���λ��-1��ʼ�滻
    //p.s.��ʼ״̬��S1Ϊ�ǿմ�
    int pos = 0;
    if (StrEmpty(S1))
        return ERROR;
    //��pos����ʱѭ��,��ȫ��������Ϻ�posΪ0
    //���������ĸ�λ�ö�Ӧ���Ӵ�ɾ�����ٲ����µ��Ӵ�
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
    //�����ѯ�ĳ���Ϊ0,���Ӵ��ÿ�
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

/*
��ʾ����Ҫ��Ϊ�˱�����⺯��ָ���Լ���Ӧ��.��linux c�����,�������õ�.
����,��*nux��signal����,��ANSI C���䶨��ԭ������:
void (*signal(int signo,void (*func)(int))) (int)
���������Ų���������,�����ĺ�����
void (* XXX )(int)������һ��ָ��,ָ��һ������XXX��ָ��,XXX������ĺ�����Ҫһ��int�͵Ĳ���,����void����.
����ȽϺ����.
signal(int signo, void(*func)(int))��signal����������.
���Կ���,signal������������:int signo�Լ�һ��ָ�����ĺ���void (*func)(int).
�����临����,��[Plauger 1992]��typedef��������м�.
typedef void Sigfuc(int);//������Կ���һ������ֵ .
�ٶ�signal�������м򻯾��Ǳ�Ϊ: Sigfunc *signal(int,Sigfuc *);
typedef֮��ͱ�ü���.
����һ���÷������ڽṹ����Ӧ�ú���ָ��,��ʵ��C���������...O(��_��)O~����ȽϿ��!
���ϵĶ����������ʾ��������.
����ֵΪָ��ĺ���,�뺯��ָ��:
char *func();//����һ���ַ�ָ��
char (*func)();//����ָ��.
ֻҪ��ס����()�����ȼ���ָ��*�ߣ�����������.
�� char *func()��char func()�Ա�,���ǿ��Խ�char *func()�е�char *��������һ�������������ˡ�
��char (*func)()�е�func�����ڼ������ţ������Ⱥ�ָ��*��ϣ����һ��ָ�룬
������ͨ����char function()�е�function��ʵҲ��һ������ָ��һ����
��������Խ�(*func)������һ����ͨ������
*/
#include <stdio.h>
#include <stdlib.h>

//ǿ��ת��0Ϊ(void(*)(int))����ָ������
#define SIG_ERR (void(*)(int))0
#define SIG_OK  (void(*)(int))1

typedef struct calc{
char id[10];
int (* func)(int x,int y);/*func,Ϊ����ָ��,��������int���Ͳ���,����һ������.*/
}CALC;

int add(int x,int y){
    return x+y;
}

int multi(int x,int y){
   return x*y;
}

void test1(int t){
   printf("test1(%d) ing...\n",t);
}
void test2(int t){
   printf("test2(%d) ing...\n",t);
}
void test3(int t){
   printf("test3(%d) ing...\n",t);
}

typedef  void (* PRTFUNC)(int);
//void ( *signal( int sig, void( *func )( int ) ) )( int );
//=> void ( *  signal_test( int sig, void( *func )( int )  )   )( int );
PRTFUNC  signal_test(int sig,PRTFUNC func){
    PRTFUNC t;
    switch(sig){
     case 1:
         t=test1;
         printf("���ú���test1()~\n");
         break;
     case 2:
         t=test2;
         printf("���ú���test2()~\n");
         break;
    default:
        t=(void(*)(int))0;
         printf("���ú���test3()~\n");
    }
  func(sig);
  return t;
}

//���Է���ֵ
void check_singal_ret(PRTFUNC ret){
     if(ret==SIG_ERR)
         printf("signal_test function returns SIG_ERR.\n");
      else
        printf("signal_test function returns SIG_OK.\n");
}

int main()
{
    int i;
    //��������CALC�ṹ�����,�ֱ���ָ��add������multi����.
    CALC  test[2] = {{"No.1",add},{"No.2",multi}};
    PRTFUNC  test_ptr_array[3]={test1,test2,test3};

    printf("1.���к���ָ��Ľṹ��:\n");
    printf("CALC test[0]: %s,and result:%d\n",test[0].id,test[0].func(12,13));
    printf("CALC test[1]: %s,and result:%d\n",test[1].id,test[1].func(12,13));

    printf("\n2.����ָ������:\n");
    for(i=0;i<3;++i)
      test_ptr_array[i](i*10);

    printf("\n3.����ָ��ͷ��غ���ָ��ʵ��:\n");
    check_singal_ret(signal_test(1,test2));
    check_singal_ret(signal_test(2,test1));
    check_singal_ret(signal_test(3,test3));

    if(signal_test(3,test2)==SIG_ERR){
      printf("signal_test(3,test2) returns SIG_ERR.");
    }else{
      printf("signal_test(3,test2) returns SIG_OK.");
    }
    return 0;
}


/*
本示例主要是为了便于理解函数指针以及其应用.在linux c编程中,常常会用到.
比如,在*nux中signal函数,在ANSI C中其定义原型如下:
void (*signal(int signo,void (*func)(int))) (int)
咱们来试着拆分这个函数,最外层的函数体
void (* XXX )(int)表明是一个指针,指向一个函数XXX的指针,XXX所代表的函数需要一个int型的参数,返回void类型.
这个比较好理解.
signal(int signo, void(*func)(int))是signal函数的主体.
可以看到,signal函数两个参数:int signo以及一个指向函数的函数void (*func)(int).
由于其复杂性,在[Plauger 1992]用typedef来对其进行简化.
typedef void Sigfuc(int);//这里可以看成一个返回值 .
再对signal函数进行简化就是变为: Sigfunc *signal(int,Sigfuc *);
typedef之后就变得简单了.
还有一个用法就是在结构体中应用函数指针,来实现C面向对象编程...O(∩_∩)O~这个比较酷吧!
以上的都可在下面的示例中体现.
返回值为指针的函数,与函数指针:
char *func();//返回一个字符指针
char (*func)();//函数指针.
只要记住函数()的优先级比指针*高，就容易区分.
将 char *func()和char func()对比,咱们可以将char *func()中的char *看成是有一个括号括起来了。
而char (*func)()中的func，由于加了括号，所以先和指针*结合，变成一个指针，
正如普通函数char function()中的function其实也是一个函数指针一样，
所以你可以将(*func)当成是一个普通变量。
*/
#include <stdio.h>
#include <stdlib.h>

//强制转换0为(void(*)(int))函数指针类型
#define SIG_ERR (void(*)(int))0
#define SIG_OK  (void(*)(int))1

typedef struct calc{
char id[10];
int (* func)(int x,int y);/*func,为函数指针,接收两个int类型参数,返回一个整型.*/
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
         printf("调用函数test1()~\n");
         break;
     case 2:
         t=test2;
         printf("调用函数test2()~\n");
         break;
    default:
        t=(void(*)(int))0;
         printf("调用函数test3()~\n");
    }
  func(sig);
  return t;
}

//测试返回值
void check_singal_ret(PRTFUNC ret){
     if(ret==SIG_ERR)
         printf("signal_test function returns SIG_ERR.\n");
      else
        printf("signal_test function returns SIG_OK.\n");
}

int main()
{
    int i;
    //声明两个CALC结构体变量,分别用指定add函数和multi函数.
    CALC  test[2] = {{"No.1",add},{"No.2",multi}};
    PRTFUNC  test_ptr_array[3]={test1,test2,test3};

    printf("1.带有函数指针的结构体:\n");
    printf("CALC test[0]: %s,and result:%d\n",test[0].id,test[0].func(12,13));
    printf("CALC test[1]: %s,and result:%d\n",test[1].id,test[1].func(12,13));

    printf("\n2.函数指针数组:\n");
    for(i=0;i<3;++i)
      test_ptr_array[i](i*10);

    printf("\n3.函数指针和返回函数指针实验:\n");
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


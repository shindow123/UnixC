#include<iostream>
#include<iterator>
#include<vector>
using namespace std;

void Print(int* &p){   //指针的引用
                       //&作用 限定参数是指针，如果是地址，会报错，不允许地址隐式转换为指针。
    (*p)++;            //相当于(*b)++
    cout << *p << endl;
    p = new int(10);   //相当于b = new int(10);
}

int main(){
    int a=1;
    //Print(&a);   不能传入地址，一定要是指针
    int* b = &a;
    Print(b);     //传入指针OK
    cout << a << endl;
    cout << *b << endl;
}

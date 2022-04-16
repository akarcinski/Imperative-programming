#include <iostream>

using namespace std;

int add( int a, int b ){
    return a+b;
}

int multiply( int a, int b){
    return a*b;
}

void xd( int( *f )( int, int ), int a, int b ){
    cout << f( a, b ) << endl;
}

int main(){

    int a, b;
    cin >> a >> b;

    xd( add, a, b );
    xd( multiply, a, b);

    return 0;
}
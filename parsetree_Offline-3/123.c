int a, b;

float x, y;

int add_int(int, int);

float mul_float(float, float, float);

void print_global();
int add_int(int a, int b, int c) { 
    return a+b;
}

float mul_float(float x, float y, float z) {
    return x*y*z;
}

void print_global() {
    print(a);
    print(b);
    print(x);
    print(y);
}


int main() {
    int list[5], another_list[5], i;


    x = 1+2*3-4/2+(7.1%3); 
    a = 2.3*4.7;  

    list[1] = a+list[0.4]+another_list[0]; 

    {
        int i, j;
        int i;  
    }

    j = 7.5;  

    for(i=0; i<5; i++) {
        list[0]++; 
    }

    i();  

    if(list[4]>10 || another_list[4]>=10) {
        i = !0;
    }

    if(a==0 && x!=3.9) {
        y = mul_float(1.2, 2.5*2.0);  
        b = mul_float(1.2, 2.5, 2.0);  
    } else {
        b = add_int(list, 7);  
    }

    list[2] = print_global();  

    return 0;
}
int main(){
  int step , array[5],x,l,j,key ;
  array[0] = 9 ;
  array[1] = 4 ;
  array[2] = 7 ; 
  array[3] = 12 ;
  array[4] = 2 ;
  for(step = 0 ; step < 5; step++) {
	key = array[step];
	j = step - 1;
	l = array[j] ;
    while (key < l && j >= 0) {
      array[j + 1] = array[j];
      j--;
    }
    array[j + 1] = key;
  }
  for(step = 0 ; step < 5 ; step++) {
	x = array[step] ;
	println(x) ;
  }
}

int num;
scanf("%d", &num);
int n1;
int pid = -1;
for(int i=0; i<2; i++) {
   if(pid) {
      n1 = num%10;
      pid = fork();
      num /= 10;
   }
}
for(int i=0; i<3; i++) {
   if(i == 0 && n1%2) {
      if(!fork()) {
      	printf("X");
        return 0;
      }
      
   }
   printf("%d", num);
   num /= 10;
}

#include<stdio.h>
#include<string.h>
char*T[]={
"auto",
"extrn",
"goto",
"return",
"if",
"{","}",
"(",")",
"[","]",
";",":",",",0};
char*s="main(){}";
int i;
int n(){
	int i,l;
	while(*s==' '||*s=='\t'||*s=='\n')++s;
	if (!*s)return-1;
	if(isdigit(*s))return-2;
	for(i=0;T[i];++i){
		l=strlen(T[i]);
		if(!memcmp(s,T[i],l)&&
		(*s!='_'||!isalnum(*s)||
		s[l]!='_'||!isalnum(s[l])))
			return s+=l,i;
	}
	return 12;
}
char*iden(){
	static char k[8];
	int i;
	for(i=0;s[i]=='_'||isalnum(s[i]);++i)k[i]=s[i];
	return s+=i,k;
}
int main(){
	int t;
rpt:switch(t=n()){
	case-1:printf("eof\n");break;
	case-2:printf("num%d\n",strtol(s,&s,0));goto rpt;
	case 12:printf("iden%s\n",iden());goto rpt;
	default:printf("sym%s\n",T[t]);goto rpt;
	}
}
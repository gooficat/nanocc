nnum(){
	extern num;
	extern c;
	num=0;
	do num=num*10+c-'0';
	while((c=getchar())>='0'&&c<='9');
}

nidn(){
	extern c;
	extern char idn[];
	auto i;
	i=0;
	do idn[i]=c;
	while((c=getchar())>='a'&&c<='z');
	idn[i]=0;
	if (!strcmp(idn,"extrn"))
		return 0;
	if (!strcmp(idn,"auto"))
		return 1;
	if (!strcmp(idn,"goto"))
		return 2;
	if (!strcmp(idn,"retrn"))
		return 3;
	if (!strcmp(idn,"if"))
		return 4;
	return -3;
}

neat(){
	extern c;
	auto n;
	n=c;
	c=getchar();
	return n;
}

next(){
	extern c;
	while(c=='\n'||c=='\t'||c==' '){
		c=getchar();
	}
	if(c==-1)
		return -1;
	if(c>='0'&&c<='9')
		return nnum(), -2;
	if (c>='a'&&c<='z')
		return nidn();
	return neat();
}

main(){
	extern tok,num,c;
	extern char idn[];
	c=getchar();
rpt:
	switch(tok=next()){
	case-1:break;
	case-2:
		printf("num %d\n", num);
		goto rpt;
	case -3:
		printf("idn %s\n", idn);
		goto rpt;
	default:
		if (tok<=4)
			printf("tok %d\n", tok);
		else
			printf("tok `%c`\n", tok);
		goto rpt;
	}
}

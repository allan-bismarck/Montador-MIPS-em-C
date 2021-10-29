#include<stdio.h>
#include<stdlib.h>
#include<string.h>

  char instrucao[23][5],funct[23][7],opcode[23][7],registradores[32][6],numREG[32][6],tipo,linha[80],c,strings[4][20],**label,**LABEL_bin,binario[33],hexa[4][5];
  FILE *arquivo,*destino;
  int numlinhas=0,numLABEL=0,*linhaLABEL,numlinhasDEST=0,cont=0;


void verifica_tipo_identifica_label() {
  char aux[15],*p;
  p=strchr(linha,':');
  tipo='l';
  if(p) {
  	sscanf(linha,"%s %s",aux,aux);
  } else {
  	  sscanf(linha,"%s",aux);
    }
  if(!strcmp(aux,"add\0")||!strcmp(aux,"sub\0")||!strcmp(aux,"sll\0")||!strcmp(aux,"srl\0")||!strcmp(aux,"and\0")||!strcmp(aux,"or\0")||!strcmp(aux,"xor\0")||!strcmp(aux,"slt\0")||!strcmp(aux,"mul\0")||!strcmp(aux,"div\0")||!strcmp(aux,"jr\0")) {
    tipo='r';
  }
  if(!strcmp(aux,"addi\0")||!strcmp(aux,"lui\0")||!strcmp(aux,"andi\0")||!strcmp(aux,"ori\0")||!strcmp(aux,"xori\0")||!strcmp(aux,"slti\0")||!strcmp(aux,"beq\0")||!strcmp(aux,"bne\0")||!strcmp(aux,"lw\0")||!strcmp(aux,"sw\0")) {
    tipo='i';
  }
  if(!strcmp(aux,"j\0")||!strcmp(aux,"jal\0")) {
  	tipo='j';
  }
}  
void ler_e_separa() {
  	char *p,teste[6],*ch;      
		ch=strchr(linha,':');
		if(!ch) {
		  sscanf(linha,"%s",teste);
	    } else {
	   	   sscanf(linha,"%s %s",teste,teste); 
	     }
		switch(tipo) {          
		  case 'r':
		  	if(!strcmp(teste,"jr\0")) {
		  	  if(!ch) {
		  	    sscanf(linha,"%s %[^#]",strings[0],strings[1]);	
		      } else {
		      	  sscanf(linha,"%s %s %[^#]",strings[0],strings[0],strings[1]);
			    }
			  sscanf(strings[1],"%s",strings[1]);
			} else {
				if(!ch) {
  	              sscanf(linha,"%s %s %s %[^#]",strings[0],strings[1],strings[2],strings[3]);
  	            } else {
  	            	sscanf(linha,"%s %s %s %s %[^#]",strings[0],strings[0],strings[1],strings[2],strings[3]);
  	            }
  	            sscanf(strings[3],"%s",strings[3]);
				p=strchr(strings[1],',');
  	            *(p)='\0';
  	            p=strchr(strings[2],',');
  	            *(p)='\0';
  	          }
  	        break;
  	      case 'i':
  	      	if(!strcmp(teste,"lw\0")||!strcmp(teste,"sw\0")) {
  	      	  if(!ch) {
  	      	  	sscanf(linha,"%s %[^)]",strings[0],strings[1]);
  	      	  } else {
  	      	  	  sscanf(linha,"%s %s %[^)]",strings[0],strings[0],strings[1]);
  	      	    }
                strcpy(strings[3],strings[1]);
                strcpy(strings[2],strings[1]);
                sscanf(strings[1],"%[^,]",strings[1]);
                sscanf(strings[1],"%s",strings[1]);
                p=strchr(strings[2],',');
                strcpy(strings[2],p+1);
                sscanf(strings[2],"%[^(]",strings[2]);
                sscanf(strings[2],"%s",strings[2]);
                p=strchr(strings[3],'(');
                strcpy(strings[3],p+1);
                sscanf(strings[3],"%s",strings[3]);
			} else {
				if(!ch) {
				  sscanf(linha,"%s %s %s %[^#]",strings[0],strings[1],strings[2],strings[3]);
				} else {
  	            	sscanf(linha,"%s %s %s %s %[^#]",strings[0],strings[0],strings[1],strings[2],strings[3]);
  	              }
  	              sscanf(strings[3],"%s",strings[3]);
  	              p=strchr(strings[1],',');
				  if(p) {
				    *(p)='\0';	
				  }
  	              p=strchr(strings[2],',');
  	              if(p) {
  	              	*(p)='\0'; 
			      }
			  }
			break;
		  case 'j':
		  	if(!ch) {
		  	  sscanf(linha,"%s %s",strings[0],strings[1]);
		    } else {
		    	sscanf(linha,"%s %s %s",strings[0],strings[0],strings[1]);
			  }
		  	break;
		  default:
		  	break;
		}

} 

void alocar_e_registrar_LABEL() {
	int z=0,y;
	char *ch;
	label=(char**)calloc(numLABEL,sizeof(char*));
	linhaLABEL=(int*)calloc(numLABEL,sizeof(int));
	
   for(z=0;z<numLABEL;z++) {
		label[z]=(char*)calloc(15,sizeof(char));
	}
	z=0;
	y=0;	
	rewind(arquivo);
	while(!feof(arquivo)) {
	  fgets(linha,sizeof(linha),arquivo);
	  ch=strchr(linha,':');
	  if(ch) {
	  	sscanf(linha,"%[^:]",label[z]);
	  	linhaLABEL[z]=y;
	  	z++;
	  }
	  y++;
   }
   
}

void transformar_em_binario() {
	int i=0,x,aux[16],r; 
	LABEL_bin=(char**)calloc(numLABEL,sizeof(char*));
	for(x=0;x<numLABEL;x++) {
	   LABEL_bin[x]=(char*)calloc(17,sizeof(char));
    }
    for(x=0;x<numLABEL;x++) {
	    for(i=15;i>=0;i--) {
           r=linhaLABEL[x]>>i;
           if(r&1) {
              aux[15-i]=1;
            } else {
                aux[15-i]=0;
              }
        }
        for(i=0;i<16;i++) {
           sprintf(&LABEL_bin[x][i],"%d",aux[i]);
        }
    }  

}

void retorne_binario_5bits(int num) {
  char vetor[6];
  int i=0,sinal=0,vet_bin[5],r;
  
  if(num<0) {
    num=-num;
  	sinal=1;
  }
 // Utiliza um número de 32 bits como base para a conversão.
 for(i=4;i>=0;i--) {
    // Executa a operação shift right até a 
    // última posição da direita para cada bit.
    r=num>>i;
    if(r&1) {
        vet_bin[4-i]=1;
    } else {
        vet_bin[4-i]=0;
    }
 }
  vet_bin[0]=sinal;
  for(i=0;i<5;i++) {
    sprintf(&vetor[i],"%d",vet_bin[i]);
  }
	strcat(binario,vetor);   	
}

void retorne_binario_16bits(int num) {
  char vetor[17];
  int i=0,sinal=0,vet_bin[16],r;
  
  if(num<0) {
    num=-num;
  	sinal=1;
  }
 for(i=15;i>=0;i--) {
    // Executa a operação shift right até a 
    // última posição da direita para cada bit.
    r=num>>i;
    if(r&1) {
        vet_bin[15-i]=1;
    } else {
        vet_bin[15-i]=0;
    }
 }
  vet_bin[0]=sinal;
  for(i=0;i<16;i++) {
    sprintf(&vetor[i],"%d",vet_bin[i]);
  }
	strcat(binario,vetor);   	
}

void endereco_em_BINARIO(char hexa[]) {
	int x;
	char endereco[17],c;
	for(x=0;x<4;x++) {
		c=hexa[x];
	 	if(!x) {
	 		if(c=='0') 
		        strcpy(endereco,"0000");
	 	    if(c=='1') 
		        strcpy(endereco,"0001");
		    if(c=='2') 
		        strcpy(endereco,"0010");
		    if(c=='3') 
		        strcpy(endereco,"0011");
		    if(c=='4') 
		        strcpy(endereco,"0100");
		    if(c=='5') 
		        strcpy(endereco,"0101");
		    if(c=='6') 
		        strcpy(endereco,"0110");
		    if(c=='7') 
		        strcpy(endereco,"0111");
		    if(c=='8') 
		        strcpy(endereco,"1000");
		    if(c=='9') 
		        strcpy(endereco,"1001");
			if(c=='A') 
		        strcpy(endereco,"1010");
		    if(c=='B') 
		        strcpy(endereco,"1011");
		    if(c=='C') 
		        strcpy(endereco,"1100");
		    if(c=='D') 
		        strcpy(endereco,"1101");
		    if(c=='E') 
		        strcpy(endereco,"1110");
		    if(c=='F') 
		        strcpy(endereco,"1111");
	   } else {
	   	    if(c=='0') 
		        strcat(endereco,"0000");
	   	    if(c=='1') 
		        strcat(endereco,"0001");
		    if(c=='2') 
		        strcat(endereco,"0010");
		    if(c=='3') 
		        strcat(endereco,"0011");
		    if(c=='4') 
		        strcat(endereco,"0100");
		    if(c=='5') 
		        strcat(endereco,"0101");
		    if(c=='6') 
		        strcat(endereco,"0110");
		    if(c=='7') 
		        strcat(endereco,"0111");
		    if(c=='8') 
		        strcat(endereco,"1000");
		    if(c=='9') 
		        strcat(endereco,"1001");
			if(c=='A') 
		        strcat(endereco,"1010");
		    if(c=='B') 
		        strcat(endereco,"1011");
		    if(c=='C') 
		        strcat(endereco,"1100");
		    if(c=='D') 
		        strcat(endereco,"1101");
		    if(c=='E') 
		        strcat(endereco,"1110");
		    if(c=='F') 
		        strcat(endereco,"1111"); 
	     }
	}
	strcat(binario,endereco);	
} 

void gerar_binario_INSTRUCAO() {
	char *p,teste[5],end[17];
	int x;
	long int aux;
	for(x=0;x<23;x++) {
	   if(!strcmp(strings[0],instrucao[x])) {
	   	  strcpy(binario,opcode[x]);
	   }	
	}
	switch(tipo) {
		case 'r':
			if(!strcmp(strings[0],"add\0")||!strcmp(strings[0],"sub\0")||!strcmp(strings[0],"and\0")||!strcmp(strings[0],"or\0")||!strcmp(strings[0],"xor\0")||!strcmp(strings[0],"slt\0")) {
				for(x=0;x<32;x++) {
					if(!strcmp(strings[2],registradores[x])) {
					   strcat(binario,numREG[x]);	
					}
				}
				for(x=0;x<32;x++) {
				    if(!strcmp(strings[3],registradores[x])) {
				    	strcat(binario,numREG[x]);
					}
				}
				for(x=0;x<32;x++) {
				    if(!strcmp(strings[1],registradores[x])) {
				    	strcat(binario,numREG[x]);
					}
				}
				strcat(binario,"00000");
			}
			if(!strcmp(strings[0],"jr\0")) {
				for(x=0;x<32;x++) {
					if(!strcmp(strings[1],registradores[x])) {
						strcat(binario,numREG[x]);
					}
				}
				strcat(binario,"00000");
				strcat(binario,"00000");
				strcat(binario,"00000");
		    }
		    if(!strcmp(strings[0],"sll\0")||!strcmp(strings[0],"srl\0")) {
		    	strcat(binario,"00000");
				for(x=0;x<32;x++) {
		    		if(!strcmp(strings[2],registradores[x])) {
		    			strcat(binario,numREG[x]);
					}
				}

				for(x=0;x<32;x++) {
		    		if(!strcmp(strings[1],registradores[x])) {
		    			strcat(binario,numREG[x]);
					}
				}
				sscanf(strings[3],"%li",&aux);
				retorne_binario_5bits(aux);
			}
		    for(x=0;x<32;x++) {
				if(!strcmp(strings[0],instrucao[x])) {
					strcat(binario,funct[x]);
				}
			}
			break;
		case 'i':
			if(!strcmp(strings[0],"addi\0")||!strcmp(strings[0],"slti\0")||!strcmp(strings[0],"andi\0")||!strcmp(strings[0],"ori\0")||!strcmp(strings[0],"xori\0")) {
				for(x=0;x<32;x++) {
					if(!strcmp(strings[2],registradores[x])) {
						strcat(binario,numREG[x]);
					}
				}
				for(x=0;x<32;x++) {
					if(!strcmp(strings[1],registradores[x])) {
						strcat(binario,numREG[x]);
					}
				}
				p=strstr(strings[3],"0x");
				if(!p) {
					sscanf(strings[3],"%li",&aux);
					retorne_binario_16bits(aux);
				} else {
					sscanf(&strings[3][2],"%s",teste);
					endereco_em_BINARIO(teste);
				}
			}
			if(!strcmp(strings[0],"lui\0")) {
				strcat(binario,"00000");
				for(x=0;x<32;x++) {
					if(!strcmp(strings[1],registradores[x])) {
						strcat(binario,numREG[x]);
					}
				}
				p=strstr(strings[2],"0x");
				if(!p) {
					sscanf(strings[2],"%li",&aux);
					retorne_binario_16bits(aux);
				} else {
					sscanf(&strings[2][2],"%s",teste);
					endereco_em_BINARIO(teste);
				}
			}
			if(!strcmp(strings[0],"beq\0")||!strcmp(strings[0],"bne\0")) {
				for(x=0;x<32;x++) {
					if(!strcmp(strings[2],registradores[x])) {
						strcat(binario,numREG[x]);
					}
				}
				for(x=0;x<32;x++) {
					if(!strcmp(strings[1],registradores[x])) {
						strcat(binario,numREG[x]);
					}
				}
				for(x=0;x<numLABEL;x++) {
					if(!strcmp(strings[3],label[x])) {
					   strcat(binario,LABEL_bin[x]);
				    }
				}
			}
			if(!strcmp(strings[0],"lw\0")||!strcmp(strings[0],"sw\0")) {
				for(x=0;x<32;x++) {
					if(!strcmp(strings[3],registradores[x])) {
						strcat(binario,numREG[x]);
					}
				}
				for(x=0;x<32;x++) {
					if(!strcmp(strings[1],registradores[x])) {
						strcat(binario,numREG[x]);
					}
				}
				sscanf(strings[2],"%li",&aux);
				retorne_binario_16bits(aux);
			}
			break;	
		case 'j':
			strcat(binario,"0000000000");			
			for(x=0;x<numLABEL;x++) {
				if(!strcmp(strings[1],label[x])) {
					strcat(binario,LABEL_bin[x]);
				}
			}
			break;
		default:
			break;
	}	
}

void gerar_HEXA() {
	char aux[8][5];
	int x,y=0;
	for(x=0;x<4;x++) {
		strcpy(hexa[x],"0x");
	}
	sscanf(binario,"%4s %4s %4s %4s %4s %4s %4s %4s",aux[7],aux[6],aux[5],aux[4],aux[3],aux[2],aux[1],aux[0]);
	for(x=0;x<8;x++,y++) {
		if(x==0||x==2||x==4||x==6) {
		  if(!strcmp(aux[x+1],"0000")) 
	   	    strcat(hexa[y],"0");
	   	  if(!strcmp(aux[x+1],"0001"))
	   	    strcat(hexa[y],"1");
	   	  if(!strcmp(aux[x+1],"0010")) 
	   	    strcat(hexa[y],"2");
	   	  if(!strcmp(aux[x+1],"0011"))
	   	    strcat(hexa[y],"3");
	   	  if(!strcmp(aux[x+1],"0100")) 
	   	    strcat(hexa[y],"4");
	   	  if(!strcmp(aux[x+1],"0101"))
	   	    strcat(hexa[y],"5");
	   	  if(!strcmp(aux[x+1],"0110")) 
	   	    strcat(hexa[y],"6");
	   	  if(!strcmp(aux[x+1],"0111"))
	   	    strcat(hexa[y],"7");  
	   	  if(!strcmp(aux[x+1],"1000"))
	   	    strcat(hexa[y],"8");  
	   	  if(!strcmp(aux[x+1],"1001"))
	   	    strcat(hexa[y],"9");  
	   	  if(!strcmp(aux[x+1],"1010")) 
	   	    strcat(hexa[y],"A");
	   	  if(!strcmp(aux[x+1],"1011"))
	   	    strcat(hexa[y],"B");
		  if(!strcmp(aux[x+1],"1100")) 
	   	    strcat(hexa[y],"C");
		  if(!strcmp(aux[x+1],"1101"))
	   	    strcat(hexa[y],"D");
		  if(!strcmp(aux[x+1],"1110"))
	   	    strcat(hexa[y],"E");     
	   	  if(!strcmp(aux[x+1],"1111"))
	   	    strcat(hexa[y],"F");
	   	  if(!strcmp(aux[x],"0000")) 
	   	    strcat(hexa[y],"0");
	   	  if(!strcmp(aux[x],"0001")) 
	   	    strcat(hexa[y],"1");
	   	  if(!strcmp(aux[x],"0010"))
	   	    strcat(hexa[y],"2");
	   	  if(!strcmp(aux[x],"0011"))
	   	    strcat(hexa[y],"3");
	   	  if(!strcmp(aux[x],"0100")) 
	   	    strcat(hexa[y],"4");
	   	  if(!strcmp(aux[x],"0101"))
	   	    strcat(hexa[y],"5");
	   	  if(!strcmp(aux[x],"0110")) 
	   	    strcat(hexa[y],"6");
	   	  if(!strcmp(aux[x],"0111"))
	   	    strcat(hexa[y],"7");  
	   	  if(!strcmp(aux[x],"1000"))
	   	    strcat(hexa[y],"8");  
	   	  if(!strcmp(aux[x],"1001"))
	   	    strcat(hexa[y],"9");  
	   	  if(!strcmp(aux[x],"1010"))
	   	    strcat(hexa[y],"A");
	   	  if(!strcmp(aux[x],"1011"))
	   	    strcat(hexa[y],"B");
		  if(!strcmp(aux[x],"1100"))
	   	    strcat(hexa[y],"C");
		  if(!strcmp(aux[x],"1101"))
	   	    strcat(hexa[y],"D");
		  if(!strcmp(aux[x],"1110")) 
	   	    strcat(hexa[y],"E");     
	   	  if(!strcmp(aux[x],"1111"))
	   	    strcat(hexa[y],"F");  
	   	y--;
	   }
	}	
    for(x=0;x<4;x++) {
      fprintf(destino, "0x%03X ", cont);
      fprintf(destino, "%s\n", hexa[x]);
      cont++;
	}
	
}

int main(int argc, char *argv[]) {
	char *h;
	if(argc==1) {
		printf("Entre com um argumento no minimo!\n");
		return 0;
	}
	if(argc==2) {
	  arquivo=fopen(argv[1],"r");
	  if(!arquivo) {
		printf("Erro ao abrir arquivo!\n");
		return 0;
	  }
	  destino=fopen("memoria.mif","a");
	  if(!destino) {
	  	printf("Erro ao abrir arquivo!\n");
	  	return 0;
	  }
	} 
	if(argc==3) {
	  arquivo=fopen(argv[1],"r");
	  if(!arquivo) {
		printf("Erro ao abrir arquivo!\n");
		return 0;
	  }
	  destino=fopen(argv[2],"a");
	  if(!destino) {
	  	printf("Erro ao abrir arquivo!\n");
	  	return 0;
	  }	
	}
	
	rewind(arquivo);
	while(!feof(arquivo)) {
    	fgets(linha,sizeof(linha),arquivo);
    	h=strchr(linha,':');
    	verifica_tipo_identifica_label();
        if(h) {
        	numLABEL++;
		}
        if(tipo!='l') {
          numlinhas++;	
		}
    }
    
    numlinhas+=1;	
    alocar_e_registrar_LABEL();
    transformar_em_binario();
    	
	strcpy(instrucao[0],"add\0");
	strcpy(instrucao[1],"addi\0");
	strcpy(instrucao[2],"sub\0");
	strcpy(instrucao[3],"sll\0");
	strcpy(instrucao[4],"srl\0");
	strcpy(instrucao[5],"lui\0");
	strcpy(instrucao[6],"and\0");
	strcpy(instrucao[7],"andi\0");
	strcpy(instrucao[8],"or\0");
	strcpy(instrucao[9],"ori\0");
	strcpy(instrucao[10],"xor\0");
	strcpy(instrucao[11],"xori\0");
	strcpy(instrucao[12],"slt\0");
	strcpy(instrucao[13],"slti\0");
	strcpy(instrucao[14],"beq\0");
	strcpy(instrucao[15],"bne\0");
	strcpy(instrucao[16],"mul\0");
	strcpy(instrucao[17],"div\0");
	strcpy(instrucao[18],"j\0");
	strcpy(instrucao[19],"jal\0");
	strcpy(instrucao[20],"jr\0");
	strcpy(instrucao[21],"lw\0");
	strcpy(instrucao[22],"sw\0");
	
	strcpy(opcode[0],"000000\0");
	strcpy(opcode[1],"001000\0");
	strcpy(opcode[2],"000000\0");
	strcpy(opcode[3],"000000\0");
	strcpy(opcode[4],"000000\0");
	strcpy(opcode[5],"001111\0");
	strcpy(opcode[6],"000000\0");
	strcpy(opcode[7],"001100\0");
	strcpy(opcode[8],"000000\0");
	strcpy(opcode[9],"001101\0");
	strcpy(opcode[10],"000000\0");
	strcpy(opcode[11],"001110\0");
	strcpy(opcode[12],"000000\0");
	strcpy(opcode[13],"001010\0");
	strcpy(opcode[14],"000100\0");
	strcpy(opcode[15],"000101\0");
	strcpy(opcode[16],"000000\0");
	strcpy(opcode[17],"000000\0");
	strcpy(opcode[18],"000010\0");
	strcpy(opcode[19],"000011\0");
	strcpy(opcode[20],"000000\0");
	strcpy(opcode[21],"100011\0");
	strcpy(opcode[22],"101011\0");
	
	strcpy(funct[0],"100000\0");
	strcpy(funct[2],"100010\0");
	strcpy(funct[3],"000000\0");
	strcpy(funct[4],"000010\0");
	strcpy(funct[6],"100100\0");
	strcpy(funct[8],"010101\0");
	strcpy(funct[10],"100110\0");
	strcpy(funct[12],"101010\0");
	strcpy(funct[16],"011000\0");
	strcpy(funct[17],"011010\0");
	strcpy(funct[20],"001000\0");
	
	strcpy(registradores[0],"$zero\0");
	strcpy(registradores[1],"$at\0");
	strcpy(registradores[2],"$v0\0");
	strcpy(registradores[3],"$v1\0");
	strcpy(registradores[4],"$a0\0");
	strcpy(registradores[5],"$a1\0");
	strcpy(registradores[6],"$a2\0");
	strcpy(registradores[7],"$a3\0");
	strcpy(registradores[8],"$t0\0");
	strcpy(registradores[9],"$t1\0");
	strcpy(registradores[10],"$t2\0");
	strcpy(registradores[11],"$t3\0");
	strcpy(registradores[12],"$t4\0");
	strcpy(registradores[13],"$t5\0");
	strcpy(registradores[14],"$t6\0");
	strcpy(registradores[15],"$t7\0");
	strcpy(registradores[16],"$s0\0");
	strcpy(registradores[17],"$s1\0");
	strcpy(registradores[18],"$s2\0");
	strcpy(registradores[19],"$s3\0");
	strcpy(registradores[20],"$s4\0");
	strcpy(registradores[21],"$s5\0");
	strcpy(registradores[22],"$s6\0");
	strcpy(registradores[23],"$s7\0");
	strcpy(registradores[24],"$t8\0");
	strcpy(registradores[25],"$t9\0");
	strcpy(registradores[26],"$k0\0");
	strcpy(registradores[27],"$k1\0");
	strcpy(registradores[28],"$gp\0");
	strcpy(registradores[29],"$sp\0");
	strcpy(registradores[30],"$fp\0");
	strcpy(registradores[31],"$ra\0");
	
	strcpy(numREG[0],"00000\0");
	strcpy(numREG[1],"00001\0");
	strcpy(numREG[2],"00010\0");
	strcpy(numREG[3],"00011\0");
	strcpy(numREG[4],"00100\0");
	strcpy(numREG[5],"00101\0");
	strcpy(numREG[6],"00110\0");
	strcpy(numREG[7],"00111\0");
	strcpy(numREG[8],"01000\0");
	strcpy(numREG[9],"01001\0");
	strcpy(numREG[10],"01010\0");
	strcpy(numREG[11],"01011\0");
	strcpy(numREG[12],"01100\0");
	strcpy(numREG[13],"01101\0");
	strcpy(numREG[14],"01110\0");
	strcpy(numREG[15],"01111\0");
	strcpy(numREG[16],"10000\0");
	strcpy(numREG[17],"10001\0");
	strcpy(numREG[18],"10010\0");
	strcpy(numREG[19],"10011\0");
	strcpy(numREG[20],"10100\0");
	strcpy(numREG[21],"10101\0");
	strcpy(numREG[22],"10110\0");
	strcpy(numREG[23],"10111\0");
	strcpy(numREG[24],"11000\0");
	strcpy(numREG[25],"11001\0");
	strcpy(numREG[26],"11010\0");
	strcpy(numREG[27],"11011\0");
	strcpy(numREG[28],"11100\0");
	strcpy(numREG[29],"11101\0");
	strcpy(numREG[30],"11110\0");
	strcpy(numREG[31],"11111\0");
	
	rewind(arquivo);
    while(!feof(arquivo)) {
    	fgets(linha,sizeof(linha),arquivo);
		verifica_tipo_identifica_label();
		if(tipo!='l') {
           ler_e_separa();
           gerar_binario_INSTRUCAO();
           gerar_HEXA();
        }
    }
    
    fclose(arquivo);
	fclose(destino);
	free(label);
	free(LABEL_bin);	
	free(linhaLABEL);
}

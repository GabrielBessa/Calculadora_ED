#include <stdio.h>
#include <stdlib.h>
#include "calc.h"
#include "char.h"
#include <string.h>
#include <math.h>

int read_expression(char* expressao){

	char operador;
	int i, flag_true;
	flag_true = 1;
	i = 0;
	t_pilha* pilha = aloca_pilha();
	while(expressao[i] != '\0' && flag_true == 1){
		if(expressao[i] == '(' || expressao[i] == '[' || expressao[i] == '{'){
			push(pilha, expressao[i]);
		}//Condicional para ver um operador de inicio
		if(expressao[i] == ')' || expressao[i] == ']' || expressao[i] == '}'){
			if(pilha_vazia(pilha)){
				flag_true = 0;
			}
			else{
				operador = (char) pop(pilha);
				 if(operador == '(' && expressao[i] != ')'){
				 	flag_true = 0;
				 }
				 if(operador == '[' && expressao[i] != ']'){
				 	flag_true = 0;
				 }
				 if(operador == '{' && expressao[i] != '}'){
				 	flag_true = 0;
				 }
			}
		}//Condicional para ver o operador de final
		i++;
	}
	
	if(flag_true == 0 || !pilha_vazia(pilha)){
		libera_pilha(pilha);
		pilha = NULL;
		return 0;
		//retornar pro menu e esvaziar a pilha
	}
	else{
		libera_pilha(pilha);
		pilha = NULL;
		return 1;
		//esvazia a pilha alocada e continua
	}
}

int prioridade(char* expressao, t_pilha* pilha, int counter){
	int prioridade;

	if(expressao[counter] == '+' || expressao[counter] == '-'){
		return 1;
	}
	if(expressao[counter] == '*' || expressao[counter] == '/'){
		return 2;
	}
	if(expressao[counter] == '\0'){
		return -1;
	}
	if(expressao[counter] == '('){
		return 5;
	}
	if(expressao[counter] == '['){
		return 4;
	}
	if(expressao[counter] == '{'){
		return 3;
	}
}

int infixa_to_posfixa(char* expressao) { 
	char str_aux[303]; //String auxiliar onde os valores da expressao serao colocados
	int prio_ant = 0; // flag para prioridade, para ver se a pilha deve ou não deve ser receber um elemento, ou desempilhar o seu topo
	t_pilha* pilha = aloca_pilha();  // aqui usamos o cabeçalho feito por mim 'char.h', para alocar uma pilha dinamicamente
	int i = 0, j = 0, k = 0;
	while(expressao[k] != '\0'){//Checa se a posicao recebeu algum caracter operador ou que modifica a ordem das operacoes
		if(expressao[i] != '+' && expressao[i] != '-'
			&&  expressao[i] != '*' &&  expressao[i] != '/' 
			&&  expressao[i] != '(' &&  expressao[i] != '['
			&&  expressao[i] != '{' &&  expressao[i] != ')'
			&&  expressao[i] != ']' &&  expressao[i] != '}'
			&& expressao[i] != '\0')
		{
			str_aux[j] = expressao[i];
			i++;
			j++;
			k++;
		}
		//condicional para os operandos
		else{
			if( (prioridade(expressao, pilha, i) == 1 && pilha_vazia(pilha)) || prioridade(expressao, pilha, i) == 2 && pilha_vazia(pilha) ){
				push(pilha, expressao[i]);
				prio_ant = prioridade(expressao, pilha, i);
				i++;
			}
			else{
				if(prioridade(expressao, pilha, i)  == 1 && !pilha_vazia(pilha)){
					if(prio_ant < prioridade(expressao, pilha, i)){
						push(pilha, expressao[i]);
						prio_ant = prioridade(expressao, pilha, i);
						i++;
					}
					else{
						if(prio_ant >= prioridade(expressao, pilha, i)){
							str_aux[j] = pop(pilha);
							push(pilha, expressao[i]);
							i++;
							k++;
							j++;
						}
					}
				}
				else{
					if(prioridade(expressao, pilha, i) == 2 && !pilha_vazia(pilha)){
						if(prio_ant < prioridade(expressao, pilha, i)){
							push(pilha, expressao[i]);
							prio_ant = prioridade(expressao, pilha, i);
							i++;
						}
						else{
							if(prio_ant >= prioridade(expressao, pilha, i)){
								str_aux[j] = pop(pilha);
								push(pilha, expressao[i]);
								i++;
								k++;
								j++;
							}
						}
					}
					else{
						if(prioridade(expressao, pilha, i) == -1 && !pilha_vazia(pilha)){
							str_aux[j] = pop(pilha);
							j++;
							k++;
						}
					}
				}
			}
			//Condicional para os parenteses, colchetes e chaves:
			if( (prioridade(expressao, pilha, i) == 3 && pilha_vazia(pilha)) || (prioridade(expressao, pilha, i) == 4 && pilha_vazia(pilha)) || (prioridade(expressao, pilha, i) == 5 && pilha_vazia(pilha)) )
			{
				push(pilha, expressao[i]);
				i++;
			}
			else{
				if( (prioridade(expressao, pilha, i) == 5 && !pilha_vazia(pilha)) || (prioridade(expressao, pilha, i) == 4 && !pilha_vazia(pilha)) || (prioridade(expressao, pilha, i) == 3 && !pilha_vazia(pilha)) ){
					push(pilha, expressao[i]);
					i++;
					prio_ant = 0;
				}
				else{
					if(expressao[i] == ')' || expressao[i] == ']' || expressao[i] == '}'){
						i++;
						while(!pilha_vazia(pilha)){
							if((char) pilha->topo->dado == '(' || (char) pilha->topo->dado == '[' || (char) pilha->topo->dado == '{'){
								pop(pilha);
							}
							else{
								str_aux[j] = pop(pilha);
								j++;
								k++;
							}
						}

						if(expressao[i] == '\0'){
							break;
						}
					}
				}
			}
		}
	}
	str_aux[j] = '\0'; /*Como a string tem tamanho definido, eh normal a expressao muitas vezes ser menor que a string em si, portanto, se ela for,
						*a string eh finalizada, e o que foi alocado estaticamente eh devolvido a memoria*/
	printf("Posfixa: %s\n\n", str_aux);
	libera_pilha(pilha); //Liberamos a pilha alocada
	pilha = NULL; //Aqui temos certeza que a pilha nao existe mais
	free(expressao);//No final liberamos a expressao que alocamos dinamicamente anteriormente
	return 1;
}

int modo_calc(){
	t_pilha* pilha = aloca_pilha();
	char check[100];
	float numero_final, mult, operacao_pilha = 0;
	int i = 0, size = 0, dec = 0, counter_dec = 0, size_mult_dec , size_mult_int;
	printf("Modo Calculadora:\n");
	if(pilha_vazia(pilha)){
		printf("Pilha Vazia!\n");
	}

	printf("Informe um valor('0' para sair ou pressione 'space'): ");
	scanf("%[^\n]s", check);
	getchar();
		for(i = 0; i <= strlen(check); i++){
			if((check[0] == ' ' || check[0] == '0') && strlen(check) == 1){
				printf("Usuario digitou space ou 0\n");
				libera_pilha(pilha);
				pilha = NULL;
				return 100;
			}
		}			

	while(check[0] == '+' || check[0] == '-' || check[0] == '*' || check[0] == '/'){
		printf("Numero insuficiente de operandos!\n");
		printf("Informe um valor('0' ou space para sair): ");
		scanf("%[^\n]s", check);
		getchar();
		if((check[0] == ' ' || check[0] == '0') && strlen(check) == 1){
			printf("Usuario digitou space ou 0\n");
			libera_pilha(pilha);
			pilha = NULL;
			return 100;
		}
	}


	for(i = 0; i < strlen(check); i++){
		if(check[i] == '.'){
			dec = 1; 
			break;
		}
		else{
			counter_dec++;
		}
	}
	numero_final = 0;
	size_mult_int = counter_dec -1;
	size_mult_dec = ((counter_dec+1) - strlen(check));
	//Transformacao de char pra double:
	for(i = 0; i < strlen(check); i++){
		if(check[i] != '.'){
			if(size_mult_int >= 0){
				mult = pow(10, size_mult_int);
				size_mult_int--;
			}
			else{
				mult = pow(10, size_mult_dec);
				size_mult_dec--;
			}
			numero_final += ((float)check[i] - 48) * mult;
		}
	}

	while(check[0] != '0'){
		if(check[0] != '+' && check[0] != '-' && check[0] != '*' && check[0] != '/' && check[1] != '!'){
			push(pilha, numero_final);
			size++;
			printf("Topo: ");
			print_pilha(pilha);
		}
		numero_final = 0;
		mult = 0;
		counter_dec = 0;
		printf("Informe um valor('0' ou 'space' para sair), ou um operador: ");
		scanf("%[^\n]s", check);
		getchar();
		if((check[0] == '0' || check[0] == ' ') && strlen(check) == 1){
			printf("O usuario encerrou.\n");
			libera_pilha(pilha);
			pilha = NULL;
			return 100;
		}

		if(size < 2 && ((check[0] == '+') || (check[0] == '-') || (check[0] == '*') || (check[0] == '/')) ){
			printf("Numero de operandos insuficientes!\n");
			do
			{	
				printf("Informe um valor('0' ou 'space' para sair): ");
				scanf("%[^\n]s", check);
				getchar();
				if(check[0] == '0' || check[0] == ' '){
					printf("O usuario encerrou.\n");
					libera_pilha(pilha);
					return 100;
				}
			} while (check[0] == '+' || check[0] == '-' || check[0] == '*' || check[0] == '/');
		}

		if(check[0] != '+' && check[0] != '-' && check[0] != '*' && check[0] != '/' && check[0] != 'c' ){
				for(i = 0; i < strlen(check); i++){
				if(check[i] == '.'){
					dec = 1; 
					break;
				}
				else{
					counter_dec++;
				}
			}
			size_mult_int = counter_dec -1;
			size_mult_dec = ((counter_dec+1) - strlen(check));
			//Transformacao de char pra double:
			for(i = 0; i < strlen(check); i++){
				if(check[i] != '.'){
					if(size_mult_int >= 0){
						mult = pow(10, size_mult_int);
						size_mult_int--;
					}
					else{
						mult = pow(10, size_mult_dec);
						size_mult_dec--;
					}
					numero_final += ((float)check[i] - 48) * mult;
				}
			}
		}
		else{
			if(check[0] == '+' && check[1] != '!'){
				operacao_pilha = pop(pilha);
				operacao_pilha += pilha->topo->dado;
				pilha->topo->dado = operacao_pilha;
				print_pilha(pilha);
				size--;
				operacao_pilha = 0;
			}
			if(check[0] == '-' && check[1] != '!'){
				operacao_pilha = pop(pilha);
				operacao_pilha -= pilha->topo->dado;
				pilha->topo->dado = operacao_pilha;
				print_pilha(pilha);
				size--;
				operacao_pilha = 0;
			}
			if(check[0] == '*' && check[1] != '!'){
				operacao_pilha = pop(pilha);
				operacao_pilha *= pilha->topo->dado;
				pilha->topo->dado = operacao_pilha;
				print_pilha(pilha);
				size--;
				operacao_pilha = 0;
			}
			if(check[0] == '/'){
				operacao_pilha = pop(pilha);
				operacao_pilha /= pilha->topo->dado;
				pilha->topo->dado = operacao_pilha;
				print_pilha(pilha);
				size--;
				operacao_pilha = 0;
			}
			if(check[0] == '+' && check[1] == '!'){
				while(!pilha_vazia(pilha)){
					operacao_pilha += pop(pilha);
					size = 0;
				}
				push(pilha, operacao_pilha);
				size++;
				printf("Topo: ");
				print_pilha(pilha);
				operacao_pilha = 0;
			}
			if(check[0] == '-' && check[1] == '!'){
				operacao_pilha += pop(pilha);
				while(!pilha_vazia(pilha)){
					operacao_pilha -= pop(pilha);
					size = 0;
				}
				push(pilha, operacao_pilha);
				size++;
				printf("Topo: ");
				print_pilha(pilha);
				operacao_pilha = 0;
			}
			if(check[0] == '*' && check[1] == '!'){
				operacao_pilha += pop(pilha);
				while(!pilha_vazia(pilha)){
					operacao_pilha *= pop(pilha);
					size = 0;
				}
				push(pilha, operacao_pilha);
				size++;
				printf("Topo: ");
				print_pilha(pilha);
				operacao_pilha = 0;
			}
		}
	}
	return 100;
}


int menu(){
	int mode;
	printf("1- Modo Expressao\n");
	printf("2- Modo Calculadora\n");
	printf("0- Para sair\n");
	printf("Escolha um modo: ");
	scanf("%d", &mode);
	getchar();
	return mode;
}

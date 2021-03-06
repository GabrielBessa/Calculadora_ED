#ifndef __CALC_H__
#define __CALC_H__
#include "char.h"
#include <math.h>

int read_expression(char* expressao);
int infixa_to_posfixa(char* expressao);
int prioridade(char* expressao, t_pilha* pilha, int counter);
int modo_calc();
int menu();

#endif
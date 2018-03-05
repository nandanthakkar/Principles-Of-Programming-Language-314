/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Spring 2017                              *
 *  Author: Ulrich Kremer                    *
 *  Student Version                          *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

Instruction * head;

void eliminateDeadCode(){
	int start = 0;
	Instruction * ptr = head->next;
	Instruction * prev = head;
	while(prev != NULL){
		if(prev-> critical != 'c'){
			if(start == 0){
				free(prev);
				head = ptr;
				head-> prev = NULL;
				prev = ptr;
				ptr = ptr->next;
			}
			else{
				prev->prev->next = ptr;
				if(ptr != NULL) {ptr->prev = prev->prev;}
				free(prev);
				prev = ptr;
				if(ptr != NULL) {ptr = ptr->next;}
			}
		}
		else{
			start = 1;
			prev = ptr;
			if(ptr != NULL)ptr = ptr->next;
		}
	}
}

void optimize(Instruction * instruct){
	Instruction * temp = instruct;
	
	switch(instruct->opcode){
		case OUTPUTAI:
			while(temp->prev != NULL){
				temp = temp->prev;
				if(temp->opcode == STOREAI && temp->field2 == instruct->field1 && temp->field3 == instruct->field2 ){	
					temp->critical = 'c';
					optimize(temp);
					break;
				}
			}
			return;
		case STOREAI:
			while(temp->prev != NULL ){
				temp = temp->prev; 
				if((temp->opcode != OUTPUTAI && temp->opcode != STOREAI && temp->field3 == instruct->field1) || (temp->opcode == LOADI && temp->field2 == instruct->field1)){
					temp->critical = 'c';
					optimize(temp);
					break;
				}
			}
			return;
		case LOADAI:
			while(temp->prev != NULL){
				temp =  temp->prev; 
				if(temp->opcode == STOREAI && temp->field2 == instruct->field1 && temp->field3 == instruct->field2){
					temp->critical = 'c';
					optimize(temp);
					break;
				}
			}
			return;
		case ADD:
		case SUB:
		case MUL:
		case DIV:
			while(temp->prev != NULL){
                                temp =  temp->prev; int i= 0; int j=0;
                                if(temp->opcode != STOREAI && temp->opcode != OUTPUTAI){
                                        if((temp->opcode == LOADI)){
						if(temp->field2 == instruct->field1 && i == 0){
							temp->critical = 'c'; i = 1;
						}
						if(temp->field2 == instruct->field2 && j == 0){
							temp->critical = 'c'; j = 1;
						}
						if(i == 1 && j == 1){ break;} 
					}
					else{
						if(temp->field3 == instruct->field1 && i == 0){
							i = 1;
							temp->critical = 'c';
							optimize(temp);
						}
						if(temp->field3 == instruct->field2 && j == 0){
                                                        j = 1;
							temp->critical = 'c';
                                                        optimize(temp);
                                                }
					}
                                }
				if(i == 1 && j == 1){ break;}
                        }
                        return;
		case LOADI:
			instruct->critical = 'c';
			return;
	}
	return;
}

int main()
{

	head = ReadInstructionList(stdin);
	if (!head) {
		WARNING("No instructions\n");
		exit(EXIT_FAILURE);
	}
    
	/* YOUR CODE GOES HERE */
	
	Instruction * ptr = head;
	while(ptr != NULL){
		if(ptr->opcode == OUTPUTAI){
			ptr->critical = 'c';
		}
		ptr = ptr->next;
	}
	ptr = head;

	while(ptr != NULL){
		if(ptr->critical == 'c'){
			optimize(ptr);
		}
		ptr = ptr->next;
	}

	head-> critical = 'c';
	eliminateDeadCode();
    	
	if (head) 
		PrintInstructionList(stdout, head);
	
	return EXIT_SUCCESS;
}

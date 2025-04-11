/**
 * @author Marios Nikolaou
 * @brief this class is responsible for the extension of a chemical formula
 * 
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"stack.h"
#include"formulaProcessor.h"


int validateParentheses(const char *formula) {
    Stack *stack = initStack();

    for (int i = 0; i < strlen(formula); i++) {
        if (formula[i] == '(') {
            // Push opening parenthesis to stack
            push("(",stack);
        } else if (formula[i] == ')') {
            // If stack is empty or top is not an opening parenthesis, return failure
            if (IsEmptyStack(stack)) {
                //freeStack(stack);
                return 0; // Unbalanced
            }
            pop2(stack); // Pop the matching '('
        }
    }

    // Check if stack is empty at the end
    int balanced = IsEmptyStack(stack);
    //freeStack(stack);
    return balanced;
}

// Function to expand the chemical formula
int expandFormula(Stack* stack, char* formula) {
    int length = strlen(formula);
    char **str=(char**)malloc(length*sizeof(char *));
    if(str==NULL){
        printf("no more space");
        return EXIT_FAILURE;
    }
    for (int i = 0; i < length; i++) {
        str[i] = malloc(2 * sizeof(char));  // Allocate space for one character + null terminator
        if (str[i] == NULL) {
            printf("Memory allocation failed\n");
            free(str);
            return EXIT_FAILURE;
        }
        str[i][0] = formula[i];  // Copy the character
        str[i][1] = '\0';        // Null terminator
    }
    for(int i=0;i<length;i++){
        if(isupper(str[i][0])){
            push(str[i],stack);
        }
        else if(islower(str[i][0])&&(stack->top != NULL && stack->top->data != NULL)){
            strcat(stack->top->data,str[i]);
        }
        else if(str[i][0]=='('||str[i][0]==')'){
            push(str[i],stack);
        }
        else if(isdigit(str[i][0])&&((stack->top->data[0])!=')')&&(stack->top != NULL && stack->top->data != NULL)){
            char *Snum=malloc(100*sizeof(char));
            strcpy(Snum,str[i]);
            while(isdigit(str[i+1][0])){
                strcat(Snum,str[i+1]);
                i++;
                if(i==length-1)
                    break;
            }
            int num=atoi(Snum);
            for(int i=0;i<num-1;i++){
                push(stack->top->data,stack);
            }       
        }
        else if(isdigit(str[i][0])&&((stack->top->data[0])==')')){
            char *Snum2=malloc(100*sizeof(char));
            strcpy(Snum2,str[i]);
            while(isdigit(str[i+1][0])){
                strcat(Snum2,str[i+1]);
                i++;
                if(i==length-1)
                    break;
            }
            int num2=atoi(Snum2);
            int size=stack->size;
            char **retval=(char**)malloc(size*sizeof(char *));
            if(retval==NULL){
                printf("memory problem\n");
                return EXIT_FAILURE;
            }
            int count=0;
            pop2(stack);
            while (stack->top->data[0] != '(') {
            if (pop(stack, &retval[count]) == EXIT_SUCCESS) {
                count++;
            } else {
                printf("Failed to pop from stack\n");
                break; // Exit if pop fails
                }
            }
            pop2(stack);
            for(int j=0;j<num2;j++){
            for(int k=count-1;k>=0;k--){
                push(retval[k],stack);
            }
            }
            
        }
    }
    
    return EXIT_SUCCESS;
}

int getProtonNumber(char *element,char *arrc[MAX_ELEMENTS],int arri[MAX_ELEMENTS]) {
    for (int i = 0; i < MAX_ELEMENTS; i++) {
        if (strcmp(element, arrc[i]) == 0) {
            return arri[i];
        }
    }
    return EXIT_FAILURE;  // Return 0 if not found
}

int calculateProtons(Stack *stack,char *arrc[MAX_ELEMENTS],int arri[MAX_ELEMENTS]) {
    Node *p=stack->top;
    int sum=0;
    while(p!=NULL){
        sum+=getProtonNumber(p->data,arrc,arri);
        p=p->next;
    }
    return sum;
}

#ifdef RUN_MAINfp // fp = formulaProcessor

    int main() {
    // Test `validateParentheses`
    const char *formula1 = "(H2O)";
    const char *formula2 = "H2(O(H))";
    const char *formula3 = "(H2(O)"; // unbalanced
    
    printf("Testing validateParentheses...\n");
    printf("Formula %s: %s\n", formula1, validateParentheses(formula1) ? "Balanced" : "Unbalanced");
    printf("Formula %s: %s\n", formula2, validateParentheses(formula2) ? "Balanced" : "Unbalanced");
    printf("Formula %s: %s\n", formula3, validateParentheses(formula3) ? "Balanced" : "Unbalanced");
    printf("\n");

    // Test `expandFormula`
    Stack *stack = initStack();
    char formulaToExpand[] = "H2O";
    printf("Testing expandFormula with formula %s...\n", formulaToExpand);
    if (expandFormula(stack, formulaToExpand) == EXIT_SUCCESS) {
        printf("Expanded stack content: ");
        printStack(stack);
    } else {
        printf("Failed to expand formula %s\n", formulaToExpand);
    }
    printf("\n");

    // Test `getProtonNumber` and `calculateProtons`
    char *elements[MAX_ELEMENTS] = {"H", "O", "C", "N", "Na", "Cl", "K", "Ca", "Mg", "Fe"};
    int protons[MAX_ELEMENTS] = {1, 8, 6, 7, 11, 17, 19, 20, 12, 26};

    printf("Testing getProtonNumber...\n");
    printf("Proton number of H: %d\n", getProtonNumber("H", elements, protons));
    printf("Proton number of O: %d\n", getProtonNumber("O", elements, protons));
    printf("Proton number of Cl: %d\n", getProtonNumber("Cl", elements, protons));
    printf("\n");

    // Push elements onto stack for proton calculation
    Stack *stack2 = initStack();
    push("H", stack2);
    push("O", stack2);
    push("O", stack2); // Represents "H2O"

    printf("Testing calculateProtons for formula H2O...\n");
    int totalProtons = calculateProtons(stack, elements, protons);
    printf("Total protons in H2O: %d\n", totalProtons);
    
    return 0;
}
#endif

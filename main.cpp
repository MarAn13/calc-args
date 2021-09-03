#include <stdio.h> // needed for printf()
#include <math.h> // needed for pow() and atof()
#include <malloc.h> // needed for dynamic array allocation
//VERSION 1.2
//fixed ^ and C++ -> C
void push(char* argv[], int size, int* current, char element[]);
void push(double argv[], int size, int* current, double element);
char* pop(char* argv[], int size, int* current);
double popper(double argv[], int size, int* current);
double calc(char* argv[], int argc);
bool check(char* argv[], int argc);

//input format:
//expression input is performed via command line parameters
//please enter your expression with spaces
//example:
//1 + 2 + 3 / 5 * 8 ^ 2 + 5 ^ ( 6 / 3 ) + -1.5 - -5.6 * -2.4 / -1.2 ^ ( -6 / -3 ), also works with double
//also, if the power is calculated by a complex expression, then put it in brackets 8 ^ 3 , but 8 ^ ( 8 / 2 )
//thank you for using our calculator

int main(int argc, char* argv[]) {
	if (argc > 2 && check(argv, argc)) {
		printf("%s %f\n", "Result:", calc(argv, argc));
	}
	else printf("%s\n", "Problem occurred, please check your input");

	return 0;
}

bool check(char* argv[], int argc) { //checking mistakes in input
	int optBrakes = 0; //variable to control number of elements
	int optSign = 1; //variable to control number of elements
	for (int i = 1; i < argc; i++) {
		if (*argv[i] == '(')optBrakes++;
		if (*argv[i] == ')')optBrakes--;
		if (argv[i][1] == '\0') {
			if (*argv[i] != '(' && *argv[i] != ')' && *argv[i] != '+' && *argv[i] != '-' && *argv[i] != '/' && *argv[i] != '*' && *argv[i] != '^') {
				optSign--;
			}
			if (*argv[i] == '+' || *argv[i] == '-' || *argv[i] == '/' || *argv[i] == '*' || *argv[i] == '^') {
				optSign++;
			}
		}
		else {
			int size = sizeof(argv[i]);//get the size of an array element
			int signExp = 0;//variable to control number of elements in array element
			int dotExp = 0;//variable to control number of elements in array element
			for (int j = 0; j < size - 1; j++) {
				if (argv[i][j] == '+' || argv[i][j] == '-')signExp++;
				if (argv[i][j] == '.')dotExp++;
				if (argv[i][j] == '/' || argv[i][j] == '*' || argv[i][j] == '^' || signExp >= 2 || dotExp >= 2)return false;
			}
			optSign--;
		}
		if (optSign < 0 || optSign > 1)return false;
		if (optBrakes < 0)return false;
	}
	if (optBrakes != 0 || optSign != 0)return false;
	return true;
}

void push(char* argv[], int size, int* current, char element[]) {//pushes element in array,like stack push method
	if (*current < size) {
		argv[*current] = element;
		(*current)++;
	}
	else printf("%s", "Push error");
}

void push(double argv[], int size, int* current, double element) {//pushes element in array,like stack push method
	if (*current < size) {
		argv[*current] = element;
		(*current)++;
	}
	else printf("%s", "Push error");
}

char* pop(char* argv[], int size, int* current) {//pop element from array,like stack pop method
	if (*current >= 0) {
		(*current)--;
	}
	else printf("%s", "Pop error");
	return argv[*current];
}

double popper(double argv[], int size, int* current) {//pop element from array,like stack pop method
	if (*current >= 0) {
		(*current)--;
	}
	else printf("%s", "Pop error");
	return argv[*current];
}

double calc(char* argv[], int argc) {//calculates input
	//INFIX->POSTFIX
	int size = argc - 1; //size of an array without 1 element

	char** operands = (char**)calloc(size, sizeof(char*));//array with postfix result
	char** operations = (char**)calloc(size, sizeof(char*));//array needed in infix->postfix process
	int currentOperands = 0;//number of elements in operands
	int currentOperations = 0;//number of elements in operations
	//displays input expression
	printf("%s ", "Expression entered:");
	for (int i = 1; i < argc; i++) {
		printf("%s ", argv[i]);
	}
	printf("\n");
	for (int i = 1; i < argc; i++) {
		if (argv[i][1] == '\0') {
			switch (*argv[i]) {
			case ('('):
				push(operations, size, &currentOperations, argv[i]);
				break;
			case (')'):
				while (*operations[currentOperations - 1] != '(') {
					push(operands, size, &currentOperands, pop(operations, size, &currentOperations));
				}
				pop(operations, size, &currentOperations);
				break;
			case ('+'):
				if (currentOperations != 0 && *operations[currentOperations - 1] != '(') {
					if (*operations[currentOperations - 1] == '^') {
						while (currentOperations != 0 && *operations[currentOperations - 1] != '(') {
							push(operands, size, &currentOperands, pop(operations, size, &currentOperations));
						}
						push(operations, size, &currentOperations, argv[i]);
					}
					else {
						push(operands, size, &currentOperands, pop(operations, size, &currentOperations));
						push(operations, size, &currentOperations, argv[i]);
					}
				}
				else push(operations, size, &currentOperations, argv[i]);
				break;
			case ('-'):
				if (currentOperations != 0 && *operations[currentOperations - 1] != '(') {
					if (*operations[currentOperations - 1] == '^') {
						while (currentOperations != 0 && *operations[currentOperations - 1] != '(') {
							push(operands, size, &currentOperands, pop(operations, size, &currentOperations));
						}
						push(operations, size, &currentOperations, argv[i]);
					}
					else {
						push(operands, size, &currentOperands, pop(operations, size, &currentOperations));
						push(operations, size, &currentOperations, argv[i]);
					}
				}
				else push(operations, size, &currentOperations, argv[i]);
				break;
			case ('*'):
				if (currentOperations != 0 && (*operations[currentOperations - 1] == '*' || *operations[currentOperations - 1] == '/' || *operations[currentOperations - 1] == '^')) {
					if (*operations[currentOperations - 1] == '^') {
						while (currentOperations != 0 && *operations[currentOperations - 1] != '(' && *operations[currentOperations - 1] != '+' && *operations[currentOperations - 1] != '-') {
							push(operands, size, &currentOperands, pop(operations, size, &currentOperations));
						}
						push(operations, size, &currentOperations, argv[i]);
					}
					else {
						push(operands, size, &currentOperands, pop(operations, size, &currentOperations));
						push(operations, size, &currentOperations, argv[i]);
					}
				}
				else push(operations, size, &currentOperations, argv[i]);
				break;
			case ('/'):
				if (currentOperations != 0 && (*operations[currentOperations - 1] == '*' || *operations[currentOperations - 1] == '/' || *operations[currentOperations - 1] == '^')) {
					if (*operations[currentOperations - 1] == '^') {
						while (currentOperations != 0 && *operations[currentOperations - 1] != '(' && *operations[currentOperations - 1] != '+' && *operations[currentOperations - 1] != '-') {
							push(operands, size, &currentOperands, pop(operations, size, &currentOperations));
						}
						push(operations, size, &currentOperations, argv[i]);
					}
					else {
						push(operands, size, &currentOperands, pop(operations, size, &currentOperations));
						push(operations, size, &currentOperations, argv[i]);
					}
				}
				else push(operations, size, &currentOperations, argv[i]);
				break;
			case ('^'):
				if (currentOperations != 0 && (*operations[currentOperations - 1] == '^')) {
					push(operands, size, &currentOperands, pop(operations, size, &currentOperations));
					push(operations, size, &currentOperations, argv[i]);
				}
				else push(operations, size, &currentOperations, argv[i]);
				break;
			default:
				push(operands, size, &currentOperands, argv[i]);
			}
		}
		else {
			push(operands, size, &currentOperands, argv[i]);
		}
		if (i == argc - 1) {
			while (currentOperations != 0) {
				push(operands, size, &currentOperands, pop(operations, size, &currentOperations));
			}
		}
	}
	free(operations);//remove no longer needed array
	//POSTFIX->RESULT
	double* solve = (double*)calloc(size, sizeof(double));//array for arithmetic conversions
	int currentSolve = 0;//number of elements in solve
	double result, var1, var2 = 0;//variables for arithmetic conversions
	//displays postfix expression
	printf("%s ", "Expression in reverse polish notation:");
	for (int i = 0; i < currentOperands; i++) {
		printf("%s ", operands[i]);
	}
	printf("\n");
	for (int i = 0; i < currentOperands; i++) {
		if (operands[i][1] == '\0') {
			switch (*operands[i]) {
			case ('+'):
				var2 = popper(solve, size, &currentSolve);
				var1 = popper(solve, size, &currentSolve);
				result = (var1 + var2);
				push(solve, size, &currentSolve, result);
				break;
			case ('-'):
				var2 = popper(solve, size, &currentSolve);
				var1 = popper(solve, size, &currentSolve);
				result = (var1 - var2);
				push(solve, size, &currentSolve, result);
				break;
			case ('*'):
				var2 = popper(solve, size, &currentSolve);
				var1 = popper(solve, size, &currentSolve);
				result = (var1 * var2);
				push(solve, size, &currentSolve, result);
				break;
			case ('/'):
				var2 = popper(solve, size, &currentSolve);
				var1 = popper(solve, size, &currentSolve);
				result = (var1 / var2);
				push(solve, size, &currentSolve, result);
				break;
			case ('^'):
				var2 = popper(solve, size, &currentSolve);
				var1 = popper(solve, size, &currentSolve);
				result = pow(var1, var2);
				push(solve, size, &currentSolve, result);
				break;
			default:
				push(solve, size, &currentSolve, atof(operands[i]));
			}
		}
		else {
			push(solve, size, &currentSolve, atof(operands[i]));
		}
	}
	if (solve != NULL)result = *solve;
	else result = 0;
	free(operands), free(solve);//remove no longer needed arrays
	return result;
}
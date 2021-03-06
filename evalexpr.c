#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	char *text;
	struct node *lptr;
	struct node *rptr;
} nodet;

typedef struct arrnode {
	char operator;
	int number;
} arrnode_t;

int ft_strlen(char argument[])
{
	//Find the length of the character array/string "argument"
	int i = 0;
	while (argument[i] != '\0')
		i++;
	return (i);
}

int isnegnum (char argv[], int negindex)
{
	//Given a string, and the index number of the '-' sign, this function returns 1 if the symbol means the # after the sign is negative, and 0 if it is
	//a subtraction sign.
	if (negindex == 0)
		return (1); //if first char, then it is neg number
	negindex--;
	while (argv[negindex] == ' ' && negindex > 0)
		negindex--;
	if (argv[negindex] == ' ' && negindex == 0)
		return (1);
	if (argv[negindex] == '+' || argv[negindex] == '-' || argv[negindex] == '*' || argv[negindex] == '/' || 
			argv[negindex] == '%' || argv[negindex] == '(') //left paren means neg but not right paren (i.e. (3 + 4) - 5)
		return (1);
	else //if not any of those symbols or a space, then it is a number or the ')' sign, so it is a subtraction sign
		return (0);
}
int findheadindex (char argv[], int leftptr, int rightptr)
{
	int maxindex = -1;
	int leftptrtemp = leftptr;
	//int rightptrtemp = rightptr;
	int leftparencounter = 0;
	int rightparencounter = 0;
	while (leftptrtemp != rightptr)
	{
		if (argv[leftptrtemp] == '(')
			leftparencounter += 1; //do leftptr++?
		if (argv[leftptrtemp] == ')')
			rightparencounter += 1;
		if (rightparencounter == leftparencounter)
		{
			if (argv[leftptrtemp] == '+' || (argv[leftptrtemp] == '-' && isnegnum(argv, leftptrtemp) == 0))
				maxindex = leftptrtemp;
			else if (maxindex < 0) //only if no positive or negative symbols got there first
			{
				if (argv[leftptrtemp] == '*' || argv[leftptrtemp] == '/' || argv[leftptrtemp] == '%')
					maxindex = leftptrtemp;
			}
			else if ((argv[maxindex] != '+' && argv[maxindex] != '-') && (argv[leftptrtemp] == '*' || 
					argv[leftptrtemp] == '/' || argv[leftptrtemp] == '%')) //is this necessary?
				maxindex = leftptrtemp;
		}
		leftptrtemp++;
	}
	if (maxindex != -1)
		return (maxindex);
	while (argv[rightptr] != ')' && rightptr != leftptr)
	{
		/*if (argv[rightptr] == '+' || argv[rightptr] == '-')
			return (rightptr);
		if (argv[rightptr] == '*' || argv[rightptr] == '/' || argv[rightptr] == '%')
			maxindex = rightptr;*/
		rightptr--;
	} //redundant?
	if (rightptr == leftptr) //meaning that there are no more parentheses, we want to not break into futher nodes
		return (-1);
	while (argv[leftptr] != '(')
	{
		leftptr++;
	}
	return (findheadindex(argv, leftptr + 1, rightptr - 1));

}

int isoperator (char argv[])
{
	//Returns 1 if there an operator in the string, and 0 if there are no operators
	int i = -1;
	int len = ft_strlen(argv);
	while (++i < len)
	{
		if (argv[i] == '+' || argv[i] == '-' || argv[i] == '*' || argv[i] == '/' || argv[i] == '%')
			return (1);
	}
	return (0);
}

void leftportion (nodet *headptr, int opindex)
{
	//Puts the subexpression on the left side of the operator we hook expression on into the text of a new node in the headptr->lptr.
	int startindex = opindex - 1;
	int lparencounter = 0;
	int rparencounter = 0;
	int index; //index of array we are inserting into
	nodet* nodeptr;
	headptr->lptr = nodeptr = malloc(sizeof(nodet));
	headptr->lptr->lptr = NULL;
	headptr->lptr->rptr = NULL;
	nodeptr->text = malloc(sizeof(char) * (opindex + 1));
	while (startindex > 0)
	{
		if (headptr->text[startindex] == '(')
			lparencounter++;
		if (headptr->text[startindex] == ')')
			rparencounter++;
		if (headptr->text[startindex - 1] == '(' && lparencounter == rparencounter) //startindex can at the very least be 1
			break;
		startindex--;
	}
	index = 0;
	for (int i = startindex; i < opindex; i++)
		(nodeptr->text)[index++] = (headptr->text)[i];
	nodeptr->text[opindex - startindex] = '\0';
	//printf("Lnode: %s\n", headptr->lptr->text);
	//print this out?
}

void rightportion (nodet *headptr, int opindex, int strlen)
{
	//Puts the subexpression on the right side of the operator we are hooking the expression on into the text of a new node in the headptr->rptr.
	int lparencounter = 0;
	int rparencounter = 0;
	nodet* nodeptr;
	int lastind = opindex + 1;
	int index = 0;
	headptr->rptr = nodeptr = malloc(sizeof(nodet));
	headptr->rptr->rptr = NULL;
	headptr->rptr->lptr = NULL;
	nodeptr->text = malloc(sizeof(char) * (strlen - opindex)); //last index is strlen - 1, and length of last part is distance to travel from operator
	while (lastind < strlen)
	{
		if (headptr->text[lastind] == '(')
			lparencounter++;
		if (headptr->text[lastind] == ')')
			rparencounter++;
		if (lparencounter < rparencounter) //break after one past last index we want to go to
			break;
		lastind++;
	}

	for (int i = opindex + 1; i < lastind; i++)
		(nodeptr->text)[index++] = (headptr->text)[i];
	nodeptr->text[lastind - opindex - 1] = '\0';
	//printf("Rnode: %s\n", headptr->rptr->text);
}
void addnodes (nodet *headptr) //going to be recursive
{
	int strlen = ft_strlen(headptr->text);
	int headindex = findheadindex(headptr->text, 0, strlen - 1);
	char temp;
	if (isoperator(headptr->text))
	{
		leftportion(headptr, headindex);
		rightportion(headptr, headindex, strlen);
		temp = (headptr->text)[headindex];
		free(headptr->text);
		headptr->text = malloc(sizeof(char) * 2);
		headptr->text[0] = temp; //replacing head with operator
		headptr->text[1] = '\0';
		addnodes(headptr->lptr);
		addnodes(headptr->rptr);
	}
	else //if no more operators
		return ;
}

void printtree (nodet *headptr, int rnode, arrnode_t array[], int *largestindex)
{
	/*if (headptr == NULL && rnode == 0)
	{
		printf("N\n");
		return ;
	} */
	char text;
	int index = rnode - 1; //our index is less than the counting to the next levels
	if (headptr == NULL)
	{
		array[index].operator = 'N'; //N means that there is no number
		if (index > *largestindex)
			*largestindex = index;
		return ;
	}
	text = headptr->text[0];
	if (text == '-' || text == '+' || text == '*' || text == '%' || text == '/')
		array[index].operator = headptr->text[0]; //if there is an operator, it will be in the operator box
	else
	{
		array[index].operator = 'P'; //P means that there is a number
		array[index].number = atoi(headptr->text);
	}
	printtree(headptr->lptr, (rnode * 2), array, largestindex);
	printtree(headptr->rptr, ((rnode * 2) + 1), array, largestindex);
}
int ft_atoi(char *string)
{
	//Converts a string into a number
	int neg = 1;
	long number = 0;
	int index = 0;
	while (string[index] == ' ')
		index++;
	if (string[index] == '-')
	{
		neg *= -1;
		index++;
	}
	while (string[index] != '\0' && string[index] != ' ')
		number = (number * 10) + (string[index++] - '0');
	return (neg * number);
}

int numlen(int number)
{
	//Calculates the length of a number (accounting for negative sign)
	if (number == 0)
		return (1);
	int len = 0;
	long numb = number;
	if (numb < 0)
	{
		len++;
		numb *= -1;
	}
	while (numb != 0)
	{
		numb /= 10;
		len++;
	}
	return (len);
}
void ft_numstring (char *textptr, long number, int *indexptr)
{
	//Converts "number" into a string and puts it in textptr using the index stored at the memory address of indexptr
	if (number < 0)
	{
		textptr[(*indexptr)++] = '-';
		number *= -1;
	}
	if (number >= 10)
		ft_numstring(textptr, number / 10, indexptr);
	textptr[(*indexptr)++] = (number % 10) + '0';
}


void solvetree (nodet *headptr)
{
	//Solves the tree recursively, starting with the smallest subexpression and replacing textnodes with the result of their expression.
	int leftnumber;
	int rightnumber;
	int numbertoinsert;
	int index;
	if (/*headptr == NULL ||*/ (headptr->lptr == NULL && headptr->rptr == NULL)) // if both left and right nodes are NULL, then we don't have to do any computation
		return ;

	//computing result of subtrees before using their result
	if (headptr->lptr->lptr != NULL && headptr->lptr->rptr != NULL)
		solvetree(headptr->lptr);
	if (headptr->rptr->lptr != NULL && headptr->rptr->rptr != NULL)
		solvetree(headptr->rptr);
	/*if (headptr->lptr->lptr != NULL || headptr->lptr->rptr != NULL || headptr->rptr->lptr != NULL || headptr->rptr->rptr != NULL) //can also do to ch
	{
		solvetree(headptr->lptr);
		solvetree(headptr->rptr);
	}*/
	leftnumber = ft_atoi(headptr->lptr->text);
	rightnumber = ft_atoi(headptr->rptr->text);
	if (headptr->text[0] == '*')
		numbertoinsert = leftnumber * rightnumber; //need to put them in this string...
	else if (headptr->text[0] == '/')
		numbertoinsert = leftnumber / rightnumber;
	else if (headptr->text[0] == '+')
		numbertoinsert = leftnumber + rightnumber;
	else if (headptr->text[0] == '-')
		numbertoinsert = leftnumber - rightnumber;
	else if (headptr->text[0] == '%')
		numbertoinsert = leftnumber % rightnumber;
	else
		exit(5);
	free(headptr->text);
	headptr->text = malloc(sizeof(char) * (numlen(numbertoinsert) + 1));
	index = 0;
	ft_numstring(headptr->text, numbertoinsert, &index);
	headptr->text[numlen(numbertoinsert)] = '\0';
	headptr->lptr = NULL; //setting prev numbers to null so that higher order processes can happen and you don't solve this subtree again
	headptr->rptr = NULL; 
}

int main(int argc, char* argv[])
{
	int index = 0;
	int argvlen;

	if (argc < 2)
	{
		printf("You done goofed\n");
		return (1);
	}
	argvlen = ft_strlen(argv[1]);
	if (isoperator(argv[1]) == 0) //if no operators
	{
		while (argv[1][index] == '(')
			index++;
		while (argv[1][index] != ')' && argv[1][index] != '\0')
			printf("%c", argv[1][index++]);
		printf("\n");
		return (0);
	}

	int headindex = findheadindex(argv[1], 0, ft_strlen(argv[1]));
	//printf("%c\n", argv[1][findheadindex(argv[1], 0, ft_strlen(argv[1]))]);
	
	//putting whole string inside headnode
	nodet headnode;
	headnode.text = malloc(sizeof(char) * (argvlen + 1));
	for (int i = 0; i < argvlen; i++)
		headnode.text[i] = argv[1][i];
	headnode.text[argvlen] = '\0';
	headnode.lptr = NULL;
	headnode.rptr = NULL;

	//adding leaves to the tree
	addnodes(&headnode);

	//printing the tree
	int largenumber = 0;
	int *largestnumber = &largenumber;
	arrnode_t *array = malloc(sizeof(arrnode_t) * (ft_strlen(argv[1]) + 1)); //upper bound on numbers is amount of characters in expression
	for (int i = 0; i < (ft_strlen(argv[1]) + 1); i++)
		array[i].operator = 'U'; //saying unused
	array[ft_strlen(argv[1])].operator = '\0'; 
	printtree(&headnode, 1, array, largestnumber);
	int binarycounter = 1;
	int rows = 0;
	while (binarycounter <= *largestnumber + 1)
	{
		binarycounter *= 2; //binary counter becomes the power of two greater than the amount of numbers 
		rows += 1;
	}
	printf("Number of units (counting): %i,  Rows: %i\n", *largestnumber + 1, rows);
	int counter = 1;
	int base2 = 2;
	int i = 0;
	int spacing = rows;
	for (int l = 0; l < spacing; l++)
		printf(" ");
	spacing -= 1;
	while (/*array[i].operator != 'U' &&*/ array[i].operator != '\0') //printing out array (how did this become '\0'?
	{
		if (array[i].operator == 'P')
			printf("%i ", array[i].number);
		else if (array[i].operator == 'N')
			printf("N ");
		else if (array[i].operator == 'U')
			printf("  ");
		else //meaning that it is the operator
			printf("%c ", array[i].operator);
		if (i % 2 == 0)
			printf(" ");
		counter++;
		if (counter == base2)
		{
			printf("\n");
			for (int l = 0; l < spacing; l++)
				printf(" ");
			spacing -= 1;
			base2 *= 2;
		}
		i++;
	}
	//solving the tree
	solvetree(&headnode);
	printf("\nAnswer:  %s\n", headnode.text);

	//printf("Atoi of -32: %i\n", ft_atoi("0"));

}
	


#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	char *text;
	struct node *lptr;
	struct node *rptr;
} nodet;

int ft_strlen(char argument[])
{
	int i = 0;
	while (argument[i] != '\0')
		i++;
	return (i);
}

int isnegnum (char argv[], int negindex)
{
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
	else
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

void printtree (nodet *headptr, int rnode)
{
	if (headptr == NULL && rnode == 0)
	{
		printf("N\n");
		return ;
	}
	if (headptr == NULL)
	{
		printf("N ");
		return ;
	}
	printf(" %s ", headptr->text);
	if (rnode == 0)
		printf("\n");
	printtree(headptr->lptr, 1);
	/*if (rnode == 1)
		printtree(headptr->rptr, 1);
	if (rnode == 0)
		printtree(headptr->rptr, 0);*/
	printtree(headptr->rptr, 0);
}
int ft_atoi(char *string)
{
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
	int leftnumber;
	int rightnumber;
	int numbertoinsert;
	int index;
	if (headptr == NULL || (headptr->lptr == NULL && headptr->rptr == NULL)) // taking care of when we have NULLs surrounding first number
		return ;

	if (headptr->lptr->lptr != NULL || headptr->lptr->rptr != NULL || headptr->rptr->lptr != NULL || headptr->rptr->rptr != NULL) //can also do to check if nums
	{
		solvetree(headptr->lptr);
		solvetree(headptr->rptr);
	}
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
	headptr->lptr = NULL; //setting prev numbers to null so that higher order processes can happen
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
	//printtree(&headnode, 0);

	//solving the tree
	solvetree(&headnode);
	printf("Answer:  %s\n", headnode.text);

	//printf("Atoi of -32: %i\n", ft_atoi("0"));

}
	


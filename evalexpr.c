#include <stdio.h>

int ft_strlen(char argument[])
{
	int i = 0;
	while (argument[i] != '\0')
		i++;
	return (i);
}
int findheadindex (char argv[], int leftptr, int rightptr)
{
	int maxindex = -1;
	while (argv[leftptr] != '(')
	{
		if (argv[leftptr] == '+' || argv[leftptr] == '-')
			return (leftptr);
		if (argv[leftptr] == '*' || argv[leftptr] == '/' || argv[leftptr] == '%')
			maxindex = leftptr;
		leftptr++;
	}
	while (argv[rightptr] != ')')
	{
		if (argv[rightptr] == '+' || argv[rightptr] == '-')
			return (rightptr);
		if (argv[rightptr] == '*' || argv[rightptr] == '/' || argv[rightptr] == '%')
			maxindex = rightptr;
		rightptr--;
	}
	if (maxindex != -1)
		return (maxindex);
	else
		return (findheadindex(argv, leftptr + 1, rightptr - 1));

}

int isoperator (char argv[])
{
	int i = -1;
	int len = ft_strlen(argv);
	while (++i < len)
	{
		if (argv[i] == '+' || argv[i] == '-' || argv[i] == '*' || argv[i] == '/' || argv[i] == '%')
			return (0);
	}
	return (1);
}
int main(int argc, char *argv[])
{
	int index = 0;
	if (argc < 2)
	{
		printf("You done goofed\n");
		return (1);
	}
	if (isoperator(argv[1]) == 1)
	{
		while (argv[1][index] == '(')
			index++;
		while (argv[1][index] == ')' || argv[1][index] == '\0')
			printf("%c", argv[1][index++]);
		printf("\n");
		return (0);
	}

	int headindex = findheadindex(argv[1], 0, ft_strlen(argv[1]));
	printf("%c\n", argv[1][findheadindex(argv[1], 0, ft_strlen(argv[1]))]);
}
	


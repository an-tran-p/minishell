#include "Libft/libft.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

// struct def
typedef enum e_token_type
{
	WORD_VAR,
	WORD,
	INFILE,
	OUTFILE,
	CMD,
	ARG,
	RD_LEFT,
	RD_HEREDOC,
	RD_RIGHT,
	RD_APPEND,
	PIPE
}					t_etype;

typedef struct s_token
{
	t_etype			type;
	char			*s;
	struct s_token	*next;
}					t_token;

// list_functions start
t_token	*tk_lstnew(t_etype type, char *s)
{
	t_token	*new;

	new = (t_token *)calloc(1, sizeof(t_token));
	if (!new)
		return (NULL);
	new->type = type;
	new->s = s;
	return (new);
}

t_token	*tk_lstlast(t_token *head)
{
	while (head && head->next)
		head = head->next;
	return (head);
}

void	tk_lstadd_back(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!*head)
		*head = new;
	else if (new)
	{
		tmp = tk_lstlast(*head);
		tmp->next = new;
	}
}

void	tk_lstclear(t_token **head)
{
	t_token	*tmp;

	while (*head)
	{
		tmp = *head;
		*head = (*head)->next;
		// free(tmp->s);
		free(tmp);
	}
}

void	tk_appendlst(t_token **head, t_etype type, char *s)
{
	t_token	*new;

	new = tk_lstnew(type, s);
	if (!new)
	{
		tk_lstclear(head);
		// exit needed!!!
	}
	else
		tk_lstadd_back(head, new);
}

char	*get_tk_name(t_etype type)
{
	switch (type)
	{
	case WORD_VAR:
		return ("WORD_VAR");
	case WORD:
		return ("WORD");
	case INFILE:
		return ("INFILE");
	case OUTFILE:
		return ("OUTFILE");
	case CMD:
		return ("CMD");
	case ARG:
		return ("ARG");
	case RD_LEFT:
		return ("RD_LEFT");
	case RD_HEREDOC:
		return ("RD_HEREDOC");
	case RD_RIGHT:
		return ("RD_RIGHT");
	case RD_APPEND:
		return ("RD_APPEND");
	case PIPE:
		return ("PIPE");
	}
}

void	tk_lstprint(t_token *head)
{
	while (head)
	{
		printf("\x1B[31m"
				"%s"
				"\x1B[0m",
				head->s);
		printf("(%s) ", get_tk_name(head->type));
		head = head->next;
	}
	printf("\n");
}
// list_functions end

void	list_process1(t_token **head)
{
	tk_appendlst(head, CMD, "cat");
	tk_appendlst(head, ARG, "test.txt");
	tk_appendlst(head, PIPE, "|");
	tk_appendlst(head, CMD, "grep");
	tk_appendlst(head, ARG, "apple");
	tk_appendlst(head, PIPE, "|");
	tk_appendlst(head, CMD, "sort");
	tk_appendlst(head, ARG, "output.txt");
}

void	list_process2(t_token **head)
{
	tk_appendlst(head, CMD, "sort");
	tk_appendlst(head, RD_LEFT, "<");
	tk_appendlst(head, INFILE, "test.txt");
	tk_appendlst(head, RD_RIGHT, ">");
	tk_appendlst(head, OUTFILE, "sorted_output.txt");
}

void	list_process3(t_token **head)
{
	tk_appendlst(head, CMD, "echo");
	tk_appendlst(head, ARG, "This is a test");
	tk_appendlst(head, RD_RIGHT, ">");
	tk_appendlst(head, OUTFILE, "test.txt");
	tk_appendlst(head, PIPE, "|");
	tk_appendlst(head, CMD, "echo");
	tk_appendlst(head, ARG, "Apppended line");
	tk_appendlst(head, RD_APPEND, ">>");
	tk_appendlst(head, OUTFILE, "sorted_output.txt");
}

void	list_process4(t_token **head)
{
	tk_appendlst(head, CMD, "ls");
	tk_appendlst(head, ARG, "-l");
	tk_appendlst(head, PIPE, "|");
	tk_appendlst(head, CMD, "grep");
	tk_appendlst(head, ARG, "txt");
	tk_appendlst(head, PIPE, "|");
	tk_appendlst(head, CMD, "wc");
	tk_appendlst(head, ARG, "-l");
	tk_appendlst(head, RD_RIGHT, ">");
	tk_appendlst(head, OUTFILE, "count.txt");
}

int	main(void)
{
	t_token	*lst1= NULL;
	t_token	*lst2;
	t_token	*lst3;
	t_token	*lst4;

	//	char	*lst1= "cat test.txt | grep \"apple\" | sort output.txt";
	//	char	*lst2= "sort < test.txt > sorted_output.txt";
	//	char	*lst3= "echo \"This is a test\" > test.txt | echo \"Appended line\" >> test.txt";
	//	char	*lst4= "ls -l | grep \"txt\" | wc -l > count.txt";
	
	lst2 = NULL;
	lst3 = NULL;
	lst4 = NULL;
	list_process1(&lst1);
	list_process2(&lst2);
	list_process3(&lst3);
	list_process4(&lst4);
	tk_lstprint(lst1);
	tk_lstclear(&lst1);
	tk_lstprint(lst2);
	tk_lstclear(&lst2);
	tk_lstprint(lst3);
	tk_lstclear(&lst3);
	tk_lstprint(lst4);
	tk_lstclear(&lst4);
}

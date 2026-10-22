/*
	Do:
		#define LIBLLIST_IMPLEMENTATION 
	to get access to the lib functions.
	This should be done only one time !
*/

#ifndef LIBLLIST_H
# define LIBLLIST_H

# include <stdlib.h>

/**
 * @brief A pointer to a function that can destroys t_llist data.
 */
typedef void (*t_llistDestroyFn)(void *data);

/**
 * @brief A pointer to a function that can duplicate t_llist data.
 * @return A pointer to the duplicated data on success, or NULL on failure.
 */
typedef void *(*t_llistDupFn)(void *data);


typedef struct s_llist
{
	void			*data;
	struct s_llist	*next;
}	t_llist;


/**
 * @brief Creates a new allocated t_llist node using @a data.
 * @return The allocated node on success, or NULL on failure.
 */
t_llist	*llistNew(void  *data);

/**
 * @brief Returns the number of nodes in @a head.
 */
size_t	llistLength(t_llist *head);

/**
 * @brief Returns the @a nth node in @a head, starting from 0.
 */
t_llist	*llistGet(t_llist *head, size_t n);

/**
 * @brief Duplicates @a head, meaning it creates a new t_llist, with duplicated data.
 * @param dup If not NULL, duplicates each node data, or copies each data pointer otherwise.
 * @param destroy If not NULL, frees each allocated node's data in case of error, or does noting otherwise.
 * @return A pointer to the duplicated t_llist on success, or NULL on failure.
 */
t_llist	*llistDup(t_llist *head, t_llistDupFn dup, t_llistDestroyFn destroy);

/**
 * @brief Appends @a lst at the end of @a head.
 * @return @a head on success, or NULL on failure.
 */
t_llist	*llistPushBack(t_llist **head, t_llist *lst);

/**
 * @brief Removes a node at the end of @a head.
 * @warning If @a head posses only one node, sets @a *head to NULL.
 * @return A pointer to the popped node or NULL if head is empty.
 */
t_llist	*llistPop(t_llist **head);

/**
 * @brief Destroys @a head and all its data using @a destroyDataFn, and sets @a head to NULL;
 * @param destroy If not NULL, frees each allocated node's data, or does noting otherwise.
 */
void	llistDestroy(t_llist **head, t_llistDestroyFn destroy);

# ifdef LIBLLIST_IMPLEMENTATION

t_llist	*llistNew(void *data)
{
	t_llist	*new;

	new = malloc(sizeof(t_llist));
	if (NULL == new)
		return (NULL);
	new->data = data;
	new->next = NULL;
	return (new);
}

size_t	llistLength(t_llist *head)
{
	size_t	length = 0;

	if (NULL == head)
		return (0);
	while (head)
	{
		++length;
		head = head->next;
	}
	return (length);
}

t_llist	*llistGet(t_llist *head, size_t n)
{
	size_t	index = 0;

	while (head && index < n)
	{
		head = head->next;
		++index;
	}
	return (head);	
}

t_llist	*llistDup(t_llist *head, t_llistDupFn dup, t_llistDestroyFn destroy)
{
	t_llist	*duped = NULL;

	if (NULL == dup)
		destroy = NULL;
	while (head)
	{
		void	*data = NULL;
		t_llist	*cpy;

		data = head->data;
		if (dup)
		{
			data = dup(head->data);
			if (NULL == data)
			{
				llistDestroy(&duped, destroy);
				return (NULL);
			}
		}
		cpy = llistNew(data);
		if (NULL == cpy)
		{
			llistDestroy(&duped, destroy);
			return (NULL);
		}
		llistPushBack(&duped, cpy);
		head = head->next;
	}
	return (duped);
}

t_llist	*llistPushBack(t_llist **head, t_llist *lst)
{
	t_llist	*cpy;

	if (NULL == head || NULL == lst)
		return (NULL);
	if (NULL == *head)
	{
		*head = lst;
		return (*head);
	}
	cpy = *head;
	while (cpy->next)
		cpy = cpy->next;
	cpy->next = lst;
	return (*head);
}

t_llist	*llistPop(t_llist **head)
{
	t_llist	*cpy;
	t_llist	*last;

	if (NULL == head || NULL == *head)
		return (NULL);
	cpy = *head;
	if (NULL == cpy->next)
	{
		*head = NULL;
		return (cpy);
	}
	while (cpy->next->next)
		cpy = cpy->next;
	last = cpy->next;
	cpy->next = NULL;
	return (last);
}

void	llistDestroy(t_llist **head, t_llistDestroyFn destroy)
{
	t_llist	*cpy;

	if (NULL == head || NULL == *head)
		return ;
	cpy = *head;
	while (cpy)
	{
		t_llist	*tmp = cpy->next;
		
		if (destroy)
			destroy(cpy->data);
		free(cpy);
		cpy = tmp;
	}
	*head = NULL;
	return ;
}

# endif //LIBLLIST_IMPLEMENTATION

#endif //LIBLLIST_H

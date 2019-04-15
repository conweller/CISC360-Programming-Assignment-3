#include "watch.h"

int *nthreads;
long int *result_mail;
pthread_mutex_t lock_user;
pthread_mutex_t lock_mail;
struct Node *head;

struct Node *insert(char *name) {
	struct Node *node1;

	node1 = (struct Node *) malloc(sizeof(struct Node));        // malloc space for Node

	node1->name = (char *) malloc(strlen(name) + 1);  // malloc space for name
	strcpy(node1->name, name);                        // "assign" name via copy

	node1->tty = NULL;
	node1->next = NULL;
	node1->size = 0;

	if (head == NULL) {
		head = node1;
	} else {
		while (head->next) {
			head = head->next;
		}
		head->next = node1;
	}
	return node1;
}

void delete(char *name) {
	struct Node *temp, *prev;
	int i = 0;
	if (head == NULL) {
		return;
	} else {
		temp = head;
		while (temp != NULL) { //traverses whole list
			if (strcmp(temp->name, name) == 0) {
				//condition if deleting the only node left in list
				if ((temp == head) && (head->next = NULL)) {
					free(temp->name);
					free(temp); //frees the Node record
					head = NULL;
					temp = NULL;
				}
				//condition if head node is deleted
				else if (temp == head) {
					head = temp->next;
					free(temp->name);
					free(temp); //frees the Node record
					temp = NULL;
				} else {
					prev->next = temp->next; //changes next pointer for previous node
					free(temp->name);
					free(temp); //frees the Node record
					temp = NULL;
				}
			}
			prev = temp;
		}
	}
}

void freeList() {
	struct Node *temp;
	while (head != NULL) {
		temp = head;
		head = head->next; // point to next Node
		free(temp->tty);
		free(temp->name);  // first free name inside Node
		free(temp);        // then free Node
	}
}

int watchuser(char *name, int off, int first) {

	void *nothing;
	pthread_mutex_init(&lock_user, NULL);
	if (first) {
		insert(name);
		pthread_t id;
		long int random = 0;
		pthread_create(&id, NULL, threaduser, (void *)random);
	} else if (off) {
		delete(name);
	} else {
		insert(name);
	}
	//pthread_join(id, &nothing);
	return 0;
}

void *threaduser(void *something) {
	int i = 0;
	while (i < 7) {
		if (head != NULL) {
			struct utmpx *up;
			setutxent();			/* start at beginning */
			pthread_mutex_lock(&lock_user);
			while (up = getutxent())	/* get an entry */
			{
				if ( up->ut_type == USER_PROCESS )	/* only care about users */
				{
					struct Node *temp;
					temp = head;
					while (temp != NULL) {
						if (strcmp(temp->name,up->ut_user) == 0) {
							//Condition of this is a user's first logon
							if (temp->tty == NULL) {
								temp->tty = (char **) malloc(sizeof(char **));
								temp->tty[0] = (char *) malloc(strlen(up->ut_line) + 1);// malloc space for name
								strcpy(temp->tty[0], up->ut_line); // "assign" name via copy
								temp->size = 1;
								printf("%s has logged on %s from %s\n",up->ut_user,up->ut_line,up ->ut_host);	
							}
							//Condition of watched user has another new logon (adds new node to linked list)
							else {
								int i = 0;
								int found = 0;
								while (i < temp->size) {
									if (strcmp(temp->tty[i],up->ut_line) == 0) {
										found = 1;
										break;
									}
									i++;
								}
								if (!found) {
									temp->size += 1;
									temp->tty = realloc(temp->tty, temp->size * sizeof(char **));
									temp->tty[temp->size-1] = (char *) malloc(strlen(up->ut_line) + 1);// malloc space for name
									strcpy(temp->tty[temp->size-1], up->ut_line); // "assign" name via copy
									printf("%s has logged on %s from %s\n",up->ut_user,up->ut_line,up ->ut_host);							
								}

							}
						}
						temp = temp->next;
					}
				}
			}
			pthread_mutex_unlock(&lock_user);
		}
		i++;
		sleep(1);
	}
	return NULL;
}

int watchmail(char *file) {
	nthreads = malloc(sizeof(int));
	*nthreads = 2;
	result_mail = malloc(sizeof(long int));
	*result_mail = 0;
	void *nothing;

	pthread_mutex_init(&lock_mail, NULL);

	pthread_t *threads = (pthread_t *) malloc(*nthreads *sizeof(pthread_t));
	for (long int tid = 0; tid < *nthreads; tid++) {
		pthread_create(&threads[tid], NULL, threadmail, (void *)tid);
	} for (int tid = 0; tid < *nthreads; tid++) {
		pthread_join(threads[tid], &nothing);
	}
	free(threads);
	free(result_mail);
	free(nthreads);
	return 0;
}

void *threadmail(void *something) {
	int local_result = 420;

	pthread_mutex_lock(&lock_mail);
	*result_mail = local_result;
	pthread_mutex_unlock(&lock_mail);
	return NULL;
}

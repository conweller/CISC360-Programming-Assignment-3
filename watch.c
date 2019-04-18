#include "watch.h"

int *nthreads;
long int *result_mail;
pthread_mutex_t lock_user;
pthread_mutex_t lock_mail;
struct Node *head;
pthread_t id;

void insert(char *name) {
	struct Node *node1;

	node1 = (struct Node *) malloc(sizeof(struct Node));        // malloc space for Node

	node1->name = (char *) malloc(strlen(name) + 1);  // malloc space for name
	strcpy(node1->name, name);                        // "assign" name via copy
	
	node1->on = 0;
	node1->tty = NULL;
	node1->host = NULL;
	node1->next = NULL;

	if (head == NULL) {
		head = node1;
	} else {
		struct Node *temp = head;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = node1;
	}
}

void delete(char *name) {
	struct Node *temp, *prev;
	if (head == NULL) {
		return;
	} else {
		temp = head;
		while (temp != NULL) { //traverses whole list
			if (strcmp(temp->name, name) == 0) {
				//condition if deleting the only node left in list
				if ((temp == head) && (head->next = NULL)) {
					free(temp->name);
					if (temp->tty != NULL) {
						free(temp->tty);
						free(temp->host);
					}
					free(temp); //frees the Node record
					head = NULL;
				}
				//condition if head node is deleted but more nodes are present
				else if (temp == head) {
					head = temp->next;
					free(temp->name);
					if (temp->tty != NULL) {
						free(temp->tty);
						free(temp->host);
					}
					free(temp); //frees the Node record
				} else {
					prev->next = temp->next; //changes next pointer for previous node
					free(temp->name);
					if (temp->tty != NULL) {
						free(temp->tty);
						free(temp->host);
					}
					free(temp); //frees the Node record
				}
				break;
			}
			prev = temp;
			temp = temp->next;
		}
	}
}

void freeList() {
	struct Node *temp;
	while (head != NULL) {
		temp = head;
		head = head->next; // point to next Node
		free(temp->name);  // first free name inside Node
		if (temp->tty != NULL) {
			free(temp->tty);
			free(temp->host);
		}
		free(temp);        // then free Node
	}
}

int watchuser(char *name, int off, int first) {

	pthread_mutex_init(&lock_user, NULL);
	if (strcmp(name,"exit") == 0) {
		if (pthread_cancel(id)!= 0) {
			perror("pthread_cancel() error");
		}
  		if (pthread_join(id, NULL) != 0) {                                          
	    	perror("pthread_join() error");                                             
			exit(4);                                                                    
		}
		freeList();
	} else if (first) {
		insert(name);
		if (pthread_create(&id, NULL, threaduser, NULL) != 0) {                         
    		perror("pthread_create() error");                                           
    		exit(1);                                                                    
  		}
	} else if (off) {
		delete(name);
	} else {
		/* Checks to see if name is already in list */
		struct Node *temp = head;
		int unique = 1;
		while (temp != NULL) {
			if (strcmp(name,temp->name) == 0) {
				unique = 0;
				break;
			}
			temp = temp->next;
		}
		/* If it is a new name, inserts the appropriate node */
		if (unique) {
			insert(name);
		}
	}
	return 0;
}

void *threaduser(void *something) {
	while (1) {
		if (head != NULL) {
			struct Node *temp;
			temp = head;
			while (temp != NULL) {
				struct utmpx *up;
				setutxent();			/* start at beginning */
				int found = 0;
				pthread_mutex_lock(&lock_user);
				while (up = getutxent())	/* get an entry */
				{
					if ( up->ut_type == USER_PROCESS )	/* only care about users */
					{
						if (temp->on) {
							if (strcmp(temp->name,up->ut_user) == 0) {
								found = 1;
								break;
							}
						} else {
							if (strcmp(temp->name,up->ut_user) == 0) {
								printf("%s has logged on %s from %s\n",up->ut_user,up->ut_line,up ->ut_host);
								temp->tty = (char *) malloc(strlen(up->ut_line) + 1);
								strcpy(temp->tty, up->ut_line);
								temp->host = (char *) malloc(strlen(up->ut_host) + 1);
								strcpy(temp->host, up->ut_host);
								temp->on = 1;
								found = 1;
								break;
							}
						}
					}
				}
				if (temp->on && !found) {
					printf("%s has logged off %s from %s\n",temp->name,temp->tty,temp->host);
					free(temp->tty);
					temp->tty = NULL;
					free(temp->host);
					temp->host = NULL;
					temp->on = 0;
				}
				pthread_mutex_unlock(&lock_user);
				temp = temp->next;
			}

		}
		sleep(2);
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

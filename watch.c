#include "watch.h"

long int *result_mail;
pthread_mutex_t lock_user;
struct Node *userhead;
struct Node *mailhead;
pthread_t user_id;

void insert(char *name, pthread_t id, char *type) {
	struct Node *node1;

	node1 = (struct Node *) malloc(sizeof(struct Node));        // malloc space for Node

	node1->name = (char *) malloc(strlen(name) + 1);  // malloc space for name
	strcpy(node1->name, name);                        // "assign" name via copy
	
	node1->on = 0;
	node1->tty = NULL;
	node1->host = NULL;
	node1->id = id;
	node1->next = NULL;
	
	if (strcmp(type, "user") == 0) {
		if (userhead == NULL) {
			userhead = node1;
		} else {
			struct Node *temp = userhead;
			while (temp->next) {
				temp = temp->next;
			}
			temp->next = node1;
		}	
	} else {
		if (mailhead == NULL) {
			mailhead = node1;
		} else {
			struct Node *temp = mailhead;
			while (temp->next) {
				temp = temp->next;
			}
			temp->next = node1;
		}
	}

}

void delete(char *name, char *type) {
	struct Node *temp, *prev;
	if (strcmp(type, "user") == 0) {
		if (userhead == NULL) {
			return;
		} else {
			temp = userhead;
		}
	} else {
		if (mailhead == NULL) {
			return;
		} else {
			temp = mailhead;
		}
	}
	while (temp != NULL) { //traverses whole list
		if (strcmp(temp->name, name) == 0) {
			if (temp->id != (pthread_t)NULL) {
				if (pthread_cancel(temp->id)!= 0) {
					perror("pthread_cancel() error");
				}
				if (pthread_join(temp->id, NULL) != 0) {
					perror("pthread_join() error");
					exit(4);
				}
				// MAY NEED THIS IDK free(temp->id);
			}
			if (strcmp(type, "user") == 0) {
				//condition if deleting the only node left in list
				if ((temp == userhead) && (userhead->next = NULL)) {
					free(temp->name);
					if (temp->tty != NULL) {
						free(temp->tty);
						free(temp->host);
					}
					free(temp); //frees the Node record
					userhead = NULL;
				}
				//condition if head node is deleted but more nodes are present
				else if (temp == userhead) {
					userhead = temp->next;
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
			} else {
				//condition if deleting the only node left in list
				if ((temp == mailhead) && (mailhead->next = NULL)) {
					free(temp->name);
					if (temp->tty != NULL) {
						free(temp->tty);
						free(temp->host);
					}
					free(temp); //frees the Node record
					mailhead = NULL;
				}
				//condition if head node is deleted but more nodes are present
				else if (temp == mailhead) {
					mailhead = temp->next;
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
			}
			break;
		}
		prev = temp;
		temp = temp->next;
	}
}

void freeList(char *type) {
	struct Node *temp;
	if (strcmp(type, "user") == 0) {
		while (userhead != NULL) {
			temp = userhead;
			userhead = userhead->next; // point to next Node
			free(temp->name);  // first free name inside Node
			if (temp->tty != NULL) {
				free(temp->tty);
				free(temp->host);
			}
			free(temp);        // then free Node
		}
	} else {
		while (mailhead != NULL) {
			temp = mailhead;
			mailhead = mailhead->next; // point to next Node
			free(temp->name);  // first free name inside Node
			if (temp->tty != NULL) {
				free(temp->tty);
				free(temp->host);
			}
			free(temp);        // then free Node
		}
	}

}

int watchuser(char *name, int off, int first) {

	pthread_mutex_init(&lock_user, NULL);
	if (strcmp(name,"exit") == 0) {
		if (pthread_cancel(user_id)!= 0) {
			perror("pthread_cancel() error");
		}
  		if (pthread_join(user_id, NULL) != 0) {                                          
	    	perror("pthread_join() error");                                             
			exit(4);                                                                    
		}
		freeList("user");
	} else if (first) {
		insert(name, (pthread_t)NULL, "user");
		if (pthread_create(&user_id, NULL, threaduser, NULL) != 0) {                         
    		perror("pthread_create() error");                                           
    		exit(1);                                                                    
  		}
	} else if (off) {
		delete(name, "user");
	} else {
		/* Checks to see if name is already in list */
		struct Node *temp = userhead;
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
			insert(name, (pthread_t)NULL, "user");
		}
	}
	return 0;
}

void *threaduser(void *something) {
	while (1) {
		pthread_mutex_lock(&lock_user);
		if (userhead != NULL) {
			struct Node *temp;
			temp = userhead;
			while (temp != NULL) {
				struct utmpx *up;
				setutxent();			/* start at beginning */
				int found = 0;
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
								printf("\n%s has logged on %s from %s\n",up->ut_user,up->ut_line,up ->ut_host);
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
					printf("\n%s has logged off %s from %s\n",temp->name,temp->tty,temp->host);
					free(temp->tty);
					temp->tty = NULL;
					free(temp->host);
					temp->host = NULL;
					temp->on = 0;
				}
				temp = temp->next;
			}
			pthread_mutex_unlock(&lock_user);
		}
		sleep(2);
	}
	return NULL;
}

int watchmail(char *file, int off) {

	if (strcmp(file,"exit") == 0) {
		struct Node *temp = mailhead;
		while (temp != NULL) {
			if (pthread_cancel(temp->id)!= 0) {
				perror("pthread_cancel() error");
			}
			if (pthread_join(temp->id, NULL) != 0) {                                          
				perror("pthread_join() error");                                             
				exit(4);                                                                    
			}
			temp = temp->next;
		}
		freeList("mail");
		return 0;
	}
	if (access(file, F_OK) != 0) {
		perror("File not found.");
	} else if (off) {
		delete(file, "mail");
	} else {
		/* Checks to see if name is already in list */
		struct Node *temp = mailhead;
		int unique = 1;
		while (temp != NULL) {
			if (strcmp(file,temp->name) == 0) {
				unique = 0;
				break;
			}
			temp = temp->next;
		}
		/* If it is a new name, inserts the appropriate node */
		if (unique) {
			pthread_t id;
			insert(file, id, "mail");		
			char *name = (char *) malloc(strlen(file) + 1);  // malloc space for name
			strcpy(name,file);
			if (pthread_create(&id, NULL, threadmail, name) != 0) { 
				perror("pthread_create() error");                                           
				exit(1);                                                                    
			}
		}
	}
	return 0;
}

void *threadmail(void *file) {
	struct stat buff;
	char *name = (char *) file;
	stat(name, &buff);
	long int past = buff.st_size;
	while (1) {
		stat(name, &buff);
		if (buff.st_size > past) {
			printf("\a You've got Mail in %s at %s\n", name, ctime(&buff.st_ctim.tv_sec));
			past = buff.st_size;
		}
		sleep(2);
	}
	return NULL;
}

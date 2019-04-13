#include "watch.h"

int *nthreads;
long int *result_user;
long int *result_mail;
pthread_mutex_t lock_user;
pthread_mutex_t lock_mail;

int watchuser(char *user, int off) {
	
	result_user = malloc(sizeof(long int));
	*result_user = 0;
	void *nothing;

	pthread_mutex_init(&lock_user, NULL);

	pthread_t id;
	long int random = 0;
	pthread_create(&id, NULL, threaduser, (void *)random);
	pthread_join(id, &nothing);
	
	free(result_user);
	return 0;
}

void *threaduser(void *something) {
	int local_result = 7;

	pthread_mutex_lock(&lock_user);
	*result_user = local_result;
	pthread_mutex_unlock(&lock_user);
	
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

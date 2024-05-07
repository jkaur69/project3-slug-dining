#include "dining.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
typedef struct dining {
  // TODO: Add your variables here
  int capacity;
  int num_students;
  int cleaning;
  pthread_mutex_t mutex;
  pthread_cond_t clean_cond;
  sem_t enter_sem;
  sem_t leave_sem;

} dining_t;

dining_t *dining_init(int capacity) {
  // TODO: Initialize necessary variables
  dining_t *dining = (dining_t *)malloc(sizeof(dining_t));
  dining->capacity = capacity;
  dining->num_students = 0;
  dining->cleaning = 0;
  pthread_mutex_init(&dining->mutex, NULL);
  pthread_cond_init(&dining->clean_cond, NULL);
  sem_init(&dining->enter_sem, 0, capacity);
  sem_init(&dining->leave_sem, 0, 0);
  return dining;
}
void dining_destroy(dining_t **dining_ptr) {
  // TODO: Free dynamically allocated memory
  dining_t *dining = *dining_ptr;
  sem_destroy(&dining->enter_sem);
  sem_destroy(&dining->leave_sem);
  pthread_mutex_destroy(&dining->mutex);
  pthread_cond_destroy(&dining->clean_cond);
  free(dining);
  *dining_ptr = NULL;
}

void dining_student_enter(dining_t *dining) {
  // TODO: Your code goes here
  sem_wait(&dining->enter_sem);
  pthread_mutex_lock(&dining->mutex);
  dining->num_students++;
  pthread_mutex_unlock(&dining->mutex);
}

void dining_student_leave(dining_t *dining) {
  // TODO: Your code goes here
  pthread_mutex_lock(&dining->mutex);
  dining->num_students--;
  pthread_mutex_unlock(&dining->mutex);
  sem_post(&dining->enter_sem);
  sem_post(&dining->leave_sem);
}

void dining_cleaning_enter(dining_t *dining) {
  // TODO: Your code goes here
  pthread_mutex_lock(&dining->mutex);
  dining->cleaning = 1;
  while (dining->num_students > 0) {
    pthread_cond_wait(&dining->clean_cond, &dining->mutex);
  }
}

void dining_cleaning_leave(dining_t *dining) {
  // TODO: Your code goes here
  dining->cleaning = 0;
  pthread_mutex_unlock(&dining->mutex);
}

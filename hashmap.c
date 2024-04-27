#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) 
{
  long pos = hash(key, map->capacity);
  if(map->buckets[pos] != NULL) {
    while(map->buckets[pos] != NULL) {
      pos = (pos + 1) % map->capacity;
    }
  }
  Pair *aux = createPair(key, value);
  map->buckets[pos] = aux;
  map->size += 1;
}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  Pair **oldPairs = map->buckets;
  long newCap = map->capacity * 2;
  Pair **newPairs = (Pair **) calloc (newCap, sizeof(Pair *) * newCap);
  map->buckets = newPairs;
  map->size = 0;
  printf("%ld\n", newCap);
  map->size = 0;
  
  //printf("%s\n",(char *) oldMap[0]->value);
  for(long i = 0; i < map->capacity; i++) {
    if(oldPairs[i] != NULL) {
      insertMap(map, oldPairs[i]->key, oldPairs[i]->value);
      printf("%s -- %ld\n",(char *) oldPairs[i]->value, i);
    }
  }
  for(long i = 0; i < newCap; i++) {
    if(map->buckets[i] == NULL) {
      printf("Nulo - %ld\n", i);
    }
  }
}


HashMap * createMap(long capacity) 
{
  HashMap *map = (HashMap *) malloc (sizeof(HashMap));
  map->buckets = (Pair **) malloc (sizeof(Pair *) * capacity);
  for(long i = 0; i < capacity; i++) {
    map->buckets[i] = NULL;
  }
  map->capacity = capacity;
  map->current = -1;
  map->size = 0;
  return map;
}

void eraseMap(HashMap * map,  char * key) 
{
  long pos = hash(key, map->capacity);
  while(map->buckets[pos] != NULL) {
    if(strcmp(map->buckets[pos]->key, key) == 0) {
      map->buckets[pos]->key = NULL;
      map->size -= 1;
    }
    pos = (pos + 1) % map->capacity;
  }
}

Pair * searchMap(HashMap * map,  char * key) 
{
  long pos = hash(key, map->capacity);
  while(map->buckets[pos] != NULL) {
    if(strcmp(map->buckets[pos]->key, key) == 0) {
      map->current = pos;
      return map->buckets[pos];
    }
    pos = (pos + 1) % map->capacity;
  }
  return NULL;
}

Pair * firstMap(HashMap * map) {
  long pos = 0;
  while(map->buckets[pos] == NULL || map->buckets[pos]->key == NULL) {
    pos = (pos + 1) % map->capacity;
  }
  map->current = pos;
  return map->buckets[pos];
}

Pair * nextMap(HashMap * map) 
{
  long pos = map->current;
  long aux = pos;
  do {
    pos = (pos + 1) % map->capacity;
    if(aux > pos) {
      return NULL;
    }
  } while(map->buckets[pos] == NULL || map->buckets[pos]->key == NULL);
  map->current = pos;
  return map->buckets[pos];
}

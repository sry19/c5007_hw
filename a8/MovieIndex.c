/*
 *  Adrienne Slaughter
 *  5007 Spr 2020
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  See <http://www.gnu.org/licenses/>.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "MovieIndex.h"
#include "htll/LinkedList.h"
#include "htll/Hashtable.h"
#include "Movie.h"
#include "MovieSet.h"

void DestroyMovieSetWrapper(void *movie_set) {
  DestroyMovieSet((MovieSet)movie_set);
}

void toLower(char *str, int len) {
  for (int i = 0; i < len; i++) {
    str[i] = tolower(str[i]);
  }
}

Index BuildMovieIndex(LinkedList movies, enum IndexField field_to_index) {
  Index movie_index = CreateIndex();

  // STEP 4(Student): Check that there is at least one movie
  // What happens if there is not at least one movie?
  if (NumElementsInLinkedList(movies) <= 0) {
    return movie_index;
  }

  LLIter iter = CreateLLIter(movies);
  Movie* cur_movie;
  LLIterGetPayload(iter, (void**)&cur_movie);

  int result = AddMovieToIndex(movie_index, cur_movie, field_to_index);

  while (LLIterHasNext(iter)) {
    LLIterNext(iter);
    LLIterGetPayload(iter, (void**)&cur_movie);
    result = AddMovieToIndex(movie_index, cur_movie, field_to_index);
  }
  DestroyLLIter(iter);
  return movie_index;
}


int AddMovieActorsToIndex(Index index, Movie *movie) {

  // STEP 6(Student): Add movies to the index via actors.
  //  Similar to STEP 5.
  for (int i = 0; i < movie->num_actors; i++) {
    HTKeyValue kvp;
    kvp.key = ComputeKey(movie, Actor, i);
    HTKeyValue result;
    HTKeyValue *old_kvp = NULL;
    if (LookupInHashtable(index, kvp.key, &result) == 0) {
      MovieSet movieset = result.value;
      LinkedList list = movieset->movies;
      LLIter iter = CreateLLIter(list);
      void* old_kv;
      LLIterGetPayload(iter, &old_kv);
      if ((Movie*)old_kv == movie) {
        DestroyLLIter(iter);
        continue;
      }
      int flag = 0;
      while (LLIterHasNext(iter)) {
        LLIterNext(iter);
        LLIterGetPayload(iter, &old_kv);
        if ((Movie*)old_kv == movie) {
          DestroyLLIter(iter);
          flag = 1;
          break;
        }
      }
      if (flag == 0) {
        DestroyLLIter(iter);
        AddMovieToSet((MovieSet)result.value, movie);
      }
    }
    else {
      char * desc = movie->actor_list[i];
      MovieSet movieset = CreateMovieSet(desc);
      AddMovieToSet(movieset, movie);
      kvp.value = movieset;
      PutInHashtable(index, kvp, old_kvp);
    }
  }
  return 0;
}


int AddMovieToIndex(Index index, Movie *movie, enum IndexField field) {
  if (field == Actor) {
    return AddMovieActorsToIndex(index, movie);
  }

  HTKeyValue kvp;
  kvp.key = ComputeKey(movie, field, 0);

  // STEP 5(Student): How do we add movies to the index?
  // The general idea:
  // Check hashtable to see if relevant MovieSet already exists
  // If it does, grab access to it from the hashtable
  // If it doesn't, create the new MovieSet and get the pointer to it
  // Put the new MovieSet into the Hashtable.
  char *desc;
  if (field == Genre) {
    desc = movie->genre;
  }
  if (field == StarRating) {
    char rating_str[10];
    snprintf(rating_str, 10, "%f", movie->star_rating);
    desc = rating_str;
  }
  if (field == ContentRating) {
    desc = movie->content_rating;
  }
  HTKeyValue result;
  HTKeyValue *old_kvp = NULL;
  if (LookupInHashtable(index, kvp.key, &result) == 0) {
    MovieSet movieset = result.value;
    LinkedList list = movieset->movies;
    LLIter iter = CreateLLIter(list);
    void* old_kv;
    LLIterGetPayload(iter, &old_kv);
    if ((Movie*)old_kv == movie) {
        DestroyLLIter(iter);
        return 0;
      }
    while (LLIterHasNext(iter)) {
        LLIterNext(iter);
        LLIterGetPayload(iter, &old_kv);
        if ((Movie*)old_kv == movie) {
            DestroyLLIter(iter);
            return 0;
          }
      }
    DestroyLLIter(iter);
    AddMovieToSet((MovieSet)result.value, movie);
    return 0;
  }
  MovieSet movieset = CreateMovieSet(desc);
  AddMovieToSet(movieset, movie);
  kvp.value = movieset;
  PutInHashtable(index, kvp, old_kvp);
  return 0;
  // After we either created or retrieved the MovieSet from the Hashtable:
}

uint64_t ComputeKey(Movie* movie, enum IndexField which_field, int which_actor) {
  char rating_str[10];
  switch (which_field) {
    case Genre:
      return FNVHash64((unsigned char*)movie->genre, strlen(movie->genre));
    case StarRating:
      snprintf(rating_str, 10, "%f", movie->star_rating);
      return FNVHash64((unsigned char*)rating_str, strlen(rating_str));
    case ContentRating:
      return FNVHash64((unsigned char*)movie->content_rating,
                       strlen(movie->content_rating));
    case Actor:
      if (which_actor < movie->num_actors) {
        return FNVHash64((unsigned char*)movie->actor_list[which_actor],
                         strlen(movie->actor_list[which_actor]));
      }
      break;
  }
  return -1u;
}

// Removed for simplicity
//MovieSet GetMovieSet(Index index, const char *term){}

int DestroyIndex(Index index) {
  DestroyHashtable(index, &DestroyMovieSetWrapper);
  return 0;
}

Index CreateIndex() {
  return CreateHashtable(128);
}

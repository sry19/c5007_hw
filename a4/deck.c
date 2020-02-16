#include<stdio.h>
#include <stdlib.h>
#include "a4.h"
#define PRINT_DEBUG 1
#include <assert.h>
Deck* CreateDeck() {
  Deck* d = (Deck*) malloc(sizeof(Deck));
    d->top_card = -1;
    return d;
}

Deck* PushCardToDeck(Card* c, Deck* d) {
  if(d->top_card+1>=kNumCardsInDeck) {return d;}
    d->cards[++d->top_card] = c;
    return d;
}

Card* PeekAtTopCard(Deck* d) {
  if(d->top_card == -1 || d->top_card>=kNumCardsInDeck) {return NULL;}
    return d->cards[d->top_card];
}

Card* PopCardFromDeck(Deck* d) {
  if (IsDeckEmpty(d)) {return NULL;}
    Card* popped = d->cards[d->top_card--];
    return popped;
}

int IsDeckEmpty(Deck* deck) {
    if (deck->top_card == -1) {
        return 1;
    }
    else {
        return 0;
    }
}

void DestroyDeck(Deck* deck) {
  // for (int i=0;i<kNumCardsInDeck;i++) {
  //    free(deck->cards[i]);
  //}
  for (int i=0;i<=deck->top_card;i++){
    DestroyCard(deck->cards[i]);
  }
  //  free(deck->cards);
    free(deck);
}

Card* CreateCard(Suit s,Name n) {
  Card* c = (Card*) malloc(sizeof(Card));
  c->name = n;
  c->suit = s;
  c->value = (int) n;
}

void DestroyCard(Card* card) {
  free(card);
}

//?
Deck* PopulateDeck() {//Create all the cards and pushes them into the Deck.
    Deck* d = CreateDeck();
    int i;
    for (i=NINE;i<=ACE;i++) {
        int j;
        for (j=HEARTS;j<=DIAMONDS;j++) {
            Card* card = (Card*) malloc(sizeof(Card));
            card->suit = (enum suit)j;
            card->value = i;
            card->name = (enum name)i;
            PushCardToDeck(card, d);
        }
    }
    return d;
}

//?Takes all the cards in the deck, rearrange the order, and push the cards back into the Deck.
void Shuffle(Deck* d) {
  int len = d->top_card;
  for (int i=0;i<len;i++){
    int r = rand()%len;
    Card* tmp = d->cards[i];
    d->cards[i] = d->cards[r];
    d->cards[r] = tmp;
  }
}


//?Takes a shuffled deck and deals the cards one by one to the each hand.
void Deal(Deck* d, Hand* hand_1, Hand* hand_2) {
    for (int i=0;i<5;i++) {
        Card* card_1 = PopCardFromDeck(d);
        AddCardToHand(card_1, hand_1);
        Card* card_2 = PopCardFromDeck(d);
        AddCardToHand(card_2, hand_2);
    }
}

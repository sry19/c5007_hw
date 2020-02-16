#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#include "a4.h"

#define kPrintDebug 1

// Implement the Hand and other functions in here
Hand* CreateHand() {
  Hand* h = (Hand*) malloc(sizeof(Hand));
    h->first_card = NULL;
    h->num_cards_in_hand = 0;
    return h;
}

void AddCardToHand(Card *card, Hand *hand) {
    CardNode* card_node = (CardNode*) malloc(sizeof(CardNode));
    card_node->next_card = NULL;
    card_node->prev_card = NULL;
    card_node->this_card = card;
    if (hand->first_card == NULL) {
        hand->first_card = card_node;
    } else {
        card_node->next_card = hand->first_card;
        hand->first_card->prev_card = card_node;
        hand->first_card = card_node;
    }
    hand->num_cards_in_hand++;
}

Card* RemoveCardFromHand(Card *card, Hand *hand) {
    if (hand->first_card == NULL) {return NULL;}
    CardNode* cur = hand->first_card;
    if (cur->this_card == card) {
      if (cur->next_card == NULL) {
        free(cur);
        hand->first_card = NULL;
        hand->num_cards_in_hand--;
        return card;
      }
      hand->first_card = hand->first_card->next_card;
      // free(hand->first_card->prev_card);
      if (hand->first_card != NULL) {
        // free(hand->first_card->prev_card);
        hand->first_card->prev_card = NULL;
      }
      free(cur);
        hand->num_cards_in_hand--;
        return card;
    }
    while (cur != NULL && cur->next_card != NULL) {
        if (cur->next_card->this_card == card) {
          CardNode* r = cur->next_card;
            cur->next_card = cur->next_card->next_card;
            if (cur->next_card != NULL) {
              free(cur->next_card->prev_card);
              cur->next_card->prev_card = cur;
            } else {
            free(r);
            }
            hand->num_cards_in_hand--;
            return card;
        }
        cur = cur->next_card;
    }
    Card* cur_card = cur->this_card;
    free(cur);
    return cur_card;
}

int IsHandEmpty(Hand* hand) {
    if (hand->num_cards_in_hand == 0) {
        return 1;
    } else {
        return 0;
    }
}

void DestroyHand(Hand* hand) {
    CardNode* cur = hand->first_card;
    while (cur != NULL) {
        free(cur->prev_card);
        cur = cur->next_card;
    }
    free(cur);
    free(hand);
}

int IsLegalMove(Hand *hand, Card *leadCard, Card *playedCard) {
    if (leadCard->suit == playedCard->suit) {
        return 1;
    }
    CardNode* cur = hand->first_card;
    while (cur != NULL) {
        if (cur->this_card->suit == leadCard->suit) {
            return 0;
        }
        cur = cur->next_card;
    }
    return 1;
}

int WhoWon(Card *leadCard, Card *followedCard, Suit trump) {
    if (leadCard->suit == followedCard->suit) {
        if (leadCard->name>followedCard->name) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if (followedCard->suit == trump) {
            return 0;
        }
        return 1;
    }
}

void ReturnHandToDeck(Hand *hand, Deck *deck) {
  CardNode* n = hand->first_card;
  while (n != NULL) {
    if (n->prev_card) {
      PushCardToDeck(n->prev_card->this_card, deck);
    }
    n = n->next_card;
    free(n->prev_card);
  }
  free(hand);
}

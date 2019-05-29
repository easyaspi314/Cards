#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "cards.h"

const int CARDS_IN_DECK = 52;

static void initialize(int array[SUITS_NUMBER][CARDS_PER_SUIT]);
static void set_seed(int *check);
static Card next_available(Card card, Deck *deck);

int seed_checker = 1; /*
                         This variable, together with the set_seed function,
                         is needed not to set the srand seed more than once
                      */



/*
   Creates a new deck given user parameters and returns it.
   deck_nmb refers to the number of unique decks of which the new deck will be made out of.
   For example, usually in blackjack 6 decks are used at once, all shuffled between eachother,
   so deck_nmb would be equal to 6.
   jokers = number of jokers in the deck
   shuffle_markers = Number of shuffle markers in the deck. Once a shuffle marker card is drawn,
   all the cards are shuffled back into the deck, including the shuffle marker.
*/
Deck create_deck(int deck_nmb, int jokers, int shuffle_markers){
  Deck new_deck = {deck_nmb * CARDS_IN_DECK, jokers, shuffle_markers};
  initialize(new_deck.checker.cards); /* Initializes the cards array, setting all values to 0. This
                                         indicates that no card has been drawn yet */
  return new_deck;
}

Card draw(Deck *deck){
  /*
     If the set_seed function is called for the first time, it sets the srand seed, else it
     does nothing. It checks if it is called for the first time by looking at the seed_checker
     variable, passed along as a parameter. If it is equal to 1, set_seed has been called for the
     first time.
  */
  set_seed(&seed_checker);

  /*
     Before doing anything, the function checks if there are actually any cards to
     draw inside the deck. If all the cards have already been drawn, it shuffles the deck.
     This is done by simply resetting the deck.checker.cards array and to 0, together with
     the deck.checker.jokers variable
  */
  if(all_cards_drawn(*deck) && all_jokers_drawn(*deck)){
    printf("All cards have been drawn. Shuffling...\n");
    initialize(deck->checker.cards);
    deck->checker.jokers = 0;
    printf("The deck has been shuffled\n");
  }

  /*
     total_cards is actually equal to the cards that have NOT been drawn in the deck.
     This is done to speed the drawing process up and to reduce useless draws to a minimum, as
     if a card is drawn but no more copies of that card should exist in the deck, the deck.checker.cards
     array is gone through, looking for the first available card after the randomly drawn card.
  */
  int total_cards = available_cards(*deck);
  Card drawn_card;

  /*
     Draws a random card. The numbers from 0 to (deck.jokers - deck.checker.jokers - 1) will show that
     a joker has been drawn. deck.jokers - deck.checker.jokers indicates the number of the jokers that are
     yet to be drawn.
     The numbers from (deck.jokers - deck.checker.jokers) to (deck.shuffle_markers + (deck.jokers - deck.checker.jokers) - 1)
     show that a shuffle marker has been drawn, and all the other numbers that a regular
     card has been drawn.
     If a card is drawn but no more copies of that card should exist in the
     deck, the first available card after the drawn card is returned.
     EXAMPLE:
     drawn card = 6 of Spades - ALREADY DRAWN
     7 of Spades - ALREADY DRAWN
     8 of Spades - ALREADY DRAWN
     9 of Spades - NOT DRAWN
     return 9 of Spades

     To check weather all of the drawn card's occurencies have already been drawn,
     the deck.checker variable is used. Keep in mind that the maximum number of occurencies that
     a card can have is (deck.cards_nmb / CARDS_IN_DECK).
  */

  int card = rand() % total_cards;

  if(card < (deck->jokers - deck->checker.jokers)){
    (deck->checker.jokers)++; //increase the number of the drawn jokers
    drawn_card.suit = JOKER;
    drawn_card.value = JOKER_VALUE;
    return drawn_card;
  }
  else if(card >= (deck->jokers - deck->checker.jokers) && card < deck->shuffle_markers + (deck->jokers - deck->checker.jokers)){
    drawn_card.suit = SHUFFLE_MARKER;
    drawn_card.value = SHUFFLE_MARKER_VALUE;

    printf("A shuffle marker has been drawn. Shuffling...\n");
    /*
      To "shuffle the deck" I'm simply resetting all the variables in the
      deck.checker variable to 0, this includes the jokers.
    */
    initialize(deck->checker.cards);
    deck->checker.jokers = 0;
    printf("The deck has been shuffled.\n\n");

    return drawn_card;
  }
  else{
    /*
       In this case, a valid card has been drawn. The actual drawn number doesn't
       really matter, as I'm now doing two specifics "draws": one for the suit and
       one for the face value of the card.
    */
    drawn_card.suit = rand() % 4;
    drawn_card.value = rand() % 13;
    if(deck->checker.cards[drawn_card.suit][drawn_card.value] < deck->cards_nmb / CARDS_IN_DECK){
      (deck->checker.cards[drawn_card.suit][drawn_card.value])++; // Counts the drawn_card as drawn.
      return drawn_card;
    }
  }

  /*
     If the function reaches this point, the drawn card has already reached the maxium number of possible occurencies.
     In this case, the first available card after the drawn card is returned. The next_available() function purpose
     is to do just that.
     EXAMPLE:
     drawn card = 6 of Spades - ALREADY DRAWN
     7 of Spades - ALREADY DRAWN
     8 of Spades - ALREADY DRAWN
     9 of Spades - NOT DRAWN
     return 9 of Spades
  */
  drawn_card = next_available(drawn_card, deck);
  (deck->checker.cards[drawn_card.suit][drawn_card.value])++; // Counts the drawn_card as drawn.
  return drawn_card;
}

/*
   Finds the first card that hasn't reached the maximum number of occurencies yet
   given a random starting position (the card variable passed as a parameter).
   This is done to speed up the drawing process while still providing a random draw.
*/
static Card next_available(Card card, Deck *deck){
  Card new_card;
  int first_run = 1;

  /*
    This first for loop has to wind around the array. The first_run variable is used so
    that when the for loop goes onto analyzing the next row after card.suit, it starts from
    the very first column, and not the (card.value)th
  */
  for(int i = card.suit; i < SUITS_NUMBER; i++){
    if(first_run){
      for(int k = card.value; k < CARDS_PER_SUIT; k++){
        if(deck->checker.cards[i][k] < deck->cards_nmb / CARDS_IN_DECK){
          new_card.suit = i;
          new_card.value = k;
          return new_card;
        }
      }
      first_run = 0;
    }
    else{
      for(int k = 0; k < CARDS_PER_SUIT; k++){
        if(deck->checker.cards[i][k] < deck->cards_nmb / CARDS_IN_DECK){
          new_card.suit = i;
          new_card.value = k;
          return new_card;
        }
      }
    }
  }

  /*
     After the for loop above has gone through, the rows before card.suit are still left unchecked,
     and that's what the following for loop does. The <= is used instead of the < because there might
     still be some values (the ones before card.value) in the same row as card.suit that might have been
     left unchecked from the previous for loop.
  */
  for(int i = 0; i <= card.suit; i++){
    for(int k = 0; k < CARDS_PER_SUIT; k++){
      if(deck->checker.cards[i][k] < deck->cards_nmb / CARDS_IN_DECK){
        new_card.suit = i;
        new_card.value = k;
        return new_card;
      }
    }
  }

  /*
     If the function hasn't returned anything at this point, the deck is empty.
     As the draw function automatically recognizes an empty deck of cards, shuffling it
     and returning a newly drawn carn, the draw function is called.
     Note that, in theory, this should never happen and the following two lines are added
     for safety purpose only and to silence some warnings.
  */
  new_card = draw(deck);
  return new_card;
}

// Returns the number of cards that have NOT been drawn in the deck
int available_cards(Deck deck){
  int cards_count = 0;
  int maximum_occurencies = deck.cards_nmb / CARDS_IN_DECK;

  cards_count += (deck.jokers - deck.checker.jokers);

  for(int i = 0; i < SUITS_NUMBER; i++){
    for(int k = 0; k < CARDS_PER_SUIT; k++){
      cards_count += (maximum_occurencies - (deck.checker.cards)[i][k]);
    }
  }

  return cards_count;
}

// Cheks if all the jokers in the deck passed as a parameter have been drawn.
int all_jokers_drawn(Deck deck){
  if(deck.checker.jokers == deck.jokers){
    return 1;
  }
  return 0;
}

/*
   Cheks if all the cards (shuffle markers and jokers not included) in the deck passed as a parameter
   have been drawn. It does so by checking if all the values in the deck.checker.cards array have
   reached the maximum number of occurencies.
*/
int all_cards_drawn(Deck deck){
  int max_occurencies = deck.cards_nmb / CARDS_IN_DECK; // Maximum number of occurencies that a card can have.

  for(int i = 0; i < SUITS_NUMBER; i++){
    for(int k = 0; k < CARDS_PER_SUIT; k++){
      if((deck.checker.cards)[i][k] < max_occurencies){
        return 0;
      }
    }
  }
  return 1;
}

// Returns a string literal for the suit of the card passed as a parameter.
char *suit(Card card){
  switch(card.suit){
    case 0: return "Hearts";
    case 1: return "Diamonds";
    case 2: return "Spades";
    case 3: return "Clubs";
    case 4: return "Joker";
    default: printf("Error: the card has an invalid suit\n");
             return "ERROR - SUIT";

  }
}

// Returns a string literal for the value of the card passed as a parameter.
char *value(Card card){
  switch(card.value){
    case 0: return "Ace";
    case 1: return "Two";
    case 2: return "Three";
    case 3: return "Four";
    case 4: return "Five";
    case 5: return "Six";
    case 6: return "Seven";
    case 7: return "Eight";
    case 8: return "Nine";
    case 9: return "Ten";
    case 10: return "Jack";
    case 11: return "Queen";
    case 12: return "King";
    case 13: return "Joker";
    default: printf("Error: the card has an invalid face value\n");
             return "ERROR - VALUE";
  }
}

// Initializes all the values of an array to 0
static void initialize(int array[SUITS_NUMBER][CARDS_PER_SUIT]){
  for(int i = 0; i < SUITS_NUMBER; i++){
    for(int k = 0; k < CARDS_PER_SUIT; k++){
      array[i][k] = 0;
    }
  }
}

/*
   If the set_seed function is called for the first time, it sets the srand seed, else it
   does nothing. It checks if it is called for the first time by looking at the seed_checker
   variable, passed along as a parameter. If it is equal to 1, set_seed has been called for the
   first time.
*/
static void set_seed(int *check){
  if(*check == 1){
    *check = 0;
    srand(time(NULL));
  }
}

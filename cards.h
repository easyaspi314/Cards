#ifndef CARDS_H
#define CARDS_H

#define CARDS_PER_SUIT  13
#define SUITS_NUMBER 4

typedef enum {HEARTS, DIAMONDS, SPADES, CLUBS, JOKER, SHUFFLE_MARKER} Suit;
typedef enum {ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, JOKER_VALUE, SHUFFLE_MARKER_VALUE} Value;

typedef struct{
  Suit suit;
  Value value; // Face value of the card
} Card;

/*
   The point of the Drawn_cards struct is to keep track of all the cards that have been
   drawn in a specific deck, so that already drawn card aren't drawn again.
   Cards are saved into the cards array in this order: ace to king, hearts, diamonds,
   spades and clubs. If a card is drawn but no more copies of that card should exist in the
   deck, it is simply discarded and a redraw occurs. Keep in mind that the maximum number of
   occurencies that a card can have is (deck.cards_nmb / CARDS_IN_DECK).
*/
typedef struct{
  int cards[SUITS_NUMBER][CARDS_PER_SUIT];
  int jokers;
} Drawn_cards;

typedef struct{
  int cards_nmb;            // Number of cards in the deck
  int jokers;               // Number of jokers in the deck
  int shuffle_markers;      /*
                               Number of shuffle markers in the deck. Once a shuffle marker card is drawn,
                               all the cards are shuffled back into the deck, including the shuffle marker.
                            */
  Drawn_cards checker;
} Deck;


/*
   Creates a new deck given user parameters and returns it.
   deck_nmb refers to the number of unique decks of which the new deck will be made out of.
   For example, usually in blackjack 6 decks are used at once, all shuffled between eachother,
   so deck_nmb would be equal to 6.
   jokers = number of jokers in the deck
   shuffle_markers = Number of shuffle markers in the deck. Once a shuffle marker card is drawn,
   all the cards are shuffled back into the deck, including the shuffle marker.
*/
Deck *create_deck(int deck_nmb, int jokers, int shuffle_markers);

/*
   Draws a card from the deck passed as a parameter and returns it. To track the number of the drawn cards
   from the deck, the decks needs to be passed as a pointer.
   For detaield infos on how a random card is drawn, check the cards.c file.
*/
Card draw(Deck *deck);

/*
   Shuffles the deck passed as a parameter. This implies resetting the deck.checker.cards array and the
   deck.checker.jokers variable. "Shuffling", in this case, does NOT mean shuffling all the cards that
   are yet to be drawn in the deck. Think of it as collecting all the cards that have already been dealt
   and shuffling them back with the cards that haven't been drawn.
*/
void shuffle(Deck *deck);

// Returns the number of cards that have NOT been drawn in the deck
int available_cards(Deck *deck);

/*
   Cheks if all the cards (shuffle markers and jokers not included) in the deck passed as a parameter
   have been drawn. It does so by checking if all the values in the deck.checker.cards array have
   reached the maximum number of occurencies.
*/
int all_cards_drawn(Deck *deck);

// Cheks if all the jokers in the deck passed as a parameter have been drawn.
int all_jokers_drawn(Deck *deck);

// Returns a string literal for the value of the card passed as a parameter.
const char *value(Card card);

// Returns a string literal for the suit of the card passed as a parameter.
const char *suit(Card card);

#endif

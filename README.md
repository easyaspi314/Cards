# Cards

Implements some useful functions and elements to use a deck of cards in your favourite C projects!

# Some basic infos about the types

Given a ```Card new_card``` variable, you can access and assign its suit and face value with ```Suit suit = new_card.suit``` and ```Value value = new_card.value```. The following suits and values are supported:

_SUITS:_
- HEARTS
- DIAMONDS
- SPADES
- CLUBS
- JOKER
- SHUFFLE_MARKER

_VALUES:_
- ACE
- TWO
- THREE
- FOUR
- FIVE
- SIX
- SEVEN
- EIGHT
- NINE
- TEN
- JACK
- QUEEN
- KING
- JOKER_VALUE
- SHUFFLE_MARKER_VALUE

You can get the string literal associated with a card's suit or value with ```char *new_suit = suit(new_card)```and ```char *new_value = value(new_card)``` please note that you **can't** get the suit literal or value literal of a shuffle marker.

Given a ```Deck *new_deck``` variable you can access the number of its cards (drawn + NOT drawn, jokers and shuffle markers not included) with ```new_deck->cards_nmb```, the number of jokers (drawn + NOT drawn) with ```new_deck->jokers``` and the number of shuffle markers with ```new_deck->shuffle_markers```.

# Brief tutorial

**_To create a new deck:_**

```c
Deck *new_deck = create_deck(NUMBER_OF_DECKS, NUMBER_OF_JOKERS, NUMBER_OF_SHUFFLE_MARKERS);
```
_NUMBER_OF_DECKS_ indicates the number of decks the new deck will be made out of. For example, keeping in mind that a standard deck of playing cards has 52 cards (13 per suit), if NUMBER_OF_DECKS = 3 the new_deck will be made out of 3 * 52 cards, with 3 copies of each card.
_NUMBER_OF_JOKERS_ indicates the number of jokers present in the deck.
_NUMBER_OF_SHUFFLE_MARKERS_ specifies how many shuffle markers are there in the deck. Once a shuffle marker is drawn, all the previously dealt card get shuffled back in the deck, including all the shuffle markers. Drawing a shuffle marker is like "resetting the deck".

**_To draw a card:_**

```c
Card drawn_card = draw(new_deck);
```

Where deck is the deck from which you want your card to be drawn from.

_The decks get shuffled automatically once all cards have been drawn from it_

**_To check the number of cards yet to be drawn_**

```c
int not_drawn = available_cards(new_deck);
``` 

**_To shuffle a deck_**

```c
shuffle(new_deck);
```

Keep in mind that shuffling here means taking all the dealt cards and shuffling them back into the deck.

# In conclusion

I hope you find this library useful, I had lots of fun writing this! I recommend you check the _cards.h_ header to discover more functions that you may find useful and the _cards.c_ file to have a more in depth understanding of how everything works. I tried to make each function as efficient as possible, minimizing wasted time and CPU use while giving you the same odds of drawing a card that you would normally get if you drew a card with a real deck of cards.  I am open to suggestions on new functions to implement or on how to make things more efficient... _enjoy!_



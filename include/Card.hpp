#pragma once

enum class Card_Color : unsigned int {SPADE, HEART, CLUB, DIAMOND, None}; 
enum class Card_Figure : unsigned int {A, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, None};

class Card {
  Card_Color color;
  Card_Figure figure;
public:
  explicit Card(Card_Figure newFigure = Card_Figure::None,
                Card_Color newColor = Card_Color::None)
      : color(newColor), figure(newFigure) {};
  const Card_Color getColor();
  const Card_Figure getFigure();
  bool operator ==(const Card& card);
  bool operator !=(const Card& card);
};

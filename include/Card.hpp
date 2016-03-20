#pragma once

enum class Card_Color : unsigned int {None = 0, SPADE, HEART, CLUB, DIAMOND}; 
enum class Card_Figure : unsigned int {None = 0, A, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};

class Card {
  Card_Color color;
  Card_Figure figure;
public:
  constexpr explicit Card(Card_Figure newFigure = Card_Figure::None,
                          Card_Color newColor = Card_Color::None) :color(newColor), figure(newFigure) {};
  constexpr Card_Color getColor() const { return color; }
  constexpr Card_Figure getFigure() const { return figure; }
  bool operator ==(const Card& card) const;
  bool operator !=(const Card& card) const;
};

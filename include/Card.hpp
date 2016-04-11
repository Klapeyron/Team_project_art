#pragma once

enum class Card_Color : unsigned int {None = 0, SPADE, HEART, CLUB, DIAMOND}; 
enum class Card_Figure : unsigned int {None = 0, A, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};

inline Card_Color operator++(Card_Color & x) { return x = static_cast<Card_Color>(static_cast<unsigned int>(x) + 1); }
inline Card_Color operator*(Card_Color x) {return x;}
inline Card_Color begin(Card_Color x) {return Card_Color::SPADE;}
inline Card_Color end(Card_Color x) {return static_cast<Card_Color>(static_cast<unsigned int>(Card_Color::DIAMOND) + 1);}

inline Card_Figure operator++(Card_Figure & x) {return x = static_cast<Card_Figure>(static_cast<unsigned int>(x) + 1);}
inline Card_Figure operator*(Card_Figure x) {return x;}
inline Card_Figure begin(Card_Figure x) {return Card_Figure::A;}
inline Card_Figure end(Card_Figure x) {return static_cast<Card_Figure>(static_cast<unsigned int>(Card_Figure::KING) + 1);}

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

#include "TableSubject.hpp"

void TableSubject::notify(TableSnapshot const& snapshot) {
  for(auto const& observer : observers)
    observer->onUpdate(snapshot);
}

void TableSubject::registerOberver(TableObserver& observer) {
   observers.push_back(&observer);
}

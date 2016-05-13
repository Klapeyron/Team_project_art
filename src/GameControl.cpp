 #include "GameControl.hpp"
void GameControl::pressPass()
{
  std::cout << "pressPass" << std::endl;
  setPosition(tableSnapshot.buttons[ButtonsConstants::PAS_BUTTON].second);
}
void GameControl::pressKnock()
{
  std::cout << "pressKnock" << std::endl;
  setPosition(tableSnapshot.buttons[ButtonsConstants::KNOCK_KNOCK_BUTTON].second);
}
void GameControl::onUpdate(TableSnapshot const& snapshot)
{
  tableSnapshot = snapshot;
}

void GameControl::pickCardFromHiddenStack()
{
  std::cout << "pickCardFromHiddenStack" << std::endl;
  setPosition(tableSnapshot.buttons[ButtonsConstants::HIDDEN_STACK].second);
}

void GameControl:: pickCardFromStack()
{
  std::cout << "pickCardFromStack" << std::endl;
  Position pos(tableSnapshot.stackCard.getX(),tableSnapshot.stackCard.getY());
  setPosition(pos);
}

void GameControl:: throwMyCard(Card const &card)
{
  std::cout << "throwMyCard " << card << std::endl;
  Position pos(card.getX(),card.getY());
  setPosition(pos);
}

void GameControl:: endGame()
{
  std::cout << "endGame" << std::endl;
  setPosition(tableSnapshot.buttons[ButtonsConstants::KNOCK_KNOCK_BUTTON].second);
}

void GameControl::touchCard(const Card &card)
{
  std::cout << "touchCard " << card << std::endl;
  Position pos(card.getX(),card.getY());
  setPosition(pos);
}

void GameControl::pressOK()
{
  std::cout << "pressOk" << std::endl;
  setPosition(tableSnapshot.buttons[ButtonsConstants::OK_BUTTON].second);
}

void GameControl::setPosition(const Position & position)
{
 Display *display = XOpenDisplay(0);
 Window root = DefaultRootWindow(display);
 XWarpPointer(display, None, root, 0, 0, 0, 0, position.getX(), position.getY());
 mouseClick(Button1);
}

void GameControl::mouseClick(int button)
{
    Display *display = XOpenDisplay(0);

    XEvent event;

    if(display == NULL)
    {
      std::cerr<< "Error" << std::endl;
      exit(-1);
    }

    memset(&event, 0x00, sizeof(event));

    event.type = ButtonPress;
    event.xbutton.button = button;
    event.xbutton.same_screen = True;

    XQueryPointer(display, RootWindow(display, DefaultScreen(display)), 
		  &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, 
		  &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

    event.xbutton.subwindow = event.xbutton.window;

    while(event.xbutton.subwindow)
    {
        event.xbutton.window = event.xbutton.subwindow;

        XQueryPointer(display, event.xbutton.window, &event.xbutton.root, 
		      &event.xbutton.subwindow, &event.xbutton.x_root, 
		      &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    }

    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) 
      std::cerr << "Error\n" << std::endl;

    XFlush(display);

    usleep(100000);

    event.type = ButtonRelease;
    event.xbutton.state = 0x100;

    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) 
      std::cerr << "Error\n" << std::endl;

    XFlush(display);
    XCloseDisplay(display);
}


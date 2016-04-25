 #include "GameControl.hpp"

void GameControl::onUpdate(TableSnapshot const& snapshot)
{
  tableSnapshot = snapshot;
}

void GameControl::pickCardFromHiddenStack()
{
  setPosition(ButtonsSnapshot::getButtonPosition(HIDDEN_STACK_BUTTON));
}

void GameControl:: pickCardFromStack()
{
  setPosition(ButtonsSnapshot::getButtonPosition(STACK_BUTTON));
}

void GameControl:: throwMyCard(const Card &card)
{
  Position pos(card.getX(),card.getY());
  setPosition(pos);
}

void GameControl:: endGame()
{
  setPosition(ButtonsSnapshot::getButtonPosition(END_GAME_BUTTON));
}

void GameControl::touchCard(const Card &card)
{
  Position pos(card.getX(),card.getY());
  setPosition(pos);
}

void GameControl::pressOK()
{
  setPosition(ButtonsSnapshot::getButtonPosition(OK_BUTTON));
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


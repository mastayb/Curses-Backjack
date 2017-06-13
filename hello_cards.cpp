#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <array>
#include <memory>
#include <functional>
#include <cstring>
#include <utility>

static void finish(int sig);



auto CreateWindow (int max_x, int max_y, int x=0, int y=0)
{
    auto delwin_lambda = [](WINDOW* w) {
        delwin(w);
    };
    return std::unique_ptr<WINDOW, decltype(delwin_lambda)> (newwin(max_x,max_y,x,y), delwin_lambda);
}

enum Suit {HEARTS,DIAMONDS,SPADES,CLUBS};

static const int bigCardWidth=15, bigCardHeight=9;


auto TenCard()
{
   auto width = 15, height = 9;
   auto pat = {"x   x",
               "  x  ",
               "x   x",
               "     ",
               "x   x",
               "  x  ",
               "x   x"};

   auto patStartRow = 1;
   auto patStartCol = 5;

   auto num = "10";
   const char suit[] = "\xE2\x99\xA5";

   auto topLeft = std::make_pair(0,0);
   auto bottomRight = std::make_pair(height-1, width-std::strlen(num));

   auto w = CreateWindow(height, width);

   //Add numbers
   mvwaddstr(w.get(), topLeft.first,topLeft.second, num);
   mvwaddstr(w.get(), bottomRight.first, bottomRight.second, num);

   //Add pattern
   auto row = 0;
   for (auto line : pat) {
      for (size_t col=0;col<std::strlen(line);col++) {
         if (line[col] == 'x') mvwaddstr(w.get(), row+patStartRow, col+patStartCol, suit);
      }
      row++;
   }
   
   return w;
}






auto CreateCard()
{
    auto width=bigCardWidth;
    auto height=bigCardHeight;

    auto w = CreateWindow(height,width);

    const char cornerNum[] = "10";
    const char suit[] = "\xE2\x99\xA0"; //"\x2660"  "♠";
    auto cntr = width/2;
    auto lcol= cntr-2;
    auto rcol = cntr+2;
    auto r = 1;
    mvwaddstr(w.get(),0,0, cornerNum);
    mvwaddstr(w.get(), r,lcol, suit);
    mvwaddstr(w.get(), r,rcol, suit);
    mvwaddstr(w.get(), ++r,cntr, suit);
    mvwaddstr(w.get(), ++r,lcol, suit);
    mvwaddstr(w.get(), r,rcol, suit);
    ++r;
    mvwaddstr(w.get(), ++r,lcol, suit);
    mvwaddstr(w.get(), r,rcol, suit);
    mvwaddstr(w.get(), ++r,cntr, suit);
    mvwaddstr(w.get(), ++r,lcol, suit);
    mvwaddstr(w.get(), r,rcol, suit);
    mvwaddstr(w.get(), height-1,width-2, cornerNum);

    return w;
}

auto CreateCard2()
{
    const int width=5,height=3;
    auto w = CreateWindow(height,width);

    const char cornerNum[] = "10";
    const char suit[] = "\xE2\x99\xA0"; //"\x2660"; "♠";

    mvwaddstr(w.get(),0,0, cornerNum);
    mvwaddstr(w.get(), 1,2, suit);
    mvwaddstr(w.get(), 2,3, cornerNum);

    return w;
}



int
main(__attribute__((unused)) int argc, __attribute__((unused)) char *argv[])
{
    signal(SIGINT, finish);      /*w arrange interrupts to terminate */

    setlocale(LC_ALL, "");
    initscr();      /* initialize the curses library */
    keypad(stdscr, TRUE);  /* enable keyboard mapping */
    nonl();         /* tell curses not to do NL->CR/NL on output */
    cbreak();       /* take input chars one at a time, no wait for \n */
    noecho();
    curs_set(FALSE);
    refresh();

    enum {BLACK_ON_WHITE=1, RED_ON_WHITE, YELLOW_ON_GREEN};
    start_color();
    init_pair(BLACK_ON_WHITE, COLOR_BLACK, COLOR_WHITE);
    init_pair(RED_ON_WHITE, COLOR_RED, COLOR_WHITE);
    init_pair(YELLOW_ON_GREEN, COLOR_YELLOW, COLOR_GREEN);

    init_color(COLOR_RED, 175, 0, 0);
    init_color(COLOR_BLACK, 0,0,0);
    init_color(COLOR_GREEN, 0,135,0);
    init_color(COLOR_WHITE,255,255,255);
    init_color(COLOR_YELLOW, 255,215,0);

    //int max_x,max_y;
    //getmaxyx(stdscr, max_y, max_x);
    //
    bkgd( COLOR_PAIR(YELLOW_ON_GREEN));
    refresh();


    auto card = CreateCard();
    wbkgd(card.get(), COLOR_PAIR(BLACK_ON_WHITE));
    mvwin( card.get(), 1, 1);
    wrefresh(card.get());

    auto card2 = CreateCard2();
    wbkgd(card2.get(), COLOR_PAIR(BLACK_ON_WHITE));
    mvwin( card2.get(), 1, bigCardWidth+2);
    wrefresh(card2.get());
    
    auto card3 = TenCard();

    wbkgd(card3.get(), COLOR_PAIR(RED_ON_WHITE));
    mvwin(card3.get(), 1, bigCardWidth + 2 + 4 +2);
    wrefresh(card3.get());

    getch();



    finish(0);               /* we're done */
}

static void finish(__attribute__((unused)) int sig)
{
    endwin();

    /* do your non-curses wrapup here */
    if(can_change_color())    system("reset");

    exit(0);
}

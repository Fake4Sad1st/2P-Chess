#include "open_screen.hpp"
#include "match.hpp"

int main(int argc, char* args[]){
    // init game, in a badway
    Basic::instance();

    OpenScreen firstScreen;
    Match matchScreen;

    while(1){
        firstScreen.mainEvent();
        if(Basic::instance().askQuit()) break;

        matchScreen.mainEvent();
        if(Basic::instance().askQuit()) break;
    }

    Basic::free();
	return 0;
}

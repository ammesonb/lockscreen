#include <stdio.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/extensions/scrnsaver.h>

void printIndex(char* string, int index) {
    printf("%c\n", string[index]);
    return;
}

int main(int argc, char *argv[]) {
    // Authentication string
    char* passString = "0abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567";

    // Set up X stuff
    Display *display;
    Window win;

    display = XOpenDisplay(NULL);
    if (!display) {fprintf(stderr, "Failed to open display!\n");}

    // Root window means all windows
    win = RootWindow(display, DefaultScreen(display));
    int looping = 0;
        
    // Loop forever
    while (1) {
        looping = 0;
        // info.idle is idle time in ms
        XScreenSaverInfo info;
        XScreenSaverQueryInfo(display, win, &info);
        sleep(2);
    
        // If system idle for 10 minutes and it isn't looping, 
        // which it really shouldn't be
        if (info.idle > 600000 && looping == 0) {
            looping = 1;
            // Technically, a 1 in position 2 (which are 0s now) of both of these functions will allow events
            // to propagate as normal. However, both 0 and 1 seem to block events, which is desirable in this context.
            XGrabKeyboard(display, win, 0, GrabModeAsync, GrabModeAsync, CurrentTime);
            XGrabPointer(display, win, 0, 0, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
        
            XEvent event;
        
            int loop = 1;
            time_t last = time(NULL);
            int correct = 0;
            while (loop) {
                // Get next event (or block until then)
                XNextEvent(display, &event);
                // If last key press was over 30 seconds ago, reset
                if (event.type == KeyPress) {
                    if ((last + 30) < time(NULL)) {
                        correct = 0;
                    }
                    // Classes to facilitate reading of keys
                    XKeyEvent* ke = (XKeyEvent*)&event;
                    KeySym keysym;
                    // Get human-readable string from event
                    XLookupString(ke, NULL, 0, &keysym, NULL);
        
                    // Escape key resets
                    if (keysym == 65307) {correct = 0; continue;}
        
                    time_t rawtime;
                    struct tm* timeinfo;
                    time(&rawtime);
                    timeinfo = localtime(&rawtime);
                    // Different states - modify this to whatever authentication you want to use
                    switch(correct) {
                        int index;
                        case 0:
                            index = (int)timeinfo->tm_year % 100;
                            printIndex(passString, index);
                            if (keysym == passString[index]) {
                                correct++;
                            }
                            break;
                        case 1:
                            index = (int)timeinfo->tm_mon + 1;
                            printIndex(passString, index);
                            if (keysym == passString[index]) {
                                correct++;
                            }
                            break;
                        case 2:
                            index = (int)timeinfo->tm_mday;
                            printIndex(passString, index);
                            if (keysym == passString[index]) {
                                correct++;
                            }
                            break;
                        case 3:
                            index = (int)timeinfo->tm_hour;
                            printIndex(passString, index);
                            if (keysym == passString[index]) {
                                correct++;
                            }
                            break;
                        case 4:
                            index = (int)timeinfo->tm_min;
                            printIndex(passString, index);
                            if (keysym == passString[index]) {
                                correct++;
                                loop = 0;
                            }
                            break;
                        case 5:
                            loop = 0;
                            break;
                    }
        
                    if (keysym == 'q') {
                        loop = 0;
                    }
                    // Reset time counter
                    last = time(NULL);
                }
            }
            XUngrabKeyboard(display, CurrentTime);
            XUngrabPointer(display, CurrentTime);
            //XCloseDisplay(display);
            //return 0;
        }
    }
}

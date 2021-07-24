#include "widget.hpp"
#include <QApplication>
#include <iostream>
#include <signal.h>

static int setup_unix_signal_handlers();

int main(int argc, char *argv[])
{
    std::cout << "Program 1" << std::endl;
    QApplication a(argc, argv);

    std::cout << "Program 2" << std::endl;
    Widget w;

    std::cout << "Program 3" << std::endl;
    w.show();

    setup_unix_signal_handlers();

    std::cout << "Program 4" << std::endl;
    int ret_val = a.exec();

    std::cout << "Program dont't crash" << std::endl;
    return ret_val;
}


static int setup_unix_signal_handlers()
{
    struct sigaction hup, term;

    hup.sa_handler = Widget::hupSignalHandler;
    sigemptyset(&hup.sa_mask);
    hup.sa_flags = 0;
    hup.sa_flags |= SA_RESTART;

    if (sigaction(SIGHUP, &hup, 0))
       return 1;

    term.sa_handler = Widget::termSignalHandler;
    sigemptyset(&term.sa_mask);
    term.sa_flags = 0;
    term.sa_flags |= SA_RESTART;

    if (sigaction(SIGTERM, &term, 0))
       return 2;

    return 0;
}

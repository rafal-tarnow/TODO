#include "widget.hpp"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << "Program 1" << std::endl;
    QApplication a(argc, argv);

    std::cout << "Program 2" << std::endl;
    Widget w;

    std::cout << "Program 3" << std::endl;
    w.show();

    std::cout << "Program 4" << std::endl;
    int ret_val = a.exec();

    std::cout << "Program dont't crash" << std::endl;
    return ret_val;
}

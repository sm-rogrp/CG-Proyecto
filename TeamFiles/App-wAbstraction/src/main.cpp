#include <iostream>
#include "app.hpp"

AppOpenGL app;

int main(){

    app.init();
    app.setup();

    while (!app.shouldClose())
        app.display();

    app.cleanAll();
}

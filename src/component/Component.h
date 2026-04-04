#pragma once

namespace Component {
    class Component {
    public:
        Component();
        virtual ~Component();

        virtual void start();
        virtual void update();
    };
}
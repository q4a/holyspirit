#ifndef TAB_HPP
#define TAB_HPP

#include "Button.hpp"

class Tab : public Button
{
    public:
        Tab(std::string label = "");
        Tab(int, int, std::string label = "");
        Tab(int, int, int, int, std::string label = "");
        virtual ~Tab();

        virtual void Update();

        virtual void SetGeometry(int x, int y, int w, int h);

    protected:

    private:

};

#endif

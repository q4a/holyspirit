#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include <GUI_Object.h>


class GUI_Button : public GUI_Object
{
    public:
        GUI_Button();
        virtual ~GUI_Button();

        bool getState();

    protected:
    private:
    bool m_is_clicked;
};

#endif // GUI_BUTTON_H

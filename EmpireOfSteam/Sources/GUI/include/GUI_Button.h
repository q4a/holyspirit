#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include "../include/GUI_Object.h"


class GUI_Button : public GUI_Object
{
    public:
        GUI_Button();
        virtual ~GUI_Button();

        bool GetState();
        void Update();

    protected:
    private:
    bool m_is_clicked;
};

#endif // GUI_BUTTON_H

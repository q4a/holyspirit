#ifndef INPUT_HPP
#define INPUT_HPP

#include "GUIImage.hpp"
#include "Label.hpp"

class GUIInput : public Widget
{
    public:
        GUIInput(std::string label = "");
        GUIInput(int, int, std::string label = "");
        GUIInput(int, int, int, int, std::string label = "");
        virtual ~GUIInput();

        virtual void Update();

                void SetText(std::string );

                void SetMax (int );

        Label m_label;

    protected:
        int m_max;

    private:
        int     m_last_event;
        float   m_repetition_time;

        float   m_cur_time;
        int     m_cur_pos;

        std::string m_text;

};

#endif


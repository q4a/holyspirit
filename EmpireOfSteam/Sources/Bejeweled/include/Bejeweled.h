#ifndef BEJEWELED_H
#define BEJEWELED_H


class Bejeweled
{
    public:
        Bejeweled();
        virtual ~Bejeweled();

        void Update();

        void Generate(sf::vector2 size);

        const sf::Image &GetImage();

    protected:
    private:
};

#endif // BEJEWELED_H

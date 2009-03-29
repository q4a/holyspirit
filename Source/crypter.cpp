

/*Copyright (C) 2009 Naisse Grégoire

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/



#include <iostream>
#include <list>

int tableau[]={3,8,7,6,9,4,1,5,2,0};
const int base=5;

int crypter(int valeur)
{


    std::list <int>nombre;


    int exposant=0;
    int r=1;
    while((int)(valeur/(r))>0)
    {
        r*=base,exposant++;
    }

    while(exposant>=0)
    {
        int diviseur=1;

        for(int i=0;i<exposant;i++)
            diviseur*=base;

        int b=(int)(valeur/diviseur);
        valeur=valeur-b*diviseur;
        nombre.push_front(b);
        exposant--;
    }

    int final=0,i=0;

    std::list<int>::iterator iter;
    int p=1;

    for(iter=nombre.begin();iter!=nombre.end();iter++)
    {
        final+=p*tableau[*iter*2+rand()%2];
        i++;
        p*=10;
    }

    return final;
}

int decrypter(int valeur)
{
    int final=0;

    int exposant=-1;
    int r=1;
    while((int)(valeur/(r))>0)
    {
        r*=10,exposant++;
    }

    for(int i=exposant;i>=0;i--)
    {
        int diviseur=1;
        for(int j=0;j<i;j++)
            diviseur*=10;

        int multipliant=1;
        for(int j=0;j<i;j++)
            multipliant*=base;

        for(int j=0;j<10;j++)
        {
            if(tableau[j]==(int)((int)(valeur/diviseur)))
                final+=(int)(j/2)*multipliant;
        }

        valeur=valeur%diviseur;
    }

    return final;
}

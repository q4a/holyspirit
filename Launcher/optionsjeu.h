#ifndef OPTIONSJEU_H
#define OPTIONSJEU_H

#include <QDialog>
#include <QLabel>
#include <QSlider>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>

#include "configuration.h"

class OptionsJeu : public QDialog {
    Q_OBJECT

    public:
        OptionsJeu();
        ~OptionsJeu();

    public slots:
        void ChangerContraste(int );
        void ChangerLuminosite(int );
        void ChangerVolume(int );
        void ChangerLumiere(int );
        void ChangerAlpha(int );

        void Quitter();

    private:
        QPushButton     *m_boutonRetour;

        QLabel          *ms_contrastes,
                        *ms_luminosite,
                        *ms_volume,
                        *ms_resolution,
                        *ms_qualiteLumieres,
                        *ms_alpha;

        QSlider         *barreTauxContrastes,
                        *barreTauxLuminosite,
                        *barreVolume,
                        *barreLumiere,
                        *barreAlpha;

        QCheckBox       *modeFenetre,
                        *syncro,
                        *ombres,
                        *fx,
                        *distortion,
                        *item_background,
                        *herbes,
                        *particule,
                        *lissage,
                        *reflect;

        QComboBox       *listeResolution;

        Configuration config;
};

#endif // OPTIONSJEU_H

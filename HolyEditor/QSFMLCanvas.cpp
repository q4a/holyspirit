
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "QSFMLCanvas.h"
#include <QEvent>

// Platform-specific headers
#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif

using namespace sf;

////////////////////////////////////////////////////////////
/// Construct the QSFMLCanvas
////////////////////////////////////////////////////////////
QSFMLCanvas::QSFMLCanvas(const QSize& size, unsigned int frameTime, QWidget* parent) :
QWidget(parent)
{
    // Resize the widget
    resize(size);

    // Setup some states to allow direct rendering into the widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    // Set strong focus to enable keyboard events to be received
    setFocusPolicy(Qt::StrongFocus);

    // Setup the timer
    myTimer.setInterval(frameTime);
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
QSFMLCanvas::~QSFMLCanvas()
{
    // Nothing to do...
}


////////////////////////////////////////////////////////////
/// Notification for the derived class that moment is good
/// for doing initializations
////////////////////////////////////////////////////////////
void QSFMLCanvas::OnInit()
{
    // Nothing to do by default...
}


////////////////////////////////////////////////////////////
/// Notification for the derived class that moment is good
/// for doing its update and drawing stuff
////////////////////////////////////////////////////////////
void QSFMLCanvas::OnUpdate()
{
    // Nothing to do by default...
}


////////////////////////////////////////////////////////////
/// Return the paint engine used by the widget to draw itself
////////////////////////////////////////////////////////////
QPaintEngine* QSFMLCanvas::paintEngine() const
{
    return 0;
}


////////////////////////////////////////////////////////////
/// Called each time an event is received by the widget ;
/// we use it to catch the Polish event and initialize
/// our SFML window
////////////////////////////////////////////////////////////
bool QSFMLCanvas::event(QEvent* event)
{
    if (event->type() == QEvent::Polish)
    {
        // Under X11, we need to flush the commands sent to the server to ensure that
        // SFML will get an updated view of the windows
        #ifdef Q_WS_X11
            XFlush(QX11Info::display());
        #endif

        // Create the SFML window with the widget handle
        Create(winId());

        // Let the derived class do its specific stuff
        OnInit();

        // Setup the timer to trigger a refresh at specified framerate
        //connect(&myTimer, SIGNAL(timeout()), this, SLOT(repaint()));
        //myTimer.start();
    }

    return QWidget::event(event);
}


////////////////////////////////////////////////////////////
/// Called when the widget needs to be painted ;
/// we use it to display a new frame
////////////////////////////////////////////////////////////
void QSFMLCanvas::paintEvent(QPaintEvent*)
{
    // Let the derived class do its specific stuff
   // OnUpdate();

    // Display on screen
   // Display();
}

sf::Vector2f QSFMLCanvas::ConvertCoords(unsigned int x, unsigned int y, const View& view) const
{
    // First, convert from viewport coordinates to homogeneous coordinates
    Vector2f coords;
    IntRect viewport = GetViewport(view);
    coords.x = -1.f + 2.f * (static_cast<int>(x) - viewport.Left) / viewport.Width;
    coords.y =  1.f - 2.f * (static_cast<int>(y) - viewport.Top)  / viewport.Height;

    // Then transform by the inverse of the view matrix
    return view.GetInverseMatrix().Transform(coords);
}



#include "ControlMapper.h"

#include "DAVAEngine.h"
#include "UI/UIControlSystem.h"
#include "Platform/Qt5/QtLayer.h"

#include <QWindow>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QDragMoveEvent>
#include <QDebug>


ControlMapper::ControlMapper( QWindow *w )
    : window(w)
{
}

void ControlMapper::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key_Alt:
            DAVA::InputSystem::Instance()->GetKeyboard().OnKeyPressed( DAVA::DVKEY_ALT );
            return;
        case Qt::Key_Control:
            DAVA::InputSystem::Instance()->GetKeyboard().OnKeyPressed( DAVA::DVKEY_CTRL );
            return;
        case Qt::Key_Shift:
            DAVA::InputSystem::Instance()->GetKeyboard().OnKeyPressed( DAVA::DVKEY_SHIFT );
            return;
        case Qt::Key_CapsLock:
            DAVA::InputSystem::Instance()->GetKeyboard().OnKeyPressed( DAVA::DVKEY_CAPSLOCK );
            return;
        case Qt::Key_Meta:
            // Ignore Win key on windows, Ctrl key on OSX
            return;
        default:
            break;
    }
    
#ifdef Q_OS_MAC
    // OS X doesn't send keyReleaseEvent for hotkeys with Cmd modifier
    // Temporary disable such keys
    if ( e->modifiers() & Qt::Modifier::CTRL )
    {
        return;
    }
#endif
    
    const auto davaKey = DAVA::InputSystem::Instance()->GetKeyboard().GetDavaKeyForSystemKey( e->nativeVirtualKey() );
    if (davaKey != DAVA::DVKEY_UNKNOWN)
    {
        DAVA::QtLayer::Instance()->KeyPressed( davaKey, e->count(), e->timestamp() );
    }
}

void ControlMapper::keyReleaseEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key_Alt:
            DAVA::InputSystem::Instance()->GetKeyboard().OnKeyUnpressed( DAVA::DVKEY_ALT );
            return;
        case Qt::Key_Control:
            DAVA::InputSystem::Instance()->GetKeyboard().OnKeyUnpressed( DAVA::DVKEY_CTRL );
            return;
        case Qt::Key_Shift:
            DAVA::InputSystem::Instance()->GetKeyboard().OnKeyUnpressed( DAVA::DVKEY_SHIFT );
            return;
        case Qt::Key_CapsLock:
            DAVA::InputSystem::Instance()->GetKeyboard().OnKeyUnpressed( DAVA::DVKEY_CAPSLOCK );
            return;
        case Qt::Key_Meta:
            // Ignore Win key on windows, Ctrl key on OSX
            return;
        default:
            break;
    }
    
    const auto davaKey = DAVA::InputSystem::Instance()->GetKeyboard().GetDavaKeyForSystemKey( e->nativeVirtualKey() );
    if (davaKey != DAVA::DVKEY_UNKNOWN)
    {
        DAVA::InputSystem::Instance()->GetKeyboard().OnKeyUnpressed(davaKey);
    }
}

void ControlMapper::mouseMoveEvent(QMouseEvent * event)
{
    const Qt::MouseButtons buttons = event->buttons();
    DAVA::UIEvent davaEvent = MapMouseEventToDAVA(event);
    bool dragWasApplied = false;
    
    davaEvent.phase = DAVA::UIEvent::PHASE_DRAG;
    
    if(buttons & Qt::LeftButton)
    {
        dragWasApplied = true;
        davaEvent.tid = MapQtButtonToDAVA(Qt::LeftButton);
        DAVA::QtLayer::Instance()->MouseEvent(davaEvent);
    }
    if(buttons & Qt::RightButton)
    {
        dragWasApplied = true;
        davaEvent.tid = MapQtButtonToDAVA(Qt::RightButton);
        DAVA::QtLayer::Instance()->MouseEvent(davaEvent);
    }
    if(buttons & Qt::MiddleButton)
    {
        dragWasApplied = true;
        davaEvent.tid = MapQtButtonToDAVA(Qt::MiddleButton);
        DAVA::QtLayer::Instance()->MouseEvent(davaEvent);
    }
    
    if(!dragWasApplied)
    {
        davaEvent.phase = DAVA::UIEvent::PHASE_MOVE;
        davaEvent.tid = MapQtButtonToDAVA(Qt::LeftButton);
        DAVA::QtLayer::Instance()->MouseEvent(davaEvent);
    }
}

void ControlMapper::mousePressEvent(QMouseEvent * event)
{
    DAVA::UIEvent davaEvent = MapMouseEventToDAVA(event);
    davaEvent.phase = DAVA::UIEvent::PHASE_BEGAN;
    
    DAVA::QtLayer::Instance()->MouseEvent(davaEvent);
}

void ControlMapper::mouseReleaseEvent(QMouseEvent * event)
{
    DAVA::UIEvent davaEvent = MapMouseEventToDAVA(event);
    davaEvent.phase = DAVA::UIEvent::PHASE_ENDED;
    
    DAVA::QtLayer::Instance()->MouseEvent(davaEvent);
}

void ControlMapper::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    // Doubleclick handling disabled by request of QA
    
    //DAVA::UIEvent davaEvent = MapMouseEventToDAVA(event);
    //davaEvent.phase = DAVA::UIEvent::PHASE_ENDED;
    //davaEvent.tapCount = 2;
    //
    //DAVA::QtLayer::Instance()->MouseEvent(davaEvent);
}

void ControlMapper::wheelEvent(QWheelEvent *event)
{
    DAVA::UIEvent davaEvent;
    
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
    
    davaEvent.point = davaEvent.physPoint = DAVA::Vector2(numSteps, numSteps);
    davaEvent.tid = DAVA::UIEvent::PHASE_WHEEL;
    davaEvent.phase = DAVA::UIEvent::PHASE_WHEEL;
    
    davaEvent.timestamp = event->timestamp();
    davaEvent.tapCount = 1;
    
    DAVA::QtLayer::Instance()->MouseEvent(davaEvent);
}

void ControlMapper::dragMoveEvent(QDragMoveEvent * event)
{
    DAVA::UIEvent davaEvent;
    auto pos = event->pos();
    const auto currentDPR = static_cast<int>( window->devicePixelRatio() );
    
    davaEvent.point = davaEvent.physPoint = DAVA::Vector2(pos.x() * currentDPR, pos.y() * currentDPR);
    davaEvent.tid = MapQtButtonToDAVA(Qt::LeftButton);
    davaEvent.timestamp = 0;
    davaEvent.tapCount = 1;
    davaEvent.phase = DAVA::UIEvent::PHASE_MOVE;

    DAVA::QtLayer::Instance()->MouseEvent( davaEvent );
}

void ControlMapper::ClearAllKeys()
{
    DAVA::InputSystem::Instance()->GetKeyboard().ClearAllKeys();
}

DAVA::UIEvent ControlMapper::MapMouseEventToDAVA(QMouseEvent *event) const
{
    DAVA::UIEvent davaEvent;
    QPoint pos = event->pos();
    
    int currentDPR = window->devicePixelRatio();
    davaEvent.point = davaEvent.physPoint = DAVA::Vector2(pos.x() * currentDPR, pos.y() * currentDPR);
    davaEvent.tid = MapQtButtonToDAVA(event->button());
    davaEvent.timestamp = event->timestamp();
    davaEvent.tapCount = 1;
    
    return davaEvent;
}

DAVA::UIEvent::eButtonID ControlMapper::MapQtButtonToDAVA(const Qt::MouseButton button) const
{
    switch (button)
    {
        case Qt::LeftButton:
            return DAVA::UIEvent::BUTTON_1;
            
        case Qt::RightButton:
            return DAVA::UIEvent::BUTTON_2;
            
        case Qt::MiddleButton:
            return DAVA::UIEvent::BUTTON_3;
            
        default:
            break;
    }
    
    return DAVA::UIEvent::BUTTON_NONE;
}
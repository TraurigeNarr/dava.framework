/*==================================================================================
    Copyright (c) 2008, binaryzebra
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    * Neither the name of the binaryzebra nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE binaryzebra AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL binaryzebra BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
=====================================================================================*/



#include "DAVAEngine.h"

#include "Platform/Qt5/QtLayer.h"

#if defined (__DAVAENGINE_MACOS__)
    #include "Platform/Qt5/MacOS/CoreMacOSPlatformQt.h"
#elif defined (__DAVAENGINE_WIN32__)
#endif

#include "davaglwidget.h"
#include "FocusTracker.h"
#include "ControlMapper.h"

#include <QMessageBox>
#include <QTimer>
#include <QElapsedTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QScreen>
#include <QDragMoveEvent>
#include <QApplication>
#include <QLineEdit>
#include <QSpinBox>

#include <QOpenGLContext>
#include <QOpenGLPaintDevice>

#include <QBoxLayout>

#if defined( Q_OS_WIN )
#include <QtPlatformheaders/QWGLNativeContext>
#elif defined( Q_OS_MAC )
#endif

#include "QtTools/FrameworkBinding/FrameworkLoop.h"


namespace
{
    const QSize cMinSize = QSize( 180, 180 );
}



OpenGLWindow::OpenGLWindow()
    : QWindow()
    , paintDevice(nullptr)
    , controlMapper(new ControlMapper(this))
{
    setSurfaceType(QWindow::OpenGLSurface);
    
    setKeyboardGrabEnabled(true);
    setMouseGrabEnabled(true);

    setMinimumSize( cMinSize );
}

OpenGLWindow::~OpenGLWindow()
{
}

void OpenGLWindow::render()
{
    if (!paintDevice)
    {
        paintDevice = new QOpenGLPaintDevice();
    }

    if (paintDevice->size() != size())
    {
        paintDevice->setSize(size());
    }
}

void OpenGLWindow::renderNow()
{
    if (!isExposed())
        return;

    render();
    auto context = FrameworkLoop::Instance()->Context();
    context->swapBuffers( this );
}

void OpenGLWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);
    
    if (isExposed())
    {
        renderNow();
        emit Exposed();
    }
}

bool OpenGLWindow::event(QEvent *event)
{
    switch ( event->type() )
    {
    case QEvent::UpdateRequest:
        renderNow();
        return true;
    case QEvent::FocusOut:
        controlMapper->ClearAllKeys();
        break;
    default:
        break;
    }
    
    return QWindow::event(event);
}

void OpenGLWindow::keyPressEvent(QKeyEvent *e)
{
    controlMapper->keyPressEvent(e);
}

void OpenGLWindow::keyReleaseEvent(QKeyEvent *e)
{
    controlMapper->keyReleaseEvent(e);
}

void OpenGLWindow::mouseMoveEvent(QMouseEvent * e)
{
    controlMapper->mouseMoveEvent(e);
}

void OpenGLWindow::mousePressEvent(QMouseEvent * e)
{
    controlMapper->mousePressEvent(e);
    emit mousePressed();
}

void OpenGLWindow::mouseReleaseEvent(QMouseEvent * e)
{
    controlMapper->mouseReleaseEvent(e);
}

void OpenGLWindow::mouseDoubleClickEvent(QMouseEvent *e)
{
    controlMapper->mouseDoubleClickEvent(e);
}

void OpenGLWindow::wheelEvent(QWheelEvent *e)
{
    controlMapper->wheelEvent(e);
}

void OpenGLWindow::handleDragMoveEvent(QDragMoveEvent* e)
{
    controlMapper->dragMoveEvent(e);
}


///=======================
DavaGLWidget::DavaGLWidget(QWidget *parent)
    : QWidget(parent)
    , isInitialized(false)
    , currentDPR(1)
    , currentWidth(0)
    , currentHeight(0)
{
    setAcceptDrops(true);
    setMouseTracking(true);

    setFocusPolicy(Qt::NoFocus);
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    setMinimumSize(cMinSize);
    
    openGlWindow = new OpenGLWindow();
    connect( openGlWindow, &OpenGLWindow::Exposed, this, &DavaGLWidget::OnWindowExposed );
    connect( openGlWindow, &QWindow::screenChanged, this, &DavaGLWidget::PerformSizeChange );
    
    auto l = new QBoxLayout(QBoxLayout::TopToBottom, this);
    l->setMargin( 0 );
    setLayout( l );
    
    container = createWindowContainer( openGlWindow );
    container->setAcceptDrops(true);
    container->setMouseTracking(true);
    container->setFocusPolicy(Qt::WheelFocus);
    //container->setFocusPolicy(Qt::NoFocus);

    openGlWindow->installEventFilter(this);

    layout()->addWidget(container);

    focusTracker = new FocusTracker(this);
}

DavaGLWidget::~DavaGLWidget()
{
}

OpenGLWindow* DavaGLWidget::GetGLWindow() const
{
    return openGlWindow;
}

void DavaGLWidget::OnWindowExposed()
{
    disconnect( openGlWindow, &OpenGLWindow::Exposed, this, &DavaGLWidget::OnWindowExposed );

    currentDPR = devicePixelRatio();
    
    const auto contextId = FrameworkLoop::Instance()->GetRenderContextId();
    DAVA::QtLayer::Instance()->InitializeGlWindow( contextId );
    
    isInitialized = true;

    PerformSizeChange();
    emit Initialized();
}

void DavaGLWidget::resizeEvent(QResizeEvent *e)
{
    currentWidth = e->size().width();
    currentHeight = e->size().height();
    
    QWidget::resizeEvent(e);
    PerformSizeChange();
}

bool DavaGLWidget::eventFilter( QObject* watched, QEvent* event )
{
    if ( watched == openGlWindow )
    {
        switch ( event->type() )
        {
        case QEvent::DragEnter:
            {
                auto e = static_cast<QDragEnterEvent *>( event );
                e->setDropAction( Qt::LinkAction );
                e->accept();
            }
            break;
        case QEvent::DragMove:
            {
                auto e = static_cast<QDragMoveEvent *>( event );
                openGlWindow->handleDragMoveEvent( e );
                e->setDropAction( Qt::LinkAction );
                e->accept();
            }
            break;
        case QEvent::DragLeave:
            break;
        case QEvent::Drop:
            {
                auto e = static_cast<QDropEvent *>( event );
                emit OnDrop( e->mimeData() );
                e->setDropAction( Qt::LinkAction );
                e->accept();
            }
            break;

        case QEvent::MouseButtonPress:
            focusTracker->OnClick();
            break;
        case QEvent::Enter:
            focusTracker->OnEnter();
            break;
        case QEvent::Leave:
            focusTracker->OnLeave();
            break;
        case QEvent::FocusIn:
            focusTracker->OnFocusIn();
            break;
        case QEvent::FocusOut:
            focusTracker->OnFocusOut();
            break;
        default:
            break;
        }
    }

    return QWidget::eventFilter( watched, event );
}

void DavaGLWidget::PerformSizeChange()
{
    currentDPR = openGlWindow->devicePixelRatio();
    if (isInitialized)
    {
        DAVA::QtLayer::Instance()->Resize(currentWidth * currentDPR, currentHeight * currentDPR);
    }
    
    emit Resized(currentWidth, currentHeight, currentDPR);
}

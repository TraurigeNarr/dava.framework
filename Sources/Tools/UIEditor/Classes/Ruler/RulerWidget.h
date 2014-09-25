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

#ifndef __RULER_WIDGET__H__
#define __RULER_WIDGET__H__

#include "RulerSettings.h"

#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>

class RulerWidget : public QWidget
{
    Q_OBJECT

public:
    enum eOrientation
    {
        Horizontal,
        Vertical
    };

    explicit RulerWidget(QWidget *parent = 0);
    virtual ~RulerWidget();

    // Whether the ruler horisontal or vertical?
    void SetOrientation(eOrientation rulerOrientation);

    // Set the initial Ruler Settings.
    void SetRulerSettings(const RulerSettings& rulerSettings);

    // Update the rulers by sending appropriate events.
    void UpdateRulers();

    virtual void paintEvent(QPaintEvent *event);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

signals:
    // Guide drag is completed.
    void GuideDropped(Qt::DropAction dropAction);
    
public slots:
    // Ruler Settings are changed.
    void OnRulerSettingsChanged(const RulerSettings& rulerSettings);

    // Marker Position is changed.
    void OnMarkerPositionChanged(int position);

protected:
    // We are using double buffering to avoid flicker and excessive updates.
    void UpdateDoubleBufferImage();
    
    // Draw different types of scales.
    void DrawScale(QPainter& painter, int tickStep, int tickStartPos, int tickEndPos,
                   bool drawValues, bool isHorizontal);
    
private:
    // Ruler orientation.
    eOrientation orientation;
    
    // Ruler settings.
    RulerSettings settings;
    
    // Ruler double buffer.
    QPixmap* doubleBuffer;
    
    // Marker position.
    int markerPosition;
    
    // Drag start position (for Guides).
    QPoint dragStartPos;
};

#endif /* defined(__RULER_WIDGET__H__) */

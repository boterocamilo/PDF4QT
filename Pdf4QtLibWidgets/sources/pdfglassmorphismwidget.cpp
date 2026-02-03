// MIT License
//
// Copyright (c) 2018-2025 Jakub Melka and Contributors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "pdfglassmorphismwidget.h"

#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QApplication>
#include <QPalette>
#include <QStyleOption>
#include <QVBoxLayout>

namespace pdfviewer
{

// PDFGlassmorphismStyle implementation

PDFGlassmorphismStyle::PDFGlassmorphismStyle(QStyle* baseStyle) :
    QProxyStyle(baseStyle)
{
}

bool PDFGlassmorphismStyle::isDarkTheme() const
{
    QPalette palette = QApplication::palette();
    QColor windowColor = palette.color(QPalette::Window);
    // Consider dark if luminance is less than 128
    return windowColor.lightness() < 128;
}

void PDFGlassmorphismStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option,
                                         QPainter* painter, const QWidget* widget) const
{
    // Apply modern styling to buttons and other primitives
    if (element == PE_PanelButtonTool && option)
    {
        painter->setRenderHint(QPainter::Antialiasing);
        
        QRect rect = option->rect;
        QPainterPath path;
        path.addRoundedRect(rect, 6, 6);
        
        if (option->state & State_MouseOver)
        {
            QColor hoverColor = isDarkTheme() ? QColor(80, 80, 85, 100) : QColor(200, 200, 200, 100);
            painter->fillPath(path, hoverColor);
        }
        
        if (option->state & State_Sunken)
        {
            QColor pressColor = isDarkTheme() ? QColor(60, 60, 65, 150) : QColor(180, 180, 180, 150);
            painter->fillPath(path, pressColor);
        }
        
        return;
    }
    
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void PDFGlassmorphismStyle::drawControl(ControlElement element, const QStyleOption* option,
                                       QPainter* painter, const QWidget* widget) const
{
    QProxyStyle::drawControl(element, option, painter, widget);
}

// PDFGlassmorphismToolbar implementation

PDFGlassmorphismToolbar::PDFGlassmorphismToolbar(QWidget* parent) :
    QWidget(parent),
    m_toolbar(nullptr),
    m_position(TopCenter),
    m_autoHideEnabled(false),
    m_autoHideTimeout(3000),
    m_opacity(1.0),
    m_dragging(false),
    m_fadeAnimation(nullptr),
    m_autoHideTimer(nullptr)
{
    // Set window flags for frameless, translucent window
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground);
    
    // Setup layout
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(0);
    
    // Setup fade animation
    m_fadeAnimation = new QPropertyAnimation(this, "opacity", this);
    m_fadeAnimation->setDuration(300);
    m_fadeAnimation->setEasingCurve(QEasingCurve::InOutCubic);
    
    // Setup auto-hide timer
    m_autoHideTimer = new QTimer(this);
    m_autoHideTimer->setSingleShot(true);
    connect(m_autoHideTimer, &QTimer::timeout, this, &PDFGlassmorphismToolbar::hideAnimated);
    
    applyGlassmorphismStyle();
}

void PDFGlassmorphismToolbar::setToolbar(QToolBar* toolbar)
{
    if (m_toolbar == toolbar)
        return;
    
    if (m_toolbar)
    {
        layout()->removeWidget(m_toolbar);
        m_toolbar->setParent(nullptr);
    }
    
    m_toolbar = toolbar;
    
    if (m_toolbar)
    {
        layout()->addWidget(m_toolbar);
        m_toolbar->setStyleSheet("QToolBar { background: transparent; border: none; }");
        adjustSize();
    }
}

void PDFGlassmorphismToolbar::setPosition(Position position)
{
    m_position = position;
    updatePosition();
}

void PDFGlassmorphismToolbar::setAutoHideEnabled(bool enabled)
{
    m_autoHideEnabled = enabled;
    if (enabled)
    {
        resetAutoHideTimer();
    }
    else
    {
        m_autoHideTimer->stop();
    }
}

void PDFGlassmorphismToolbar::setAutoHideTimeout(int milliseconds)
{
    m_autoHideTimeout = milliseconds;
}

void PDFGlassmorphismToolbar::setOpacity(qreal opacity)
{
    m_opacity = qBound(0.0, opacity, 1.0);
    update();
}

void PDFGlassmorphismToolbar::showAnimated()
{
    show();
    m_fadeAnimation->stop();
    m_fadeAnimation->setStartValue(m_opacity);
    m_fadeAnimation->setEndValue(1.0);
    m_fadeAnimation->start();
}

void PDFGlassmorphismToolbar::hideAnimated()
{
    m_fadeAnimation->stop();
    m_fadeAnimation->setStartValue(m_opacity);
    m_fadeAnimation->setEndValue(0.0);
    connect(m_fadeAnimation, &QPropertyAnimation::finished, this, [this]() {
        hide();
        disconnect(m_fadeAnimation, &QPropertyAnimation::finished, this, nullptr);
    });
    m_fadeAnimation->start();
}

void PDFGlassmorphismToolbar::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setOpacity(m_opacity);
    
    // Create rounded rectangle path
    QPainterPath path;
    QRect rect = this->rect();
    path.addRoundedRect(rect, 12, 12);
    
    // Draw glassmorphism background
    QLinearGradient gradient(rect.topLeft(), rect.bottomRight());
    if (isDarkTheme())
    {
        // Dark theme: semi-transparent dark with slight gradient
        gradient.setColorAt(0, QColor(40, 40, 45, 204));
        gradient.setColorAt(1, QColor(30, 30, 35, 204));
    }
    else
    {
        // Light theme: semi-transparent white with gradient
        gradient.setColorAt(0, QColor(255, 255, 255, 178));
        gradient.setColorAt(1, QColor(245, 245, 250, 178));
    }
    
    painter.fillPath(path, gradient);
    
    // Draw border
    QPen borderPen;
    if (isDarkTheme())
    {
        borderPen.setColor(QColor(255, 255, 255, 40));
    }
    else
    {
        borderPen.setColor(QColor(0, 0, 0, 40));
    }
    borderPen.setWidth(1);
    painter.setPen(borderPen);
    painter.drawPath(path);
    
    // Draw subtle drop shadow (inner glow effect)
    painter.setOpacity(m_opacity * 0.3);
    QPainterPath shadowPath;
    QRect shadowRect = rect.adjusted(2, 2, -2, -2);
    shadowPath.addRoundedRect(shadowRect, 10, 10);
    
    if (isDarkTheme())
    {
        painter.fillPath(shadowPath, QColor(0, 0, 0, 30));
    }
    else
    {
        painter.fillPath(shadowPath, QColor(0, 0, 0, 20));
    }
}

void PDFGlassmorphismToolbar::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void PDFGlassmorphismToolbar::mouseMoveEvent(QMouseEvent* event)
{
    if (m_dragging)
    {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void PDFGlassmorphismToolbar::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragging = false;
        event->accept();
    }
}

void PDFGlassmorphismToolbar::enterEvent(QEnterEvent* event)
{
    Q_UNUSED(event);
    
    if (m_autoHideEnabled)
    {
        m_autoHideTimer->stop();
        if (m_opacity < 1.0)
        {
            showAnimated();
        }
    }
}

void PDFGlassmorphismToolbar::leaveEvent(QEvent* event)
{
    Q_UNUSED(event);
    
    if (m_autoHideEnabled)
    {
        resetAutoHideTimer();
    }
}

void PDFGlassmorphismToolbar::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    updatePosition();
}

void PDFGlassmorphismToolbar::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    updatePosition();
    
    if (m_autoHideEnabled)
    {
        resetAutoHideTimer();
    }
}

void PDFGlassmorphismToolbar::updatePosition()
{
    if (!parentWidget())
        return;
    
    QRect parentRect = parentWidget()->geometry();
    QSize size = sizeHint();
    
    int x = 0;
    int y = 0;
    
    switch (m_position)
    {
        case TopCenter:
            x = (parentRect.width() - size.width()) / 2;
            y = 20;
            break;
        case TopLeft:
            x = 20;
            y = 20;
            break;
        case TopRight:
            x = parentRect.width() - size.width() - 20;
            y = 20;
            break;
        case BottomCenter:
            x = (parentRect.width() - size.width()) / 2;
            y = parentRect.height() - size.height() - 20;
            break;
        case BottomLeft:
            x = 20;
            y = parentRect.height() - size.height() - 20;
            break;
        case BottomRight:
            x = parentRect.width() - size.width() - 20;
            y = parentRect.height() - size.height() - 20;
            break;
    }
    
    move(x, y);
}

void PDFGlassmorphismToolbar::resetAutoHideTimer()
{
    if (m_autoHideEnabled)
    {
        m_autoHideTimer->start(m_autoHideTimeout);
    }
}

void PDFGlassmorphismToolbar::applyGlassmorphismStyle()
{
    // Apply modern glassmorphism style to toolbar
    PDFGlassmorphismStyle* style = new PDFGlassmorphismStyle();
    setStyle(style);
}

bool PDFGlassmorphismToolbar::isDarkTheme() const
{
    QPalette palette = QApplication::palette();
    QColor windowColor = palette.color(QPalette::Window);
    return windowColor.lightness() < 128;
}

} // namespace pdfviewer

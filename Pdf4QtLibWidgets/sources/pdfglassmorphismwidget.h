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

#ifndef PDFGLASSMORPHISMWIDGET_H
#define PDFGLASSMORPHISMWIDGET_H

#include "pdfwidgetsglobal.h"

#include <QWidget>
#include <QToolBar>
#include <QProxyStyle>
#include <QPropertyAnimation>
#include <QTimer>

namespace pdfviewer
{

/// Glassmorphism style for modern UI elements
class PDF4QTLIBWIDGETSSHARED_EXPORT PDFGlassmorphismStyle : public QProxyStyle
{
    Q_OBJECT

public:
    explicit PDFGlassmorphismStyle(QStyle* baseStyle = nullptr);
    virtual ~PDFGlassmorphismStyle() override = default;

    virtual void drawPrimitive(PrimitiveElement element, const QStyleOption* option,
                              QPainter* painter, const QWidget* widget = nullptr) const override;
    virtual void drawControl(ControlElement element, const QStyleOption* option,
                            QPainter* painter, const QWidget* widget = nullptr) const override;

private:
    bool isDarkTheme() const;
};

/// Floating toolbar with translucent blur effects
class PDF4QTLIBWIDGETSSHARED_EXPORT PDFGlassmorphismToolbar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)

public:
    enum Position
    {
        TopCenter,
        TopLeft,
        TopRight,
        BottomCenter,
        BottomLeft,
        BottomRight
    };

    explicit PDFGlassmorphismToolbar(QWidget* parent = nullptr);
    virtual ~PDFGlassmorphismToolbar() override = default;

    /// Set the toolbar widget
    void setToolbar(QToolBar* toolbar);
    QToolBar* getToolbar() const { return m_toolbar; }

    /// Position management
    void setPosition(Position position);
    Position getPosition() const { return m_position; }

    /// Auto-hide functionality
    void setAutoHideEnabled(bool enabled);
    bool isAutoHideEnabled() const { return m_autoHideEnabled; }

    void setAutoHideTimeout(int milliseconds);
    int getAutoHideTimeout() const { return m_autoHideTimeout; }

    /// Opacity for animations
    qreal opacity() const { return m_opacity; }
    void setOpacity(qreal opacity);

    /// Show/hide with animation
    void showAnimated();
    void hideAnimated();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void showEvent(QShowEvent* event) override;

private:
    void updatePosition();
    void resetAutoHideTimer();
    void applyGlassmorphismStyle();
    bool isDarkTheme() const;

    QToolBar* m_toolbar;
    Position m_position;
    bool m_autoHideEnabled;
    int m_autoHideTimeout;
    qreal m_opacity;
    
    // Dragging support
    bool m_dragging;
    QPoint m_dragPosition;
    
    // Animation
    QPropertyAnimation* m_fadeAnimation;
    QTimer* m_autoHideTimer;
};

} // namespace pdfviewer

#endif // PDFGLASSMORPHISMWIDGET_H

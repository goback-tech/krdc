/***************************************************************************
                  kvncview.h  -  widget that shows the vnc client
                             -------------------
    begin                : Thu Dec 20 15:11:42 CET 2001
    copyright            : (C) 2001-2003 by Tim Jansen
    email                : tim@tjansen.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KVNCVIEW_H
#define KVNCVIEW_H

#include "kremoteview.h"
#include <QCursor>
#include <QMap>
//Added by qt3to4:
#include <QWheelEvent>
#include <QFocusEvent>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QCustomEvent>

#include "pointerlatencyometer.h"
#include "vnctypes.h"
#include "threads.h"


class KVncView : public KRemoteView
{
	Q_OBJECT
private:
	ControllerThread m_cthread;
	WriterThread m_wthread;
	volatile bool m_quitFlag; // if set: all threads should die ASAP
	QMutex m_framebufferLock;
	bool m_enableFramebufferLocking;
	bool m_enableClientCursor;

	QSize m_framebufferSize;
	bool m_scaling;
	bool m_remoteMouseTracking;
	bool m_viewOnly;

	int m_buttonMask;
	QMap<unsigned int,bool> m_mods;

	QString m_host;
	int m_port;

	QClipboard *m_cb;
	bool m_dontSendCb;
	QCursor m_cursor;
	DotCursorState m_cursorState;
	QString m_caption;
	PointerLatencyOMeter m_plom;

	void mouseEvent(QMouseEvent*);
	unsigned long toKeySym(QKeyEvent *k);
	bool checkLocalKRfb();
	void paintMessage(const QString &msg);
	void showDotCursorInternal();
	void unpressModifiers();

protected:
	void paintEvent(QPaintEvent*);
	void customEvent(QEvent*);
	void mousePressEvent(QMouseEvent*);
	void mouseDoubleClickEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void wheelEvent(QWheelEvent *);
	void focusOutEvent(QFocusEvent *);
	bool x11Event(XEvent*);

public:
	KVncView(QWidget* parent=0,
		 const QString &host = QString(""), int port = 5900,
		 const QString &password = QString(),
		 KRemoteView::Quality quality = Unknown,
		 DotCursorState dotCursorState = CursorAuto,
		 const QString &encodings = QString(),
		 const QString &caption = QString());
	~KVncView();
	QSize sizeHint() const;
	void drawRegion(int x, int y, int w, int h);
	void lockFramebuffer();
	void unlockFramebuffer();
	void enableClientCursor(bool enable);
	virtual bool scaling() const;
	virtual bool supportsScaling() const;
	virtual bool supportsLocalCursor() const;
	virtual QSize framebufferSize();
	void setRemoteMouseTracking(bool s);
	bool remoteMouseTracking();
	void configureApp(Quality q, const QString specialEncodings = QString());
	void showDotCursor(DotCursorState state);
	DotCursorState dotCursorState() const;
	virtual void startQuitting();
	virtual bool isQuitting();
	virtual QString host();
	virtual int port();
	virtual bool start();

	virtual bool viewOnly();


public slots:
	virtual void enableScaling(bool s);
	virtual void setViewOnly(bool s);
	virtual void pressKey(XEvent *k);


private slots:
	void clipboardChanged();
	void selectionChanged();
};

#endif

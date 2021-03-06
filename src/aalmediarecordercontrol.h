/*
 * Copyright (C) 2013-2014 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef AALMEDIARECORDERCONTROL_H
#define AALMEDIARECORDERCONTROL_H

#include <QLatin1String>
#include <QMediaRecorderControl>
#include <QSize>
#include <QUrl>
#include <QThread>

#include <stdint.h>

class AalCameraService;
struct CameraControl;
struct CameraControlListener;
struct MediaRecorderWrapper;
class AudioCapture;
class QThread;
class QTimer;

class AalMediaRecorderControl : public QMediaRecorderControl
{
Q_OBJECT
public:
    AalMediaRecorderControl(AalCameraService *service, QObject *parent = 0);
    ~AalMediaRecorderControl();

    virtual void applySettings();
    virtual qint64 duration() const;
    virtual bool isMuted() const;
    virtual QUrl outputLocation() const;
    virtual bool setOutputLocation(const QUrl & location);
    virtual QMediaRecorder::State state() const;
    virtual QMediaRecorder::Status status() const;
    virtual qreal volume() const;

    static void errorCB(void* context);

    void init(CameraControl *control, CameraControlListener *listener);
    MediaRecorderWrapper* mediaRecorder() const;
    AudioCapture *audioCapture() const;

public Q_SLOTS:
    virtual void setMuted(bool muted);
    virtual void setState(QMediaRecorder::State state);
    virtual void setVolume(qreal gain);
    void startAudioCaptureThread();

signals:
    void audioCaptureThreadStarted();

private Q_SLOTS:
    virtual void updateDuration();
    void handleError();
    void deleteAudioCapture();

private:
    bool initRecorder();
    void deleteRecorder();
    int initAudioCapture();
    void setStatus(QMediaRecorder::Status status);
    int startRecording();
    void stopRecording();
    void setParameter(const QString &parameter, int value);
    static void recorderReadAudioCallback(void *context);

    AalCameraService *m_service;
    MediaRecorderWrapper *m_mediaRecorder;
    AudioCapture *m_audioCapture;
    int m_outfd;
    QUrl m_outputLocation;
    qint64 m_duration;
    QMediaRecorder::State m_currentState;
    QMediaRecorder::Status m_currentStatus;
    QTimer *m_recordingTimer;
    QThread m_audioCaptureThread;
    bool m_audioCaptureAvailable;

    static const int RECORDER_GENERAL_ERROR = -1;
    static const int RECORDER_NOT_AVAILABLE_ERROR = -2;
    static const int RECORDER_INITIALIZATION_ERROR = -3;

    static const int DURATION_UPDATE_INTERVAL = 1000; // update every second

    static const QLatin1String PARAM_AUDIO_BITRATE;
    static const QLatin1String PARAM_AUDIO_CHANNELS;
    static const QLatin1String PARAM_AUTIO_SAMPLING;
    static const QLatin1String PARAM_LATITUDE;
    static const QLatin1String PARAM_LONGITUDE;
    static const QLatin1String PARAM_ORIENTATION;
    static const QLatin1String PARAM_VIDEO_BITRATE;
};

#endif

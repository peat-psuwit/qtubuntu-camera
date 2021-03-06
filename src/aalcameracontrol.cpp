/*
 * Copyright (C) 2013 Canonical, Ltd.
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

#include "aalcameracontrol.h"
#include "aalcameraservice.h"

#include <hybris/camera/camera_compatibility_layer.h>

AalCameraControl::AalCameraControl(AalCameraService *service, QObject *parent)
   : QCameraControl(parent),
    m_service(service),
    m_state(QCamera::UnloadedState),
    m_status(QCamera::UnloadedStatus),
    m_captureMode(QCamera::CaptureStillImage)
{
}

AalCameraControl::~AalCameraControl()
{
}

QCamera::State AalCameraControl::state() const
{
    return m_state;
}

void AalCameraControl::setState(QCamera::State state)
{
    if (m_state == state)
        return;

    if (state == QCamera::ActiveState) {
        bool ok = m_service->connectCamera();
        if (!ok) {
            Q_EMIT error(QCamera::ServiceMissingError, QLatin1String("Unable to connect to camera"));
            return;
        }
        if (m_captureMode == QCamera::CaptureStillImage) {
            m_service->enablePhotoMode();
        } else {
            m_service->enableVideoMode();
        }
        Q_EMIT captureModeChanged(m_captureMode);
        m_service->startPreview();
    } else if (state == QCamera::LoadedState) {
        if (m_state == QCamera::UnloadedState) {
            bool ok = m_service->connectCamera();
            if (!ok) {
                Q_EMIT error(QCamera::ServiceMissingError, QLatin1String("Unable to connect to camera"));
                return;
            }
        } else {
            m_service->stopPreview();
        }
    } else if (state == QCamera::UnloadedState) {
        m_service->disconnectCamera();
    }

    m_state = state;
    Q_EMIT stateChanged(m_state);
    m_service->updateCaptureReady();
}

QCamera::Status AalCameraControl::status() const
{
    return m_status;
}

QCamera::CaptureModes AalCameraControl::captureMode() const
{
    return m_captureMode;
}

void AalCameraControl::setCaptureMode(QCamera::CaptureModes mode)
{
    if (m_captureMode == mode)
        return;

    if (m_service->isRecording())
        return;

    m_captureMode = mode;
    if (m_service->androidControl()) {
        if (mode == QCamera::CaptureStillImage) {
            m_service->enablePhotoMode();
        } else {
            m_service->enableVideoMode();
        }
        Q_EMIT captureModeChanged(mode);
    }
}

bool AalCameraControl::isCaptureModeSupported(QCamera::CaptureModes mode) const
{
    return (QCamera::CaptureStillImage==mode) | (QCamera::CaptureVideo==mode);
}

bool AalCameraControl::canChangeProperty(QCameraControl::PropertyChangeType changeType, QCamera::Status status) const
{
    Q_UNUSED(changeType);
    Q_UNUSED(status);

    return true;
}

void AalCameraControl::init(CameraControl *control, CameraControlListener *listener)
{
    Q_UNUSED(control);
    listener->on_msg_error_cb = &AalCameraControl::errorCB;
}

void AalCameraControl::handleError()
{
    Q_EMIT error(QCamera::CameraError, QLatin1String("Unknown error in camera"));
}

void AalCameraControl::errorCB(void *context)
{
    Q_UNUSED(context);
    QMetaObject::invokeMethod(AalCameraService::instance()->cameraControl(),
                              "handleError", Qt::QueuedConnection);
}

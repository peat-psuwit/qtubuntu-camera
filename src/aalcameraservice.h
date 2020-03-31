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

#ifndef AALCAMERASERVICE_H
#define AALCAMERASERVICE_H

#include <QMediaService>
#include <QSize>
#include <QtMultimedia/QCamera>

class AalCameraControl;
class AalCameraFlashControl;
class AalCameraFocusControl;
class AalCameraZoomControl;
class AalImageCaptureControl;
class AalImageEncoderControl;
class AalMediaRecorderControl;
class AalMetaDataWriterControl;
class AalVideoDeviceSelectorControl;
class AalVideoEncoderSettingsControl;
class AalVideoRendererControl;
class AalViewfinderSettingsControl;
class AalCameraExposureControl;
class AalCameraInfoControl;
class QCameraControl;

struct CameraControl;
struct CameraControlListener;

class StorageManager;
class RotationHandler;

class AalCameraService : public QMediaService
{
Q_OBJECT
public:
    AalCameraService(QObject *parent = 0);
    ~AalCameraService();

    QMediaControl* requestControl(const char *name);
    void releaseControl(QMediaControl *control);

    AalCameraControl *cameraControl() const { return m_cameraControl; }
    AalCameraFlashControl *flashControl() const { return m_flashControl; }
    AalCameraFocusControl *focusControl() const { return m_focusControl; }
    AalCameraZoomControl *zoomControl() const { return m_zoomControl; }
    AalImageCaptureControl *imageCaptureControl() const { return m_imageCaptureControl; }
    AalImageEncoderControl *imageEncoderControl() const { return m_imageEncoderControl; }
    AalMediaRecorderControl *mediaRecorderControl() const { return m_mediaRecorderControl; }
    AalMetaDataWriterControl *metadataWriterControl() const { return m_metadataWriter; }
    AalVideoDeviceSelectorControl *deviceSelector() const { return m_deviceSelectControl; }
    AalVideoEncoderSettingsControl *videoEncoderControl() const { return m_videoEncoderControl; }
    AalVideoRendererControl *videoOutputControl() const { return m_videoOutput; }
    AalViewfinderSettingsControl *viewfinderControl() const { return m_viewfinderControl; }
    AalCameraExposureControl *exposureControl() const { return m_exposureControl; }
    AalCameraInfoControl *infoControl() const { return m_infoControl; }

    CameraControl *androidControl();

    StorageManager *storageManager();
    RotationHandler *rotationHandler();

    bool connectCamera();
    void disconnectCamera();
    void startPreview();
    void stopPreview();
    bool isPreviewStarted() const;

    bool isCameraActive() const;
    bool isBackCameraUsed() const;

    void enablePhotoMode();
    void enableVideoMode();

    bool isRecording() const;
    QSize selectSizeWithAspectRatio(const QList<QSize> &sizes, float targetAspectRatio) const;

    static AalCameraService *instance() { return m_service; }

public Q_SLOTS:
    void updateCaptureReady();

private:
    void initControls(CameraControl *camControl, CameraControlListener *listener);

    static AalCameraService *m_service;

    AalCameraControl *m_cameraControl;
    AalCameraFlashControl *m_flashControl;
    AalCameraFocusControl *m_focusControl;
    AalCameraZoomControl *m_zoomControl;
    AalImageCaptureControl *m_imageCaptureControl;
    AalImageEncoderControl *m_imageEncoderControl;
    AalMediaRecorderControl *m_mediaRecorderControl;
    AalMetaDataWriterControl *m_metadataWriter;
    AalVideoDeviceSelectorControl *m_deviceSelectControl;
    AalVideoEncoderSettingsControl *m_videoEncoderControl;
    AalVideoRendererControl *m_videoOutput;
    AalViewfinderSettingsControl *m_viewfinderControl;
    AalCameraExposureControl *m_exposureControl;
    AalCameraInfoControl *m_infoControl;

    CameraControl *m_androidControl;
    CameraControlListener *m_androidListener;

    StorageManager *m_storageManager;
    RotationHandler *m_rotationHandler;
};

#endif

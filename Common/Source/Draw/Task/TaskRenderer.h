/*
 * LK8000 Tactical Flight Computer -  WWW.LK8000.IT
 * Released under GNU/GPL License v.2
 * See CREDITS.TXT file for authors and copyrights
 *
 * File:   TaskRenderer.h
 * Author: Bruno de Lacheisserie
 *
 * Created on October 21, 2016, 12:41 AM
 */

#ifndef TASKRENDERER_H
#define TASKRENDERER_H

#include "Geographic/GeoPoint.h"

class ScreenProjection;
class LKSurface;

class TaskRenderer {
public:

    TaskRenderer() : _Visible() {
    }

    virtual ~TaskRenderer() {
    }

    TaskRenderer(const TaskRenderer&) = delete;
    TaskRenderer& operator=(const TaskRenderer&) = delete;

    void Draw(LKSurface& Surface, const RECT &rc, bool bFill) const;
    void CalculateScreenPosition(const rectObj &screenbounds, const ScreenProjection& _Proj);

    PixelRect GetScreenBounds() const;

protected:

    typedef std::vector<GeoPoint> GeoPoints_t;
    typedef std::vector<RasterPoint> ScreenPoints_t;

    GeoPoints_t _GeoPoints;
    ScreenPoints_t _ScreenPoints;

    bool _Visible;
    rectObj _bounds;
};

#endif /* TASKRENDERER_H */

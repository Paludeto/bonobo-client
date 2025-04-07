#include "basic.h"
#include "worldmap/worldmap.h"

#include <math.h>


float Basic::getAngle(QVector2D currentPos, QVector2D targetPos) {

    return atan2(targetPos.y() - currentPos.y(), targetPos.x() - currentPos.x());

}

float Basic::getDistance(const QVector2D currentPos, const QVector2D &targetPos) {

    return sqrt(pow(targetPos.x() - currentPos.x(), 2) + pow(targetPos.y() - currentPos.y(), 2));

}

float Basic::smallestAngleDiff(float &currentPos, float &targetPos) {
    float a = fmod(currentPos + 2*M_PI, 2*M_PI) - fmod(targetPos + 2*M_PI, 2*M_PI);
    
    if(a > M_PI) {
        a = a - 2 * M_PI;
    } 
    else if(a < -M_PI) {
        a = a + 2 * M_PI;
    }
    
    return a;
}


bool Basic::checkCanBeReversed(float &currentAng, float targetAng) {
    float angDiff = Basic::smallestAngleDiff(currentAng, targetAng);

    return (fabs(angDiff) > M_PI/2.0 + M_PI/20);  
}

float Basic::normalizeAngle(const float &angle) {
    float angleRet = fmod(angle + M_PI, 2.0 * M_PI);
    angleRet = (angleRet < 0.0) ? angleRet + M_PI : angleRet - M_PI;

    return angleRet;
}

QVector2D Basic::getBallImpactPosition(float incomingAngle, float posX, WorldMap *wm, VSSRef::Color color) {
    float angle  = incomingAngle; 

    if(wm->isOurSideLeft(color)) {
        angle = M_PI - angle;
    }

    if(angle == M_2_PI) {
        angle = 0.0f;
    }

    if(angle >= M_PI) {
        angle += (angle > 0 ? -M_2_PI : M_2_PI);
    }

    if(fabs(angle) >= M_PI/2) {
        return QVector2D(0.0, 0.0);
    }

    float x = fabs(posX - wm->getBallPosition().x());
    float tan = std::tan(angle);
    float y = tan * x;
    
    float y_impact = wm->getBallPosition().y() + y;

    return QVector2D(posX, y_impact);
}

QVector2D Basic::calculateItermediatePoint(QVector2D near, QVector2D far, float distance, float beta) {
    float alpha = std::atan2(far.y() - near.y(), far.x() - near.x());
    float gamma = alpha + beta;

    return QVector2D(near.x() + distance * std::cos(gamma), near.y() + distance * std::sin(gamma));
}

bool Basic::pointInCircumcicle(const QVector2D p, const Triangle tri) {
    float ax = tri.a.x() - p.x();
    float ay = tri.a.y() - p.y();
    float bx = tri.b.x() - p.x();
    float by = tri.b.y() - p.y();
    float cx = tri.c.x() - p.x();
    float cy = tri.c.y() - p.y();

    float det = (ax * (bx * (cx * cx + cy * cy) - cy * (bx * bx + by * by)) -
                 ay * (bx * (cx * cx + cy * cy) - cx * (bx * bx + by * by)) +
                (ax * ax + ay * ay) * (bx * cy - by * cx));

    float orientation = (tri.b.x() - tri.a.x()) * (tri.c.y() - tri.a.y()) - (tri.b.y() - tri.a.y()) * (tri.c.x() - tri.a.x());


    if (orientation < 0)
    det = -det;

return det > 0;
    
    return det > 0;

}

std::vector<std::vector<QVector2D>> Basic::triangularization(std::vector<QVector2D> points) {
    std::vector<Triangle> triangulation; 

    float minX = points[0].x(), minY = points[0].y();;
    float maxX = points[0].x(), maxY = points[0].y();;
    for(QVector2D p : points) {
        if(p.x() < minX) {
            minX = p.x();
        }
        if(p.x() > maxX) {
            maxX = p.x();
        }
        if(p.y() < minY) {
            minY = p.y();
        }
        if(p.y() > maxY) {
            maxY = p.y();
        }
    }

    float dx = maxX - minX;
    float dy = maxY - minY;
    float deltaMax = std::max(dx, dy);
    float midX = (minX + maxX) / 2;
    float midY = (minY + maxY) / 2;

    QVector2D p1(midX - 20 * deltaMax, midY - deltaMax);
    QVector2D p2(midX, midY + 20 * deltaMax);
    QVector2D p3(midX + 20 * deltaMax, midY - deltaMax);

    triangulation.push_back({p1, p2, p3});

    for(auto point : points) {
        std::vector<Triangle> badTriangles;
        for(auto tri : triangulation) {
            if(pointInCircumcicle(point, tri)) {
                badTriangles.push_back(tri);
            }
        }

        std::vector<Edge> polygon;

        for(auto tri : badTriangles) {
            std::vector<Edge> edges = {
                {tri.a, tri.b},
                {tri.b, tri.c},
                {tri.c, tri.a}
            };
            
            for(auto e : edges) {
                auto it = std::find(polygon.begin(), polygon.end(), e);
                if(it != polygon.end()) {
                    polygon.erase(it);
                } else {
                    polygon.push_back(e);
                }
            }
        }

        triangulation.erase(std::remove_if(triangulation.begin(), triangulation.end(), 
                                [&](const Triangle t) {
                                    return std::find(badTriangles.begin(), badTriangles.end(), t) != badTriangles.end();
                                }),
                                triangulation.end()
                            );
        
        for(auto edge : polygon) {
            triangulation.push_back(Triangle(edge.p1, edge.p2, point));
        }
    }

    triangulation.erase(
        std::remove_if(triangulation.begin(), triangulation.end(), [&](const Triangle t) {
            return t.containsVertex(p1) || t.containsVertex(p2) || t.containsVertex(p3);
        }),
        triangulation.end()
    );


    std::vector<std::vector<QVector2D>> result;
    for(auto tri : triangulation) {
        result.push_back({tri.a, tri.b, tri.c});
    }

    return result;
}

QVector2D Basic::getCircumcenter(std::vector<QVector2D> t) {
    QVector2D A = t[0];
    QVector2D B = t[1];
    QVector2D C = t[2];

    float ax = A.x(), ay = A.y();
    float bx = B.x(), by = B.y();
    float cx = C.x(), cy = C.y();

    float d = 2 * (ax*(by - cy) + bx*(cy - ay) + cx*(ay - by));
    std::cout << "Teste" << std::endl;
    if (std::abs(d) < 1e-6) return QVector2D(0, 0); 

    float ax2ay2 = ax*ax + ay*ay;
    float bx2by2 = bx*bx + by*by;
    float cx2cy2 = cx*cx + cy*cy;

    float ux = (ax2ay2 * (by - cy) + bx2by2 * (cy - ay) + cx2cy2 * (ay - by)) / d;
    float uy = (ax2ay2 * (cx - bx) + bx2by2 * (ax - cx) + cx2cy2 * (bx - ax)) / d;

    return QVector2D(ux, uy);
}

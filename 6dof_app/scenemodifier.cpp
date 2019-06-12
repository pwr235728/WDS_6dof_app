
#include "scenemodifier.h"

#include <QtCore/QDebug>

SceneModifier::SceneModifier(Qt3DCore::QEntity *rootEntity)
    : m_rootEntity(rootEntity)
{



    // Cuboid shape data
    Qt3DExtras::QCuboidMesh *cuboid = new Qt3DExtras::QCuboidMesh();
    cuboid->setYExtent(0.2);
    cuboid->setZExtent(0.5);

    // CuboidMesh Transform
    cuboidTransform = new Qt3DCore::QTransform();
    cuboidTransform->setScale(7.0f);
    cuboidTransform->setTranslation(QVector3D(0.0f, -5.0f,-30.0f));

    Qt3DExtras::QPhongMaterial *cuboidMaterial = new Qt3DExtras::QPhongMaterial();
    cuboidMaterial->setDiffuse(QColor(QRgb(0x665423)));

    //Cuboid
    m_cuboidEntity = new Qt3DCore::QEntity(m_rootEntity);
    m_cuboidEntity->addComponent(cuboid);
    m_cuboidEntity->addComponent(cuboidMaterial);
    m_cuboidEntity->addComponent(cuboidTransform);
}

SceneModifier::~SceneModifier()
{
}

void SceneModifier::setRotation(QQuaternion rotation)
{
    auto rot = QQuaternion::fromAxisAndAngle(QVector3D(1,0,0), -90);
    auto rot2 = QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), -90);
    cuboidTransform->setRotation(rot2*(rot*rotation*rot.inverted())*rot2.inverted());
}

void SceneModifier::enableCuboid(bool enabled)
{
    m_cuboidEntity->setEnabled(enabled);
}


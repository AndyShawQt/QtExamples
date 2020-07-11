#include <Qt3DRender>
#include <Qt3DExtras>

// Key thing to remember with vertices is that it is like a graph, i.e. inverse of points so y
// goes up and not down.

int main(int argc, char **argv)
{
    QGuiApplication ga(argc, argv);
    Qt3DExtras::Qt3DWindow view;
    auto entity = new Qt3DCore::QEntity;
    auto camera = view.camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0, 0, 20.0f));
    camera->setUpVector(QVector3D(0, 1, 0));
    camera->setViewCenter(QVector3D(0, 0, 0));

    QList<QVector3D> vertices;
/*
    // Drawing lines to make 2 squares non connected with Lines but connected with LineStrip/LineLoop
    vertices.append(QVector3D(0, 0, 0));
    vertices.append(QVector3D(1, 0, 0));
    vertices.append(QVector3D(1, 0, 0));
    vertices.append(QVector3D(1, 1, 0));
    vertices.append(QVector3D(1, 1, 0));
    vertices.append(QVector3D(0, 1, 0));
    vertices.append(QVector3D(0, 1, 0));
    vertices.append(QVector3D(0, 0, 0));
    
    // Now a second square
    vertices.append(QVector3D(5, 5, 5));
    vertices.append(QVector3D(6, 5, 5));
    vertices.append(QVector3D(6, 5, 5));
    vertices.append(QVector3D(6, 6, 5));
    vertices.append(QVector3D(6, 6, 5));
    vertices.append(QVector3D(5, 6, 5));
    vertices.append(QVector3D(5, 6, 5));
    vertices.append(QVector3D(5, 5, 5));
    Qt3DRender::QGeometryRenderer::PrimitiveType primitiveType = Qt3DRender::QGeometryRenderer::Lines;
    int nCount = 2;
 */

    // Drawing triangles to make 2 squares
    vertices.append(QVector3D(0, 1, 0));
    vertices.append(QVector3D(0, 0, 0));
    vertices.append(QVector3D(1, 0, 0));
    vertices.append(QVector3D(1, 1, 0));
    vertices.append(QVector3D(0, 1, 0));
    vertices.append(QVector3D(1, 0, 0));

    // Now a second square
    vertices.append(QVector3D(5, 6, 5));
    vertices.append(QVector3D(5, 5, 5));
    vertices.append(QVector3D(6, 5, 5));
    vertices.append(QVector3D(6, 6, 5));
    vertices.append(QVector3D(5, 6, 5));
    vertices.append(QVector3D(6, 5, 5));
    Qt3DRender::QGeometryRenderer::PrimitiveType primitiveType = Qt3DRender::QGeometryRenderer::Triangles;
    int nCount = 3;

    auto customRenderer = new Qt3DRender::QGeometryRenderer(entity);
    auto customGeometry = new Qt3DRender::QGeometry(customRenderer);
    auto dataBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
    
    QByteArray vertexData;
    vertexData.resize(vertices.size() * nCount * sizeof(QVector3D));
    auto positionData = reinterpret_cast<QVector3D *>(vertexData.data());
    
    QList<QVector3D>::const_iterator it;
    for (it = vertices.begin(); it != vertices.end(); it++) {
        *positionData = *it;
        ++positionData;
    }
    dataBuffer->setData(vertexData);
    
    auto positionAttribute = new Qt3DRender::QAttribute;
    positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(dataBuffer);
    positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setByteOffset(0);
    positionAttribute->setByteStride((0 + 3) * sizeof(float));
    positionAttribute->setCount(vertices.size());
    
    customGeometry->addAttribute(positionAttribute);
    customRenderer->setGeometry(customGeometry);
    customRenderer->setPrimitiveType(primitiveType);

    auto material = new Qt3DExtras::QPhongMaterial(entity);
    material->setAmbient(Qt::red);
    
    auto linesEntity = new Qt3DCore::QEntity(entity);
    linesEntity->addComponent(material);
    linesEntity->addComponent(customRenderer);

    view.setRootEntity(entity);
    view.show();
    return ga.exec();
}
    
    

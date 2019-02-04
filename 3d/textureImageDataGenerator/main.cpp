#include <Qt3DRender>
#include <Qt3DExtras>

class MyTextureImageDataFunctor : public Qt3DRender::QTextureImageDataGenerator
{
    QT3D_FUNCTOR(MyTextureImageDataFunctor)
public:
    MyTextureImageDataFunctor(int id, const Qt3DRender::QTextureImageDataPtr &currentData) : Qt3DRender::QTextureImageDataGenerator(), dataId(id), data(currentData)
    {}
    Qt3DRender::QTextureImageDataPtr operator()() override
    {
        return data;
    }
    bool operator==(const QTextureImageDataGenerator &other) const override
    {
        const MyTextureImageDataFunctor *otherFunctor = functor_cast<MyTextureImageDataFunctor>(&other);
        return otherFunctor->dataId == dataId;
    }
private:
    int dataId;
    Qt3DRender::QTextureImageDataPtr data;
};

class MyTextureImage : public Qt3DRender::QAbstractTextureImage
{
public:
    MyTextureImage(Qt3DCore::QNode *parent = nullptr) : QAbstractTextureImage(parent)
    {
        currentData = Qt3DRender::QTextureImageDataPtr::create();
    }
    Qt3DRender::QTextureImageDataGeneratorPtr dataGenerator() const override
    {
        return generatorPtr;
    }
    // We update via an image to save time, but you could use raw data in place of this
    void updateData(const QImage &image)
    {
        QImage glImage = image.convertToFormat(QImage::Format_RGBA8888);
        QByteArray imageBytes((const char*) glImage.constBits(), glImage.sizeInBytes());
        currentData->setHeight(image.height());
        currentData->setWidth(image.width());
        currentData->setData(imageBytes, 4, false);
        currentData->setDepth(1);
        currentData->setFaces(1);
        currentData->setLayers(1);
        currentData->setMipLevels(1);
        currentData->setFormat(QOpenGLTexture::RGBA8_UNorm);
        currentData->setPixelFormat(QOpenGLTexture::RGBA);
        currentData->setPixelType(QOpenGLTexture::UInt8);
        currentData->setTarget(QOpenGLTexture::Target2D);
        
        generatorPtr = Qt3DRender::QTextureImageDataGeneratorPtr(new MyTextureImageDataFunctor(count, currentData));
        notifyDataGeneratorChanged();
        count++;
    }
private:
    Qt3DRender::QTextureImageDataPtr currentData;
    Qt3DRender::QTextureImageDataGeneratorPtr generatorPtr;
    int count = 0;
};

int main(int argc, char **argv)
{
    QGuiApplication ga(argc, argv);
    Qt3DExtras::Qt3DWindow view;
    Qt3DCore::QEntity *entity = new Qt3DCore::QEntity;
    Qt3DRender::QCamera *camera = view.camera();
    camera->lens()->setPerspectiveProjection(45.0, 16.0 / 9.0, 0.1, 1000.0);
    camera->setPosition(QVector3D(0.0, 0.0, 40.0));
    camera->setViewCenter(QVector3D(0.0, 0.0, 0.0));
    
    MyTextureImage *texture = new MyTextureImage;
    QImage i(500, 500, QImage::Format_ARGB32_Premultiplied);
    i.fill(Qt::red);
        
    texture->updateData(i);

    // Material
    Qt3DExtras::QTextureMaterial *material = new Qt3DExtras::QTextureMaterial(entity);
    Qt3DRender::QTexture2D *texture2D = new Qt3DRender::QTexture2D(material);
    texture2D->addTextureImage(texture);
    material->setTexture(texture2D);

    Qt3DCore::QEntity* m_sphereEntity = new Qt3DCore::QEntity(entity);
    Qt3DExtras::QSphereMesh* m_sphereMesh = new Qt3DExtras::QSphereMesh();
    m_sphereMesh->setRadius(12.);
    m_sphereEntity->addComponent(m_sphereMesh);
    m_sphereEntity->addComponent(material);
    
    view.setRootEntity(entity);
    view.show();
    QTimer::singleShot(5000, [texture]() {
        QImage i(500, 500, QImage::Format_ARGB32_Premultiplied);
        i.fill(Qt::blue);
        texture->updateData(i);
    });
    return ga.exec();
}

#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>

#include <QString>
#include <QStringList>
#include <QVector>
#include <QVector2D>
#include <QVector3D>

/**
 * @brief The OBJModel class
 *
 * Loads all data from a Wavefront .obj file
 * Currently only supports traingle meshes with
 * one normal/texture coordinate per vertex
 *
 */
class OBJModel
{
public:
    OBJModel(QString filepathModel, QVector<QString> filepathTexture);
    ~OBJModel();

    QString getFilepathModel() {return filepathModel;}
    QString getFilepathTexture(int i) {return filepathTextures.at(i);}

    void createVAO() {VAO.create();}
    void bindVAO() {VAO.bind();}
    void releaseVAO() {VAO.release();}

    void addTexture(QOpenGLTexture *texture) {textures.append(texture);}
    QOpenGLTexture* getTexture(int i) {return textures.at(i);}
    int getNumTextures() {return filepathTextures.size();}
    void setUsedTexture(int i) {useTexture = i;}
    int getUsedTexture() {return useTexture;}

    void setCoordinatesBuffer(QOpenGLBuffer *coordinatesBuffer) {this->coordinatesBuffer = coordinatesBuffer;}
    void setNormalsBuffer(QOpenGLBuffer *normalsBuffer) {this->normalsBuffer = normalsBuffer;}
    void addTextureBuffer(QOpenGLBuffer *textureBuffer) {textureBuffers.append(textureBuffer);}
    QOpenGLBuffer* getCoordinatesBuffer() {return coordinatesBuffer;}
    QOpenGLBuffer* getNormalsBuffer() {return normalsBuffer;}
    QOpenGLBuffer* getTextureBuffer(int i) {return textureBuffers.at(i);}

    QVector<QVector3D> getVertices() {return vertices;}
    QVector<QVector3D> getNormals() {return normals;}
    QVector<QVector2D> getTexcoords() {return texcoords;}

    int getVecticesSize() {return nVertices;}
    int getNormalsSize() {return nNormals;}
    int getTexcoordsSize() {return nTexcoords;}

private:
    // Filepaths
    QString filepathModel;
    QVector<QString> filepathTextures;

    // Vertex array object
    QOpenGLVertexArrayObject VAO;

    // Texture
    QVector<QOpenGLTexture*> textures;
    int useTexture;

    // Vertex buffer objects
    QOpenGLBuffer *coordinatesBuffer;
    QOpenGLBuffer *normalsBuffer;
    QVector<QOpenGLBuffer*> textureBuffers;

    // Used for glDrawArrays()
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<QVector2D> texcoords;

    // Used for glDrawElements()
    QVector<QVector3D> vertices_indexed;
    QVector<QVector3D> normals_indexed;
    QVector<QVector2D> texcoords_indexed;
    QVector<unsigned> indices;

    int nVertices;
    int nNormals;
    int nTexcoords;

    void initialize();
    void loadModel();
    void parseVertex(QStringList tokens);
    void parseNormal(QStringList tokens);
    void parseTexture(QStringList tokens);
    void parseFace(QStringList tokens);
    void alignData();
    void unpackIndices();
    void unitize();

    QVector<QVector3D> norm;
    QVector<QVector2D> tex;
    QVector<unsigned> normal_indices;
    QVector<unsigned> texcoord_indices;

};

#endif // OBJMODEL_H

#ifndef FRAGMENTPANE_H
#define FRAGMENTPANE_H

#include <QtWidgets>
#include <QGLWidget>
#include <QtOpenGL>
#include <QOpenGLFunctions>
#include <QOpenGLShader>

class FragmentPane : public QGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit FragmentPane(QWidget* parent = 0);

    void SetNormalMap(QImage texture);
    void SetDiffuse(QImage texture);

signals: 

public slots:
    void slotZoomFactorChange(int value);
    void update();

protected:
    void initializeGL();
    void paintGL();
    void mousePressEvent(QMouseEvent *event);

private:
    QVector<QVector3D> vertices;
    QVector<QVector2D> texCoords;
    QVector3D lightPos;
    QGLShaderProgram *program;
    float zoomFactor;
    bool hasDiffuse;
    bool hasNormal;
    GLuint diffuseTexture;
    GLuint normalTexture;
    float t;
    void createSprite(int width, int height);

};

#endif // FRAGMENTPANE_H

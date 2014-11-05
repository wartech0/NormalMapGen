#include "fragmentpane.h"

FragmentPane::FragmentPane(QWidget* parent) :
    QGLWidget(parent)
{
    program = 0;
    zoomFactor = 1.0f;
    hasDiffuse = false;
    t = 0.0f;
}


void FragmentPane::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glViewport(0, 0, 500.0f, 500.0f);
    program = new QGLShaderProgram(this);
    program->addShaderFromSourceFile(QGLShader::Vertex, ":/shaders/vertexShader.vert");
    program->addShaderFromSourceFile(QGLShader::Fragment, ":/shaders/fragShader.frag");
    program->bindAttributeLocation("vertex", 0);
    program->bindAttributeLocation("texCoord", 1);
    program->link();
    program->bind();

    //Create a plane
    vertices.append(QVector3D(-10.0f, 10.0f, 0));
    texCoords.append(QVector2D(0, 0));
    vertices.append(QVector3D(10.0f, 10.0f, 0));
    texCoords.append(QVector2D(1, 0));
    vertices.append(QVector3D(-10.0f, -10.0f, 0));
    texCoords.append(QVector2D(0, 1));

    vertices.append(QVector3D(10.0f, 10.0f, 0));
    texCoords.append(QVector2D(1, 0));
    vertices.append(QVector3D(10.0f, -10.0f, 0));
    texCoords.append(QVector2D(1, 1));
    vertices.append(QVector3D(-10.0f, -10.0f, 0));
    texCoords.append(QVector2D(0, 1));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    updateGL();
}

void FragmentPane::paintGL()
{
    qglClearColor(QColor(64, 64, 64));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 m;
    m.ortho(-256*(1.0f/zoomFactor), 256*(1.0f/zoomFactor), 256*(1.0f/zoomFactor), -256*(1.0f/zoomFactor), 0.1f, 100.0f);
    m.translate(0.0f, 0.0f, -1.0f);
    program->setUniformValue("matrix", m);
    program->setUniformValue("lightPos", lightPos);

    program->enableAttributeArray(0);
    program->enableAttributeArray(1);
    program->setAttributeArray(0, vertices.constData());
    program->setAttributeArray(1, texCoords.constData());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    program->setUniformValue("texture", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalTexture);
    program->setUniformValue("normal_texture", 1);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void FragmentPane::slotZoomFactorChange(int value)
{
    zoomFactor = (float)value;
    updateGL();
}

void FragmentPane::SetDiffuse(QImage texture)
{
    if(hasDiffuse)
    {
        deleteTexture(diffuseTexture);
    }

    diffuseTexture = bindTexture(texture);
    createSprite(texture.width(), texture.height());
}

void FragmentPane::SetNormalMap(QImage texture)
{
    if(hasNormal)
    {
        deleteTexture(normalTexture);
    }

    normalTexture = bindTexture(texture);
}

void FragmentPane::createSprite(int width, int height)
{
    float halfWidth = (float)width / 2.0f;
    float halfHeight = (float)height / 2.0f;
    vertices.clear();
    vertices.append(QVector3D(-halfWidth, halfHeight, 0));
    vertices.append(QVector3D(halfWidth, halfHeight, 0));
    vertices.append(QVector3D(-halfWidth, -halfHeight, 0));

    vertices.append(QVector3D(halfWidth, halfHeight, 0));
    vertices.append(QVector3D(halfWidth, -halfHeight, 0));
    vertices.append(QVector3D(-halfWidth, -halfHeight, 0));

    updateGL();
}

void FragmentPane::mousePressEvent(QMouseEvent *event)
{
    lightPos.setX(event->pos().x());
    lightPos.setY(500 - event->pos().y());
    updateGL();
}

void FragmentPane::update()
{
    lightPos.setY((sin(t)*100.0f)+250);
    lightPos.setX((cos(t)*100.0f)+250);
    t+=0.1f;
    updateGL();
}

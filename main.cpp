#include <GL/glut.h>

// Dimensões da janela
const int WIDTH = 800;
const int HEIGHT = 600;

// Cor das paredes
GLfloat wallColor[] = { 0.78f, 0.69f, 0.58f };

// Cor do chão
GLfloat floorColor[] = { 0.44f,0.5f,0.56f};

// Função para inicializar a cena
void init() {
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f); // Cor de fundo da janela
    glEnable(GL_DEPTH_TEST); // Habilitar teste de profundidade
    glEnable(GL_LIGHTING); // Habilita iluminação
    glEnable(GL_LIGHT0); // Habilita a luz 0

    // Define a posição da luz
    GLfloat lightPos[] = { -5.0f, 5.0f, 5.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

// Função para desenhar a cena
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa o buffer de cor e de profundidade
    glLoadIdentity(); // Carrega a matriz de identidade

    gluLookAt(-5.0f, 2.5f, 6.0f,    // Posição da câmera
              0.0f, 0.0f, -3.0f,    // Ponto para onde a câmera está olhando
              0.0f, 1.0f, 0.0f);   // Vetor de orientação da câmera

    // Define o material da esfera (ambiente, difuso, especular, emissivo)
    GLfloat matAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // Cor ambiente (sem efeito de luz ambiente)
    GLfloat matDiffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f }; // Cor difusa (cinza claro)
    GLfloat matSpecular[] = { 0.4f, 0.4f, 0.4f, 1.0f }; // Cor especular
    GLfloat matEmission[] = {  0.0f, 0.0f, 0.0f, 1.0f }; // Cor emissiva (sem emissão)
    GLfloat matShininess[] = { 30.0f }; // Brilho

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    // Desenha o chão listrado
    /*glPushMatrix();
    glBegin(GL_QUADS);
    for (float z = -2.5f; z <= 1.5f; z += 1.0f) {
        for (float x = -2.5f; x <= 2.5f; x += 1.0f) {
            if (((int)(x + z) % 2) == 0) {
                glColor3f(1.0f, 1.0f, 1.0f); // Cor branca
            } else {
                glColor3f(0.0f, 0.0f, 0.0f); // Cor preta
            }
            glVertex3f(x, -2.5f, z);
            glVertex3f(x + 1.0f, -2.5f, z);
            glVertex3f(x + 1.0f, -2.5f, z + 1.0f);
            glVertex3f(x, -2.5f, z + 1.0f);
        }
    }
    glEnd();
    glPopMatrix();
*/
     // Desenhar o chão preto
    glPushMatrix();
    glColor3fv(floorColor); // Define a cor do chão
    glBegin(GL_QUADS);
        glVertex3f(-5.5f, -2.5f, -11.5f); // Canto inferior esquerdo
        glVertex3f(8.5f, -2.5f, -11.5f);  // Canto inferior direito
        glVertex3f(8.5f, -2.5f, 1.5f);  // Canto superior direito
        glVertex3f(-5.5f, -2.5f, 1.5f); // Canto superior esquerdo
    glEnd();
    glPopMatrix();

    // Desenha a esfera
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 0.0f); // Posição da esfera
    glutSolidSphere(0.5, 150, 150); // Raio, número de divisões em meridianos, número de divisões em paralelos
    glPopMatrix();

    glutSwapBuffers(); // Troca os buffers
}

// Função para redimensionar a janela
void reshape(int w, int h) {
    glViewport(0, 0, w, h); // Define a viewport da janela
    glMatrixMode(GL_PROJECTION); // Define a matriz atual como matriz de projeção
    glLoadIdentity(); // Carrega a matriz de identidade
    gluPerspective(45.0, (double)w / (double)h, 1.0, 500.0); // Define a perspectiva da câmera
    glMatrixMode(GL_MODELVIEW); // Define a matriz atual como matriz de modelo e visualização
}

// Função para controlar a entrada do teclado
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // Verifica se a tecla pressionada foi ESC
        exit(0); // Sai do programa
    }
}

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv); // Inicializa o GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Define o modo de exibição do GLUT
    glutInitWindowSize(WIDTH, HEIGHT); // Define o tamanho da janela
    glutCreateWindow("Museu 3D"); // Cria a janela com o título "Museu 3D"

    init(); // Inicializa a cena
    glutDisplayFunc(display); // Define a função de exibição
    glutReshapeFunc(reshape); // Define a função de redimensionamento da janela
    glutKeyboardFunc(keyboard); // Define a função de controle do teclado

    glutMainLoop(); // Entra no loop principal do GLUT

    return 0; // Retorna 0 (programa encerrado com sucesso)
}

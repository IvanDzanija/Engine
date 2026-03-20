// Local Headers
#include "AppState.h"
#include "FPSManager.h"
#include "Global.h"
#include "Shader.h"

// System Headers
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

static int32 width = 500;
static int32 height = 500;
static constexpr std::string shaders_dir = "/shaders/";

// funkcija koja se poziva prilikom mijenjanja velicine prozora, moramo ju povezati
// pomocu glfwSetFramebufferSizeCallback
void framebuffer_size_callback(GLFWwindow *window, int Width, int Height) {
  width = Width;
  height = Height;

  glViewport(0, 0, width, height);
}

int main(int argc, char *argv[]) {
  eng::AppState appState(width, height);
  /*********************************************************************************************/
  // postavljanje OpenGL konteksta, dohvacanje dostupnih OpenGL naredbi
  GLFWwindow *window;
#if defined(__APPLE__)
  glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
#endif

  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

  window = glfwCreateWindow(width, height, "Zadatak X", nullptr, nullptr);
  // provjeri je li se uspio napraviti prozor
  if (window == nullptr) {
    fprintf(stderr, "Failed to Create OpenGL Context");
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);

  // is this useless?
  // gladLoadGL();
  // dohvati sve dostupne OpenGL funkcije
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD");
    exit(-1);
  }
  fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

  std::string dir = std::filesystem::current_path().string() + shaders_dir;
  eng::Shader shader(dir, "shader");

  glEnable(GL_DEPTH_TEST);  // ukljuci z spremnik (prikazuju se oni fragmenti koji su
                            // najblizi promatracu)
  glDepthFunc(GL_LESS);

  // glEnable(GL_CULL_FACE); //ukljuci uklanjanje straznjih poligona -- za ovaj primjer
  // je iskljuceno glCullFace(GL_BACK);

  glClearColor(0.15, 0.1, 0.1, 1);  // boja brisanja platna izmedu iscrtavanja dva
                                    // okvira

  glfwSwapInterval(0);  // ne cekaj nakon iscrtavanja (vsync)

  FPSManager FPSManagerObject(window, 60, 1.0, "Zadatak X");

  glfwSetFramebufferSizeCallback(
      window, framebuffer_size_callback);  // funkcija koja se poziva prilikom
                                           // mijenjanja velicine prozora

  /*********************************************************************************************/
  // indeksirani vrhovi i boje u odvojenim poljima. Konkretni podaci

  // svaki redak je jedna koordinata (x, y, z)
  // clang-format off
  float indeksiraniVrhovi[18] = {-1,   -1,   0,
                                  1,   -1,   0,
                                  0,    1,   0,
                                 -0.4, -0.1, 0,
                                  0.4, -0.1, 0,
                                  0, -0.9, 0};

  // svaki redak je jedna boja vezana uz vrh na istoj poziciji kao u prethodnom polju
  // (r, g, b)
  float indeksiraneBoje[18] = {1, 0, 0,
                               0, 1, 0,
                               0, 0, 1,
                               0, 1, 1,
                               1, 0, 1,
                                1, 1, 0};

  // svaki redak je jedan trokut
  unsigned int indeksi[3] = {0, 5, 3};
  // clang-format on

  // ucitavanje shadera i dohvat uniform varijable

  GLint lokacijaUniformVarijable = glGetUniformLocation(shader.ID, "tMatrica");

  /*********************************************************************************************/
  // prenosenje podataka i objasnjavanje u kojem formatu su ti podaci
  // generiranje buffera
  GLuint VAO;
  GLuint VBO[2];
  GLuint EBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(2, VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  // buffer za koordinate i povezi s nultim mjestom u shaderu -- layout (location = 0)
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(indeksiraniVrhovi), indeksiraniVrhovi,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  // buffer za boje i povezi s prvim mjestom u shaderu -- layout (location = 1)
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(indeksiraneBoje), indeksiraneBoje,
               GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  // buffer za indekse, moze biti samo jedan GL_ELEMENT_ARRAY_BUFFER po VAO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeksi), (void *)(&indeksi[0]),
               GL_STATIC_DRAW);
  glBindVertexArray(0);

  /*********************************************************************************************/
  //"instanciranje objekata" svaka matrica reprezentira novu instancu objekta. Izrada
  // polja transformacija koji  postavljaju objekte u mrezu 4x4

  glm::mat4 jedinicna = glm::mat4(1);
  glm::mat4 skaliranje = glm::scale(jedinicna, glm::vec3(0.25, 0.25, 0.25));

  glm::mat4 poljeTransformacija[32];

  glm::vec3 osRotacije = glm::vec3(1, 0, 0);
  float kutRotacije = 3.1415 / 32;

  int brojac = 0;
  for (float i = -1; i < 1; i += 0.5) {
    for (float j = -1; j < 1; j += 0.5) {
      poljeTransformacija[brojac] =
          glm::translate(jedinicna, glm::vec3(j + 0.25, i + 0.25, 0)) * skaliranje;

      poljeTransformacija[brojac + 16] =
          glm::translate(jedinicna, glm::vec3(j + 0.25, i + 0.25, 0)) *
          glm::rotate(jedinicna, brojac * kutRotacije, osRotacije) *
          glm::rotate(jedinicna, std::numbers::pi_v<float>, glm::vec3(0, 0, 1)) *
          skaliranje;
      brojac++;
    }
  }

  /*********************************************************************************************/
  // glavna petlja za prikaz
  while (glfwWindowShouldClose(window) == false) {
    float deltaTime = (float)FPSManagerObject.enforceFPS(false);

    /****************************/
    // osvjezavanje podataka
    poljeTransformacija[28] =
        poljeTransformacija[28] *
        glm::rotate(jedinicna, deltaTime * (float)3.141592 / 4, glm::vec3(0, 0, 1));

    // pobrisi platno
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /****************************/
    // iscrtavanje
    // za svaku instancu objekta saljemo naredbu za iscrtavanje. podaci o modelu ostaju
    // na grafickoj, mijenja se samo uniform varijabla.
    glUseProgram(shader.ID);

    glBindVertexArray(VAO);

    glUniformMatrix4fv(lokacijaUniformVarijable, 1, GL_FALSE, &jedinicna[0][0]);
    glDrawElements(GL_TRIANGLES, sizeof(indeksi) / sizeof(unsigned int),
                   GL_UNSIGNED_INT, 0);
    // for (int i = 0; i < 32; i++) {
    //   glUniformMatrix4fv(lokacijaUniformVarijable, 1, GL_FALSE,
    //                      &poljeTransformacija[i][0][0]);
    //   glDrawElements(GL_TRIANGLES, sizeof(indeksi) / sizeof(unsigned int),
    //                  GL_UNSIGNED_INT, 0);
    // }

    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
  }

  glDeleteBuffers(2, VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteVertexArrays(1, &VAO);

  glfwTerminate();

  return EXIT_SUCCESS;
}

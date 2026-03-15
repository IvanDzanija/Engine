// Local Headers

// System Headers
#include "Graphics.h"

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

static constexpr int WIDTH = 97;
static constexpr int HEIGHT = 97;

static std::vector<std::pair<int, int>> mouse_clicks;
void mouse_click(int x, int y, int button) {
  switch (button) {
    case 0:
      std::cout << "Left:";
      break;
    case 1:
      std::cout << "Right:";
      break;
    default:
      std::cout << "Unknown:";
  }
  mouse_clicks.emplace_back(x, y);
}

void iscrtajLiniju(eng::Graphics &grafika, int x0, int y0, int x1, int y1) {
  grafika.osvijetliFragment(x0, y0, glm::vec3(0, 0.8, 0));

  // odabir varijante bresenhama i algoritam
}

int main(int argc, char *argv[]) {
  eng::Graphics screen(WIDTH, HEIGHT, glm::vec3(0, 0, 0), argv[0]);

  eng::Graphics::registrirajFunkcijuZaKlikMisa(mouse_click);
  // grafika.registrirajFunkcijuZaKlikMisa(klikMisa); bad practice, static function
  // should be called on class, not instance

  while (eng::Graphics::trebaZatvoriti()) {
    // osvjezavanje pozicija razlicitih objekata i fizikalna simulacija bi se izvrsavala
    // u ovom dijelu

    screen.pobrisiProzor();

    // iscrtavanje uzorka sahovnice
    for (int i = 0; i < HEIGHT; i += 1) {
      for (int j = 0; j < WIDTH; j += 1) {
        if ((i + j) % 2 == 0) {
          screen.osvijetliFragment(i, j, glm::vec3(0.1, 0.1, 0.1));
        }
        if (i % 10 == 0 && j % 10 == 0) {
          screen.osvijetliFragment(i, j, glm::vec3(0.15, 0.15, 0.2));
        }
      }
    }

    // iscrtavanje pritisnutih fragmenata
    // ishodiste koordinatnog sustava za operacijski sustav je u gornjem lijevom kutu, a
    // za OpenGL je u donjem lijevom, pa je potrebno okrenuti predznak
    for (auto &klik : mouse_clicks) {
      screen.osvijetliFragment(klik.first, HEIGHT - klik.second - 1,
                               glm::vec3(0.6, 0.2, 0));
    }

    screen.iscrtajRaster();

    // kako program ne bi trosio previse resursa, na kraj petlje se moze dodati poziv
    // funkcije za cekanje kao npr Sleep ili od c++11 na dalje
    // this_thread::sleep_for(chrono::milliseconds(16));
  }

  return EXIT_SUCCESS;
}

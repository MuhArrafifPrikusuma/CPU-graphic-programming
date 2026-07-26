#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 800
#define HEIGTH 650

typedef struct {
  uint8_t r, g, b;
} Pixel;

Pixel framebuffer[WIDTH * HEIGTH];

void pixel_put(double x, double y, Pixel color);
void draw_line(double x0, double y0, double x1, double y1, Pixel color);
void save_file(const char *filename);
Pixel color_me();

int main(int argc, char *argv[]) {
  srand(time(NULL));
  Pixel background = {0, 0, 0};

  for (int i = 0; i < WIDTH * HEIGTH; i++) {
    framebuffer[i] = background;
  }
  int xs0 = 150, ys0 = 150;
  int xs1 = -150, ys1 = -150;
  int x0 = xs0 + (WIDTH / 2), x1 = xs1 + (WIDTH / 2);
  int y0 = ys0 + (HEIGTH / 2), y1 = ys1 + (HEIGTH / 2);

  draw_line(x0, y0, x1, y0, color_me());
  draw_line(x0, y0, x1, y1, color_me());
  draw_line(x1, y0, x0, y1, color_me());
  draw_line(x0, y1, x1, y1, color_me());

  save_file(argv[1]);

  return EXIT_SUCCESS;
}

void pixel_put(double x, double y, Pixel color) {
  uint64_t px = (int)roundf(x);
  uint64_t py = (int)roundf(y);
  if (px >= 0 && px <= WIDTH && py >= 0 && py <= HEIGTH) {
    framebuffer[py * WIDTH + px] = color;
  }
}

Pixel color_me() {
  Pixel color;
  color.r = rand() % 256;
  color.g = rand() % 256;
  color.b = rand() % 256;
  return color;
}

void draw_line(double x0, double y0, double x1, double y1, Pixel color) {
  double dx = x1 - x0;
  double dy = y1 - y0;

  double steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);
  if (steps == 0) {
    pixel_put(x0, y0, color);
    return;
  }

  for (int i = 0; i <= (int)steps; i++) {
    double t = i / steps;

    double x = x0 + t * dx;
    double y = y0 + t * dy;

    pixel_put(x, y, color);
  }
}

void save_file(const char *filename) {
  FILE *f = fopen(filename, "wb");

  fprintf(f, "P6\n%d %d\n255\n", WIDTH, HEIGTH);

  fwrite(framebuffer, sizeof(Pixel), WIDTH * HEIGTH, f);

  fclose(f);
}

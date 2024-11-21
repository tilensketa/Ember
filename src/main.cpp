#include "Application.h"

int main() {
  Application app(800, 600, false);
  app.Run();
  app.Close();
  return 0;
}

/*
  Requirements:
    Directory test with files transmissions{number}.txt or mcode{number}.txt
    Cpp std <= 20
  Authors:
    Nicolás Amaya
    María Espínola
    Fransico
*/
#include <filesystem>
#include <iostream>
#include <fstream>

using std::string, std::ifstream;
namespace fs = std::filesystem;
class Transmition
{
  string data = "";
  string name = "";

public:
  Transmition(const string &path)
  {
    ifstream file(path);
    string temp;
    while (getline(file, temp))
    {
      data += temp;
    }
    file.close();
    name = path.substr(path.find("test/"));
  }
};
int main()
{
  const string dir = "test";
  for (const auto &entry : fs::directory_iterator(dir))
    std::cout << entry.path() << std::endl;
}

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
#include <vector>
#include <regex>
#include "z_function.cpp"

using std::string, std::ifstream, std::vector, std::cout, std::endl, std::regex_match, std::regex, std::pair, std::setw, std::left;
namespace fs = std::filesystem;
string getStringFile(const string &path)
{
  ifstream file(path);
  string temp = "", data = "";
  while (getline(file, temp))
  {
    data += temp;
  }
  file.close();
  return data;
}
class Mcode
{
public:
  string name = "", content = "";
  Mcode(const string &name, const string &path) : name(name), content(getStringFile(path)) {};
};
class Transmition
{
  void print_found_comoun(const Transmition &transmision, string &prefix, size_t found_index) const
  {
    cout << "The longest substring found was ";
    cout << "`" << prefix << "`";
    cout << " found in ";
    cout << transmision.name << " ";
    cout << "in position " << found_index;
    cout << " to index " << found_index + prefix.size();
    cout << " of " << name;
    cout << endl;
  }

public:
  string name = "", data = "";
  Transmition(const string &name, const string &path) : name(name), data(getStringFile(path)) {}
  void check_mcode(const Mcode &mcode) const
  {
    auto [isFound, position] = z_funtion(data, mcode.content);
    cout << left << setw(9) << (isFound ? "Found" : "Not found") << " the ";
    cout << mcode.name << " in ";
    cout << name << " file ";
    if (position.has_value())
    {
      cout << "patter in possition " << position.value();
    }
    cout << endl;
  }
  void check_sufix(const Transmition &transmision) const
  {
    string pattern;
    size_t start_index;
    print_found_comoun(transmision, pattern, start_index + 1);
  }
};
int main()
{
  const string dir = "test";
  vector<Transmition> transmitions;
  vector<Mcode> mcodes;
  for (const auto &entry : fs::directory_iterator(dir))
  {
    const auto &path = entry.path();
    const string &filename = path.filename().string();
    if (regex_match(filename, regex("mcode\\d+\\.txt")))
    {
      mcodes.emplace_back(filename, path);
      continue;
    }
    // making sure is only the one with the correct prefix
    if (regex_match(filename, regex("transmission\\d+\\.txt")))
    {
      transmitions.emplace_back(filename, path);
      continue;
    }
  }
  for (const auto &transition : transmitions)
  {
    for (const auto &mcode : mcodes)
    {
      transition.check_mcode(mcode);
    }
  }
  transmitions[0].check_sufix(transmitions[1]);
  transmitions[1].check_sufix(transmitions[0]);
  return 0;
}

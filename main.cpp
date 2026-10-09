/*
  Requirements:
    Directory test with files transmissions{number}.txt or mcode{number}.txt
    Cpp std <= 20
  Authors:
    Nicolás Amaya
    María Espínola
    Fco Javier Becerra
*/
#include "manacher.cpp"
#include "z_function.cpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

using std::string, std::ifstream, std::vector, std::cout, std::endl,
    std::regex_match, std::regex, std::setw, std::left;
namespace fs = std::filesystem;

// Reed the file, add each file without the espaces and end lines
string getStringFile(const string &path) {
  ifstream file(path);
  string temp = "", data = "";
  while (getline(file, temp)) {
    data += temp;
  }
  file.close();
  return data;
}

// Search in transmissions and saves the name and the content
class Mcode {
public:
  string name = "", content = "";
  Mcode(const string &name, const string &path)
      : name(name), content(getStringFile(path)) {};
};

// Analize the transmission
class Transmission {
  void print_found_common(const Transmission &transmision, const string &prefix,
                          const size_t found_index) const {
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
  // Save the name and load the data
  string name = "", data = "";
  Transmission(const string &name, const string &path)
      : name(name), data(getStringFile(path)) {}

  // Search the pattern and the position only if it conteins a value
  void check_mcode(const Mcode &mcode) const {
    auto [isFound, position] = z_funtion(data, mcode.content);
    cout << left << setw(9) << (isFound ? "Found" : "Not found") << " the ";
    cout << mcode.name << " in ";
    cout << name << " file ";
    if (position.has_value()) {
      cout << "pattern in possition " << position.value();
    }
    cout << endl;
  }

  void check_palindrome() const {
    const auto [start, length] = manacher(data);
    cout << name << " longest palidrome: " << data.substr(start, length)
         << "\n";
    cout << "From:" << start + 1 << " to " << start + length << endl;
  }

  // Search the largest substring in the transmission
  void check_sufix(const Transmission &transmision) const {
    if (transmision.data == data) {
      print_found_common(transmision, data, 0);
      return;
    }
    int n = data.size() + 1;
    while (n > 0) {
      for (size_t l = 0; l + n <= data.size(); l++) {
        string pattern = data.substr(l, n);

        auto [isFound, pos] = z_funtion(transmision.data, pattern);
        if (isFound) {
          print_found_common(transmision, pattern, pos.value() + 1);
          return;
        }
      }
      n--;
    }
    cout << "There is not common substring bewtween files" << endl;
  }
};

int main() {
  vector<Transmission> transmissions;
  vector<Mcode> mcodes;
  // Reads directory ./test
  const string dir = "test";
  for (const auto &entry : fs::directory_iterator(dir)) {
    const auto &path = entry.path();
    const string &filename = path.filename().string();
    // finds exact mcode{num}.txt files
    if (regex_match(filename, regex("mcode\\d+\\.txt"))) {
      mcodes.emplace_back(filename, path);
      continue;
    }
    // making sure is only the one with the correct prefix
    if (regex_match(filename, regex("transmission\\d+\\.txt"))) {
      transmissions.emplace_back(filename, path);
      continue;
    }
  }
  cout << "PART 1" << endl;
  // Check mcodes group by transmissions
  for (const auto &transition : transmissions) {
    for (const auto &mcode : mcodes) {
      transition.check_mcode(mcode);
    }
  }
  cout << "\n\n"
       << "PART 2" << endl;
  for (const auto &transimision : transmissions) {
    transimision.check_palindrome();
    cout << endl;
  }
  cout << "\n\n"
       << "PART 3" << endl;
  // Compare the first two transmissions
  transmissions[0].check_sufix(transmissions[1]);
  transmissions[1].check_sufix(transmissions[0]);
  return 0;
}

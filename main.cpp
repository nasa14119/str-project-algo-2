/*
  Requirements:
    Directory test with files transmissions{number}.txt or mcode{number}.txt
    Cpp std <= 20
  Authors:
    Nicolás Amaya
    María Espínola
    Fransico
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include <stdexcept>
#include "z_function.cpp"

using std::string, std::ifstream, std::vector, std::cout,
      std::endl, std::pair, std::min;

//Integrate Manacher without its original main.
// Add separators to handle odd- and even-length palindromes.
string add_placeholder(const string &str)
{
  const char delimiter = '#';
  string result;

  for (size_t i = 0; i < str.size(); i++)
  {
    result += delimiter;
    result += str[i];
  }

  result += delimiter;
  return result;
}

// Return the zero-based start and length of the longest palindrome.
pair<size_t, size_t> mancher(const string &str)
{
  if (str.empty())
    return {0, 0};

  const string mirror = add_placeholder(str);
  const size_t n = mirror.size();
  vector<size_t> p_array(n, 0);

  size_t center = 0, right = 0;
  size_t max_length = 0, center_index = 0;

  for (size_t i = 1; i < n - 1; i++)
  {
    if (right > i)
    {
      // Reuse the radius of the mirrored position.
      size_t mirror_i = 2 * center - i;
      p_array[i] = min(right - i, p_array[mirror_i]);
    }

    // Expand while the characters on both sides match.
    while (i >= 1 + p_array[i] &&
           i + 1 + p_array[i] < n &&
           mirror[i + 1 + p_array[i]] ==
               mirror[i - 1 - p_array[i]])
    {
      p_array[i]++;
    }

    if (i + p_array[i] > right)
    {
      center = i;
      right = i + p_array[i];
    }

    if (p_array[i] > max_length)
    {
      max_length = p_array[i];
      center_index = i;
    }
  }

  const size_t start_index = (center_index - max_length) / 2;
  return {start_index, max_length};
}

// Read the character sequence without line breaks.
string getStringFile(const string &path)
{
  ifstream file(path);

  //Detect unreadable files.
  if (!file)
    throw std::runtime_error("Cannot open file: " + path);

  string temp = "", data = "";

  while (getline(file, temp))
  {
    //Remove Windows carriage returns too.
    if (!temp.empty() && temp.back() == '\r')
      temp.pop_back();

    data += temp;
  }

  file.close();
  return data;
}

// Store a malicious code file and its character sequence.
class Mcode
{
public:
  string name = "", content = "";

  Mcode(const string &name, const string &path)
      : name(name), content(getStringFile(path)) {}
};

// Store and analyze a transmission.
class Transmition
{
public:
  string name = "", data = "";

  Transmition(const string &name, const string &path)
      : name(name), data(getStringFile(path)) {}

  // Print only "true position" or "false".
  void check_mcode(const Mcode &mcode) const
  {
    auto [isFound, position] = z_funtion(data, mcode.content);

    if (isFound)
      cout << "true " << position.value() + 1 << endl;
    else
      cout << "false" << endl;
  }

  //Print one-based, inclusive palindrome positions.
  void check_palindrome() const
  {
    const auto [start, length] = mancher(data);
    cout << start + 1 << " " << start + length << endl;
  }

  //Report positions in THIS transmission.
  void check_sufix(const Transmition &transmision) const
  {
    size_t n = min(data.size(), transmision.data.size());

    // Try longer substrings first.
    // If there is a tie, select the earliest start.
    while (n > 0)
    {
      for (size_t l = 0; l + n <= data.size(); l++)
      {
        string pattern = data.substr(l, n);

        auto [isFound, pos] =
            z_funtion(transmision.data, pattern);

        if (isFound)
        {
          //l belongs to this file.
          // pos belongs to the other transmission.
          cout << l + 1 << " " << l + n << endl;
          return;
        }
      }

      n--;
    }

    // No valid interval exists without a common substring.
    cout << "0 0" << endl;
  }
};

int main()
{
  try
  {
    //Read the fixed filenames in the required order.
    // Files must be in the current working directory.
    vector<Transmition> transmitions = {
        Transmition("transmission1.txt", "transmission1.txt"),
        Transmition("transmission2.txt", "transmission2.txt")};

    vector<Mcode> mcodes = {
        Mcode("mcode1.txt", "mcode1.txt"),
        Mcode("mcode2.txt", "mcode2.txt"),
        Mcode("mcode3.txt", "mcode3.txt")};

    // Part 1: Print six results, grouped by transmission.
    for (const auto &transition : transmitions)
    {
      for (const auto &mcode : mcodes)
      {
        transition.check_mcode(mcode);
      }
    }

    // Part 2 calls Manacher for both transmissions.
    for (const auto &transition : transmitions)
    {
      transition.check_palindrome();
    }

    //Part 3 runs once and reports positions in file 1.
    transmitions[0].check_sufix(transmitions[1]);
  }
  catch (const std::exception &error)
  {
    // Keep errors separate from the required standard output.
    std::cerr << error.what() << endl;
    return 1;
  }

  return 0;
}
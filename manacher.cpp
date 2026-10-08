#include <iostream>

using std::cout, std::endl, std::string, std::vector, std::min;

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
};

std::pair<size_t, size_t> manacher(const string &str)
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
      size_t mirror_i = 2 * center - i;
      p_array[i] = min(right - i, p_array[mirror_i]);
    }
    while (i >= 1 + p_array[i] && i + 1 + p_array[i] < n &&
           mirror[i + 1 + p_array[i]] == mirror[i - 1 - p_array[i]])
      p_array[i]++;

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
};
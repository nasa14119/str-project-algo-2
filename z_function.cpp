#include <iostream>
#include <vector>

using std::string, std::vector;
std::pair<bool, std::optional<size_t>> z_funtion(const string &main, const string &pattern)
{
  const string iterator = pattern + "$" + main;
  vector<size_t> z_array(iterator.size(), 0);
  const size_t p_size = pattern.size(); // pattern size
  size_t rt = 0, lt = 0;
  for (size_t i = 1; i < iterator.size(); i++)
  {
    if (i > rt)
    {
      // Normal calculation
      size_t count = 0;
      while (count + i < iterator.size() && iterator[count] == iterator[count + i])
        count++;
      z_array[i] = count;
      if (count > 0)
      {
        lt = i;
        rt = i + count - 1;
      }
    }
    else
    {
      // inside the zbox
      size_t start = i - lt, len_z_box = rt - i + 1;
      if (z_array[start] < len_z_box)
      {
        // safe to not compute
        z_array[i] = z_array[start];
      }
      else
      {
        // z box with resize
        size_t count_resize = rt + 1; // starts checking in the end
        while (count_resize < iterator.size() && iterator[count_resize] == iterator[count_resize - i])
          count_resize++;
        z_array[i] = count_resize - i;
        lt = i;
        rt = count_resize - 1;
      }
    }
    if (z_array[i] == p_size && i > p_size)
    {
      // Guaranteed to be >= 0 without underflow
      size_t og_pos = i - p_size - 1;
      return {true, og_pos};
    }
  }
  return {false, std::nullopt};
}

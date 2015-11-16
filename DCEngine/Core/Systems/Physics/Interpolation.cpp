
namespace DCEngine
{
  template <typename T>
  T Interpolate(const T &start, const T &end, float ratio)
  {
    return start + (end - start) * ratio;
  }
}


namespace DCEngine
{
  float Interpolate(float start, float end, float ratio)
  {
    return start + (end - start) * ratio;
  }
  
  Vec3 Interpolate(Vec3 start, Vec3 end, float ratio)
  {
    return start + (end - start) * ratio;
  }
}

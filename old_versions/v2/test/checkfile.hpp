# include <string>
# include <sys/stat.h>

inline bool fileExists(const std::string& fileName)
{
  struct stat buffer;
  return (stat (fileName.c_str(), &buffer) == 0);
}



#include <realm_core/loguru.h>

#include <realm_io/utilities.h>

using namespace realm;

bool io::fileExists(const std::string &filepath)
{
  return boost::filesystem::exists(filepath);
}

bool io::dirExists(const std::string &directory)
{
  return boost::filesystem::exists(directory);
}

void io::createDir(const std::string &directory)
{
  if (io::dirExists(directory))
    return;
  boost::filesystem::path dir(directory);
  try
  {
    boost::filesystem::create_directory(dir);
  }
  catch (...)
  {
    LOG_F(WARNING, "Creating path failed: %s", directory.c_str());
  }
}

bool io::removeFileOrDirectory(const std::string &path)
{
  boost::system::error_code error;
  boost::filesystem::remove_all(path, error);

  if(error)
  {
    throw(std::runtime_error(error.message()));
  }
  return true;
}

std::string io::createFilename(const std::string &prefix, uint32_t frame_id, const std::string &suffix)
{
  char filename[1000];
  sprintf(filename, (prefix + "%06i" + suffix).c_str(), frame_id);
  return std::string(filename);
}

std::string io::getTempDirectoryPath()
{
  boost::system::error_code error;
  boost::filesystem::path path = boost::filesystem::temp_directory_path(error);

  if(error)
  {
    throw(std::runtime_error(error.message()));
  }
  return path.string();
}

std::string io::getDateTime()
{
  time_t     now = time(nullptr);
  tm  tstruct = *localtime(&now);
  char tim[20];
  strftime(tim, sizeof(tim), "%y-%m-%d_%H-%M-%S", &tstruct);
  return std::string(tim);
}

uint64_t io::getCurrentTimeNano()
{
  using namespace std::chrono;
  nanoseconds ms = duration_cast<nanoseconds>(system_clock::now().time_since_epoch());
  return static_cast<uint64_t>(ms.count());
}

uint32_t io::extractFrameIdFromFilepath(const std::string &filepath)
{
  std::vector<std::string> tokens_path = io::split(filepath.c_str(), '/');
  std::vector<std::string> tokens_name = io::split(tokens_path.back().c_str(), '.');
  std::string filename = tokens_name[0];
  return static_cast<uint32_t>(std::stoul(filename.substr(filename.size()-4,filename.size())));
}

std::vector<std::string> io::split(const char *str, char c)
{
  std::vector<std::string> result;

  do
  {
    const char *begin = str;

    while(*str != c && *str)
      str++;

    result.emplace_back(std::string(begin, str));
  }
  while (0 != *str++);

  return result;
}

std::vector<std::string> io::getFileList(const std::string& dir, const std::string &suffix)
{
  std::vector<std::string> filenames;
  if (!dir.empty())
  {
    boost::filesystem::path apk_path(dir);
    boost::filesystem::recursive_directory_iterator end;

    for (boost::filesystem::recursive_directory_iterator it(apk_path); it != end; ++it)
    {
      const boost::filesystem::path cp = (*it);

      const std::string &filepath = cp.string();
      if (suffix.empty() || filepath.substr(filepath.size() - suffix.size(), filepath.size()) == suffix)
        filenames.push_back(cp.string());
    }
  }
  std::sort(filenames.begin(), filenames.end());
  return filenames;
}
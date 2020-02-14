// File   reloader_parser.h
// Author lidongming
// Date   2018-07-26 12:52:03
// Brief

#ifndef RELOADER_RELOADER_PARSER_H_
#define RELOADER_RELOADER_PARSER_H_

#include <assert.h>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include "commonlib/include/string_utils.h"

namespace feature_engine {
namespace reloader {

enum ParserStatus {
    PARSER_OK = 0,
    PARSER_INVALID_LINE,
    PARSER_INVALID_KEY,
    PARSER_INVALID_VALUE,
    PARSER_ERROR = 256,
};

// struct ParserFirstColumnAsKey {  };

template<class K>
class SimpleParser {
 public:
  ParserStatus Parse(std::string& line, K& k) {
      return PARSER_ERROR;
  }
};

template<>
class SimpleParser<std::string> {
 public:
  ParserStatus Parse(std::string& line, std::string& k) {
      if (line.empty()) { return PARSER_INVALID_LINE; }
      // Process Key
      k = line;
      return PARSER_OK;
  }
};

template<class K, class V>
class CommonParser {
 public:
  ParserStatus Parse(std::vector<std::string>& line_vec, K& k, V& v) {
      return PARSER_ERROR;
  }
};

template<>
class CommonParser<int, int> {
 public:
  ParserStatus Parse(std::vector<std::string>& line_vec, int& k, int& v) {
      if (line_vec.size() < 2) { return PARSER_INVALID_LINE; }

      // Process Key
      try {
          k = stoi(line_vec[0]);
          v = stoi(line_vec[1]);
      } catch(std::exception& ex) {
          return PARSER_INVALID_LINE;
      }
      return PARSER_OK;
  }
};


template<>
class CommonParser<int64_t, int> {
 public:
  ParserStatus Parse(std::vector<std::string>& line_vec, int64_t& k, int& v) {
      if (line_vec.size() < 2) { return PARSER_INVALID_LINE; }

      // Process Key
      try {
          k = stoll(line_vec[0]);
          v = stoi(line_vec[1]);
      } catch(std::exception& ex) {
          return PARSER_INVALID_LINE;
      }
      return PARSER_OK;
  }
};

template<>
class CommonParser<int, std::string> {
 public:
  ParserStatus Parse(std::vector<std::string>& line_vec,
                     int& k, std::string& v) {
      if (line_vec.size() < 2) {
        return PARSER_INVALID_LINE;
      }

      // Process Key
      try {
          k = stoi(line_vec[0]);
      } catch(std::exception& ex) {
          return PARSER_INVALID_LINE;
      }
      v = line_vec[1];
      return PARSER_OK;
  }
};

template<>
class CommonParser<int64_t, std::string> {
 public:
  ParserStatus Parse(std::vector<std::string>& line_vec,
                     int64_t& k, std::string& v) {
      if (line_vec.size() < 2) { return PARSER_INVALID_LINE; }

      // Process Key
      try {
          k = stoll(line_vec[0]);
      } catch(std::exception& ex) {
          return PARSER_INVALID_LINE;
      }
      v = line_vec[1];

      return PARSER_OK;
  }
};

template<>
class CommonParser<std::string, std::string> {
 public:
  ParserStatus Parse(std::vector<std::string>& line_vec,
                     std::string& k, std::string& v) {
      if (line_vec.size() < 2) { return PARSER_INVALID_LINE; }

      // Process Key
      k = line_vec[0];
      v = line_vec[1];

      return PARSER_OK;
  }
};

template<typename T>
bool ConvertStr(const std::string& val, T& t) {
  assert(true == false);
  return true;
}

template<>
inline bool ConvertStr(const std::string& val, std::string& t) {
  t = val;
  return true;
}

template<>
inline bool ConvertStr(const std::string& val, int& t) {
  try {
    t = stoi(val);
  } catch (std::exception& ex) {
    return false;
  }

  return true;
}
template<>
inline bool ConvertStr(const std::string& val, long int& t) {
  try {
    t = stoll(val);
  } catch (std::exception& ex) {
    return false;
  }

  return true;
}

template<>
inline bool ConvertStr(const std::string& val, double& t) {
  try {
    t = stod(val);
  } catch (std::exception& ex) {
    return false;
  }

  return true;
}

template<class K, class V>
class MultiValueParser {
 public:
  MultiValueParser() : _sep(",") { }
  explicit MultiValueParser(std::string sep) : _sep(sep) { }

  ParserStatus Parse(std::vector<std::string>& line_vec, K& k,
                     std::vector<V>& v);

 private:
  std::string _sep;
};

template<class K, class V>
ParserStatus MultiValueParser<K, V>::Parse(
    std::vector<std::string>& line_vec, K& k,
    std::vector<V>& v) {
  if (line_vec.size() < 2) {
    return PARSER_INVALID_LINE;
  }

  if (!ConvertStr<K>(line_vec[0], k)) {
    return PARSER_INVALID_LINE;
  }

  V val;
  std::vector<std::string> val_vec;
  commonlib::StringUtils::Split(line_vec[1], _sep, val_vec);
  for (auto& elem : val_vec) {
    if (!ConvertStr<V>(elem, val)) {
      continue;
    }
    v.push_back(val);
  }

  return PARSER_OK;
}

}  // namespace reloader
}  // namespace feature_engine

#endif  // RELOADER_RELOADER_PARSER_H_

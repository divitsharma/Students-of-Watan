#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>


struct WatanException {
  virtual std::string what() const = 0;
};


struct NoResources : public WatanException {
  std::string what() const override {
    return "You do not have enough resources.";
  }
};

struct InvalidBuild : public WatanException {
  std::string what() const override {
    return "You cannot build here.";
  }
};

struct InvalidInput : public WatanException {
  std::string what() const override {
    return "Invalid input.";
  }
};

#endif

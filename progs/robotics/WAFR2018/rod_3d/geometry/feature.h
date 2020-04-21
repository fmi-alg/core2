/*
 * feature.h
 *
 * Created on: June 30, 2018
 * Author: Ching-Hsiang Hsu (chhsu@nyu.edu)
 *
 * Copyright (c) 2015-2018 Ching-Hsiang Hsu
 */

#include <string>

#ifndef FEATURE_H
#define FEATURE_H

enum FEATURE_TYPE { TYPE_CORNER = 0, TYPE_EDGE, TYPE_WALL };

class Feature {
 private:
  std::string name_;
  int id_, type_ = -1;

 public:
  Feature(int type) : type_(type) {}

  void setName(std::string name) { this->name_ = name; }
  std::string name() { return this->name_; }

  void setId(int id) { this->id_ = id; }
  int id() { return this->id_; }

  int type() { return this->type_; }
};

#endif  // FEATURE_H

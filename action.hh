#pragma once

#include "G4VUserActionInitialization.hh"

#include "generator.hh"

class MyRunAction;

class MyActionInitialization : public G4VUserActionInitialization {
public:
  MyActionInitialization();
  ~MyActionInitialization();

  virtual void Build() const;
};

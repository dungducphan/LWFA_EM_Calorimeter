#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction() {
}

MyDetectorConstruction::~MyDetectorConstruction() {
}

G4VPhysicalVolume* MyDetectorConstruction::Construct() {
  G4NistManager* nist = G4NistManager::Instance();

  G4Material* worldMat = nist->FindOrBuildMaterial("G4_Galactic");

  G4Box* solidWorld = new G4Box("solidWorld", 3*m, 3*m, 3*m);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicWorld, "physWorld", 0, false, 0, true);
  
  G4double calorimeterXY = 20 * cm;
  G4double absorberHalfThickness = 1./2. * cm;
  G4double scintillatorHalfThickness = 3./2 * cm;
  G4double layerThickness = 2 * (absorberHalfThickness + scintillatorHalfThickness);

  G4Material* absorberMat = nist->FindOrBuildMaterial("G4_Fe");
  G4Box* solidAbsorber = new G4Box("solidAbsorber", calorimeterXY, calorimeterXY, absorberHalfThickness);
  G4LogicalVolume* logicAbsorber = new G4LogicalVolume(solidAbsorber, absorberMat, "logicAbsorber");
  for (int i = 0; i < 20; ++i) {
    new G4PVPlacement(0, G4ThreeVector(0, 0, -1. * i * layerThickness), logicAbsorber, Form("physAbsorber_%02i", i), logicWorld, false, i);
  }

  return physWorld;
}

void MyDetectorConstruction::ConstructSDandField() {
  // const G4String name = "NeutronSD";
  // NeutronSD* sd = new NeutronSD(name);
  // G4SDManager::GetSDMpointer()->AddNewDetector(sd);
  // SetSensitiveDetector("logicSD", sd);
}

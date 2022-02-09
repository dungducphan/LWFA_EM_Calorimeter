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
  
  G4double calorimeterHalfXY = 20 * cm;
  G4double absorberHalfThickness = 0.5/2. * cm;
  G4double scintillatorHalfThickness = 2./2 * cm;
  G4double layerThickness = 2 * (absorberHalfThickness + scintillatorHalfThickness);
  G4int numLayers = 20;

  G4Material* absorberMat = nist->FindOrBuildMaterial("G4_Al");
  G4Box* solidAbsorber = new G4Box("solidAbsorber", calorimeterHalfXY, calorimeterHalfXY, absorberHalfThickness);
  G4LogicalVolume* logicAbsorber = new G4LogicalVolume(solidAbsorber, absorberMat, "logicAbsorber");
  for (int i = 0; i < numLayers; ++i) {
    new G4PVPlacement(0, G4ThreeVector(0, 0,- 2 * scintillatorHalfThickness - absorberHalfThickness - i * layerThickness), logicAbsorber, Form("physAbsorber_%02i", i), logicWorld, false, i);
  }

  G4Material* scintillatorMat = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");
  G4Box* solidScintillator = new G4Box("solidScintillator", calorimeterHalfXY, calorimeterHalfXY, scintillatorHalfThickness);
  G4LogicalVolume* logicScintillator = new G4LogicalVolume(solidScintillator, scintillatorMat, "logicScintillator");
  for (int i = 0; i < numLayers; ++i) {
      new G4PVPlacement(0, G4ThreeVector(0, 0, - scintillatorHalfThickness - i * layerThickness), logicScintillator, Form("physScintillator_%02i", i), logicWorld, false, i);
  }

  G4double photonEnergy[4] = {0.001*eV, 1*eV, 10*eV, 100*eV};
  G4double refractiveIndexScintillator[4] = {1.85, 1.85, 1.85, 1.85};

  FillScintillatorFromFile("NaI_emission.csv");
  G4MaterialPropertiesTable* MPTScintillator = new G4MaterialPropertiesTable();
  MPTScintillator->AddProperty("RINDEX", photonEnergy, refractiveIndexScintillator, 4);
  MPTScintillator->AddProperty("SCINTILLATIONCOMPONENT1", fPhotonEnergyScintillator, fScintillatingAmplitudeScintillator);
  MPTScintillator->AddConstProperty("SCINTILLATIONYIELD", 550. / MeV); // FIXME: Fake for fast check. Use 55/keV for production.
  MPTScintillator->AddConstProperty("RESOLUTIONSCALE", 1.0);
  MPTScintillator->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 2.1 * ns);
  MPTScintillator->AddConstProperty("SCINTILLATIONRISETIME1", 0.9 * ns);
  MPTScintillator->AddConstProperty("SCINTILLATIONYIELD1", 1.0);
  scintillatorMat->SetMaterialPropertiesTable(MPTScintillator);

  return physWorld;
}


void MyDetectorConstruction::FillScintillatorFromFile(std::string spectrumFile) {
    auto DFScintillatorEmissionSpectrum = ROOT::RDF::MakeCsvDataFrame(spectrumFile.c_str());
    auto gr = DFScintillatorEmissionSpectrum.Graph("Wavelength_nm", "RelYieldAmp");
    for (int i = 0; i < gr->GetN(); ++i) {
        fPhotonEnergyScintillator.push_back(1239.84193 * eV / gr->GetPointX(i));
        fScintillatingAmplitudeScintillator.push_back(gr->GetPointY(i));
    }
}
void MyDetectorConstruction::ConstructSDandField() {
  // const G4String name = "NeutronSD";
  // NeutronSD* sd = new NeutronSD(name);
  // G4SDManager::GetSDMpointer()->AddNewDetector(sd);
  // SetSensitiveDetector("logicSD", sd);
}

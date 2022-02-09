#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator() {
  fGeneralParticleSource = new G4GeneralParticleSource();

  G4SingleParticleSource* electronSource = fGeneralParticleSource->GetCurrentSource();
  G4ParticleDefinition* electron = G4ParticleTable::GetParticleTable()->FindParticle("e-");

  electronSource->SetParticleDefinition(electron);
  electronSource->SetNumberOfParticles(1);

  electronSource->GetPosDist()->SetPosDisType("Point"); // Point, Beam, Plane, Surface, Volume
  electronSource->GetPosDist()->SetCentreCoords(G4ThreeVector(0., 0., 2.8 * m));

  electronSource->GetAngDist()->SetAngDistType("iso"); // Isotropic, Cosine-law, Beam, User-defined
  electronSource->GetAngDist()->SetMinTheta(0.);
  electronSource->GetAngDist()->SetMaxTheta(0. + 0.01);
  electronSource->GetAngDist()->SetMinPhi(0);
  electronSource->GetAngDist()->SetMaxPhi(TMath::Pi() * 2.);

  electronSource->GetEneDist()->SetEnergyDisType("Mono"); // Mono, Lin, Pow, Exp, Gaus, Brem, BBody, Cdg (cosmic diffuse gamma), User, Arb, Epn (energy per nucleon)
  electronSource->GetEneDist()->SetMonoEnergy(1 * GeV);
}

MyPrimaryGenerator::~MyPrimaryGenerator() { delete fGeneralParticleSource; }

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
  fGeneralParticleSource->GeneratePrimaryVertex(anEvent);
}
